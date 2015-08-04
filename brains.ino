//This is the prototype program.

/*
After push button, pump starts. When desired vacuum reached,
stop pump. Wait for 5 secs(for demonstration purpose), then 
start CO2. When desired pressure reached, wait for 5 secs,
then release pressure.
*/

#include <Wire.h>
#include <Adafruit_MPL115A2.h> //sensor library

Adafruit_MPL115A2 mpl115a2;

// SDA connected to A4, SCL connected to A5
// pin 1 - 6: Vdd, GND, SCL, SDA, RST, SDWN
// arduino: 5V, GND, A5, A4, 13, 12

//CONSTANTS

const int Button = 9;
//const int Sensor = A1
const int CO2Relay = 11;//Relay on right, CO2 tied to this pin.
const int VacuumRelay = 10;
const int Relay = 8;//Relay on left

const int RESET =13; //should be connected to 5V for sensor to work
const int SHUTDOWN = 12; //should be connected to 5V for sensor to work

const float VacuumLimit = 80;//Change this to limit.
float PressureLimit;
float NormalPressure = 0;


const int VacuumHoldTime = 5;//seconds
const int CO2HoldTime = 5;//seconds


//VARIABLES
float SensorValue = 0;
int VacuumState = LOW;
int CO2State = LOW;
boolean isWorking = false;
int count = 0;



void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
  Serial.println("Serial Started...");
  
  pinMode(Button,INPUT);
  //pinMode(Sensor,INPUT);
  pinMode(CO2Relay,OUTPUT);
  pinMode(VacuumRelay,OUTPUT);
  pinMode(Relay,OUTPUT);
  pinMode(RESET,OUTPUT);
  pinMode(SHUTDOWN,OUTPUT);
  digitalWrite(Button,LOW);
  digitalWrite(CO2Relay,LOW);
  digitalWrite(VacuumRelay,LOW);
  digitalWrite(Relay,LOW);
  //analogWrite(Sensor,0);
  digitalWrite(RESET,HIGH);
  digitalWrite(SHUTDOWN,HIGH);
  delay(1000);
  Serial.println("Getting barometric pressure...");
  mpl115a2.begin();
  NormalPressure = mpl115a2.getPressure();
  delay(1000);
  Wire.begin();
  Serial.print("Pressure: ");
  Serial.print(NormalPressure);
  Serial.print(" kPa\n");
  delay(1000);
  Serial.println("Push Button to Start. Push and Hold To Stop.");
}

