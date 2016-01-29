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
int pinCount = 9;     // how many pins are used for led transistors

int IRS = 23;         // pin for reading the IR Sensor
int sensIn = 5;
int dist = 5;

int fade = 5;    // how fast to fade the LEDs
bool up[9];      // fading up or down (array of bools)

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
  sensIn = analogRead(IRS);
  dist = map(sensIn, 0, 1023, 100, 550);
  fade = dist*0.2;
//  fade = analogRead(pot)*0.2;
  Serial.println(fade);


  // Fade every LED
  // This should be put into a seperate function
  for(int thisLED = 0; thisLED<pinCount; thisLED++){
    int bright = brightness[thisLED]; //read brightness from the array
    
    if(bright < 0){  //When dim, change  fade direction to up
      bright = 0;
      up[thisLED] = true;
    }
    else if (bright > 255){ //When bright, change fade direction to down
      bright = 255;
      up[thisLED] = false;
    }

    if(up[thisLED]){  //When fading up, add fade amount
      bright += fade;
    }

    else{
      bright -=fade;  //When fading down, minus fade amount
    }

    brightness[thisLED] = bright;  //Save to the array
    
  } 

}

