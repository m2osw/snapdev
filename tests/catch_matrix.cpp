// Copyright (c) 2018-2025  Made to Order Software Corp.  All Rights Reserved
//
// https://snapwebsites.org/
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
 * \brief Verify the various matrix implementation.
 *
 * This file implements tests to verify that the matrix templates
 * work as expected.
 */

// self
//
#include    "catch_main.h"



// ignore the == and != against float warnings
//
#pragma GCC diagnostic ignored "-Wfloat-equal"


// snapdev lib
//
#include    <snapdev/matrix.h>


// last include
//
#include    <snapdev/poison.h>





namespace
{
    auto frand = []()
    {
        return static_cast<double>(rand()) / static_cast<double>(rand());
    };
}
// no name namespace



CATCH_TEST_CASE("matrix_init", "[matrix][math]")
{
    // constructor/copy
    // and
    // zero/identity
    //
    CATCH_GIVEN("constructor")
    {
        CATCH_START_SECTION("matrix: empty")
        {
            snapdev::matrix<double> empty;

            CATCH_REQUIRE(empty.rows() == 0);
            CATCH_REQUIRE(empty.columns() == 0);

            snapdev::matrix<double> copy(empty);

            CATCH_REQUIRE(empty.rows() == 0);
            CATCH_REQUIRE(empty.columns() == 0);
        }
        CATCH_END_SECTION()

        CATCH_START_SECTION("matrix: 2x2")
        {
            snapdev::matrix<double> m(2, 2);

            CATCH_REQUIRE(m.rows() == 2);
            CATCH_REQUIRE(m.columns() == 2);

            // by default we get an identity
            //
            CATCH_REQUIRE(m[0][0] == 1.0);
            CATCH_REQUIRE(m[0][1] == 0.0);
            CATCH_REQUIRE(m[1][0] == 0.0);
            CATCH_REQUIRE(m[1][1] == 1.0);

            double const r00(frand());
            double const r01(frand());
            double const r10(frand());
            double const r11(frand());

            m[0][0] = r00;
            m[0][1] = r01;
            m[1][0] = r10;
            m[1][1] = r11;

            CATCH_REQUIRE(m[0][0] == r00);
            CATCH_REQUIRE(m[0][1] == r01);
            CATCH_REQUIRE(m[1][0] == r10);
            CATCH_REQUIRE(m[1][1] == r11);

            snapdev::matrix<double> copy(m);
            snapdev::matrix<double> c2;

            CATCH_REQUIRE(copy[0][0] == r00);
            CATCH_REQUIRE(copy[0][1] == r01);
            CATCH_REQUIRE(copy[1][0] == r10);
            CATCH_REQUIRE(copy[1][1] == r11);

            m.clear();

            CATCH_REQUIRE(m[0][0] == 0.0);
            CATCH_REQUIRE(m[0][1] == 0.0);
            CATCH_REQUIRE(m[1][0] == 0.0);
            CATCH_REQUIRE(m[1][1] == 0.0);

            // copy is still intact
            //
            CATCH_REQUIRE(copy[0][0] == r00);
            CATCH_REQUIRE(copy[0][1] == r01);
            CATCH_REQUIRE(copy[1][0] == r10);
            CATCH_REQUIRE(copy[1][1] == r11);

            CATCH_REQUIRE(c2.rows() == 0);
            CATCH_REQUIRE(c2.columns() == 0);

            c2 = copy;

            CATCH_REQUIRE(c2[0][0] == r00);
            CATCH_REQUIRE(c2[0][1] == r01);
            CATCH_REQUIRE(c2[1][0] == r10);
            CATCH_REQUIRE(c2[1][1] == r11);

            c2.swap(m);

            CATCH_REQUIRE(m[0][0] == r00);
            CATCH_REQUIRE(m[0][1] == r01);
            CATCH_REQUIRE(m[1][0] == r10);
            CATCH_REQUIRE(m[1][1] == r11);

            CATCH_REQUIRE(c2[0][0] == 0.0);
            CATCH_REQUIRE(c2[0][1] == 0.0);
            CATCH_REQUIRE(c2[1][0] == 0.0);
            CATCH_REQUIRE(c2[1][1] == 0.0);
        }
        CATCH_END_SECTION()

        CATCH_START_SECTION("matrix: 3x3")
        {
            snapdev::matrix<double> m(3, 3);

            CATCH_REQUIRE(m.rows() == 3);
            CATCH_REQUIRE(m.columns() == 3);

            // by default we get an identity
            //
            CATCH_REQUIRE(m[0][0] == 1.0);
            CATCH_REQUIRE(m[0][1] == 0.0);
            CATCH_REQUIRE(m[0][2] == 0.0);
            CATCH_REQUIRE(m[1][0] == 0.0);
            CATCH_REQUIRE(m[1][1] == 1.0);
            CATCH_REQUIRE(m[1][2] == 0.0);
            CATCH_REQUIRE(m[2][0] == 0.0);
            CATCH_REQUIRE(m[2][1] == 0.0);
            CATCH_REQUIRE(m[2][2] == 1.0);

            double const r00(frand());
            double const r01(frand());
            double const r02(frand());
            double const r10(frand());
            double const r11(frand());
            double const r12(frand());
            double const r20(frand());
            double const r21(frand());
            double const r22(frand());

            m[0][0] = r00;
            m[0][1] = r01;
            m[0][2] = r02;
            m[1][0] = r10;
            m[1][1] = r11;
            m[1][2] = r12;
            m[2][0] = r20;
            m[2][1] = r21;
            m[2][2] = r22;

            CATCH_REQUIRE(m[0][0] == r00);
            CATCH_REQUIRE(m[0][1] == r01);
            CATCH_REQUIRE(m[0][2] == r02);
            CATCH_REQUIRE(m[1][0] == r10);
            CATCH_REQUIRE(m[1][1] == r11);
            CATCH_REQUIRE(m[1][2] == r12);
            CATCH_REQUIRE(m[2][0] == r20);
            CATCH_REQUIRE(m[2][1] == r21);
            CATCH_REQUIRE(m[2][2] == r22);

            snapdev::matrix<double> copy(m);
            snapdev::matrix<double> c2;

            CATCH_REQUIRE(copy[0][0] == r00);
            CATCH_REQUIRE(copy[0][1] == r01);
            CATCH_REQUIRE(copy[0][2] == r02);
            CATCH_REQUIRE(copy[1][0] == r10);
            CATCH_REQUIRE(copy[1][1] == r11);
            CATCH_REQUIRE(copy[1][2] == r12);
            CATCH_REQUIRE(copy[2][0] == r20);
            CATCH_REQUIRE(copy[2][1] == r21);
            CATCH_REQUIRE(copy[2][2] == r22);

            m.clear();

            CATCH_REQUIRE(m[0][0] == 0.0);
            CATCH_REQUIRE(m[0][1] == 0.0);
            CATCH_REQUIRE(m[0][2] == 0.0);
            CATCH_REQUIRE(m[1][0] == 0.0);
            CATCH_REQUIRE(m[1][1] == 0.0);
            CATCH_REQUIRE(m[1][2] == 0.0);
            CATCH_REQUIRE(m[2][0] == 0.0);
            CATCH_REQUIRE(m[2][1] == 0.0);
            CATCH_REQUIRE(m[2][2] == 0.0);

            // copy is still intact
            //
            CATCH_REQUIRE(copy[0][0] == r00);
            CATCH_REQUIRE(copy[0][1] == r01);
            CATCH_REQUIRE(copy[0][2] == r02);
            CATCH_REQUIRE(copy[1][0] == r10);
            CATCH_REQUIRE(copy[1][1] == r11);
            CATCH_REQUIRE(copy[1][2] == r12);
            CATCH_REQUIRE(copy[2][0] == r20);
            CATCH_REQUIRE(copy[2][1] == r21);
            CATCH_REQUIRE(copy[2][2] == r22);

            CATCH_REQUIRE(c2.rows() == 0);
            CATCH_REQUIRE(c2.columns() == 0);

            c2 = copy;

            CATCH_REQUIRE(c2[0][0] == r00);
            CATCH_REQUIRE(c2[0][1] == r01);
            CATCH_REQUIRE(c2[0][2] == r02);
            CATCH_REQUIRE(c2[1][0] == r10);
            CATCH_REQUIRE(c2[1][1] == r11);
            CATCH_REQUIRE(c2[1][2] == r12);
            CATCH_REQUIRE(c2[2][0] == r20);
            CATCH_REQUIRE(c2[2][1] == r21);
            CATCH_REQUIRE(c2[2][2] == r22);

            c2.swap(m);

            CATCH_REQUIRE(m[0][0] == r00);
            CATCH_REQUIRE(m[0][1] == r01);
            CATCH_REQUIRE(m[0][2] == r02);
            CATCH_REQUIRE(m[1][0] == r10);
            CATCH_REQUIRE(m[1][1] == r11);
            CATCH_REQUIRE(m[1][2] == r12);
            CATCH_REQUIRE(m[2][0] == r20);
            CATCH_REQUIRE(m[2][1] == r21);
            CATCH_REQUIRE(m[2][2] == r22);

            CATCH_REQUIRE(c2[0][0] == 0.0);
            CATCH_REQUIRE(c2[0][1] == 0.0);
            CATCH_REQUIRE(c2[0][2] == 0.0);
            CATCH_REQUIRE(c2[1][0] == 0.0);
            CATCH_REQUIRE(c2[1][1] == 0.0);
            CATCH_REQUIRE(c2[1][2] == 0.0);
            CATCH_REQUIRE(c2[2][0] == 0.0);
            CATCH_REQUIRE(c2[2][1] == 0.0);
            CATCH_REQUIRE(c2[2][2] == 0.0);
        }
        CATCH_END_SECTION()

        CATCH_START_SECTION("matrix: 4x4")
        {
            snapdev::matrix<double> m(4, 4);

            CATCH_REQUIRE(m.rows() == 4);
            CATCH_REQUIRE(m.columns() == 4);

            // by default we get an identity
            //
            CATCH_REQUIRE(m[0][0] == 1.0);
            CATCH_REQUIRE(m[0][1] == 0.0);
            CATCH_REQUIRE(m[0][2] == 0.0);
            CATCH_REQUIRE(m[0][3] == 0.0);
            CATCH_REQUIRE(m[1][0] == 0.0);
            CATCH_REQUIRE(m[1][1] == 1.0);
            CATCH_REQUIRE(m[1][2] == 0.0);
            CATCH_REQUIRE(m[1][3] == 0.0);
            CATCH_REQUIRE(m[2][0] == 0.0);
            CATCH_REQUIRE(m[2][1] == 0.0);
            CATCH_REQUIRE(m[2][2] == 1.0);
            CATCH_REQUIRE(m[2][3] == 0.0);
            CATCH_REQUIRE(m[3][0] == 0.0);
            CATCH_REQUIRE(m[3][1] == 0.0);
            CATCH_REQUIRE(m[3][2] == 0.0);
            CATCH_REQUIRE(m[3][3] == 1.0);

            double const r00(frand());
            double const r01(frand());
            double const r02(frand());
            double const r03(frand());
            double const r10(frand());
            double const r11(frand());
            double const r12(frand());
            double const r13(frand());
            double const r20(frand());
            double const r21(frand());
            double const r22(frand());
            double const r23(frand());
            double const r30(frand());
            double const r31(frand());
            double const r32(frand());
            double const r33(frand());

            m[0][0] = r00;
            m[0][1] = r01;
            m[0][2] = r02;
            m[0][3] = r03;
            m[1][0] = r10;
            m[1][1] = r11;
            m[1][2] = r12;
            m[1][3] = r13;
            m[2][0] = r20;
            m[2][1] = r21;
            m[2][2] = r22;
            m[2][3] = r23;
            m[3][0] = r30;
            m[3][1] = r31;
            m[3][2] = r32;
            m[3][3] = r33;

            CATCH_REQUIRE(m[0][0] == r00);
            CATCH_REQUIRE(m[0][1] == r01);
            CATCH_REQUIRE(m[0][2] == r02);
            CATCH_REQUIRE(m[0][3] == r03);
            CATCH_REQUIRE(m[1][0] == r10);
            CATCH_REQUIRE(m[1][1] == r11);
            CATCH_REQUIRE(m[1][2] == r12);
            CATCH_REQUIRE(m[1][3] == r13);
            CATCH_REQUIRE(m[2][0] == r20);
            CATCH_REQUIRE(m[2][1] == r21);
            CATCH_REQUIRE(m[2][2] == r22);
            CATCH_REQUIRE(m[2][3] == r23);
            CATCH_REQUIRE(m[3][0] == r30);
            CATCH_REQUIRE(m[3][1] == r31);
            CATCH_REQUIRE(m[3][2] == r32);
            CATCH_REQUIRE(m[3][3] == r33);

            snapdev::matrix<double> copy(m);
            snapdev::matrix<double> c2;

            CATCH_REQUIRE(copy[0][0] == r00);
            CATCH_REQUIRE(copy[0][1] == r01);
            CATCH_REQUIRE(copy[0][2] == r02);
            CATCH_REQUIRE(copy[0][3] == r03);
            CATCH_REQUIRE(copy[1][0] == r10);
            CATCH_REQUIRE(copy[1][1] == r11);
            CATCH_REQUIRE(copy[1][2] == r12);
            CATCH_REQUIRE(copy[1][3] == r13);
            CATCH_REQUIRE(copy[2][0] == r20);
            CATCH_REQUIRE(copy[2][1] == r21);
            CATCH_REQUIRE(copy[2][2] == r22);
            CATCH_REQUIRE(copy[2][3] == r23);
            CATCH_REQUIRE(copy[3][0] == r30);
            CATCH_REQUIRE(copy[3][1] == r31);
            CATCH_REQUIRE(copy[3][2] == r32);
            CATCH_REQUIRE(copy[3][3] == r33);

            m.clear();

            CATCH_REQUIRE(m[0][0] == 0.0);
            CATCH_REQUIRE(m[0][1] == 0.0);
            CATCH_REQUIRE(m[0][2] == 0.0);
            CATCH_REQUIRE(m[0][3] == 0.0);
            CATCH_REQUIRE(m[1][0] == 0.0);
            CATCH_REQUIRE(m[1][1] == 0.0);
            CATCH_REQUIRE(m[1][2] == 0.0);
            CATCH_REQUIRE(m[1][3] == 0.0);
            CATCH_REQUIRE(m[2][0] == 0.0);
            CATCH_REQUIRE(m[2][1] == 0.0);
            CATCH_REQUIRE(m[2][2] == 0.0);
            CATCH_REQUIRE(m[2][3] == 0.0);

            // copy is still intact
            //
            CATCH_REQUIRE(copy[0][0] == r00);
            CATCH_REQUIRE(copy[0][1] == r01);
            CATCH_REQUIRE(copy[0][2] == r02);
            CATCH_REQUIRE(copy[0][3] == r03);
            CATCH_REQUIRE(copy[1][0] == r10);
            CATCH_REQUIRE(copy[1][1] == r11);
            CATCH_REQUIRE(copy[1][2] == r12);
            CATCH_REQUIRE(copy[1][3] == r13);
            CATCH_REQUIRE(copy[2][0] == r20);
            CATCH_REQUIRE(copy[2][1] == r21);
            CATCH_REQUIRE(copy[2][2] == r22);
            CATCH_REQUIRE(copy[2][3] == r23);
            CATCH_REQUIRE(copy[3][0] == r30);
            CATCH_REQUIRE(copy[3][1] == r31);
            CATCH_REQUIRE(copy[3][2] == r32);
            CATCH_REQUIRE(copy[3][3] == r33);

            CATCH_REQUIRE(c2.rows() == 0);
            CATCH_REQUIRE(c2.columns() == 0);

            c2 = copy;

            CATCH_REQUIRE(c2[0][0] == r00);
            CATCH_REQUIRE(c2[0][1] == r01);
            CATCH_REQUIRE(c2[0][2] == r02);
            CATCH_REQUIRE(c2[0][3] == r03);
            CATCH_REQUIRE(c2[1][0] == r10);
            CATCH_REQUIRE(c2[1][1] == r11);
            CATCH_REQUIRE(c2[1][2] == r12);
            CATCH_REQUIRE(c2[1][3] == r13);
            CATCH_REQUIRE(c2[2][0] == r20);
            CATCH_REQUIRE(c2[2][1] == r21);
            CATCH_REQUIRE(c2[2][2] == r22);
            CATCH_REQUIRE(c2[2][3] == r23);
            CATCH_REQUIRE(c2[3][0] == r30);
            CATCH_REQUIRE(c2[3][1] == r31);
            CATCH_REQUIRE(c2[3][2] == r32);
            CATCH_REQUIRE(c2[3][3] == r33);

            //std::cout << c2 << std::endl;

            c2.swap(m);

            CATCH_REQUIRE(m[0][0] == r00);
            CATCH_REQUIRE(m[0][1] == r01);
            CATCH_REQUIRE(m[0][2] == r02);
            CATCH_REQUIRE(m[0][3] == r03);
            CATCH_REQUIRE(m[1][0] == r10);
            CATCH_REQUIRE(m[1][1] == r11);
            CATCH_REQUIRE(m[1][2] == r12);
            CATCH_REQUIRE(m[1][3] == r13);
            CATCH_REQUIRE(m[2][0] == r20);
            CATCH_REQUIRE(m[2][1] == r21);
            CATCH_REQUIRE(m[2][2] == r22);
            CATCH_REQUIRE(m[2][3] == r23);
            CATCH_REQUIRE(m[3][0] == r30);
            CATCH_REQUIRE(m[3][1] == r31);
            CATCH_REQUIRE(m[3][2] == r32);
            CATCH_REQUIRE(m[3][3] == r33);

            CATCH_REQUIRE(c2[0][0] == 0.0);
            CATCH_REQUIRE(c2[0][1] == 0.0);
            CATCH_REQUIRE(c2[0][2] == 0.0);
            CATCH_REQUIRE(c2[0][3] == 0.0);
            CATCH_REQUIRE(c2[1][0] == 0.0);
            CATCH_REQUIRE(c2[1][1] == 0.0);
            CATCH_REQUIRE(c2[1][2] == 0.0);
            CATCH_REQUIRE(c2[1][3] == 0.0);
            CATCH_REQUIRE(c2[2][0] == 0.0);
            CATCH_REQUIRE(c2[2][1] == 0.0);
            CATCH_REQUIRE(c2[2][2] == 0.0);
            CATCH_REQUIRE(c2[2][3] == 0.0);
        }
        CATCH_END_SECTION()
    }
}


