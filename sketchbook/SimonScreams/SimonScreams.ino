
#include <Servo.h>
 
Servo jawServo;
Servo neckServo;

const int kledPin = 11;
const int kmicPin = A0;

float filtMicValue;
float avgMicValue;
int samples;

void setup(){
  
  jawServo.attach(9);
  neckServo.attach(10);
  
  pinMode(kledPin, OUTPUT);
  
  Serial.begin(9600); 
}

void loop() {
  
  avgMicValue = 0.0;
  filtMicValue = 0.0;
  samples = 0;
  
  while(1)
  {
    float micValue = analogRead(kmicPin);
    if(micValue > 1 && micValue < 1024)
    {
      
      avgMicValue = ( (9.9*avgMicValue) + (0.1*micValue) )/10.0;
      
      Serial.print(micValue);
      Serial.print("\t");
      
      Serial.print(avgMicValue);
      Serial.print("\t");
      
      micValue -= avgMicValue;
      
      Serial.print(micValue);
      Serial.print("\t");
      
      micValue = sq(micValue);
      
      Serial.print(micValue);
      Serial.print("\t");
      
      filtMicValue = ((9.5*filtMicValue) + (0.5*micValue))/10.0;
      
      Serial.print(samples);
      Serial.print("\t");
      
      Serial.println(filtMicValue);
      
      if(samples > 200 && filtMicValue > 5000.0)
        break;
        
      samples++;
    }
    
    delay(10);
  }
  
  for(int i = 0; i < 5; i++)
  {  
    for(int j = 0; j < 2000; j++)
    {
      jawServo.write(180.0 * ((1 + cos(6.28 * j/-1000.0))/2.0));
      neckServo.write(180.0 * ((1 + sin(6.28 * j/2000.0))/2.0));
      
      analogWrite(kledPin, 255.0 * ((1 + cos(6.28 * j/500.0))/-2.0));
      
      delay(1);
    }
  }
}
