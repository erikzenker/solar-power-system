#include <WiFi.h>
#include <WiFiMulti.h>

#include "LocalWifiSettings.h"
#include "ina260.h"
#include "mqtt.h"
#include "sleep.h"

#include <memory>

// Global objects
std::array<Ina260, 3> sensors = {Ina260{"input", 0x40}, Ina260{"output", 0x41}, Ina260{"battery", 0x44}};
WiFiMulti WiFiMulti;
WiFiClient client;
Mqtt<WiFiClient> mqtt(client);

void enableWifi(){
  WiFi.disconnect(false);
  WiFi.mode(WIFI_STA);
  
  WiFiMulti.addAP(wifiSsid, wifiPassword);

  Serial.println();
  Serial.println();
  Serial.print("Waiting for WiFi... ");

  while (WiFiMulti.run() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }


  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void disableWifi(){
  delay(100);
  WiFi.disconnect(true); 
  WiFi.mode(WIFI_OFF);
  Serial.println();
  Serial.println("WiFi disconnected!");
}

void setup()
{
  Serial.println();    
  Serial.println("Setup");  
  // Setup serial connections
  Serial.begin(115200);
  while (!Serial)
  {
    delay(10);
  }

  // Init status led
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // Setup current sensors
  for (auto &sensor : sensors)
  {
    sensor.setup();
  }
}


#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  30


void loop()
{
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.println();  
  Serial.println();
  printWakeupReason();

  enableWifi();

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

  // Main loop
  mqtt.reconnect();
  mqtt.loop();

  for (auto &sensor : sensors)
  {
    sensor.print();
    mqtt.sendSensorData(sensor);
  }

  // Going back to sleep
  // startWakeupTimer(std::chrono::seconds{30});
  // sleep();
  disableWifi();
  digitalWrite(LED_BUILTIN, LOW);
  esp_light_sleep_start();
}
