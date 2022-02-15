#pragma once
#include <Adafruit_INA260.h>

#include <sstream>
#include <string>

class Ina260
{
public:
    Ina260(const std::string &name, std::size_t i2cAddress) : _ina260(Adafruit_INA260()), _name(name), _i2cAddress(i2cAddress)
    {
    }

    void setup()
    {
        if (!_ina260.begin(_i2cAddress))
        {
            Serial.print(_name.c_str());
            Serial.println(": Couldn't find INA260 chip");
            while (1)
            {
            }
        }
        Serial.print(_name.c_str());
        Serial.println(": Found INA260 chip");
    }

    Adafruit_INA260 &get()
    {
        return _ina260;
    }

    std::string name() const
    {
        return _name;
    }    

    void print()
    {
        const auto current = _ina260.readCurrent();
        const auto voltage = _ina260.readBusVoltage();
        const auto power = _ina260.readPower();

        std::stringstream ss;
        Serial.println();
        ss << _name << ": " << current << " mA, " << voltage << " mV, " << power << " mW";
        Serial.println(ss.str().c_str());
    }

private:
    Adafruit_INA260 _ina260;
    const std::string _name;
    const std::size_t _i2cAddress;
};
