// Copyright (c) 2014-2024  Made to Order Software Corp.  All Rights Reserved
//
// https://snapwebsites.org/project/snapdev
// contact@m2osw.com
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// self
//
#include    <snapdev/matrix.h>
#include    <snapdev/not_reached.h>


// C++ lib
//
#include    <deque>
#include    <fstream>


// last include
//
#include    <snapdev/poison.h>




class matrix_expr
{
public:
                    matrix_expr(std::istream & in);

    void            run();

private:
    typedef snapdev::matrix<double>        matrix_t;

    enum class token_t
    {
        TOK_EOF,
        TOK_COMMAND,
        TOK_NUMBER
    };

    enum class type_t
    {
        TYPE_NUMBER,
        TYPE_MATRIX
    };

    struct data_t
    {
        data_t(double number = 0.0)
            : f_number(number)
        {
        }

        data_t(std::size_t rows, std::size_t columns)
            : f_type(type_t::TYPE_MATRIX)
            , f_matrix(rows, columns)
        {
        }

        data_t(matrix_t m)
            : f_type(type_t::TYPE_MATRIX)
            , f_matrix(m)
        {
        }

        type_t                  f_type = type_t::TYPE_NUMBER;
        double                  f_number = 0;   // simple number
        matrix_t                f_matrix = matrix_t();
    };

    typedef std::deque<data_t>  stack_t;

    static constexpr int    type_mix(type_t const l, type_t const r);
    void                    ungetc(int c);
    int                     getc();
    token_t                 get_any_token();
    token_t                 get_token();
    void                    interpret_tokens();
    void                    execute_command();
    void                    print(data_t const & value);
    void                    command_show_stack();
    void                    command_print();
    void                    command_dup();
    void                    command_swap();
    void                    command_matrix();
    void                    command_identity();
    void                    command_add();
    void                    command_subtract();
    void                    command_multiply();
    void                    command_divide();
    void                    command_negate();
    void                    command_inverse();
    void                    command_deg2rad();
    void                    command_rotatex();
    void                    command_rotatey();
    void                    command_rotatez();
    void                    verify_stack(std::size_t required_size);

    std::istream &          f_in;
    int                     f_line = 1;
    int                     f_last = '\0';
    int                     f_last_line = 0;
    int                     f_unget_pos = 0;
    int                     f_unget[8];
    token_t                 f_token = token_t::TOK_EOF;
    std::string             f_command = std::string();
    double                  f_number = 0.0;
    stack_t                 f_stack = stack_t();
};



matrix_expr::matrix_expr(std::istream & in)
    : f_in(in)
{
}


constexpr int matrix_expr::type_mix(type_t const l, type_t const r)
{
    return static_cast<int>(l) + static_cast<int>(r) * 256;
}


// when the parser needs a lookahead, we can unget the extra characters
//
void matrix_expr::ungetc(int c)
{
    if(static_cast<std::size_t>(f_unget_pos) >= sizeof(f_unget))
    {
        std::logic_error("too many ungetc() calls, maybe you need to increase the buffer size.");
    }
    f_unget[f_unget_pos++] = c;
}


// get the next character
//
int matrix_expr::getc()
{
    if(f_unget_pos > 0)
    {
        f_last = f_unget[--f_unget_pos];
        return f_last;
    }

    int c(f_in.get());
    if(!f_in)
    {
        f_last = EOF;
        return f_last;
    }

    if(c == '\r')
    {
        ++f_line;
        c = f_in.get();
        if(c != '\n')
        {
            ungetc(c);
            c = '\n';   // change '\r' in '\n'
        }
    }
    else if(c == '\n')
    {
        ++f_line;
    }

    f_last = c;
    return f_last;
}


// get next token except blanks
//
matrix_expr::token_t matrix_expr::get_any_token()
{
    for(;;)
    {
        f_last_line = f_line;
        f_command.clear();
        for(;;)
        {
            int c(getc());
            if(c == EOF)
            {
                if(f_command.empty())
                {
                    return token_t::TOK_EOF;
                }
                break;
            }

            if(isspace(c) || c == '\n')
            {
                break;
            }

            f_command += c;
        }

        if(!f_command.empty())
        {
            char * end(nullptr);
            f_number = strtod(f_command.c_str(), &end);
            if(end == nullptr
            || *end != '\0')
            {
                return token_t::TOK_COMMAND;
            }
            return token_t::TOK_NUMBER;
        }
    }
}


