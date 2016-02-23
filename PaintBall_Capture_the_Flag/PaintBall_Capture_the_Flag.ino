/* a capture the flag style timer for paintball
a team will need to press there coloured button and controll the box for a set time
the time required is accumulated time for each team */

// setup for includes
#include <Servo.h>
Servo AirHorn;

// setup for variables and costants
const int GreenButton = 13; // Green Teams Activation Button
const int YellowButton = 12; // Yellow Team Activation Button
const int AirHornOut = 10;
const long GameTimer = 90000; // Game Time 15 Min in milli seconds
const int AirHornOn = 180; // Servo position to blow the horn
const int AirHornOff = 0; // Servo position when horn is off
const int YellowLED = 8;
const int GreenLED = 7;

int GBState = LOW; // for debounce control of Green Button
int PrevGBState = LOW;
int YBState = LOW; // for debounce control of Yellow Button
int PrevYBState = LOW;
int InControl = 0; // 0 is neutral 1 = Green 2 = Yellow

unsigned long GreenTotalTime = 0;
unsigned long YellowTotalTime = 0;
unsigned long prevMilli = 0;
unsigned long currentMilli = 0;

void setup()
{
  pinMode(GreenButton, INPUT);
  pinMode(YellowButton, INPUT);
  pinMode(AirHornOut, OUTPUT);
  pinMode(YellowLED, OUTPUT);
  pinMode(GreenLED, OUTPUT);
  AirHorn.attach(AirHornOut); // Attach the servo control to pwm pin 10
  Serial.begin(9600); // setup serial for debuging *** remove once all is working ***
}

void loop()
{ // Start Loop
  // if the Green Button is pressed then Green is in control
  GBState = digitalRead(GreenButton);
  if ((GBState == HIGH) && (PrevGBState == LOW)) // start the debounce
  {
    PrevGBState = HIGH;
    delay(50);
    GBState = digitalRead(GreenButton);
    if (((GBState == HIGH) && (PrevGBState == HIGH)) && (InControl != 1)) // if the debounce was true
    {
     // InControl = 1; //Green incontrol of the flag station
     currentMilli = millis();
     if (InControl == 0)
     {
       Serial.println("The Game has Begun");
       BlowTheHorn(500);
       BlowTheHorn(500);
       delay(1000);
     }
     InControl = 1; // Green is now incontrol  
     digitalWrite(GreenLED,HIGH);
     //GBState = LOW;//release control from the yellow
     PrevGBState = LOW;
     BlowTheHorn(500); // To signify that the team has taken control
     Serial.println("Green has taken Control");
     
    }
   else
   {
     PrevGBState = LOW;
   }
  }
  // if the yelow button is pressed then yellow is in control
  
  YBState = digitalRead(YellowButton);
  if ((YBState ==HIGH) && (PrevYBState == LOW))
  {
    PrevYBState = HIGH;
    delay(50);
    YBState = digitalRead(YellowButton);
    if (((YBState == HIGH) && (PrevYBState == HIGH)) && (InControl !=2))
    {
     //InControl = 2; //Yellow incontrol of the flag station
     currentMilli = millis();
     if (InControl == 0)
     {
       Serial.println("The Game has Begun");
       BlowTheHorn(500);
       BlowTheHorn(500);
       delay(1000);
     }
     InControl = 2;
     digitalWrite(YellowLED,HIGH);
     //YBState = LOW;
     PrevYBState = LOW;
     BlowTheHorn(500);
     Serial.println("Yellow is In Control");
    }
   else
   {
     PrevYBState = LOW;
   }
  }
  
  // Keeping track of time
 if (InControl = 1)
 {
   GreenTotalTime = GreenTotalTime + millis() - currentMilli;
   currentMilli = millis();
   Serial.print("Total Green Time ");
   Serial.println(GreenTotalTime);
 }
 if (InControl = 2)
 {
   YellowTotalTime = YellowTotalTime + millis() - currentMilli;
   currentMilli = millis();
   Serial.print("Total Yellow Time ");
   Serial.println(YellowTotalTime);
 }
 // Finding a winner
 if (GreenTotalTime >= GameTimer)
 {
   digitalWrite(GreenLED,HIGH);
   digitalWrite(YellowLED,LOW);
   Serial.println("Green is the Winner");
   //Blow the horn!!!
   BlowTheHorn(2000);
 }
 if (YellowTotalTime >=GameTimer)
 {
   digitalWrite(YellowLED,HIGH);
   digitalWrite(GreenLED,LOW);
   Serial.println("Yellow is the Winner");
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

