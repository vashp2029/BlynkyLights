/* Debugging Utility 
 * By: 		Vash Patel 
 * Date: 	May 2018
 *
 * This utility allows me to print to serial while coding but removes all the
 * debugging code before final upload to the board to reduce overhead. To enable
 * debugging, make sure to have #define ENABLE_DEBUG in the main file. Once you
 * are ready for final upload, comment that line out.
 *
 * Keep in mind that in the code, you'll want to use DEBUG_PRINT or DEBUG_PRINTLN
 * instead of Serial.print and Serial.println (you can still use those, but they
 * will not be removed on the final upload, so those will always print to serial.
 * 
 * Also, you will use DEBUG_BEGIN(BAUD) in your setup() function rather than
 * Serial.begin(BAUD). 
 */

#ifndef DEBUGUTIL_H
#define DEBUGUTIL_H

#ifdef ENABLE_DEBUG
	#define DEBUG_PRINT(...) Serial.print(__VA_ARGS__)
	#define DEBUG_PRINTLN(...) Serial.println(__VA_ARGS__)
	#define DEBUG_BEGIN(...) Serial.begin(__VA_ARGS__)

	//FIXIT This method doesn't work for printf. Possible solution here:
	//FIXIT https://goo.gl/2Vv8Wh
	#define DEBUG_PRINTF(...) Serial.printf(__VA_ARGS__)
#else
	#define DEBUG_PRINT(...)
	#define DEBUG_PRINTLN(...)
	#define DEBUG_BEGIN(...)
#endif

#endif