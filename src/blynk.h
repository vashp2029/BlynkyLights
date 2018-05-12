/* Blynk Inputs
 * By: 		Vash Patel
 * Date: 	May 2018
 *
 * This is where all the Blynk inputs are configured.
 */

#ifndef BLYNK_H
#define BLYNK_H

	#include "BlynkSimpleEsp8266.h"

////////////////////////////////////////////////////////////////////////////////
//DETAILED EXPLANATIONS                                                       //
////////////////////////////////////////////////////////////////////////////////



    ///Pin Configuration////////////////////////////////////////////////////////
	//#define PIN_SELECTGROUP 	V0
	//#define PIN_ONOFF			V1
	//#define PIN_ONTIME		V2
	//#define PIN_OFFTIME		V3
	//#define PIN_RGB			V4
	//#define PIN_BRIGHTNESS	V5
	//#define PIN_SPEED			V6
	//#define PIN_SENSITIVITY	V7
	//#define PIN_EFFECT		V8
	//#define PIN_NOISE			V9
	//#define PIN_TERMINAL		V20
	//#define PIN_WEBHOOK		V21
	//#define PIN_GPS			V22

	//NoPin	= Real time clock
	//NoPin	= Eventor
	//NoPin	= Notification



	///Group Selection//////////////////////////////////////////////////////////
	//Anytime selectedGroup is set to "LEDGROUP" each ESP you are controlling
	//will think it is the one that is selected to respond to Blynk inputs. This
	//acts as a psuedo-global selection. Let's say, for example, that you want to
	//only tell the "bed" LEDs to turn off. You would set selectedGroup equal to
	//"2" (which is the identifier for "bed" in locationSpecific.h) and any further
	//inputs from the Blynk app would only apply to the "bed" leds. But if you want
	//to turn off ALL the LEDs, you would set selectedGroup equal to 'LEDGROUP'.


////////////////////////////////////////////////////////////////////////////////
//BASIC INPUT FUNCTIONS                                                       //
////////////////////////////////////////////////////////////////////////////////



	///Group Selection//////////////////////////////////////////////////////////

	uint8_t selectedGroup = LEDGROUP;
	uint8_t &refGroup = selectedGroup;

	//Select which group of LEDs will be controlled with any following inputs
	//from the app.
	BLYNK_WRITE(PIN_SELECTGROUP){
		//If the first option is chosen (which should be "global" or "all"),
		//each ESP should set selectedGroup to its own group, effectively making
		//each one think it should respond to Blynk inputs.
		if(param.asInt() == 0){
			refGroup = LEDGROUP;
			DEBUG_PRINTLN("Controlling all LEDs simultaneously.");
		}

		//If any option other than "all" or "global" are chosen, tell all the ESPs
		//which group is the one that should respond to further Blynk inputs.
		else{
			refGroup = param.asInt();
			DEBUG_PRINTF("Controlling the '%s' LEDs (group number %u).", SENSORNAME, LEDGROUP);
		}
	}



	///On/Off Switch////////////////////////////////////////////////////////////
	bool onOffToggle = false;
	bool &refOnOff = onOffToggle;

	BLYNK_WRITE(PIN_ONOFF){
		if(refGroup == LEDGROUP){
			refOnOff = param.asInt();

			DEBUG_PRINTF("Either '%s' or 'All' are selected, responding to on/off command.", SENSORNAME);
			DEBUG_PRINTF("On: %s.", refOnOff ? "true" : "false");
		}

		else{
			DEBUG_PRINTF("(%s) is not selected, so ignoring on/off command.", SENSORNAME);
		}
	}



	///Auto-on/Auto-off Times///////////////////////////////////////////////////
	BLYNK_WRITE(PIN_ONTIME){
		//TODO Planning on changing the way real time is pulled in time.h to use
		//TODO Blynk RTC, so will come back to this.
		//TODO Note that it might be easier to use the 'eventor' widget in Blynk
		//TODO to turn lights on/off at a particular time rather than coding it
		//TODO into the program and having to keep track of time. It might also
		//TODO remove the need to keep track of time completely.
	}



	BLYNK_WRITE(PIN_OFFTIME){
		//TODO Planning on changing the way real time is pulled in time.h to use
		//TODO Blynk RTC, so will come back to this.
		//TODO Note that it might be easier to use the 'eventor' widget in Blynk
		//TODO to turn lights on/off at a particular time rather than coding it
		//TODO into the program and having to keep track of time. It might also
		//TODO remove the need to keep track of time completely.
	}



	///RGB Selection////////////////////////////////////////////////////////////
	//Use FastLED CRGB type to store RGB data for simplicity.
	CRGB currentRGB;
	CRGB &refRGB = currentRGB;

	BLYNK_WRITE(PIN_RGB){
		if(refGroup == LEDGROUP){
			refRGB[0] = param[0].asInt();
			refRGB[1] = param[1].asInt();
			refRGB[2] = param[2].asInt();

			DEBUG_PRINTF("Either '%s' or 'All' are selected, responding to RGB command.", SENSORNAME);
			DEBUG_PRINTF("R: %u, G: %u, B: %u.", refRGB[0], refRGB[1], refRGB[2]);
		}

		else{
			DEBUG_PRINTF("(%s) is not selected, so ignoring RGB command.", SENSORNAME);
		}
	}



	///Brightness Slider////////////////////////////////////////////////////////
	uint8_t brightness = 0;
	uint8_t &refBright = brightness;

	BLYNK_WRITE(PIN_BRIGHTNESS){
		if(refGroup == LEDGROUP){
			refBright = param.asInt();

			DEBUG_PRINTF("Either '%s' or 'All' are selected, responding to brightness command.", SENSORNAME);
			DEBUG_PRINTF("Brightness: %u", refBright);
		}

		else{
			DEBUG_PRINTF("(%s) is not selected, so ignoring brightness command.", SENSORNAME);
		}
	}



	///Animation Speed Slider///////////////////////////////////////////////////
	uint8_t animationSpeed = 0;
	uint8_t &refSpeed = animationSpeed;

	BLYNK_WRITE(PIN_SPEED){
		if(refGroup == LEDGROUP){
			refSpeed = param.asInt();

			DEBUG_PRINTF("Either '%s' or 'All' are selected, responding to speed command.", SENSORNAME);
			DEBUG_PRINTF("Brightness: %u", refSpeed);
		}

		else{
			DEBUG_PRINTF("(%s) is not selected, so ignoring speed command.", SENSORNAME);
		}
	}



	///Mic Sensitivity Slider///////////////////////////////////////////////////
	uint8_t micSensitivity = 0;
	uint8_t &refSensitivity = micSensitivity;

	BLYNK_WRITE(PIN_SENSITIVITY){
		if(refGroup == LEDGROUP){
			refSensitivity = param.asInt();

			DEBUG_PRINTF("Either '%s' or 'All' are selected, responding to mic sensitivity command.", SENSORNAME);
			DEBUG_PRINTF("Brightness: %u", refSensitivity);
		}

		else{
			DEBUG_PRINTF("(%s) is not selected, so ignoring mic sensitivity command.", SENSORNAME);
		}
	}



	///Effect Selection/////////////////////////////////////////////////////////

	BLYNK_WRITE(PIN_EFFECT){
		//TODO Saving for later. Need to think about how effect selection will be
		//TODO passed on to other functions in the most efficient way.
	}



	///Zero Noise Switch////////////////////////////////////////////////////////

	BLYNK_WRITE(PIN_NOISE){
		//If this or all LEDs are selected, run the zeroNoise function which will
		//update the value of 'ambientNoise' after taking some samples. After
		//running zeroNoise function, turn this switch back to the 'off' position.
		if(refGroup == LEDGROUP){
			DEBUG_PRINTF("Either '%s' or 'All' are selected, calculating ambient noise now.", SENSORNAME);
			zeroNoise();

			DEBUG_PRINTF("Ambient noise around %s is %u. Turning off noise switch.", SENSORNAME, refNoise);
			Blynk.virtualWrite(PIN_NOISE, OFF);
		}

		else{
			DEBUG_PRINTF("(%s) is not selected, so ignoring ambient noise command.", SENSORNAME);
		}
	}




