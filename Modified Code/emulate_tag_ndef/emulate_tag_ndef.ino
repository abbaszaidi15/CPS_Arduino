
#include "SPI.h"
#include "PN532_SPI.h"
#include "emulatetag.h"
#include "NdefMessage.h"

#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#define START 1
#define STOP 0

PN532_SPI pn532spi(SPI, 10);
EmulateTag nfc(pn532spi);

uint8_t ndefBuf[120];
NdefMessage message;
int messageSize;
int mode;
bool writeSuccessful;

static const int RXPin = 6, TXPin = 1;
static const uint32_t GPSBaud = 4800;
// The TinyGPS++ object
TinyGPSPlus gps;
// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

boolean gpslocked = false;
String gpslocation= "";
String gpslat = "";
String gpslong = "";
String gpsmonth = "";
String gpsday = "";
String gpsyear = "";
String gpshour = "";
String gpsminute = "";
String gpssecond = "";


uint8_t uid[3] = { 0xAB, 0xCD, 0xEF };

//"start parking" and "stop parking" in HEX
byte start_parking[13] = {0x73, 0x74, 0x61, 0x72, 0x74, 0x20, 0x70, 0x61, 0x72, 0x6b, 0x69, 0x6e, 0x67};
byte stop_parking[12] = {0x73, 0x74, 0x6f, 0x70, 0x20, 0x70, 0x61, 0x72, 0x6b, 0x69, 0x6e, 0x67};
String cur_GPS;

void setup()
{
  Serial.begin(9600);
  ss.begin(GPSBaud);
  Serial.println("------- Emulate Tag --------");
  message = NdefMessage();
  message.addTextRecord("Please download the CPS App from www.CPSApp.com");
  messageSize = message.getEncodedSize();
  if (messageSize > sizeof(ndefBuf)) {
      Serial.println("ndefBuf is too small");
      while (1) { }
  }
  
  //Serial.print("Ndef encoded message size: ");
  //Serial.println(messageSize);
  
  

  message.encode(ndefBuf);
  
  // comment out this command for no ndef message
  nfc.setNdefFile(ndefBuf, messageSize);
  
  // uid must be 3 bytes!
  nfc.setUid(uid);
  
  nfc.init();
  
  mode = STOP;
  
  Serial.println("Initilization Complete");
   
}
void loop(){
  while (!gpslocked)
  {
    getGPSdata();
  }
  
//  writeSuccessful = false;
//   
//   NdefMessage new_message = NdefMessage();
//  new_message.addTextRecord(cur_GPS);
//  int new_messageSize = new_message.getEncodedSize();
//  if (new_messageSize > sizeof(ndefBuf)) {
//      Serial.println("ndefBuf is too small");
//      while (1) { }
//  }
//  
//  new_message.encode(ndefBuf); 
//    // uncomment for overriding ndef in case a write to this tag occured
//    nfc.setNdefFile(ndefBuf, new_messageSize); 
//    
//    // start emulation (blocks)
//    //nfc.emulate();
//    
//    
//    
//    // or start emulation with timeout
//    if(!nfc.emulate(3000)){ // timeout 1 second
//      Serial.println("timed out");
//    }
//    
//    // deny writing to the tag
//    // nfc.setTagWriteable(false);
//    
//    if(nfc.writeOccured()){
//       //Serial.println("\nWrite occured !");
//       
//       uint8_t* tag_buf;
//       uint16_t length;
//       
//       nfc.getContent(&tag_buf, &length);
//       NdefMessage msg = NdefMessage(tag_buf, length);
//       //msg.print();
//       
//       int recordCount = msg.getRecordCount();
//       if(recordCount > 1){
//          Serial.println("ERROR, MESSAGE SHOULD ONLY CONTAIN ONE RECORD"); 
//       }
//       //Serial.print(" Record Count: ");
//       //Serial.println(recordCount);
//          
//       NdefRecord receivedMessage = NdefRecord();
//       //Get record index from recordCount
//       int recordIndex = 0;
//       receivedMessage = msg.getRecord(recordIndex);
//       int receivedPayloadSize = receivedMessage.getPayloadLength();
//
//       
//      
//       byte receivedPayload[receivedPayloadSize];
//       receivedMessage.getPayload(receivedPayload);
//       byte payloadNoEnglish[receivedPayloadSize - 3];
//       for(int i = 0; i < receivedPayloadSize - 3; i++){
//          payloadNoEnglish[i] = receivedPayload[i+3];
//       }
//       
//       if(CompareByteArrays(start_parking, payloadNoEnglish, 13, 13)){
//          mode = START;
//          writeSuccessful = true;
//          
//       }
//       else if(CompareByteArrays(stop_parking, payloadNoEnglish, 12, 12)){
//          mode = STOP; 
//          writeSuccessful = true;
//       }
//
//              
//       //Serial.print(" Payload Size: ");
//       //Serial.println(receivedPayloadSize);
//      
//       //Serial.println(F("Rajan: Payload Data "));PrintHexChar(receivedPayload, receivedPayloadSize);
//       //Serial.println(F("Rajan: No English Payload Data "));PrintHexChar(payloadNoEnglish, receivedPayloadSize-3);
//       
//    }
//
//    if(nfc.writeOccured() == true){
//      if(mode == START){
//         Serial.println("Start Timer");
//      }
//      else if(mode == STOP){
//         Serial.println("Stop Timer"); 
//      }
//    }
//    
//    delay(1000);
}

bool CompareByteArrays(byte *array1, byte *array2, int size1, int size2){
  if(size1!=size2){
    //Serial.println("ERROR: Byte arrays are different sizes");
    return false;
  }
  else{ 
    for(int i = 0; i<size1; i++){
        if(array1[i] != array2[i]){
           //Serial.print("ERROR: Byte arrays do not match");
          return false;
        }
    }
    //Serial.println("Byte Arrays Match");
    return true;
  }  
   return false; 
}

void getGPSdata()
{
    if (ss.available() > 0)
    {
        if (gps.encode(ss.read())) 
        {
            displayInfo();
            gpslocked = true;
        }
    }
    gpslocked = false;
}

void displayInfo()
{
  gpslocation = "";
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(),6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(),6);
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
