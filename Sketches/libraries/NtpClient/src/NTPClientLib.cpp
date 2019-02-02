/* Beck 2/1/19, NTPClientLib.cpp in NtpClient library, not NtpClientLib library
Copyright 2016 German Martin (gmag11@gmail.com). All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are
permitted provided that the following conditions are met :

1. Redistributions of source code must retain the above copyright notice, this list of
conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list
of conditions and the following disclaimer in the documentation and / or other materials
provided with the distribution.

THIS SOFTWARE IS PROVIDED BY <COPYRIGHT HOLDER> ''AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.IN NO EVENT SHALL <COPYRIGHT HOLDER> OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT(INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "NtpClientLib.h"
#include "BeckMiniLib.h"      //Beck 2/1/19 for logging

#define DBG_PORT Serial

#ifdef DEBUG_NTPCLIENT
#define DEBUGLOG(...) DBG_PORT.printf(__VA_ARGS__)
#else
#define DEBUGLOG(...)
#endif


NTPClient::NTPClient() {
}   //Constructor


bool NTPClient::setNtpServerName(String ntpServerName){
  char* name = (char *)malloc ((ntpServerName.length () + 1) * sizeof (char));
  if (!name)
      return false;
  ntpServerName.toCharArray (name, ntpServerName.length () + 1);
  DEBUGLOG ("NTP server set to %s\n", name);
  free (_ntpServerName);
  _ntpServerName = name;
  Serial << "NTPClient::setNtpServerName(String): _ntpServerName set to " << _ntpServerName << endl;
  return true;
} //setNtpServerName(String)


bool NTPClient::setNtpServerName(char* ntpServerName){
  char *name = ntpServerName;
  Serial << "NTPClient::setNtpServerName(char*): Setting Server name to " << name << endl;
  if (name == NULL)
      return false;
  DEBUGLOG ("NTP server set to %s\n", name);
  free (_ntpServerName);
  _ntpServerName = name;
  return true;
} //setNtpServerName(char*)


String NTPClient::getNtpServerName() {
    return String (_ntpServerName);
}


char* NTPClient::getNtpServerNamePtr() {
    return _ntpServerName;
}


bool NTPClient::setTimeZone(int8_t timeZone, int8_t minutes) {
  Serial << "NTPClient::setTimeZone(): Setting Timezone= " << timeZone << ", minutes= " << minutes << endl;
  if ((timeZone >= -12) && (timeZone <= 14) && (minutes >= -59) && (minutes <= 59)) {
    // Temporarily set time to new time zone, before trying to synchronize
    int8_t timeDiff = timeZone - _timeZone;
    _timeZone = timeZone;
    _minutesOffset = minutes;
    setTime (now () + timeDiff * SECS_PER_HOUR + minutes * SECS_PER_MIN);
    if (udp && (timeStatus () != timeNotSet)) {
        setTime (getTime ());
    }
    DEBUGLOG ("NTP time zone set to: %d\r\n", timeZone);
    return true;
  }
  return false;
} //setTimeZone


boolean sendNTPpacket(const char* address, UDP *udp) {
  Serial << "NTPClient::sendNTPpacket(char*): Sending to " << address << endl;
  uint8_t     ntpPacketBuffer[NTP_PACKET_SIZE];       //Buffer to store request message

  memset(ntpPacketBuffer, 0, NTP_PACKET_SIZE);   // set all bytes in the buffer to 0
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  ntpPacketBuffer[0] = 0b11100011;   // LI, Version, Mode
  ntpPacketBuffer[1] = 0;     // Stratum, or type of clock
  ntpPacketBuffer[2] = 6;     // Polling Interval
  ntpPacketBuffer[3] = 0xEC;  // Peer Clock Precision
                              // 8 bytes of zero for Root Delay & Root Dispersion
  ntpPacketBuffer[12] = 49;
  ntpPacketBuffer[13] = 0x4E;
  ntpPacketBuffer[14] = 49;
  ntpPacketBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  //Serial << "NTPClient::sendNTPpacket(): Call udp->beginPacket()" << endl;
    udp->beginPacket (address, DEFAULT_NTP_PORT); //NTP requests are to port 123
  //Serial << "NTPClient::sendNTPpacket(): Call udp->write()" << endl;
    udp->write (ntpPacketBuffer, NTP_PACKET_SIZE);
  //Serial << "NTPClient::sendNTPpacket(): Call udp->endPacket()" << endl;
    udp->endPacket ();
  //Serial << "NTPClient::sendNTPpacket(): Done" << endl;
  return true;
} //sendNTPpacket


time_t NTPClient::getTime(){
  //Serial << "NTPClient::getTime(): Begin " << endl;
  //DNSClient dns;
  //WiFiUDP *udpClient = new WiFiUDP(*udp);
  IPAddress timeServerIP; //NTP server IP address
  char ntpPacketBuffer[NTP_PACKET_SIZE]; //Buffer to store response message

  DEBUGLOG ("Starting UDP\n");
  udp->begin (DEFAULT_NTP_PORT);
  //DEBUGLOG ("UDP port: %d\n",udp->localPort());
  while (udp->parsePacket () > 0); // discard any previously received packets

  /*dns.begin(WiFi.dnsServerIP());
  uint8_t dnsResult = dns.getHostByName(NTP.getNtpServerName().c_str(), timeServerIP);
  DEBUGLOG(F("NTP Server hostname: "));
  DEBUGLOGCR(NTP.getNtpServerName());
  DEBUGLOG(F("NTP Server IP address: "));
  DEBUGLOGCR(timeServerIP);
  DEBUGLOG(F("Result code: "));
  DEBUGLOG(dnsResult);
  DEBUGLOG(" ");
  DEBUGLOGCR(F("-- IP Connected. Waiting for sync"));
  DEBUGLOGCR(F("-- Transmit NTP Request"));*/

  //if (dnsResult == 1) { //If DNS lookup resulted ok
  Serial << "NTPClient::getTime(): Call sendNTPpacket()" << endl;
  sendNTPpacket (getNtpServerName ().c_str (), udp);
  uint32_t beginWait = millis ();
  while((millis() - beginWait) < NTP_TIMEOUT) {
    int size = udp->parsePacket ();
    if (size >= NTP_PACKET_SIZE) {
      DEBUGLOG ("-- Receive NTP Response\n");
      udp->read (ntpPacketBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      time_t timeValue = decodeNtpMessage (ntpPacketBuffer);
      setSyncInterval (getLongInterval ());
      if (!_firstSync) {
          //    if (timeStatus () == timeSet)
          _firstSync = timeValue;
      }
      //getFirstSync (); // Set firstSync value if not set before
      DEBUGLOG ("Sync frequency set low\n");
      udp->stop ();
      setLastNTPSync (timeValue);
      DEBUGLOG ("Successful NTP sync at %s", getTimeDateString(getLastNTPSync()).c_str());
      //Serial << "NTPClient::getTime(): Successful NTP sync at " << getTimeDateString(getLastNTPSync()) << endl;
      if (onSyncEvent){
        onSyncEvent(timeSyncd);
      }
      //Serial << "NTPClient::getTime(): Returning " << timeValue << endl;
      return timeValue;
    } //if (size>=NTP_PACKET_SIZE)
#ifdef ARDUINO_ARCH_ESP8266
    ESP.wdtFeed ();
#endif
  } //while
  DEBUGLOG ("-- No NTP Response :-(\n");
  Serial << "NTPClient::getTime(): No NTP Response, calling udp->stop()" << endl;
  udp->stop();
  setSyncInterval (getShortInterval ()); // Retry connection more often
  if (onSyncEvent){
    onSyncEvent (noResponse);
  }
  Serial << "NTPClient::getTime(): Unable to get time, returning 0" << endl;
  return 0; // return 0 if unable to get the time
} //getTime


