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
        std::vector<int> zahlen{1, 2, 3, 4, 5, 6, 7};
        g.write("zahlen", zahlen);
        std::vector<int> readZahlen;
        g.read("zahlen", readZahlen);
        for (auto x : readZahlen) {
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

struct Stuff {
    int a;
    float x;
    std::array<short, 3> xyz;
};

auto getCompoundTypes = [](h5rd::Object *parentFile) {
    using namespace h5rd;
    NativeCompoundType stuffType = NativeCompoundTypeBuilder(sizeof(Stuff), parentFile)
            .insert<decltype(std::declval<Stuff>().a)>("a", offsetof(Stuff, a))
            .insert<decltype(std::declval<Stuff>().x)>("x", offsetof(Stuff, x))
            .insertStdArray<decltype(std::declval<Stuff>().xyz)>("xyz", offsetof(Stuff, xyz))
            .build();
    STDCompoundType stdStuffType(stuffType);
    return std::make_tuple(std::move(stuffType), std::move(stdStuffType));
};

TEST(TestH5ReaDDy, ReadWriteCompoundType) {
    using namespace h5rd;
    {
        File f("test.h5", File::Action::CREATE, File::Flag::OVERWRITE);
        auto group = f.createGroup("/my/compound/group");

        Stuff s1{3, 5.f, {{1, 1, 1}}};
        Stuff s2{4, 5.f, {{2, 2, 2}}};
        Stuff s3{6, 7.f, {{3, 3, 3}}};
        std::vector<Stuff> stuffs{s1, s2, s3};

        auto types = getCompoundTypes(f.parentFile());

        auto ds = group.createDataSet("stuffs", {3}, {UNLIMITED_DIMS}, std::get<0>(types), std::get<1>(types));
        ds->append({stuffs.size()}, stuffs.data());

    }
    {
        File f("test.h5", File::Action::OPEN, File::Flag::READ_ONLY);
        auto group = f.subgroup("/my/compound/group");
        auto types = getCompoundTypes(f.parentFile());
        std::vector<Stuff> stuffs;
        group.read("stuffs", stuffs, &std::get<0>(types), &std::get<1>(types));

        ASSERT_EQ(stuffs.size(), 3);
        for (const auto &s : stuffs) {
            std::cout << "stuff: a=" << s.a << ", x=" << s.x << ", xyz=" << s.xyz.at(0) << "," << s.xyz.at(1) << ","
                      << s.xyz.at(2) << std::endl;
        }
    }

}

TEST(TestH5ReaDDy, ReadWriteCompoundTypeClose) {
    using namespace h5rd;
    std::unique_ptr<File> f = std::make_unique<File>("test.h5", File::Action::CREATE, File::Flag::OVERWRITE);
    auto group = f->createGroup("/my/compound/group");

    std::vector<Stuff> stuffs;
    {
        Stuff s1{1, 1.f, {{1, 1, 1}}};
        Stuff s2{2, 2.f, {{2, 2, 2}}};
        Stuff s3{3, 3.f, {{3, 3, 3}}};
        stuffs.push_back(s1);
        stuffs.push_back(s2);
        stuffs.push_back(s3);
    }

    auto types = getCompoundTypes(f->parentFile());


    auto ds = group.createDataSet("stuffs", {3}, {UNLIMITED_DIMS}, std::get<0>(types), std::get<1>(types));
    {
        ds->append({stuffs.size()}, stuffs.data());
        ds->append({stuffs.size()}, stuffs.data());
        f->close();
        ASSERT_TRUE(f->closed());

        f.reset();
    }

    {
        File f2("test.h5", File::Action::OPEN, File::Flag::READ_ONLY);
        auto group2 = f2.subgroup("/my/compound/group");
        auto types2 = getCompoundTypes(f2.parentFile());
        std::vector<Stuff> stuffs2;
        group2.read("stuffs", stuffs2, &std::get<0>(types2), &std::get<1>(types2));

        ASSERT_EQ(stuffs2.size(), 6);

        int i = 1;
        for (const auto &s : stuffs2) {
            ASSERT_EQ(s.a, 1 + ((i - 1) % 3));
            ASSERT_EQ(s.x, 1 + ((i - 1) % 3));
            ASSERT_EQ(s.xyz.at(0), 1 + ((i - 1) % 3));
            ASSERT_EQ(s.xyz.at(1), 1 + ((i - 1) % 3));
            ASSERT_EQ(s.xyz.at(2), 1 + ((i - 1) % 3));
            ++i;
        }
    }

    File f3("test.h5", File::Action::CREATE, File::Flag::OVERWRITE);
    auto gtest123 = f3.createGroup("/test123");

    gtest123.write("hier", "stehtwas");

    auto types123 = getCompoundTypes(f3.parentFile());
    auto ds213 = gtest123.createDataSet("stuffs", {3}, {UNLIMITED_DIMS}, std::get<0>(types123), std::get<1>(types123));
    ds.reset();
    ds213->append({stuffs.size()}, stuffs.data());


}

}
