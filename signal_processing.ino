// EOG Filter - BioAmp EXG Pill
// https://github.com/upsidedownlabs/BioAmp-EXG-Pill

// Upside Down Labs invests time and resources providing this open source code,
// please support Upside Down Labs and open-source hardware by purchasing
// products from Upside Down Labs!

// Copyright (c) 2021 Upside Down Labs - contact@upsidedownlabs.tech

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <SimpleKalmanFilter.h>
//#include "SerialDataExporter.h"


#define SAMPLE_RATE 75
#define BAUD_RATE 9600
#define INPUT_PIN_V A0
#define INPUT_PIN_H A1
#define BUFFER_SIZE 50
#define LED_UP 8
#define LED_DOWN 9
#define DATA_LENGTH 10
#define RUNTIME_PERIOD 50
#define IGNORE_PERIOD 80

float signalBuffer[BUFFER_SIZE]; // Buffer for storing the signal values
int bufferIndex = 0; // Index for the buffer
float sum = 0; // Sum for calculating the moving average
float average = 0; // Result of the moving average




float HIGH_THRESHOLD_V = 100;
float LOW_THRESHOLD_V = -80;

float HIGH_THRESHOLD_H = 100;
float LOW_THRESHOLD_H = -80;


float EOGFilter(float input);


SimpleKalmanFilter simpleKalmanFilterVer(1, 1, 0.01);
SimpleKalmanFilter simpleKalmanFilterHoz(1, 1, 0.01);


////exporter variable for json export
//int bufferSizes[] = {100, 0, 1};
//SerialDataExporter exporter = SerialDataExporter(Serial, bufferSizes);


//function to get the first positive peak of the look up wave
float get_pos_peak_v() {
  unsigned long past = 0;
  float peak = 0;
  unsigned long start = micros();
  while(1) {
    if(micros() - past >= 1000000 / SAMPLE_RATE) {
      past = micros();
      float sensor_value = analogRead(INPUT_PIN_V);
      float signal = EOGFilterV(sensor_value);
      float filtered_signal = simpleKalmanFilterVer.updateEstimate(signal);
//      Serial.println(filtered_signal);
      if(micros() - start > 2000000) {
        if(peak < filtered_signal) {
          peak = filtered_signal;
        }
        else if(peak - filtered_signal >= 170) {
          if(filtered_signal > 0) {
            return peak;
          }  
        }
      }   
    }
  }
}


//function to get the first negative peak of the look down wave
float get_neg_peak_v() {
  unsigned long past = 0;
  float peak = 0;
  unsigned long start = micros();
  while(1) {
    if(micros() - past >= 1000000 / SAMPLE_RATE) {
      past = micros();
      float sensor_value = analogRead(INPUT_PIN_V);
      float signal = EOGFilterV(sensor_value);
      float filtered_signal = simpleKalmanFilterVer.updateEstimate(signal);
//      Serial.println(filtered_signal);
      if(micros() - start > 2000000) 
        if(peak > filtered_signal) {
          peak = filtered_signal;
        }
        else if(filtered_signal - peak >= 90) {
          if(filtered_signal < 0) {
            return peak;
          }
        }
    }
  }
}


//function to get the first positive peak of the look up wave
float get_pos_peak_h() {
  unsigned long past = 0;
  float peak = 0;
  unsigned long start = micros();
  while(1) {
    if(micros() - past >= 1000000 / SAMPLE_RATE) {
      past = micros();
      float sensor_value = analogRead(INPUT_PIN_H);
      float signal = EOGFilterH(sensor_value);
      float filtered_signal = simpleKalmanFilterHoz.updateEstimate(signal);
//      Serial.println(filtered_signal);
      if(micros() - start > 2000000) {
        if(peak < filtered_signal) {
          peak = filtered_signal;
        }
        else if(peak - filtered_signal >= 170) {
          if(filtered_signal > 0) {
            return peak;
          }  
        }
      }   
    }
  }
}


//function to get the first negative peak of the look down wave
float get_neg_peak_h() {
  unsigned long past = 0;
  float peak = 0;
  unsigned long start = micros();
  while(1) {
    if(micros() - past >= 1000000 / SAMPLE_RATE) {
      past = micros();
      float sensor_value = analogRead(INPUT_PIN_H);
      float signal = EOGFilterH(sensor_value);
      float filtered_signal = simpleKalmanFilterHoz.updateEstimate(signal);
//      Serial.println(filtered_signal);
      if(micros() - start > 2000000) 
        if(peak > filtered_signal) {
          peak = filtered_signal;
        }
        else if(filtered_signal - peak >= 90) {
          if(filtered_signal < 0) {
            return peak;
          }
        }
    }
  }
}




