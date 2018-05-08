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
#define FASLET_INTERRUPT_RETRY_COUNT 0


////////////////////////////////////////////////////////////////////////////////
//INCLUDES                                                                    //
////////////////////////////////////////////////////////////////////////////////
//Public Libraries
#include "ESP8266WiFi.h"

//Personal Files
#include "debug.h"
#include "locationSpecific.h"
#include "apikeys.h"


////////////////////////////////////////////////////////////////////////////////
//MAIN PROGRAM                                                                //
////////////////////////////////////////////////////////////////////////////////
void setup() {
    // put your setup code here, to run once:
}

void loop() {
    // put your main code here, to run repeatedly:
}