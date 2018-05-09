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
	void setupTime(){}

	void updateTime(){}


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