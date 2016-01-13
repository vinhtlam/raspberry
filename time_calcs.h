/* 
 * File:   time_calcs.h
 * Author: m.bennett
 *
 *  This was written by Matthew Bennett
 * (abishur on the raspberry pi forums - http://www.raspberrypi.org/phpBB3/)
 */

#include <cmath>
#include <time.h>
#include <sstream>
#include <string>

using namespace std;

#ifndef TIME_CALCS_H
#define    TIME_CALCS_H

#include <iostream>
#include <iomanip>
int findDays(int month, int year);
struct tm calc_end(struct tm startEnd, int duration);
string countDays (int days);
bool isDay (int day);

struct tm calc_end(struct tm startEnd, int duration)
{
    int inc=0;
    int remainder = 0;
    int days = 0;
    
    startEnd.tm_min += duration;
    if (startEnd.tm_min > 59 ) {
        remainder = (startEnd.tm_min + 1)%60; // tm_min is a range of 0-59, I want 1-60 for division
        inc = (startEnd.tm_min + 1)/60;
        startEnd.tm_min = remainder - 1; // store it back a range of 0-59
        startEnd.tm_hour += inc; // increase hours by the amount of additional hours
    } 
        
    if (inc == 0) return startEnd; // If it's in the same hour there's no need to go further
    inc = remainder = 0; // housekeeping
    
    if (startEnd.tm_hour > 23) {
        remainder = (startEnd.tm_hour + 1)%24; //tm_hour is a range of 0-23, I want 1-24 for division
        inc = (startEnd.tm_hour + 1)/24;
        startEnd.tm_hour = remainder - 1;
        startEnd.tm_mday += inc;
    }
    
    
    if (inc == 0) return startEnd; // if it's the same day there's no need to go further
    inc = remainder = 0; // housekeeping
    
    days = findDays((startEnd.tm_mon+1), (startEnd.tm_year+1900));
    if (startEnd.tm_mday > days) {
        remainder = startEnd.tm_mday - days;
        inc = 1;
        startEnd.tm_mday = remainder;
        startEnd.tm_mon += 1;
    }
    
    if (inc == 0) return startEnd;
    inc = remainder = 0;
    
    if (startEnd.tm_mon > 11){
        startEnd.tm_mon = 0;
        startEnd.tm_year += 1;
    }
                                
    return startEnd;
}

int findDays(int month, int year)
{
int Days;

if (month == 4 || month == 6 || month == 9 || month == 11) Days = 30;

else if (month == 2) {
    bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (isLeapYear) Days = 29;
    else Days = 28;
}
else Days = 31;

return Days;
}

string countDays (int days) {
    int x;
    int y;
    int dayArr[7];
    for (x=0;x<7;x++){
        dayArr[x] = 0;
    }
        if ((days % 2)  == 1) {
            dayArr[0] = 1;
            days = days - 1;
        }
    
    while (days != 0) {
        x = 0;
        y = days;
        while (y != 1){
            if (y%2 == 1) {
		y=1;
	    }
	    else {
		y = y / 2;
            	x++;
            }
	};
        dayArr[x] = 1;
        days = (days-(pow(2,x)));
    };
    
    std::ostringstream stm ;
    stm << '[' ;
    for( std::size_t i = 0 ; i < 7 ; ++i ) stm << dayArr[i] << ", " ;

    std::string result = stm.str() ;
    // remove the last ", " and add a ']'
    return result.substr( 0, result.size()-2 ) + ']' ;
}

bool isDay (int day, string channelString){
    
    int y, x;
    int channelDays[7];
    bool boolDay = false;
    for (x=0;x<7;x++){
        channelDays[x]=0;
    }
    y=0;
    for (x=1;x<21;x+=3){ // only works for single int values, first int is in position 1, every 3 char is the next int
        stringstream str;
        str << channelString[x];
        str >> channelDays[y];
        y++;
    }
    
    switch (day){
        case 0:
            if (channelDays[0] == 1) boolDay = true;
            break;
        case 1:
            if (channelDays[1] == 1) boolDay = true;
            break;    
        case 2:
            if (channelDays[2] == 1) boolDay = true;
            break;
        case 3:
            if (channelDays[3] == 1) boolDay = true;
            break;
        case 4:
            if (channelDays[4] == 1) boolDay = true;
            break;
        case 5:
            if (channelDays[5] == 1) boolDay = true;
            break;
        case 6:
            if (channelDays[6] == 1) boolDay = true;
            break;
    }
        
    return boolDay;
}

#endif	/* TIME_CALCS_H */