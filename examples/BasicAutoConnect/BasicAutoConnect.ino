#include "WiFiConnect.h" //include before SSD1306.h if using custom fonts
WiFiConnect wc;
void configModeCallback(WiFiConnect *mWiFiConnect) {
  Serial.println("Entering Access Point");
}


void startWiFi(boolean showParams = false) {
 
  wc.setDebug(true);
  /* Set our callbacks */
  wc.setAPCallback(configModeCallback);

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

  if (WiFi.status() != WL_CONNECTED) {
    if (!wc.autoConnect()) wc.startConfigurationPortal(AP_RESET);
  }
}
