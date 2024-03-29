#include <PBEnhancer.h>

using Event = PBEnhancer::Event; //To define for abbreviation of identifiers. (It can be specified in the form like PBEnhancer::Event::SINGLE without writing this.)

PBEnhancer pb(2, INPUT_PULLUP, 200, 100); //Create a PBEnhancer object. The third and fourth arguments are optional.

const uint8_t LED = 13;

//The function to toggle LED on and off.
void changeLed() {
  static bool state = false;

  digitalWrite(LED, state);
  state = !state;
}

void setup() {
  Serial.begin(9600);

  pinMode(LED, OUTPUT);

  //Registering a callback function specifying the type of event.
  pb.registerCallback(Event::LONG, changeLed);

}

void loop() {
  //You can also confirm using the isOccurred() method without registering a callback function.
  if (pb.hasOccurred(Event::DOUBLE)) {
    Serial.println("Double clicked!");
  }

  pb.update(); //It is recommended to call this method at the earliest possible cycle to update events.

  delay(10); //Optional debounce time to prevent chattering.
}