void loop() {
  PressureLimit = NormalPressure;
  SensorValue = mpl115a2.getPressure();
  if(digitalRead(Button) == HIGH){
    isWorking = true;
  }
    
  
  while(isWorking){
    
    Serial.println("Cycle Starts...");
    delay(1000);
    Serial.println("Pulling Vacuum...");
    digitalWrite(VacuumRelay,HIGH);
    digitalWrite(Relay,HIGH);
    VacuumState = HIGH;
    
    
    
    while(VacuumState == HIGH){
        SensorValue = mpl115a2.getPressure();
        Serial.print("Pressure: ");
        Serial.print(SensorValue);
        Serial.print(" kPa\n");
        delay(1000);
            //Stop when button pushed, while working.
      if(digitalRead(Button) == HIGH){
        isWorking = false;
        Serial.println("Cycle Ended by User.");
        digitalWrite(VacuumRelay,LOW);
        //Add steps to release pressure here.
        delay(1000);
        Serial.println("Releasing Pressure...");
        SensorValue = mpl115a2.getPressure();
        while(SensorValue <= (NormalPressure+10)){
          digitalWrite(CO2Relay,HIGH);
          SensorValue = mpl115a2.getPressure();
          Serial.print("Pressure: ");
          Serial.print(SensorValue);
          Serial.print(" kPa\n");
          delay(1000);
          if(SensorValue >= (NormalPressure)){
            digitalWrite(CO2Relay,LOW);
            Serial.println("Done.");
            Serial.print("Pressure: ");
            Serial.print(SensorValue);
            Serial.print(" kPa\n");
            delay(5000);
            Serial.println("Push Button to Start. Push Again To Stop.");
            break;
          }
        }
        break;
        }
    
    if(SensorValue <= VacuumLimit){
      digitalWrite(VacuumRelay,LOW);
      digitalWrite(Relay,LOW);
      VacuumState = LOW;
      Serial.println("Vacuum Done.");
      delay(1000);
      count = VacuumHoldTime;
      Serial.println("Holding Vacuum...");
      while(count > 0){
        Serial.println(count);
        count--;
        delay(1000);
        if(digitalRead(Button) == HIGH){
        isWorking = false;
        Serial.println("Cycle Ended by User.");
        //Add steps to release pressure here.
        delay(1000);
        Serial.println("Releasing Pressure...");
        SensorValue = mpl115a2.getPressure();
        while(SensorValue <= (NormalPressure+10)){
          digitalWrite(CO2Relay,HIGH);
          SensorValue = mpl115a2.getPressure();
          Serial.print("Pressure: ");
          Serial.print(SensorValue);
          Serial.print(" kPa\n");
          delay(1000);
          if(SensorValue >= (NormalPressure)){
            digitalWrite(CO2Relay,LOW);
            Serial.println("Done.");
            Serial.print("Pressure: ");
            Serial.print(SensorValue);
            Serial.print(" kPa\n");
            delay(5000);
            Serial.println("Push Button to Start. Push and Hold To Stop.");
            break;
          }
        }
        break;
        }
      }
    }
   }
   if(!isWorking){
      break;
    }
    
    Serial.println("Injecting CO2...");
    digitalWrite(CO2Relay,HIGH);
    CO2State = HIGH;
    
    while(CO2State == HIGH){
        SensorValue = mpl115a2.getPressure();
        Serial.print("Pressure: ");
        Serial.print(SensorValue);
        Serial.print(" kPa\n");
        delay(1000);
        if(digitalRead(Button) == HIGH){
          isWorking = false;
          Serial.println("Cycle Ended by User.");
        //Add steps to release pressure here.
          delay(1000);
          Serial.println("Releasing Pressure...");
          SensorValue = mpl115a2.getPressure();
          while(SensorValue <= (NormalPressure+10)){
          digitalWrite(CO2Relay,HIGH);
          SensorValue = mpl115a2.getPressure();
          Serial.print("Pressure: ");
          Serial.print(SensorValue);
          Serial.print(" kPa\n");
          delay(1000);
          if(SensorValue >= (NormalPressure)){
            digitalWrite(CO2Relay,LOW);
            Serial.println("Done.");
            Serial.print("Pressure: ");
            Serial.print(SensorValue);
            Serial.print(" kPa\n");
            delay(5000);
            Serial.println("Push Button to Start. Push and Hold To Stop.");
            break;
          }
        }
        break;
        }

    if(SensorValue >= (PressureLimit)){
      digitalWrite(CO2Relay,LOW);
      CO2State = LOW;
      Serial.println("CO2 Injecting Done.");
      count = CO2HoldTime;
      delay(1000);
      Serial.println("Holding CO2 Pressure...");
      while(count > 0){
        Serial.println(count);
        count--;
        if(digitalRead(Button) == HIGH){
        isWorking = false;
        Serial.println("Cycle Ended by User.");
        //Add steps to release pressure here.
        delay(1000);
        Serial.println("Releasing Pressure...");
        SensorValue = mpl115a2.getPressure();
        while(SensorValue <= (NormalPressure+10)){
          digitalWrite(CO2Relay,HIGH);
          SensorValue = mpl115a2.getPressure();
          Serial.print("Pressure: ");
          Serial.print(SensorValue);
          Serial.print(" kPa\n");
          delay(1000);
          if(SensorValue >= (NormalPressure)){
            digitalWrite(CO2Relay,LOW);
            Serial.println("Done.");
            Serial.print("Pressure: ");
            Serial.print(SensorValue);
            Serial.print(" kPa\n");
            delay(5000);
            Serial.println("Push Button to Start. Push and Hold To Stop.");
            break;
          }
        }
        break;
        }
        delay(1000);
      }
    }
    }
   if(!isWorking){
      break;
    }
    /*
    Serial.println("Releaseing Pressure...");
    SensorValue = mpl115a2.getPressure();
      while(SensorValue >= (NormalPressure+10)){
        digitalWrite(Relay,HIGH);
        SensorValue = mpl115a2.getPressure();
        if(SensorValue <= (NormalPressure + 10) || SensorValue >= (NormalPressure - 10)){
          digitalWrite(Relay,LOW);
          Serial.println("Done.");
          isWorking = false;
          break;
        }
      }
      */
      Serial.println("Done.");
      Serial.print("Pressure: ");
      Serial.print(SensorValue);
      Serial.print(" kPa\n");
      isWorking = false;
      delay(5000);
      Serial.println("Push Button to Start. Push and Hold To Stop.");
  }
}