CATCH_TEST_CASE("matrix_additive", "[matrix][math]")
{
    // create two random 4x4 matrices and make sure the add works
    //
    CATCH_GIVEN("add")
    {
        CATCH_START_SECTION("matrix: a+=<scalar>")
        {
            // setup A
            //
            snapdev::matrix<double> a(4, 4);

            CATCH_REQUIRE(a.rows() == 4);
            CATCH_REQUIRE(a.columns() == 4);

            double const a00(frand());
            double const a01(frand());
            double const a02(frand());
            double const a03(frand());
            double const a10(frand());
            double const a11(frand());
            double const a12(frand());
            double const a13(frand());
            double const a20(frand());
            double const a21(frand());
            double const a22(frand());
            double const a23(frand());
            double const a30(frand());
            double const a31(frand());
            double const a32(frand());
            double const a33(frand());

            a[0][0] = a00;
            a[0][1] = a01;
            a[0][2] = a02;
            a[0][3] = a03;
            a[1][0] = a10;
            a[1][1] = a11;
            a[1][2] = a12;
            a[1][3] = a13;
            a[2][0] = a20;
            a[2][1] = a21;
            a[2][2] = a22;
            a[2][3] = a23;
            a[3][0] = a30;
            a[3][1] = a31;
            a[3][2] = a32;
            a[3][3] = a33;

            CATCH_REQUIRE(a[0][0] == a00);
            CATCH_REQUIRE(a[0][1] == a01);
            CATCH_REQUIRE(a[0][2] == a02);
            CATCH_REQUIRE(a[0][3] == a03);
            CATCH_REQUIRE(a[1][0] == a10);
            CATCH_REQUIRE(a[1][1] == a11);
            CATCH_REQUIRE(a[1][2] == a12);
            CATCH_REQUIRE(a[1][3] == a13);
            CATCH_REQUIRE(a[2][0] == a20);
            CATCH_REQUIRE(a[2][1] == a21);
            CATCH_REQUIRE(a[2][2] == a22);
            CATCH_REQUIRE(a[2][3] == a23);
            CATCH_REQUIRE(a[3][0] == a30);
            CATCH_REQUIRE(a[3][1] == a31);
            CATCH_REQUIRE(a[3][2] == a32);
            CATCH_REQUIRE(a[3][3] == a33);

            // create a scalar for our test
            //
            double scalar(frand());
            while(scalar == 0.0)
            {
                scalar = frand();
            }

            // run operation A += <scalar>
            //
            a += scalar;

            // this can't fail because we ensure scalar != 0
            //
            CATCH_REQUIRE_FALSE(a[0][0] == a00);
            CATCH_REQUIRE_FALSE(a[0][1] == a01);
            CATCH_REQUIRE_FALSE(a[0][2] == a02);
            CATCH_REQUIRE_FALSE(a[0][3] == a03);
            CATCH_REQUIRE_FALSE(a[1][0] == a10);
            CATCH_REQUIRE_FALSE(a[1][1] == a11);
            CATCH_REQUIRE_FALSE(a[1][2] == a12);
            CATCH_REQUIRE_FALSE(a[1][3] == a13);
            CATCH_REQUIRE_FALSE(a[2][0] == a20);
            CATCH_REQUIRE_FALSE(a[2][1] == a21);
            CATCH_REQUIRE_FALSE(a[2][2] == a22);
            CATCH_REQUIRE_FALSE(a[2][3] == a23);
            CATCH_REQUIRE_FALSE(a[3][0] == a30);
            CATCH_REQUIRE_FALSE(a[3][1] == a31);
            CATCH_REQUIRE_FALSE(a[3][2] == a32);
            CATCH_REQUIRE_FALSE(a[3][3] == a33);

            CATCH_REQUIRE(a[0][0] == a00 + scalar);
            CATCH_REQUIRE(a[0][1] == a01 + scalar);
            CATCH_REQUIRE(a[0][2] == a02 + scalar);
            CATCH_REQUIRE(a[0][3] == a03 + scalar);
            CATCH_REQUIRE(a[1][0] == a10 + scalar);
            CATCH_REQUIRE(a[1][1] == a11 + scalar);
            CATCH_REQUIRE(a[1][2] == a12 + scalar);
            CATCH_REQUIRE(a[1][3] == a13 + scalar);
            CATCH_REQUIRE(a[2][0] == a20 + scalar);
            CATCH_REQUIRE(a[2][1] == a21 + scalar);
            CATCH_REQUIRE(a[2][2] == a22 + scalar);
            CATCH_REQUIRE(a[2][3] == a23 + scalar);
            CATCH_REQUIRE(a[3][0] == a30 + scalar);
            CATCH_REQUIRE(a[3][1] == a31 + scalar);
            CATCH_REQUIRE(a[3][2] == a32 + scalar);
            CATCH_REQUIRE(a[3][3] == a33 + scalar);
        }
        CATCH_END_SECTION()

        CATCH_START_SECTION("matrix: b=a+<scalar>")
        {
            // setup A
            //
            snapdev::matrix<double> a(4, 4);

            CATCH_REQUIRE(a.rows() == 4);
            CATCH_REQUIRE(a.columns() == 4);

            double const a00(frand());
            double const a01(frand());
            double const a02(frand());
            double const a03(frand());
            double const a10(frand());
            double const a11(frand());
            double const a12(frand());
            double const a13(frand());
            double const a20(frand());
            double const a21(frand());
            double const a22(frand());
            double const a23(frand());
            double const a30(frand());
            double const a31(frand());
            double const a32(frand());
            double const a33(frand());

            a[0][0] = a00;
            a[0][1] = a01;
            a[0][2] = a02;
            a[0][3] = a03;
            a[1][0] = a10;
            a[1][1] = a11;
            a[1][2] = a12;
            a[1][3] = a13;
            a[2][0] = a20;
            a[2][1] = a21;
            a[2][2] = a22;
            a[2][3] = a23;
            a[3][0] = a30;
            a[3][1] = a31;
            a[3][2] = a32;
            a[3][3] = a33;

            CATCH_REQUIRE(a[0][0] == a00);
            CATCH_REQUIRE(a[0][1] == a01);
            CATCH_REQUIRE(a[0][2] == a02);
            CATCH_REQUIRE(a[0][3] == a03);
            CATCH_REQUIRE(a[1][0] == a10);
            CATCH_REQUIRE(a[1][1] == a11);
            CATCH_REQUIRE(a[1][2] == a12);
            CATCH_REQUIRE(a[1][3] == a13);
            CATCH_REQUIRE(a[2][0] == a20);
            CATCH_REQUIRE(a[2][1] == a21);
            CATCH_REQUIRE(a[2][2] == a22);
            CATCH_REQUIRE(a[2][3] == a23);
            CATCH_REQUIRE(a[3][0] == a30);
            CATCH_REQUIRE(a[3][1] == a31);
            CATCH_REQUIRE(a[3][2] == a32);
            CATCH_REQUIRE(a[3][3] == a33);

            // setup B
            //
            snapdev::matrix<double> b(4, 4);

            CATCH_REQUIRE(b.rows() == 4);
            CATCH_REQUIRE(b.columns() == 4);

            double const b00(frand());
            double const b01(frand());
            double const b02(frand());
            double const b03(frand());
            double const b10(frand());
            double const b11(frand());
            double const b12(frand());
            double const b13(frand());
            double const b20(frand());
            double const b21(frand());
            double const b22(frand());
            double const b23(frand());
            double const b30(frand());
            double const b31(frand());
            double const b32(frand());
            double const b33(frand());

            b[0][0] = b00;
            b[0][1] = b01;
            b[0][2] = b02;
            b[0][3] = b03;
            b[1][0] = b10;
            b[1][1] = b11;
            b[1][2] = b12;
            b[1][3] = b13;
            b[2][0] = b20;
            b[2][1] = b21;
            b[2][2] = b22;
            b[2][3] = b23;
            b[3][0] = b30;
            b[3][1] = b31;
            b[3][2] = b32;
            b[3][3] = b33;

            CATCH_REQUIRE(b[0][0] == b00);
            CATCH_REQUIRE(b[0][1] == b01);
            CATCH_REQUIRE(b[0][2] == b02);
            CATCH_REQUIRE(b[0][3] == b03);
            CATCH_REQUIRE(b[1][0] == b10);
            CATCH_REQUIRE(b[1][1] == b11);
            CATCH_REQUIRE(b[1][2] == b12);
            CATCH_REQUIRE(b[1][3] == b13);
            CATCH_REQUIRE(b[2][0] == b20);
            CATCH_REQUIRE(b[2][1] == b21);
            CATCH_REQUIRE(b[2][2] == b22);
            CATCH_REQUIRE(b[2][3] == b23);
            CATCH_REQUIRE(b[3][0] == b30);
            CATCH_REQUIRE(b[3][1] == b31);
            CATCH_REQUIRE(b[3][2] == b32);
            CATCH_REQUIRE(b[3][3] == b33);

            // create a scalar for our test
            //
            double scalar(frand());
            while(scalar == 0.0)
            {
                scalar = frand();
            }

            // run operation B = A + <scalar>
            //
            b = a + scalar;

            CATCH_REQUIRE(a[0][0] == a00);
            CATCH_REQUIRE(a[0][1] == a01);
            CATCH_REQUIRE(a[0][2] == a02);
            CATCH_REQUIRE(a[0][3] == a03);
            CATCH_REQUIRE(a[1][0] == a10);
            CATCH_REQUIRE(a[1][1] == a11);
            CATCH_REQUIRE(a[1][2] == a12);
            CATCH_REQUIRE(a[1][3] == a13);
            CATCH_REQUIRE(a[2][0] == a20);
            CATCH_REQUIRE(a[2][1] == a21);
            CATCH_REQUIRE(a[2][2] == a22);
            CATCH_REQUIRE(a[2][3] == a23);
            CATCH_REQUIRE(a[3][0] == a30);
            CATCH_REQUIRE(a[3][1] == a31);
            CATCH_REQUIRE(a[3][2] == a32);
            CATCH_REQUIRE(a[3][3] == a33);

            // this can't fail because we ensure scalar != 0
            //
            CATCH_REQUIRE_FALSE(b[0][0] == b00);
            CATCH_REQUIRE_FALSE(b[0][1] == b01);
            CATCH_REQUIRE_FALSE(b[0][2] == b02);
            CATCH_REQUIRE_FALSE(b[0][3] == b03);
            CATCH_REQUIRE_FALSE(b[1][0] == b10);
            CATCH_REQUIRE_FALSE(b[1][1] == b11);
            CATCH_REQUIRE_FALSE(b[1][2] == b12);
            CATCH_REQUIRE_FALSE(b[1][3] == b13);
            CATCH_REQUIRE_FALSE(b[2][0] == b20);
            CATCH_REQUIRE_FALSE(b[2][1] == b21);
            CATCH_REQUIRE_FALSE(b[2][2] == b22);
            CATCH_REQUIRE_FALSE(b[2][3] == b23);
            CATCH_REQUIRE_FALSE(b[3][0] == b30);
            CATCH_REQUIRE_FALSE(b[3][1] == b31);
            CATCH_REQUIRE_FALSE(b[3][2] == b32);
            CATCH_REQUIRE_FALSE(b[3][3] == b33);

            CATCH_REQUIRE(b[0][0] == a00 + scalar);
            CATCH_REQUIRE(b[0][1] == a01 + scalar);
            CATCH_REQUIRE(b[0][2] == a02 + scalar);
            CATCH_REQUIRE(b[0][3] == a03 + scalar);
            CATCH_REQUIRE(b[1][0] == a10 + scalar);
            CATCH_REQUIRE(b[1][1] == a11 + scalar);
            CATCH_REQUIRE(b[1][2] == a12 + scalar);
            CATCH_REQUIRE(b[1][3] == a13 + scalar);
            CATCH_REQUIRE(b[2][0] == a20 + scalar);
            CATCH_REQUIRE(b[2][1] == a21 + scalar);
            CATCH_REQUIRE(b[2][2] == a22 + scalar);
            CATCH_REQUIRE(b[2][3] == a23 + scalar);
            CATCH_REQUIRE(b[3][0] == a30 + scalar);
            CATCH_REQUIRE(b[3][1] == a31 + scalar);
            CATCH_REQUIRE(b[3][2] == a32 + scalar);
            CATCH_REQUIRE(b[3][3] == a33 + scalar);
        }
        CATCH_END_SECTION()

        CATCH_START_SECTION("matrix: c=a+b")
        {
            // setup A
            //
            snapdev::matrix<double> a(4, 4);

            CATCH_REQUIRE(a.rows() == 4);
            CATCH_REQUIRE(a.columns() == 4);

            double const a00(frand());
            double const a01(frand());
            double const a02(frand());
            double const a03(frand());
            double const a10(frand());
            double const a11(frand());
            double const a12(frand());
            double const a13(frand());
            double const a20(frand());
            double const a21(frand());
            double const a22(frand());
            double const a23(frand());
            double const a30(frand());
            double const a31(frand());
            double const a32(frand());
            double const a33(frand());

            a[0][0] = a00;
            a[0][1] = a01;
            a[0][2] = a02;
            a[0][3] = a03;
            a[1][0] = a10;
            a[1][1] = a11;
            a[1][2] = a12;
            a[1][3] = a13;
            a[2][0] = a20;
            a[2][1] = a21;
            a[2][2] = a22;
            a[2][3] = a23;
            a[3][0] = a30;
            a[3][1] = a31;
            a[3][2] = a32;
            a[3][3] = a33;

            CATCH_REQUIRE(a[0][0] == a00);
            CATCH_REQUIRE(a[0][1] == a01);
            CATCH_REQUIRE(a[0][2] == a02);
            CATCH_REQUIRE(a[0][3] == a03);
            CATCH_REQUIRE(a[1][0] == a10);
            CATCH_REQUIRE(a[1][1] == a11);
            CATCH_REQUIRE(a[1][2] == a12);
            CATCH_REQUIRE(a[1][3] == a13);
            CATCH_REQUIRE(a[2][0] == a20);
            CATCH_REQUIRE(a[2][1] == a21);
            CATCH_REQUIRE(a[2][2] == a22);
            CATCH_REQUIRE(a[2][3] == a23);
            CATCH_REQUIRE(a[3][0] == a30);
            CATCH_REQUIRE(a[3][1] == a31);
            CATCH_REQUIRE(a[3][2] == a32);
            CATCH_REQUIRE(a[3][3] == a33);

            // setup B
            //
            snapdev::matrix<double> b(4, 4);

            CATCH_REQUIRE(b.rows() == 4);
            CATCH_REQUIRE(b.columns() == 4);

            double const b00(frand());
            double const b01(frand());
            double const b02(frand());
            double const b03(frand());
            double const b10(frand());
            double const b11(frand());
            double const b12(frand());
            double const b13(frand());
            double const b20(frand());
            double const b21(frand());
            double const b22(frand());
            double const b23(frand());
            double const b30(frand());
            double const b31(frand());
            double const b32(frand());
            double const b33(frand());

            b[0][0] = b00;
            b[0][1] = b01;
            b[0][2] = b02;
            b[0][3] = b03;
            b[1][0] = b10;
            b[1][1] = b11;
            b[1][2] = b12;
            b[1][3] = b13;
            b[2][0] = b20;
            b[2][1] = b21;
            b[2][2] = b22;
            b[2][3] = b23;
            b[3][0] = b30;
            b[3][1] = b31;
            b[3][2] = b32;
            b[3][3] = b33;

            CATCH_REQUIRE(b[0][0] == b00);
            CATCH_REQUIRE(b[0][1] == b01);
            CATCH_REQUIRE(b[0][2] == b02);
            CATCH_REQUIRE(b[0][3] == b03);
            CATCH_REQUIRE(b[1][0] == b10);
            CATCH_REQUIRE(b[1][1] == b11);
            CATCH_REQUIRE(b[1][2] == b12);
            CATCH_REQUIRE(b[1][3] == b13);
            CATCH_REQUIRE(b[2][0] == b20);
            CATCH_REQUIRE(b[2][1] == b21);
            CATCH_REQUIRE(b[2][2] == b22);
            CATCH_REQUIRE(b[2][3] == b23);
            CATCH_REQUIRE(b[3][0] == b30);
            CATCH_REQUIRE(b[3][1] == b31);
            CATCH_REQUIRE(b[3][2] == b32);
            CATCH_REQUIRE(b[3][3] == b33);

            // setup C
            //
            snapdev::matrix<double> c(4, 4);

            CATCH_REQUIRE(c.rows() == 4);
            CATCH_REQUIRE(c.columns() == 4);

            double const c00(frand());
            double const c01(frand());
            double const c02(frand());
            double const c03(frand());
            double const c10(frand());
            double const c11(frand());
            double const c12(frand());
            double const c13(frand());
            double const c20(frand());
            double const c21(frand());
            double const c22(frand());
            double const c23(frand());
            double const c30(frand());
            double const c31(frand());
            double const c32(frand());
            double const c33(frand());

            c[0][0] = c00;
            c[0][1] = c01;
            c[0][2] = c02;
            c[0][3] = c03;
            c[1][0] = c10;
            c[1][1] = c11;
            c[1][2] = c12;
            c[1][3] = c13;
            c[2][0] = c20;
            c[2][1] = c21;
            c[2][2] = c22;
            c[2][3] = c23;
            c[3][0] = c30;
            c[3][1] = c31;
            c[3][2] = c32;
            c[3][3] = c33;

            CATCH_REQUIRE(c[0][0] == c00);
            CATCH_REQUIRE(c[0][1] == c01);
            CATCH_REQUIRE(c[0][2] == c02);
            CATCH_REQUIRE(c[0][3] == c03);
            CATCH_REQUIRE(c[1][0] == c10);
            CATCH_REQUIRE(c[1][1] == c11);
            CATCH_REQUIRE(c[1][2] == c12);
            CATCH_REQUIRE(c[1][3] == c13);
            CATCH_REQUIRE(c[2][0] == c20);
            CATCH_REQUIRE(c[2][1] == c21);
            CATCH_REQUIRE(c[2][2] == c22);
            CATCH_REQUIRE(c[2][3] == c23);
            CATCH_REQUIRE(c[3][0] == c30);
            CATCH_REQUIRE(c[3][1] == c31);
            CATCH_REQUIRE(c[3][2] == c32);
            CATCH_REQUIRE(c[3][3] == c33);

            // run operation C = A + B
            //
            c = a + b;

            CATCH_REQUIRE(a[0][0] == a00);
            CATCH_REQUIRE(a[0][1] == a01);
            CATCH_REQUIRE(a[0][2] == a02);
            CATCH_REQUIRE(a[0][3] == a03);
            CATCH_REQUIRE(a[1][0] == a10);
            CATCH_REQUIRE(a[1][1] == a11);
            CATCH_REQUIRE(a[1][2] == a12);
            CATCH_REQUIRE(a[1][3] == a13);
            CATCH_REQUIRE(a[2][0] == a20);
            CATCH_REQUIRE(a[2][1] == a21);
            CATCH_REQUIRE(a[2][2] == a22);
            CATCH_REQUIRE(a[2][3] == a23);
            CATCH_REQUIRE(a[3][0] == a30);
            CATCH_REQUIRE(a[3][1] == a31);
            CATCH_REQUIRE(a[3][2] == a32);
            CATCH_REQUIRE(a[3][3] == a33);

            CATCH_REQUIRE(b[0][0] == b00);
            CATCH_REQUIRE(b[0][1] == b01);
            CATCH_REQUIRE(b[0][2] == b02);
            CATCH_REQUIRE(b[0][3] == b03);
            CATCH_REQUIRE(b[1][0] == b10);
            CATCH_REQUIRE(b[1][1] == b11);
            CATCH_REQUIRE(b[1][2] == b12);
            CATCH_REQUIRE(b[1][3] == b13);
            CATCH_REQUIRE(b[2][0] == b20);
            CATCH_REQUIRE(b[2][1] == b21);
            CATCH_REQUIRE(b[2][2] == b22);
            CATCH_REQUIRE(b[2][3] == b23);
            CATCH_REQUIRE(b[3][0] == b30);
            CATCH_REQUIRE(b[3][1] == b31);
            CATCH_REQUIRE(b[3][2] == b32);
            CATCH_REQUIRE(b[3][3] == b33);

            // this could fail
            //CATCH_REQUIRE_FALSE(c[0][0] == c00);
            //CATCH_REQUIRE_FALSE(c[0][1] == c01);
            //CATCH_REQUIRE_FALSE(c[0][2] == c02);
            //CATCH_REQUIRE_FALSE(c[0][3] == c03);
            //CATCH_REQUIRE_FALSE(c[1][0] == c10);
            //CATCH_REQUIRE_FALSE(c[1][1] == c11);
            //CATCH_REQUIRE_FALSE(c[1][2] == c12);
            //CATCH_REQUIRE_FALSE(c[1][3] == c13);
            //CATCH_REQUIRE_FALSE(c[2][0] == c20);
            //CATCH_REQUIRE_FALSE(c[2][1] == c21);
            //CATCH_REQUIRE_FALSE(c[2][2] == c22);
            //CATCH_REQUIRE_FALSE(c[2][3] == c23);
            //CATCH_REQUIRE_FALSE(c[3][0] == c30);
            //CATCH_REQUIRE_FALSE(c[3][1] == c31);
            //CATCH_REQUIRE_FALSE(c[3][2] == c32);
            //CATCH_REQUIRE_FALSE(c[3][3] == c33);

            CATCH_REQUIRE(c[0][0] == a00 + b00);
            CATCH_REQUIRE(c[0][1] == a01 + b01);
            CATCH_REQUIRE(c[0][2] == a02 + b02);
            CATCH_REQUIRE(c[0][3] == a03 + b03);
            CATCH_REQUIRE(c[1][0] == a10 + b10);
            CATCH_REQUIRE(c[1][1] == a11 + b11);
            CATCH_REQUIRE(c[1][2] == a12 + b12);
            CATCH_REQUIRE(c[1][3] == a13 + b13);
            CATCH_REQUIRE(c[2][0] == a20 + b20);
            CATCH_REQUIRE(c[2][1] == a21 + b21);
            CATCH_REQUIRE(c[2][2] == a22 + b22);
            CATCH_REQUIRE(c[2][3] == a23 + b23);
            CATCH_REQUIRE(c[3][0] == a30 + b30);
            CATCH_REQUIRE(c[3][1] == a31 + b31);
            CATCH_REQUIRE(c[3][2] == a32 + b32);
            CATCH_REQUIRE(c[3][3] == a33 + b33);
        }
        CATCH_END_SECTION()

        CATCH_START_SECTION("matrix: a+=b")
        {
            // setup A
            //
            snapdev::matrix<double> a(4, 4);

            CATCH_REQUIRE(a.rows() == 4);
            CATCH_REQUIRE(a.columns() == 4);

            double const a00(frand());
            double const a01(frand());
            double const a02(frand());
            double const a03(frand());
            double const a10(frand());
            double const a11(frand());
            double const a12(frand());
            double const a13(frand());
            double const a20(frand());
            double const a21(frand());
            double const a22(frand());
            double const a23(frand());
            double const a30(frand());
            double const a31(frand());
            double const a32(frand());
            double const a33(frand());

            a[0][0] = a00;
            a[0][1] = a01;
            a[0][2] = a02;
            a[0][3] = a03;
            a[1][0] = a10;
            a[1][1] = a11;
            a[1][2] = a12;
            a[1][3] = a13;
            a[2][0] = a20;
            a[2][1] = a21;
            a[2][2] = a22;
            a[2][3] = a23;
            a[3][0] = a30;
            a[3][1] = a31;
            a[3][2] = a32;
            a[3][3] = a33;

            CATCH_REQUIRE(a[0][0] == a00);
            CATCH_REQUIRE(a[0][1] == a01);
            CATCH_REQUIRE(a[0][2] == a02);
            CATCH_REQUIRE(a[0][3] == a03);
            CATCH_REQUIRE(a[1][0] == a10);
            CATCH_REQUIRE(a[1][1] == a11);
            CATCH_REQUIRE(a[1][2] == a12);
            CATCH_REQUIRE(a[1][3] == a13);
            CATCH_REQUIRE(a[2][0] == a20);
            CATCH_REQUIRE(a[2][1] == a21);
            CATCH_REQUIRE(a[2][2] == a22);
            CATCH_REQUIRE(a[2][3] == a23);
            CATCH_REQUIRE(a[3][0] == a30);
            CATCH_REQUIRE(a[3][1] == a31);
            CATCH_REQUIRE(a[3][2] == a32);
            CATCH_REQUIRE(a[3][3] == a33);

            // setup B
            //
            snapdev::matrix<double> b(4, 4);

            CATCH_REQUIRE(b.rows() == 4);
            CATCH_REQUIRE(b.columns() == 4);

            double const b00(frand());
            double const b01(frand());
            double const b02(frand());
            double const b03(frand());
            double const b10(frand());
            double const b11(frand());
            double const b12(frand());
            double const b13(frand());
            double const b20(frand());
            double const b21(frand());
            double const b22(frand());
            double const b23(frand());
            double const b30(frand());
            double const b31(frand());
            double const b32(frand());
            double const b33(frand());

            b[0][0] = b00;
            b[0][1] = b01;
            b[0][2] = b02;
            b[0][3] = b03;
            b[1][0] = b10;
            b[1][1] = b11;
            b[1][2] = b12;
            b[1][3] = b13;
            b[2][0] = b20;
            b[2][1] = b21;
            b[2][2] = b22;
            b[2][3] = b23;
            b[3][0] = b30;
            b[3][1] = b31;
            b[3][2] = b32;
            b[3][3] = b33;

            CATCH_REQUIRE(b[0][0] == b00);
            CATCH_REQUIRE(b[0][1] == b01);
            CATCH_REQUIRE(b[0][2] == b02);
            CATCH_REQUIRE(b[0][3] == b03);
            CATCH_REQUIRE(b[1][0] == b10);
            CATCH_REQUIRE(b[1][1] == b11);
            CATCH_REQUIRE(b[1][2] == b12);
            CATCH_REQUIRE(b[1][3] == b13);
            CATCH_REQUIRE(b[2][0] == b20);
            CATCH_REQUIRE(b[2][1] == b21);
            CATCH_REQUIRE(b[2][2] == b22);
            CATCH_REQUIRE(b[2][3] == b23);
            CATCH_REQUIRE(b[3][0] == b30);
            CATCH_REQUIRE(b[3][1] == b31);
            CATCH_REQUIRE(b[3][2] == b32);
            CATCH_REQUIRE(b[3][3] == b33);

            // run operation A += B
            //
            a += b;

            // this could fail if any bXX is 0.0
            //
            //CATCH_REQUIRE_FALSE(a[0][0] == a00);
            //CATCH_REQUIRE_FALSE(a[0][1] == a01);
            //CATCH_REQUIRE_FALSE(a[0][2] == a02);
            //CATCH_REQUIRE_FALSE(a[0][3] == a03);
            //CATCH_REQUIRE_FALSE(a[1][0] == a10);
            //CATCH_REQUIRE_FALSE(a[1][1] == a11);
            //CATCH_REQUIRE_FALSE(a[1][2] == a12);
            //CATCH_REQUIRE_FALSE(a[1][3] == a13);
            //CATCH_REQUIRE_FALSE(a[2][0] == a20);
            //CATCH_REQUIRE_FALSE(a[2][1] == a21);
            //CATCH_REQUIRE_FALSE(a[2][2] == a22);
            //CATCH_REQUIRE_FALSE(a[2][3] == a23);
            //CATCH_REQUIRE_FALSE(a[3][0] == a30);
            //CATCH_REQUIRE_FALSE(a[3][1] == a31);
            //CATCH_REQUIRE_FALSE(a[3][2] == a32);
            //CATCH_REQUIRE_FALSE(a[3][3] == a33);

            CATCH_REQUIRE(b[0][0] == b00);
            CATCH_REQUIRE(b[0][1] == b01);
            CATCH_REQUIRE(b[0][2] == b02);
            CATCH_REQUIRE(b[0][3] == b03);
            CATCH_REQUIRE(b[1][0] == b10);
            CATCH_REQUIRE(b[1][1] == b11);
            CATCH_REQUIRE(b[1][2] == b12);
            CATCH_REQUIRE(b[1][3] == b13);
            CATCH_REQUIRE(b[2][0] == b20);
            CATCH_REQUIRE(b[2][1] == b21);
            CATCH_REQUIRE(b[2][2] == b22);
            CATCH_REQUIRE(b[2][3] == b23);
            CATCH_REQUIRE(b[3][0] == b30);
            CATCH_REQUIRE(b[3][1] == b31);
            CATCH_REQUIRE(b[3][2] == b32);
            CATCH_REQUIRE(b[3][3] == b33);

            CATCH_REQUIRE(a[0][0] == a00 + b00);
            CATCH_REQUIRE(a[0][1] == a01 + b01);
            CATCH_REQUIRE(a[0][2] == a02 + b02);
            CATCH_REQUIRE(a[0][3] == a03 + b03);
            CATCH_REQUIRE(a[1][0] == a10 + b10);
            CATCH_REQUIRE(a[1][1] == a11 + b11);
            CATCH_REQUIRE(a[1][2] == a12 + b12);
            CATCH_REQUIRE(a[1][3] == a13 + b13);
            CATCH_REQUIRE(a[2][0] == a20 + b20);
            CATCH_REQUIRE(a[2][1] == a21 + b21);
            CATCH_REQUIRE(a[2][2] == a22 + b22);
            CATCH_REQUIRE(a[2][3] == a23 + b23);
            CATCH_REQUIRE(a[3][0] == a30 + b30);
            CATCH_REQUIRE(a[3][1] == a31 + b31);
            CATCH_REQUIRE(a[3][2] == a32 + b32);
            CATCH_REQUIRE(a[3][3] == a33 + b33);
        }
        CATCH_END_SECTION()
    }

    // create two random 4x4 matrices and make sure the add works
    //
    CATCH_GIVEN("sub")
    {
        CATCH_START_SECTION("matrix: b=a-<scalar>")
        {
            // setup A
            //
            snapdev::matrix<double> a(4, 4);

            CATCH_REQUIRE(a.rows() == 4);
            CATCH_REQUIRE(a.columns() == 4);

            double const a00(frand());
            double const a01(frand());
            double const a02(frand());
            double const a03(frand());
            double const a10(frand());
            double const a11(frand());
            double const a12(frand());
            double const a13(frand());
            double const a20(frand());
            double const a21(frand());
            double const a22(frand());
            double const a23(frand());
            double const a30(frand());
            double const a31(frand());
            double const a32(frand());
            double const a33(frand());

            a[0][0] = a00;
            a[0][1] = a01;
            a[0][2] = a02;
            a[0][3] = a03;
            a[1][0] = a10;
            a[1][1] = a11;
            a[1][2] = a12;
            a[1][3] = a13;
            a[2][0] = a20;
            a[2][1] = a21;
            a[2][2] = a22;
            a[2][3] = a23;
            a[3][0] = a30;
            a[3][1] = a31;
            a[3][2] = a32;
            a[3][3] = a33;

            CATCH_REQUIRE(a[0][0] == a00);
            CATCH_REQUIRE(a[0][1] == a01);
            CATCH_REQUIRE(a[0][2] == a02);
            CATCH_REQUIRE(a[0][3] == a03);
            CATCH_REQUIRE(a[1][0] == a10);
            CATCH_REQUIRE(a[1][1] == a11);
            CATCH_REQUIRE(a[1][2] == a12);
            CATCH_REQUIRE(a[1][3] == a13);
            CATCH_REQUIRE(a[2][0] == a20);
            CATCH_REQUIRE(a[2][1] == a21);
            CATCH_REQUIRE(a[2][2] == a22);
            CATCH_REQUIRE(a[2][3] == a23);
            CATCH_REQUIRE(a[3][0] == a30);
            CATCH_REQUIRE(a[3][1] == a31);
            CATCH_REQUIRE(a[3][2] == a32);
            CATCH_REQUIRE(a[3][3] == a33);

            // setup B
            //
            snapdev::matrix<double> b(4, 4);

            CATCH_REQUIRE(b.rows() == 4);
            CATCH_REQUIRE(b.columns() == 4);

            double const b00(frand());
            double const b01(frand());
            double const b02(frand());
            double const b03(frand());
            double const b10(frand());
            double const b11(frand());
            double const b12(frand());
            double const b13(frand());
            double const b20(frand());
            double const b21(frand());
            double const b22(frand());
            double const b23(frand());
            double const b30(frand());
            double const b31(frand());
            double const b32(frand());
            double const b33(frand());

            b[0][0] = b00;
            b[0][1] = b01;
            b[0][2] = b02;
            b[0][3] = b03;
            b[1][0] = b10;
            b[1][1] = b11;
            b[1][2] = b12;
            b[1][3] = b13;
            b[2][0] = b20;
            b[2][1] = b21;
            b[2][2] = b22;
            b[2][3] = b23;
            b[3][0] = b30;
            b[3][1] = b31;
            b[3][2] = b32;
            b[3][3] = b33;

            CATCH_REQUIRE(b[0][0] == b00);
            CATCH_REQUIRE(b[0][1] == b01);
            CATCH_REQUIRE(b[0][2] == b02);
            CATCH_REQUIRE(b[0][3] == b03);
            CATCH_REQUIRE(b[1][0] == b10);
            CATCH_REQUIRE(b[1][1] == b11);
            CATCH_REQUIRE(b[1][2] == b12);
            CATCH_REQUIRE(b[1][3] == b13);
            CATCH_REQUIRE(b[2][0] == b20);
            CATCH_REQUIRE(b[2][1] == b21);
            CATCH_REQUIRE(b[2][2] == b22);
            CATCH_REQUIRE(b[2][3] == b23);
            CATCH_REQUIRE(b[3][0] == b30);
            CATCH_REQUIRE(b[3][1] == b31);
            CATCH_REQUIRE(b[3][2] == b32);
            CATCH_REQUIRE(b[3][3] == b33);

            // create a scalar for our test
            //
            double scalar(frand());
            while(scalar == 0.0)
            {
                scalar = frand();
            }

            // run operation B = A - <scalar>
            //
            b = a - scalar;

            CATCH_REQUIRE(a[0][0] == a00);
            CATCH_REQUIRE(a[0][1] == a01);
            CATCH_REQUIRE(a[0][2] == a02);
            CATCH_REQUIRE(a[0][3] == a03);
            CATCH_REQUIRE(a[1][0] == a10);
            CATCH_REQUIRE(a[1][1] == a11);
            CATCH_REQUIRE(a[1][2] == a12);
            CATCH_REQUIRE(a[1][3] == a13);
            CATCH_REQUIRE(a[2][0] == a20);
            CATCH_REQUIRE(a[2][1] == a21);
            CATCH_REQUIRE(a[2][2] == a22);
            CATCH_REQUIRE(a[2][3] == a23);
            CATCH_REQUIRE(a[3][0] == a30);
            CATCH_REQUIRE(a[3][1] == a31);
            CATCH_REQUIRE(a[3][2] == a32);
            CATCH_REQUIRE(a[3][3] == a33);

            // this can't fail because we ensure scalar != 0
            //
            CATCH_REQUIRE_FALSE(b[0][0] == b00);
            CATCH_REQUIRE_FALSE(b[0][1] == b01);
            CATCH_REQUIRE_FALSE(b[0][2] == b02);
            CATCH_REQUIRE_FALSE(b[0][3] == b03);
            CATCH_REQUIRE_FALSE(b[1][0] == b10);
            CATCH_REQUIRE_FALSE(b[1][1] == b11);
            CATCH_REQUIRE_FALSE(b[1][2] == b12);
            CATCH_REQUIRE_FALSE(b[1][3] == b13);
            CATCH_REQUIRE_FALSE(b[2][0] == b20);
            CATCH_REQUIRE_FALSE(b[2][1] == b21);
            CATCH_REQUIRE_FALSE(b[2][2] == b22);
            CATCH_REQUIRE_FALSE(b[2][3] == b23);
            CATCH_REQUIRE_FALSE(b[3][0] == b30);
            CATCH_REQUIRE_FALSE(b[3][1] == b31);
            CATCH_REQUIRE_FALSE(b[3][2] == b32);
            CATCH_REQUIRE_FALSE(b[3][3] == b33);

            CATCH_REQUIRE(b[0][0] == a00 - scalar);
            CATCH_REQUIRE(b[0][1] == a01 - scalar);
            CATCH_REQUIRE(b[0][2] == a02 - scalar);
            CATCH_REQUIRE(b[0][3] == a03 - scalar);
            CATCH_REQUIRE(b[1][0] == a10 - scalar);
            CATCH_REQUIRE(b[1][1] == a11 - scalar);
            CATCH_REQUIRE(b[1][2] == a12 - scalar);
            CATCH_REQUIRE(b[1][3] == a13 - scalar);
            CATCH_REQUIRE(b[2][0] == a20 - scalar);
            CATCH_REQUIRE(b[2][1] == a21 - scalar);
            CATCH_REQUIRE(b[2][2] == a22 - scalar);
            CATCH_REQUIRE(b[2][3] == a23 - scalar);
            CATCH_REQUIRE(b[3][0] == a30 - scalar);
            CATCH_REQUIRE(b[3][1] == a31 - scalar);
            CATCH_REQUIRE(b[3][2] == a32 - scalar);
            CATCH_REQUIRE(b[3][3] == a33 - scalar);
        }
        CATCH_END_SECTION()

        CATCH_START_SECTION("matrix: a-=<scalar>")
        {
            // setup A
            //
            snapdev::matrix<double> a(4, 4);

            CATCH_REQUIRE(a.rows() == 4);
            CATCH_REQUIRE(a.columns() == 4);

            double const a00(frand());
            double const a01(frand());
            double const a02(frand());
            double const a03(frand());
            double const a10(frand());
            double const a11(frand());
            double const a12(frand());
            double const a13(frand());
            double const a20(frand());
            double const a21(frand());
            double const a22(frand());
            double const a23(frand());
            double const a30(frand());
            double const a31(frand());
            double const a32(frand());
            double const a33(frand());

            a[0][0] = a00;
            a[0][1] = a01;
            a[0][2] = a02;
            a[0][3] = a03;
            a[1][0] = a10;
            a[1][1] = a11;
            a[1][2] = a12;
            a[1][3] = a13;
            a[2][0] = a20;
            a[2][1] = a21;
            a[2][2] = a22;
            a[2][3] = a23;
            a[3][0] = a30;
            a[3][1] = a31;
            a[3][2] = a32;
            a[3][3] = a33;

            CATCH_REQUIRE(a[0][0] == a00);
            CATCH_REQUIRE(a[0][1] == a01);
            CATCH_REQUIRE(a[0][2] == a02);
            CATCH_REQUIRE(a[0][3] == a03);
            CATCH_REQUIRE(a[1][0] == a10);
            CATCH_REQUIRE(a[1][1] == a11);
            CATCH_REQUIRE(a[1][2] == a12);
            CATCH_REQUIRE(a[1][3] == a13);
            CATCH_REQUIRE(a[2][0] == a20);
            CATCH_REQUIRE(a[2][1] == a21);
            CATCH_REQUIRE(a[2][2] == a22);
            CATCH_REQUIRE(a[2][3] == a23);
            CATCH_REQUIRE(a[3][0] == a30);
            CATCH_REQUIRE(a[3][1] == a31);
            CATCH_REQUIRE(a[3][2] == a32);
            CATCH_REQUIRE(a[3][3] == a33);

            // create a scalar for our test
            //
            double scalar(frand());
            while(scalar == 0.0)
            {
                scalar = frand();
            }

            // run operation A -= <scalar>
            //
            a -= scalar;

            // this can't fail because we ensure scalar != 0
            //
            CATCH_REQUIRE_FALSE(a[0][0] == a00);
            CATCH_REQUIRE_FALSE(a[0][1] == a01);
            CATCH_REQUIRE_FALSE(a[0][2] == a02);
            CATCH_REQUIRE_FALSE(a[0][3] == a03);
            CATCH_REQUIRE_FALSE(a[1][0] == a10);
            CATCH_REQUIRE_FALSE(a[1][1] == a11);
            CATCH_REQUIRE_FALSE(a[1][2] == a12);
            CATCH_REQUIRE_FALSE(a[1][3] == a13);
            CATCH_REQUIRE_FALSE(a[2][0] == a20);
            CATCH_REQUIRE_FALSE(a[2][1] == a21);
            CATCH_REQUIRE_FALSE(a[2][2] == a22);
            CATCH_REQUIRE_FALSE(a[2][3] == a23);
            CATCH_REQUIRE_FALSE(a[3][0] == a30);
            CATCH_REQUIRE_FALSE(a[3][1] == a31);
            CATCH_REQUIRE_FALSE(a[3][2] == a32);
            CATCH_REQUIRE_FALSE(a[3][3] == a33);

            CATCH_REQUIRE(a[0][0] == a00 - scalar);
            CATCH_REQUIRE(a[0][1] == a01 - scalar);
            CATCH_REQUIRE(a[0][2] == a02 - scalar);
            CATCH_REQUIRE(a[0][3] == a03 - scalar);
            CATCH_REQUIRE(a[1][0] == a10 - scalar);
            CATCH_REQUIRE(a[1][1] == a11 - scalar);
            CATCH_REQUIRE(a[1][2] == a12 - scalar);
            CATCH_REQUIRE(a[1][3] == a13 - scalar);
            CATCH_REQUIRE(a[2][0] == a20 - scalar);
            CATCH_REQUIRE(a[2][1] == a21 - scalar);
            CATCH_REQUIRE(a[2][2] == a22 - scalar);
            CATCH_REQUIRE(a[2][3] == a23 - scalar);
            CATCH_REQUIRE(a[3][0] == a30 - scalar);
            CATCH_REQUIRE(a[3][1] == a31 - scalar);
            CATCH_REQUIRE(a[3][2] == a32 - scalar);
            CATCH_REQUIRE(a[3][3] == a33 - scalar);
        }
        CATCH_END_SECTION()

        CATCH_START_SECTION("matrix: c=a-b")
        {
            // setup A
            //
            snapdev::matrix<double> a(4, 4);

            CATCH_REQUIRE(a.rows() == 4);
            CATCH_REQUIRE(a.columns() == 4);

            double const a00(frand());
            double const a01(frand());
            double const a02(frand());
            double const a03(frand());
            double const a10(frand());
            double const a11(frand());
            double const a12(frand());
            double const a13(frand());
            double const a20(frand());
            double const a21(frand());
            double const a22(frand());
            double const a23(frand());
            double const a30(frand());
            double const a31(frand());
            double const a32(frand());
            double const a33(frand());

            a[0][0] = a00;
            a[0][1] = a01;
            a[0][2] = a02;
            a[0][3] = a03;
            a[1][0] = a10;
            a[1][1] = a11;
            a[1][2] = a12;
            a[1][3] = a13;
            a[2][0] = a20;
            a[2][1] = a21;
            a[2][2] = a22;
            a[2][3] = a23;
            a[3][0] = a30;
            a[3][1] = a31;
            a[3][2] = a32;
            a[3][3] = a33;

            CATCH_REQUIRE(a[0][0] == a00);
            CATCH_REQUIRE(a[0][1] == a01);
            CATCH_REQUIRE(a[0][2] == a02);
            CATCH_REQUIRE(a[0][3] == a03);
            CATCH_REQUIRE(a[1][0] == a10);
            CATCH_REQUIRE(a[1][1] == a11);
            CATCH_REQUIRE(a[1][2] == a12);
            CATCH_REQUIRE(a[1][3] == a13);
            CATCH_REQUIRE(a[2][0] == a20);
            CATCH_REQUIRE(a[2][1] == a21);
            CATCH_REQUIRE(a[2][2] == a22);
            CATCH_REQUIRE(a[2][3] == a23);
            CATCH_REQUIRE(a[3][0] == a30);
            CATCH_REQUIRE(a[3][1] == a31);
            CATCH_REQUIRE(a[3][2] == a32);
            CATCH_REQUIRE(a[3][3] == a33);

            // setup B
            //
            snapdev::matrix<double> b(4, 4);

            CATCH_REQUIRE(b.rows() == 4);
            CATCH_REQUIRE(b.columns() == 4);

            double const b00(frand());
            double const b01(frand());
            double const b02(frand());
            double const b03(frand());
            double const b10(frand());
            double const b11(frand());
            double const b12(frand());
            double const b13(frand());
            double const b20(frand());
            double const b21(frand());
            double const b22(frand());
            double const b23(frand());
            double const b30(frand());
            double const b31(frand());
            double const b32(frand());
            double const b33(frand());

            b[0][0] = b00;
            b[0][1] = b01;
            b[0][2] = b02;
            b[0][3] = b03;
            b[1][0] = b10;
            b[1][1] = b11;
            b[1][2] = b12;
            b[1][3] = b13;
            b[2][0] = b20;
            b[2][1] = b21;
            b[2][2] = b22;
            b[2][3] = b23;
            b[3][0] = b30;
            b[3][1] = b31;
            b[3][2] = b32;
            b[3][3] = b33;

            CATCH_REQUIRE(b[0][0] == b00);
            CATCH_REQUIRE(b[0][1] == b01);
            CATCH_REQUIRE(b[0][2] == b02);
            CATCH_REQUIRE(b[0][3] == b03);
            CATCH_REQUIRE(b[1][0] == b10);
            CATCH_REQUIRE(b[1][1] == b11);
            CATCH_REQUIRE(b[1][2] == b12);
            CATCH_REQUIRE(b[1][3] == b13);
            CATCH_REQUIRE(b[2][0] == b20);
            CATCH_REQUIRE(b[2][1] == b21);
            CATCH_REQUIRE(b[2][2] == b22);
            CATCH_REQUIRE(b[2][3] == b23);
            CATCH_REQUIRE(b[3][0] == b30);
            CATCH_REQUIRE(b[3][1] == b31);
            CATCH_REQUIRE(b[3][2] == b32);
            CATCH_REQUIRE(b[3][3] == b33);

            // setup C
            //
            snapdev::matrix<double> c(4, 4);

            CATCH_REQUIRE(c.rows() == 4);
            CATCH_REQUIRE(c.columns() == 4);

            double const c00(frand());
            double const c01(frand());
            double const c02(frand());
            double const c03(frand());
            double const c10(frand());
            double const c11(frand());
            double const c12(frand());
            double const c13(frand());
            double const c20(frand());
            double const c21(frand());
            double const c22(frand());
            double const c23(frand());
            double const c30(frand());
            double const c31(frand());
            double const c32(frand());
            double const c33(frand());

            c[0][0] = c00;
            c[0][1] = c01;
            c[0][2] = c02;
            c[0][3] = c03;
            c[1][0] = c10;
            c[1][1] = c11;
            c[1][2] = c12;
            c[1][3] = c13;
            c[2][0] = c20;
            c[2][1] = c21;
            c[2][2] = c22;
            c[2][3] = c23;
            c[3][0] = c30;
            c[3][1] = c31;
            c[3][2] = c32;
            c[3][3] = c33;

            CATCH_REQUIRE(c[0][0] == c00);
            CATCH_REQUIRE(c[0][1] == c01);
            CATCH_REQUIRE(c[0][2] == c02);
            CATCH_REQUIRE(c[0][3] == c03);
            CATCH_REQUIRE(c[1][0] == c10);
            CATCH_REQUIRE(c[1][1] == c11);
            CATCH_REQUIRE(c[1][2] == c12);
            CATCH_REQUIRE(c[1][3] == c13);
            CATCH_REQUIRE(c[2][0] == c20);
            CATCH_REQUIRE(c[2][1] == c21);
            CATCH_REQUIRE(c[2][2] == c22);
            CATCH_REQUIRE(c[2][3] == c23);
            CATCH_REQUIRE(c[3][0] == c30);
            CATCH_REQUIRE(c[3][1] == c31);
            CATCH_REQUIRE(c[3][2] == c32);
            CATCH_REQUIRE(c[3][3] == c33);

            // run operation C = A - B
            //
            c = a - b;

            CATCH_REQUIRE(a[0][0] == a00);
            CATCH_REQUIRE(a[0][1] == a01);
            CATCH_REQUIRE(a[0][2] == a02);
            CATCH_REQUIRE(a[0][3] == a03);
            CATCH_REQUIRE(a[1][0] == a10);
            CATCH_REQUIRE(a[1][1] == a11);
            CATCH_REQUIRE(a[1][2] == a12);
            CATCH_REQUIRE(a[1][3] == a13);
            CATCH_REQUIRE(a[2][0] == a20);
            CATCH_REQUIRE(a[2][1] == a21);
            CATCH_REQUIRE(a[2][2] == a22);
            CATCH_REQUIRE(a[2][3] == a23);
            CATCH_REQUIRE(a[3][0] == a30);
            CATCH_REQUIRE(a[3][1] == a31);
            CATCH_REQUIRE(a[3][2] == a32);
            CATCH_REQUIRE(a[3][3] == a33);

            CATCH_REQUIRE(b[0][0] == b00);
            CATCH_REQUIRE(b[0][1] == b01);
            CATCH_REQUIRE(b[0][2] == b02);
            CATCH_REQUIRE(b[0][3] == b03);
            CATCH_REQUIRE(b[1][0] == b10);
            CATCH_REQUIRE(b[1][1] == b11);
            CATCH_REQUIRE(b[1][2] == b12);
            CATCH_REQUIRE(b[1][3] == b13);
            CATCH_REQUIRE(b[2][0] == b20);
            CATCH_REQUIRE(b[2][1] == b21);
            CATCH_REQUIRE(b[2][2] == b22);
            CATCH_REQUIRE(b[2][3] == b23);
            CATCH_REQUIRE(b[3][0] == b30);
            CATCH_REQUIRE(b[3][1] == b31);
            CATCH_REQUIRE(b[3][2] == b32);
            CATCH_REQUIRE(b[3][3] == b33);

            // this could fail
            //CATCH_REQUIRE_FALSE(c[0][0] == c00);
            //CATCH_REQUIRE_FALSE(c[0][1] == c01);
            //CATCH_REQUIRE_FALSE(c[0][2] == c02);
            //CATCH_REQUIRE_FALSE(c[0][3] == c03);
            //CATCH_REQUIRE_FALSE(c[1][0] == c10);
            //CATCH_REQUIRE_FALSE(c[1][1] == c11);
            //CATCH_REQUIRE_FALSE(c[1][2] == c12);
            //CATCH_REQUIRE_FALSE(c[1][3] == c13);
            //CATCH_REQUIRE_FALSE(c[2][0] == c20);
            //CATCH_REQUIRE_FALSE(c[2][1] == c21);
            //CATCH_REQUIRE_FALSE(c[2][2] == c22);
            //CATCH_REQUIRE_FALSE(c[2][3] == c23);
            //CATCH_REQUIRE_FALSE(c[3][0] == c30);
            //CATCH_REQUIRE_FALSE(c[3][1] == c31);
            //CATCH_REQUIRE_FALSE(c[3][2] == c32);
            //CATCH_REQUIRE_FALSE(c[3][3] == c33);

            CATCH_REQUIRE(c[0][0] == a00 - b00);
            CATCH_REQUIRE(c[0][1] == a01 - b01);
            CATCH_REQUIRE(c[0][2] == a02 - b02);
            CATCH_REQUIRE(c[0][3] == a03 - b03);
            CATCH_REQUIRE(c[1][0] == a10 - b10);
            CATCH_REQUIRE(c[1][1] == a11 - b11);
            CATCH_REQUIRE(c[1][2] == a12 - b12);
            CATCH_REQUIRE(c[1][3] == a13 - b13);
            CATCH_REQUIRE(c[2][0] == a20 - b20);
            CATCH_REQUIRE(c[2][1] == a21 - b21);
            CATCH_REQUIRE(c[2][2] == a22 - b22);
            CATCH_REQUIRE(c[2][3] == a23 - b23);
            CATCH_REQUIRE(c[3][0] == a30 - b30);
            CATCH_REQUIRE(c[3][1] == a31 - b31);
            CATCH_REQUIRE(c[3][2] == a32 - b32);
            CATCH_REQUIRE(c[3][3] == a33 - b33);
        }
        CATCH_END_SECTION()

        CATCH_START_SECTION("matrix: a-=b")
        {
            // setup A
            //
            snapdev::matrix<double> a(4, 4);

            CATCH_REQUIRE(a.rows() == 4);
            CATCH_REQUIRE(a.columns() == 4);

            double const a00(frand());
            double const a01(frand());
            double const a02(frand());
            double const a03(frand());
            double const a10(frand());
            double const a11(frand());
            double const a12(frand());
            double const a13(frand());
            double const a20(frand());
            double const a21(frand());
            double const a22(frand());
            double const a23(frand());
            double const a30(frand());
            double const a31(frand());
            double const a32(frand());
            double const a33(frand());

            a[0][0] = a00;
            a[0][1] = a01;
            a[0][2] = a02;
            a[0][3] = a03;
            a[1][0] = a10;
            a[1][1] = a11;
            a[1][2] = a12;
            a[1][3] = a13;
            a[2][0] = a20;
            a[2][1] = a21;
            a[2][2] = a22;
            a[2][3] = a23;
            a[3][0] = a30;
            a[3][1] = a31;
            a[3][2] = a32;
            a[3][3] = a33;

            CATCH_REQUIRE(a[0][0] == a00);
            CATCH_REQUIRE(a[0][1] == a01);
            CATCH_REQUIRE(a[0][2] == a02);
            CATCH_REQUIRE(a[0][3] == a03);
            CATCH_REQUIRE(a[1][0] == a10);
            CATCH_REQUIRE(a[1][1] == a11);
            CATCH_REQUIRE(a[1][2] == a12);
            CATCH_REQUIRE(a[1][3] == a13);
            CATCH_REQUIRE(a[2][0] == a20);
            CATCH_REQUIRE(a[2][1] == a21);
            CATCH_REQUIRE(a[2][2] == a22);
            CATCH_REQUIRE(a[2][3] == a23);
            CATCH_REQUIRE(a[3][0] == a30);
            CATCH_REQUIRE(a[3][1] == a31);
            CATCH_REQUIRE(a[3][2] == a32);
            CATCH_REQUIRE(a[3][3] == a33);

            // setup B
            //
            snapdev::matrix<double> b(4, 4);

            CATCH_REQUIRE(b.rows() == 4);
            CATCH_REQUIRE(b.columns() == 4);

            double const b00(frand());
            double const b01(frand());
            double const b02(frand());
            double const b03(frand());
            double const b10(frand());
            double const b11(frand());
            double const b12(frand());
            double const b13(frand());
            double const b20(frand());
            double const b21(frand());
            double const b22(frand());
            double const b23(frand());
            double const b30(frand());
            double const b31(frand());
            double const b32(frand());
            double const b33(frand());

            b[0][0] = b00;
            b[0][1] = b01;
            b[0][2] = b02;
            b[0][3] = b03;
            b[1][0] = b10;
            b[1][1] = b11;
            b[1][2] = b12;
            b[1][3] = b13;
            b[2][0] = b20;
            b[2][1] = b21;
            b[2][2] = b22;
            b[2][3] = b23;
            b[3][0] = b30;
            b[3][1] = b31;
            b[3][2] = b32;
            b[3][3] = b33;

            CATCH_REQUIRE(b[0][0] == b00);
            CATCH_REQUIRE(b[0][1] == b01);
            CATCH_REQUIRE(b[0][2] == b02);
            CATCH_REQUIRE(b[0][3] == b03);
            CATCH_REQUIRE(b[1][0] == b10);
            CATCH_REQUIRE(b[1][1] == b11);
            CATCH_REQUIRE(b[1][2] == b12);
            CATCH_REQUIRE(b[1][3] == b13);
            CATCH_REQUIRE(b[2][0] == b20);
            CATCH_REQUIRE(b[2][1] == b21);
            CATCH_REQUIRE(b[2][2] == b22);
            CATCH_REQUIRE(b[2][3] == b23);
            CATCH_REQUIRE(b[3][0] == b30);
            CATCH_REQUIRE(b[3][1] == b31);
            CATCH_REQUIRE(b[3][2] == b32);
            CATCH_REQUIRE(b[3][3] == b33);

            // run operation A -= B
            //
            a -= b;

            // this could fail if one of bXX is 0.0
            //
            //CATCH_REQUIRE(a[0][0] == a00);
            //CATCH_REQUIRE(a[0][1] == a01);
            //CATCH_REQUIRE(a[0][2] == a02);
            //CATCH_REQUIRE(a[0][3] == a03);
            //CATCH_REQUIRE(a[1][0] == a10);
            //CATCH_REQUIRE(a[1][1] == a11);
            //CATCH_REQUIRE(a[1][2] == a12);
            //CATCH_REQUIRE(a[1][3] == a13);
            //CATCH_REQUIRE(a[2][0] == a20);
            //CATCH_REQUIRE(a[2][1] == a21);
            //CATCH_REQUIRE(a[2][2] == a22);
            //CATCH_REQUIRE(a[2][3] == a23);
            //CATCH_REQUIRE(a[3][0] == a30);
            //CATCH_REQUIRE(a[3][1] == a31);
            //CATCH_REQUIRE(a[3][2] == a32);
            //CATCH_REQUIRE(a[3][3] == a33);

            CATCH_REQUIRE(b[0][0] == b00);
            CATCH_REQUIRE(b[0][1] == b01);
            CATCH_REQUIRE(b[0][2] == b02);
            CATCH_REQUIRE(b[0][3] == b03);
            CATCH_REQUIRE(b[1][0] == b10);
            CATCH_REQUIRE(b[1][1] == b11);
            CATCH_REQUIRE(b[1][2] == b12);
            CATCH_REQUIRE(b[1][3] == b13);
            CATCH_REQUIRE(b[2][0] == b20);
            CATCH_REQUIRE(b[2][1] == b21);
            CATCH_REQUIRE(b[2][2] == b22);
            CATCH_REQUIRE(b[2][3] == b23);
            CATCH_REQUIRE(b[3][0] == b30);
            CATCH_REQUIRE(b[3][1] == b31);
            CATCH_REQUIRE(b[3][2] == b32);
            CATCH_REQUIRE(b[3][3] == b33);

            CATCH_REQUIRE(a[0][0] == a00 - b00);
            CATCH_REQUIRE(a[0][1] == a01 - b01);
            CATCH_REQUIRE(a[0][2] == a02 - b02);
            CATCH_REQUIRE(a[0][3] == a03 - b03);
            CATCH_REQUIRE(a[1][0] == a10 - b10);
            CATCH_REQUIRE(a[1][1] == a11 - b11);
            CATCH_REQUIRE(a[1][2] == a12 - b12);
            CATCH_REQUIRE(a[1][3] == a13 - b13);
            CATCH_REQUIRE(a[2][0] == a20 - b20);
            CATCH_REQUIRE(a[2][1] == a21 - b21);
            CATCH_REQUIRE(a[2][2] == a22 - b22);
            CATCH_REQUIRE(a[2][3] == a23 - b23);
            CATCH_REQUIRE(a[3][0] == a30 - b30);
            CATCH_REQUIRE(a[3][1] == a31 - b31);
            CATCH_REQUIRE(a[3][2] == a32 - b32);
            CATCH_REQUIRE(a[3][3] == a33 - b33);
        }
        CATCH_END_SECTION()
    }
}


