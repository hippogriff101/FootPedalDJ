#include "usb_names.h"

#define MIDI_NAME   {'B','L','U','E','P','R','I','N','T'}
#define MIDI_NAME_LEN  9

#define MANUFACTURER_NAME  {'F','r','e','d','d','i','e'}
#define MANUFACTURER_NAME_LEN 7

struct usb_string_descriptor_struct usb_string_product_name = {
  2 + MIDI_NAME_LEN * 2,
  3,
  MIDI_NAME
};

struct usb_string_descriptor_struct usb_string_manufacturer_name = {
  2 + MANUFACTURER_NAME_LEN * 2,
  3,
  MANUFACTURER_NAME
};