int8_t NTPClient::getTimeZone () {
    return _timeZone;
}


int8_t NTPClient::getTimeZoneMinutes () {
    return _minutesOffset;
}

/*void NTPClient::setLastNTPSync(time_t moment) {
    _lastSyncd = moment;
}*/

time_t NTPClient::s_getTime(){
  Serial << "NTPClient::s_getTime() SyncProvider: Call NTP.getTime()" << endl;
  //return NTP.getTime ();
  time_t lNtpTime= NTP.getTime();
  //Serial << "NTPClient::s_getTime(): Returning time= " << lNtpTime << endl;
  return lNtpTime;
} //s_getTime

/*
#if NETWORK_TYPE == NETWORK_W5100
bool NTPClient::begin (String ntpServerName, int8_t timeZone, bool daylight, int8_t minutes, EthernetUDP* udp_conn) {
#elif NETWORK_TYPE == NETWORK_ESP8266 || NETWORK_TYPE == NETWORK_WIFI101 || NETWORK_TYPE == NETWORK_ESP32
bool NTPClient::begin (String ntpServerName, int8_t timeZone, bool daylight, int8_t minutes, WiFiUDP* udp_conn) {
#endif
*/
#if NETWORK_TYPE == NETWORK_W5100
  bool NTPClient::begin (String ntpServerName, int8_t timeZone, bool daylight, int8_t minutes, EthernetUDP* udp_conn) {
#elif ((NETWORK_TYPE == NETWORK_ESP8266) || (NETWORK_TYPE == NETWORK_WIFI101) || (NETWORK_TYPE == NETWORK_ESP32))
  bool NTPClient::begin (String ntpServerName, int8_t timeZone, bool daylight, int8_t minutes, WiFiUDP* udp_conn) {
#endif
  Serial << "NTPClient::begin(): Servername= " << ntpServerName << ", timeZone= " << timeZone
          << ", daylight= " << daylight << ", minutes= " << minutes << endl;
  Serial << "NTPClient::begin(): Call setNtpServerName()" << endl;
  if (!setNtpServerName(ntpServerName)) {
    DEBUGLOG ("Time sync not started\r\n");
    Serial << "NTPClient::begin(): setNtpServerName() returned false, Time sync not started" << endl;
    return false;
  }
  Serial << "NTPClient::begin(): Call setTimeZone()" << endl;
  if (!setTimeZone (timeZone, minutes)) {
    DEBUGLOG ("Time sync not started\r\n");
    Serial << "NTPClient::begin(): setTimeZone() returned false, Time sync not started" << endl;
    return false;
  }
  if (udp_conn)
      udp = udp_conn;
  else
#if NETWORK_TYPE == NETWORK_W5100
      udp = new EthernetUDP ();
#else
  Serial << "NTPClient::begin(): Construct udp= new WiFiUDP()" << endl;
  udp= new WiFiUDP();
#endif
  //_timeZone = timeZone;
  setDayLight (daylight);
  _lastSyncd = 0;

  Serial << "NTPClient::begin(): Call setInterval()" << endl;
  if (!setInterval(DEFAULT_NTP_SHORTINTERVAL, DEFAULT_NTP_INTERVAL)) {
    DEBUGLOG ("Time sync not started\r\n");
    Serial << "NTPClient::begin(): setInterval() returned false, Time sync not started" << endl;
    return false;
  }
  DEBUGLOG ("Time sync started\r\n");

  Serial << "NTPClient::begin(): Call setSyncInterval()" << endl;
    setSyncInterval(getShortInterval());    //Line 318 in Time.cpp
  Serial << "NTPClient::begin(): Call setSyncProvider()" << endl;
    setSyncProvider(s_getTime);             //Line 312 in Time.cpp, calls now()
  Serial << "NTPClient::begin(): Done" << endl;
  return true;
} //setup


bool NTPClient::stop(){
    setSyncProvider (NULL);
    DEBUGLOG ("Time sync disabled\n");
    Serial << "NTPClient::stop(): Done" << endl;
    return true;
} //stop


bool NTPClient::setInterval(int interval){
  Serial << "NTPClient::setInterval(int): Set interval to " << interval << endl;
  if(interval >= 10){
    if(_longInterval != interval){
      _longInterval = interval;
      DEBUGLOG ("Sync interval set to %d\n", interval);
      if(timeStatus () == timeSet){
        setSyncInterval (interval);
      } //if(timeStatus () == timeSet)
    } //if(_longInterval!=interval)
    return true;
  }
  else{
    Serial << "NTPClient::setInterval(int): interval < 10, return false" << endl;
    return false;
  } //if(interval>=10)
} //setInterval(int)


bool NTPClient::setInterval (int shortInterval, int longInterval) {
  Serial << "NTPClient::setInterval(int,int): Set shortInterval to " << shortInterval
        << ", longInterval to " << longInterval << endl;
  if (shortInterval >= 10 && longInterval >= 10) {
    _shortInterval = shortInterval;
    _longInterval = longInterval;
    if (timeStatus () != timeSet) {
        setSyncInterval (shortInterval);
    } else {
        setSyncInterval (longInterval);
    }
    DEBUGLOG ("Short sync interval set to %d\n", shortInterval);
    DEBUGLOG ("Long sync interval set to %d\n", longInterval);
    return true;
  }
  else{
    Serial << "NTPClient::setInterval(int,int): intervals < 10, return false" << endl;
    return false;
  }
} //setInterval(int,int)


int NTPClient::getInterval () {
    return _longInterval;
}


int NTPClient::getShortInterval () {
    return _shortInterval;
}


void NTPClient::setDayLight (bool daylight) {
    _daylight = daylight;
    DEBUGLOG ("--Set daylight saving %s\n", daylight ? "ON" : "OFF");
    setTime (getTime ());
}

bool NTPClient::getDayLight () {
    return _daylight;
}

String NTPClient::getTimeStr (time_t moment) {
    char timeStr[10];
    sprintf (timeStr, "%02d:%02d:%02d", hour (moment), minute (moment), second (moment));

    return timeStr;
}


String NTPClient::getDateStr (time_t moment) {
    char dateStr[12];
    sprintf (dateStr, "%02d/%02d/%4d", day (moment), month (moment), year (moment));

    return dateStr;
}


String NTPClient::getTimeDateString (time_t moment) {
    return getTimeStr (moment) + " " + getDateStr (moment);
}


time_t NTPClient::getLastNTPSync () {
    return _lastSyncd;
}


void NTPClient::onNTPSyncEvent (onSyncEvent_t handler) {
    onSyncEvent = handler;
}


time_t NTPClient::getUptime () {
    _uptime = _uptime + (millis () - _uptime);
    return _uptime / 1000;
}


String NTPClient::getUptimeString () {
    uint16_t days;
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;

    time_t uptime = getUptime();

    seconds = uptime % SECS_PER_MIN;
    uptime -= seconds;
    minutes = (uptime % SECS_PER_HOUR) / SECS_PER_MIN;
    uptime -= minutes * SECS_PER_MIN;
    hours = (uptime % SECS_PER_DAY) / SECS_PER_HOUR;
    uptime -= hours * SECS_PER_HOUR;
    days = uptime / SECS_PER_DAY;

    char uptimeStr[20];
    sprintf (uptimeStr, "%4u days %02d:%02d:%02d", days, hours, minutes, seconds);

    return uptimeStr;
}


time_t NTPClient::getLastBootTime () {
    if (timeStatus () == timeSet) {
        return (now () - getUptime ());
    }
    return 0;
}


time_t NTPClient::getFirstSync () {
    /*if (!_firstSync) {
        if (timeStatus () == timeSet) {
            _firstSync = now () - getUptime ();
        }
    }*/
    return _firstSync;
}


bool NTPClient::summertime (int year, byte month, byte day, byte hour, byte tzHours)
// input parameters: "normal time" for year, month, day, hour and tzHours (0=UTC, 1=MEZ)
{
    if ((month < 3) || (month > 10)) return false; // keine Sommerzeit in Jan, Feb, Nov, Dez
    if ((month > 3) && (month < 10)) return true; // Sommerzeit in Apr, Mai, Jun, Jul, Aug, Sep
    if ((month == 3 && (hour + 24 * day) >= (1 + tzHours + 24 * (31 - (5 * year / 4 + 4) % 7))) || (month == 10 && (hour + 24 * day) < (1 + tzHours + 24 * (31 - (5 * year / 4 + 1) % 7))))
        return true;
    else
        return false;
}

boolean NTPClient::isSummerTimePeriod (time_t moment) {
    return summertime (year (), month (), day (), hour (), getTimeZone ());
}


void NTPClient::setLastNTPSync (time_t moment) {
    _lastSyncd = moment;
}


time_t NTPClient::decodeNtpMessage (char *messageBuffer) {
    unsigned long secsSince1900;
    // convert four bytes starting at location 40 to a long integer
    secsSince1900 = (unsigned long)messageBuffer[40] << 24;
    secsSince1900 |= (unsigned long)messageBuffer[41] << 16;
    secsSince1900 |= (unsigned long)messageBuffer[42] << 8;
    secsSince1900 |= (unsigned long)messageBuffer[43];

#define SEVENTY_YEARS 2208988800UL
    time_t timeTemp = secsSince1900 - SEVENTY_YEARS + _timeZone * SECS_PER_HOUR + _minutesOffset * SECS_PER_MIN;

    if (_daylight) {
        if (summertime (year (timeTemp), month (timeTemp), day (timeTemp), hour (timeTemp), _timeZone)) {
            timeTemp += SECS_PER_HOUR;
            DEBUGLOG ("Summer Time\n");
        } else {
            DEBUGLOG ("Winter Time\n");
        }
    } else {
        DEBUGLOG ("No daylight\n");
    }
    return timeTemp;
}

NTPClient NTP;
//Last line.
