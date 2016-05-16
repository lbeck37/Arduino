//BeckLib.h, May 15, 2016
#ifndef BECKLIB_H
#define BECKLIB_H
#include <Streaming.h>
#include <Firebase.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

#define LOG0    	  szLogLineHeader(++lLineCount)
#define BLog		    pBeckFBase->LogToBoth
#define BLogF		    pBeckFBase->LogToFirebase
#define BLogS		    LogJustToSerial

static const long   lSerialMonitorBaud    = 115200;
static const long   lMsecPerDay           = 86400000;
static const long   lMsecPerHour          =  3600000;
static const long   lMsecPerMin           =    60000;
static const long   lMsecPerSec           =     1000;

class BeckFirebase {
public:
	BeckFirebase(String sDatabaseURL,String sFirebaseSecret, String sPushPath, String sMyName);
/*
	BeckFirebase() {};
	~BeckFirebase() {};
*/
	void 		    LogToSerial(String sLogline);
	bool		    LogToFirebase(String sLogline);
	void		    LogToBoth(String sLogline);
	String		  GetDatabaseURL(void);
	String		  GetLogPath(void);
	String		  GetPushPath(void);

	bool		    bFirebaseOk_= true;

protected:
	String		  sDatabaseURL_;
	String		  sFirebaseSecret_;
	//Firebase 	oFBase_;
	Firebase* 	pFBase_;
	String		  sLogPath_;
	String 		  sMyName_;
	String		  sPushPath_;

	String		sMakeJSONObject(String sName, String sValue);
};	//BeckFirebase

//Non-class function protos.
BeckFirebase*	StartBeckFirebase(String sDatabaseURL, String sFirebaseSecret, String sLogPath, String sMyName);
bool 			TestFirebase(void);
void			SendInfoToLog(void);
void 			SetupWiFi(const char* pcRouterName, const char* pcRouterPW);
String    szWiFiStatus(wl_status_t status);
void 			SetupHttpServer(const char* acHostname,
                          ESP8266WebServer&        oHttpServer,
                          ESP8266HTTPUpdateServer& oHttpUpdateServer);
void 			HandleHttpServer(ESP8266WebServer& oHttpServer);
void 			LogJustToSerial(String sLogline);
void			LogESPValues();
String 		szLogLineHeader(long lLineCount);
String 		szGetTime(long lMsec);
String 		szAddZeros(int sValue, int sNumDigits);
String 		szIPaddress(IPAddress oIP);


//Global variables
extern long         			      lLineCount;      	//Serial Monitor uses for clarity.
extern int32_t					        wHttpServerCount;	//To allow logging every nth call at UPLOAD_FILE_WRITE
extern String					          szLogLine;
extern BeckFirebase*			      pBeckFBase;
extern bool                     bWiFiOn;
extern bool                     bFirebaseOn;
extern ESP8266WebServer			    oHttpServer;
extern ESP8266HTTPUpdateServer	oHttpUpdateServer;

#endif   //BECKLIB_H
