/* Magic Castle - Open Sesame Door
 modified 15 July 2018
 by Ryan Kelley
 version 6

  Three button input. 
  Primary relay output
  OWL action removed
  Modified Door Safety loop location
 
*/ 

const byte OPNBTN = 4, CLSBTN = 5, MOMBTN = 3, DOOROPN = 9, DOORCLS = 10, STOP = 6, OWL = 11;
byte doorState = 0, momState = 0;
int safeOpenDelay = 0, doorOCount = 600, doorCCount = 600, momDeb = 100;
unsigned long currentMillis=0, previousMillis=0;
 
void setup() 
{  
  pinMode (OPNBTN, INPUT); pinMode (CLSBTN, INPUT); pinMode (MOMBTN, INPUT); pinMode (STOP, INPUT);
  pinMode (DOOROPN, OUTPUT); pinMode (DOORCLS, OUTPUT); pinMode(OWL, OUTPUT);
  digitalWrite(DOOROPN, LOW); digitalWrite(DOORCLS, HIGH);
  Serial.begin(9600);
} 
 
void loop() 
{
//////////////////////////////////////////////TIMING LOOP///////////////////////////////////////////////////////////////
  
  currentMillis = millis();
  if(currentMillis >= (previousMillis + 10))
  {
    previousMillis = currentMillis;
    if(safeOpenDelay != 0)
      {safeOpenDelay--;}
    if((doorState == 1) || (momState == 1))
    {
      digitalWrite(OWL, HIGH);
      if(doorOCount != 0)
        {doorOCount--;}
      doorCCount=600;
    }
    else
    {
      digitalWrite(OWL, LOW);
      if(doorCCount != 0)
        {doorCCount--;}
      doorOCount=600;
    }
  }
    
//////////////////////////////////////////////// STATE LOOP /////////////////////////////////////////////////////////////  
  
    if(doorState == 0)                  //checks to see if door state is 'closed', this is default mode, door closed looking for open or momentary buttons
    {
      if(momState == 1)                 //checks to see if momentary state is true
        {doorOpen();}                     //if momentary state is true, opens door                     
      else
        {doorClose();}                    //if door state is closed and no momentary state than door is closed
    }
    else
      {doorOpen();}                       //if door state is 'open', opens door
    
////////////////////////////////////////////// BUTTON LOOP /////////////////////////////////////////////////////////////    
    
    if(digitalRead(STOP) != HIGH)
      {safeOpenDelay = 300;}
    if(digitalRead(OPNBTN) != HIGH)     //checks open button
      {doorState = 1;}                    //sets door state to true if button is pressed
    if(digitalRead(CLSBTN) != HIGH)     //checks close button
      {doorState = 0;}                    //sets door state to false if button is pressed
    if(digitalRead(MOMBTN) != LOW)     //checks momentary button
    {
      if(momDeb > 0)
      {
        momDeb--;
        if(momDeb == 0)
        {
          momState = 1;
        }
      }
    }                     //sets momentary state to true if button is pressed
    else
      {
        momState = 0;
        momDeb = 100;
      }                     //sets momentary state to false if button not pressed    
    Serial.println(momDeb);
} 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void doorOpen()
{
  digitalWrite(DOORCLS, LOW);
  if(doorOCount == 0)
    {digitalWrite(DOOROPN, LOW);}
  else
    {digitalWrite(DOOROPN, HIGH);}
}

void doorClose()
{
  digitalWrite(DOOROPN, LOW);
  if(doorCCount == 0)
    {digitalWrite(DOORCLS, LOW);}
  else
  {
    if(safeOpenDelay != 0)   
    {
      doorCCount = 600;
      digitalWrite(DOOROPN, HIGH);
      digitalWrite(DOORCLS, LOW);
    }
    else
    {digitalWrite(DOORCLS, HIGH);}
  }
} 


/*
void doorClose()
{
  if(safeOpenDelay != 0)   
  {
    doorCCount = 600;
    digitalWrite(DOOROPN, HIGH);
    digitalWrite(DOORCLS, LOW);    
  }
  else
  {
    digitalWrite(DOOROPN, LOW);
    if(doorCCount == 0)
      {digitalWrite(DOORCLS, LOW);}
    else
      {digitalWrite(DOORCLS, HIGH);}
  } 
}
*/
