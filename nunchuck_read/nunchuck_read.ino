/*
   ArduinoNunchukDemo.ino

   Copyright 2011-2013 Gabriel Bianconi, http://www.gabrielbianconi.com/

   Project URL: http://www.gabrielbianconi.com/projects/arduinonunchuk/

*/

#include <Wire.h>
#include <ArduinoNunchuk.h>

#define BAUDRATE 115200

ArduinoNunchuk nunchuk = ArduinoNunchuk();


  
void setup()
{
  Serial.begin(BAUDRATE);
  nunchuk.init();

}

void loop()
{
  nunchuk.update();

  int analogX = nunchuk.analogX ;
  int analogY = nunchuk.analogY ;
  int AccX = nunchuk.accelX;

  bool buttonZ = nunchuk.zButton;
  bool buttonC = nunchuk.cButton;

  int mapTestX = map(analogX , 129 , 225 , 0 , 500);
  int mapTestY = map(analogY , 129 , 225 , 0 , 500);
  int AccXmap = map(AccX , 280 , 680 , - 500 , 500);

  if(mapTestX>500)
    mapTestX=500;
  if(mapTestX<-500)
    mapTestX=-500;

  if(mapTestY>500)
    mapTestY=500;
  if(mapTestY<-500)
    mapTestY=-500;

  if(AccXmap>500)
    AccXmap=500;
  if(AccXmap<-500)
    AccXmap=-500;    
    
  //int theta = sqrt((mapTestX^2) + (mapTestY^2));
  Serial.print("$");
  Serial.print(mapTestX);
  Serial.print(',');
  Serial.print(mapTestY);
  Serial.print(',');
  if(buttonC)
    Serial.print(AccXmap);
  else
    Serial.print("0");
  Serial.println();
  //displayData();

 // if (buttonC == true) {
   // Serial.println("its workss");
 // }

}


void displayData()
{
  int analogX = nunchuk.analogX ;
  int analogY = nunchuk.analogY ;

  int mapTest = map(analogX , 28 , 225 , -100 , 100);

  bool buttonZ = nunchuk.zButton;
  bool buttonC = nunchuk.cButton;

  

  Serial.print("X: ");
  Serial.print(analogX);
  Serial.print(' ');
  Serial.print("Y: ");
  Serial.print(analogY);
  Serial.print(' ');
  Serial.print("buttonZ: ");
  Serial.print(buttonZ);
  Serial.print(' ');
  Serial.print("buttonC: ");
  Serial.print(buttonC);
  Serial.print(' ');
  Serial.print("maptest:");
  Serial.println(mapTest);
  
}
