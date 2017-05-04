//BeckControlLib.cpp
#include <BeckControlLib.h>
#include <OneWire.h>

const bool	 bUseFlowSwitch				 = false;
const int    sSwitchOpen           = 0;
const int    sSwitchClosed         = 1;
const int    sOff                  = 0;
const int    sOn                   = 1;
const int    sNotInit              = -3737;

const int    sNumSwitches_         = 4;
const long   sThermoTimesInRow     = 3;      //Max times temp is outside range before switch
const float  fMaxHeatRangeF        = 2.00;   //Temp above setpoint before heat is turned off

bool         abSwitchState_[]       = {false, false, false, false, false};
bool         abSwitchLastState_[]   = {false, false, false, false, false};
int          sSetpointF_            = 37;
int          sThermoTimesCount_     = 0;      //Number of times temperature out of range
bool         bThermoOn_             = true;   //Whether thermostat is running.
bool         bHeatOn_               = false;  //If switch is on to turn on furnace.
//float        fThermoOffDegF_        = sSetpointF_ + fMaxHeatRangeF;
float        fThermoOffDegF_;

bool          bFlowState_       			= false;
bool          bFlowLastState_   			= false;
bool					bNoFlow_								= false;
bool					bOverheatOn_						= false;

//const int    asSwitchPin[]         = {-1, 4, 5, 15, 16};      //0 is not a switch, switches are at 1,2,3,4
//const int    asSwitchPin[]         = {-1, 12, 13, 14, 15};    //15 is 8266 TXD0 and broke I2c ????
//const int    asSwitchPin[]         = {-1, 12, 13, 14, 2};     //2 is 8266 TXd1 and I2C stopped and blue LED on ESP12 module went on
//const int    asSwitchPin[]         = {-1, 12, 13, 14, 0};     //0 is 8266 Flash pin and when used caused relay to go on and off
//const int    asSwitchPin[]         = {-1, 12, 13, 14, 16};    //16 is 8266 User and Wake and broke I2C
const int    asSwitchPin[]         = {-1, 12, 13, 14, 15};      //0 is not a switch, switches are at 1,2,3,4
const bool   abSwitchInverted[]    = {0, true, true, true, true};  //Opto-isolated relays close when pulled low.
const int    sThermoDummySwitchNum = 0;  			//Thermostat Blynk LED lives with unused switch Relay #0.
const int    sOverheatSwitchNum    = 1;      	//Relay 1 opens on overheat and kills power to heater
const int    sHeatSwitchNum        = 2;      	//Relay 2 turns heat on
const int    sPumpSwitchNum        = 3;      	//Relay 3 turns pump on
const int    sSpeedSwitchNum       = 4;      //Relay 4 controls pump speed, Off is Low, On is High
const int		 sFlowSensorPin_			 = 16;
const float	 fOverheatDegF_				 = 105.0;

void SetupHotTub(){
  String szLogString = "SetupHotTub(): Begin";
  LogToBoth(szLogString);
	if(!bUseFlowSwitch){
		String szLogString = "***** SetupHotTub(): bUseFlowSwitch SET TO FALSE**************";
		LogToBoth(szLogString);
	}
	TurnHeatOn(false);
	bTurnPumpOn(false);
	sSetpointF_= 80;
	SetThermoState(true);
	if (bCheckOverheat(true)) {
		SetOverheatSwitch(true);
	}
	return;
}	//SetupHotTub


void HandleHeatSwitch(){
  String szLogString = "HandleHeatSwitch(): bHeatOn_=";
  LogToBoth(szLogString, bHeatOn_);
  //Serial << LOG0 << "HandleHeatSwitch(): bThermoOn, bFurnaceOn " << bThermoOn << ", " << bFurnaceOn << endl;
  //Make sure  switch state represents bHeatOn correctly.
  if (bHeatOn_){
    //Serial << LOG0 << "HandleFurnaceSwitch(): Set asSwitchState[sFurnaceSwitchNum] to sOn" << endl;
    abSwitchState_[sHeatSwitchNum]= sOn;
  } //if(bHeatOn)
  else{
    //Serial << LOG0 << "HandleFurnaceSwitch(): Set asSwitchState[sFurnaceSwitchNum] to sOff" << endl;
    abSwitchState_[sHeatSwitchNum]= sOff;
  } //if(bFurnaceOn)else
  SetSwitch(sHeatSwitchNum, abSwitchState_[sHeatSwitchNum]);
  return;
} //HandleHeatSwitch


void TurnHeatOn(bool bTurnOn){
  String szLogString= "TurnHeatOn(): bTurnOn=";
  LogToBoth(szLogString, bTurnOn);
  if (bTurnOn){
    if(bTurnPumpOn(true)){
			bHeatOn_= true;
			SetHeatSwitch(sSwitchClosed);
			sThermoTimesCount_= 0;
    }	//if(bTurnPumpOn(true))
    else{
    	//Pump didn't turn on and stay on, flow switch may have errored.
      sThermoTimesCount_= 0;
    }	//if(bTurnPumpOn(true))else
  } //if(bTurnOn)
  else{
    bHeatOn_= false;
    SetHeatSwitch(sSwitchOpen);
    sThermoTimesCount_= 0;
  } //if(bTurnOn)else
  return;
} //TurnHeatOn


void SetHeatSwitch(int sSwitchState){
  SetSwitch(sHeatSwitchNum, sSwitchState);
  return;
} //SetHeatSwitch


