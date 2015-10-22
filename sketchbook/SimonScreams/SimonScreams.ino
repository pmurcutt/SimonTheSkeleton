
#include <Servo.h>
 
Servo jawServo;
Servo neckServo;

const int kledPin = 11;

float lastNeckPos = 90;

const int kMaxNeckPos = 135;
const int kMinNeckPos = 45;
const int kMaxJawPos = 135;
const int kMinJawPos = 45;

const int kcycles = 5;

//const int kmicPin = A0;

//float filtMicValue;
//float avgMicValue;
//int samples;

void setup(){
  
  jawServo.attach(9);
  neckServo.attach(10);
  
  pinMode(kledPin, OUTPUT);
  
  
  
  Serial.begin(9600); 
}

void loop() {
  
//  avgMicValue = 0.0;
//  filtMicValue = 0.0;
//  samples = 0;
  
//  while(1)
//  {
//    float micValue = analogRead(kmicPin);
//    if(micValue > 1 && micValue < 1024)
//    {
//      
//      avgMicValue = ( (9.9*avgMicValue) + (0.1*micValue) )/10.0;
//      
//      Serial.print(micValue);
//      Serial.print("\t");
//      
//      Serial.print(avgMicValue);
//      Serial.print("\t");
//      
//      micValue -= avgMicValue;
//      
//      Serial.print(micValue);
//      Serial.print("\t");
//      
//      micValue = sq(micValue);
//      
//      Serial.print(micValue);
//      Serial.print("\t");
//      
//      filtMicValue = ((9.5*filtMicValue) + (0.5*micValue))/10.0;
//      
//      Serial.print(samples);
//      Serial.print("\t");
//      
//      Serial.println(filtMicValue);
//      
//      if(samples > 200 && filtMicValue > 5000.0)
//        break;
//        
//      samples++;
//    }
//    
//    delay(10);
//  }
  
  float neckTargetPos = random(kMinNeckPos, kMaxNeckPos);
  
  for(int i = 0; i < kcycles; i++)
  { 
    if(i == kcycles - 1)
      neckTargetPos  = 90;
      
    for(int j = 0; j < 2000; j++)
    {
      jawServo.write(map(180.0 * ((1 - cos(6.28 * j/500.0))/2.0), 0, 180, kMinJawPos, kMaxJawPos));
      
      lastNeckPos = (0.999 * lastNeckPos) + (0.001 * neckTargetPos); 
      neckServo.write((int)lastNeckPos);
      
      analogWrite(kledPin, 255.0 * ((1 + cos(6.28 * j/500.0))/-2.0));
      
      delay(1);
    }
  }
  
  //dim leds fully
  analogWrite(kledPin, 0);
  
  //delay(1000);
  //delay for 30s
  delay(30000);
  //delay for 10s-30s
  delay(random(10000,30000));
}
