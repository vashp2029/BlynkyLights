/* Microphone Analysis
 * By: 		Vash Patel
 * Date: 	May 2018
 *
 * This file is specifically for functions relating to reading and analyzing
 * input from the microphone. The effects based on these inputs will be placed
 * in effects.h.
 */

//Example of ArduinoFFT usage here: https://goo.gl/XvGM2u

#ifndef MIC_H
#define MIC_H

	#include "arduinoFFT.h"

	#define SAMPLINGFREQ 	10000	//Max frequency in Hz to be analyzed (limit 10000)
	#define AMPLITUDE		50		//Increase this if clipping occurs

	double vReal[SOUNDSAMPLES];		//Real number values
	double vImag[SOUNDSAMPLES];		//Imaginary number values (see here: https://goo.gl/6DvU7K)
	uint8_t freqValues[6];			//Store values for each frequency after computing through FFT
	uint8_t peak[6];				//Store the value of the peak for each frequency

	//Since the effect functions may call the values of each frequency many times
	//and many effects will be calling them, we can use pointers instead of calling
	//the variable itself. This will prevent copying of variables to new memory
	//blocks during runtime. This should be safe to do since the effects won't
	//need to actually modify the variables, only read them.
	//TODO can you use a class or struct here instead of declaring a million
	//TODO pointers?
	uint8_t *p125hz		= &freqValues[0];
	uint8_t *p250hz		= &freqValues[1];
	uint8_t *p500hz		= &freqValues[2];
	uint8_t *p1000hz	= &freqValues[3];
	uint8_t *p2000z		= &freqValues[4];
	uint8_t *p4000hz	= &freqValues[5];
	uint8_t *p8000hz	= &freqValues[6];

	uint8_t *pPeak125	= &peak[0];
	uint8_t *pPeak250	= &peak[1];
	uint8_t *pPeak500	= &peak[2];
	uint8_t *pPeak1000	= &peak[3];
	uint8_t *pPeak2000	= &peak[4];
	uint8_t *pPeak4000	= &peak[5];
	uint8_t *pPeak8000	= &peak[6];

	arduinoFFT FFT = arduinoFFT();	//Create an instance of arduinoFFT called FFT

	void freqAnalysis(){
		const static unsigned long samplingPeriod = round(1000000 * (1.0/SAMPLINGFREQ));
		static unsigned long newTime;
		static unsigned long oldTime;

		for(int i = 0; i < SOUNDSAMPLES; i++){
			//Read input from the mic and store it sequentially in vReal array.
			vReal[i] = analogRead(MICPIN);
			vImag[i] = 0;

			//Keep track of how many microseconds since the last iteration of
			//the 'for' loop to prevent from running faster than the sampling
			//period. If the 'for' loop iterated in less time than the sampling
			//period, do nothing until the sampling period is over.
			newTime = micros() - oldTime;
			oldTime = newTime;
			while(micros() < (newTime + samplingPeriod)){
			}
		}

		//Windowing will magnify the frequencies of note and zero out all others.
		//See here: https://goo.gl/pc7efg
		FFT.Windowing(vReal, SOUNDSAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
		FFT.Compute(vReal, vImag, SOUNDSAMPLES, FFT_FORWARD);
		FFT.ComplexToMagnitude(vReal, vImag, SOUNDSAMPLES);

		//The first two samples and the second half of the  array "vReal" are 
		//unusable because of the limitations of ESP8266. Iterate through the
		//rest of them.
		for(int i = 2; i < (SOUNDSAMPLES/2); i++){

			//If the current frequency being iterated over is greater than some
			//threshold we want to store it in the appropriate variable for that
			//frequency for effect functions to use.
			if(vReal[i] > (4 * AMPLITUDE)){
				if(i<=5)			freqValues[0] = (int)vReal[i]/AMPLITUDE;	//125hz
				if(i>5 && i<=12)	freqValues[1] = (int)vReal[i]/AMPLITUDE;	//250hz
				if(i>12 && i<=32)	freqValues[2] = (int)vReal[i]/AMPLITUDE;	//500hz
				if(i>32 && i<=62)	freqValues[3] = (int)vReal[i]/AMPLITUDE;	//1000hz
				if(i>62 && i<=105)	freqValues[4] = (int)vReal[i]/AMPLITUDE;	//2000hz
				if(i>105 && i<=120)	freqValues[5] = (int)vReal[i]/AMPLITUDE;	//4000hz
				if(i>120 && i<=146)	freqValues[6] = (int)vReal[i]/AMPLITUDE;	//8000hz
			}

			for(byte band = 0; band <= 6; band++){
				//TODO the example code prints a band at the peak to decay in the next part
				//TODO of the code, but we're not printing to a display so modify it to 
				//TODO store the peak values in a variable for use in effect functions.
			}
		}

		//Every 4 milliseconds, run a 'for' loop. Iterate the loop from 0
		//to 6 (one iteration for each of the frequencies above). If the peak
		//is greater than 0, reduce it by 1 on each iteration, effectively
		//decaying the peak over time.
		if(millis() % 4 == 0){
			for(byte band = 0; band <= 6; band++){
				if(peak[band] > 0){
					peak[band] -= 1;
				}}}
	}

#endif