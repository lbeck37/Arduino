String acSketchName  = "BeckESP8266Base.ino";
String acFileDate    = "Sept 18, 2016_LBT_K";		//Was May 15, 2016_HP7N, June 19, 2016_LBT_D

//#define TEST_WIFI

#include <BeckLib.h>
/*
static const char   acRouterName[]        = "TrailheadBoise";
static const char   acRouterPW[]          = "Trailhead2015";
static const char           acRouterName[]      = "LBTspot";
static const char           acRouterName[]      = "Aspot24";
static const char           acRouterName[]      = "C1200spot";
static const char           acRouterPW[]        = "Qazqaz11";
*/
static const char           acRouterName[]      = "C1200spot";
static const char           acRouterPW[]        = "Qazqaz11";


static const char           	acMyURL[]           = "esp1101Dev";   //Beck, Dev type sensor, #1

void setup(void){
  Serial.begin(lSerialMonitorBaud);
  Serial << endl;
  BLogS("setup(): Initialized serial to " + String(lSerialMonitorBaud) + " baud");
  BLogS("Sketch: " + acSketchName + ", " + acFileDate);

  SetupWiFi(acRouterName, acRouterPW);

  //StartBeckFirebase(sDatabaseURL, sFirebaseSecret, sLogPath, acMyFbaseName);
  //SendInfoToLog();

  SetupHttpServer(acMyURL, oHttpServer, oHttpUpdateServer);

  BLog(acFileDate + ", " + acSketchName + ", setup() Done");
  return;
} //setup


void loop(void){
  HandleHttpServer(oHttpServer);
} //loop
//Last line.