void setup() {
  // Serial connection begin
  Serial.begin(BAUD_RATE);
//  Serial.println("Start calibration process");
  pinMode(LED_UP, OUTPUT);
  pinMode(LED_DOWN, OUTPUT);
  pinMode(INPUT_PIN_V, INPUT);
  pinMode(INPUT_PIN_H, INPUT);
  pinMode(2, INPUT_PULLUP);

  //Start the calibration
  float pos_peak_data_ver[3] = {0.0};
  int pos_peak_count = 0;
  float neg_peak_data_ver[3] = {0.0};
  int neg_peak_count = 0;
  for(int i = 0; i < 6; i++) {
    Serial.println("3");
    delay(1000);
    Serial.println("2");
    delay(1000);
    Serial.println("1");
    delay(1000);
    Serial.println("0");
    if(i % 2 == 0) {
       pos_peak_data_ver[pos_peak_count++] = get_pos_peak_v();
       
    }
    else {
      neg_peak_data_ver[neg_peak_count++] = get_neg_peak_v();
    }
  }
  for(int i = 0; i < 3; i++) {
    HIGH_THRESHOLD_V += pos_peak_data_ver[i];
    LOW_THRESHOLD_V += neg_peak_data_ver[i];
  }
  HIGH_THRESHOLD_V = (HIGH_THRESHOLD_V / 3) * 0.6; //set 60% of the mean as threshold
  LOW_THRESHOLD_V = (LOW_THRESHOLD_V / 3) * 0.5; //set 60% of the mean as threshold
//  Serial.print("High threshold is:");
//  Serial.println(HIGH_THRESHOLD);
//  Serial.print("Low threshold is:");
//  Serial.println(LOW_THRESHOLD);



  //Start the calibration
  float pos_peak_data_hoz[3] = {0.0};
  pos_peak_count = 0;
  float neg_peak_data_hoz[3] = {0.0};
  neg_peak_count = 0;
  for(int i = 0; i < 6; i++) {
    Serial.println("3");
    delay(1000);
    Serial.println("2");
    delay(1000);
    Serial.println("1");
    delay(1000);
    Serial.println("0");
    if(i % 2 == 0) {
       pos_peak_data_hoz[pos_peak_count++] = get_pos_peak_h();
       
    }
    else {
      neg_peak_data_hoz[neg_peak_count++] = get_neg_peak_h();
    }
  }
  for(int i = 0; i < 3; i++) {
    HIGH_THRESHOLD_H += pos_peak_data_hoz[i];
    LOW_THRESHOLD_H += neg_peak_data_hoz[i];
  }
  HIGH_THRESHOLD_H = (HIGH_THRESHOLD_H / 3) * 0.5; //set 60% of the mean as threshold
  LOW_THRESHOLD_H = (LOW_THRESHOLD_H / 3) * 0.6; //set 60% of the mean as threshold
  //End the calibration
  Serial.print("Z");
}



int peak_ver[4] = {0};
int positive_peak_flag_v;
int negative_peak_flag_v;

int peak_hoz[4] = {0};
int positive_peak_flag_h;
int negative_peak_flag_h;


double filtered_signal_v; //eog signal for printout
double filtered_signal_h; //eog signal for printout

void shift_input_v(int new_input) {
  for(int i = 2; i >= 0; i--) {
    peak_ver[i+1] = peak_ver[i];
  }
  peak_ver[0] = new_input;
}



void reset_input_v() {
  for(int i = 0; i < 3; i++) {
    peak_ver[i] = 0;
  }
}


void shift_input_h(int new_input) {
  for(int i = 2; i >= 0; i--) {
    peak_hoz[i+1] = peak_hoz[i];
  }
  peak_hoz[0] = new_input;
}



void reset_input_h() {
  for(int i = 0; i < 3; i++) {
    peak_hoz[i] = 0;
  }
}




char data_send;


