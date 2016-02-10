// Quantum - Splore 2016
// Hubbub
// Make fairy lights turn on and off

/*
controlPins are for the first transistors and control the direction of current.
pins are for the second transistors and control the voltage level for the LEDs.
The pins and controlPins have to be opposite sides of the H bridge.
For example, controlPin 0 can only be used with ledPin 0 (pin3), 
  ControlPin 1 with LEDpin 1, etc. 
Each H bridge controls one strand of LEDs with 2 controlPins and 2 pins .
*/

// analogue out pins: 3, 4, 5, 6, 9, 10, 20, 21, 22,
// control  out pins: 0, 1, 2, 7, 8, 11, 12, 14, 15,

int pins[] = {3,4,5,6,9,10,20,21}; 

float brightness[8]; // Stores the brightness for each LED strip
float fade[8];       // Stores how fast the leds change brightness
bool up[8];          // fading up or down
int pinCount = 8;     // how many pins are used for led transistors

int IRS = 23;         // pin for reading the IR Sensor (can also add 22)
int sensIn;
int dist;
int distDiffMin = 30;   // minimum difference in distance to trigger

int prevDist;
int pprevDist;




void setup() {
  Serial.begin(115200);
  
  
  // set pins as outputs
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    pinMode(pins[thisPin], OUTPUT);
    brightness[thisPin] = random(10,245);  // Initialise with random brightness
  }
  
  // Set input pin for pot
  pinMode(23,INPUT);
}


//---------------------------end of setup----------------------------

//--------------------------start of loop----------------------------

void loop() {
  // set even pins to voltage
  for (int thisPin = 0; thisPin < pinCount; thisPin+=2) {
    analogWrite(pins[thisPin], brightness[thisPin]);
  }
  delay(8);

  // Set everything low again
  for (int thisPin = 0; thisPin < pinCount; thisPin+=2) {
    analogWrite(pins[thisPin], 0); 
  }



  // set odd pins to voltage
  for (int thisPin = 1; thisPin < pinCount; thisPin+=2) {
    analogWrite(pins[thisPin], brightness[thisPin]);
  }
  delay(8);

  // Set everything low again 
  for (int thisPin = 1; thisPin < pinCount; thisPin+=2) {
    analogWrite(pins[thisPin], 0);
  }
  


  // Read pot to get fade speed
//  Serial.print(analogRead(IRS));
//  Serial.print(" - ");
  sensIn = analogRead(IRS);
  Serial.print(sensIn);
  dist = (230000/sensIn)-150;  // Change analog signal to cms (aprox.)
  if(dist<1) dist = 1;
  if(dist>500) dist = 500;  // limit dist to 5m

  
  Serial.print(" - ");
  Serial.print(dist);

  distDiffMin = prevDist*0.2;

  if(prevDist < pprevDist-distDiffMin && prevDist > dist+distDiffMin){
    Serial.println("trig");
    for (int thisPin = 0; thisPin < pinCount; thisPin++) {
      fade[thisPin] = 120;
    }
  }
  else if(prevDist > pprevDist+distDiffMin && prevDist < dist-distDiffMin){
    Serial.println("--------trigger");
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
    // Move fade speed towards the closeness
    if(fade[thisPin]>(500-dist)*0.05+1) fade[thisPin] = fade[thisPin]-(thisPin*0.1+0.5); 
    else if(fade[thisPin]<(500-dist)*0.05+1) fade[thisPin] = fade[thisPin]+0.8; 

  }

  
  
  pprevDist = prevDist;
  prevDist = dist;
  

  
  fadeAll();
  if(millis() % 60000 <100) blinkSeq();

  Serial.println(" ");
}


//------------------------------end of loop-----------------------------




void fadeStrip(int pin){
  

  int bright = brightness[pin]; //read brightness from the array
  
  if(bright < 0){  //When dim, change  fade direction to up
    bright = 0;
    up[pin] = true;
  }
  else if (bright > 255){ //When bright, change fade direction to down
    bright = 255;
    up[pin] = false;
  }

  if(up[pin]){  //When fading up, add fade amount
    bright += fade[pin];
  }
  else{
    bright -=fade[pin];  //When fading down, minus fade amount
  }
  
  brightness[pin] = bright+(random(-1,2));  //Save to the array with some randomness
}



void fadeAll(){
  
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    fadeStrip(thisPin);
  }
}

void blinkSeq(){
  Serial.println("Wait for it...");
  // wait for 2 seconds
  delay(2000);

  // run sequence
  for (int seqStep = 0; seqStep < pinCount; seqStep++){
    // Make sure all are off
    for (int thisPin = 0; thisPin < pinCount; thisPin++){
      analogWrite(pins[thisPin], 0);
    }
    //turn on one pin
    analogWrite(pins[seqStep], 255);
    delay(100);
    Serial.print("BAM! ");
  }
  delay(1000);
  // set all brightness levels to 0
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    brightness[thisPin] = 0;
  }
  
}

