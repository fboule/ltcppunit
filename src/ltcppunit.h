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

#ifndef __LTCPPUNIT_H
#define __LTCPPUNIT_H

#include <string>
#include <vector>
#include "common.h"

using namespace std;

#define FMT_DEFAULT 0
#define FMT_DUMP 1

class AssertError
{
public:
    string _reason;
    AssertError(const string, const UINT8, const UINT8);
    AssertError(const string, const string, const string, const int);
};

class UnitTest
{
public:
    class TestSuite;

    class TestCase
    {
    public:
        TestCase(TestSuite&);
        string _name;
        virtual int run() = 0;
    };

    class TestSuite
    {
    private:
        vector<TestCase*> _items;

    public:
        string _name;

        TestSuite();

        vector<TestCase*> getItems();
        TestSuite& addItem(TestCase*);
    };

private:
    static vector<TestSuite*>* _tests;

public:
    UnitTest();
    static int assert(const string, const UINT8, const UINT8);
    static int assert(const string, const string, const string);
    static int assert(const string, const string, const string, const int);
    static int assert(const string, const char*, const char*);
    static int assert(const string, const char*, const char*, const int);
    static int run();
};

#endif