void loop() {

  
  
  // Calculate elapsed time
  static unsigned long past = 0;
  unsigned long present = micros();
  unsigned long interval = present - past;
  past = present;

  // Run timer
  static long timer = 0;
  timer -= interval;

  if(digitalRead(2) == LOW) {
    reset_input_v();
    reset_input_h();
  }
  

  // Sample
  if (timer < 0) {
    timer += 1000000 / SAMPLE_RATE;
    double sensor_value_v = analogRead(INPUT_PIN_V);
    double sensor_value_h = analogRead(INPUT_PIN_H);
    double signal_v = EOGFilterV(sensor_value_v);
    double signal_h = EOGFilterH(sensor_value_h);
    filtered_signal_v = simpleKalmanFilterVer.updateEstimate(signal_v);
    filtered_signal_h = simpleKalmanFilterHoz.updateEstimate(signal_h);
//    Serial.print(filtered_signal_v);
//    Serial.print(",");
//    Serial.println(filtered_signal_h);

    //vertical peak detection
    if (positive_peak_flag_v == 1) {
      if (filtered_signal_v < HIGH_THRESHOLD_V) { //up signal detected
        positive_peak_flag_v = 0;
        shift_input_v(1);
      }

    }


    else if (negative_peak_flag_v == 1) {
      if (filtered_signal_v > LOW_THRESHOLD_V) { //down signal detected
        negative_peak_flag_v = 0;
        shift_input_v(-1);
      }

    }

    else if (filtered_signal_v > HIGH_THRESHOLD_V) { //start detection of up signal

        positive_peak_flag_v = 1;
    }

    else if (filtered_signal_v < LOW_THRESHOLD_V) { //start detection of down signal

      negative_peak_flag_v = 1;
    }

    //horizontal peak detection
    if (positive_peak_flag_h == 1) {
      if (filtered_signal_h < HIGH_THRESHOLD_H) { //up signal detected
        positive_peak_flag_h = 0;
        shift_input_h(1);
      }

    }


    else if (negative_peak_flag_h == 1) {
      if (filtered_signal_h > LOW_THRESHOLD_H) { //down signal detected
        negative_peak_flag_h = 0;
        shift_input_h(-1);
      }

    }

    else if (filtered_signal_h > HIGH_THRESHOLD_H) { //start detection of up signal

        positive_peak_flag_h = 1;
    }

    else if (filtered_signal_h < LOW_THRESHOLD_H) { //start detection of down signal

      negative_peak_flag_h = 1;
    }
  }

  
  //vertical
  if (peak_ver[3] != 0) {
    if ((peak_ver[0] == 1 && peak_ver[1] == -1 && peak_ver[2] == -1 && peak_ver[3] == 1)) {
      Serial.print("U");
    } else if (peak_ver[0] == -1 && peak_ver[1] == 1 && peak_ver[2] == 1 && peak_ver[3] == -1) {
      Serial.print("D");
    }
    reset_input_v();
  }


  
  if(peak_ver[0] == -1 && peak_ver[1] == 1 && peak_ver[2] == -1) {
      Serial.print("D");
      reset_input_v();
  }

  if(peak_ver[0] == 1 && peak_ver[1] == -1 && peak_ver[2] == 1) {
    Serial.print("U");
    reset_input_v();
  }

  //horizontal
  if (peak_hoz[3] != 0) {
    if (peak_hoz[0] == 1 && peak_hoz[1] == -1 && peak_hoz[2] == -1 && peak_hoz[3] == 1) {
      Serial.print("L");
    } else if (peak_hoz[0] == -1 && peak_hoz[1] == 1 && peak_hoz[2] == 1 && peak_hoz[3] == -1) {
      Serial.print("R");
    }
    reset_input_h();
  }

  if(peak_hoz[0] == -1 && peak_hoz[1] == 1 && peak_hoz[2] == -1) {
    Serial.print("R");
    reset_input_h();
  }

  if(peak_hoz[0] == 1 && peak_hoz[1] == -1 && peak_hoz[2] == 1) {
    Serial.print("L");
    reset_input_h();
  }

}








// Band-Pass Butterworth IIR digital filter, generated using filter_gen.py.
// Sampling rate: 75.0 Hz, frequency: [0.5, 19.5] Hz.
// Filter is order 4, implemented as second-order sections (biquads).
// Reference:
// https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.butter.html
// https://courses.ideate.cmu.edu/16-223/f2020/Arduino/FilterDemos/filter_gen.py
float EOGFilterV(float input)
{
  float output = input;
  {
    static float z1, z2; // filter section state
    float x = output - 0.02977423 * z1 - 0.04296318 * z2;
    output = 0.09797471 * x + 0.19594942 * z1 + 0.09797471 * z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - 0.08383952 * z1 - 0.46067709 * z2;
    output = 1.00000000 * x + 2.00000000 * z1 + 1.00000000 * z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - -1.92167271 * z1 - 0.92347975 * z2;
    output = 1.00000000 * x + -2.00000000 * z1 + 1.00000000 * z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - -1.96758891 * z1 - 0.96933514 * z2;
    output = 1.00000000 * x + -2.00000000 * z1 + 1.00000000 * z2;
    z2 = z1;
    z1 = x;
  }
  return output;
}


float EOGFilterH(float input)
{
  float output = input;
  {
    static float z1, z2; // filter section state
    float x = output - 0.02977423 * z1 - 0.04296318 * z2;
    output = 0.09797471 * x + 0.19594942 * z1 + 0.09797471 * z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - 0.08383952 * z1 - 0.46067709 * z2;
    output = 1.00000000 * x + 2.00000000 * z1 + 1.00000000 * z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - -1.92167271 * z1 - 0.92347975 * z2;
    output = 1.00000000 * x + -2.00000000 * z1 + 1.00000000 * z2;
    z2 = z1;
    z1 = x;
  }
  {
    static float z1, z2; // filter section state
    float x = output - -1.96758891 * z1 - 0.96933514 * z2;
    output = 1.00000000 * x + -2.00000000 * z1 + 1.00000000 * z2;
    z2 = z1;
    z1 = x;
  }
  return output;
}
