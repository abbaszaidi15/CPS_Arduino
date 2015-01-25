#include <TinyGPS++.h>
#include <SoftwareSerial.h>
/*
   This sketch demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 6(rx) and 1(tx).
*/
static const int RXPin = 6, TXPin = 1;
static const uint32_t GPSBaud = 4800;

unsigned long seconds = 2000L; // !!! SEE THE CAPITAL "L" USED!!!

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  Serial.begin(9600);
  ss.begin(GPSBaud);
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  if (ss.available() > 0){
    if (gps.encode(ss.read()))
      displayInfo();
  }
  //if (millis() > 5000 && gps.charsProcessed() < 10)
  //{
  //  Serial.println(F("No GPS detected: check wiring."));
  //}
}

void displayInfo()
{
  
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat());
    Serial.print(F(","));
    Serial.print(gps.location.lng());
    Serial.print(F(","));
  }
  else
  {
    Serial.print(F("INVALID"));
    Serial.print(F(","));
  }
  
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
    Serial.print(F(","));
  }
  else
  {
    Serial.print(F("INVALID"));
    Serial.print(F(","));
  }
  
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}
