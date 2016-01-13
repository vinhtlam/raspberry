/* 
 * File:   sprinkler.cpp
 * Author: m.bennett
 *
 *  This was written by Matthew Bennett
 * (abishur on the raspberry pi forums - http://www.raspberrypi.org/phpBB3/)
 */

// Includes
#include <wiringPi.h> // By Gordon
#include <mysql/mysql.h> //mysql connector library for C++
#include "my_sql_header.h" // customer header for making SQL queries
#include "time_calcs.h" // performs math operations on struct tm objects

using namespace std;

// change these to match your system
#define HOST "localhost" // PC hosting database
#define USER "SQL USERNAME HERE" // User authorized to connecto to database
#define PASSWD "SQL PASSWORD HERE" // User's password
#define DB "gpio" // Database name
#define waitTime 5 // time to wait between gpio reads

// ----------- DO NOT CHANGE ANYTHING BELOW THIS LINE -----------------------------

MYSQL *con, mysql; // Needed for mysql connections
MYSQL_RES *result; // Needed for mysql connections
MYSQL_ROW row, rowRes; // Needed for mysql connections
struct tm channelTimeStart[8]; // Time for sprinkler to turn on
struct tm channelTimeEnd[8]; // Time for sprinkler to turn off
struct tm* theTime; // holds the "current" time
int gpio_status[8]; // Reads current status of the physical GPIO
int pinStart[8]; // User request to turn GPIO on or off
bool dayRun[8]; // Is today one of the days it's supposed to run
bool garageUpdate;

// Functions
struct tm getTime(int x); // gets start time from mysql
int compTimes(struct tm &time1, struct tm &time2);// compares two times
void init (); // initializes system
struct tm * UpdateTime(); // Gets current time
int query_result(const char* query1); // returns SQL query as an integer
void gpioUpdate(int x); // updates current status of GPIO
void autoRun(struct tm &start, struct tm &end, int x);
void gpioInit();
//string direction_result(const char* query1);
void garage();

int main ()
{
    mysql_init(&mysql); //initialize mysql connection library
    try {
        con = mysql_real_connect(&mysql,HOST,USER,PASSWD,DB,0,0,0); //connect to mysql database
        while (con==NULL)
        {
            sleep(1);
            con = mysql_real_connect(&mysql,HOST,USER,PASSWD,DB,0,0,0); //connect to mysql database
        };
    }
    catch(int){}

    int needUpdate[8];
    int x; // Every program has to have an int x for loop control
    int y; // If you need more than one loop control, the second one has to be y.  It's a rule
    int loop = 1; // controls loop
    const char* query; // holds query to SQL database
    int pinManMode[8]; // Is the pin being controlled manual?
    int pinAutoMode[8]; // What auto mode is it in?
    int channelDuration[8]; // Activation time in minutes
    string channelstr[8];
    bool firstRun = true;

    init();
while (1){
    while (loop == 1) {
        x = 0;
        do {
            gpioUpdate(x);
            theTime = UpdateTime();
            gpio_status[x] = digitalRead(x);
            pinManMode[x] = query_result(query_build("SELECT pinMode FROM `pinMode` WHERE pinNumber = '", x));
            if (pinManMode[x] == 1) {
                pinStart[x] = query_result(query_build("SELECT pinStart FROM `manual_start` WHERE pinNumber = '", x));
		channelDuration[x] = query_result(query_build("SELECT manualDuration FROM `manual_start` WHERE pinNumber = '", x));
                needUpdate[x] = 1;
                if ((pinStart[x] == 1) && (gpio_status[x] != pinStart[x])) {
			cout << pinStart[x] << endl;
                    while (digitalRead(x) != 1) {
                        digitalWrite(x, 1);
			sleep(0.05);
                    };
                    gpio_status[x] = digitalRead(x);
                    channelTimeStart[x] = *theTime;
                    channelTimeEnd[x] = calc_end(channelTimeStart[x], channelDuration[x]);
                }
                if ((compTimes(*theTime, channelTimeEnd[x]) != -1) || (pinStart[x] == 0)){
                    while (digitalRead(x) != 0) {
                        digitalWrite(x, 0);
			sleep(0.05);
                    };
                    mysql_query(con,query_build("UPDATE `gpio`.`manual_start` SET `pinStart` = '0' WHERE `manual_start`.`pinNumber` = '", x));
                }
            }
            else { // Pin is in auto mode
                pinAutoMode[x] = query_result(query_build("SELECT pinAutoMode FROM `pinAutoMode` WHERE pinNumber = '", x));
                if ((needUpdate[x] == 1)||(firstRun)) {
                    channelTimeStart[x] = getTime(x); // Gets auto run start time
                    channelDuration[x] = query_result(query_build("SELECT autoDuration FROM `auto_start` WHERE pinNumber = '", x));
                    channelTimeEnd[x] = calc_end(channelTimeStart[x], channelDuration[x]);
                    channelstr[x] = countDays(query_result(query_build("SELECT pinDays FROM `pinDays` WHERE pinNumber = '", x)));
                    mysql_query(con,query_build("UPDATE `gpio`.`auto_start` SET `needUpdate` = '0' WHERE `auto_start`.`pinNumber` = '", x));
                }

                switch (pinAutoMode[x]){
                    case 0: // Runs on even numbered days
                        if (theTime->tm_mday%2 == 0){
                            autoRun(channelTimeStart[x], channelTimeEnd[x], x);
                        }

                        break;
                    case 1: // Runs on odd numbered days
                        if (theTime->tm_mday %2 != 0){
                            autoRun(channelTimeStart[x], channelTimeEnd[x], x);
                        }

                        break;
                    case 2: // Runs on user defined days days are defined as day_num^2 (sunday = 1, monday = 2; etc)
                        if (((theTime->tm_hour == 0) && (theTime->tm_min == 0)) || (firstRun)|| (needUpdate[x] == 1)){
                         dayRun[x] = isDay(theTime->tm_wday, channelstr[x]);
                        }
                        if (dayRun[x]){
                            autoRun(channelTimeStart[x], channelTimeEnd[x], x);
                        }
                        break;
                }
            }

                needUpdate[x] = query_result(query_build("SELECT needUpdate FROM `auto_start` WHERE pinNumber = '", x));
		if (digitalRead(x) == 1){
   		    mysql_query(con,query_build("UPDATE `gpio`.`pinStatus` SET `pinStatus` = '1' WHERE `pinStatus`.`pinNumber` = '", x));
		} else mysql_query(con,query_build("UPDATE `gpio`.`pinStatus` SET `pinStatus` = '0' WHERE `pinStatus`.`pinNumber` = '", x));

                x++;
		garage();
                } while (x<8);

                firstRun = false;
                loop = query_result(query_build("SELECT doLoop FROM `doLoop` WHERE numbers = '", 1));
               sleep (waitTime);
    };

};

	cout << "What? How did you get here?" << endl;
return 0;
}

