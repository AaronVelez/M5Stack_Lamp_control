/*
 Name:		Main.ino
 Created:	4/23/2021 10:37:42 PM
 Author:	aivel
*/


//////////////////////////////////////////////////////////////////
////// Libraries and its associated constants and variables //////
//////////////////////////////////////////////////////////////////

////// Board library
#include <M5Stack.h>


////// Libraries
#include <WiFi.h>
#include <Wire.h>

// External ADC using ADS1115
#include <Adafruit_ADS1X15.h>
Adafruit_ADS1115 ads;

// GFX Free Fonts https://rop.nl/truetype2gfx/
#include "FreeSans5pt7b.h"
#include "FreeSans6pt7b.h"
#include "FreeSans7pt7b.h"
#include "FreeSans8pt7b.h"
//#include "FreeSans9pt7b.h"
#include "FreeSans10pt7b.h"
#include "FreeSans15pt7b.h"
#include "FreeSans20pt7b.h"


//////////////////////////////////////////
////// User Constants and Variables //////
//////////////////////////////////////////


////// Constants
// LED Temp
const float Max_LED_Temp = 55;
const float Min_LED_Temp = 15;
// Thermistor
const float Ref_voltage = 3.3;  // Thermistor excitation voltage
const float Nom_R = 10;      // Nominal thermistor resistance in kohms at 25°C
const float Div_R = 3.32;
// Fan Speed
const float Fan_max_rpm = 3000 * 1.15;    // Nominal max fan speed plus 15 % tolerance
// Non-linear voltage Temp control (due to zener diode)
const float a = 1.1;


////// Measured instantaneous variables
float LampCtrlSum = 0;
float LampCtrlVolt = 0;     // ads 0
float LampCtrlPct = 0;      // Perecentage

float TempCtrlSum = 0;
float TempCtrlVolt = 0;     // ads 1
float LampCtrl = 0;
float TempCtrlTrinketVolt = 0;
float TempCtrlCDeg = 0;

float LampTempSum = 0;
float LampTempVolt = 0;     // ads 2
float Thermistor_R = 0;
float LampTempCDeg = 0;

float FanSpeedSum = 0;
float FanSpeedVolt = 0;     // ads 3
float FanSpeedRPM = 0;

////// Color values
int LampCtrlRGB = 0;
int TempCtrlRGB = 0;
int LampTempRGB = 0;
int FanSpeedRGB = 0;




// the setup function runs once when you press reset or power the board
void setup() {
    ////// Initialize and setup M5Stack
    M5.begin();
    M5.Power.begin();
    M5.Lcd.println(F("M5 started"));

    ////// Use Adafruit ADS1115 library and DFRobot ADS1115 board
    M5.Lcd.println();
    M5.Lcd.println(F("Check that ADC address switch is set to 0x48"));
    ads.begin(0x48);
    ads.setDataRate(RATE_ADS1115_8SPS);
    ads.setGain(GAIN_TWOTHIRDS);
    
    // Configure LCD
    M5.Lcd.setTextColor(WHITE);
    

}

// the loop function runs over and over again until power down or reset
void loop() {
    ////// Read analog LED intensity control
    LampCtrlVolt = ads.computeVolts(ads.readADC_SingleEnded(0));
    LampCtrlPct = min( ((LampCtrlVolt * 100) / 5), 100);

    ////// Read analog Heatsink Temperatre control
    TempCtrlVolt = ads.computeVolts(ads.readADC_SingleEnded(1));    // Read voltage (0-5 V)
    TempCtrlTrinketVolt = ;     //
    TempCtrlCDeg = ;            // Convert control voltage (0-5 v range) to control temperautre

    // TempCtrlCDeg = min( Min_LED_Temp + (((Max_LED_Temp - Min_LED_Temp) / 3.3) * TempCtrlVolt),
                        Max_LED_Temp);

    ////// Read heatsink Temp analog signal
    LampTempVolt = ads.computeVolts(ads.readADC_SingleEnded(2));
    Thermistor_R = (LampTempVolt * Div_R) / (Ref_voltage - LampTempVolt);
    LampTempCDeg = RtoTemp(Thermistor_R, Nom_R);

    ////// Read Fan speed analog singal
    FanSpeedVolt = ads.computeVolts(ads.readADC_SingleEnded(3));
    FanSpeedRPM = (FanSpeedVolt * Fan_max_rpm) / 3.3;

    ////// Print to LCD
    LampCtrlRGB = round((LampCtrlPct * 255) / 100);
    TempCtrlRGB = round((TempCtrlCDeg * 255) / 100);
    LampTempRGB = round((LampTempCDeg * 255) / 100);
    FanSpeedRGB = round((FanSpeedRPM * 255) / Fan_max_rpm);

    // Top Left
    M5.Lcd.fillRect(0, 0, 160, 120, M5.Lcd.color565(LampCtrlRGB, LampCtrlRGB, 0));
    M5.Lcd.setTextColor(M5.Lcd.color565(255 - LampCtrlRGB, 255 - LampCtrlRGB, 255));
    M5.Lcd.setFreeFont(&FreeSans7pt7b);
    M5.Lcd.setTextDatum(TL_DATUM);
    M5.Lcd.drawString(F("LED Intensity (%):"), 10, 10);
    M5.Lcd.setFreeFont(&FreeSans20pt7b);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.drawString(String(LampCtrlPct), 80, 60);

    // Top Right
    M5.Lcd.fillRect(160, 0, 160, 120, M5.Lcd.color565(0, 0, FanSpeedRGB));
    M5.Lcd.setTextColor(M5.Lcd.color565(255, 255, 255 - FanSpeedRGB));
    M5.Lcd.setFreeFont(&FreeSans7pt7b);
    M5.Lcd.setTextDatum(TL_DATUM);
    M5.Lcd.drawString(F("Fan Speed (rmp):"), 10 + 160, 10);
    M5.Lcd.setFreeFont(&FreeSans20pt7b);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.drawString(String(FanSpeedRPM), 80 + 160, 60);

    // Bottom Left
    M5.Lcd.fillRect(0, 120, 160, 120, M5.Lcd.color565(TempCtrlRGB, 0, 0));
    M5.Lcd.setTextColor(M5.Lcd.color565(255 - TempCtrlRGB, 255, 255 - TempCtrlRGB));
    M5.Lcd.setFreeFont(&FreeSans7pt7b);
    M5.Lcd.setTextDatum(TL_DATUM);
    M5.Lcd.drawString(F("LED Temp setpt (C):"), 10, 10 + 120);
    M5.Lcd.setFreeFont(&FreeSans20pt7b);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.drawString(String(TempCtrlCDeg), 80, 60 + 120);

    // Bottom Right
    M5.Lcd.fillRect(160, 120, 160, 120, M5.Lcd.color565(LampTempRGB, 0, 0));
    M5.Lcd.setTextColor(M5.Lcd.color565(255 - LampTempRGB, 255, 255 - LampTempRGB));
    M5.Lcd.setFreeFont(&FreeSans7pt7b);
    M5.Lcd.setTextDatum(TL_DATUM);
    M5.Lcd.drawString(F("LED Temp real (C):"), 10 + 160, 10 + 120);
    M5.Lcd.setFreeFont(&FreeSans20pt7b);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.drawString(String(LampTempCDeg), 80 + 160, 60 + 120);
    
}
