// Copyright (c) 2021-2023  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Verify that the path functions work.
 *
 * This file implements tests for the pathinfo function.
 */

// self
//
#include    <snapdev/glob_to_list.h>

#include    "catch_main.h"


// C++ lib
//
#include    <fstream>
#include    <set>


// last include
//
#include    <snapdev/poison.h>




CATCH_TEST_CASE("glob_to_list", "[glob_to_list]")
{
    CATCH_START_SECTION("pathinfo: replace existing suffix")
    {
        std::string top_dir(SNAP_CATCH2_NAMESPACE::g_tmp_dir() + "/top");
        int r(system(("rm -rf " + top_dir).c_str()));
        CATCH_REQUIRE(r == 0);
        CATCH_REQUIRE(mkdir(top_dir.c_str(), 0777) == 0);

        std::string foo_dir(top_dir + "/foo");
        CATCH_REQUIRE(mkdir(foo_dir.c_str(), 0777) == 0);

        std::string bar_dir(top_dir + "/bar");
        CATCH_REQUIRE(mkdir(bar_dir.c_str(), 0777) == 0);

        std::string sub_bar_dir(bar_dir + "/sub");
        CATCH_REQUIRE(mkdir(sub_bar_dir.c_str(), 0777) == 0);

        std::string hidden_bar_dir(sub_bar_dir + "/hidden");
        CATCH_REQUIRE(mkdir(hidden_bar_dir.c_str(), 0777) == 0);

        std::string foo_txt(foo_dir + "/foo.txt");
        std::ofstream foo_file;
        foo_file.open(foo_txt);
        foo_file << "foo file\n";
        CATCH_REQUIRE(foo_file.good());

        std::string foo2_txt(foo_dir + "/foo-2.txt");
        std::ofstream foo2_file;
        foo2_file.open(foo2_txt);
        foo2_file << "foo #2 file\n";
        CATCH_REQUIRE(foo2_file.good());

        std::string bar_txt(bar_dir + "/bar.txt");
        std::ofstream bar_file;
        bar_file.open(bar_txt);
        bar_file << "bar file\n";
        CATCH_REQUIRE(bar_file.good());

        std::string bar2_txt(bar_dir + "/bar_2.txt");
        std::ofstream bar2_file;
        bar2_file.open(bar2_txt);
        bar2_file << "bar two file\n";
        CATCH_REQUIRE(bar2_file.good());

        std::string sub_bar3_txt(sub_bar_dir + "/sub-bar3.txt");
        std::ofstream sub_bar3_file;
        sub_bar3_file.open(sub_bar3_txt);
        sub_bar3_file << "no. 3 -- sub-bar file\n";
        CATCH_REQUIRE(sub_bar3_file.good());

        std::string hidden_settings_conf(hidden_bar_dir + "/settings.conf");
        std::ofstream hidden_settings_file;
        hidden_settings_file.open(hidden_settings_conf);
        hidden_settings_file << "hidden=\"config file\"\n";
        CATCH_REQUIRE(hidden_settings_file.good());

        // soft links to a file are kept
        //
        std::string soft_link_txt(sub_bar_dir + "/soft-link.txt");
        CATCH_REQUIRE(symlink("../../foo/foo-2.txt", soft_link_txt.c_str()) == 0);

        // soft links to a directory are ignored by default
        //
        std::string soft_dir(sub_bar_dir + "/soft-dir");
        CATCH_REQUIRE(symlink("../../bar", soft_dir.c_str()) == 0);

        // soft links which loops forever
        //
        std::string loop_dir(hidden_bar_dir + "/loop.lnk");
        CATCH_REQUIRE(symlink("../hidden", loop_dir.c_str()) == 0);

        // read all the files (*)
        //
        typedef std::set<std::string> strings_t;
        snapdev::glob_to_list<strings_t> all_glob;
        CATCH_REQUIRE(all_glob.read_path<
                snapdev::glob_to_list_flag_t::GLOB_FLAG_RECURSIVE>(top_dir + "/*"));

        // soft-links are followed and the realpath() function returns
        // the target; here we "computed" what the glob() is expected to
        // return (i.e. not the target)
        //
        std::string const soft_link_link(all_glob.get_real_path(sub_bar_dir) + "/soft-link.txt");
        std::string const soft_dir_link(all_glob.get_real_path(sub_bar_dir) + "/soft-dir");
        std::string const loop_dir_link(all_glob.get_real_path(hidden_bar_dir) + "/loop.lnk");

        CATCH_REQUIRE(all_glob.size() == 13);
        CATCH_REQUIRE(all_glob.count(all_glob.get_real_path(bar_dir)) == 1);
        CATCH_REQUIRE(all_glob.count(all_glob.get_real_path(bar_txt)) == 1);
        CATCH_REQUIRE(all_glob.count(all_glob.get_real_path(bar2_txt)) == 1);
        CATCH_REQUIRE(all_glob.count(all_glob.get_real_path(sub_bar_dir)) == 1);
        CATCH_REQUIRE(all_glob.count(all_glob.get_real_path(hidden_bar_dir)) == 1);
        CATCH_REQUIRE(all_glob.count(loop_dir_link) == 1);
        CATCH_REQUIRE(all_glob.count(all_glob.get_real_path(hidden_settings_conf)) == 1);
        CATCH_REQUIRE(all_glob.count(soft_dir_link) == 1);
        CATCH_REQUIRE(all_glob.count(soft_link_link) == 1);
        CATCH_REQUIRE(all_glob.count(all_glob.get_real_path(sub_bar3_txt)) == 1);
        CATCH_REQUIRE(all_glob.count(all_glob.get_real_path(foo_dir)) == 1);
        CATCH_REQUIRE(all_glob.count(all_glob.get_real_path(foo2_txt)) == 1);
        CATCH_REQUIRE(all_glob.count(all_glob.get_real_path(foo_txt)) == 1);

        // read all text files (*.txt)
        //
        typedef std::set<std::string> strings_t;
        snapdev::glob_to_list<strings_t> txt_glob;
        CATCH_REQUIRE(txt_glob.read_path<
                snapdev::glob_to_list_flag_t::GLOB_FLAG_RECURSIVE>(top_dir + "/*.txt"));

        CATCH_REQUIRE(txt_glob.size() == 6);
        CATCH_REQUIRE(txt_glob.count(txt_glob.get_real_path(bar_txt)) == 1);
        CATCH_REQUIRE(txt_glob.count(txt_glob.get_real_path(bar2_txt)) == 1);
        CATCH_REQUIRE(txt_glob.count(txt_glob.get_real_path(soft_link_link)) == 1);
        CATCH_REQUIRE(txt_glob.count(txt_glob.get_real_path(sub_bar3_txt)) == 1);
        CATCH_REQUIRE(txt_glob.count(txt_glob.get_real_path(foo2_txt)) == 1);
        CATCH_REQUIRE(txt_glob.count(txt_glob.get_real_path(foo_txt)) == 1);

        // read text file within the bar sub-directory
        //
        typedef std::set<std::string> strings_t;
        snapdev::glob_to_list<strings_t> sub_bar_glob;
        CATCH_REQUIRE(sub_bar_glob.read_path<
                snapdev::glob_to_list_flag_t::GLOB_FLAG_RECURSIVE>(sub_bar_dir + "/*.txt"));

        CATCH_REQUIRE(sub_bar_glob.size() == 2);
        CATCH_REQUIRE(sub_bar_glob.count(soft_link_link) == 1);
        CATCH_REQUIRE(sub_bar_glob.count(sub_bar_glob.get_real_path(sub_bar3_txt)) == 1);

        // read text files within the bar sub-directory and follow symlinks
        //
        typedef std::set<std::string> strings_t;
        snapdev::glob_to_list<strings_t> symlink_glob;
        CATCH_REQUIRE(symlink_glob.read_path<
                snapdev::glob_to_list_flag_t::GLOB_FLAG_FOLLOW_SYMLINK
              , snapdev::glob_to_list_flag_t::GLOB_FLAG_RECURSIVE>(sub_bar_dir + "/*.txt"));

        CATCH_REQUIRE(symlink_glob.size() == 4);
        CATCH_REQUIRE(symlink_glob.count(symlink_glob.get_real_path(bar_txt)) == 1);
        CATCH_REQUIRE(symlink_glob.count(symlink_glob.get_real_path(bar2_txt)) == 1);
        CATCH_REQUIRE(symlink_glob.count(soft_link_link) == 1);
        CATCH_REQUIRE(symlink_glob.count(symlink_glob.get_real_path(sub_bar3_txt)) == 1);

    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
