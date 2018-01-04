//BeckLogLib.h, 8/3/17
#ifndef BECKLOGLIB_H
#define BECKLOGLIB_H
#include <BeckMiniLib.h>
//#include <Streaming.h>
#include <Time.h>

#define LOG0        szLogLineHeader(++lLineCount)
//#define BLog        LogToSerial

/*
#define INT8        int8_t
#define UINT8       uint8_t
#define INT16       int16_t
#define UINT16      uint16_t
#define INT32       int32_t
#define UINT32      uint32_t
#define INT64       int64_t
#define UINT64      uint64_t
*/
#define LOG0        szLogLineHeader(++lLineCount)

/*
static const long   lMsecPerDay           = 86400000;
static const long   lMsecPerHour          =  3600000;
static const long   lMsecPerMin           =    60000;
static const long   lMsecPerSec           =     1000;
*/

extern bool           bDebugLog;    //Used to limit number of printouts.

//Function protos
void      LogJustToSerial(String sLogline);
String    szLogLineHeader(long lLineCount);
//String    szGetTime(long lMsec);
String    szAddZeros(int sValue, int sNumDigits);


//Global variables
extern long                     lLineCount;       //Serial Monitor uses for clarity.
extern String                   szLogLine;

  extern void LogToSerial(String szLogString);
  extern void LogToSerial(String szLogString, String szLogValue);
  extern void LogToSerial(String szLogString, int sLogValue);
/*
  extern void LogToSerial(String szLogString, int sValue1, int sValue2, int sValue3);
  extern void LogToSerial(String szLogString, UINT16 usLogValue);
*/
  extern void LogToSerial(String szLogString, UINT32 uwLogValue);
  extern void LogToSerial(String szLogString, float fLogValue);
  extern void LogToSerial(String szLogString, double dLogValue);

#endif   //BECKLOGLIB_H
