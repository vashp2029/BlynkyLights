/* Main File
 * By: 		Vash Patel 
 * Date: 	May 2018
 * 
 * This is where most of the code lives, but you will want to be sure to look at
 * debug.h and locationSpecific.h before you begin going through this. The idea
 * behind this project is to enable controlling multiple ESP8266 controllers with
 * the Blynk library to display different effects. For example, you can have
 * under-furniture lighting throughout your living room with a different board
 * for each area of the room. You will be able to control all of them individually
 * when you want to, as well as controlling all of them simultaneously if need be.
 *
 * I use Sublime as my editor with the "HighlightWords" package which allows me
 * to annotate my code so I remember if I need to look back at something. The
 * main keywords I use are listed below. You will see these throughout the code
 * as I work through the project. They may not be appealing to the eye, but that
 * is kind of the point--I'll see them every time I open the file and remember
 * I need to fix something.
 *
 * Keywords:
 * WORKON:	If I need to stop what I'm doing midway through, I will use this tag
 *			to remind myself what I was working on.
 * TODO: 	Some feature I may have thought up while I was coding but that didn't 
 *			warrant taking attention away from what I was working on at the time.
 * FIXIT:	Something that's just broken but I don't have time to look into it
 *			immediately.
 * QSTION:	I have narrowed down what the problem could be but am not sure how to
 *			fix it, so I need to ask online. This is just a more narrow "FIXIT".
 * B4UP:	This tag is permanent. This is just a label for things that need to
 *			be checked/altered before final upload (after debugging).
 */

//B4UP Comment out the following lines to disable debugging to keep overhead low.
#define ENABLE_DEBUG
#define BLYNK_PRINT Serial

//B4UP Define whichever board you are uploading to (select from locationSpecific.h)
#define BED

//If using an LED chipset without clock pin, leave this uncommented to prevent
//flickering. If using something like the APA102C, which has a clock pin, comment
//this out.
#define FASTLED_INTERRUPT_RETRY_COUNT 0




////////////////////////////////////////////////////////////////////////////////
///INCLUDES AND TYPES                                                        ///
////////////////////////////////////////////////////////////////////////////////
//Public Libraries
#include "ESP8266WiFi.h"
#include "WiFiManager.h"
#include "ESP8266mDNS.h"
#include "WiFiUDP.h"
#include "ArduinoOTA.h"
#include "ESP8266WebServer.h"
#include "ESP8266HTTPUpdateServer.h"
#include "FastLED.h"
#include "BlynkSimpleEsp8266.h"

//Personal Files
#include "debug.h"              //Macros for turning DEBUG_PRINT to Serial.print
#include "locationSpecific.h"   //Easily define different parameters for different microcontrollers
#include "apikeys.h"            //Not necessarily just API keys, really any secret info
#include "config.h"             //User configuration file
#include "time.h"               //For pulling time data

//Wifi Manager
WiFiManager wifiManager;

//OTA and HTTP Updates
ESP8266WebServer httpServer(HTTPPORT);
ESP8266HTTPUpdateServer httpUpdater;

//FastLED
struct CRGB leds[NUMLEDS];

//Blynk
BlynkTimer blynkTimer;




