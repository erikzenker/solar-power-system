#pragma once
#include <chrono>

/*
Method to print the reason by which ESP32
has been awaken from sleep
*/
void printWakeupReason()
{
    esp_sleep_wakeup_cause_t wakeup_reason;

    wakeup_reason = esp_sleep_get_wakeup_cause();

    switch (wakeup_reason)
    {
    case ESP_SLEEP_WAKEUP_EXT0:
        Serial.println("Wakeup caused by external signal using RTC_IO");
        break;
    case ESP_SLEEP_WAKEUP_EXT1:
        Serial.println("Wakeup caused by external signal using RTC_CNTL");
        break;
    case ESP_SLEEP_WAKEUP_TIMER:
        Serial.println("Wakeup caused by timer");
        break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD:
        Serial.println("Wakeup caused by touchpad");
        break;
    case ESP_SLEEP_WAKEUP_ULP:
        Serial.println("Wakeup caused by ULP program");
        break;
    default:
        Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
        break;
    }
}

void startWakeupTimer(const std::chrono::seconds &seconds)
{
    const auto timeToSleepInMs = std::chrono::duration_cast<std::chrono::microseconds>(seconds).count();
    esp_sleep_enable_timer_wakeup(timeToSleepInMs);
    Serial.println("Setup ESP32 to sleep for every " + String(static_cast<float>(seconds.count())) + " s");
}

void sleep()
{
    Serial.println();
    Serial.println();
    Serial.println("Going to sleep now");
    delay(1000);
    Serial.flush();

    esp_deep_sleep_start();
}