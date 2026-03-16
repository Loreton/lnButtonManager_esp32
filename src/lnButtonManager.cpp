#include "lnButtonManager.h"

ButtonManager::ButtonManager(uint16_t debounce, uint16_t minPress)
{
    m_debounceTime = debounce;
    m_minPressTime = minPress;
    m_buttonCount = 0;
}

bool ButtonManager::addButton(uint8_t pin, ButtonCallback cb, uint8_t activeLevel, uint8_t mode) {
    if (m_buttonCount >= MAX_BUTTONS)
        return false;

    Button &b = buttons[m_buttonCount];

    b.pin = pin;
    b.activeLevel = activeLevel;
    b.callback = cb;

    pinMode(pin, mode);

    b.currentState = digitalRead(pin);
    b.lastState = b.currentState;

    b.pressStart = 0;
    b.lastChange = millis();

    m_buttonCount++;

    return true;
}


void ButtonManager::setBeep(uint8_t pin, const uint16_t *times, uint8_t count, uint16_t beepDuration) {
    m_beepPin = pin;
    m_beepTimes = times;
    m_beepCount = count;
    m_beepDuration = beepDuration;

    pinMode(m_beepPin, OUTPUT);
    digitalWrite(m_beepPin, HIGH);
    delay(1000);
    digitalWrite(m_beepPin, LOW);

}

void ButtonManager::update() {
    uint32_t now = millis();

    // spegne il beep se scaduto
    if (m_beepActive && (now - m_beepStart >= m_beepDuration)) {
        digitalWrite(m_beepPin, LOW);
        m_beepActive = false;
    }

    for (uint8_t i = 0; i < m_buttonCount; i++) {
        Button &b = buttons[i];

        bool reading = digitalRead(b.pin);

        if (reading != b.lastState) {
            b.lastState = reading;
            b.lastChange = now;
        }


        if ((now - b.lastChange) > m_debounceTime) {
            // gestione beep durante pressione
            if (b.currentState == b.activeLevel && m_beepPin != 255 && m_beepTimes) {

                uint32_t pressTime = now - b.pressStart;

                if (b.beepIndex < m_beepCount && pressTime >= m_beepTimes[b.beepIndex]) {

                    digitalWrite(m_beepPin, HIGH);
                    m_beepStart = now;
                    m_beepActive = true;

                    b.beepIndex++;
                }
            }
            if (reading != b.currentState) {
                b.currentState = reading;

                bool pressed = (reading == b.activeLevel);

                if (pressed) {
                    b.pressStart = now;
                    b.beepIndex = 0;
                }
                else {
                    uint32_t pressTime = now - b.pressStart;
                    if (pressTime >= m_minPressTime && b.callback) {
                        b.callback(b.pin, pressTime);
                    }
                }
            }
        }
    }
}