////////////////////////////////////////////////////////////////////////////////
///MAIN PROGRAM                                                              ///
////////////////////////////////////////////////////////////////////////////////
void setup() {
    DEBUG_BEGIN(115200);



    ///FastLED//////////////////////////////////////////////////////////////////
    DEBUG_PRINTF("Beginning FastLED setup.");

    FastLED.addLeds<CHIPSET, DATAPIN, COLORORDER>(leds, NUMLEDS);
    FastLED.setCorrection(TypicalLEDStrip);		//Color correction

    //Use FastLED's power management function for controlling brightness
    //throughout the sketch. Details here: https://goo.gl/VrST2K
    set_max_power_in_volts_and_milliamps(MAXVOLTS, MAXAMPS);

    DEBUG_PRINTF("FastLED setup is complete.");



    ///Wifi/////////////////////////////////////////////////////////////////////
    DEBUG_PRINTF("Beginning WiFi setup.");

    //If autoconnect fails, WifiManager will start an access point with the
    //SENSORNAME as the SSID. Connect to it and use the capture page to
    //configure a new wifi connection.
    wifiManager.autoConnect(SENSORNAME, PASSWORD);

    //Don't move forward with the program until WiFi is connected.
    while(WiFi.status() != WL_CONNECTED){
    	DEBUG_PRINT(".");
    	delay(500);
    }

    IPAddress ip = WiFi.localIP();

    DEBUG_PRINTF("WiFi setup is complete. Current IP address is: %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);



    ///OTA Updates//////////////////////////////////////////////////////////////
    //TODO Implement a check for free space before starting OTA update using
    //TODO ESP.getFreeSketchSpace(); More info here: https://goo.gl/WeSaLj
    
    //Great example of ArduinoOTA implementation here: https://goo.gl/3yx6ah
	DEBUG_PRINTF("Beginning OTA setup.");

	ArduinoOTA.setPort(OTAPORT);
	ArduinoOTA.setHostname(SENSORNAME);
	ArduinoOTA.setPassword(PASSWORD);

	ArduinoOTA.onStart([](){
		DEBUG_PRINTF("Recieving OTA update.");
	});

	ArduinoOTA.onProgress([](uint8_t progress, uint8_t total){
		DEBUG_PRINTF("Download progress: %u%%\r", (progress / (total / 100)));
	});

	ArduinoOTA.onEnd([](){
		DEBUG_PRINTF("Completed recieving OTA update.");
	});

    ArduinoOTA.onError([](ota_error_t error){
		DEBUG_PRINTF("Error[%u]: ", error);

		if(error == OTA_AUTH_ERROR) DEBUG_PRINTLN("Failed authorization.");
		else if(error == OTA_BEGIN_ERROR) DEBUG_PRINTLN("Could not begin download.");
		else if(error == OTA_CONNECT_ERROR) DEBUG_PRINTLN("Connection failed.");
		else if(error == OTA_RECEIVE_ERROR) DEBUG_PRINTLN("Update not received.");
		else if(error == OTA_END_ERROR) DEBUG_PRINTLN("Could not finish download.");
	});
    
    ArduinoOTA.begin();

    DEBUG_PRINTF("Beginning setup of HTTP server to recieve updates.");

    MDNS.begin(SENSORNAME);
    httpUpdater.setup(&httpServer);
    httpServer.begin();
    MDNS.addService("http", "tcp", HTTPPORT);

    DEBUG_PRINTF("Accepting updates over HTTP at URL: http://%s.local/update", SENSORNAME);

    DEBUG_PRINTF("OTA setup is complete.");



    ///Blynk////////////////////////////////////////////////////////////////////
    DEBUG_PRINTF("Setting up connection to Blynk servers...");

    //Use Blynk.config() instead of Blynk.begin() because the latter attempts to
    //manage WiFi which is already being done with WifiManager.
    Blynk.config(BLYNKAUTH);

    while(!Blynk.connect()){
        DEBUG_PRINT(".");
        delay(500);
    }

    DEBUG_PRINTF("Successfully connected to Blynk servers.");

    blynkTimer.setInterval(TIMEINTERVAL, syncTime);
    blynkTimer.setInterval(1000L, incrementTime);



    ///Initial Time Sync////////////////////////////////////////////////////////
    syncTime();
    getSunriseSunset();
}




void loop() {
	///OTA Updates//////////////////////////////////////////////////////////////
    httpServer.handleClient();
    ArduinoOTA.handle();



    ///Blynk////////////////////////////////////////////////////////////////////
    Blynk.run();
    blynkTimer.run();
}