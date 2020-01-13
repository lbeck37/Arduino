const char szSketchName[]  = "Beck_FirebaseStub.ino";
const char szFileDate[]    = "1/4/20j";

#ifndef ESP8266
  #define ESP8266
#endif

#define DO_ACCESS_POINT       false

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <BeckLogLib.h>
#include <Streaming.h>

#if DO_ACCESS_POINT
  #include <BeckAccessPointLib_Simple.h>
  const char*   _acAccessPointSSID    = "StubSpot";
  const char*   _acAccessPointPW      = "Qazqaz11";
#endif

//const long      lSerialMonitorBaud  = 115200;

const char*     WiFi_SSID           = "Aspot24";
const char*     WiFi_Password       = "Qazqaz11";

#define 				FIREBASE_HOST 			"https://test-70884.firebaseio.com"
#define 				FIREBASE_AUTH 			"AIzaSyD-Nm1dYBV6ehphAOQgkM5sz4oYLKF9ahg"

//#define FIREBASE_HOST   "https://thermo-2b830.firebaseio.com/"
//#define FIREBASE_AUTH   "AIzaSyAkFumb-wjDUQ9HQjTOoHeXqTKztFSqf6o"

bool        _bWiFiConnected;

void setup(){
  Serial.begin(115200);
  delay(100);
  Serial << endl  << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;

  Serial << "Setup(): Connecting to WiFi" << endl;
  WiFi.begin(WiFi_SSID, WiFi_Password);

  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  } //while
  Serial << endl;

  if (WiFi.status() == WL_CONNECTED) {
    _bWiFiConnected= true;
    Serial << "setup(): Connected with IP: " << WiFi.localIP() << endl;
  }
  else {
    _bWiFiConnected= false;
    Serial << "setup(): WiFi did not connect" << endl;
  }

  if (_bWiFiConnected) {
	  Serial << "setup(): Call Firebase.begin()" << endl;
	  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  }	//if(_bWiFiConnected)

#if DO_ACCESS_POINT
  SetupAccessPt(_acAccessPointSSID, _acAccessPointPW);
#endif  //DO_ACCESS_POINT

  Serial << "setup(): Done" << endl;
  return;
} //setup


/*
void loop(){
#if DO_ACCESS_POINT
  if (_bWiFiConnected){
    HandleSoftAPClient();       //Listen for HTTP requests from clients
  } //if(_bWiFiConnected)
  delay(250);
#else
  Serial << "loop(): Do nothing" << endl;
  delay(5000);
#endif  //DO_ACCESS_POINT
  return;
} //loop
*/


void loop() {
  if (WiFi.status() != WL_CONNECTED) {
	  Serial << LOG0 << "loop(): Do nothing because (WiFi.status() != WL_CONNECTED)" << endl;
	  delay(5000);
	  return;
  }	//if(WiFi.status()!=WL_CONNECTED)

  // get value
  Serial << LOG0 << "loop(): Read a value from the database" << endl;
  Serial << LOG0 << "loop(): Call Firebase.getFloat(\"Setpoint\")" << endl;
  //Serial.println(Firebase.getFloat("Setpoint"));
  float fValue= Firebase.getFloat("Setpoint");
  Serial << LOG0 << "loop(): Call Firebase.getFloat() returned fValue= " << fValue << endl;
  if (Firebase.failed()) {
		Serial << LOG0 << "loop(): Reading /Setpoint failed, error= |" << Firebase.error() << "|" << endl;
		Serial << "loop(): Firebase.error()= |" << Firebase.error() << "|" << endl;
  }
  Serial << LOG0 << "loop(): Delay 5 seconds" << endl;
  delay(5000);

  // set value
  //Serial << "loop(): Set a value in the database" << endl;
  //Serial << "loop(): Call Firebase.setFloat(\"Setpoint\", 42.0)" << endl;
  Serial << LOG0 << "loop(): Set a value into the database" << endl;
  Serial << LOG0 << "loop(): Call Firebase.setFloat(\"Setpoint\", 42.0)" << endl;
  Firebase.setFloat("Setpoint", 42.0);
  // handle error
  if (Firebase.failed()) {
		Serial << LOG0 << "loop(): Setting /Setpoint failed, error= |" << Firebase.error() << "|" << endl;
		Serial << "loop(): Firebase.error()= |" << Firebase.error() << "|" << endl;
  }
  Serial << LOG0 << "loop(): Delay 5 seconds" << endl;
  delay(5000);

/*
  // update value
  Firebase.setFloat("Setpoint", 43.0);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /Setpoint failed:");
      Serial.println(Firebase.error());
      return;
  }
  delay(1000);

  // remove value
  Firebase.remove("Setpoint");
  delay(1000);

  // set string value
  Firebase.setString("message", "hello world");
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /message failed:");
      Serial.println(Firebase.error());
      return;
  }
  delay(1000);

  // set bool value
  Firebase.setBool("truth", false);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /truth failed:");
      Serial.println(Firebase.error());
      return;
  }
  delay(1000);

  static int wLogCount = 0;

  // append a new value to /logs
  String name = Firebase.pushInt("logs", wLogCount++);
  // handle error
  if (Firebase.failed()) {
      Serial.print("pushing /logs failed:");
      Serial.println(Firebase.error());
      return;
  }
  Serial.print("pushed: /logs/");
  Serial.println(name);
  delay(1000);
*/
  return;
}	//loop
//Last line.