// get the next token except comments
//
matrix_expr::token_t matrix_expr::get_token()
{
    for(;;)
    {
        token_t t(get_any_token());
        if(t == token_t::TOK_COMMAND)
        {
            if(f_command == "\\")
            {
                // inline comment ("\ this is a comment\n")
                //
                if(f_last != '\n')
                {
                    for(;;)
                    {
                        int const c(getc());
                        if(c == EOF || c == '\n')
                        {
                            break;
                        }
                    }
                }
            }
            else if(f_command == "(")
            {
                // multiline comment ("( this is a comment )")
                //
                t = get_any_token();
                for(;;)
                {
                    int const c(getc());
                    if(c == EOF || c == ')')
                    {
                        break;
                    }
                }
            }
            else
            {
                return t;
            }
        }
        else
        {
            return t;
        }
    }
    snapdev::NOT_REACHED();
}


void matrix_expr::run()
{
    try
    {
        interpret_tokens();
    }
    catch(std::runtime_error const & e)
    {
        std::cerr << "error: " << e.what() << std::endl;
    }
    catch(...)
    {
        throw;
    }
}


void matrix_expr::interpret_tokens()
{
    for(;;)
    {
        token_t t(get_token());
        switch(t)
        {
        case token_t::TOK_EOF:
            // done
            return;

        case token_t::TOK_NUMBER:
            {
                data_t const number(f_number);
                f_stack.push_back(number);
            }
            break;

        case token_t::TOK_COMMAND:
            execute_command();
            break;

        default:
            std::cerr << "error: unknown token type." << std::endl;
            break;

        }
    }
}


void matrix_expr::execute_command()
{
    if(f_command == ".s")
    {
        command_show_stack();
        return;
    }

    if(f_command == ".")
    {
        command_print();
        return;
    }

    if(f_command == "dup")
    {
        command_dup();
        return;
    }

    if(f_command == "swap")
    {
        command_swap();
        return;
    }

    if(f_command == "matrix")
    {
        command_matrix();
        return;
    }

    if(f_command == "identity")
    {
        command_identity();
        return;
    }

    if(f_command == "+")
    {
        command_add();
        return;
    }

    if(f_command == "-")
    {
        command_subtract();
        return;
    }

    if(f_command == "*")
    {
        command_multiply();
        return;
    }

    if(f_command == "/")
    {
        command_divide();
        return;
    }

    if(f_command == "negate")
    {
        command_negate();
        return;
    }

    if(f_command == "inverse")
    {
        command_inverse();
        return;
    }

    if(f_command == "deg2rad")
    {
        command_deg2rad();
        return;
    }

    if(f_command == "rotatex")
    {
        command_rotatex();
        return;
    }

    if(f_command == "rotatey")
    {
        command_rotatey();
        return;
    }

    if(f_command == "rotatez")
    {
        command_rotatez();
        return;
    }

    std::cerr << "error: unknown command \"" << f_command << "\"." << std::endl;
}


void matrix_expr::print(data_t const & value)
{
    switch(value.f_type)
    {
    case type_t::TYPE_NUMBER:
        std::cout << value.f_number;
        break;

    case type_t::TYPE_MATRIX:
        // TODO: look into a way to make the matrix look like a forth stack
        //       (because otherwise it's not in/out compatible)
        //
        std::cout << value.f_matrix;
        break;

    default:
        std::cerr << "error: unknown stack item type." << std::endl;
        break;

    }
}


void matrix_expr::command_show_stack()
{
    if(f_stack.empty())
    {
        std::cout << "*empty*" << std::endl;
        return;
    }

    char const * space = "";
    for(auto it(f_stack.begin()); it != f_stack.end(); ++it)
    {
        std::cout << space;
        switch(it->f_type)
        {
        case type_t::TYPE_NUMBER:
            std::cout << it->f_number;
            space = " ";
            break;

        case type_t::TYPE_MATRIX:
            // TODO: look into a way to make the matrix look like a forth stack
            //       (because otherwise it's not in/out compatible)
            //
            std::cout << it->f_matrix;
            space = "";
            break;

        default:
            std::cerr << "error: unknown stack item type." << std::endl;
            break;

        }
    }
    if(*space != '\0')
    {
        std::cout << std::endl;
    }
}


void matrix_expr::command_print()
{
    if(f_stack.empty())
    {
        std::cout << "*empty*" << std::endl;
        return;
    }

    print(f_stack.back());

    f_stack.pop_back();
}


void matrix_expr::command_dup()
{
    verify_stack(1);

    f_stack.push_back(f_stack.back());
}


void matrix_expr::command_swap()
{
    verify_stack(2);

    // TODO: look in a way to do this without copies
    //
    data_t r(f_stack.back());
    f_stack.pop_back();
    data_t l(f_stack.back());
    f_stack.pop_back();

    f_stack.push_back(r);
    f_stack.push_back(l);
}


