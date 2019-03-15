/*!
   @file WiFiConnectOLED.cpp

   WiFi1 Connection Manager with Captive Portal

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

   Author

   Written by Stuart Blair.

   License

   GNU General Public License v3.0 licence, all text here must be included in any redistribution and you should receive a copy of the license file.

*/
#include "WiFiConnectOLED.h"
#include "imagedata.h"
/**************************************************************************/
/*!
    @brief  Class initialiser.
    @param    odisplay
              A reference to the OLED display object for drawing
              to the screen.
    @param    pwrpin
              The number of the pin that supplies power to the screen,
              allows us to turn it off if we wish.
    @param    screenW
              The pixel width of the OLED.             
     @param   screenH
              The pixel height of the OLED.             
*/
/**************************************************************************/
WiFiConnectOLED::WiFiConnectOLED(OLEDDisplay *odisplay, int pwrpin, int screenW, int screenH) {
  this->_display = odisplay;
  _pwrPin=pwrpin;
  pinMode(_pwrPin, OUTPUT);
  _screenW = screenW;
  _screenH = screenH;
}
/**************************************************************************/
/*!
    @brief  Starts the display and optionally sets its orientation
    @param    flipV
              Flips the screen display vertically.
    @param    mirror
              Mirrors the screen image
*/
/**************************************************************************/
void WiFiConnectOLED::begin(boolean flipV,boolean mirror) {
  digitalWrite(_pwrPin, HIGH);
  delay(50);
  if (!this->_display->init()) {
    Serial.println(F("Failed to initialize display"));
    for (;;); // Don't proceed, loop forever
  } else {
    Serial.println(F("Display Initialized.."));
  }
  if(flipV)  this->_display->flipScreenVertically();
  if(mirror)this->_display->mirrorScreen();
  showInit();
}
/**************************************************************************/
/*!
    @brief  Called during begin(), shows the logo xbm

*/
/**************************************************************************/
void WiFiConnectOLED::showInit() {
  if(_initDone){return;}
  _initDone = true;
  this->_display->setContrast(255);
  this->_display->setFont(Roboto_16);
  this->_display->drawString(0, 0, F("Initialising"));
  for (int x = 70; x <= 95; x += 2) {
    this->_display->drawString(x, 0, F(" ."));
    this->_display->display();
    delay(50);
  }
  clear();
    this->_display->drawXbm((_screenW - wcInitW) / 2, (_screenH - wcInitH) / 2 , wcInitW, wcInitH, wcInit);
    this->_display->display();
    delay(2500);
  clear();
  this->_display->display();
}
/**************************************************************************/
/*!
    @brief  Clears the screen buffer.
            note: Does not alter whats already showing
*/
/**************************************************************************/
void WiFiConnectOLED::clear() {
  checkInit();
  this->_display->clear();
}
/**************************************************************************/
/*!
    @brief  Starts a runthrough of the screens, useful for testing.
*/
/**************************************************************************/
void WiFiConnectOLED::screenTest()
{
  _screenTest=true;
  int delayMS = 1500;
  clear();
  FlashTurningOffScreen();
  digitalWrite(_pwrPin, HIGH);
  delay(100);
  clear();
  displayManualReset();
  delay(delayMS);
  clear();
  for (int x = 1; x <= 4; x++) {
    displayConnecting(x, 4);
    delay(delayMS);
    clear();
  }
  displayAP();
  delay(delayMS);
  clear();
  displayParams();
  delay(delayMS);
  clear();
  displayConnected();
  delay(delayMS);
  clear();
  _screenTest=false;
}
/**************************************************************************/
/*!
    @brief  Displays a screen that flashes the words Turning Off before
            turning off the display.
*/
/**************************************************************************/
void WiFiConnectOLED::FlashTurningOffScreen() {
  OLEDDISPLAY_COLOR cNow = this->_display->getColor();
  int secs = 0;
  int maxsecs = 5 * 2;
  OLEDDISPLAY_COLOR color = WHITE;
  clear();
  this->_display->setFont(Roboto_24);

  this->_display->setTextAlignment(TEXT_ALIGN_CENTER);

  for (secs = 0; secs < maxsecs; secs++) {
    clear();
    this->_display->setColor(color);
    this->_display->fillRect(0, 0, _screenW, _screenH);
    color = (color == WHITE ? BLACK : WHITE);
    this->_display->setColor(color);
    this->_display->drawStringMaxWidth(_screenW / 2, (_screenH / 2) - 26, _screenW - 10, F("Turning off display"));
    this->_display->display();
    delay(500);
  }
  this->_display->setColor(cNow);
  delay(100);
    digitalWrite(_pwrPin, LOW);
}
/**************************************************************************/
/*!
    @brief  When put in another or main loop, check to see if it should
            turn off the display.
*/
/**************************************************************************/
void WiFiConnectOLED::displayLoop() {
  if (_displayTurnOffAtMS > 0) {
    if (millis() >= _displayTurnOffAtMS) {
      FlashTurningOffScreen();
    }
  }
}
/**************************************************************************/
/*!
    @brief  Turns of the display using FlashTurningOffScreen() or via
            displayLoop() if a parameter >0 is supplied.
    @param    ms
              Milliseconds to wait before turnung off.
*/
/**************************************************************************/
void WiFiConnectOLED::displayTurnOFF(int ms) {
  if (ms <= 0) {
    FlashTurningOffScreen();
  } else if (ms > 0) {
    _displayTurnOffAtMS = millis() + ms;
  }
}
/**************************************************************************/
/*!
    @brief  Send a High signal to the power pin, turning the display on.
*/
/**************************************************************************/
void WiFiConnectOLED::displayON(){
      digitalWrite(_pwrPin, HIGH);
}
/**************************************************************************/
/*!
    @brief  When in an infinate loop after failing to connect to a network
            from within a access point, displays the Manual Reset required
            screen to the user.
*/
/**************************************************************************/
void WiFiConnectOLED::displayManualReset() {
  clear();
  this->_display->drawXbm((_screenW - manual_reset_dataW) / 2, (_screenH - manual_reset_dataH) / 2 , manual_reset_dataW, manual_reset_dataH, manual_reset_data);
  this->_display->display();
}
/**************************************************************************/
/*!
    @brief  Displays the attempting to connect screen, showing the progess
            of the attempts made to connect.
    @param    attempt
              The number of the current attempt.
    @param    totalAttempts
              The total number of allowable attempts.
*/
/**************************************************************************/
void WiFiConnectOLED::displayConnecting(int attempt, int totalAttempts) {
  clear();
  this->_display->setFont(Roboto_16);
  this->_display->setTextAlignment(TEXT_ALIGN_CENTER);
  this->_display->drawStringMaxWidth(64, 5, 128, F("Trying to connect to WiFi"));
  //String s = "Attempt " + String(attempt) + (totalAttempts > 1 ? " of " + String(totalAttempts) : "");
  String s = "Attempt " + String(attempt) +  " of " + String(totalAttempts) ;
  this->_display->setFont(Roboto_12);
  this->_display->drawStringMaxWidth(64, 47, 128, s);
  this->_display->display();
  if (attempt == 1) {
    delay(500);
  }
}
/**************************************************************************/
/*!
    @brief  Displays the access point screen with the ap name and ip address.
*/
/**************************************************************************/
void WiFiConnectOLED::displayAP() {
  clear();
  this->_display->drawXbm(0, (_screenH - ap_dataH) , ap_dataW, ap_dataH, ap_data);
  this->_display->setFont(Roboto_10);
  this->_display->setTextAlignment(TEXT_ALIGN_CENTER);
  this->_display->drawString(64, 0,  F("Access Point Mode"));
  this->_display->setFont(Roboto_16);
  this->_display->drawString(64, 12,  getAPName());
  this->_display->setFont(Roboto_12);
  this->_display->drawString(((_screenW - ap_dataW) / 2) + ap_dataW, _screenH - (ap_dataH / 2) - 8,  (_screenTest?"199.199.199.199":WiFi.softAPIP().toString()));
  this->_display->display();
}
/**************************************************************************/
/*!
    @brief  Displays the paramets access point screen with the ap name and ip address.
*/
/**************************************************************************/
void WiFiConnectOLED::displayParams() {
  clear();
  this->_display->drawXbm(0, (_screenH - ap_param_dataH) , ap_param_dataW, ap_param_dataH, ap_param_data);
  this->_display->setFont(Roboto_10);
  this->_display->setTextAlignment(TEXT_ALIGN_CENTER);
  this->_display->drawString(64, 0,  F("Access Point Mode"));
  this->_display->setFont(Roboto_16);
  this->_display->drawString(64, 12,  getAPName());
  this->_display->setFont(Roboto_12);
  this->_display->drawString(((_screenW - ap_dataW) / 2) + ap_dataW, _screenH - (ap_dataH / 2) - 8, (_screenTest?"199.199.199.199":WiFi.softAPIP().toString()));
  this->_display->display();
}
/**************************************************************************/
/*!
    @brief  Displays the screen showing the ip address within the connected network.
*/
/**************************************************************************/
void WiFiConnectOLED::displayConnected() {
  clear();
  this->_display->drawXbm((_screenW - wifi_connected_width) / 2, (_screenH - wifi_connected_height) , wifi_connected_width, wifi_connected_height, wifi_connected_data);
  this->_display->setFont(Roboto_16);
  this->_display->setTextAlignment(TEXT_ALIGN_CENTER);
  this->_display->drawStringMaxWidth(64, 5, _screenW, (_screenTest?"199.199.199.199":WiFi.localIP().toString()));
  this->_display->display();
}
/**************************************************************************/
/*!
    @brief  Clear a certain portion of the screen
            TODO: Not implemented yet.
    @param    startX
              Starting X position
    @param    startY
              Starting Y position
    @param    endX
              Ending X position
    @param    endY
              Ending Y position              \
*/
/**************************************************************************/
void WiFiConnectOLED::clear(int startX, int startY, int endX, int endY) {
  checkInit();
}
/**************************************************************************/
/*!
    @brief  Checks to see if screen has been initialised.
*/
/**************************************************************************/
void WiFiConnectOLED::checkInit() {
  if (!_initDone) {
    showInit();
  }
}
/**************************************************************************/
/*!
    @brief  Ourputs messages to the Serial debugger/monitor
    @param text
            The text to be displayed.
*/
/**************************************************************************/
template <typename Generic>
void WiFiConnectOLED::DEBUG_WC(Generic text) {
  if (_debug) {
    Serial.print("*WCO: ");
    Serial.println(text);
  }
}
