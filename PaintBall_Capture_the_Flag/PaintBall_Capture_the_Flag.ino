/* a capture the flag style timer for paintball
a team will need to press there coloured button and controll the box for a set time
the time required is accumulated time for each team */

// setup for includes
#include <Servo.h>
Servo AirHorn;

// setup for variables and costants
const int GreenButton = 13; // Green Teams Activation Button
const int YellowButton = 12;
const long GameTimer = 90000; // Game Time 15 Min in milli seconds
const int AirHornOn = 180; // Servo position to blow the horn
const int AirHornOff = 0; // Servo position when horn is off
const int YellowLED = 8;
const int GreenLED = 7;

int GBState = LOW; // for debounce control of Green Button
int YBState = LOW; // for debounce control of Yellow Button
int InControl = 0; // 0 is neutral 1 = Green 2 = Yellow

unsigned long GreenTotalTime = 0;
unsigned long YellowTotalTime = 0;
unsigned long prevMilli = 0;
unsigned long currentMilli = 0;

void setup()
{
  pinMode(GreenButton, INPUT);
  pinMode(YellowButton, INPUT);
  //pinMode(AirHorn, OUTPUT);
  pinMode(YellowLED, OUTPUT);
  pinMode(GreenLED, OUTPUT);
  AirHorn.attach(10); // Attach the servo control to pwm pin 10
}

void loop()
{ // Start Loop
  // if the Green Button is pressed then Green is in control
  if ((digitalRead(GreenButton) ==HIGH) && (GBState == LOW)) // start the debounce
  {
    GBState = HIGH;
    delay(50);
    if ((digitalRead(GreenButton) == HIGH) && (GBState == HIGH)) // if the debounce was true
    {
     // InControl = 1; //Green incontrol of the flag station
     currentMilli = millis();
     if (InControl == 0)
     {
       BlowTheHorn(500);
       BlowTheHorn(500);
       delay(1000);
     }
     InControl = 1; // Green is now incontrol  
     digitalWrite(GreenLED,HIGH);
     GBState = LOW;//release control from the yellow
     BlowTheHorn(500); // To signify that the team has taken control
     
    }
   else
   {
     GBState = LOW;
   }
  }
  // if the yelow button is pressed then yellow is in control    
  if ((digitalRead(YellowButton) ==HIGH) && (YBState == LOW))
  {
    YBState = HIGH;
    delay(50);
    if ((digitalRead(YellowButton) == HIGH) && (YBState == HIGH))
    {
     //InControl = 2; //Yellow incontrol of the flag station
     currentMilli = millis();
     if (InControl == 0)
     {
       BlowTheHorn(500);
       BlowTheHorn(500);
       delay(1000);
     }
     InControl = 2;
     digitalWrite(YellowLED,HIGH);
     YBState = LOW;
     BlowTheHorn(500);
    }
   else
   {
     YBState = LOW;
   }
  }
  
  // Keeping track of time
 if (InControl = 1)
 {
   GreenTotalTime = millis()-currentMilli;
   currentMilli = millis();
 }
 if (InControl = 2)
 {
   YellowTotalTime = millis()-currentMilli;
   currentMilli = millis();
 }
 // Finding a winner
 if (GreenTotalTime >= GameTimer)
 {
   digitalWrite(GreenLED,HIGH);
   digitalWrite(YellowLED,LOW);
   //Blow the horn!!!
   BlowTheHorn(2000);
 }
 if (YellowTotalTime >=GameTimer)
 {
   digitalWrite(YellowLED,HIGH);
   digitalWrite(GreenLED,LOW);
   //Blow The Horn
   BlowTheHorn(2000);
 }
} // End of loop

//**********

// Start of functions
void BlowTheHorn(int Blast)
{
  AirHorn.write(AirHornOn);
  delay(Blast);
  AirHorn.write(AirHornOff);
  delay(50);
}
/*
void greenButton()
{
    // if the Green Button is pressed then Gree is ion control
  if ((GreenButton ==HIGH) && (GBState == LOW))
  {
    GBState = HIGH;
    delay(50);
    if ((GreenButton == HIGH) && (GBState == HIGH))
    {
     InControl = 1; //Green in control of the flag station
     currentMilli = millis();
     digitalWrite(GreenLED,HIGH);
     YBState = LOW;//release control from the yellow
    }
   else
   {
     GBState = LOW;
   }
  }
}

void yellowButton()
{
    // if the yelow button is pressed then yellow is in control    
  if ((YellowButton ==HIGH) && (YBState == LOW))
  {
    YBState = HIGH;
    delay(50);
    if ((YellowButton == HIGH) && (YBState == HIGH))
    {
     InControl = 2; //Green incontrol of the flag station
     currentMilli = millis();
     digitalWrite(YellowLED,HIGH);
     GBState = LOW;
    }
   else
   {
     YBState = LOW;
   }
  }
 
}

*/
