// LTCPPUNIT - <URL>
//
// Copyright (C) 2011 Fabien Bouleau
//
// This file is part of LTCPPUNIT.
//
// LTCPPUNIT is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// LTCPPUNIT is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with LTCPPUNIT.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __LTCPPUNIT_LTCPPUNIT_H
#define __LTCPPUNIT_LTCPPUNIT_H

#include <string>
#include <vector>
#include <ltcppunit/common.h>

#define FMT_DEFAULT 0
#define FMT_DUMP 1
#define FMT_MULTILINE 2

class AssertError
{
public:
    std::string _reason;
    AssertError(const std::string, const UINT8, const UINT8);
    AssertError(const std::string, const std::string, const std::string, const int);
};

class UnitTest
{
public:
    class TestSuite;

    class TestCase
    {
    public:
        TestCase(TestSuite&);
        std::string _name;
        virtual int run() = 0;
    };

    class TestSuite
    {
    private:
        std::vector<TestCase*> _items;

    public:
        std::string _name;

        TestSuite();

        std::vector<TestCase*> getItems();
        TestSuite& addItem(TestCase*);
    };

private:
    static std::vector<TestSuite*>* _tests;

public:
    static int _verbosity;

    UnitTest();

    // Dump functions
    static std::vector<std::string> array_dump(const std::vector<BYTE>);
    static std::vector<std::string> array_dump(const std::vector<BYTE>, int);
    static std::string dump(const std::string);
    static std::string dump(const std::vector<BYTE>);
    static std::string dump(const std::string, const std::string);
    static std::string dump(const std::vector<BYTE>, const std::vector<BYTE>);

    // Assert functions
    static int assert(const std::string, const UINT8, const UINT8);
    static int assert(const std::string, const std::string, const std::string);
    static int assert(const std::string, const std::string, const std::string, const int);
    static int assert(const std::string, const char*, const char*);
    static int assert(const std::string, const char*, const char*, const int);

    static int run();
};

#endif
