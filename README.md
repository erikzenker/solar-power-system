# Solar Power System

Source code for the project described in this [gist](https://gist.github.com/erikzenker/c7c082db0377468c1f32368b9aaf247b)

# Subscribe Mqtt Broker to Check Metrics
```
mosquitto_sub -h broker.hivemq.com -p 1883 -t v1/devices/solar-power-system/+
```