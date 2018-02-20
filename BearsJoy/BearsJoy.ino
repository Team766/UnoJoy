
#include "UnoJoy.h"

void setup(){
  setupPins();
  setupUnoJoy();
}

void loop(){
  // Always be getting fresh data
  dataForController_t controllerData = getControllerData();
  setControllerData(controllerData);
}

void setupPins(void){
  // Set all the digital pins as inputs
  // with the pull-up enabled, except for the 
  // two serial line pins
  for (int i = 2; i <= 13; i++){
    pinMode(i, INPUT);
    digitalWrite(i, HIGH);
  }
  pinMode(A0, INPUT);
  digitalWrite(A0, HIGH);
  pinMode(A1, INPUT);
  digitalWrite(A1, HIGH);
  pinMode(A2, INPUT);
  digitalWrite(A2, HIGH);
  pinMode(A3, INPUT);
  digitalWrite(A3, HIGH);
  pinMode(A4, INPUT);
  digitalWrite(A4, HIGH);
  pinMode(A5, INPUT);
  digitalWrite(A5, HIGH);
}

bool readDigitalFromAnalog(int port){\
  //  Since analogRead(pin) returns a 10 bit value,
  //  we need to perform a bit shift operation to
  //  lose the 2 least significant bits and get an
  //  8 bit number that we can use 
  return analogRead(port) > 512;
}

dataForController_t getControllerData(void){
  // Since our buttons are all held high and
  //   pulled low when pressed, we negate the
  //   readings from the pins.
  // If your controller doesn't rely on the
  //   Arduino's pullup resistors
  bool negate_all = true;
  
  // Set up a place for our controller data
  //  Use the getBlankDataForController() function, since
  //  just declaring a fresh dataForController_t tends
  //  to get you one filled with junk from other, random
  //  values that were in those memory locations before
  dataForController_t controllerData = getBlankDataForController();

  // Comments indicate the resultant button number in the Windows gamepad driver
  controllerData.squareOn = negate_all ^ readDigitalFromAnalog(A3); // 1
  controllerData.crossOn = negate_all ^ digitalRead(2); // 2
  controllerData.circleOn = negate_all ^ digitalRead(3); // 3
  controllerData.triangleOn = negate_all ^ digitalRead(4); // 4
  controllerData.l1On = negate_all ^ digitalRead(5); // 5
  controllerData.r1On = negate_all ^ digitalRead(6); // 6
  controllerData.l2On = negate_all ^ digitalRead(7); // 7
  controllerData.r2On = negate_all ^ digitalRead(8); // 8
  controllerData.selectOn = negate_all ^ digitalRead(9); // 9
  controllerData.startOn = negate_all ^ digitalRead(10); // 10
  controllerData.l3On = negate_all ^ digitalRead(11); // 11
  controllerData.r3On = negate_all ^ digitalRead(12); // 12
  controllerData.homeOn = negate_all ^ digitalRead(13); // 13
  controllerData.dpadLeftOn = negate_all ^ readDigitalFromAnalog(A4); // D-pad LEFT
  controllerData.dpadDownOn = negate_all ^ readDigitalFromAnalog(A5); // D-pad DOWN

  // 2018 Game Specific code
  // Analog input A0 is used to enable (or disable) the climber switches plugged
  // into A1 and A2. (i.e. If A0 (guard_button) must be switched on, for A1 or
  // A2 to return true.
  bool guard_button_pressed = negate_all ^ readDigitalFromAnalog(A0); // Not mapped to Windows button
  bool guarded_button_1 = guard_button_pressed && (negate_all ^ readDigitalFromAnalog(A1));
  bool guarded_button_2 = guard_button_pressed && (negate_all ^ readDigitalFromAnalog(A2));
  controllerData.dpadUpOn = guarded_button_1; // D-pad UP
  controllerData.dpadRightOn = guarded_button_2; // D-pad RIGHT

  // Since there are no analog sticks attached to the Arduino, we don't populate
  // any of the stick inputs.
  /*controllerData.leftStickX = analogRead(A1) >> 2;*/
  /*controllerData.leftStickY = analogRead(A1) >> 2;*/
  /*controllerData.rightStickX = analogRead(A2) >> 2;*/
  /*controllerData.rightStickY = analogRead(A3) >> 2;*/

  // And return the data!
  return controllerData;
}