CATCH_TEST_CASE("matrix_util", "[matrix][math]")
{
    // various ways to change the data order, minor, enlarge
    //
    CATCH_GIVEN("move data")
    {
        CATCH_START_SECTION("matrix: minor")
        {
            // setup A
            //
            snapdev::matrix<double> a(4, 4);

            CATCH_REQUIRE(a.rows() == 4);
            CATCH_REQUIRE(a.columns() == 4);

            double const a00(frand());
            double const a01(frand());
            double const a02(frand());
            double const a03(frand());
            double const a10(frand());
            double const a11(frand());
            double const a12(frand());
            double const a13(frand());
            double const a20(frand());
            double const a21(frand());
            double const a22(frand());
            double const a23(frand());
            double const a30(frand());
            double const a31(frand());
            double const a32(frand());
            double const a33(frand());

            a[0][0] = a00;
            a[0][1] = a01;
            a[0][2] = a02;
            a[0][3] = a03;
            a[1][0] = a10;
            a[1][1] = a11;
            a[1][2] = a12;
            a[1][3] = a13;
            a[2][0] = a20;
            a[2][1] = a21;
            a[2][2] = a22;
            a[2][3] = a23;
            a[3][0] = a30;
            a[3][1] = a31;
            a[3][2] = a32;
            a[3][3] = a33;

            CATCH_REQUIRE(a[0][0] == a00);
            CATCH_REQUIRE(a[0][1] == a01);
            CATCH_REQUIRE(a[0][2] == a02);
            CATCH_REQUIRE(a[0][3] == a03);
            CATCH_REQUIRE(a[1][0] == a10);
            CATCH_REQUIRE(a[1][1] == a11);
            CATCH_REQUIRE(a[1][2] == a12);
            CATCH_REQUIRE(a[1][3] == a13);
            CATCH_REQUIRE(a[2][0] == a20);
            CATCH_REQUIRE(a[2][1] == a21);
            CATCH_REQUIRE(a[2][2] == a22);
            CATCH_REQUIRE(a[2][3] == a23);
            CATCH_REQUIRE(a[3][0] == a30);
            CATCH_REQUIRE(a[3][1] == a31);
            CATCH_REQUIRE(a[3][2] == a32);
            CATCH_REQUIRE(a[3][3] == a33);

            {
                snapdev::matrix<double> p = a.minor_matrix(0, 0);

                CATCH_REQUIRE(p.rows() == 3);
                CATCH_REQUIRE(p.columns() == 3);

                CATCH_REQUIRE(p[0][0] == a11);
                CATCH_REQUIRE(p[0][1] == a12);
                CATCH_REQUIRE(p[0][2] == a13);
                CATCH_REQUIRE(p[1][0] == a21);
                CATCH_REQUIRE(p[1][1] == a22);
                CATCH_REQUIRE(p[1][2] == a23);
                CATCH_REQUIRE(p[2][0] == a31);
                CATCH_REQUIRE(p[2][1] == a32);
                CATCH_REQUIRE(p[2][2] == a33);
            }

            {
                snapdev::matrix<double> p = a.minor_matrix(0, 1);

                CATCH_REQUIRE(p.rows() == 3);
                CATCH_REQUIRE(p.columns() == 3);

                CATCH_REQUIRE(p[0][0] == a10);
                CATCH_REQUIRE(p[0][1] == a12);
                CATCH_REQUIRE(p[0][2] == a13);
                CATCH_REQUIRE(p[1][0] == a20);
                CATCH_REQUIRE(p[1][1] == a22);
                CATCH_REQUIRE(p[1][2] == a23);
                CATCH_REQUIRE(p[2][0] == a30);
                CATCH_REQUIRE(p[2][1] == a32);
                CATCH_REQUIRE(p[2][2] == a33);
            }

            {
                snapdev::matrix<double> p = a.minor_matrix(0, 2);

                CATCH_REQUIRE(p.rows() == 3);
                CATCH_REQUIRE(p.columns() == 3);

                CATCH_REQUIRE(p[0][0] == a10);
                CATCH_REQUIRE(p[0][1] == a11);
                CATCH_REQUIRE(p[0][2] == a13);
                CATCH_REQUIRE(p[1][0] == a20);
                CATCH_REQUIRE(p[1][1] == a21);
                CATCH_REQUIRE(p[1][2] == a23);
                CATCH_REQUIRE(p[2][0] == a30);
                CATCH_REQUIRE(p[2][1] == a31);
                CATCH_REQUIRE(p[2][2] == a33);
            }

            {
                snapdev::matrix<double> p = a.minor_matrix(0, 3);

                CATCH_REQUIRE(p.rows() == 3);
                CATCH_REQUIRE(p.columns() == 3);

                CATCH_REQUIRE(p[0][0] == a10);
                CATCH_REQUIRE(p[0][1] == a11);
                CATCH_REQUIRE(p[0][2] == a12);
                CATCH_REQUIRE(p[1][0] == a20);
                CATCH_REQUIRE(p[1][1] == a21);
                CATCH_REQUIRE(p[1][2] == a22);
                CATCH_REQUIRE(p[2][0] == a30);
                CATCH_REQUIRE(p[2][1] == a31);
                CATCH_REQUIRE(p[2][2] == a32);
            }

            {
                snapdev::matrix<double> p = a.minor_matrix(2, 1);

                CATCH_REQUIRE(p.rows() == 3);
                CATCH_REQUIRE(p.columns() == 3);

                CATCH_REQUIRE(p[0][0] == a00);
                CATCH_REQUIRE(p[0][1] == a02);
                CATCH_REQUIRE(p[0][2] == a03);
                CATCH_REQUIRE(p[1][0] == a10);
                CATCH_REQUIRE(p[1][1] == a12);
                CATCH_REQUIRE(p[1][2] == a13);
                CATCH_REQUIRE(p[2][0] == a30);
                CATCH_REQUIRE(p[2][1] == a32);
                CATCH_REQUIRE(p[2][2] == a33);
            }
        }
        CATCH_END_SECTION()

        CATCH_START_SECTION("matrix: transpose 4x4")
        {
            // setup A
            //
            snapdev::matrix<double> a(4, 4);

            CATCH_REQUIRE(a.rows() == 4);
            CATCH_REQUIRE(a.columns() == 4);

            double const a00(frand());
            double const a01(frand());
            double const a02(frand());
            double const a03(frand());
            double const a10(frand());
            double const a11(frand());
            double const a12(frand());
            double const a13(frand());
            double const a20(frand());
            double const a21(frand());
            double const a22(frand());
            double const a23(frand());
            double const a30(frand());
            double const a31(frand());
            double const a32(frand());
            double const a33(frand());

            a[0][0] = a00;
            a[0][1] = a01;
            a[0][2] = a02;
            a[0][3] = a03;
            a[1][0] = a10;
            a[1][1] = a11;
            a[1][2] = a12;
            a[1][3] = a13;
            a[2][0] = a20;
            a[2][1] = a21;
            a[2][2] = a22;
            a[2][3] = a23;
            a[3][0] = a30;
            a[3][1] = a31;
            a[3][2] = a32;
            a[3][3] = a33;

            CATCH_REQUIRE(a[0][0] == a00);
            CATCH_REQUIRE(a[0][1] == a01);
            CATCH_REQUIRE(a[0][2] == a02);
            CATCH_REQUIRE(a[0][3] == a03);
            CATCH_REQUIRE(a[1][0] == a10);
            CATCH_REQUIRE(a[1][1] == a11);
            CATCH_REQUIRE(a[1][2] == a12);
            CATCH_REQUIRE(a[1][3] == a13);
            CATCH_REQUIRE(a[2][0] == a20);
            CATCH_REQUIRE(a[2][1] == a21);
            CATCH_REQUIRE(a[2][2] == a22);
            CATCH_REQUIRE(a[2][3] == a23);
            CATCH_REQUIRE(a[3][0] == a30);
            CATCH_REQUIRE(a[3][1] == a31);
            CATCH_REQUIRE(a[3][2] == a32);
            CATCH_REQUIRE(a[3][3] == a33);

            snapdev::matrix<double> t = a.transpose();

            CATCH_REQUIRE(t.rows() == 4);
            CATCH_REQUIRE(t.columns() == 4);

            CATCH_REQUIRE(t[0][0] == a00);
            CATCH_REQUIRE(t[0][1] == a10);
            CATCH_REQUIRE(t[0][2] == a20);
            CATCH_REQUIRE(t[0][3] == a30);
            CATCH_REQUIRE(t[1][0] == a01);
            CATCH_REQUIRE(t[1][1] == a11);
            CATCH_REQUIRE(t[1][2] == a21);
            CATCH_REQUIRE(t[1][3] == a31);
            CATCH_REQUIRE(t[2][0] == a02);
            CATCH_REQUIRE(t[2][1] == a12);
            CATCH_REQUIRE(t[2][2] == a22);
            CATCH_REQUIRE(t[2][3] == a32);
            CATCH_REQUIRE(t[3][0] == a03);
            CATCH_REQUIRE(t[3][1] == a13);
            CATCH_REQUIRE(t[3][2] == a23);
            CATCH_REQUIRE(t[3][3] == a33);
        }
        CATCH_END_SECTION()

        CATCH_START_SECTION("matrix: transpose 6x2")
        {
            // setup A
            //
            snapdev::matrix<double> a(6, 2);

            CATCH_REQUIRE(a.rows() == 6);
            CATCH_REQUIRE(a.columns() == 2);

            double const a00(frand());
            double const a01(frand());
            double const a10(frand());
            double const a11(frand());
            double const a20(frand());
            double const a21(frand());
            double const a30(frand());
            double const a31(frand());
            double const a40(frand());
            double const a41(frand());
            double const a50(frand());
            double const a51(frand());

            a[0][0] = a00;
            a[0][1] = a01;
            a[1][0] = a10;
            a[1][1] = a11;
            a[2][0] = a20;
            a[2][1] = a21;
            a[3][0] = a30;
            a[3][1] = a31;
            a[4][0] = a40;
            a[4][1] = a41;
            a[5][0] = a50;
            a[5][1] = a51;

            CATCH_REQUIRE(a[0][0] == a00);
            CATCH_REQUIRE(a[0][1] == a01);
            CATCH_REQUIRE(a[1][0] == a10);
            CATCH_REQUIRE(a[1][1] == a11);
            CATCH_REQUIRE(a[2][0] == a20);
            CATCH_REQUIRE(a[2][1] == a21);
            CATCH_REQUIRE(a[3][0] == a30);
            CATCH_REQUIRE(a[3][1] == a31);
            CATCH_REQUIRE(a[4][0] == a40);
            CATCH_REQUIRE(a[4][1] == a41);
            CATCH_REQUIRE(a[5][0] == a50);
            CATCH_REQUIRE(a[5][1] == a51);

            snapdev::matrix<double> t = a.transpose();

            CATCH_REQUIRE(t.rows() == 2);
            CATCH_REQUIRE(t.columns() == 6);

            // original
            // a00 a01
            // a10 a11
            // a20 a21
            // a30 a31
            // a40 a41
            // a50 a51
            //
            // transposed
            // a00 a10 a20 a30 a40 a50
            // a01 a11 a21 a31 a41 a51
            //

            CATCH_REQUIRE(t[0][0] == a00);
            CATCH_REQUIRE(t[0][1] == a10);
            CATCH_REQUIRE(t[0][2] == a20);
            CATCH_REQUIRE(t[0][3] == a30);
            CATCH_REQUIRE(t[0][4] == a40);
            CATCH_REQUIRE(t[0][5] == a50);
            CATCH_REQUIRE(t[1][0] == a01);
            CATCH_REQUIRE(t[1][1] == a11);
            CATCH_REQUIRE(t[1][2] == a21);
            CATCH_REQUIRE(t[1][3] == a31);
            CATCH_REQUIRE(t[1][4] == a41);
            CATCH_REQUIRE(t[1][5] == a51);
        }
        CATCH_END_SECTION()

        CATCH_START_SECTION("matrix: adjugate 2x2")
        {
            // setup A
            //
            snapdev::matrix<double> a(2, 2);

            CATCH_REQUIRE(a.rows() == 2);
            CATCH_REQUIRE(a.columns() == 2);

            double const a00(frand());
            double const a01(frand());
            double const a10(frand());
            double const a11(frand());

            a[0][0] = a00;
            a[0][1] = a01;
            a[1][0] = a10;
            a[1][1] = a11;

            CATCH_REQUIRE(a[0][0] == a00);
            CATCH_REQUIRE(a[0][1] == a01);
            CATCH_REQUIRE(a[1][0] == a10);
            CATCH_REQUIRE(a[1][1] == a11);

            snapdev::matrix<double> m = a.adjugate();

            CATCH_REQUIRE(m.rows() == 2);
            CATCH_REQUIRE(m.columns() == 2);

            CATCH_REQUIRE(m[0][0] ==  a11);
            CATCH_REQUIRE(m[0][1] == -a01);
            CATCH_REQUIRE(m[1][0] == -a10);
            CATCH_REQUIRE(m[1][1] ==  a00);
        }
        CATCH_END_SECTION()

        CATCH_START_SECTION("matrix: adjugate 3x3")
        {
            // setup A
            //
            snapdev::matrix<double> a(3, 3);

            CATCH_REQUIRE(a.rows() == 3);
            CATCH_REQUIRE(a.columns() == 3);

            double const a00(frand());
            double const a01(frand());
            double const a02(frand());
            double const a10(frand());
            double const a11(frand());
            double const a12(frand());
            double const a20(frand());
            double const a21(frand());
            double const a22(frand());

            a[0][0] = a00;
            a[0][1] = a01;
            a[0][2] = a02;
            a[1][0] = a10;
            a[1][1] = a11;
            a[1][2] = a12;
            a[2][0] = a20;
            a[2][1] = a21;
            a[2][2] = a22;

            CATCH_REQUIRE(a[0][0] == a00);
            CATCH_REQUIRE(a[0][1] == a01);
            CATCH_REQUIRE(a[0][2] == a02);
            CATCH_REQUIRE(a[1][0] == a10);
            CATCH_REQUIRE(a[1][1] == a11);
            CATCH_REQUIRE(a[1][2] == a12);
            CATCH_REQUIRE(a[2][0] == a20);
            CATCH_REQUIRE(a[2][1] == a21);
            CATCH_REQUIRE(a[2][2] == a22);

            snapdev::matrix<double> m = a.adjugate();

            CATCH_REQUIRE(m.rows() == 3);
            CATCH_REQUIRE(m.columns() == 3);

            {
                snapdev::matrix<double> p(a.minor_matrix(0, 0));
                double const e(m[0][0] - p.determinant());
                CATCH_REQUIRE(fabs(e) < 0.0001);
            }

            {
                snapdev::matrix<double> p(a.minor_matrix(0, 1));
                double const e(m[1][0] + p.determinant());
                CATCH_REQUIRE(fabs(e) < 0.0001);
            }

            {
                snapdev::matrix<double> p(a.minor_matrix(0, 2));
                double const e(m[2][0] - p.determinant());
                CATCH_REQUIRE(fabs(e) < 0.0001);
            }

            {
                snapdev::matrix<double> p(a.minor_matrix(1, 0));
                double const e(m[0][1] + p.determinant());
                CATCH_REQUIRE(fabs(e) < 0.0001);
            }

            {
                snapdev::matrix<double> p(a.minor_matrix(1, 1));
                double const e(m[1][1] - p.determinant());
                CATCH_REQUIRE(fabs(e) < 0.0001);
            }

            {
                snapdev::matrix<double> p(a.minor_matrix(1, 2));
                double const e(m[2][1] + p.determinant());
                CATCH_REQUIRE(fabs(e) < 0.0001);
            }

            {
                snapdev::matrix<double> p(a.minor_matrix(2, 0));
                double const e(m[0][2] - p.determinant());
                CATCH_REQUIRE(fabs(e) < 0.0001);
            }

            {
                snapdev::matrix<double> p(a.minor_matrix(2, 1));
                double const e(m[1][2] + p.determinant());
                CATCH_REQUIRE(fabs(e) < 0.0001);
            }

            {
                snapdev::matrix<double> p(a.minor_matrix(2, 2));
                double const e(m[2][2] - p.determinant());
                CATCH_REQUIRE(fabs(e) < 0.0001);
            }
        }
        CATCH_END_SECTION()
    }

    // calculations such as the determinant
    //
    CATCH_GIVEN("calculations")
    {
        CATCH_START_SECTION("matrix: 2x2 determinant")
        {
            // setup A
            //
            snapdev::matrix<double> a(2, 2);

            CATCH_REQUIRE(a.rows() == 2);
            CATCH_REQUIRE(a.columns() == 2);

            double const a00(frand());
            double const a01(frand());
            double const a10(frand());
            double const a11(frand());

            a[0][0] = a00;
            a[0][1] = a01;
            a[1][0] = a10;
            a[1][1] = a11;

            CATCH_REQUIRE(a[0][0] == a00);
            CATCH_REQUIRE(a[0][1] == a01);
            CATCH_REQUIRE(a[1][0] == a10);
            CATCH_REQUIRE(a[1][1] == a11);

            double d(a.determinant());

            //return f_vector[0 + 0 * 2] * f_vector[1 + 1 * 2]
            //     - f_vector[1 + 0 * 2] * f_vector[0 + 1 * 2];
            double e = a00 * a11 - a10 * a01;

            CATCH_REQUIRE(fabs(d - e) < 0.0001);
        }
        CATCH_END_SECTION()

        CATCH_START_SECTION("matrix: 3x3 determinant with specific data")
        {
            // setup A
            //
            snapdev::matrix<double> a(3, 3);

            CATCH_REQUIRE(a.rows() == 3);
            CATCH_REQUIRE(a.columns() == 3);

            a[0][0] =  5.0;
            a[0][1] = -2.0;
            a[0][2] =  1.0;
            a[1][0] =  0.0;
            a[1][1] =  3.0;
            a[1][2] = -1.0;
            a[2][0] =  2.0;
            a[2][1] =  0.0;
            a[2][2] =  7.0;

            double d(a.determinant());

            // we know that the exact answer for this one is 103
            //
            CATCH_REQUIRE(fabs(d - 103.0) < 0.0001);
        }
        CATCH_END_SECTION()

        CATCH_START_SECTION("matrix: 3x3 determinant with random data")
        {
            // verify 10 times (with different values)
            //
            for(int repeat(0); repeat < 10; ++repeat)
            {
                // setup A
                //
                snapdev::matrix<double> a(3, 3);

                CATCH_REQUIRE(a.rows() == 3);
                CATCH_REQUIRE(a.columns() == 3);

                double const a00(frand());
                double const a01(frand());
                double const a02(frand());
                double const a10(frand());
                double const a11(frand());
                double const a12(frand());
                double const a20(frand());
                double const a21(frand());
                double const a22(frand());

                a[0][0] = a00;
                a[0][1] = a01;
                a[0][2] = a02;
                a[1][0] = a10;
                a[1][1] = a11;
                a[1][2] = a12;
                a[2][0] = a20;
                a[2][1] = a21;
                a[2][2] = a22;

                CATCH_REQUIRE(a[0][0] == a00);
                CATCH_REQUIRE(a[0][1] == a01);
                CATCH_REQUIRE(a[0][2] == a02);
                CATCH_REQUIRE(a[1][0] == a10);
                CATCH_REQUIRE(a[1][1] == a11);
                CATCH_REQUIRE(a[1][2] == a12);
                CATCH_REQUIRE(a[2][0] == a20);
                CATCH_REQUIRE(a[2][1] == a21);
                CATCH_REQUIRE(a[2][2] == a22);

                double d(a.determinant());

                //return f_vector[0 + 0 * 2] * f_vector[1 + 1 * 2]
                //     - f_vector[1 + 0 * 2] * f_vector[0 + 1 * 2];
                double e = a00 * a11 * a22 + a01 * a12 * a20 + a02 * a10 * a21
                         - a00 * a12 * a21 - a01 * a10 * a22 - a02 * a11 * a20;

                CATCH_REQUIRE(fabs(d - e) < 0.0001);
            }
        }
        CATCH_END_SECTION()
    }
}


