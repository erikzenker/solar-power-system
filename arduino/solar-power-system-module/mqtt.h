#pragma once
#include <PubSubClient.h>
#include "ina260.h"

template <class TcpClient>
class Mqtt
{
private:
  PubSubClient _mqtt;
  uint32_t _lastReconnectAttempt = 0;

  // MQTT details
  const char *broker = "broker.hivemq.com";
  const char *topicInit = "v1/devices/solar-power-system/init";
  const char *topic = "v1/devices/solar-power-system";

public:
  Mqtt(TcpClient &client) : _mqtt(client)
  {
    _mqtt.setServer(broker, 1883);
  }

  void reconnect()
  {
    if (!_mqtt.connected())
    {
      Serial.println("=== MQTT NOT CONNECTED ===");
      // Reconnect every 10 seconds
      uint32_t t = millis();
      if (t - _lastReconnectAttempt > 10000L)
      {
        _lastReconnectAttempt = t;
        if (connect())
        {
          _lastReconnectAttempt = 0;
        }
      }
      delay(100);
      return;
    }
  }

  void loop(){
    _mqtt.loop();  
  }

  void sendSensorData(Ina260 &ina260)
  {
    String comma = String(",");
    String name = String("\"name\":") + String("\"") + String(ina260.name().c_str()) + String("\"");
    String current = String("\"current\":") + String(ina260.get().readCurrent());
    String voltage = String("\"voltage\":") + String(ina260.get().readBusVoltage());
    String power = String("\"power\":") + String(ina260.get().readPower());
    String message = String("{") + name + comma + current + comma + voltage + comma + power + String("}");

    String sensorTopic = String(topic) + String("/") + String(ina260.name().c_str());    
    Serial.print("Send mqtt message: " + sensorTopic + " " + message);
    _mqtt.publish(sensorTopic.c_str(), message.c_str());
  }

private:
  boolean connect()
  {
    Serial.print("Connecting to ");
    Serial.print(broker);

    // Connect to MQTT Broker
    boolean status = _mqtt.connect(topic);

    // Or, if you want to authenticate MQTT:
    // boolean status = mqtt.connect("GsmClientName", "mqtt_user", "mqtt_pass");

    if (status == false)
    {
      Serial.println(" fail");
      return false;
    }
    Serial.println(" success");
    _mqtt.publish(topicInit, "v1/devices/solar-power-system-started");
    return _mqtt.connected();
  }
};