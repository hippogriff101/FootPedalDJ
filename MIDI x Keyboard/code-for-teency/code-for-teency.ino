/******************************
   EDLS-FS v1.1 (modified)
   Adds: Keyboard mode + macros
 ******************************/

#include <Bounce.h>
#include <usb_keyboard.h>   // Teensy keyboard support

//MIDI Channel
#define MIDI_CHANNEL 1

//Switch CC Numbers
#define LOOP1 38
#define LOOP2 39
#define LOOP4 40
#define LOOP8 41

//Pin Numbers
#define L1_PIN 0
#define L2_PIN 1
#define L4_PIN 2
#define L8_PIN 3

#define SWITCHES 4
#define VELOCITY 127
#define BOUNCE_TIME 15

// Globals
bool keyboardMode = false;

const int DIGITAL_SWITCHES[SWITCHES] = {L1_PIN, L2_PIN, L4_PIN, L8_PIN};
const int CC_NUMBER[SWITCHES] = {LOOP1, LOOP2, LOOP4, LOOP8};

Bounce digital[] = {
  Bounce(DIGITAL_SWITCHES[0], BOUNCE_TIME),
  Bounce(DIGITAL_SWITCHES[1], BOUNCE_TIME),
  Bounce(DIGITAL_SWITCHES[2], BOUNCE_TIME),
  Bounce(DIGITAL_SWITCHES[3], BOUNCE_TIME),
};

// ----------- MACROS -------------
void macro1() {
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press(KEY_1);
  delay(30);
  Keyboard.releaseAll();
}

void macro2() {
  Keyboard.print("Hello World!");
}

void macro3() {
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press(KEY_DELETE);
  delay(30);
  Keyboard.releaseAll();
}
// --------------------------------


// Setup
void setup() {
  for (int i = 0; i < SWITCHES; i++) {
    pinMode(DIGITAL_SWITCHES[i], INPUT_PULLUP);
  }
}


// Main loop
void loop() {
  readSwitches();
  while (usbMIDI.read()) {}
}


// Read Buttons
void readSwitches() {
  static byte input = 0;

  digital[input].update();

  if (digital[input].fallingEdge()) {

    // Button 4 → toggle keyboard mode
    if (input == 3) {  // Loop 8 button
      keyboardMode = !keyboardMode;
      return;
    }

    // If in keyboard mode → use macros
    if (keyboardMode) {
      if (input == 0) macro1();
      if (input == 1) macro2();
      if (input == 2) macro3();
    }

    // If NOT in keyboard mode → normal MIDI behavior
    else {
      usbMIDI.sendControlChange(CC_NUMBER[input], VELOCITY, MIDI_CHANNEL);
    }
  }
  else if (digital[input].risingEdge()) {
    // Only send MIDI release if not in keyboard mode
    if (!keyboardMode) {
      usbMIDI.sendControlChange(CC_NUMBER[input], 0, MIDI_CHANNEL);
    }
  }

  input++;
  if (input >= SWITCHES) input = 0;
}
