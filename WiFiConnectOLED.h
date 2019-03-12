/*!
   @file WiFiConnectOLED.h

   WiFi Connection Manager with Captive Portal

   Introduction

   This is the documentation for WiFiConnect for the Arduino platform.
   It is a WiFi connection manager for use with the popular ESP8266 and ESP32 chips.
   It contains a captive portal to allow easy connection and changing of WiFi netwoks
   via a web based interface and allows for additional user parameters.
   It can also display messages via a OLED screen see WiFiConnectOLED class.

   This is a heavily customised version from the origional <a href="https://github.com/tzapu/WiFiManager">WiFiManager</a>
   developed by https://github.com/tzapu .

   Dependencies

   This library depends on <a href="https://github.com/esp8266/Arduino">
    ESP8266 Arduino Core</a> and <a href="https://github.com/espressif/arduino-esp32">ESP32 Arduino Core</a> being present on your system.
    Please make sure you have installed the latest version before using this library.

   
   Written by Stuart Blair.

   License

   GNU General Public License v3.0 licence, all text here must be included in any redistribution and you should receive a copy of the license file.

*/
#ifndef WIFI_CONNECT_OLED
#define WIFI_CONNECT_OLED
#include <Arduino.h>
#include "Roboto_Font.h" //include before oledisplay to use file from sketch folder
#define OLEDDISPLAYFONTS_h
#define OLEDDISPLAY_DEFAULTFONT Roboto_10
#include "OLEDDisplay.h"
#include "WiFiConnect.h"
/**************************************************************************/
/*! 
    @brief  Class extending WiFiConnect to allow the use of OLED for showing
            information to users.
*/
/**************************************************************************/
class WiFiConnectOLED: public WiFiConnect {
  public:
    WiFiConnectOLED(OLEDDisplay *odisplay, int pwrpin, int screenW = 128, int screenH = 64);
    void begin(boolean flipV = false, boolean mirror = false);
    void clear();
    void clear(int startX, int startY, int endX, int endY);
    void FlashTurningOffScreen();
    void screenTest();
    void displayTurnOFF(int ms = 5000) override;
    void displayLoop() override;
    void displayON() override;
  private:
    OLEDDisplay *_display;
    int _screenW = 128;
    int _screenH = 64;
    void showInit();
    boolean _initDone = false;
    void checkInit();
    long _displayTurnOffAtMS = -1;
    int _pwrPin;

  protected:
    boolean _screenTest = false; ///< Flag for use during screenTest() method.
    template <typename Generic>
    void DEBUG_WC(Generic text);
    void displayConnecting(int attempt, int totalAttempts) override;
    void displayConnected() override;
    void displayAP() override;
    void displayParams() override;
    void displayManualReset() override;

};


#endif
