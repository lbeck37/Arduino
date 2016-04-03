//
// Copyright 2015 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// FirebasePush_ESP8266 is a sample that push a new timestamp to firebase
// on each reset.

#include <Streaming.h>
#include <Firebase.h>

static const char szSketchName[]  = "FirebasePush_ESP8266.ino";
static const char szFileDate[]    = "Apr 3, 2016A";

#define LOG0    	szLogLineHeader(++lLineCount)
static long         lLineCount            = 0;      //Serial Monitor uses for clarity.
static const long   lSerialMonitorBaud    = 115200;
static const long   lMsecPerDay           = 86400000;
static const long   lMsecPerHour          =  3600000;
static const long   lMsecPerMin           =    60000;
static const long   lMsecPerSec           =     1000;

static const char   acRouterName[]        = "Aspot24";
static const char   acRouterPW[]          = "Qazqaz11";
//static const char   acRouterName[]        = "TrailheadBoise";
//static const char   acRouterPW[]          = "Trailhead2015";
static const char   acHostname[]          = "esp39";

// create firebase client.
Firebase fbase = Firebase("example.firebaseio.com")
                   .auth("secret_or_token");

void setup() {
  //Serial.begin(9600);
  Serial.begin(lSerialMonitorBaud);
  Serial << endl << LOG0 << " setup(): Initialized serial to " << lSerialMonitorBaud << " baud" << endl;
  Serial << LOG0 << " setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;

  // connect to wifi.
  //WiFi.begin("SSID", "PASSWORD");
  Serial << LOG0 << " setup(): Call WiFi.begin(" << acRouterName << ", " << acRouterPW << ")" << endl;
  WiFi.begin(acRouterName, acRouterPW);

  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

   // add a new entry.
  FirebasePush push = fbase.push("/logs", "{\".sv\": \"timestamp\"}");
  if (push.error()) {
      Serial.println("Firebase push failed");
      Serial.println(push.error().message());
      return;
  }

  // print key.
  Serial.println(push.name());

  // get all entries.
  FirebaseGet get = fbase.get("/logs");
  if (get.error()) {
      Serial.println("Firebase get failed");
      Serial.println(push.error().message());
      return;
  }
  // print json.
  Serial.println(get.json());
}	//setup

void loop() {
}	//loop


String szLogLineHeader(long lLineCount){
  String szHeader= "";
  szHeader += lLineCount;
  szHeader += " ";
  //szTermString += szTime;
  szHeader += szGetTime(millis());
  //szHeader += " ";
  return szHeader;
} //szLogLineHeader


String szGetTime(long lMsec){
  String  szString;
  int sDays    =    lMsec                                               / lMsecPerDay ;
  int sHours   =   (lMsec % lMsecPerDay)                                / lMsecPerHour;
  int sMinutes =  ((lMsec % lMsecPerDay) % lMsecPerHour)                / lMsecPerMin ;
  int sSeconds = (((lMsec % lMsecPerDay) % lMsecPerHour) % lMsecPerMin) / lMsecPerSec;
  int sMsec    =    lMsec % lMsecPerSec;
  szString = String(sDays) + ":";
  szString+= String(szAddZeros(sHours, 2)) + ":";
  szString+= String(szAddZeros(sMinutes, 2)) + ":";
  szString+= String(szAddZeros(sSeconds, 2)) + ".";
  szString+= String(szAddZeros(sMsec, 3)) + " ";  //Trailing blank.
  return szString;
} //szGetTime


//szAddLeadingZeros() adds 1 or 2 zeros (depending on sNumDigits being 3 or not).
String szAddZeros(int sValue, int sNumDigits){
  String szReturn;
  if ((sNumDigits == 3) && (sValue < 100)){
    szReturn= "0";
  } //if((sNumDigits==3)&&(sValue<100)
  if (sValue < 10){
    szReturn += "0";
  } //if(lValue<10)
  szReturn += String(sValue);
  return szReturn;
} //szAddZeros
//Last line.
