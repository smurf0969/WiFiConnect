#include <FS.h> //this needs to be first, or it all crashes and burns...
#include "WiFiConnectOLED.h" //include before SSD1306.h if using custom fonts

#ifdef ESP32
#include <SPIFFS.h> // Used to save custom parameters
#endif

#include <ArduinoJson.h> //https://github.com/bblanchon/ArduinoJson

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

/* Our custom Parameters */
char server_ip[16] = "192.168.1.187";
char server_port[5] = "8000";
char server_path[16] = "/live";
int json_doc_size = 256;
WiFiConnectParam custom_server_ip("server_ip", "server_ip", server_ip, 16);
WiFiConnectParam custom_server_port("server_port", "server_port", server_port, 5);
WiFiConnectParam custom_server_path("server_path", "server_path", server_path, 16);

WiFiConnectOLED wc(&display, oledPWR); //Initialise our connector with an OLED display

/*
    Callback for when parameters need saving.
    Can be used to set a flag preferably with saving taking place in main loop.

*/
bool configNeedsSaving = false;
void saveConfigCallback() {
  Serial.println("Should save config");
  configNeedsSaving = true;
}
void configModeCallback(WiFiConnect *mWiFiConnect) {
  Serial.println("Entering Access Point");
}
/* Save our custom parameters */
void saveConfiguration() {
  configNeedsSaving = false;
  if (!SPIFFS.begin()) {
    Serial.println("UNABLE to open SPIFFS");
    return;
  }

  strcpy(server_ip, custom_server_ip.getValue());
  strcpy(server_port, custom_server_port.getValue());
  strcpy(server_path, custom_server_path.getValue());

  Serial.println("saving config");
  DynamicJsonDocument doc(json_doc_size);


  doc["server_ip"] = server_ip;
  doc["server_port"] = server_port;
  doc["server_path"] = server_path;

  File configFile = SPIFFS.open("/config.json", "w");

  if (!configFile) {
    Serial.println("failed to open config file for writing");
  }
  serializeJson(doc, Serial);
  Serial.println("");
  serializeJson(doc, configFile);
  configFile.close();
  //end save
  SPIFFS.end();
}


void startWiFi(boolean showParams = false) {
  wc.begin(true);
  wc.setDebug(true);
  /* Set our callbacks */
  wc.setSaveConfigCallback(saveConfigCallback);
  wc.setAPCallback(configModeCallback);
  /* Add some custom parameters */
  wc.addParameter(&custom_server_ip);
  wc.addParameter(&custom_server_port);
  wc.addParameter(&custom_server_path);

  //wc.screenTest(); //test screen by cycling through the presete screens
  //wc.resetSettings(); //helper to remove the stored wifi connection, comment out after first upload and re upload

  if (!showParams) {
    /*
       AP_NONE = Continue executing code
       AP_LOOP = Trap in a continuous loop
       AP_RESET = Restart the chip
    */
    if (!wc.autoConnect()) { // try to connect to wifi
      /* We could also use button etc. to trigger the portal on demand within main loop */
      wc.startConfigurationPortal(AP_LOOP);//if not connected show the configuration portal
    }
  } else {
    /* We could also use button etc. to trigger the portal on demand within main loop */
    wc.startParamsPortal(AP_NONE); //Show the parameters portal
  }
  // when displayLoop is called from main loop, will turn of display after time period
  wc.displayTurnOFF((60 * 1000 * 10));
}

void loadConfiguration() {
  if (!SPIFFS.begin()) {
    Serial.println(F("Unable to start SPIFFS"));
    while (1) {
      delay(1000);
    }
  } else {
    Serial.println(F("mounted file system"));

    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");

      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println(F("opened config file"));
        size_t sizec = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[sizec]);

        configFile.readBytes(buf.get(), sizec);
        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, buf.get());
        if (error) {
          Serial.println("failed to load json config");
        } else {
          strcpy(server_ip, doc["server_ip"]);
          strcpy(server_port, doc["server_port"]);
          strcpy(server_path, doc["server_path"]);
          custom_server_ip.setValue(server_ip);
          custom_server_port.setValue(server_port);
          custom_server_path.setValue(server_path);
        }
        configFile.close();

      }
    } else {
      Serial.println(F("Config file not found"));
    }
    SPIFFS.end();
  }
}
void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(100);
  }

  loadConfiguration();

  startWiFi();

}

void loop() {
  delay(100);
  if (configNeedsSaving) {
    saveConfiguration();
  }
  wc.displayLoop();
  if (WiFi.status() != WL_CONNECTED) {
    if (!wc.autoConnect()) wc.startConfigurationPortal(AP_RESET);
  }
}
