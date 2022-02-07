#include <Adafruit_NeoPixel.h>
int LEDPIN = 5;
int NUMPIXELS = 72;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);

#include <Wire.h>
#include <SparkFun_I2C_Mux_Arduino_Library.h> 
QWIICMUX myMux;
#define NUMBER_OF_SENSORS 8
#include "SparkFun_VL53L1X.h" 
SFEVL53L1X **distanceSensor; 

void upaliTraku(int pocetak, int kraj,int r, int g, int b)
{
  int i=pocetak;
  for (i; i < kraj; i++) {
     pixels.setPixelColor(i, pixels.Color(r,g,b));
  }
}

void provjera(String traka)
{
  pixels.clear();
  if(traka[0]=='1'){upaliTraku(0,9,255,0,0);}
  if(traka[1]=='1'){upaliTraku(9,18,0,255,0);}
  if(traka[2]=='1'){upaliTraku(18,27,255,0,255);}
  if(traka[3]=='1'){upaliTraku(27,36,0,0,255);}
  if(traka[4]=='1'){upaliTraku(36,45,204,0,0);}
  if(traka[5]=='1'){upaliTraku(45,54,255,255,0);}
  if(traka[6]=='1'){upaliTraku(54,63,0,255,153);}
  if(traka[7]=='1'){upaliTraku(63,72,255,51,153);}
  pixels.show();
}

void setup()
{
  Serial.begin(9600);
  pixels.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.show(); 
  Wire.begin();
  distanceSensor = new SFEVL53L1X *[NUMBER_OF_SENSORS];

  for (int x = 0; x < NUMBER_OF_SENSORS; x++)
    distanceSensor[x] = new SFEVL53L1X(Wire);

  if (myMux.begin() == false)
  {
    while (1)
      ;
  }
  byte currentPortNumber = myMux.getPort();
  bool initSuccess = true;

  for (byte x = 0; x < NUMBER_OF_SENSORS; x++)
  {
    myMux.setPort(x);
    if (distanceSensor[x]->begin() != 0) 
    {
      initSuccess = false;
    }
    else
    {
      // Sensor configuration
      distanceSensor[x]->setIntermeasurementPeriod(50);
      distanceSensor[x]->setDistanceModeLong();
      distanceSensor[x]->startRanging();
    }
  }

  if (initSuccess == false)
  {
    while (1);
  }
}

void loop()
{
  delay(29);
  int distance[NUMBER_OF_SENSORS];
  float distanceFeet;
  String traka = "";
  for (byte x = 0; x < NUMBER_OF_SENSORS; x++)
  {
    myMux.setPort(x);
    distance[x] = distanceSensor[x]->getDistance();
    if(distance[x]>=10 && distance[x]<1450){
      Serial.print('1');
      traka+='1';
    }
    else {
      Serial.print('0');
      traka+='0';
    }
  }
  provjera(traka);
  Serial.println();
  traka="";
  delay(40);
}
