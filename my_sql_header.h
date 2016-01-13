/* 
 * File:   my_sql_header.h
 *
 *  This was written by Matthew Bennett
 * (abishur on the raspberry pi forums - http://www.raspberrypi.org/phpBB3/)
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

#ifndef MY_SQL_HEADER_H
#define    MY_SQL_HEADER_H

// Builds a query in the acceptable const char* format with a variable
// int "x" it the end of the string i.e. 
// "select * from pinID where pinNum = '" x "' ;";


// Builds a query in the acceptable const char* format with a variable
// int "x" it the end of the string i.e. 
// "select * from pinID where pinNum = '" x "' ;";
const char* query_build(string str, int x)
{
    const char* c;
    string intHold;
    intHold = '0'+x;
        str += intHold;
        str += "' ;";
        c = str.c_str();
        return c;
}

// Builds a query in the acceptable const char* format with a variable
// int "x" in the middle of the string i.e.
// "Select id = '" x "' from pinNum;"
const char* query_build(string str1, int x, string str2)
{
    const char* c;
    string intHold;
    intHold = '0'+x;
        str1 += intHold;
        str1 += "' '";
        str1 += str2;
        str1 += "' ;";
        c = str1.c_str();
        return c;
}

#endif	/* MY_SQL_HEADER_H */