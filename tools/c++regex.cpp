// Copyright (c) 2011-2024  Made to Order Software Corp.  All Rights Reserved
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

/** \file
 * \brief Run one or more strings against a regular expression.
 *
 * This tool can be used to check various strings against a regular
 * expression:
 *
 * \code
 *     c++regex <regex> <string1> <string2> ... <stringN>
 * \endcode
 */

// C++
//
#include    <iostream>
#include    <regex>
#include    <string>



void usage()
{
    std::cout << "Usage: c++regex [--opts] <regex> <string1> [<string2> [... [<stringN>]]]\n";
    std::cout << "where --opts is one or more of:\n";
    std::cout << "  -h | --help         print out this help screen\n";
    std::cout << "  -m | --matches      only show matches\n";
    std::cout << "  -q | --quiet        don't show results in console\n";
}


int main(int argc, char * argv[])
{
    std::string regex;
    std::vector<std::string> strings;
    bool quiet(false);
    bool matches(false);

    for(int i(1); i < argc; ++i)
    {
        if(argv[i][0] == '-')
        {
            if(strcmp(argv[i], "-h") == 0
            || strcmp(argv[i], "--help") == 0)
            {
                usage();
                return 1;
            }
            else if(strcmp(argv[i], "-q") == 0
                 || strcmp(argv[i], "--quiet") == 0)
            {
                quiet = true;
            }
            else if(strcmp(argv[i], "-m") == 0
                 || strcmp(argv[i], "--matches") == 0)
            {
                matches = true;
            }
        }
        else
        {
            if(regex.empty())
            {
                regex = argv[i];
            }
            else
            {
                strings.push_back(argv[i]);
            }
        }
    }

    if(regex.empty()
    || strings.empty())
    {
        std::cerr << "error: at least one regular expression and one string are needed.\n";
        return 1;
    }

    std::regex re(regex);

    int r(0);
    for(auto s : strings)
    {
        if(std::regex_match(s, re))
        {
            if(!quiet)
            {
                if(matches)
                {
                    std::cout << s << '\n';
                }
                else
                {
                    std::cout << s << " is a match.\n";
                }
            }
        }
        else
        {
            r = 1;

            if(!quiet && !matches)
            {
                std::cout << s << " is not a match.\n";
            }
        }
    }

    return r;
}


// vim: ts=4 sw=4 et