void garage(){
	if ((digitalRead(10) == 0) && (garageUpdate)) {
	    mysql_query(con, query_build("UPDATE `gpio`.`garage` SET `isOpen` = '1' WHERE `garage`.`pinID` = '", 1));
	    garageUpdate = false;
	}
	if ((digitalRead(10) == 1) && (garageUpdate==false)){
	     mysql_query(con,query_build("UPDATE `gpio`.`garage` SET `isOpen` = '0' WHERE `garage`.`pinID` = '", 1));
	     garageUpdate = true;
	}

	return;
}

void autoRun(struct tm &start, struct tm &end, int x){
    if ((compTimes(start, *theTime) == -1) and (compTimes(*theTime, end) == -1)) {
                                digitalWrite(x, 1);
                                while (digitalRead(x) != 1) {
                                        digitalWrite(x, 1);
                                        sleep(0.05);
                                };
                            }
                            else {
                                digitalWrite(x, 0);
                                while (digitalRead(x) != 0) {
                                        digitalWrite(x, 0);
                                        sleep(0.05);
                                };
                            }
    return;
}

// compares two times to see value between them, returns -1 if time2 is greater than time1
// otherwise returns difference between time1 and time 2 in minutes
int compTimes(struct tm &time1, struct tm &time2)
{
        int diffTime = 0; // value in difference of minutes between time1 minus time 2
        int time1day, time2day, dayDiff;
	time1day = time2day = dayDiff = 0; //day of the month (1-31)
        int time1hour, time2hour, hourDiff;
	time1hour = time2hour = hourDiff = 0; // hours since midnight (0-23)
        int time1min, time2min, minDiff;
	time1min = time2min = minDiff = 0; // minutes after the hour (0-59)

        time1day = time1.tm_mday;
        time2day = time2.tm_mday;

        time1hour = time1.tm_hour;
        time2hour = time2.tm_hour;

        time1min = time1.tm_min;
        time2min = time2.tm_min;

        if (time1day >= time2day) {
            dayDiff = time1day-time2day;
            diffTime += dayDiff*24*60;
            if (time1hour >= time2hour){
                hourDiff = time1hour-time2hour;
                diffTime += hourDiff*60;
                if (time1min >= time2min){
        	    minDiff = time1min - time2min;
        	    diffTime += minDiff;
       	        }
	   }
	}

        if (diffTime <= 0) diffTime = -1;
        return diffTime;
}

struct tm getTime(int x)
{
    struct tm get;

    get = *theTime;

    get.tm_hour = query_result(query_build("SELECT pinHours FROM `auto_start` WHERE pinNumber = '", x));
    get.tm_min = query_result(query_build("SELECT pinMins FROM `auto_start` WHERE pinNumber = '", x));

    return get;

}

void gpioUpdate(int x) {
int temp;
        temp = 0;
        temp = query_result(query_build("SELECT pinDirection FROM `pinDirection` WHERE pinNumber = '", x));
       if (temp == 1) pinMode(x, INPUT);
       else pinMode(x, OUTPUT);
    return;
}

void init ()
{
    for (int x=0;x<8;x++){
        dayRun[x] =0;
}
    wiringPiSetup();
    pinMode(10, INPUT);
    if (digitalRead(10) == 0) {
	garageUpdate = true;
    }
    else {
	garageUpdate =false;
    }
    return;
}

int query_result(const char* query1)
{
    int query_state=-2; // Needed for mysql connections
    int rowInt;
    query_state = mysql_query(con, query1);
    if (query_state != -2){
       result = mysql_store_result(con);
       row = mysql_fetch_row(result);
       rowInt = atoi(row[0]);
       mysql_free_result(result);
       return rowInt;
    }
    return -1;
}

struct tm* UpdateTime()
{
        struct tm *theCurrentTime;
        time_t long_time;
        time ( &long_time );
        theCurrentTime = localtime ( &long_time);
        return theCurrentTime;
}