/* Location Specific Definitions
 * By: 		Vash Patel 
 * Date: 	May 2018
 * 
 * This file is simply for definitions that may vary between two different 
 * microcontrollers you have running. For example, if you are using this project
 * for furniture underlighting, you can define locations such as "BED" and "DESK"
 * and have different definitions for each. That way, you only have to alter one
 * line before uploading to each board.
 *
 * You will want to list all the "locations" you have as #ifdef statements here
 * then in the main project file, you will simply add "#define [location]". So
 * for example, let's say your board controlling the lighting under your bed is
 * controlling 150 LEDs, but the one controlling your desk lighting has 100. In
 * that case, you would have "#define NUMLEDS 150" under "#ifdef BED" and under
 * "#ifdef DESK" you would have "#define NUMLEDS 100". Once you set those,
 * you likely will not have to change them too often. Instead, you will simply
 * have "#define BED" in the main file before uploading to that controller.
 * 
 * Descriptions of defintions:

 * SENSORNAME: 	This is exactly what it sounds like. For certain parts of the code
 *				you may want to tell Blynk what the name of the selected sensor
 *				is, for example. You may want to allow OTA updates and using
 *				SENSORNAME in the URL will make it easy to remember how to update
 *				each of your boards.
 *
 * LEDGROUP:	This is a number designation for each of your boards. It will make
 *				it easier to program in when you want a particular board to react
 *				to input from Blynk. For example, let's say you only want the 
 *				LEDs under your bed to react to input from Blynk, you can say
 *				"if selectedLEDs == LEDGROUP then do something". Note that
 *				LEDGROUP 1 is reserved for controlling all groups simultaneously.
 */

#ifndef LOCATIONSPECIFIC_H
  #define LOCATIONSPECIFIC_H

  #ifdef BED
    #define SENSORNAME "bed"
    #define NUMLEDS 150
    #define LEDGROUP 2 //LEDGROUP 1 is reserved as "global" or "all groups"

  #elif COUCH
    #define SENSORNAME "couch"
    #define NUMLEDS 150
    #define LEDGROUP 3

  #elif TV
    #define SENSORNAME "tv"
    #define NUMLEDS 150
    #define LEDGROUP 4

  #elif TVSTAND
    #define SENSORNAME "tv-stand"
    #define NUMLEDS 150
    #define LEDGROUP 5

  #elif WALLDESK
    #define SENSORNAME "wall-desk"
    #define NUMLEDS 150
    #define LEDGROUP 6

  #elif GLASSDESK
    #define SENSORNAME "glass-desk"
    #define NUMLEDS 150
    #define LEDGROUP 7

  #endif

#endif
