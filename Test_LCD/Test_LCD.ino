/*
 Name:		Test_LCD.ino
 Created:	4/26/2021 9:04:12 PM
 Author:	aivel
*/

#include <M5Stack.h>
#include <WiFi.h>

///// GFX Free Fonts https://rop.nl/truetype2gfx/
#include "FreeSans5pt7b.h"
#include "FreeSans6pt7b.h"
#include "FreeSans7pt7b.h"
#include "FreeSans8pt7b.h"
//#include "FreeSans9pt7b.h"
#include "FreeSans10pt7b.h"
#include "FreeSans15pt7b.h"
#include "FreeSans20pt7b.h"

// the setup function runs once when you press reset or power the board
void setup() {
    ////// Initialize and setup M5Stack
    M5.begin();
    M5.Power.begin();
    M5.Lcd.println(F("M5 started"));
    M5.Lcd.setTextColor(WHITE);
    //M5.Lcd.setTextDatum(MC_DATUM);
    //M5.Lcd.setTextSize(2);
    delay(2000);
}

// the loop function runs over and over again until power down or reset
void loop() {
    for (int i = 0; i <= 255; i++) {
        // 0
        M5.Lcd.fillRect(0, 0, 160, 120, M5.Lcd.color565(i, i, 0));
        M5.Lcd.setTextColor( M5.Lcd.color565(255 - i, 255 - i, 255) );
        M5.Lcd.setFreeFont(&FreeSans7pt7b);
        M5.Lcd.setTextDatum(TL_DATUM);
        M5.Lcd.drawString(F("LED Intensity (%):"), 10, 10);
        M5.Lcd.setFreeFont(&FreeSans20pt7b);
        M5.Lcd.setTextDatum(MC_DATUM);
        M5.Lcd.drawString(String(i), 80, 60);

        // 1
        M5.Lcd.fillRect(160, 0, 160, 120, M5.Lcd.color565(0, 0, i));
        M5.Lcd.setTextColor(M5.Lcd.color565(255, 255, 255 - i));
        M5.Lcd.setFreeFont(&FreeSans7pt7b);
        M5.Lcd.setTextDatum(TL_DATUM);
        M5.Lcd.drawString(F("Fan Speed (rmp):"), 10 + 160, 10);
        M5.Lcd.setFreeFont(&FreeSans20pt7b);
        M5.Lcd.setTextDatum(MC_DATUM);
        M5.Lcd.drawString(String(i * 100), 80 + 160, 60);
        
        // 2
        M5.Lcd.fillRect(0, 120, 160, 120, M5.Lcd.color565(i, 0, 0));
        M5.Lcd.setTextColor(M5.Lcd.color565(255 - i, 255, 255 - i));
        M5.Lcd.setFreeFont(&FreeSans7pt7b);
        M5.Lcd.setTextDatum(TL_DATUM);
        M5.Lcd.drawString(F("LED Temp setpt (C):"), 10, 10 + 120);
        M5.Lcd.setFreeFont(&FreeSans20pt7b);
        M5.Lcd.setTextDatum(MC_DATUM);
        M5.Lcd.drawString(String(i), 80, 60 + 120);

        // 3
        M5.Lcd.fillRect(160, 120, 160, 120, M5.Lcd.color565(i, 0, 0));
        M5.Lcd.setTextColor(M5.Lcd.color565(255 - i, 255, 255 - i));
        M5.Lcd.setFreeFont(&FreeSans7pt7b);
        M5.Lcd.setTextDatum(TL_DATUM);
        M5.Lcd.drawString(F("LED Temp real (C):"), 10 + 160, 10 + 120);
        M5.Lcd.setFreeFont(&FreeSans20pt7b);
        M5.Lcd.setTextDatum(MC_DATUM);
        M5.Lcd.drawString(String(i), 80 + 160, 60 + 120);



        delay(100);

    }


  
}