CATCH_TEST_CASE("matrix_multiplicative", "[matrix][math]")
{
    // create two random 4x4 matrices and make sure the add works
    //
    CATCH_GIVEN("mul")
    {
        CATCH_START_SECTION("matrix: b=a*<scalar>")
        {
            // setup A
            //
            snapdev::matrix<double> a(4, 4);

            CATCH_REQUIRE(a.rows() == 4);
            CATCH_REQUIRE(a.columns() == 4);

            double const a00(frand());
            double const a01(frand());
            double const a02(frand());
            double const a03(frand());
            double const a10(frand());
            double const a11(frand());
            double const a12(frand());
            double const a13(frand());
            double const a20(frand());
            double const a21(frand());
            double const a22(frand());
            double const a23(frand());
            double const a30(frand());
            double const a31(frand());
            double const a32(frand());
            double const a33(frand());

            a[0][0] = a00;
            a[0][1] = a01;
            a[0][2] = a02;
            a[0][3] = a03;
            a[1][0] = a10;
            a[1][1] = a11;
            a[1][2] = a12;
            a[1][3] = a13;
            a[2][0] = a20;
            a[2][1] = a21;
            a[2][2] = a22;
            a[2][3] = a23;
            a[3][0] = a30;
            a[3][1] = a31;
            a[3][2] = a32;
            a[3][3] = a33;

            CATCH_REQUIRE(a[0][0] == a00);
            CATCH_REQUIRE(a[0][1] == a01);
            CATCH_REQUIRE(a[0][2] == a02);
            CATCH_REQUIRE(a[0][3] == a03);
            CATCH_REQUIRE(a[1][0] == a10);
            CATCH_REQUIRE(a[1][1] == a11);
            CATCH_REQUIRE(a[1][2] == a12);
            CATCH_REQUIRE(a[1][3] == a13);
            CATCH_REQUIRE(a[2][0] == a20);
            CATCH_REQUIRE(a[2][1] == a21);
            CATCH_REQUIRE(a[2][2] == a22);
            CATCH_REQUIRE(a[2][3] == a23);
            CATCH_REQUIRE(a[3][0] == a30);
            CATCH_REQUIRE(a[3][1] == a31);
            CATCH_REQUIRE(a[3][2] == a32);
            CATCH_REQUIRE(a[3][3] == a33);

            // setup B
            //
            snapdev::matrix<double> b(4, 4);

            CATCH_REQUIRE(b.rows() == 4);
            CATCH_REQUIRE(b.columns() == 4);

            double const b00(frand());
            double const b01(frand());
            double const b02(frand());
            double const b03(frand());
            double const b10(frand());
            double const b11(frand());
            double const b12(frand());
            double const b13(frand());
            double const b20(frand());
            double const b21(frand());
            double const b22(frand());
            double const b23(frand());
            double const b30(frand());
            double const b31(frand());
            double const b32(frand());
            double const b33(frand());

            b[0][0] = b00;
            b[0][1] = b01;
            b[0][2] = b02;
            b[0][3] = b03;
            b[1][0] = b10;
            b[1][1] = b11;
            b[1][2] = b12;
            b[1][3] = b13;
            b[2][0] = b20;
            b[2][1] = b21;
            b[2][2] = b22;
            b[2][3] = b23;
            b[3][0] = b30;
            b[3][1] = b31;
            b[3][2] = b32;
            b[3][3] = b33;

            CATCH_REQUIRE(b[0][0] == b00);
            CATCH_REQUIRE(b[0][1] == b01);
            CATCH_REQUIRE(b[0][2] == b02);
            CATCH_REQUIRE(b[0][3] == b03);
            CATCH_REQUIRE(b[1][0] == b10);
            CATCH_REQUIRE(b[1][1] == b11);
            CATCH_REQUIRE(b[1][2] == b12);
            CATCH_REQUIRE(b[1][3] == b13);
            CATCH_REQUIRE(b[2][0] == b20);
            CATCH_REQUIRE(b[2][1] == b21);
            CATCH_REQUIRE(b[2][2] == b22);
            CATCH_REQUIRE(b[2][3] == b23);
            CATCH_REQUIRE(b[3][0] == b30);
            CATCH_REQUIRE(b[3][1] == b31);
            CATCH_REQUIRE(b[3][2] == b32);
            CATCH_REQUIRE(b[3][3] == b33);

            // create a scalar for our test
            //
            double scalar(frand());
            while(scalar == 0.0 || scalar == 1.0)
            {
                scalar = frand();
            }

            // run operation B = A + <scalar>
            //
            b = a * scalar;

            CATCH_REQUIRE(a[0][0] == a00);
            CATCH_REQUIRE(a[0][1] == a01);
            CATCH_REQUIRE(a[0][2] == a02);
            CATCH_REQUIRE(a[0][3] == a03);
            CATCH_REQUIRE(a[1][0] == a10);
            CATCH_REQUIRE(a[1][1] == a11);
            CATCH_REQUIRE(a[1][2] == a12);
            CATCH_REQUIRE(a[1][3] == a13);
            CATCH_REQUIRE(a[2][0] == a20);
            CATCH_REQUIRE(a[2][1] == a21);
            CATCH_REQUIRE(a[2][2] == a22);
            CATCH_REQUIRE(a[2][3] == a23);
            CATCH_REQUIRE(a[3][0] == a30);
            CATCH_REQUIRE(a[3][1] == a31);
            CATCH_REQUIRE(a[3][2] == a32);
            CATCH_REQUIRE(a[3][3] == a33);

            // this can't fail because we ensure scalar != 0 or 1
            //
            CATCH_REQUIRE_FALSE(b[0][0] == b00);
            CATCH_REQUIRE_FALSE(b[0][1] == b01);
            CATCH_REQUIRE_FALSE(b[0][2] == b02);
            CATCH_REQUIRE_FALSE(b[0][3] == b03);
            CATCH_REQUIRE_FALSE(b[1][0] == b10);
            CATCH_REQUIRE_FALSE(b[1][1] == b11);
            CATCH_REQUIRE_FALSE(b[1][2] == b12);
            CATCH_REQUIRE_FALSE(b[1][3] == b13);
            CATCH_REQUIRE_FALSE(b[2][0] == b20);
            CATCH_REQUIRE_FALSE(b[2][1] == b21);
            CATCH_REQUIRE_FALSE(b[2][2] == b22);
            CATCH_REQUIRE_FALSE(b[2][3] == b23);
            CATCH_REQUIRE_FALSE(b[3][0] == b30);
            CATCH_REQUIRE_FALSE(b[3][1] == b31);
            CATCH_REQUIRE_FALSE(b[3][2] == b32);
            CATCH_REQUIRE_FALSE(b[3][3] == b33);

            CATCH_REQUIRE(b[0][0] == a00 * scalar);
            CATCH_REQUIRE(b[0][1] == a01 * scalar);
            CATCH_REQUIRE(b[0][2] == a02 * scalar);
            CATCH_REQUIRE(b[0][3] == a03 * scalar);
            CATCH_REQUIRE(b[1][0] == a10 * scalar);
            CATCH_REQUIRE(b[1][1] == a11 * scalar);
            CATCH_REQUIRE(b[1][2] == a12 * scalar);
            CATCH_REQUIRE(b[1][3] == a13 * scalar);
            CATCH_REQUIRE(b[2][0] == a20 * scalar);
            CATCH_REQUIRE(b[2][1] == a21 * scalar);
            CATCH_REQUIRE(b[2][2] == a22 * scalar);
            CATCH_REQUIRE(b[2][3] == a23 * scalar);
            CATCH_REQUIRE(b[3][0] == a30 * scalar);
            CATCH_REQUIRE(b[3][1] == a31 * scalar);
            CATCH_REQUIRE(b[3][2] == a32 * scalar);
            CATCH_REQUIRE(b[3][3] == a33 * scalar);
        }
        CATCH_END_SECTION()

        CATCH_START_SECTION("matrix: a*=<scalar>")
        {
            // setup A
            //
            snapdev::matrix<double> a(4, 4);

            CATCH_REQUIRE(a.rows() == 4);
            CATCH_REQUIRE(a.columns() == 4);

            double const a00(frand());
            double const a01(frand());
            double const a02(frand());
            double const a03(frand());
            double const a10(frand());
            double const a11(frand());
            double const a12(frand());
            double const a13(frand());
            double const a20(frand());
            double const a21(frand());
            double const a22(frand());
            double const a23(frand());
            double const a30(frand());
            double const a31(frand());
            double const a32(frand());
            double const a33(frand());

            a[0][0] = a00;
            a[0][1] = a01;
            a[0][2] = a02;
            a[0][3] = a03;
            a[1][0] = a10;
            a[1][1] = a11;
            a[1][2] = a12;
            a[1][3] = a13;
            a[2][0] = a20;
            a[2][1] = a21;
            a[2][2] = a22;
            a[2][3] = a23;
            a[3][0] = a30;
            a[3][1] = a31;
            a[3][2] = a32;
            a[3][3] = a33;

            CATCH_REQUIRE(a[0][0] == a00);
            CATCH_REQUIRE(a[0][1] == a01);
            CATCH_REQUIRE(a[0][2] == a02);
            CATCH_REQUIRE(a[0][3] == a03);
            CATCH_REQUIRE(a[1][0] == a10);
            CATCH_REQUIRE(a[1][1] == a11);
            CATCH_REQUIRE(a[1][2] == a12);
            CATCH_REQUIRE(a[1][3] == a13);
            CATCH_REQUIRE(a[2][0] == a20);
            CATCH_REQUIRE(a[2][1] == a21);
            CATCH_REQUIRE(a[2][2] == a22);
            CATCH_REQUIRE(a[2][3] == a23);
            CATCH_REQUIRE(a[3][0] == a30);
            CATCH_REQUIRE(a[3][1] == a31);
            CATCH_REQUIRE(a[3][2] == a32);
            CATCH_REQUIRE(a[3][3] == a33);

            // create a scalar for our test
            //
            double scalar(frand());
            while(scalar == 0.0 || scalar == 1.0)
            {
                scalar = frand();
            }

            // run operation A *= <scalar>
            //
            a *= scalar;

            // this can't fail because we ensure scalar != 0 or 1
            //
            CATCH_REQUIRE_FALSE(a[0][0] == a00);
            CATCH_REQUIRE_FALSE(a[0][1] == a01);
            CATCH_REQUIRE_FALSE(a[0][2] == a02);
            CATCH_REQUIRE_FALSE(a[0][3] == a03);
            CATCH_REQUIRE_FALSE(a[1][0] == a10);
            CATCH_REQUIRE_FALSE(a[1][1] == a11);
            CATCH_REQUIRE_FALSE(a[1][2] == a12);
            CATCH_REQUIRE_FALSE(a[1][3] == a13);
            CATCH_REQUIRE_FALSE(a[2][0] == a20);
            CATCH_REQUIRE_FALSE(a[2][1] == a21);
            CATCH_REQUIRE_FALSE(a[2][2] == a22);
            CATCH_REQUIRE_FALSE(a[2][3] == a23);
            CATCH_REQUIRE_FALSE(a[3][0] == a30);
            CATCH_REQUIRE_FALSE(a[3][1] == a31);
            CATCH_REQUIRE_FALSE(a[3][2] == a32);
            CATCH_REQUIRE_FALSE(a[3][3] == a33);

            CATCH_REQUIRE(a[0][0] == a00 * scalar);
            CATCH_REQUIRE(a[0][1] == a01 * scalar);
            CATCH_REQUIRE(a[0][2] == a02 * scalar);
            CATCH_REQUIRE(a[0][3] == a03 * scalar);
            CATCH_REQUIRE(a[1][0] == a10 * scalar);
            CATCH_REQUIRE(a[1][1] == a11 * scalar);
            CATCH_REQUIRE(a[1][2] == a12 * scalar);
            CATCH_REQUIRE(a[1][3] == a13 * scalar);
            CATCH_REQUIRE(a[2][0] == a20 * scalar);
            CATCH_REQUIRE(a[2][1] == a21 * scalar);
            CATCH_REQUIRE(a[2][2] == a22 * scalar);
            CATCH_REQUIRE(a[2][3] == a23 * scalar);
            CATCH_REQUIRE(a[3][0] == a30 * scalar);
            CATCH_REQUIRE(a[3][1] == a31 * scalar);
            CATCH_REQUIRE(a[3][2] == a32 * scalar);
            CATCH_REQUIRE(a[3][3] == a33 * scalar);
        }
        CATCH_END_SECTION()

        CATCH_START_SECTION("matrix: c=a*b")
        {
            // setup A
            //
            snapdev::matrix<double> a(4, 4);

            CATCH_REQUIRE(a.rows() == 4);
            CATCH_REQUIRE(a.columns() == 4);

            double const a00(frand());
            double const a01(frand());
            double const a02(frand());
            double const a03(frand());
            double const a10(frand());
            double const a11(frand());
            double const a12(frand());
            double const a13(frand());
            double const a20(frand());
            double const a21(frand());
            double const a22(frand());
            double const a23(frand());
            double const a30(frand());
            double const a31(frand());
            double const a32(frand());
            double const a33(frand());

            a[0][0] = a00;
            a[0][1] = a01;
            a[0][2] = a02;
            a[0][3] = a03;
            a[1][0] = a10;
            a[1][1] = a11;
            a[1][2] = a12;
            a[1][3] = a13;
            a[2][0] = a20;
            a[2][1] = a21;
            a[2][2] = a22;
            a[2][3] = a23;
            a[3][0] = a30;
            a[3][1] = a31;
            a[3][2] = a32;
            a[3][3] = a33;

            CATCH_REQUIRE(a[0][0] == a00);
            CATCH_REQUIRE(a[0][1] == a01);
            CATCH_REQUIRE(a[0][2] == a02);
            CATCH_REQUIRE(a[0][3] == a03);
            CATCH_REQUIRE(a[1][0] == a10);
            CATCH_REQUIRE(a[1][1] == a11);
            CATCH_REQUIRE(a[1][2] == a12);
            CATCH_REQUIRE(a[1][3] == a13);
            CATCH_REQUIRE(a[2][0] == a20);
            CATCH_REQUIRE(a[2][1] == a21);
            CATCH_REQUIRE(a[2][2] == a22);
            CATCH_REQUIRE(a[2][3] == a23);
            CATCH_REQUIRE(a[3][0] == a30);
            CATCH_REQUIRE(a[3][1] == a31);
            CATCH_REQUIRE(a[3][2] == a32);
            CATCH_REQUIRE(a[3][3] == a33);

            // setup B
            //
            snapdev::matrix<double> b(4, 4);

            CATCH_REQUIRE(b.rows() == 4);
            CATCH_REQUIRE(b.columns() == 4);

            double const b00(frand());
            double const b01(frand());
            double const b02(frand());
            double const b03(frand());
            double const b10(frand());
            double const b11(frand());
            double const b12(frand());
            double const b13(frand());
            double const b20(frand());
            double const b21(frand());
            double const b22(frand());
            double const b23(frand());
            double const b30(frand());
            double const b31(frand());
            double const b32(frand());
            double const b33(frand());

            b[0][0] = b00;
            b[0][1] = b01;
            b[0][2] = b02;
            b[0][3] = b03;
            b[1][0] = b10;
            b[1][1] = b11;
            b[1][2] = b12;
            b[1][3] = b13;
            b[2][0] = b20;
            b[2][1] = b21;
            b[2][2] = b22;
            b[2][3] = b23;
            b[3][0] = b30;
            b[3][1] = b31;
            b[3][2] = b32;
            b[3][3] = b33;

            CATCH_REQUIRE(b[0][0] == b00);
            CATCH_REQUIRE(b[0][1] == b01);
            CATCH_REQUIRE(b[0][2] == b02);
            CATCH_REQUIRE(b[0][3] == b03);
            CATCH_REQUIRE(b[1][0] == b10);
            CATCH_REQUIRE(b[1][1] == b11);
            CATCH_REQUIRE(b[1][2] == b12);
            CATCH_REQUIRE(b[1][3] == b13);
            CATCH_REQUIRE(b[2][0] == b20);
            CATCH_REQUIRE(b[2][1] == b21);
            CATCH_REQUIRE(b[2][2] == b22);
            CATCH_REQUIRE(b[2][3] == b23);
            CATCH_REQUIRE(b[3][0] == b30);
            CATCH_REQUIRE(b[3][1] == b31);
            CATCH_REQUIRE(b[3][2] == b32);
            CATCH_REQUIRE(b[3][3] == b33);

            // setup C
            //
            snapdev::matrix<double> c(4, 4);

            CATCH_REQUIRE(c.rows() == 4);
            CATCH_REQUIRE(c.columns() == 4);

            double const c00(frand());
            double const c01(frand());
            double const c02(frand());
            double const c03(frand());
            double const c10(frand());
            double const c11(frand());
            double const c12(frand());
            double const c13(frand());
            double const c20(frand());
            double const c21(frand());
            double const c22(frand());
            double const c23(frand());
            double const c30(frand());
            double const c31(frand());
            double const c32(frand());
            double const c33(frand());

            c[0][0] = c00;
            c[0][1] = c01;
            c[0][2] = c02;
            c[0][3] = c03;
            c[1][0] = c10;
            c[1][1] = c11;
            c[1][2] = c12;
            c[1][3] = c13;
            c[2][0] = c20;
            c[2][1] = c21;
            c[2][2] = c22;
            c[2][3] = c23;
            c[3][0] = c30;
            c[3][1] = c31;
            c[3][2] = c32;
            c[3][3] = c33;

            CATCH_REQUIRE(c[0][0] == c00);
            CATCH_REQUIRE(c[0][1] == c01);
            CATCH_REQUIRE(c[0][2] == c02);
            CATCH_REQUIRE(c[0][3] == c03);
            CATCH_REQUIRE(c[1][0] == c10);
            CATCH_REQUIRE(c[1][1] == c11);
            CATCH_REQUIRE(c[1][2] == c12);
            CATCH_REQUIRE(c[1][3] == c13);
            CATCH_REQUIRE(c[2][0] == c20);
            CATCH_REQUIRE(c[2][1] == c21);
            CATCH_REQUIRE(c[2][2] == c22);
            CATCH_REQUIRE(c[2][3] == c23);
            CATCH_REQUIRE(c[3][0] == c30);
            CATCH_REQUIRE(c[3][1] == c31);
            CATCH_REQUIRE(c[3][2] == c32);
            CATCH_REQUIRE(c[3][3] == c33);

            // run operation C = A * B
            //
            c = a * b;

            CATCH_REQUIRE(a[0][0] == a00);
            CATCH_REQUIRE(a[0][1] == a01);
            CATCH_REQUIRE(a[0][2] == a02);
            CATCH_REQUIRE(a[0][3] == a03);
            CATCH_REQUIRE(a[1][0] == a10);
            CATCH_REQUIRE(a[1][1] == a11);
            CATCH_REQUIRE(a[1][2] == a12);
            CATCH_REQUIRE(a[1][3] == a13);
            CATCH_REQUIRE(a[2][0] == a20);
            CATCH_REQUIRE(a[2][1] == a21);
            CATCH_REQUIRE(a[2][2] == a22);
            CATCH_REQUIRE(a[2][3] == a23);
            CATCH_REQUIRE(a[3][0] == a30);
            CATCH_REQUIRE(a[3][1] == a31);
            CATCH_REQUIRE(a[3][2] == a32);
            CATCH_REQUIRE(a[3][3] == a33);

            CATCH_REQUIRE(b[0][0] == b00);
            CATCH_REQUIRE(b[0][1] == b01);
            CATCH_REQUIRE(b[0][2] == b02);
            CATCH_REQUIRE(b[0][3] == b03);
            CATCH_REQUIRE(b[1][0] == b10);
            CATCH_REQUIRE(b[1][1] == b11);
            CATCH_REQUIRE(b[1][2] == b12);
            CATCH_REQUIRE(b[1][3] == b13);
            CATCH_REQUIRE(b[2][0] == b20);
            CATCH_REQUIRE(b[2][1] == b21);
            CATCH_REQUIRE(b[2][2] == b22);
            CATCH_REQUIRE(b[2][3] == b23);
            CATCH_REQUIRE(b[3][0] == b30);
            CATCH_REQUIRE(b[3][1] == b31);
            CATCH_REQUIRE(b[3][2] == b32);
            CATCH_REQUIRE(b[3][3] == b33);

            // this could fail (albeit rather unlikely)
            //CATCH_REQUIRE_FALSE(c[0][0] == c00);
            //CATCH_REQUIRE_FALSE(c[0][1] == c01);
            //CATCH_REQUIRE_FALSE(c[0][2] == c02);
            //CATCH_REQUIRE_FALSE(c[0][3] == c03);
            //CATCH_REQUIRE_FALSE(c[1][0] == c10);
            //CATCH_REQUIRE_FALSE(c[1][1] == c11);
            //CATCH_REQUIRE_FALSE(c[1][2] == c12);
            //CATCH_REQUIRE_FALSE(c[1][3] == c13);
            //CATCH_REQUIRE_FALSE(c[2][0] == c20);
            //CATCH_REQUIRE_FALSE(c[2][1] == c21);
            //CATCH_REQUIRE_FALSE(c[2][2] == c22);
            //CATCH_REQUIRE_FALSE(c[2][3] == c23);
            //CATCH_REQUIRE_FALSE(c[3][0] == c30);
            //CATCH_REQUIRE_FALSE(c[3][1] == c31);
            //CATCH_REQUIRE_FALSE(c[3][2] == c32);
            //CATCH_REQUIRE_FALSE(c[3][3] == c33);

//std::cout << "A = " << a << std::endl;
//std::cout << "B = " << b << std::endl;
//std::cout << "C = " << c << std::endl;

            CATCH_REQUIRE(c[0][0] == a00 * b00 + a01 * b10 + a02 * b20 + a03 * b30);
            CATCH_REQUIRE(c[0][1] == a00 * b01 + a01 * b11 + a02 * b21 + a03 * b31);
            CATCH_REQUIRE(c[0][2] == a00 * b02 + a01 * b12 + a02 * b22 + a03 * b32);
            CATCH_REQUIRE(c[0][3] == a00 * b03 + a01 * b13 + a02 * b23 + a03 * b33);

            CATCH_REQUIRE(c[1][0] == a10 * b00 + a11 * b10 + a12 * b20 + a13 * b30);
            CATCH_REQUIRE(c[1][1] == a10 * b01 + a11 * b11 + a12 * b21 + a13 * b31);
            CATCH_REQUIRE(c[1][2] == a10 * b02 + a11 * b12 + a12 * b22 + a13 * b32);
            CATCH_REQUIRE(c[1][3] == a10 * b03 + a11 * b13 + a12 * b23 + a13 * b33);

            CATCH_REQUIRE(c[2][0] == a20 * b00 + a21 * b10 + a22 * b20 + a23 * b30);
            CATCH_REQUIRE(c[2][1] == a20 * b01 + a21 * b11 + a22 * b21 + a23 * b31);
            CATCH_REQUIRE(c[2][2] == a20 * b02 + a21 * b12 + a22 * b22 + a23 * b32);
            CATCH_REQUIRE(c[2][3] == a20 * b03 + a21 * b13 + a22 * b23 + a23 * b33);

            CATCH_REQUIRE(c[3][0] == a30 * b00 + a31 * b10 + a32 * b20 + a33 * b30);
            CATCH_REQUIRE(c[3][1] == a30 * b01 + a31 * b11 + a32 * b21 + a33 * b31);
            CATCH_REQUIRE(c[3][2] == a30 * b02 + a31 * b12 + a32 * b22 + a33 * b32);
            CATCH_REQUIRE(c[3][3] == a30 * b03 + a31 * b13 + a32 * b23 + a33 * b33);

            // if swapped it would equal this instead
            //CATCH_REQUIRE(c[0][0] == a00 * b00 + a10 * b01 + a20 * b02 + a30 * b03);
            //CATCH_REQUIRE(c[0][1] == a01 * b00 + a11 * b01 + a21 * b02 + a31 * b03);
            //CATCH_REQUIRE(c[0][2] == a02 * b00 + a12 * b01 + a22 * b02 + a32 * b03);
            //CATCH_REQUIRE(c[0][3] == a03 * b00 + a13 * b01 + a23 * b02 + a33 * b03);

            //CATCH_REQUIRE(c[1][0] == a00 * b10 + a10 * b11 + a20 * b12 + a30 * b13);
            //CATCH_REQUIRE(c[1][1] == a01 * b10 + a11 * b11 + a21 * b12 + a31 * b13);
            //CATCH_REQUIRE(c[1][2] == a02 * b10 + a12 * b11 + a22 * b12 + a32 * b13);
            //CATCH_REQUIRE(c[1][3] == a03 * b10 + a13 * b11 + a23 * b12 + a33 * b13);

            //CATCH_REQUIRE(c[2][0] == a00 * b20 + a10 * b21 + a20 * b22 + a30 * b23);
            //CATCH_REQUIRE(c[2][1] == a01 * b20 + a11 * b21 + a21 * b22 + a31 * b23);
            //CATCH_REQUIRE(c[2][2] == a02 * b20 + a12 * b21 + a22 * b22 + a32 * b23);
            //CATCH_REQUIRE(c[2][3] == a03 * b20 + a13 * b21 + a23 * b22 + a33 * b23);

            //CATCH_REQUIRE(c[3][0] == a00 * b30 + a10 * b31 + a20 * b32 + a30 * b33);
            //CATCH_REQUIRE(c[3][1] == a01 * b30 + a11 * b31 + a21 * b32 + a31 * b33);
            //CATCH_REQUIRE(c[3][2] == a02 * b30 + a12 * b31 + a22 * b32 + a32 * b33);
            //CATCH_REQUIRE(c[3][3] == a03 * b30 + a13 * b31 + a23 * b32 + a33 * b33);
        }
        CATCH_END_SECTION()

        CATCH_START_SECTION("matrix: a*=b")
        {
            // setup A
            //
            snapdev::matrix<double> a(4, 4);

            CATCH_REQUIRE(a.rows() == 4);
            CATCH_REQUIRE(a.columns() == 4);

            double const a00(frand());
            double const a01(frand());
            double const a02(frand());
            double const a03(frand());
            double const a10(frand());
            double const a11(frand());
            double const a12(frand());
            double const a13(frand());
            double const a20(frand());
            double const a21(frand());
            double const a22(frand());
            double const a23(frand());
            double const a30(frand());
            double const a31(frand());
            double const a32(frand());
            double const a33(frand());

            a[0][0] = a00;
            a[0][1] = a01;
            a[0][2] = a02;
            a[0][3] = a03;
            a[1][0] = a10;
            a[1][1] = a11;
            a[1][2] = a12;
            a[1][3] = a13;
            a[2][0] = a20;
            a[2][1] = a21;
            a[2][2] = a22;
            a[2][3] = a23;
            a[3][0] = a30;
            a[3][1] = a31;
            a[3][2] = a32;
            a[3][3] = a33;

            CATCH_REQUIRE(a[0][0] == a00);
            CATCH_REQUIRE(a[0][1] == a01);
            CATCH_REQUIRE(a[0][2] == a02);
            CATCH_REQUIRE(a[0][3] == a03);
            CATCH_REQUIRE(a[1][0] == a10);
            CATCH_REQUIRE(a[1][1] == a11);
            CATCH_REQUIRE(a[1][2] == a12);
            CATCH_REQUIRE(a[1][3] == a13);
            CATCH_REQUIRE(a[2][0] == a20);
            CATCH_REQUIRE(a[2][1] == a21);
            CATCH_REQUIRE(a[2][2] == a22);
            CATCH_REQUIRE(a[2][3] == a23);
            CATCH_REQUIRE(a[3][0] == a30);
            CATCH_REQUIRE(a[3][1] == a31);
            CATCH_REQUIRE(a[3][2] == a32);
            CATCH_REQUIRE(a[3][3] == a33);

            // setup B
            //
            snapdev::matrix<double> b(4, 4);

            CATCH_REQUIRE(b.rows() == 4);
            CATCH_REQUIRE(b.columns() == 4);

            double const b00(frand());
            double const b01(frand());
            double const b02(frand());
            double const b03(frand());
            double const b10(frand());
            double const b11(frand());
            double const b12(frand());
            double const b13(frand());
            double const b20(frand());
            double const b21(frand());
            double const b22(frand());
            double const b23(frand());
            double const b30(frand());
            double const b31(frand());
            double const b32(frand());
            double const b33(frand());

            b[0][0] = b00;
            b[0][1] = b01;
            b[0][2] = b02;
            b[0][3] = b03;
            b[1][0] = b10;
            b[1][1] = b11;
            b[1][2] = b12;
            b[1][3] = b13;
            b[2][0] = b20;
            b[2][1] = b21;
            b[2][2] = b22;
            b[2][3] = b23;
            b[3][0] = b30;
            b[3][1] = b31;
            b[3][2] = b32;
            b[3][3] = b33;

            CATCH_REQUIRE(b[0][0] == b00);
            CATCH_REQUIRE(b[0][1] == b01);
            CATCH_REQUIRE(b[0][2] == b02);
            CATCH_REQUIRE(b[0][3] == b03);
            CATCH_REQUIRE(b[1][0] == b10);
            CATCH_REQUIRE(b[1][1] == b11);
            CATCH_REQUIRE(b[1][2] == b12);
            CATCH_REQUIRE(b[1][3] == b13);
            CATCH_REQUIRE(b[2][0] == b20);
            CATCH_REQUIRE(b[2][1] == b21);
            CATCH_REQUIRE(b[2][2] == b22);
            CATCH_REQUIRE(b[2][3] == b23);
            CATCH_REQUIRE(b[3][0] == b30);
            CATCH_REQUIRE(b[3][1] == b31);
            CATCH_REQUIRE(b[3][2] == b32);
            CATCH_REQUIRE(b[3][3] == b33);

            // run operation A *= B
            //
            a *= b;

            // this could fail
            //
            //CATCH_REQUIRE_FALSE(a[0][0] == a00);
            //CATCH_REQUIRE_FALSE(a[0][1] == a01);
            //CATCH_REQUIRE_FALSE(a[0][2] == a02);
            //CATCH_REQUIRE_FALSE(a[0][3] == a03);
            //CATCH_REQUIRE_FALSE(a[1][0] == a10);
            //CATCH_REQUIRE_FALSE(a[1][1] == a11);
            //CATCH_REQUIRE_FALSE(a[1][2] == a12);
            //CATCH_REQUIRE_FALSE(a[1][3] == a13);
            //CATCH_REQUIRE_FALSE(a[2][0] == a20);
            //CATCH_REQUIRE_FALSE(a[2][1] == a21);
            //CATCH_REQUIRE_FALSE(a[2][2] == a22);
            //CATCH_REQUIRE_FALSE(a[2][3] == a23);
            //CATCH_REQUIRE_FALSE(a[3][0] == a30);
            //CATCH_REQUIRE_FALSE(a[3][1] == a31);
            //CATCH_REQUIRE_FALSE(a[3][2] == a32);
            //CATCH_REQUIRE_FALSE(a[3][3] == a33);

            CATCH_REQUIRE(b[0][0] == b00);
            CATCH_REQUIRE(b[0][1] == b01);
            CATCH_REQUIRE(b[0][2] == b02);
            CATCH_REQUIRE(b[0][3] == b03);
            CATCH_REQUIRE(b[1][0] == b10);
            CATCH_REQUIRE(b[1][1] == b11);
            CATCH_REQUIRE(b[1][2] == b12);
            CATCH_REQUIRE(b[1][3] == b13);
            CATCH_REQUIRE(b[2][0] == b20);
            CATCH_REQUIRE(b[2][1] == b21);
            CATCH_REQUIRE(b[2][2] == b22);
            CATCH_REQUIRE(b[2][3] == b23);
            CATCH_REQUIRE(b[3][0] == b30);
            CATCH_REQUIRE(b[3][1] == b31);
            CATCH_REQUIRE(b[3][2] == b32);
            CATCH_REQUIRE(b[3][3] == b33);

            CATCH_REQUIRE(a[0][0] == a00 * b00 + a01 * b10 + a02 * b20 + a03 * b30);
            CATCH_REQUIRE(a[0][1] == a00 * b01 + a01 * b11 + a02 * b21 + a03 * b31);
            CATCH_REQUIRE(a[0][2] == a00 * b02 + a01 * b12 + a02 * b22 + a03 * b32);
            CATCH_REQUIRE(a[0][3] == a00 * b03 + a01 * b13 + a02 * b23 + a03 * b33);

            CATCH_REQUIRE(a[1][0] == a10 * b00 + a11 * b10 + a12 * b20 + a13 * b30);
            CATCH_REQUIRE(a[1][1] == a10 * b01 + a11 * b11 + a12 * b21 + a13 * b31);
            CATCH_REQUIRE(a[1][2] == a10 * b02 + a11 * b12 + a12 * b22 + a13 * b32);
            CATCH_REQUIRE(a[1][3] == a10 * b03 + a11 * b13 + a12 * b23 + a13 * b33);

            CATCH_REQUIRE(a[2][0] == a20 * b00 + a21 * b10 + a22 * b20 + a23 * b30);
            CATCH_REQUIRE(a[2][1] == a20 * b01 + a21 * b11 + a22 * b21 + a23 * b31);
            CATCH_REQUIRE(a[2][2] == a20 * b02 + a21 * b12 + a22 * b22 + a23 * b32);
            CATCH_REQUIRE(a[2][3] == a20 * b03 + a21 * b13 + a22 * b23 + a23 * b33);

            CATCH_REQUIRE(a[3][0] == a30 * b00 + a31 * b10 + a32 * b20 + a33 * b30);
            CATCH_REQUIRE(a[3][1] == a30 * b01 + a31 * b11 + a32 * b21 + a33 * b31);
            CATCH_REQUIRE(a[3][2] == a30 * b02 + a31 * b12 + a32 * b22 + a33 * b32);
            CATCH_REQUIRE(a[3][3] == a30 * b03 + a31 * b13 + a32 * b23 + a33 * b33);

            //CATCH_REQUIRE(a[0][0] == a00 * b00 + a10 * b01 + a20 * b02 + a30 * b03);
            //CATCH_REQUIRE(a[0][1] == a01 * b00 + a11 * b01 + a21 * b02 + a31 * b03);
            //CATCH_REQUIRE(a[0][2] == a02 * b00 + a12 * b01 + a22 * b02 + a32 * b03);
            //CATCH_REQUIRE(a[0][3] == a03 * b00 + a13 * b01 + a23 * b02 + a33 * b03);

            //CATCH_REQUIRE(a[1][0] == a00 * b10 + a10 * b11 + a20 * b12 + a30 * b13);
            //CATCH_REQUIRE(a[1][1] == a01 * b10 + a11 * b11 + a21 * b12 + a31 * b13);
            //CATCH_REQUIRE(a[1][2] == a02 * b10 + a12 * b11 + a22 * b12 + a32 * b13);
            //CATCH_REQUIRE(a[1][3] == a03 * b10 + a13 * b11 + a23 * b12 + a33 * b13);

            //CATCH_REQUIRE(a[2][0] == a00 * b20 + a10 * b21 + a20 * b22 + a30 * b23);
            //CATCH_REQUIRE(a[2][1] == a01 * b20 + a11 * b21 + a21 * b22 + a31 * b23);
            //CATCH_REQUIRE(a[2][2] == a02 * b20 + a12 * b21 + a22 * b22 + a32 * b23);
            //CATCH_REQUIRE(a[2][3] == a03 * b20 + a13 * b21 + a23 * b22 + a33 * b23);

            //CATCH_REQUIRE(a[3][0] == a00 * b30 + a10 * b31 + a20 * b32 + a30 * b33);
            //CATCH_REQUIRE(a[3][1] == a01 * b30 + a11 * b31 + a21 * b32 + a31 * b33);
            //CATCH_REQUIRE(a[3][2] == a02 * b30 + a12 * b31 + a22 * b32 + a32 * b33);
            //CATCH_REQUIRE(a[3][3] == a03 * b30 + a13 * b31 + a23 * b32 + a33 * b33);
        }
        CATCH_END_SECTION()
    }

    // create two random 4x4 matrices and make sure the add works
    //
    CATCH_GIVEN("div")
    {
        CATCH_START_SECTION("matrix: b=a/<scalar>")
        {
            // setup A
            //
            snapdev::matrix<double> a(4, 4);

            CATCH_REQUIRE(a.rows() == 4);
            CATCH_REQUIRE(a.columns() == 4);

            double const a00(frand());
            double const a01(frand());
            double const a02(frand());
            double const a03(frand());
            double const a10(frand());
            double const a11(frand());
            double const a12(frand());
            double const a13(frand());
            double const a20(frand());
            double const a21(frand());
            double const a22(frand());
            double const a23(frand());
            double const a30(frand());
            double const a31(frand());
            double const a32(frand());
            double const a33(frand());

            a[0][0] = a00;
            a[0][1] = a01;
            a[0][2] = a02;
            a[0][3] = a03;
            a[1][0] = a10;
            a[1][1] = a11;
            a[1][2] = a12;
            a[1][3] = a13;
            a[2][0] = a20;
            a[2][1] = a21;
            a[2][2] = a22;
            a[2][3] = a23;
            a[3][0] = a30;
            a[3][1] = a31;
            a[3][2] = a32;
            a[3][3] = a33;

            CATCH_REQUIRE(a[0][0] == a00);
            CATCH_REQUIRE(a[0][1] == a01);
            CATCH_REQUIRE(a[0][2] == a02);
            CATCH_REQUIRE(a[0][3] == a03);
            CATCH_REQUIRE(a[1][0] == a10);
            CATCH_REQUIRE(a[1][1] == a11);
            CATCH_REQUIRE(a[1][2] == a12);
            CATCH_REQUIRE(a[1][3] == a13);
            CATCH_REQUIRE(a[2][0] == a20);
            CATCH_REQUIRE(a[2][1] == a21);
            CATCH_REQUIRE(a[2][2] == a22);
            CATCH_REQUIRE(a[2][3] == a23);
            CATCH_REQUIRE(a[3][0] == a30);
            CATCH_REQUIRE(a[3][1] == a31);
            CATCH_REQUIRE(a[3][2] == a32);
            CATCH_REQUIRE(a[3][3] == a33);

            // setup B
            //
            snapdev::matrix<double> b(4, 4);

            CATCH_REQUIRE(b.rows() == 4);
            CATCH_REQUIRE(b.columns() == 4);

            double const b00(frand());
            double const b01(frand());
            double const b02(frand());
            double const b03(frand());
            double const b10(frand());
            double const b11(frand());
            double const b12(frand());
            double const b13(frand());
            double const b20(frand());
            double const b21(frand());
            double const b22(frand());
            double const b23(frand());
            double const b30(frand());
            double const b31(frand());
            double const b32(frand());
            double const b33(frand());

            b[0][0] = b00;
            b[0][1] = b01;
            b[0][2] = b02;
            b[0][3] = b03;
            b[1][0] = b10;
            b[1][1] = b11;
            b[1][2] = b12;
            b[1][3] = b13;
            b[2][0] = b20;
            b[2][1] = b21;
            b[2][2] = b22;
            b[2][3] = b23;
            b[3][0] = b30;
            b[3][1] = b31;
            b[3][2] = b32;
            b[3][3] = b33;

            CATCH_REQUIRE(b[0][0] == b00);
            CATCH_REQUIRE(b[0][1] == b01);
            CATCH_REQUIRE(b[0][2] == b02);
            CATCH_REQUIRE(b[0][3] == b03);
            CATCH_REQUIRE(b[1][0] == b10);
            CATCH_REQUIRE(b[1][1] == b11);
            CATCH_REQUIRE(b[1][2] == b12);
            CATCH_REQUIRE(b[1][3] == b13);
            CATCH_REQUIRE(b[2][0] == b20);
            CATCH_REQUIRE(b[2][1] == b21);
            CATCH_REQUIRE(b[2][2] == b22);
            CATCH_REQUIRE(b[2][3] == b23);
            CATCH_REQUIRE(b[3][0] == b30);
            CATCH_REQUIRE(b[3][1] == b31);
            CATCH_REQUIRE(b[3][2] == b32);
            CATCH_REQUIRE(b[3][3] == b33);

            // create a scalar for our test
            //
            double scalar(frand());
            while(scalar == 0.0 || scalar == 1.0)
            {
                scalar = frand();
            }

            // run operation B = A / <scalar>
            //
            b = a / scalar;

            CATCH_REQUIRE(a[0][0] == a00);
            CATCH_REQUIRE(a[0][1] == a01);
            CATCH_REQUIRE(a[0][2] == a02);
            CATCH_REQUIRE(a[0][3] == a03);
            CATCH_REQUIRE(a[1][0] == a10);
            CATCH_REQUIRE(a[1][1] == a11);
            CATCH_REQUIRE(a[1][2] == a12);
            CATCH_REQUIRE(a[1][3] == a13);
            CATCH_REQUIRE(a[2][0] == a20);
            CATCH_REQUIRE(a[2][1] == a21);
            CATCH_REQUIRE(a[2][2] == a22);
            CATCH_REQUIRE(a[2][3] == a23);
            CATCH_REQUIRE(a[3][0] == a30);
            CATCH_REQUIRE(a[3][1] == a31);
            CATCH_REQUIRE(a[3][2] == a32);
            CATCH_REQUIRE(a[3][3] == a33);

            // this can't fail because we ensure scalar != 0 or 1
            //
            CATCH_REQUIRE_FALSE(b[0][0] == b00);
            CATCH_REQUIRE_FALSE(b[0][1] == b01);
            CATCH_REQUIRE_FALSE(b[0][2] == b02);
            CATCH_REQUIRE_FALSE(b[0][3] == b03);
            CATCH_REQUIRE_FALSE(b[1][0] == b10);
            CATCH_REQUIRE_FALSE(b[1][1] == b11);
            CATCH_REQUIRE_FALSE(b[1][2] == b12);
            CATCH_REQUIRE_FALSE(b[1][3] == b13);
            CATCH_REQUIRE_FALSE(b[2][0] == b20);
            CATCH_REQUIRE_FALSE(b[2][1] == b21);
            CATCH_REQUIRE_FALSE(b[2][2] == b22);
            CATCH_REQUIRE_FALSE(b[2][3] == b23);
            CATCH_REQUIRE_FALSE(b[3][0] == b30);
            CATCH_REQUIRE_FALSE(b[3][1] == b31);
            CATCH_REQUIRE_FALSE(b[3][2] == b32);
            CATCH_REQUIRE_FALSE(b[3][3] == b33);

            CATCH_REQUIRE(b[0][0] == a00 / scalar);
            CATCH_REQUIRE(b[0][1] == a01 / scalar);
            CATCH_REQUIRE(b[0][2] == a02 / scalar);
            CATCH_REQUIRE(b[0][3] == a03 / scalar);
            CATCH_REQUIRE(b[1][0] == a10 / scalar);
            CATCH_REQUIRE(b[1][1] == a11 / scalar);
            CATCH_REQUIRE(b[1][2] == a12 / scalar);
            CATCH_REQUIRE(b[1][3] == a13 / scalar);
            CATCH_REQUIRE(b[2][0] == a20 / scalar);
            CATCH_REQUIRE(b[2][1] == a21 / scalar);
            CATCH_REQUIRE(b[2][2] == a22 / scalar);
            CATCH_REQUIRE(b[2][3] == a23 / scalar);
            CATCH_REQUIRE(b[3][0] == a30 / scalar);
            CATCH_REQUIRE(b[3][1] == a31 / scalar);
            CATCH_REQUIRE(b[3][2] == a32 / scalar);
            CATCH_REQUIRE(b[3][3] == a33 / scalar);
        }
        CATCH_END_SECTION()

        CATCH_START_SECTION("matrix: a/=<scalar>")
        {
            // setup A
            //
            snapdev::matrix<double> a(4, 4);

            CATCH_REQUIRE(a.rows() == 4);
            CATCH_REQUIRE(a.columns() == 4);

            double const a00(frand());
            double const a01(frand());
            double const a02(frand());
            double const a03(frand());
            double const a10(frand());
            double const a11(frand());
            double const a12(frand());
            double const a13(frand());
            double const a20(frand());
            double const a21(frand());
            double const a22(frand());
            double const a23(frand());
            double const a30(frand());
            double const a31(frand());
            double const a32(frand());
            double const a33(frand());

            a[0][0] = a00;
            a[0][1] = a01;
            a[0][2] = a02;
            a[0][3] = a03;
            a[1][0] = a10;
            a[1][1] = a11;
            a[1][2] = a12;
            a[1][3] = a13;
            a[2][0] = a20;
            a[2][1] = a21;
            a[2][2] = a22;
            a[2][3] = a23;
            a[3][0] = a30;
            a[3][1] = a31;
            a[3][2] = a32;
            a[3][3] = a33;

            CATCH_REQUIRE(a[0][0] == a00);
            CATCH_REQUIRE(a[0][1] == a01);
            CATCH_REQUIRE(a[0][2] == a02);
            CATCH_REQUIRE(a[0][3] == a03);
            CATCH_REQUIRE(a[1][0] == a10);
            CATCH_REQUIRE(a[1][1] == a11);
            CATCH_REQUIRE(a[1][2] == a12);
            CATCH_REQUIRE(a[1][3] == a13);
            CATCH_REQUIRE(a[2][0] == a20);
            CATCH_REQUIRE(a[2][1] == a21);
            CATCH_REQUIRE(a[2][2] == a22);
            CATCH_REQUIRE(a[2][3] == a23);
            CATCH_REQUIRE(a[3][0] == a30);
            CATCH_REQUIRE(a[3][1] == a31);
            CATCH_REQUIRE(a[3][2] == a32);
            CATCH_REQUIRE(a[3][3] == a33);

            // create a scalar for our test
            //
            double scalar(frand());
            while(scalar == 0.0)
            {
                scalar = frand();
            }

            // run operation A /= <scalar>
            //
            a /= scalar;

            // this can't fail because we ensure scalar != 0 or 1
            //
            CATCH_REQUIRE_FALSE(a[0][0] == a00);
            CATCH_REQUIRE_FALSE(a[0][1] == a01);
            CATCH_REQUIRE_FALSE(a[0][2] == a02);
            CATCH_REQUIRE_FALSE(a[0][3] == a03);
            CATCH_REQUIRE_FALSE(a[1][0] == a10);
            CATCH_REQUIRE_FALSE(a[1][1] == a11);
            CATCH_REQUIRE_FALSE(a[1][2] == a12);
            CATCH_REQUIRE_FALSE(a[1][3] == a13);
            CATCH_REQUIRE_FALSE(a[2][0] == a20);
            CATCH_REQUIRE_FALSE(a[2][1] == a21);
            CATCH_REQUIRE_FALSE(a[2][2] == a22);
            CATCH_REQUIRE_FALSE(a[2][3] == a23);
            CATCH_REQUIRE_FALSE(a[3][0] == a30);
            CATCH_REQUIRE_FALSE(a[3][1] == a31);
            CATCH_REQUIRE_FALSE(a[3][2] == a32);
            CATCH_REQUIRE_FALSE(a[3][3] == a33);

            CATCH_REQUIRE(a[0][0] == a00 / scalar);
            CATCH_REQUIRE(a[0][1] == a01 / scalar);
            CATCH_REQUIRE(a[0][2] == a02 / scalar);
            CATCH_REQUIRE(a[0][3] == a03 / scalar);
            CATCH_REQUIRE(a[1][0] == a10 / scalar);
            CATCH_REQUIRE(a[1][1] == a11 / scalar);
            CATCH_REQUIRE(a[1][2] == a12 / scalar);
            CATCH_REQUIRE(a[1][3] == a13 / scalar);
            CATCH_REQUIRE(a[2][0] == a20 / scalar);
            CATCH_REQUIRE(a[2][1] == a21 / scalar);
            CATCH_REQUIRE(a[2][2] == a22 / scalar);
            CATCH_REQUIRE(a[2][3] == a23 / scalar);
            CATCH_REQUIRE(a[3][0] == a30 / scalar);
            CATCH_REQUIRE(a[3][1] == a31 / scalar);
            CATCH_REQUIRE(a[3][2] == a32 / scalar);
            CATCH_REQUIRE(a[3][3] == a33 / scalar);
        }
        CATCH_END_SECTION()

        CATCH_START_SECTION("matrix: c=a/b")
        {
            // setup A
            //
            snapdev::matrix<double> a(4, 4);

            CATCH_REQUIRE(a.rows() == 4);
            CATCH_REQUIRE(a.columns() == 4);

            double const a00(frand());
            double const a01(frand());
            double const a02(frand());
            double const a03(frand());
            double const a10(frand());
            double const a11(frand());
            double const a12(frand());
            double const a13(frand());
            double const a20(frand());
            double const a21(frand());
            double const a22(frand());
            double const a23(frand());
            double const a30(frand());
            double const a31(frand());
            double const a32(frand());
            double const a33(frand());

            a[0][0] = a00;
            a[0][1] = a01;
            a[0][2] = a02;
            a[0][3] = a03;
            a[1][0] = a10;
            a[1][1] = a11;
            a[1][2] = a12;
            a[1][3] = a13;
            a[2][0] = a20;
            a[2][1] = a21;
            a[2][2] = a22;
            a[2][3] = a23;
            a[3][0] = a30;
            a[3][1] = a31;
            a[3][2] = a32;
            a[3][3] = a33;

            CATCH_REQUIRE(a[0][0] == a00);
            CATCH_REQUIRE(a[0][1] == a01);
            CATCH_REQUIRE(a[0][2] == a02);
            CATCH_REQUIRE(a[0][3] == a03);
            CATCH_REQUIRE(a[1][0] == a10);
            CATCH_REQUIRE(a[1][1] == a11);
            CATCH_REQUIRE(a[1][2] == a12);
            CATCH_REQUIRE(a[1][3] == a13);
            CATCH_REQUIRE(a[2][0] == a20);
            CATCH_REQUIRE(a[2][1] == a21);
            CATCH_REQUIRE(a[2][2] == a22);
            CATCH_REQUIRE(a[2][3] == a23);
            CATCH_REQUIRE(a[3][0] == a30);
            CATCH_REQUIRE(a[3][1] == a31);
            CATCH_REQUIRE(a[3][2] == a32);
            CATCH_REQUIRE(a[3][3] == a33);

            // setup B
            //
            snapdev::matrix<double> b(4, 4);

            CATCH_REQUIRE(b.rows() == 4);
            CATCH_REQUIRE(b.columns() == 4);

            double b00;
            double b01;
            double b02;
            double b03;
            double b10;
            double b11;
            double b12;
            double b13;
            double b20;
            double b21;
            double b22;
            double b23;
            double b30;
            double b31;
            double b32;
            double b33;

            // create an inversible matrix (most are with random numbers)
            snapdev::matrix<double> t(4, 4);
            do
            {
                b00 = frand();
                b01 = frand();
                b02 = frand();
                b03 = frand();
                b10 = frand();
                b11 = frand();
                b12 = frand();
                b13 = frand();
                b20 = frand();
                b21 = frand();
                b22 = frand();
                b23 = frand();
                b30 = frand();
                b31 = frand();
                b32 = frand();
                b33 = frand();

                b[0][0] = b00;
                b[0][1] = b01;
                b[0][2] = b02;
                b[0][3] = b03;
                b[1][0] = b10;
                b[1][1] = b11;
                b[1][2] = b12;
                b[1][3] = b13;
                b[2][0] = b20;
                b[2][1] = b21;
                b[2][2] = b22;
                b[2][3] = b23;
                b[3][0] = b30;
                b[3][1] = b31;
                b[3][2] = b32;
                b[3][3] = b33;

                CATCH_REQUIRE(b[0][0] == b00);
                CATCH_REQUIRE(b[0][1] == b01);
                CATCH_REQUIRE(b[0][2] == b02);
                CATCH_REQUIRE(b[0][3] == b03);
                CATCH_REQUIRE(b[1][0] == b10);
                CATCH_REQUIRE(b[1][1] == b11);
                CATCH_REQUIRE(b[1][2] == b12);
                CATCH_REQUIRE(b[1][3] == b13);
                CATCH_REQUIRE(b[2][0] == b20);
                CATCH_REQUIRE(b[2][1] == b21);
                CATCH_REQUIRE(b[2][2] == b22);
                CATCH_REQUIRE(b[2][3] == b23);
                CATCH_REQUIRE(b[3][0] == b30);
                CATCH_REQUIRE(b[3][1] == b31);
                CATCH_REQUIRE(b[3][2] == b32);
                CATCH_REQUIRE(b[3][3] == b33);

                t = b;
            }
            while(!t.inverse());

            // setup C
            //
            snapdev::matrix<double> c(4, 4);

            CATCH_REQUIRE(c.rows() == 4);
            CATCH_REQUIRE(c.columns() == 4);

            double const c00(frand());
            double const c01(frand());
            double const c02(frand());
            double const c03(frand());
            double const c10(frand());
            double const c11(frand());
            double const c12(frand());
            double const c13(frand());
            double const c20(frand());
            double const c21(frand());
            double const c22(frand());
            double const c23(frand());
            double const c30(frand());
            double const c31(frand());
            double const c32(frand());
            double const c33(frand());

            c[0][0] = c00;
            c[0][1] = c01;
            c[0][2] = c02;
            c[0][3] = c03;
            c[1][0] = c10;
            c[1][1] = c11;
            c[1][2] = c12;
            c[1][3] = c13;
            c[2][0] = c20;
            c[2][1] = c21;
            c[2][2] = c22;
            c[2][3] = c23;
            c[3][0] = c30;
            c[3][1] = c31;
            c[3][2] = c32;
            c[3][3] = c33;

            CATCH_REQUIRE(c[0][0] == c00);
            CATCH_REQUIRE(c[0][1] == c01);
            CATCH_REQUIRE(c[0][2] == c02);
            CATCH_REQUIRE(c[0][3] == c03);
            CATCH_REQUIRE(c[1][0] == c10);
            CATCH_REQUIRE(c[1][1] == c11);
            CATCH_REQUIRE(c[1][2] == c12);
            CATCH_REQUIRE(c[1][3] == c13);
            CATCH_REQUIRE(c[2][0] == c20);
            CATCH_REQUIRE(c[2][1] == c21);
            CATCH_REQUIRE(c[2][2] == c22);
            CATCH_REQUIRE(c[2][3] == c23);
            CATCH_REQUIRE(c[3][0] == c30);
            CATCH_REQUIRE(c[3][1] == c31);
            CATCH_REQUIRE(c[3][2] == c32);
            CATCH_REQUIRE(c[3][3] == c33);

            // run operation C = A / B
            //
            c = a / b;

            CATCH_REQUIRE(a[0][0] == a00);
            CATCH_REQUIRE(a[0][1] == a01);
            CATCH_REQUIRE(a[0][2] == a02);
            CATCH_REQUIRE(a[0][3] == a03);
            CATCH_REQUIRE(a[1][0] == a10);
            CATCH_REQUIRE(a[1][1] == a11);
            CATCH_REQUIRE(a[1][2] == a12);
            CATCH_REQUIRE(a[1][3] == a13);
            CATCH_REQUIRE(a[2][0] == a20);
            CATCH_REQUIRE(a[2][1] == a21);
            CATCH_REQUIRE(a[2][2] == a22);
            CATCH_REQUIRE(a[2][3] == a23);
            CATCH_REQUIRE(a[3][0] == a30);
            CATCH_REQUIRE(a[3][1] == a31);
            CATCH_REQUIRE(a[3][2] == a32);
            CATCH_REQUIRE(a[3][3] == a33);

            CATCH_REQUIRE(b[0][0] == b00);
            CATCH_REQUIRE(b[0][1] == b01);
            CATCH_REQUIRE(b[0][2] == b02);
            CATCH_REQUIRE(b[0][3] == b03);
            CATCH_REQUIRE(b[1][0] == b10);
            CATCH_REQUIRE(b[1][1] == b11);
            CATCH_REQUIRE(b[1][2] == b12);
            CATCH_REQUIRE(b[1][3] == b13);
            CATCH_REQUIRE(b[2][0] == b20);
            CATCH_REQUIRE(b[2][1] == b21);
            CATCH_REQUIRE(b[2][2] == b22);
            CATCH_REQUIRE(b[2][3] == b23);
            CATCH_REQUIRE(b[3][0] == b30);
            CATCH_REQUIRE(b[3][1] == b31);
            CATCH_REQUIRE(b[3][2] == b32);
            CATCH_REQUIRE(b[3][3] == b33);

            // this could fail
            //CATCH_REQUIRE_FALSE(c[0][0] == c00);
            //CATCH_REQUIRE_FALSE(c[0][1] == c01);
            //CATCH_REQUIRE_FALSE(c[0][2] == c02);
            //CATCH_REQUIRE_FALSE(c[0][3] == c03);
            //CATCH_REQUIRE_FALSE(c[1][0] == c10);
            //CATCH_REQUIRE_FALSE(c[1][1] == c11);
            //CATCH_REQUIRE_FALSE(c[1][2] == c12);
            //CATCH_REQUIRE_FALSE(c[1][3] == c13);
            //CATCH_REQUIRE_FALSE(c[2][0] == c20);
            //CATCH_REQUIRE_FALSE(c[2][1] == c21);
            //CATCH_REQUIRE_FALSE(c[2][2] == c22);
            //CATCH_REQUIRE_FALSE(c[2][3] == c23);
            //CATCH_REQUIRE_FALSE(c[3][0] == c30);
            //CATCH_REQUIRE_FALSE(c[3][1] == c31);
            //CATCH_REQUIRE_FALSE(c[3][2] == c32);
            //CATCH_REQUIRE_FALSE(c[3][3] == c33);

            // first level verification, which is exactly what the
            // division function does, it does not mean that the
            // division works properly, though
            //
            snapdev::matrix<double> r(4, 4);
            r = a * t;

            CATCH_REQUIRE(c[0][0] == r[0][0]);
            CATCH_REQUIRE(c[0][1] == r[0][1]);
            CATCH_REQUIRE(c[0][2] == r[0][2]);
            CATCH_REQUIRE(c[0][3] == r[0][3]);
            CATCH_REQUIRE(c[1][0] == r[1][0]);
            CATCH_REQUIRE(c[1][1] == r[1][1]);
            CATCH_REQUIRE(c[1][2] == r[1][2]);
            CATCH_REQUIRE(c[1][3] == r[1][3]);
            CATCH_REQUIRE(c[2][0] == r[2][0]);
            CATCH_REQUIRE(c[2][1] == r[2][1]);
            CATCH_REQUIRE(c[2][2] == r[2][2]);
            CATCH_REQUIRE(c[2][3] == r[2][3]);
            CATCH_REQUIRE(c[3][0] == r[3][0]);
            CATCH_REQUIRE(c[3][1] == r[3][1]);
            CATCH_REQUIRE(c[3][2] == r[3][2]);
            CATCH_REQUIRE(c[3][3] == r[3][3]);

            double const determinant(b.determinant());
            snapdev::matrix<double> adjugate(b.adjugate());

            snapdev::matrix<double> inv(adjugate * (1.0 / determinant));
            snapdev::matrix<double> div(a * inv);

            CATCH_REQUIRE(fabs(c[0][0] - div[0][0]) < 0.0001);
            CATCH_REQUIRE(fabs(c[0][1] - div[0][1]) < 0.0001);
            CATCH_REQUIRE(fabs(c[0][2] - div[0][2]) < 0.0001);
            CATCH_REQUIRE(fabs(c[0][3] - div[0][3]) < 0.0001);
            CATCH_REQUIRE(fabs(c[1][0] - div[1][0]) < 0.0001);
            CATCH_REQUIRE(fabs(c[1][1] - div[1][1]) < 0.0001);
            CATCH_REQUIRE(fabs(c[1][2] - div[1][2]) < 0.0001);
            CATCH_REQUIRE(fabs(c[1][3] - div[1][3]) < 0.0001);
            CATCH_REQUIRE(fabs(c[2][0] - div[2][0]) < 0.0001);
            CATCH_REQUIRE(fabs(c[2][1] - div[2][1]) < 0.0001);
            CATCH_REQUIRE(fabs(c[2][2] - div[2][2]) < 0.0001);
            CATCH_REQUIRE(fabs(c[2][3] - div[2][3]) < 0.0001);
            CATCH_REQUIRE(fabs(c[3][0] - div[3][0]) < 0.0001);
            CATCH_REQUIRE(fabs(c[3][1] - div[3][1]) < 0.0001);
            CATCH_REQUIRE(fabs(c[3][2] - div[3][2]) < 0.0001);
            CATCH_REQUIRE(fabs(c[3][3] - div[3][3]) < 0.0001);
        }
        CATCH_END_SECTION()

        CATCH_START_SECTION("matrix: a/=b")
        {
            // setup A
            //
            snapdev::matrix<double> a(4, 4);

            CATCH_REQUIRE(a.rows() == 4);
            CATCH_REQUIRE(a.columns() == 4);

            double const a00(frand());
            double const a01(frand());
            double const a02(frand());
            double const a03(frand());
            double const a10(frand());
            double const a11(frand());
            double const a12(frand());
            double const a13(frand());
            double const a20(frand());
            double const a21(frand());
            double const a22(frand());
            double const a23(frand());
            double const a30(frand());
            double const a31(frand());
            double const a32(frand());
            double const a33(frand());

            a[0][0] = a00;
            a[0][1] = a01;
            a[0][2] = a02;
            a[0][3] = a03;
            a[1][0] = a10;
            a[1][1] = a11;
            a[1][2] = a12;
            a[1][3] = a13;
            a[2][0] = a20;
            a[2][1] = a21;
            a[2][2] = a22;
            a[2][3] = a23;
            a[3][0] = a30;
            a[3][1] = a31;
            a[3][2] = a32;
            a[3][3] = a33;

            CATCH_REQUIRE(a[0][0] == a00);
            CATCH_REQUIRE(a[0][1] == a01);
            CATCH_REQUIRE(a[0][2] == a02);
            CATCH_REQUIRE(a[0][3] == a03);
            CATCH_REQUIRE(a[1][0] == a10);
            CATCH_REQUIRE(a[1][1] == a11);
            CATCH_REQUIRE(a[1][2] == a12);
            CATCH_REQUIRE(a[1][3] == a13);
            CATCH_REQUIRE(a[2][0] == a20);
            CATCH_REQUIRE(a[2][1] == a21);
            CATCH_REQUIRE(a[2][2] == a22);
            CATCH_REQUIRE(a[2][3] == a23);
            CATCH_REQUIRE(a[3][0] == a30);
            CATCH_REQUIRE(a[3][1] == a31);
            CATCH_REQUIRE(a[3][2] == a32);
            CATCH_REQUIRE(a[3][3] == a33);

            // setup B
            //
            snapdev::matrix<double> b(4, 4);

            CATCH_REQUIRE(b.rows() == 4);
            CATCH_REQUIRE(b.columns() == 4);

            double const b00(frand());
            double const b01(frand());
            double const b02(frand());
            double const b03(frand());
            double const b10(frand());
            double const b11(frand());
            double const b12(frand());
            double const b13(frand());
            double const b20(frand());
            double const b21(frand());
            double const b22(frand());
            double const b23(frand());
            double const b30(frand());
            double const b31(frand());
            double const b32(frand());
            double const b33(frand());

            b[0][0] = b00;
            b[0][1] = b01;
            b[0][2] = b02;
            b[0][3] = b03;
            b[1][0] = b10;
            b[1][1] = b11;
            b[1][2] = b12;
            b[1][3] = b13;
            b[2][0] = b20;
            b[2][1] = b21;
            b[2][2] = b22;
            b[2][3] = b23;
            b[3][0] = b30;
            b[3][1] = b31;
            b[3][2] = b32;
            b[3][3] = b33;

            CATCH_REQUIRE(b[0][0] == b00);
            CATCH_REQUIRE(b[0][1] == b01);
            CATCH_REQUIRE(b[0][2] == b02);
            CATCH_REQUIRE(b[0][3] == b03);
            CATCH_REQUIRE(b[1][0] == b10);
            CATCH_REQUIRE(b[1][1] == b11);
            CATCH_REQUIRE(b[1][2] == b12);
            CATCH_REQUIRE(b[1][3] == b13);
            CATCH_REQUIRE(b[2][0] == b20);
            CATCH_REQUIRE(b[2][1] == b21);
            CATCH_REQUIRE(b[2][2] == b22);
            CATCH_REQUIRE(b[2][3] == b23);
            CATCH_REQUIRE(b[3][0] == b30);
            CATCH_REQUIRE(b[3][1] == b31);
            CATCH_REQUIRE(b[3][2] == b32);
            CATCH_REQUIRE(b[3][3] == b33);

            double const determinant(b.determinant());
            snapdev::matrix<double> adjugate(b.adjugate());

            snapdev::matrix<double> inv(adjugate * (1.0 / determinant));
            snapdev::matrix<double> div(a * inv);

            // run operation A /= B
            //
            a /= b;

            // this could fail if one of bXX is 0.0
            //
            //CATCH_REQUIRE(a[0][0] == a00);
            //CATCH_REQUIRE(a[0][1] == a01);
            //CATCH_REQUIRE(a[0][2] == a02);
            //CATCH_REQUIRE(a[0][3] == a03);
            //CATCH_REQUIRE(a[1][0] == a10);
            //CATCH_REQUIRE(a[1][1] == a11);
            //CATCH_REQUIRE(a[1][2] == a12);
            //CATCH_REQUIRE(a[1][3] == a13);
            //CATCH_REQUIRE(a[2][0] == a20);
            //CATCH_REQUIRE(a[2][1] == a21);
            //CATCH_REQUIRE(a[2][2] == a22);
            //CATCH_REQUIRE(a[2][3] == a23);
            //CATCH_REQUIRE(a[3][0] == a30);
            //CATCH_REQUIRE(a[3][1] == a31);
            //CATCH_REQUIRE(a[3][2] == a32);
            //CATCH_REQUIRE(a[3][3] == a33);

            CATCH_REQUIRE(b[0][0] == b00);
            CATCH_REQUIRE(b[0][1] == b01);
            CATCH_REQUIRE(b[0][2] == b02);
            CATCH_REQUIRE(b[0][3] == b03);
            CATCH_REQUIRE(b[1][0] == b10);
            CATCH_REQUIRE(b[1][1] == b11);
            CATCH_REQUIRE(b[1][2] == b12);
            CATCH_REQUIRE(b[1][3] == b13);
            CATCH_REQUIRE(b[2][0] == b20);
            CATCH_REQUIRE(b[2][1] == b21);
            CATCH_REQUIRE(b[2][2] == b22);
            CATCH_REQUIRE(b[2][3] == b23);
            CATCH_REQUIRE(b[3][0] == b30);
            CATCH_REQUIRE(b[3][1] == b31);
            CATCH_REQUIRE(b[3][2] == b32);
            CATCH_REQUIRE(b[3][3] == b33);

            CATCH_REQUIRE(fabs(a[0][0] - div[0][0]) < 0.0001);
            CATCH_REQUIRE(fabs(a[0][1] - div[0][1]) < 0.0001);
            CATCH_REQUIRE(fabs(a[0][2] - div[0][2]) < 0.0001);
            CATCH_REQUIRE(fabs(a[0][3] - div[0][3]) < 0.0001);
            CATCH_REQUIRE(fabs(a[1][0] - div[1][0]) < 0.0001);
            CATCH_REQUIRE(fabs(a[1][1] - div[1][1]) < 0.0001);
            CATCH_REQUIRE(fabs(a[1][2] - div[1][2]) < 0.0001);
            CATCH_REQUIRE(fabs(a[1][3] - div[1][3]) < 0.0001);
            CATCH_REQUIRE(fabs(a[2][0] - div[2][0]) < 0.0001);
            CATCH_REQUIRE(fabs(a[2][1] - div[2][1]) < 0.0001);
            CATCH_REQUIRE(fabs(a[2][2] - div[2][2]) < 0.0001);
            CATCH_REQUIRE(fabs(a[2][3] - div[2][3]) < 0.0001);
            CATCH_REQUIRE(fabs(a[3][0] - div[3][0]) < 0.0001);
            CATCH_REQUIRE(fabs(a[3][1] - div[3][1]) < 0.0001);
            CATCH_REQUIRE(fabs(a[3][2] - div[3][2]) < 0.0001);
            CATCH_REQUIRE(fabs(a[3][3] - div[3][3]) < 0.0001);
        }
        CATCH_END_SECTION()
    }
}


