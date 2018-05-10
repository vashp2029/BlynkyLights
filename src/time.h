/* Time-Related Functions
 * By: 		Vash Patel
 * Date: 	May 2018
 *
 * These functions are primarily for obtaining real time from NTP servers and
 * other time-based services.
 */

#ifndef TIME_H
#define TIME_H

	#include "Dusk2Dawn.h"
	#include "ESP8266HTTPClient.h"
	#include "ArduinoJson.h"



	///Current Time/////////////////////////////////////////////////////////////
	bool dst;	//Daylight savings time
	unsigned int timeStamp;
	unsigned int year;
	unsigned int month;
	unsigned int day;
	unsigned int hour;
	unsigned int minute;
	unsigned int second;

	unsigned int millisAtLastUpdate; //Use this to increment time between syncs

	//Using HTTP to request JSON data from TimeZoneDB, see here: https://goo.gl/STJ2Cf
	HTTPClient timeClient;

	void syncTime(){
		//Concatenate your API key and location data to create the proper URL for
		//timezonedb.
		String timeServer =
		"http://api.timezonedb.com/v2/get-time-zone?format=json&by=position&fields=dst,timestamp,formatted&key="
		+ String(TIMEZONEDBKEY) + "&lat=" + String(LATITUDE) + "&lng=" + String(LONGITUDE);

		timeClient.begin(timeServer);		//Connect to the API
		int httpCode = timeClient.GET();	//Returns 1 if proper connection is made

		//If connection was made, grab the JSON data and store it as a string.
		if(httpCode > 0){
			DEBUG_PRINTF("Successfully connected to time server.");

			//Create and object that will eventually hold the returned time data to be
			//sent to the JSON parser.
			const size_t bufferSize = JSON_OBJECT_SIZE(5) + 100;
			DynamicJsonBuffer timeBuffer(bufferSize);

			//Get the JSON data and copy it to JsonObject. The data is parsed
			//as a string temporarily but stored as bool or int.
			JsonObject& root = timeBuffer.parseObject(timeClient.getString());

			dst = root[String("dst")];
			timeStamp = root[String("timestamp")];

			//Creaete character array long enough to hold the entire formatted date
			//then use sscanf to parse out each part to its respective int var.
			char formattedTime[22];
			root["formatted"].printTo(formattedTime);
			sscanf(formattedTime, "\"%u-%u-%u %u:%u:%u\"", &year, &month, &day, &hour, &minute, &second);

			DEBUG_PRINTF("Sync to time server complete.");
			DEBUG_PRINTF("UNIX timestamp: %u", timeStamp);
			DEBUG_PRINTF("Formatted date: %u/%02u/%02u", year, month, day);
			DEBUG_PRINTF("Formatted time: %02u:%02u:%02u", hour, minute, second);
		}

		//Close the connection.
		timeClient.end();

		millisAtLastUpdate = millis();
	}

	void incrementTime(){
		//Subtract the uptime of last update from the current uptime and add
		//the resulting value to timeStamp. This should be accurate enough
		//to keep the time updated between syncs to the time server.
		timeStamp = timeStamp + (millis() - millisAtLastUpdate);
		millisAtLastUpdate = millis();

		//Uncomment the following lines to debug this function. Otherwise,
		//leave it commented as it will print to serial every second.
		//DEBUG_PRINTF("Incrementing time without sync.");
		//DEBUG_PRINTF("UNIX timestamp: %u", timeStamp);
	}


	///Sunset and Sunrise Time//////////////////////////////////////////////////
	//Use Dusk2Dawn library to obtain the sunrise and sunset times for your
	//location.
	Dusk2Dawn dusk2dawn(LATITUDE, LONGITUDE, TIMEZONE);

	unsigned int sunriseTimeStamp;
	unsigned int sunsetTimeStamp;

	void getSunriseSunset(){
		//Dusk2Dawn returns the sunrise/sunset times in minutes from midnight
		//so to keep everything standardized, we want to convert that to UNIX
		//time stamp. To do this, we subtract the seconds elapsed today from
		//the current UNIX timestamp. We can then add that to the sunrise/sunset
		//time returned by Dusk2Dawn to get the UNIX timestamp of sunrise/sunset.
		unsigned int midnightInUnixTime = timeStamp - ((hour * 3600) + (minute * 60) + second);

		sunriseTimeStamp = ((dusk2dawn.sunrise(year, month, day, dst) * 60) + midnightInUnixTime);
		sunsetTimeStamp = ((dusk2dawn.sunset(year, month, day, dst) * 60) + midnightInUnixTime);

		DEBUG_PRINTF("Sunrise (UNIX time): %u, sunset (UNIX time): %u", sunriseTimeStamp, sunsetTimeStamp);
	}

#endif