void matrix_expr::command_matrix()
{
    // transform the stack in a matrix
    //    # of rows
    //    # of columns
    //    row 1 (one row is n columns)
    //    row 2
    //    ...
    //    row n
    //
    verify_stack(2);

    data_t rows(f_stack.back());
    f_stack.pop_back();
    data_t columns(f_stack.back());
    f_stack.pop_back();

    if(rows.f_type != type_t::TYPE_NUMBER
    || columns.f_type != type_t::TYPE_NUMBER)
    {
        std::cerr << "error: rows x columns of a matrix must be NUMBERs." << std::endl;
        return;
    }

    std::size_t r(rows.f_number);
    std::size_t c(columns.f_number);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
    if(static_cast<double>(r) != rows.f_number
    || static_cast<double>(c) != columns.f_number)
    {
        std::cerr << "error: rows x columns of a matrix must be exact integers." << std::endl;
        return;
    }
#pragma GCC diagnostic pop

    std::size_t const size(r * c);
    verify_stack(size);

int count(0);
    data_t m(r, c);
    for(std::size_t j(r); j > 0; )
    {
        --j;
        for(std::size_t i(c); i > 0; )
        {
            --i;

            data_t const & n(f_stack.back());

            if(n.f_type != type_t::TYPE_NUMBER)
            {
                std::cerr << "error: all matrix elements must be NUMBERs." << std::endl;
                return;
            }

            m.f_matrix[j][i] = n.f_number;

            f_stack.pop_back(); // WARNING: this destroys the reference to `n`; don't use `n` after this line
++count;
        }
    }

    f_stack.push_back(m);
}


void matrix_expr::command_identity()
{
    // transform the stack in a matrix
    //    # of rows & columns (i.e. identity requires a square matrix)
    //
    verify_stack(1);

    data_t n(f_stack.back());
    f_stack.pop_back();

    if(n.f_type != type_t::TYPE_NUMBER)
    {
        std::cerr << "error: rows x columns of an identity matrix must be a NUMBER." << std::endl;
        return;
    }

    std::size_t count(n.f_number);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
    if(static_cast<double>(count) != n.f_number)
    {
        std::cerr << "error: rows x columns of an identity matrix must be an exact integer." << std::endl;
        return;
    }
#pragma GCC diagnostic pop

    data_t m(count, count);
    f_stack.push_back(m);
}


void matrix_expr::command_add()
{
    verify_stack(2);

    data_t r(f_stack.back());
    f_stack.pop_back();
    data_t & l(f_stack.back());

    switch(type_mix(l.f_type, r.f_type))
    {
    case type_mix(type_t::TYPE_NUMBER, type_t::TYPE_NUMBER):
        l.f_number += r.f_number;
        break;

    case type_mix(type_t::TYPE_NUMBER, type_t::TYPE_MATRIX):
        std::swap(l, r);
        l.f_matrix += r.f_number;
        break;

    case type_mix(type_t::TYPE_MATRIX, type_t::TYPE_NUMBER):
        l.f_matrix += r.f_number;
        break;

    case type_mix(type_t::TYPE_MATRIX, type_t::TYPE_MATRIX):
        l.f_matrix += r.f_matrix;
        break;

    default:
        std::cerr << "error: unknown combinaison for + operator." << std::endl;
        break;

    }
}


void matrix_expr::command_subtract()
{
    command_negate();
    command_add();
}


void matrix_expr::command_multiply()
{
    verify_stack(2);

    data_t r(f_stack.back());
    f_stack.pop_back();
    data_t & l(f_stack.back());

    switch(type_mix(l.f_type, r.f_type))
    {
    case type_mix(type_t::TYPE_NUMBER, type_t::TYPE_NUMBER):
        l.f_number *= r.f_number;
        break;

    case type_mix(type_t::TYPE_NUMBER, type_t::TYPE_MATRIX):
        std::swap(l, r);
        l.f_matrix *= r.f_number;
        break;

    case type_mix(type_t::TYPE_MATRIX, type_t::TYPE_NUMBER):
        l.f_matrix *= r.f_number;
        break;

    case type_mix(type_t::TYPE_MATRIX, type_t::TYPE_MATRIX):
        l.f_matrix *= r.f_matrix;
        break;

    default:
        std::cerr << "error: unknown combinaison for + operator." << std::endl;
        break;

    }
}


