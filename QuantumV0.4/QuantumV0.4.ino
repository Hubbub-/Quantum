// Quantum - Splore 2016
// Hubbub
// Make fairy lights turn on and off

/*
controlPins are for the first transistors and control the direction of current.
ledPins are for the second transistors and control the voltage level for the LEDs.
The ledPins and controlPins have to be opposite sides of the H bridge.
For example, controlPin 0 can only be used with ledPin 0 (pin3), 
  ControlPin 1 with LEDpin 1, etc. 
Each H bridge controls one strand of LEDs with 2 controlPins and 2 ledPins .
*/

// analogue out pins: 3, 4, 5, 6, 9, 10, 20, 21, 22,
// control  out pins: 0, 1, 2, 7, 8, 11, 12, 14, 15,

int ledPins[] = {3,4,5,6,9,10,20,21,22}; 
int controlPins[] = {0,1,2,7,8,11,12,14,15}; 
int brightness[9]; // Stores the brightness for each LED strip
int fade[9];       // Stores how fast the leds change brightness
bool up[9];        // fading up or down
int pinCount = 9;     // how many pins are used for led transistors

int IRS = 23;         // pin for reading the IR Sensor
int sensIn;
int dist;
int distDiffMin = 30;   // minimum difference in distance to trigger

int prevDist;
int pprevDist;




void setup() {
  Serial.begin(115200);
  
  // set controlPins as outputs 
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    pinMode(controlPins[thisPin], OUTPUT);
  }
  
  // set ledPins as outputs
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    pinMode(ledPins[thisPin], OUTPUT);
    brightness[thisPin] = random(10,245);  // Initialise with random brightness
  }
  
  // Set input pin for pot
  pinMode(23,INPUT);
}


//---------------------------end of setup----------------------------

//--------------------------start of loop----------------------------

void loop() {
  // set even controlPins to high and even ledPins to voltage
  for (int thisPin = 0; thisPin < pinCount; thisPin+=2) {
    digitalWrite(controlPins[thisPin],HIGH);            
    analogWrite(ledPins[thisPin], brightness[thisPin]);
  }
  delay(10);

  // Set everything low again
  for (int thisPin = 0; thisPin < pinCount; thisPin+=2) {
    digitalWrite(controlPins[thisPin],LOW);            
    analogWrite(ledPins[thisPin], 0); 
  }
  delay(1);


  // set odd controlPins to high and odd ledPins to voltage
  for (int thisPin = 1; thisPin < pinCount; thisPin+=2) {
    digitalWrite(controlPins[thisPin],HIGH);
    analogWrite(ledPins[thisPin], brightness[thisPin]);
  }
  delay(10);

  // Set everything low again 
  for (int thisPin = 1; thisPin < pinCount; thisPin+=2) {
    digitalWrite(controlPins[thisPin],LOW);
    analogWrite(ledPins[thisPin], 0);
  }
  delay(1);


  // Read pot to get fade speed
//  Serial.print(analogRead(IRS));
//  Serial.print(" - ");
  sensIn = analogRead(IRS);
  Serial.print(sensIn);
  dist = (230000/sensIn)-150;  // Change analog signal to cms (aprox.)
  if(dist<1) dist = 1;
  if(dist>500) dist = 500;  // limit dist to 5m

  
  Serial.print(" - ");
  Serial.println(dist);

  distDiffMin = prevDist*0.2;

  if(prevDist < pprevDist-distDiffMin && prevDist > dist+distDiffMin){
    Serial.println("trig");
    for (int thisPin = 0; thisPin < pinCount; thisPin++) {
      fade[thisPin] = 120;
    }
  }
  else if(prevDist > pprevDist+distDiffMin && prevDist < dist-distDiffMin){
    Serial.println("trig");
    for (int thisPin = 0; thisPin < pinCount; thisPin++) {
      fade[thisPin] = 120;
    }
  }
  
//  if(dist < prevDist-50 || dist > prevDist+50){
//    if(dist < pprevDist-50 || dist > pprevDist+50){
//      Serial.print(dist);
//      Serial.print(" - ");
//      Serial.print(prevDist);
//      Serial.print(" - ");
//      Serial.print(pprevDist);
//      for (int thisPin = 0; thisPin < pinCount; thisPin++) {
//        fade[thisPin] = 100;
//        Serial.print("--");
//      }
//    }
//  }

  
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    if(fade[thisPin]>dist*0.01) fade[thisPin] = fade[thisPin]-1;  // lower fade speed
  }
  pprevDist = prevDist;
  prevDist = dist;
  

  
  fadeAll();
}


//------------------------------end of loop-----------------------------




void fadeStrip(int strip){
  

  int bright = brightness[strip]; //read brightness from the array
  
  if(bright < 0){  //When dim, change  fade direction to up
    bright = 0;
    up[strip] = true;
  }
  else if (bright > 255){ //When bright, change fade direction to down
    bright = 255;
    up[strip] = false;
  }

  if(up[strip]){  //When fading up, add fade amount
    bright += fade[strip];
  }
  else{
    bright -=fade[strip];  //When fading down, minus fade amount
  }
  
  brightness[strip] = bright;  //Save to the array
}



void fadeAll(){
  
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    fadeStrip(thisPin);
  }
}

