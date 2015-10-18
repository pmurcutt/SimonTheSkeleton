/*

 Drive a servo based on power of vocal frequencies
 Copyright (C) 2015 P. Murcutt
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 */
#include <Servo.h> 
#include "PlainFFT.h"

Servo myservo;  // create servo object to control a servo 
// a maximum of eight servo objects can be created 

PlainFFT FFT = PlainFFT(); /* Create FFT object */
/* 
 These values can be changed in order to evaluate the functions 
 NOTE: human voice operates in 80Hz - 10kHz range
 */
 
 // TODO: up sample rate to match all human voices
const uint16_t samples = 64;
double signalFrequency = 1000;
double samplingFrequency = 5000;
uint8_t signalIntensity = 100;

/*
The mic data is sored here to persist between loops
*/
double waveform[samples];

/*
The current index of waveform
*/
int waveformHead = 0;

/* 
 These are the input and output vectors 
 Input vectors receive computed results from FFT
 */
double vReal[samples]; 
double vImag[samples];

#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02

#define mic_ana_pin 1
#define servo_pin 5

void setup()
{
  // attaches the servo pin to the servo object 
  myservo.attach(servo_pin);  

  //zero out the waveform
  for (uint8_t i = 0; i < samples; ++i) {
    waveform[i] = 0.0;
  }
  
  //start the comms
  Serial.begin(115200);
  Serial.println("Ready");
}

void loop() 
{
  //log start time
  unsigned long startMillis= millis();
  
  //read mic data
  waveform[waveformHead] = analogRead(mic_ana_pin); //0-1023
  
  //copy waveform out
  for (uint8_t i = 0; i < samples; ++i) 
  {
    vReal[i] = waveform[ (i + waveformHead) % samples ];
  }
  //update waveformHead for next pass
  waveformHead += 1;
  waveformHead %= samples;

  //FFT!
  //PrintVector(vReal, samples, SCL_TIME);
  FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);        /* Weigh data */
  //PrintVector(vReal, samples, SCL_TIME);
  FFT.Compute(vReal, vImag, samples, FFT_FORWARD); /* Compute FFT */
  //PrintVector(vReal, samples, SCL_INDEX);
  //PrintVector(vImag, samples, SCL_INDEX);
  FFT.ComplexToMagnitude(vReal, vImag, samples); /* Compute magnitudes */
  //PrintVector(vReal, (samples >> 1), SCL_FREQUENCY);      
  //double x = FFT.MajorPeak(vReal, samples, samplingFrequency);

  //TODO: extract power in system...
  double x = 0.0;
  
  for (uint8_t i = 0; i < samples; ++i) 
  {
    x += vReal[i];
  }
  
  Serial.println(x, 6);

  //TODO: Scale output to servo
  myservo.write(x);
  
  //log finished time & sleep for remaining of sampling time
  unsigned long stopMillis= millis();
  int32_t delayRemaining = (1000.0/samplingFrequency) - (stopMillis - startMillis);
  if(delayRemaining > 0)
  {
    delay(delayRemaining);
  }
  else
  {
    Serial.print("Delay overran!");
  }
  
}

void PrintVector(double *vData, uint8_t bufferSize, uint8_t scaleType) 
{       
  for (uint16_t i = 0; i < bufferSize; i++) {
    double abscissa;
    /* Print abscissa value */
    switch (scaleType) {
    case SCL_INDEX:
      abscissa = (i * 1.0);
      break;
    case SCL_TIME:
      abscissa = ((i * 1.0) / samplingFrequency);
      break;
    case SCL_FREQUENCY:
      abscissa = ((i * 1.0 * samplingFrequency) / samples);
      break;
    }
    Serial.print(abscissa, 6);
    Serial.print(" ");
    Serial.print(vData[i], 4);
    Serial.println();
  }
  Serial.println();
}

