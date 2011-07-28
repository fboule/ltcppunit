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

#include <sstream>
#include <iostream>

#include "ltcppunit.h"

using namespace std;

vector<UnitTest::TestSuite*>* UnitTest::_tests = NULL;
int UnitTest::_verbosity = 0;

UnitTest::UnitTest()
{
    if (UnitTest::_tests == NULL)
        UnitTest::_tests = new vector<UnitTest::TestSuite*>;
}

vector<string> UnitTest::array_dump(const vector<BYTE> v1)
{
    return UnitTest::array_dump(v1, v1.size());
}

vector<string> UnitTest::array_dump(const vector<BYTE> v1, int size)
{
    vector<string> res;
    string s;
    ostringstream ost, ost2;
    int width = 16;
    int length;

    if(size == 0)
        return res;

    length = (size / width) * width;
    length += (size > length ? 16 : 0);

    for(int i = 0 ; i < length ; i++)
    {
        if(i > 0 && (i % width) == 0)
        {
            if(i > v1.size() && i - v1.size() >= width)
                res.push_back("");
            else
            {
                s = ost.str() + ": " + ost2.str();
                res.push_back(s);
                ost.str("");
                ost2.str("");
            }
        }

        if(i < v1.size())
        {
            ost.width(2);
            ost.fill('0');
            ost << hex << (int) v1[i];
            ost2 << (char) (v1[i] > 31 && v1[i] < 128 ? v1[i] : '.');
            if(i < v1.size() - 1)
                ost << " ";
        }
        else if(i - v1.size() < width)
        {
            ost << "   ";
            ost2 << ' ';
        }
    }

    return res;
}

string UnitTest::dump(const vector<BYTE> v1)
{
    string s;
    vector<string> as;

    as = UnitTest::array_dump(v1);

    for(vector<string>::iterator it = as.begin() ; it != as.end() ; it++)
        s += *it + "\n";

    return s;
}

string UnitTest::dump(const string v1)
{
    vector<BYTE> ab;

    for(int i = 0 ; i < v1.size() ; i++)
        ab.push_back(v1[i]);

    return UnitTest::dump(ab);
}

string UnitTest::dump(const vector<BYTE> v1, const vector<BYTE> v2)
{
    string s;
    vector<string> as1, as2;
    vector<BYTE> ab1, ab2;
    vector<string>::iterator it1, it2;

    ab1 = v1;
    ab2 = v2;

    if(max(v1.size(), v2.size()) == 0)
        return string("");

    as1 = UnitTest::array_dump(ab1, max(v1.size(), v2.size()));
    as2 = UnitTest::array_dump(ab2, max(v1.size(), v2.size()));

    for(it1 = as1.begin(), it2 = as2.begin() ; it1 != as1.end() ; it1++, it2++)
        s += *it1 + " -- " + *it2 + "\n";

    return s;
}

string UnitTest::dump(const string v1, const string v2)
{
    vector<BYTE> ab1, ab2;

    for(int i = 0 ; i < v1.size() ; i++)
        ab1.push_back(v1[i]);

    for(int i = 0 ; i < v2.size() ; i++)
        ab2.push_back(v2[i]);

    return UnitTest::dump(ab1, ab2);
}

AssertError::AssertError(const string s, const string v1, const string v2, const int format)
{
    ostringstream ost;

    switch(format)
    {
    case FMT_DUMP:
        ost << s << endl << UnitTest::dump(v1, v2);
        break;
    case FMT_MULTILINE:
        ost << s << endl << v1 << "---" << endl << v2;
        break;
    case FMT_DEFAULT:
    default:
        ost << s << " " << v1 << " != " << v2 << endl;
        break;
    }
        
    this->_reason = ost.str();
}

AssertError::AssertError(const string s, const UINT8 v1, const UINT8 v2)
{
    ostringstream ost;
    ost << s << " " << v1 << " != " << v2;
    this->_reason = ost.str();
}

int UnitTest::assert(const string s, const string v1, const string v2)
{
    if(UnitTest::_verbosity == 1)
        cout << "Checking " << s << " " << v1 << " == " << v2 << "... ";

    if(v1 == v2)
    {
        (UnitTest::_verbosity == 1) && cout << "ok" << endl;
        return 0;
    }
    else
    {
        (UnitTest::_verbosity == 1) && cout << "no" << endl;
        throw AssertError(s, v1, v2, FMT_DEFAULT);
    }
}

int UnitTest::assert(const string s, const char* v1, const char* v2)
{
    if(UnitTest::_verbosity == 1)
        cout << "Checking " << s << " " << v1 << " == " << v2 << "... ";

    if(string(v1) == string(v2))
    {
        (UnitTest::_verbosity == 1) && cout << "ok" << endl;
        return 0;
    }
    else
    {
        (UnitTest::_verbosity == 1) && cout << "no" << endl;
        throw AssertError(s, string(v1), string(v2), FMT_DEFAULT);
    }
}

