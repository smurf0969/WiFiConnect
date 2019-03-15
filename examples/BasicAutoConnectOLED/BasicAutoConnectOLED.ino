
#include "WiFiConnectOLED.h" //include before SSD1306.h if using custom fonts

//******* OLED *********

#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
/*
   Origional library https://github.com/ThingPulse/esp8266-oled-ssd1306
   Customised library https://github.com/smurf0969/esp8266-oled-ssd1306/tree/Allow-overriding-default-font
*/
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"


// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// connect ESP12E  D1 to SCL, D2 to SDA
//OLED pins to ESP32 GPIOs via this connecthin:
//OLED_SDA -- GPIO4
//OLED_SCL -- GPIO15
//OLED_RST -- GPIO16

#ifdef ESP32
#define OLED_RESET 16
#else
#define OLED_RESET 10
#endif

#ifdef ESP32
SSD1306  display(0x3c, 4, 15);
#define oledPWR 16 //Pin to supply power to OLED
#else
SSD1306  display(0x3c, 4, 5);
#define oledPWR 10 //Pin to supply power to OLED
#endif

WiFiConnectOLED wc(&display, oledPWR); //Initialise our connector with an OLED display


void configModeCallback(WiFiConnect *mWiFiConnect) {
  Serial.println("Entering Access Point");
}


void startWiFi(boolean showParams = false) {
  wc.begin(true);
  wc.setDebug(true);
  /* Set our callbacks */
  wc.setAPCallback(configModeCallback);

  //wc.screenTest(); //test screen by cycling through the presete screens
  //wc.resetSettings(); //helper to remove the stored wifi connection, comment out after first upload and re upload


    /*
       AP_NONE = Continue executing code
       AP_LOOP = Trap in a continuous loop
       AP_RESET = Restart the chip
    */
    if (!wc.autoConnect()) { // try to connect to wifi
      /* We could also use button etc. to trigger the portal on demand within main loop */
      wc.startConfigurationPortal(AP_LOOP);//if not connected show the configuration portal
    }

  // when displayLoop is called from main loop, will turn of display after time period
  wc.displayTurnOFF((60 * 1000 * 10));
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(100);
  }

  startWiFi();

}

void loop() {
  delay(100);

  wc.displayLoop();
  if (WiFi.status() != WL_CONNECTED) {
    if (!wc.autoConnect()) wc.startConfigurationPortal(AP_RESET);
  }
}
