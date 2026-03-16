#pragma once

#include <Arduino.h>

#define MAX_BUTTONS 10

typedef void (*ButtonCallback)(uint8_t pin, uint32_t pressTime);

class ButtonManager {
public:

    ButtonManager(uint16_t debounce = 20, uint16_t minPress = 100);

    bool addButton(uint8_t pin,
                   ButtonCallback cb,
                   uint8_t activeLevel = LOW,
                   uint8_t mode = INPUT_PULLUP);

    void update();
    void setBeep(uint8_t pin, const uint16_t *times, uint8_t count, uint16_t beepDuration=200);

private:
    struct Button {
        uint8_t pin;
        uint8_t activeLevel;

        bool lastState;
        bool currentState;

        uint32_t pressStart;
        uint32_t lastChange;

        uint8_t beepIndex; // serve a ricordare per quale soglia è già stata emessa il beep.

        ButtonCallback callback;
    };


    Button buttons[MAX_BUTTONS];
    uint8_t m_buttonCount;

    uint16_t m_debounceTime;
    uint16_t m_minPressTime;

    // --- beep variables
    uint8_t m_beepPin = 255;
    const uint16_t *m_beepTimes = nullptr;
    uint8_t m_beepCount = 0;

    // --- beep runtime
    bool m_beepActive = false;
    uint32_t m_beepStart = 0;
    uint16_t m_beepDuration = 200;
};

