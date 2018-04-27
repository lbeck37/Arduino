//BeckE32_BLE_ServerLib.h, Apr 26,2018-a
/*
    Video: https://www.youtube.com/watch?v=oCMOYS71NIU
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
    Ported to Arduino ESP32 by Evandro Copercini

   Create a BLE server that, once we receive a connection, will send periodic notifications.
   The service advertises itself as: 6E400001-B5A3-F393-E0A9-E50E24DCCA9E
   Has a characteristic of: 6E400002-B5A3-F393-E0A9-E50E24DCCA9E - used for receiving data with "WRITE"
   Has a characteristic of: 6E400003-B5A3-F393-E0A9-E50E24DCCA9E - used to send data with  "NOTIFY"

   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create a BLE Service
   3. Create a BLE Characteristic on the Service
   4. Create a BLE Descriptor on the characteristic
   5. Start the service.
   6. Start advertising.

   In this example rxValue is the data received (only accessible inside that function).
   And txValue is the data to be sent, in this example just a byte incremented every second.
*/
#ifndef BLE_SERVERLIB_H
#define BLE_SERVERLIB_H

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <soc/rtc.h>
#include <Streaming.h>

BLECharacteristic *pNotifyCharact;
bool deviceConnected = false;
//float txValue = 0;
double dNotifyValue = 0;
const int readPin   = 32; // Use GPIO number. See ESP32 board pinouts
const int LED       = 5; // Could be different depending on the dev board. I used the DOIT ESP32 dev board.

//std::string rxValue; // Could also make this a global var to access it in loop()

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
//#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
//#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"
#define SERVICE_UUID          "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define NOTIFY_CHARACT_UUID   "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"


class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  };  //onConnect

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  } //onDisconnect
};  //MyServerCallbacks


/*
class MyCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string szRxValue = pCharacteristic->getValue();

    if (szRxValue.length() > 0) {
      Serial.println("*********");
      Serial.print("Received Value: ");
      for (int i = 0; i < szRxValue.length(); i++) {
        Serial.print(szRxValue[i]);
      } //for
      Serial.println();
      // Do stuff based on the command received from the app
      if (rxValue.find("A") != -1) {
        Serial.print("Turning ON!");
        digitalWrite(LED, HIGH);
      } //if(szRxValue.find("A")!=-1)
      else if (szRxValue.find("B") != -1) {
        Serial.print("Turning OFF!");
        digitalWrite(LED, LOW);
      } //else if(szRxValue.find("B")!=-1)
      Serial.println("\n*********");
    } //if(szRxValue.length()>0)
  } //onWrite
};  //MyCallbacks
*/
/*
  //This code would go in setup()
  //Create Characteristic for Write. This is local to setup().
  BLECharacteristic *pWriteCharact=
    pService->createCharacteristic(CHARACTERISTIC_UUID_RX,
                                   BLECharacteristic::PROPERTY_WRITE);
  //Set Callbacks for Characteristic for Write
  pWriteCharact->setCallbacks(new MyCallbacks());
*/
#endif		//BLE_SERVERLIB_H
