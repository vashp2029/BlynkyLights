/* Microphone Analysis
 * By: 		Vash Patel
 * Date: 	May 2018
 *
 * This file is specifically for functions relating to reading and analyzing
 * input from the microphone. The effects based on these inputs will be placed
 * in effects.h.
 *
 * There are two methods of analyzing input from the microphone: we can use FFT
 * to get a spectrum analysis and have effects respond to the different frequencies
 * that may be heard, or we can simply take the value from the microphone as is
 * and do some math to come up with peaks and averages in amplitude. The second
 * is less processer intensive but will be a simpler effect. The former method
 * will give you a lot more flexibility in the kinds of effects you are able to
 * display.
 *
 * The first method is carried out by the freqAnalysis function below, while the
 * second is carried out by the ampAnalysis function below.
 */

//Example of ArduinoFFT usage here: https://goo.gl/XvGM2u

#ifndef MIC_H
#define MIC_H

	#include "arduinoFFT.h"

	#define SOUNDSAMPLES	64		//Number of samples to take for frequency analysis
	#define SAMPLINGFREQ 	6400	//Max frequency in Hz to be analyzed (limit 10000)
	#define AMPLITUDE		10		//Increase this if clipping occurs

	double vReal[SOUNDSAMPLES];		//Real number values
	double vImag[SOUNDSAMPLES];		//Imaginary number values (see here: https://goo.gl/6DvU7K)
	uint8_t freqValues[7];			//Store values for each frequency after computing through FFT

	//Since the effect functions may call the values of each frequency many times
	//and many effects will be calling them, we can use pointers instead of calling
	//the variable itself. This will prevent copying of variables to new memory
	//blocks during runtime. This should be safe to do since the effects won't
	//need to actually modify the variables, only read them.
	//TODO can you use a class or struct here instead of declaring a million
	//TODO pointers? Also, use references instead of pointers here. They are
	//TODO similar but references cannot be changed and do not take up their
	//TODO own memory block as pointers do. See here: https://goo.gl/Lnn9Vq
	uint8_t &p100hz		= freqValues[0];
	uint8_t &p200hz		= freqValues[1];
	uint8_t &p400hz		= freqValues[2];
	uint8_t &p800hz		= freqValues[3];
	uint8_t &p1600hz	= freqValues[4];
	uint8_t &p3200hz	= freqValues[5];
	uint8_t &p6400hz	= freqValues[6];

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
		//See here: https://goo.gl/7mHmV
		//Link above also explains why we want to use FFT_WIN_TYP_HANN, as that
		//is what's recommended for spectrum anaylsis.
		FFT.Windowing(vReal, SOUNDSAMPLES, FFT_WIN_TYP_HANN, FFT_FORWARD);
		FFT.Compute(vReal, vImag, SOUNDSAMPLES, FFT_FORWARD);
		FFT.ComplexToMagnitude(vReal, vImag, SOUNDSAMPLES);

		//The first two samples and the second half of the  array "vReal" are
		//unusable because of the limitations of ESP8266. Iterate through the
		//rest of them.
		for(int i = 2; i < (SOUNDSAMPLES/2); i++){

			//The amplitude of a given sound is stored in the vReal array based
			//on what frequency it is (low frequencies begin at position 2).
			//Here we're interested in looking at which position (i.e. which
			//frequency) the amplitude is stored in and assigning it to the
			//appropriate position in our freqValues array.
			if(vReal[i] > (4 * AMPLITUDE)){
				if(i<=2)			freqValues[0] = (int)vReal[i]/AMPLITUDE;	//100hz
				if(i>2 && i<=4)		freqValues[1] = (int)vReal[i]/AMPLITUDE;	//200hz
				if(i>4 && i<=6)		freqValues[2] = (int)vReal[i]/AMPLITUDE;	//400hz
				if(i>6 && i<=10)	freqValues[3] = (int)vReal[i]/AMPLITUDE;	//800hz
				if(i>10 && i<=14)	freqValues[4] = (int)vReal[i]/AMPLITUDE;	//1600hz
				if(i>14 && i<=20)	freqValues[5] = (int)vReal[i]/AMPLITUDE;	//3200hz
				if(i>20 && i<=32)	freqValues[6] = (int)vReal[i]/AMPLITUDE;	//6400hz
			}
		}
	}




	void ampAnalysis(){

	};
#endif