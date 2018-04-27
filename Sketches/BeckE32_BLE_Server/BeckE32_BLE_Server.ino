const char szSketchName[]  = "BeckE32_BLE_Server.ino";
const char szFileDate[]    = "Apr 27, 2018-a";
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
#include <BeckE32_BLE_ServerLib.h>
/*
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <soc/rtc.h>
#include <Streaming.h>
*/

/*
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
*/


void setup() {
  Serial.begin(115200);
  Serial << endl << endl<< endl<< endl;
  Serial << "setup(): Begin " << szSketchName << ", " << szFileDate << endl;

  //Slow down CPU since HX711 needs to work with ESP32, verify BTE works
  Serial.println("setup(): Call rtc_clk_cpu_freq_set(RTC_CPU_FREQ_80M)");
  rtc_clk_cpu_freq_set(RTC_CPU_FREQ_80M);
  pinMode(LED, OUTPUT);

  //Create the  Device with a name
  //BLEDevice::init("BeckE32 BLE Server"); // Give it a name
  BLEDevice::init("RightPedal_BeckE32");

  // Create the Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a Characteristic for Notify
  pNotifyCharact = pService->createCharacteristic(NOTIFY_CHARACT_UUID,
                                                  BLECharacteristic::PROPERTY_NOTIFY);
  //Add Descriptor to Characteristic for Notify
  pNotifyCharact->addDescriptor(new BLE2902());
/*
  //Create Characteristic for Write. This is local to setup().
  BLECharacteristic *pWriteCharact=
    pService->createCharacteristic(CHARACTERISTIC_UUID_RX,
                                   BLECharacteristic::PROPERTY_WRITE);
  //Set Callbacks for Characteristic for Write
  pWriteCharact->setCallbacks(new MyCallbacks());
*/
  //Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting for a client connection to notify...");
} //setup


void loop() {
  //static int  sCount= 1;
  char szNotifyString[16]; // make sure this is big enough
  if (deviceConnected){
    dNotifyValue += 2.0;
    //Convert the value to a string:
    dtostrf(dNotifyValue, 1, 4, szNotifyString);  // float_val, min_width, digits_after_decimal, char_buffer
    //pCharacteristic->setValue(&txValue, 1); // To send the integer value
    //pCharacteristic->setValue("Hello!");    // Sending a test message
    pNotifyCharact->setValue(szNotifyString);
    pNotifyCharact->notify();                 //Send the value to the client
    Serial << "Server: loop(): Sent Notify Value= " << szNotifyString << endl;
  } //if (deviceConnected)
  //delay(1000);
  //delay(300);
  //delay(100);
  delay(10);    //32 samples/sec with 80 MHz cpu to Android app
  return;
} //Loop
//Last line.
