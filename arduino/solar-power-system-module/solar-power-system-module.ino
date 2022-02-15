#include <WiFi.h>
#include <WiFiMulti.h>

#include "LocalWifiSettings.h"
#include "ina260.h"
#include "mqtt.h"

#include <memory>

// Global objects
std::array<Ina260, 3> sensors = {Ina260{"Solar", 0x40}, Ina260{"Consumer", 0x41}, Ina260{"Batterie", 0x44}};
WiFiMulti WiFiMulti;
WiFiClient client;
Mqtt<WiFiClient> mqtt(client);

void setup()
{
  // Setup serial connections
  Serial.begin(115200);
  while (!Serial)
  {
    delay(10);
  }

  // Setup current sensors
  for (auto &sensor : sensors)
  {
    sensor.setup();
  }

  // Setup wifi
  WiFiMulti.addAP(wifiSsid, wifiPassword);

  Serial.println();
  Serial.println();
  Serial.print("Waiting for WiFi... ");

  while (WiFiMulti.run() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  mqtt.reconnect();
  mqtt.loop();

  for (auto &sensor : sensors)
  {
    sensor.print();
    mqtt.sendSensorData(sensor);
  }

  delay(5000);
}
