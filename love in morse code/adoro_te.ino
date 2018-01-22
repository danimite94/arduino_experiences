/*
  Arduino Starter Kit example
 Project 2  - Spaceship Interface
 
 This sketch is written to accompany Project 2 in the
 Arduino Starter Kit
 
 Parts required:
 1 green LED 
 2 red LEDs
 pushbutton
 10 kilohm resistor
 3 220 ohm resistors
 
 Created 13 September 2012
 by Scott Fitzgerald
 
 http://arduino.cc/starterKit

 This example code is part of the public domain 
*/

// Create a global variable to hold the 
// state of the switch. This variable is persistent 
// throughout the program. Whenever you refer to 
// switchState, you’re talking about the number it holds
int switchstate = 0;


void setup(){
  // declare the LED pins as outputs 
  
  
  pinMode(5,OUTPUT);

  // declare the switch pin as an input   
  pinMode(2,INPUT);
}

void loop(){

  // read the value of the switch
  // digitalRead() checks to see if there is voltage
  // on the pin or not  
  switchstate = digitalRead(2);

  // if the button is not pressed
  // turn on the green LED and off the red LEDs  
  if (switchstate == LOW) {
    
    digitalWrite(5, HIGH);  // turn the red LED on pin 5 off
    delay(250);
    
    digitalWrite(5, LOW);  // turn the red LED on pin 5 off
    delay(250);
    
    digitalWrite(5, HIGH);  // turn the red LED on pin 5 off
    delay(1000); //A
    
    digitalWrite(5, LOW);  // turn the red LED on pin 5 off
    delay(500);//espera
    
    digitalWrite(5, HIGH);  // turn the red LED on pin 5 off
    delay(1000);
    
    digitalWrite(5, LOW);  // turn the red LED on pin 5 off
    delay(250);
    
    digitalWrite(5, HIGH);  // turn the red LED on pin 5 off
    delay(250);
    
    digitalWrite(5, LOW);  // turn the red LED on pin 5 off
    delay(250);
    
    digitalWrite(5, HIGH);  // turn the red LED on pin 5 off
    delay(250);//D
    
    digitalWrite(5, LOW);  // turn the red LED on pin 5 off
    delay(500);//espera
    
    digitalWrite(5, HIGH);  // turn the red LED on pin 5 off
    delay(1000);
    
    digitalWrite(5, LOW);  // turn the red LED on pin 5 off
    delay(250);
    
    digitalWrite(5, HIGH);  // turn the red LED on pin 5 off
    delay(1000);
    
    digitalWrite(5, LOW);  // turn the red LED on pin 5 off
    delay(250);
    
    digitalWrite(5, HIGH);  // turn the red LED on pin 5 off
    delay(1000);//O
    
    digitalWrite(5, LOW);  // turn the red LED on pin 5 off
    delay(500);//espera
    
    digitalWrite(5, HIGH);  // turn the red LED on pin 5 off
    delay(250);
    
    digitalWrite(5, LOW);  // turn the red LED on pin 5 off
    delay(250);
    
    digitalWrite(5, HIGH);  // turn the red LED on pin 5 off
    delay(1000);
    
    digitalWrite(5, LOW);  // turn the red LED on pin 5 off
    delay(250);
    
    digitalWrite(5, HIGH);  // turn the red LED on pin 5 off
    delay(250);//R
    
    digitalWrite(5, LOW);  // turn the red LED on pin 5 off
    delay(500);//espera
    
    digitalWrite(5, HIGH);  // turn the red LED on pin 5 off
    delay(1000);
    
    digitalWrite(5, LOW);  // turn the red LED on pin 5 off
    delay(250);
    
    digitalWrite(5, HIGH);  // turn the red LED on pin 5 off
    delay(1000);
    
    digitalWrite(5, LOW);  // turn the red LED on pin 5 off
    delay(250);
    
    digitalWrite(5, HIGH);  // turn the red LED on pin 5 off
    delay(1000);//O
    
    digitalWrite(5, LOW);  // turn the red LED on pin 5 off
    delay(1000);//espera
    
    digitalWrite(5, HIGH);  // turn the red LED on pin 5 off
    delay(1000);//T
    
    digitalWrite(5, LOW);  // turn the red LED on pin 5 off
    delay(500);//espera
    
    digitalWrite(5, HIGH);  // turn the red LED on pin 5 off
    delay(250);//E
    
    digitalWrite(5, LOW);  // turn the red LED on pin 5 off
    delay(2000);
    
  }
  // this else is part of the above if() statement. 
  // if the switch is not LOW (the button is pressed)
  // turn off the green LED and blink alternatively the red LEDs 
  else {
    
    digitalWrite(5, LOW); // turn the red LED on pin 5 on
    // wait for a quarter second before changing the light
  }
}

