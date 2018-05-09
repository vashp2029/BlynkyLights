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
	#define GROUPPIN		V0 		//Select which set of LEDs to command
	#define SWITCHPIN 		V1 		//Simple on/off switch
	#define AUTOSWITCHPIN 	V2 		//Switch to turn LEDs on/off at a certain time
	#define BRIGHTNESSPIN 	V3 		//Brightness slider (range 0-255 in Blynk app)
	#define SENSITIVITYPIN 	V4 		//Mic sensitivity (range 0-255 in Blynk app)
	#define SPEEDPIN 		V5 		//Animation speed (range 0-255 in Blynk app)
	#define EFFECTPIN 		V6 		//Effect selection drop-down
	#define SOUNDEFFECTPIN	V7 		//Sound-reactive effect selection drop-down
	#define RGBPIN 			V8 		//ZeRGBa (set to "merge" in Blynk app)
	#define ESPTIMEPIN 		V9 		//Update the Blynk app with current ESP time
	#define AUTOTIMEPIN 	V10		//Time to automaticall turn LEDs on/off
	#define PINCOUNT		11 		//Total number of pins (just count the lines above)

	//Physical pin configuration
	#define MICPIN			A0		//Make sure to use an analog pin
	#define DATAPIN			D5		//Going to data port on LED strip
	#define COLORORDER		GRB		//If colors are wrong for you, change this
	#define CHIPSET			WS2812B	//APA102C, WS2813, WS2811, etc
	#define MAXVOLTS		5		//FastLED has a nifty function to control power draw
	#define MAXAMPS			1200 	//Units in milliamps

	//Geographic location for sunrise and sunset functions (currently set to Atlanta)
	#define LATITUDE		34.051490
	#define LONGITUDE		-84.071300
	#define TIMEZONE		-5 		//Use non-DST time zone

	//Microphone configuration
	#define DCOFFSET		300		//Offset the waveform above or below the zero line
	#define NOISE 			85		//Ambient noise in the room (obtained by printing values to Serial for a while)
	#define AMPLIFY			1		//In case you realize placement/casing muffles sounds, this will MULTIPLY the values
	#define SOUNDSAMPLES	64		//Number of sound samples to collect for analysis (more samples = smoother)

	//OTA updates
	#define OTAPORT			8266	//Listen on this port for OTA updates
	#define HTTPPORT		80		//Listen on this port for HTTP updates

#endif