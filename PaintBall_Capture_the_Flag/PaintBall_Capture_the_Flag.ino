/*

                                      +-----+
         +----[PWR]-------------------| USB |--+
         |                            +-----+  |
         |         GND/RST2  [ ][ ]            |
         |       MOSI2/SCK2  [ ][ ]  A5/SCL[ ] |   C5 
         |          5V/MISO2 [ ][ ]  A4/SDA[ ] |   C4 
         |                             AREF[ ] |
         |                              GND[ ] |
         | [ ]N/C                    SCK/13[x] |   B5
         | [ ]IOREF                 MISO/12[x] |   .
         | [ ]RST                   MOSI/11[ ]~|   .
         | [ ]3V3    +---+               10[x]~|   .
         | [ ]5v    -| A |-               9[ ]~|   .
         | [ ]GND   -| R |-               8[x] |   B0
         | [ ]GND   -| D |-                    |
         | [ ]Vin   -| U |-               7[x] |   D7
         |          -| I |-               6[x]~|   .
         | [ ]A0    -| N |-               5[x]~|   .
         | [ ]A1    -| O |-               4[x] |   .
         | [ ]A2     +---+           INT1/3[x]~|   .
         | [ ]A3                     INT0/2[ ] |   .
         | [ ]A4/SDA  RST SCK MISO     TX>1[ ] |   .
         | [ ]A5/SCL  [ ] [ ] [ ]      RX<0[ ] |   D0
         |            [ ] [ ] [ ]              |
         |  UNO_R3    GND MOSI 5V  ____________/
          \_______________________/


*/

/*
 *MODE 1* 
 -capture the flag acumulated time-
 
 a capture the flag style timer for paintball
 a team will need to press there coloured button and controll the box for a set time
 the time required is accumulated time for each team 
 */

/*
*MODE 2* 
 -capture the flag total time-
 
 standard caputure the flag each team will need
 to press their button to "capture" the flag.
 team in control when the timer ends wins
 
 */

/*
*MODE 3*
 -capture the flag deadman switch-
 
 
 
 */


// setup for includes
#include <Servo.h>
Servo AirHorn;

// setup for variables and costants
const int GreenButton = 13; // Green Teams Activation Button
const int YellowButton = 12; // Yellow Team Activation Button
//const int AirHornOut = 10;
const int GameHorn = 10;
const long GameTimer = 9000; // Game Time 15 Min in milli seconds
const int AirHornOn = 90; // Servo position to blow the horn
const int AirHornOff = 0; // Servo position when horn is off
const int YellowLED = 8;
const int GreenLED = 7;
const int GreenHorn = 3;
const int GreenStrobe = 4;
const int YellowHorn = 5;
const int YellowStrobe = 6;

int GBState = HIGH; // for debounce control of Green Button
int PrevGBState = HIGH;
int YBState = HIGH; // for debounce control of Yellow Button
int PrevYBState = HIGH;
int InControl = 0; // 0 is neutral 1 = Green 2 = Yellow
boolean GameOver = false;

unsigned long GreenTotalTime = 0;
unsigned long YellowTotalTime = 0;
unsigned long prevMilli = 0;
unsigned long currentMilli = 0;

void setup()
{
  pinMode(GreenButton, INPUT);
  pinMode(YellowButton, INPUT);
  // pinMode(AirHornOut, OUTPUT);
  pinMode(GameHorn, OUTPUT);
  pinMode(YellowLED, OUTPUT);
  pinMode(GreenLED, OUTPUT);
  pinMode(GreenHorn, OUTPUT);
  pinMode(GreenStrobe, OUTPUT);
  pinMode(YellowHorn, OUTPUT);
  pinMode(YellowStrobe, OUTPUT);
  // use pullup resistors for the buttons
  digitalWrite(GreenButton,HIGH);
  digitalWrite(YellowButton,HIGH);
  //AirHorn.attach(AirHornOut); // Attach the servo control to pwm pin 10
  Serial.begin(9600); // setup serial for debuging *** remove once all is working ***
}

void loop()
{ // Start Loop
  while(!GameOver)
  { // Start the while loop
    Serial.print("Game Over is ");
    Serial.println(GameOver);

    // if the Green Button is pressed then Green is in control
    GBState = digitalRead(GreenButton);
    if ((GBState == LOW) && (PrevGBState == HIGH)) // start the debounce
    {
      PrevGBState = GBState;
      delay(100);
      GBState = digitalRead(GreenButton);
      if (GBState == LOW && PrevGBState == LOW && InControl != 1) // if the debounce was true
      {
        // Green is in control of the flag station
        currentMilli = millis();
        if (InControl == 0)
        {
          Serial.println("The Game has Begun");
          BlowTheHorn(500);
          BlowTheHorn(500);
          delay(1000);
        }
        InControl = 1; // Green is now in control  
        digitalWrite(GreenLED,HIGH);
        PrevGBState = HIGH;
        BlowTheHorn(500); // To signify that the team has taken control
        Serial.println("Green has taken Control");

      }
      else
      {
        PrevGBState = HIGH;
      }
    }
    // if the yelow button is pressed then yellow is in control

    YBState = digitalRead(YellowButton);
    if (YBState == LOW && PrevYBState == HIGH)
    {
      PrevYBState = YBState;
      delay(100);
      YBState = digitalRead(YellowButton);
      if (YBState == LOW && PrevYBState == LOW && InControl !=2)
      {
        //Yellow incontrol of the flag station
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
        PrevYBState = HIGH;
        BlowTheHorn(500);
        Serial.println("Yellow is In Control");
      }
      else
      {
        PrevYBState = HIGH;
      }
    }

    // Keeping track of time
    if (InControl == 1)
    {
      GreenTotalTime = GreenTotalTime + millis() - currentMilli;
      currentMilli = millis();
      Serial.print("Total Green Time ");
      Serial.println(GreenTotalTime);
      Serial.println(InControl);
    }
    else if (InControl == 2)
    {
      YellowTotalTime = YellowTotalTime + millis() - currentMilli;
      currentMilli = millis();
      Serial.print("Total Yellow Time ");
      Serial.println(YellowTotalTime);
      Serial.println(InControl);
    }
    // Finding a winner
    if (GreenTotalTime >= GameTimer)
    {
      digitalWrite(GreenLED,HIGH);
      digitalWrite(YellowLED,LOW);
      Serial.println("Green is the Winner");
      //Blow the horn!!!
      BlowTheHorn(2000);
      //Wait for the reset key
      GameOver = true;
    }
    if (YellowTotalTime >=GameTimer)
    {
      digitalWrite(YellowLED,HIGH);
      digitalWrite(GreenLED,LOW);
      Serial.println("Yellow is the Winner");
      //Blow The Horn
      BlowTheHorn(2000);
      //Wait for the reset Key
      GameOver = true;
    }
  } // End the while loop
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

void GameBuzzer(int BlastTime)
{
  digitalWrite(GameHorn,HIGH);
  delay(BlastTime);
  digitalWrite(GameHorn,LOW);
}

void YellowBuzzer(int BlastTime)
{
  digitalWrite(YellowHorn,HIGH);
  delay(BlastTime);
  digitalWrite(YellowHorn,LOW);
}

void GreenBuzzer(int BlastTime)
{
  digitalWrite(GreenHorn,HIGH);
  delay(BlastTime);
  digitalWrite(GreenHorn,LOW);
}

//void WaitForReset()





