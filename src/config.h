/* Configuration
 * By: 		Vash Patel
 * Date: 	May 2018
 *
 * This is where all the configuration for the project lives. Everything from
 * pin declarations to your geographical location to the type of LEDs you're using
 * will be stored here to keep the main code clean.
 */

#ifndef CONFIG_H
#define CONFIG_H

	#define ON 				1		//Sometimes it's nice to use ON/OFF instead of numbers
	#define OFF 			0		//Sometimes it's nice to use ON/OFF instead of numbers


	//Blynk pin configuration


	//Physical pin configuration
	#define MICPIN			A0		//Make sure to use an analog pin
	#define DATAPIN			D5		//Going to data port on LED strip
	#define COLORORDER		GRB		//If colors are wrong for you, change this
	#define CHIPSET			WS2812B	//APA102C, WS2813, WS2811, etc
	#define MAXVOLTS		5		//FastLED has a nifty function to control power draw
	#define MAXAMPS			1200 	//Units in milliamps


	//Location and time settings
	#define LATITUDE		34.051490	//Used to pull accurate time data
	#define LONGITUDE		-84.071300	//Use Google to find lat and long
	#define TIMEZONE		-5 		//Use non-DST time zone
	#define TIMEINTERVAL	300000	//Time in ms between polling time servers


	//OTA updates
	#define OTAPORT			8266	//Listen on this port for OTA updates
	#define HTTPPORT		80		//Listen on this port for HTTP updates

#endif