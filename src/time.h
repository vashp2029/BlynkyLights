/* Time-Related Functions
 * By: 		Vash Patel 
 * Date: 	May 2018
 * 
 * These functions are primarily for obtaining real time from NTP servers and 
 * other time-based services. 
 */

#ifndef TIME_H
#define TIME_H

	#include "TimeLib.h"
	#include "Dusk2Dawn.h"


	///Current Time/////////////////////////////////////////////////////////////
	unsigned long currentUnixTime;
	unsigned long midnightInUnixTime;

	void getCurrentTime(){
		//Use TimeLib.h functions to get the number of seconds elapsed since
		//midnight.
		unsigned long secondsSinceMidnight = (hour() * 3600) + (minute() *60) + second();

		//Use TimeLib.h functions to get the current time in UNIX timestamp.
		currentUnixTime = now();

		//Subtract seconds since midnight from UNIX timestamp to get the UNIX
		//time of the most recent past midnight. This will be useful for
		//sunrise and sunset effects.
		midnightInUnixTime = currentUnixTime - secondsSinceMidnight;

		DEBUG_PRINTF("The current time in EPOCH is: %u", currentUnixTime);
		DEBUG_PRINTF("Midnight in EPOCH occurred at: %u", midnightInUnixTime);
	}


	///Sunset and Sunrise Time//////////////////////////////////////////////////
	//Use Dusk2Dawn library to obtain the sunrise and sunset times for your
	//location.
	Dusk2Dawn dusk2dawn(LATITUDE, LONGITUDE, TIMEZONE);

	bool dst;	//Daylight Savings Time
	unsigned long sunriseInUnixTime;
	unsigned long sunsetInUnixTime;

	void getSunriseSunset(){
		getCurrentTime();

		//Very rudimentary way of determining daylight savings time using TimeLib.
		//If the current month is in between March and October, DST is set to true.
		//TODO Implement a more accurate way to figure out whether or not DST
		//TODO should be set to true or false.
		if(month() >= 3 && month() <= 10){
			dst = true;
		}

		else{
			dst = false;
		}

		//Dusk2Dawn returns the sunrise/sunset times in seconds from midnight
		//so to keep everything standardized, we want to convert that to UNIX
		//time stamp. We can do this simply by adding in the UNIX timestamp of
		//the most recent midnight.
		sunriseInUnixTime = ((dusk2dawn.sunrise(year(), month(), day(), dst) * 60) + midnightInUnixTime);
		sunsetInUnixTime = ((dusk2dawn.sunset(year(), month(), day(), dst) * 60) + midnightInUnixTime);

		DEBUG_PRINTF("Sunrise (UNIX time): %u, sunset (UNIX time): %u", sunriseInUnixTime, sunsetInUnixTime);
	}

#endif