int UnitTest::assert(const string s, const string v1, const string v2, const int format)
{
    string dmp;

    if(UnitTest::_verbosity == 1)
    {
        if(format == FMT_DUMP)
        {
            cout << "Checking " << s << "... ";
            dmp = UnitTest::dump(v1, v2);
        }
        else if(format == FMT_MULTILINE)
        {
            cout << "Checking " << s << "... ";
            dmp = v1 + "---\n" + v2;
        }
        else
            cout << "Checking " << s << " " << v1 << " == " << v2 << "... ";
    }

    if(v1 == v2)
    {
        (UnitTest::_verbosity == 1) && cout << "ok" << endl << dmp;
        return 0;
    }
    else
    {
        (UnitTest::_verbosity == 1) && cout << "no" << endl << dmp;
        throw AssertError(s, v1, v2, format);
    }
}

int UnitTest::assert(const string s, const char* v1, const char* v2, const int format)
{
    string dmp;

    if(UnitTest::_verbosity == 1)
    {
        if(format == FMT_DUMP)
        {
            cout << "Checking " << s << "... ";
            dmp = UnitTest::dump(string(v1), string(v2));
        }
        else
            cout << "Checking " << s << " " << v1 << " == " << v2 << "... ";
    }

    if(string(v1) == string(v2))
    {
        (UnitTest::_verbosity == 1) && cout << "ok" << endl << dmp;
        return 0;
    }
    else
    {
        (UnitTest::_verbosity == 1) && cout << "no" << endl << dmp;
        throw AssertError(s, string(v1), string(v2), format);
    }
}

int UnitTest::assert(const string s, const UINT8 v1, const UINT8 v2)
{
    if(UnitTest::_verbosity == 1)
        cout << "Checking " << s << " " << v1 << " == " << v2 << "... ";

    if(v1 == v2)
    {
        (UnitTest::_verbosity == 1) && cout << "ok" << endl;
        return 0;
    }
    else
    {
        (UnitTest::_verbosity == 1) && cout << "no" << endl;
        throw AssertError(s, v1, v2);
    }
}

int UnitTest::run()
{
    int failed = 0;
    ostringstream ost;
    vector<TestCase*> items;

    if(UnitTest::_tests == NULL)
    {
        cout << "No tests to run." << endl;
        return 1;
    }

    for(vector<TestSuite*>::iterator test = UnitTest::_tests->begin() ; test < UnitTest::_tests->end() ; test++)
    {
        items = (*test)->getItems();

        for(vector<TestCase*>::iterator item = items.begin() ; item < items.end() ; item++)
        {
            try {
                if ((*item)->run() == 0)
                {
                    if(UnitTest::_verbosity == 0)
                        cout << ".";
                } 
            } 
            catch(AssertError e) {
                if(UnitTest::_verbosity == 0)
                    cout << "F";
                ost << "======================================================================" << endl;
                ost << "FAIL: " << (*test)->_name << "::" << (*item)->_name << endl;
                ost << "----------------------------------------------------------------------" << endl;
                ost << "Assertion error: " << e._reason;
                failed++;
            }
            catch(...) {
                if(UnitTest::_verbosity == 0)
                    cout << "F";
                ost << "======================================================================" << endl;
                ost << "FAIL: " << (*test)->_name << "::" << (*item)->_name << endl;
                ost << "----------------------------------------------------------------------" << endl;
                ost << "Fatal error: " << endl;
                failed++;
            }
        }
    }

    if(failed > 0) {
        cout << endl << ost.str();
    }

    cout << endl << "----------------------------------------------------------------------" << endl;
    cout << "Ran " << UnitTest::_tests->size() << " test" << (UnitTest::_tests->size() > 1 ? "s" : "") << endl << endl;
    
    if (failed == 0) {
        cout << "OK" << endl;
    } 
    else {
        cout << "FAILED (failures=" << failed << ")" << endl;
    }

    return 0;
}

UnitTest::TestCase::TestCase(TestSuite& test)
{
    test.addItem(this);
}

vector<UnitTest::TestCase*> UnitTest::TestSuite::getItems()
{
    return this->_items;
}

UnitTest::TestSuite& UnitTest::TestSuite::addItem(TestCase* item)
{
    this->_items.push_back(item);
    return *this;
}

UnitTest::TestSuite::TestSuite()
{
    if(UnitTest::_tests == NULL)
        UnitTest();
    UnitTest::_tests->push_back(this);
}

