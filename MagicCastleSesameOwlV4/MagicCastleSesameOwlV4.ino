/*Magic Castle - Open Sesame Door - OWL
 * version 2 - master timing sequence 
 * modified 3/5/20
 * by Ryan Kelley
 * 
 * New Linear Actuator Mech
 * 
 * 
 * 
 */

#include <Servo.h> 
Servo owlServo;
const byte OWLEYE = 10, OWLEYE2 = 11, START = 2, SRVPWR = 6, HOOT = 4;
byte pos = 30;
int flashCount = 0, servoCount = 330, seqTime = 600; //servoCount range 180-330
unsigned long currentMillis=0, previousMillis=0;
int eyesOn[] =   {2, 50, 125, 175, 250, 300, 375, 425, 500, 550};
int eyesOff[] = {25, 85, 150, 210, 275, 335, 400, 460, 525, 585};

void setup() 
{
  owlServo.attach(9);
  owlServo.write(pos);
  pinMode (START, INPUT_PULLUP);
  pinMode (OWLEYE, OUTPUT); pinMode (OWLEYE2, OUTPUT); pinMode (SRVPWR, OUTPUT); pinMode (HOOT, OUTPUT);
  digitalWrite(HOOT, LOW);
  digitalWrite(SRVPWR, HIGH);
}

void loop() 
{
  currentMillis = millis();
  if(currentMillis >= (previousMillis + 10))
  {
    previousMillis = currentMillis;
    if(digitalRead(START) == LOW)
    {
      if(seqTime > 0)
      {seqTime--;}

      digitalWrite(SRVPWR, HIGH);
    
      if(flashCount < 2000)
        {flashCount++;}

      if(seqTime < 575)
      {
        if(servoCount > 130)
        {
          servoCount--;
          digitalWrite(HOOT, LOW);
        }
      }
      if(servoCount == 130) // end of move point (head turned) 180
      {
        digitalWrite(SRVPWR, LOW);
        digitalWrite(HOOT, HIGH);
      }
      flashEye();
      owlHead();
    }
    else
    {
      if(seqTime < 600)
      {seqTime++;}
      digitalWrite(SRVPWR, HIGH);
      digitalWrite(HOOT, HIGH);
      digitalWrite(OWLEYE, LOW);
      digitalWrite(OWLEYE2, LOW);
      flashCount = 0;
      if(seqTime > 25)
      {
        if(servoCount < 330) 
        {servoCount++;}
      }
      if(servoCount == 330) // start of move point (head forward) 330
        {digitalWrite(SRVPWR, LOW);}
      owlHead();
    }
  }
}

void flashEye()
{
  for(byte i=0; i <= 9; i++)
  {
    if(eyesOn[i] == flashCount)
    {
      digitalWrite(OWLEYE, HIGH);
      digitalWrite(OWLEYE2, HIGH);
    }
    if(eyesOff[i] == flashCount)
    {
      digitalWrite(OWLEYE, LOW);
      digitalWrite(OWLEYE2, LOW);  
    }
  }
}

void owlHead()
{
  pos = (servoCount/3); //range 330 - 180 = 150 
  owlServo.write(pos+10); //head trim #
}
