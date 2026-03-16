#include "lnButtonManager.h"

#define BUZZER_pin            23  // OUTPUT

#define BUTTON_01_pin         18  // INPUT
#define BUTTON_02_pin         19  // INPUT

ButtonManager buttons;
// -----------------------------------------------------------
// --- se voglio un beep ogni secondo e considerando
// --- il riflesso ed il tempo del beep (200ms)
// --- devo mettere 800, 1800, 2800 ... o giù di lì
// -----------------------------------------------------------
uint16_t beepTimes[] = {800, 1800, 2800};


void buttonHandler(uint8_t pin, uint32_t pressTime)  {
    Serial.print("Pin: ");
    Serial.print(pin);
    Serial.print(" time: ");
    Serial.println(pressTime);

    if (pressTime > 1000)
        Serial.println("LONG PRESS");
}

void setup()
{
    Serial.begin(115200);
    /* --- tipologie di possibilità
    buttons.addButton(18, buttonHandler, LOW, INPUT_PULLUP);    // Pulsante con pull-up interno (classico)
    buttons.addButton(19, buttonHandler, HIGH, INPUT_PULLDOWN); // Pulsante con pull-down
    buttons.addButton(21, buttonHandler, HIGH, INPUT);          // Pulsante con resistenza esterna
    */

    buttons.addButton(BUTTON_01_pin, buttonHandler, LOW, INPUT_PULLUP);    // Pulsante con pull-up interno (classico)
    buttons.addButton(BUTTON_02_pin, buttonHandler, LOW, INPUT_PULLUP);    // Pulsante con pull-up interno (classico)
    buttons.setBeep(BUZZER_pin, beepTimes, 3, 200);

}

void loop() {
    buttons.update();
}