CATCH_TEST_CASE("matrix_color", "[matrix][math]")
{
    // generate a few brightness matrices
    //
    CATCH_GIVEN("brightness")
    {
        CATCH_START_SECTION("matrix: b=a*<brightness> (a is identity)")
        {
            // setup A
            //
            snapdev::matrix<double> a(4, 4);

            CATCH_REQUIRE(a.rows() == 4);
            CATCH_REQUIRE(a.columns() == 4);

            // setup B
            //
            snapdev::matrix<double> b(4, 4);

            CATCH_REQUIRE(b.rows() == 4);
            CATCH_REQUIRE(b.columns() == 4);

            for(int idx(0); idx < 1000; ++idx)
            {
                double const brightness(static_cast<double>(idx) / 1000.0);
                b = a.brightness(brightness);

                CATCH_REQUIRE(fabs(b[0][0] - brightness) < 0.0001);
                CATCH_REQUIRE(fabs(b[0][1] - 0.0) < 0.0001);
                CATCH_REQUIRE(fabs(b[0][2] - 0.0) < 0.0001);
                CATCH_REQUIRE(fabs(b[0][3] - 0.0) < 0.0001);
                CATCH_REQUIRE(fabs(b[1][0] - 0.0) < 0.0001);
                CATCH_REQUIRE(fabs(b[1][1] - brightness) < 0.0001);
                CATCH_REQUIRE(fabs(b[1][2] - 0.0) < 0.0001);
                CATCH_REQUIRE(fabs(b[1][3] - 0.0) < 0.0001);
                CATCH_REQUIRE(fabs(b[2][0] - 0.0) < 0.0001);
                CATCH_REQUIRE(fabs(b[2][1] - 0.0) < 0.0001);
                CATCH_REQUIRE(fabs(b[2][2] - brightness) < 0.0001);
                CATCH_REQUIRE(fabs(b[2][3] - 0.0) < 0.0001);
                CATCH_REQUIRE(fabs(b[3][0] - 0.0) < 0.0001);
                CATCH_REQUIRE(fabs(b[3][1] - 0.0) < 0.0001);
                CATCH_REQUIRE(fabs(b[3][2] - 0.0) < 0.0001);
                CATCH_REQUIRE(fabs(b[3][3] - 1.0) < 0.0001);

//     *       brightness_r & 0 & 0 & 0
//     *    \\ 0 & brightness_g & 0 & 0
//     *    \\ 0 & 0 & brightness_b & 0
//     *    \\ 0 & 0 & 0 & 1

            }
        }
        CATCH_END_SECTION()
    }

    // generate a few saturation matrices
    //
    CATCH_GIVEN("saturation")
    {
        CATCH_START_SECTION("matrix: b=a*<saturation> (a is identity)")
        {
            // setup A
            //
            snapdev::matrix<double> a(4, 4);

            CATCH_REQUIRE(a.rows() == 4);
            CATCH_REQUIRE(a.columns() == 4);

            // setup B
            //
            snapdev::matrix<double> b(4, 4);

            CATCH_REQUIRE(b.rows() == 4);
            CATCH_REQUIRE(b.columns() == 4);

            double luma[6][3];

            luma[0][0] = a.HDTV_LUMA_RED  ;
            luma[0][1] = a.HDTV_LUMA_GREEN;
            luma[0][2] = a.HDTV_LUMA_BLUE ;

            luma[1][0] = a.LED_LUMA_RED  ;
            luma[1][1] = a.LED_LUMA_GREEN;
            luma[1][2] = a.LED_LUMA_BLUE ;

            luma[2][0] = a.CRT_LUMA_RED  ;
            luma[2][1] = a.CRT_LUMA_GREEN;
            luma[2][2] = a.CRT_LUMA_BLUE ;

            luma[3][0] = a.NTSC_LUMA_RED  ;
            luma[3][1] = a.NTSC_LUMA_GREEN;
            luma[3][2] = a.NTSC_LUMA_BLUE ;

            luma[4][0] = a.AVERAGE_LUMA_RED  ;
            luma[4][1] = a.AVERAGE_LUMA_GREEN;
            luma[4][2] = a.AVERAGE_LUMA_BLUE ;

            luma[5][0] = 0.2; // to test any dynamic version if such exists for this algorithm
            luma[5][1] = 0.7;
            luma[5][2] = 0.1;

            for(int l(0); l < 6; ++l)
            {
                // set the luma weights
                //
                a.set_luma_vector(luma[l][0], luma[l][1], luma[l][2]);

                for(int idx(0); idx < 1000; ++idx)
                {
                    double const s(static_cast<double>(idx) / 1000.0);
                    b = a.saturation(s);

                    CATCH_REQUIRE(fabs(b[0][0] - (luma[l][0] * (1.0 - s) + s)) < 0.0001);
                    CATCH_REQUIRE(fabs(b[0][1] - (luma[l][0] * (1.0 - s))) < 0.0001);
                    CATCH_REQUIRE(fabs(b[0][2] - (luma[l][0] * (1.0 - s))) < 0.0001);
                    CATCH_REQUIRE(fabs(b[0][3] - (0.0)) < 0.0001);
                    CATCH_REQUIRE(fabs(b[1][0] - (luma[l][1] * (1.0 - s))) < 0.0001);
                    CATCH_REQUIRE(fabs(b[1][1] - (luma[l][1] * (1.0 - s) + s)) < 0.0001);
                    CATCH_REQUIRE(fabs(b[1][2] - (luma[l][1] * (1.0 - s))) < 0.0001);
                    CATCH_REQUIRE(fabs(b[1][3] - (0.0)) < 0.0001);
                    CATCH_REQUIRE(fabs(b[2][0] - (luma[l][2] * (1.0 - s))) < 0.0001);
                    CATCH_REQUIRE(fabs(b[2][1] - (luma[l][2] * (1.0 - s))) < 0.0001);
                    CATCH_REQUIRE(fabs(b[2][2] - (luma[l][2] * (1.0 - s) + s)) < 0.0001);
                    CATCH_REQUIRE(fabs(b[2][3] - (0.0)) < 0.0001);
                    CATCH_REQUIRE(fabs(b[3][0] - (0.0)) < 0.0001);
                    CATCH_REQUIRE(fabs(b[3][1] - (0.0)) < 0.0001);
                    CATCH_REQUIRE(fabs(b[3][2] - (0.0)) < 0.0001);
                    CATCH_REQUIRE(fabs(b[3][3] - (1.0)) < 0.0001);
                }
            }
        }
        CATCH_END_SECTION()
    }

    // generate a few brightness matrices
    //
    CATCH_GIVEN("hue")
    {
        CATCH_START_SECTION("matrix: b=a*<hue> (a is identity)")
        {
            // setup A
            //
            snapdev::matrix<double> a(4, 4);

            CATCH_REQUIRE(a.rows() == 4);
            CATCH_REQUIRE(a.columns() == 4);

            // setup B
            //
            snapdev::matrix<double> b(4, 4);

            CATCH_REQUIRE(b.rows() == 4);
            CATCH_REQUIRE(b.columns() == 4);

            double luma[6][3];

            luma[0][0] = a.HDTV_LUMA_RED  ;
            luma[0][1] = a.HDTV_LUMA_GREEN;
            luma[0][2] = a.HDTV_LUMA_BLUE ;

            luma[1][0] = a.LED_LUMA_RED  ;
            luma[1][1] = a.LED_LUMA_GREEN;
            luma[1][2] = a.LED_LUMA_BLUE ;

            luma[2][0] = a.CRT_LUMA_RED  ;
            luma[2][1] = a.CRT_LUMA_GREEN;
            luma[2][2] = a.CRT_LUMA_BLUE ;

            luma[3][0] = a.NTSC_LUMA_RED  ;
            luma[3][1] = a.NTSC_LUMA_GREEN;
            luma[3][2] = a.NTSC_LUMA_BLUE ;

            luma[4][0] = a.AVERAGE_LUMA_RED  ;
            luma[4][1] = a.AVERAGE_LUMA_GREEN;
            luma[4][2] = a.AVERAGE_LUMA_BLUE ;

            luma[5][0] = 0.2; // to test any dynamic version if such exists for this algorithm
            luma[5][1] = 0.7;
            luma[5][2] = 0.1;

            std::vector<std::string> last_hue_matrix{
                "hdtv",
                "led",
                "crt",
                "ntsc",
                "average",
                "dynamic"
            };

            for(int l(0); l < 6; ++l)
            {
                // set the luma weights
                //
                a.set_luma_vector(luma[l][0], luma[l][1], luma[l][2]);

//std::cerr << "*** working on luma " << l << "\n";
                for(int idx(0); idx < 1000; ++idx)
                {
                    // hue is an angle from 0 to 360 (albeit in radian)
                    //
                    double const hue(static_cast<double>(idx) / 1000.0 * M_PI * 2.0);
                    b = a.hue(hue);

#ifdef _DEBUG
                    CATCH_REQUIRE(a.get_last_hue_matrix() == last_hue_matrix[l]);
#endif

                    // this one requires us to work!
                    // here we calculate the hue matrix by hand instead of using
                    // pre-calculated numbers; that way we know we have it right
                    //

                    // $R_r$ (red rotation)
                    //
                    snapdev::matrix<double> r_r(4, 4);
                    r_r[1][1] =  1.0 / sqrt(2.0);
                    r_r[1][2] =  1.0 / sqrt(2.0);
                    r_r[2][1] = -1.0 / sqrt(2.0);
                    r_r[2][2] =  1.0 / sqrt(2.0);

                    // $R_g$ (green rotation)
                    //
                    snapdev::matrix<double> r_g(4, 4);
                    r_g[0][0] =  sqrt(2.0) / sqrt(3.0);
                    r_g[0][2] =  1.0 / sqrt(3.0);
                    r_g[2][0] = -1.0 / sqrt(3.0);
                    r_g[2][2] =  sqrt(2.0) / sqrt(3.0);

                    // $R_rg = R_r R_g$ (red and green rotations combined)
                    //
                    snapdev::matrix<double> r_rg(r_r * r_g);

                    // $w$ (luma vector, a.k.a. color weights to calcalute the perfect grayscale for your monitor)
                    //
                    snapdev::matrix<double> w(a.get_luma_vector());
                    //w[0][0] = a.RED_WEIGHT;
                    //w[1][0] = a.GREEN_WEIGHT;
                    //w[2][0] = a.BLUE_WEIGHT;
                    //w[3][0] = 0;

                    // sm (skew matrix)
                    //
                    snapdev::matrix<double> sm(r_rg * w);

                    // s (scaling with skew matrix)
                    //
                    snapdev::matrix<double> s(4, 4);
                    s[0][2] = sm[0][0] / sm[2][0];
                    s[1][2] = sm[1][0] / sm[2][0];

                    // p (skewed red & green rotation)
                    //
                    snapdev::matrix<double> p(r_rg);
                    p *= s;

                    // r_b (blue rotation)
                    //
                    snapdev::matrix<double> r_b(4, 4);
                    double const rot_cos(cos(hue));
                    double const rot_sin(sin(hue));
                    r_b[0][0] =  rot_cos;
                    r_b[0][1] =  rot_sin;
                    r_b[1][0] = -rot_sin;
                    r_b[1][1] =  rot_cos;

                    // hue_matrix (the resulting matrix depending on angle)
                    //
                    snapdev::matrix<double> hue_matrix(p * r_b / p);

//std::cout << "Got b = " << b << "\n";
//std::cout << "Got our hue matrix = " << hue_matrix << "\n";

                    CATCH_REQUIRE(fabs(b[0][0] - hue_matrix[0][0]) < 0.0001);
                    CATCH_REQUIRE(fabs(b[0][1] - hue_matrix[0][1]) < 0.0001);
                    CATCH_REQUIRE(fabs(b[0][2] - hue_matrix[0][2]) < 0.0001);
                    CATCH_REQUIRE(fabs(b[0][3] - hue_matrix[0][3]) < 0.0001);
                    CATCH_REQUIRE(fabs(b[1][0] - hue_matrix[1][0]) < 0.0001);
                    CATCH_REQUIRE(fabs(b[1][1] - hue_matrix[1][1]) < 0.0001);
                    CATCH_REQUIRE(fabs(b[1][2] - hue_matrix[1][2]) < 0.0001);
                    CATCH_REQUIRE(fabs(b[1][3] - hue_matrix[1][3]) < 0.0001);
                    CATCH_REQUIRE(fabs(b[2][0] - hue_matrix[2][0]) < 0.0001);
                    CATCH_REQUIRE(fabs(b[2][1] - hue_matrix[2][1]) < 0.0001);
                    CATCH_REQUIRE(fabs(b[2][2] - hue_matrix[2][2]) < 0.0001);
                    CATCH_REQUIRE(fabs(b[2][3] - hue_matrix[2][3]) < 0.0001);
                    CATCH_REQUIRE(fabs(b[3][0] - hue_matrix[3][0]) < 0.0001);
                    CATCH_REQUIRE(fabs(b[3][1] - hue_matrix[3][1]) < 0.0001);
                    CATCH_REQUIRE(fabs(b[3][2] - hue_matrix[3][2]) < 0.0001);
                    CATCH_REQUIRE(fabs(b[3][3] - hue_matrix[3][3]) < 0.0001);

//     * $$
//     * R_r =
//     * \begin{bmatrix}
//     *       1 &  0                  & 0                  & 0
//     *    \\ 0 &  {1 \over \sqrt 2 } & {1 \over \sqrt 2 } & 0
//     *    \\ 0 & -{1 \over \sqrt 2 } & {1 \over \sqrt 2 } & 0
//     *    \\ 0 &  0                  & 0                  & 1
//     * \end{bmatrix}
//     * $$
//     *
//     * $$
//     * R_g =
//     * \begin{bmatrix}
//     *       1 & 0 & 0 & 0
//     *    \\ 0 & {\sqrt 2 \over \sqrt 3} & -{1 \over \sqrt 3} & 0
//     *    \\ 0 & {1 \over \sqrt 3} & {\sqrt 2 \over \sqrt 3} & 0
//     *    \\ 0 & 0 & 0 & 1
//     * \end{bmatrix}
//     * $$
//     *
//     * $$
//     * R_{rg} = R_r R_g
//     * $$
//     *
//     * $$
//     * \begin{bmatrix}
//     *        L_r
//     *     \\ L_g
//     *     \\ L_b
//     *     \\ 0
//     * \end{bmatrix}
//     * =
//     * R_{rg}
//     * \begin{bmatrix}
//     *        W_r
//     *     \\ W_g
//     *     \\ W_b
//     *     \\ 0
//     * \end{bmatrix}
//     * $$
//     *
//     * Now we define a rotation matrix for the blue axis. This one uses
//     * a variable as the angle. This is the actual rotation angle which
//     * can go from 0 to $2 \pi$:
//     *
//     * $$
//     * R_b =
//     * \begin{bmatrix}
//     *       cos( \theta )  & sin( \theta ) & 0 & 0
//     *    \\ -sin( \theta ) & cos( \theta ) & 0 & 0
//     *    \\ 0              & 0             & 1 & 0
//     *    \\ 0              & 0             & 0 & 1
//     * \end{bmatrix}
//     * $$
//     *
//     * Now we have all the matrices to caculate the hue rotation
//     * of all the components of an image:
//     *
//     * $$
//     * H = R_{rg} S R_b S^{-1} R_{rg}^{-1}
//     * $$
//     *
//     * $H$ can then be used as in:
//     *
//     * $$
//     * \begin{bmatrix}
//     *      R'
//     *   \\ G'
//     *   \\ B'
//     * \end{bmatrix}
//     * =
//     * H
//     * \begin{bmatrix}
//     *      R
//     *   \\ G
//     *   \\ B
//     * \end{bmatrix}
//     * $$
//     *
                }
            }
        }
        CATCH_END_SECTION()
    }
}


// vim: ts=4 sw=4 et
