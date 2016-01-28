// Quantum - Splore 2016
// Make fairy lights turn on and off


int ledPins[] = {3,4,5,6,9,10,20,21,22};
int brightness[9];
int controlPins[] = {0,1,0,1,0,1,0,1,0};
// analogue out pins: 3, 4, 5, 6, 9, 10, 20, 21, 22,
// controll out pins: 0, 1, 0, 1, 0,  1,  0,  1,  0,
int pinCount = 9;     // how many pins are used for led transistors
int pot = 23;         // pin for reading the pot

int fade = 5;    // how many points to fade the LED by
bool up[9];  // fading up

void setup() {
  Serial.begin(115200);
  // set output pins
  pinMode(0,OUTPUT);
  pinMode(1,OUTPUT);
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    pinMode(ledPins[thisPin], OUTPUT);
    brightness[thisPin] = random(10,245);
  }
  // Set input pin for pot
  pinMode(23,INPUT);
}

void loop() {

  digitalWrite(0,HIGH);
  for (int thisPin = 0; thisPin < pinCount; thisPin+=2) {
    if (controlPins[thisPin] == 0){
      analogWrite(ledPins[thisPin], brightness[thisPin]);
    }
  }
  delay(10);
  
  digitalWrite(0,LOW);
  for (int thisPin = 0; thisPin < pinCount; thisPin+=2) {
    if (controlPins[thisPin] == 0){
      analogWrite(ledPins[thisPin], 0);
    }
  }
  delay(1);

  digitalWrite(1,HIGH);
  for (int thisPin = 1; thisPin < pinCount; thisPin+=2) {
    if (controlPins[thisPin] == 1){
      analogWrite(ledPins[thisPin], brightness[thisPin]);
    }
  }
  delay(10);

  digitalWrite(1,LOW);
  for (int thisPin = 1; thisPin < pinCount; thisPin+=2) {
    if (controlPins[thisPin] == 1){
      analogWrite(ledPins[thisPin], 0);
    }
  }



  fade = analogRead(pot)*0.2;
  Serial.println(fade);
  
  for(int thisLED = 0; thisLED<pinCount; thisLED++){
    int bright = brightness[thisLED];
    if(bright < 0){
      bright = 0;
      up[thisLED] = true;
    }
    else if (bright > 255){
      bright = 255;
      up[thisLED] = false;
    }

    if(up[thisLED]){
      bright += fade;
    }

    else{
      bright -=fade;
    }

    brightness[thisLED] = bright;
    
  }

  
//  
//  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
////    analogWrite(ledPins[thisPin],brightness);
//    setBright(thisPin,brightness);
//  }
//  
//
//  // change the brightness for next time through the loop:
//  brightness = brightness + fadeAmount;
//
//  // reverse the direction of the fading at the ends of the fade:
//  if (brightness < 0) {
//    brightness = 0;
//    fadeAmount = fade;
//  }
//  else if (brightness > 255){
//    brightness = 255;
//    fadeAmount = -fade;
//  }
//
//  Serial.println(brightness);
//  Serial.println(fadeAmount);
//  // wait for 30 milliseconds to see the dimming effect
//  delay(30);
//  

}
//
//void setBright(int p, int b){
//  int firstPin = p*2;
//  int secondPin = firstPin+1;
//  analogWrite(ledPins[firstPin],255);
//  analogWrite(ledPins[secondPin],b);
//}