////////////////////////////////////////////////////////////////////////////////
//COMPLEX INPUT FUNCTIONS                                                     //
////////////////////////////////////////////////////////////////////////////////



	///Terminal Commands////////////////////////////////////////////////////////
	//You can configure commands to be sent from the terminal which can do things
	//on the hardware. The example command here resets the ESP.
	WidgetTerminal blynkTerminal(PIN_TERMINAL);

	BLYNK_WRITE(PIN_TERMINAL){
		if(refGroup == LEDGROUP){
			if(String("restart") == param.asStr() || String("Restart") == param.asStr()){
				DEBUG_PRINTF("Restart of %s requested via terminal. Restarting...")

				blynkTerminal.print("You requested a restart of ");
				blynkTerminal.print(SENSORNAME);
				blynkTerminal.println(". Restarting now...");

				ESP.restart();
			}

			else{
				DEBUG_PRINTLN("Invalid command entered via terminal. Ignoring.");
				blynkTerminal.print("Invalid command. Ignoring.");
			}
		}

		else{
			blynkTerminal.print(SENSORNAME);
			blynkTerminal.print(" is not selected, so ignoring terminal command.");

			DEBUG_PRINTF("(%s) is not selected, so ignoring terminal command.", SENSORNAME);
		}
	}



	///Webhook//////////////////////////////////////////////////////////////////

	BLYNK_WRITE(PIN_WEBHOOK){
		//TODO Use webhook to pull real time and sunset/sunrise times from the
		//TODO web rather than using the Dusk2Dawk and TimeLib library. It should
		//TODO reduce overhead.
	}



	///GPS//////////////////////////////////////////////////////////////////////

	//This function can be configured to react in various ways based on entering
	//or exiting the area set in the app. By default, I've set it to turn off all
	//lights on exit and turn them on on enter.
	BLYNK_WRITE(PIN_GPS){
		if(param.asInt() == 0){
			DEBUG_PRINTF("You are leaving the area, turning off all the lights...");

			//Select "global" or "all" option which should be the first item in the
			//group selection menu.
			Blynk.virtualWrite(PIN_SELECTGROUP, 0);

			//Turn off selected groups (which should be all).
			Blynk.virtualWrite(PIN_ONOFF, OFF);
		}

		else{
			DEBUG_PRINTF("You are entering the area, turning on all the lights...");

			//Select "global" or "all" option which should be the first item in the
			//group selection menu.
			Blynk.virtualWrite(PIN_SELECTGROUP, 0);

			//Turn off selected groups (which should be all).
			Blynk.virtualWrite(PIN_ONOFF, ON);
		}
	}




////////////////////////////////////////////////////////////////////////////////
//NON-PIN FUNCTIONS                                                           //
////////////////////////////////////////////////////////////////////////////////



	///RTC//////////////////////////////////////////////////////////////////////
	//TODO Use the RTC widget to pull real time data from Blynk. Also, consider
	//TODO using webhook instead if you want to pull time from some other source.


	///Eventor//////////////////////////////////////////////////////////////////
	//TODO May be easier to use eventor to trigger events such as turning the lights
	//TODO on at a certain time rather than having the ESP keep track of time.


	///Notifications////////////////////////////////////////////////////////////
	//TODO May or may not use this, not sure if notifications are adding any value.
#endif