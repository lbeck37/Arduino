//BeckE8266WiFiLib.h, 1/12/19
#pragma once
#include <ESP8266WebServer.h>

extern ESP8266WebServer     *_pConfigWiFiServer;
extern IPAddress             _oStationIPAddress;
extern IPAddress             _oAccessPtIPAddress;

//IPAddress SetupWiFi             (char szRouterName[], char szRouterPW[]);
void      SetupWiFi(const char szRouterName[], const char szRouterPW[]);
IPAddress SetupAccessPoint      ();
void      SetupWebServer        (IPAddress oIPAddress);
void      SetupmDNS             (IPAddress oIPAddress, char* szName);
//Last line.
