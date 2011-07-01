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

UnitTest::UnitTest()
{
    if (UnitTest::_tests == NULL)
    {
        UnitTest::_tests = new vector<UnitTest::TestSuite*>;
    }
}

AssertError::AssertError(const string s, const string v1, const string v2, const int format)
{
    ostringstream ost, ost2, ost3;
    int i;

    switch(format)
    {
    case FMT_DUMP:
        ost << s << "\n" << hex;
        for(i = 0 ; i < max(v1.size(), v2.size()) ; i++)
        {
            if(i < v1.size())
            {
                ost.width(2);
                ost.fill('0');
                ost << (int) v1[i];
                ost2 << (v1[i] > 31 ? v1[i] : '.');
                if(i < v1.size() - 1)
                    ost << " ";
            }
            else
            {
                ost << "   ";
                ost2 << '.';
            }
        }

        ost << " : " << ost2.str() << '\n';

        for(i = 0 ; i < max(v1.size(), v2.size()) ; i++)
        {
            if(i < v2.size())
            {
                ost.width(2);
                ost.fill('0');
                ost << (int) v2[i];
                ost3 << (v2[i] > 31 ? v2[i] : '.');
                if(i < v2.size() - 1)
                    ost << " ";
            }
            else
            {
                ost << "   ";
                ost3 << '.';
            }
        }

        ost << " : " << ost3.str();
        break;
    case FMT_DEFAULT:
    default:
        ost << s << " " << v1 << " != " << v2;
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
    if(v1 == v2)
        return 0;
    else
        throw AssertError(s, v1, v2, FMT_DEFAULT);
}

int UnitTest::assert(const string s, const char* v1, const char* v2)
{
    if(string(v1) == string(v2))
        return 0;
    else
        throw AssertError(s, string(v1), string(v2), FMT_DEFAULT);
}

int UnitTest::assert(const string s, const string v1, const string v2, const int format)
{
    if(v1 == v2)
        return 0;
    else
        throw AssertError(s, v1, v2, format);
}

int UnitTest::assert(const string s, const char* v1, const char* v2, const int format)
{
    if(string(v1) == string(v2))
        return 0;
    else
        throw AssertError(s, string(v1), string(v2), format);
}

int UnitTest::assert(const string s, const UINT8 v1, const UINT8 v2)
{
    if(v1 == v2)
        return 0;
    else
        throw AssertError(s, v1, v2);
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
                    cout << ".";
                }
            } 
            catch(AssertError e) {
                cout << "F";
                ost << "======================================================================" << endl;
                ost << "FAIL: " << (*test)->_name << "::" << (*item)->_name << endl;
                ost << "----------------------------------------------------------------------" << endl;
                ost << "Assertion error: " << e._reason << endl;
                failed++;
            }
            catch(...) {
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

