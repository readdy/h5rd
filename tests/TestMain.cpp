/********************************************************************
 * Copyright © 2017 Computational Molecular Biology Group,          * 
 *                  Freie Universität Berlin (GER)                  *
 *                                                                  *
 * This file is part of ReaDDy.                                     *
 *                                                                  *
 * ReaDDy is free software: you can redistribute it and/or modify   *
 * it under the terms of the GNU Lesser General Public License as   *
 * published by the Free Software Foundation, either version 3 of   *
 * the License, or (at your option) any later version.              *
 *                                                                  *
 * This program is distributed in the hope that it will be useful,  *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of   *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the    *
 * GNU Lesser General Public License for more details.              *
 *                                                                  *
 * You should have received a copy of the GNU Lesser General        *
 * Public License along with this program. If not, see              *
 * <http://www.gnu.org/licenses/>.                                  *
 ********************************************************************/


/**
 * << detailed description >>
 *
 * @file TestMain.cpp
 * @brief << brief description >>
 * @author clonker
 * @date 04.09.17
 * @copyright GNU Lesser General Public License v3.0
 */

#include "gtest/gtest.h"
#include "h5rd/File.h"

namespace {

TEST(TestH5ReaDDy, Sanity) {
    using namespace h5rd;

    {
        File f("test.h5", File::Action::CREATE, File::Flag::OVERWRITE);
        std::cout << "/ exists: " << f.exists("/") << std::endl;
        auto g = f.createGroup("/foo/bar");
        std::cout << "foo/bar exists: " << f.exists("/foo/bar") << std::endl;

        g.write("miau", "blubs");
        std::vector<int> zahlen{1,2,3,4,5,6,7};
        g.write("zahlen", zahlen);
        std::vector<int> readZahlen;
        g.read("zahlen", readZahlen);
        for(auto x : readZahlen) {
            std::cout << x;
        }
        std::cout << std::endl;

        f.close();

        File f2("test.h5", File::Action::CREATE, File::Flag::OVERWRITE);
        std::cout << "/ exists: " << f2.exists("/") << std::endl;
        auto g2 = f2.createGroup("/foo/bar");
        std::cout << "foo/bar exists: " << f2.exists("/foo/bar") << std::endl;
    }


}

}