void matrix_expr::command_divide()
{
    verify_stack(2);

    data_t r(f_stack.back());
    f_stack.pop_back();
    data_t & l(f_stack.back());

    switch(type_mix(l.f_type, r.f_type))
    {
    case type_mix(type_t::TYPE_NUMBER, type_t::TYPE_NUMBER):
        l.f_number /= r.f_number;
        break;

    case type_mix(type_t::TYPE_NUMBER, type_t::TYPE_MATRIX):
        std::swap(l, r);
        l.f_matrix /= r.f_number;
        break;

    case type_mix(type_t::TYPE_MATRIX, type_t::TYPE_NUMBER):
        l.f_matrix /= r.f_number;
        break;

    case type_mix(type_t::TYPE_MATRIX, type_t::TYPE_MATRIX):
        l.f_matrix /= r.f_matrix;
        break;

    default:
        std::cerr << "error: unknown combinaison for + operator." << std::endl;
        break;

    }
}


void matrix_expr::command_negate()
{
    verify_stack(1);

    data_t & l(f_stack.back());

    switch(l.f_type)
    {
    case type_t::TYPE_NUMBER:
        l.f_number = -l.f_number;
        break;

    case type_t::TYPE_MATRIX:
        l.f_matrix *= -1.0;
        break;

    default:
        std::cerr << "error: unknown combinaison for + operator." << std::endl;
        break;

    }
}


void matrix_expr::command_inverse()
{
    verify_stack(1);

    data_t & l(f_stack.back());

    switch(l.f_type)
    {
    case type_t::TYPE_NUMBER:
        l.f_number = 1.0 / l.f_number;
        break;

    case type_t::TYPE_MATRIX:
        l.f_matrix.inverse();
        break;

    default:
        std::cerr << "error: unknown combinaison for + operator." << std::endl;
        break;

    }
}


void matrix_expr::command_deg2rad()
{
    verify_stack(1);

    data_t & l(f_stack.back());

    if(l.f_type != type_t::TYPE_NUMBER)
    {
        std::cerr << "error: deg2rad expected a number as input." << std::endl;
        return;
    }

    l.f_number *= M_PI / 180.0;
}


void matrix_expr::command_rotatex()
{
    verify_stack(1);

    data_t angle(f_stack.back());
    f_stack.pop_back();

    if(angle.f_type != type_t::TYPE_NUMBER)
    {
        std::cerr << "error: rotatex expected a number as its input." << std::endl;
        return;
    }

    matrix_t rotation(matrix_t::rotation_matrix_4x4_x(angle.f_number));
    data_t result(rotation);
    f_stack.push_back(result);
}


void matrix_expr::command_rotatey()
{
    verify_stack(1);

    data_t angle(f_stack.back());
    f_stack.pop_back();

    if(angle.f_type != type_t::TYPE_NUMBER)
    {
        std::cerr << "error: rotatey expected a number as its input." << std::endl;
        return;
    }

    matrix_t rotation(matrix_t::rotation_matrix_4x4_y(angle.f_number));
    data_t result(rotation);
    f_stack.push_back(result);
}


void matrix_expr::command_rotatez()
{
    verify_stack(1);

    data_t angle(f_stack.back());
    f_stack.pop_back();

    if(angle.f_type != type_t::TYPE_NUMBER)
    {
        std::cerr << "error: rotatez expected a number as its input." << std::endl;
        return;
    }

    matrix_t rotation(matrix_t::rotation_matrix_4x4_z(angle.f_number));
    data_t result(rotation);
    f_stack.push_back(result);
}



void matrix_expr::verify_stack(std::size_t required_size)
{
    if(f_stack.size() < required_size)
    {
        throw std::runtime_error(
                "your stack has "
                + std::to_string(f_stack.size())
                + ", but \""
                + f_command
                + "\" requires "
                + std::to_string(required_size)
                + " items on the stack.");
    }
}








int main(int argc, char * argv[])
{
    for(int i(1); i < argc; ++i)
    {
        if(argv[i][0] == '-')
        {
            switch(argv[i][1])
            {
            case 'h':
                std::cout << "Usage: "
                          << argv[0]
                          << " <filename>"
                          << std::endl;
                std::cout << std::endl;
                std::cout << "where <filename> is a forth like script."
                          << std::endl;
                break;

            case '\0':
                {
                    matrix_expr m(std::cin);
                    m.run();
                }
                break;

            }
        }
        else
        {
            std::ifstream in(argv[i]);
            if(!in.is_open())
            {
                std::cerr << "error: could not open input file \""
                          << argv[i]
                          << "\"."
                          << std::endl;
            }
            else
            {
                matrix_expr m(in);
                m.run();
            }
        }
    }

    exit(0);
}

// vim: ts=4 sw=4 et
