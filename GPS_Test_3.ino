#include <SoftwareSerial.h>
#define NMEA_SIZE 1024
 
SoftwareSerial GPS = SoftwareSerial(6, 1);
 
byte NMEA[NMEA_SIZE];

//new code
String cur_GPS;
//

void getData() {
 
  byte character;
  int index = 0;
   
  do {
    if (GPS.available()) {
      character = GPS.read();      
      NMEA[index] = character;
      index++;
    }
  } while(index < NMEA_SIZE && character != '$');
   
  NMEA[index - 2] = '\0';
}
 
void setup() {
  GPS.begin(4800);
  Serial.begin(9600);
}
  
void loop() {
    getData();
   
    //if(NMEA[2] == 'R' && NMEA[3] == 'M' && NMEA[4] == 'C') {
      int i = 0;
      for(i = 0; NMEA[i] != '\0'; i++) {
        //Serial.write(NMEA[i]);
      }
      //Serial.print("<END>\n");
      cur_GPS = String((char *)NMEA);
      cur_GPS = cur_GPS + "<END>\n";
      Serial.print(cur_GPS);
    //}
    
}

/*
#include <SoftwareSerial.h>
#include "./TinyGPS.h" 
 
long lat,lon; // create variable for latitude and longitude object
 
SoftwareSerial gpsSerial(2, 3); // create gps sensor connection
TinyGPS gps; // create gps object
 
void setup(){

  Serial.begin(9600); // connect serial
    Serial.println("Setup");
  gpsSerial.begin(9600); // connect gps sensor
}
 
void loop(){
  while(gpsSerial.available()){ // check for gps data
    
   if(gps.encode(gpsSerial.read())){ // encode gps data

    gps.get_position(&lat,&lon); // get latitude and longitude
    // display position
    Serial.print("Position: ");
    Serial.print("lat: ");Serial.print(lat);Serial.print(" ");// print latitude
    Serial.print("lon: ");Serial.println(lon); // print longitude
   }
  }
}
*/