void SetThermoState(int sSwitchState){
  String szLogString= "SetThermoState(): sSwitchState=";
  LogToBoth(szLogString, sSwitchState);
  abSwitchState_[sThermoDummySwitchNum]= sSwitchState;
  if (sSwitchState == sOn){
    bThermoOn_= true;
  } //if(sState==sOn)
  else{
    bThermoOn_= false;
    bHeatOn_= false;
    sThermoTimesCount_= 0;
    SetHeatSwitch(sSwitchOpen);
  } //if(sState==sOn)else
  return;
} //SetThermoState


void HandleOverheat(){
	String szLogString= "TurnHeatOn(): Heat turned OFF";
	LogToBoth(szLogString);
  bThermoOn_= false;
	bHeatOn_= false;
	SetHeatSwitch(sSwitchOpen);
  return;
} //HandleOverheat


bool bCheckOverheat(bool bSetup){
	//Returns true if below overheat limit.
	bool bReturn= true;
  float fDegF= pBeckOneWire->fGetDegF(eVP42);
  if (bSetup) {
		String szLogString= "bCheckOverheat(): DegF=";
		LogToBoth(szLogString, fDegF);
		szLogString= "                  Limit=";
		LogToBoth(szLogString, fOverheatDegF_);
  }
	if (fDegF >= fOverheatDegF_) {
		String szLogString= "bCheckOverheat(): **ERROR= Overheat**";
		LogToBoth(szLogString);
		szLogString= "bCheckOverheat(): DegF=";
		LogToBoth(szLogString, fDegF);
		szLogString= "                  Limit=";
		LogToBoth(szLogString, fOverheatDegF_);
		SetOverheatSwitch(false);
		TurnHeatOn(false);
		SetThermoState(false);
		bOverheatOn_= true;
		bReturn= false;
	}	//if(fDegF>=fOverheatDegF)
  return(bReturn);
} //bCheckOverheat


void SetOverheatSwitch(int sSwitchState){
  String szLogString= "SetOverheatSwitch(): sSwitchState=";
  LogToBoth(szLogString, sSwitchState);
  SetSwitch(sOverheatSwitchNum, sSwitchState);
  return;
} //SetOverheatSwitch


void SetSwitch(int sSwitch, int sSwitchState){
  int sSwitchPin= asSwitchPin[sSwitch];
  bool bPinSetting;
  abSwitchState_[sSwitch]= sSwitchState;
  //bDebugLog= bDebug;
  if (abSwitchInverted[sSwitch]){
    bPinSetting= !sSwitchState;
  } //if(abSwitchInverted[sSwitch])
  else{
    bPinSetting= sSwitchState;
  } //if(abSwitchInverted[sSwitch])else
  //Serial << LOG0 << "SetSwitch(): sSwitch, sSwitchState, sSwitchPin, bPinSetting " << sSwitch << ", " << sSwitchState << ", " << sSwitchPin << ", " << bPinSetting <<  endl;
  //Serial << LOG0 << "SetSwitch(): sSwitch, sSwitchState, sSwitchPin, bPinSetting" <<  endl;
  String szLogString= "SetSwitch:  ";
  szLogString += sSwitch;
  szLogString += ",";
  szLogString += sSwitchState;
  szLogString += ",";
  szLogString += sSwitchPin;
  szLogString += ",";
  szLogString += bPinSetting;
  //LogToBoth(szLogString);

  //Some switch positions don't have pins, just Blynk LEDs.
  if (sSwitchPin >= 0){
    digitalWrite(sSwitchPin, bPinSetting);
    //asSwitchState_[sSwitch]= sSwitchState;
  } //if(sSwitchPin>=0)
  //bDebugLog= true;
  //HandleBlynkLEDs();
  return;
} //SetSwitch


bool bTurnPumpOn(bool bTurnOn){
	String szLogString;
	bool bReturn= true;
	if(bTurnOn){
		//Make sure flow switch is open before turning pump on
		if(!bUseFlowSwitch || !bReadFlowSensor()){
				szLogString = "bTurnPumpOn(): *** Pump ON, looking at flow switch ***";
				LogToBoth(szLogString);
				SetPumpSwitch(true);
				//Wait 2 seconds and make sure flow switch is closed.
				delay(2000);
				if(bUseFlowSwitch && !bReadFlowSensor()){
					szLogString = "bTurnPumpOn(): ERROR: Flow switch open with pump on";
					LogToBoth(szLogString);
					SetPumpSwitch(false);
					bReturn= false;
				}	//if(!bReadFlowSensor())
		}	//if(!bReadFlowSensor())
		else{
			szLogString = "bTurnPumpOn(): ERROR: Flow switch closed with pump off";
			LogToBoth(szLogString);
			bReturn= false;
		}	//if(!bReadFlowSensor())else
	}	//if(bTurnOn)
	else{
		SetPumpSwitch(false);
	}	//if(bTurnOn)else
	return(bReturn);
}	//bTurnPumpOn


void SetPumpSwitch(int sSwitchState){
  SetSwitch(sPumpSwitchNum, sSwitchState);
  return;
} //SetPumpSwitch


bool bCheckFlowSensor(bool bExpectedState){
	bool bReturn= true;
	if(bUseFlowSwitch){
		String szLogString = "CheckFlowSensor()";
		LogToBoth(szLogString);
		bReadFlowSensor();
		if(bFlowState_ != bExpectedState){
			bNoFlow_= true;
			TurnHeatOn(false);
			SetThermoState(false);
			bReturn= true;
		}	//if(!bReadFlowSensor())
	}	//if(bUseFlowSwitch)
	else{
		bReturn= bExpectedState;
	}
  return(bReturn);
} //CheckFlowSensor


bool bReadFlowSensor(){
  bFlowState_= !digitalRead(sFlowSensorPin_);
  return(bFlowState_);
} //bReadFlowSensor
//Last line.
