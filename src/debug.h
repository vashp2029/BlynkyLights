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

	//Macro for DEBUG_PRINTF pulled from here: https://goo.gl/mGraLo
	void StreamPrint_progmem(Print &out,PGM_P format,...){
		// program memory version of printf - copy of format string and result share a buffer
		// so as to avoid too much memory use
		char formatString[128], *ptr;
		strncpy_P( formatString, format, sizeof(formatString) ); // copy in from program mem
		// null terminate - leave last char since we might need it in worst case for result's \0
		formatString[ sizeof(formatString)-2 ]='\0'; 
		ptr=&formatString[ strlen(formatString)+1 ]; // our result buffer...
		va_list args;
		va_start (args,format);
		vsnprintf(ptr, sizeof(formatString)-1-strlen(formatString), formatString, args );
		va_end (args);
		formatString[ sizeof(formatString)-1 ]='\0'; 
		out.print(ptr);
	}

	#define DEBUG_PRINTF(format, ...) StreamPrint_progmem(Serial,PSTR(format),##__VA_ARGS__); Serial.println();

#else
	#define DEBUG_PRINT(...)
	#define DEBUG_PRINTLN(...)
	#define DEBUG_BEGIN(...)
	#define DEBUG_PRINTF(...)
#endif

#endif