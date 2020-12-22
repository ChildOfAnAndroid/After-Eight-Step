int wait = 5;

//POTENTIOMETERS
 const int potsA = A0;
 const int potsB = A1;
 const int inhibitMux = 7;
 
//ROTARY ENCODER
 //Value
 const int encoderA = 2; //CLK on KY-040 breakout board
 const int encoderB = 3; //DT on KY-040 breakout board
 
 volatile int lastEncoded = 0;
 volatile long encoderValue = 0;

 long lastencoderValue = 0;

 int lastMSB = 0; //MSB is most significant bit
 int lastLSB = 0; //LSB is least significant bit

 //Button
  const int encoderButton = 4; //SW on KY-040 breakout board

  int buttonState;             // the current reading from the input pin
  int lastButtonState = LOW;   // the previous reading from the input pin

  // the following variables are unsigned long's because the time, measured in miliseconds,
  // will quickly become a bigger number than can be stored in an int.
  unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
  unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

  //Button Setting
   int setting = 0;
  
   int setting1;
   int setting2;
   int setting3;
  
//LEDs
 const int ledAPins = A2;
 const int ledBPins = A3;
 // const int ledCPins = A4;

 int incomingByte; //Read byte from Max

//LCD SCREEN
 #include <LiquidCrystal.h>
 const int registerSelect = A5;
 const int enable = 5;
 const int lcdD4 = 6;
 const int lcdD5 = 11;
 const int lcdD6 = 12;
 const int lcdD7 = 13;
 LiquidCrystal lcd(registerSelect, enable, lcdD4, lcdD5, lcdD6, lcdD7);

 //Menu
  char *lcdMenu[] = {0, "FORWARD CHANCE", "REVERSE CHANCE", "LONG NOTE CHANCE"};

void setup() 
{
  Serial.begin(9600);
 //POTENTIOMETERS
  pinMode(inhibitMux,OUTPUT);
  digitalWrite(inhibitMux,LOW);
  DDRB = 0b00000111;  //PORT 8,9,10 as output
  PORTB = 0b00000000; //PORT 8,9,10 set to low
  pinMode(potsA,INPUT);
  pinMode(potsB,INPUT);
 //ROTARY ENCODER
  pinMode(encoderA,INPUT_PULLUP);
  pinMode(encoderB,INPUT_PULLUP);
  //call updateEncoder() when high/low change seen
  //on interrupt 0 (pin 2), or interrupt 1 (pin 3)
  attachInterrupt(0, updateEncoder, CHANGE);
  attachInterrupt(1, updateEncoder, CHANGE);
 //LEDs
  pinMode(ledAPins, OUTPUT);
  pinMode(ledBPins, OUTPUT);
 //LCD SCREEN
  lcd.begin(16, 2);
  lcd.print("Markov Sequencer");
  lcd.setCursor(0,1);
  lcd.print("Charis Cat 2020");

 //TO MAX
  establishContact();  // send a byte to establish contact until receiver responds
}

void loop() 
{
  Serial.flush();

  // read the state of the switch into a local variable:
  int reading = digitalRead(encoderButton);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
 
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;
     
      // Increment the setting number by 1
      if (buttonState == HIGH) {
          setting++;
      }
    }
    }

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;

  // checking the modulo of the button push counter.
  // the modulo function gives you the remainder of
  // the division of two numbers:
  if (setting % 4 == 0) {
    setting = 1;
  } else {
    setting = setting;
  }
  
 //FROM MAX
  // see if there's incoming serial data:
  if (Serial.available() > 0) {
    // read the oldest byte in the serial buffer:
    incomingByte = Serial.read();
    // if it's a capital A, turn on LED 1:
    if (incomingByte == 'A') {
      PORTB=0b00000000; //Data flow - X0 (current step)
      digitalWrite(ledAPins, HIGH);
    }
    if (incomingByte == 'B') {
      PORTB=0b00000001; //Data flow - X1 (current step)
      digitalWrite(ledAPins, HIGH);
    }
    if (incomingByte == 'C') {
      PORTB=0b00000010;  //Data flow - X2 (current)
      digitalWrite(ledAPins, HIGH); 
    }
    if (incomingByte == 'D') {
      PORTB=0b00000011;  //Data flow - X3 (current step)
      digitalWrite(ledAPins, HIGH);
    }
    if (incomingByte == 'E') {
      PORTB=0b00000100;  //Data flow - X4 (current step)
      digitalWrite(ledAPins, HIGH);
    }
    if (incomingByte == 'F') {
      PORTB=0b00000101;  //Data flow - X5 (current step)
      digitalWrite(ledAPins, HIGH);
    }
    if (incomingByte == 'G') {
      PORTB=0b00000110;  //Data flow - X6 (current step)
      digitalWrite(ledAPins, HIGH);
    }
    if (incomingByte == 'H') {
      PORTB=0b00000111;  //Data flow - X7 (current step)
      digitalWrite(ledAPins, HIGH);
    }
    if (incomingByte == 'I') {
      PORTB=0b00000000; //Data flow - X0 (current step)
      digitalWrite(ledBPins, HIGH);
    }
    if (incomingByte == 'J') {
      PORTB=0b00000001; //Data flow - X1 (current step)
      digitalWrite(ledBPins, HIGH);
    }
    if (incomingByte == 'K') {
      PORTB=0b00000010;  //Data flow - X2 (current)
      digitalWrite(ledBPins, HIGH); 
    }
    if (incomingByte == 'L') {
      PORTB=0b00000011;  //Data flow - X3 (current step)
      digitalWrite(ledBPins, HIGH);
    }
    if (incomingByte == 'M') {
      PORTB=0b00000100;  //Data flow - X4 (current step)
      digitalWrite(ledBPins, HIGH);
    }
    if (incomingByte == 'N') {
      PORTB=0b00000101;  //Data flow - X5 (current step)
      digitalWrite(ledBPins, HIGH);
    }
    if (incomingByte == 'O') {
      PORTB=0b00000110;  //Data flow - X6 (current step)
      digitalWrite(ledBPins, HIGH);
    }
    else if (incomingByte == 'P') {
      PORTB=0b00000111;  //Data flow - X7 (current step)
      digitalWrite(ledBPins, HIGH);
    }
#if 0
    if (incomingByte == 'Q') {
      PORTB=0b00000000; //Data flow - X0 (current step)
      digitalWrite(ledCPins, HIGH);
    }
    if (incomingByte == 'R') {
      PORTB=0b00000001; //Data flow - X1 (current step)
      digitalWrite(ledCPins, HIGH);
    }
    if (incomingByte == 'S') {
      PORTB=0b00000010;  //Data flow - X2 (current)
      digitalWrite(ledCPins, HIGH); 
    }
    if (incomingByte == 'T') {
      PORTB=0b00000011;  //Data flow - X3 (current step)
      digitalWrite(ledCPins, HIGH);
    }
    if (incomingByte == 'U') {
      PORTB=0b00000100;  //Data flow - X4 (current step)
      digitalWrite(ledCPins, HIGH);
    }
    if (incomingByte == 'V') {
      PORTB=0b00000101;  //Data flow - X5 (current step)
      digitalWrite(ledCPins, HIGH);
    }
    if (incomingByte == 'W') {
      PORTB=0b00000110;  //Data flow - X6 (current step)
      digitalWrite(ledCPins, HIGH);
    }
    else if (incomingByte == 'X') {
      PORTB=0b00000111;  //Data flow - X7 (current step)
      digitalWrite(ledCPins, HIGH);
    }
#endif

  delay(wait);
  digitalWrite(ledAPins, LOW);
  digitalWrite(ledBPins, LOW);
//  digitalWrite(ledCPins, LOW);
  
  
 //TO MAX
  //#if 0
  PORTB=0b00000000; //Data flow - X0
  int step1A = analogRead(potsA); //read data from X0 potsA
  Serial.print(step1A);
  int step1B = analogRead(potsB); //read data from X0 potsB
  Serial.print(" ");
  Serial.print(step1B);
//  delay(wait);
 
  PORTB=0b00000001;  //Data flow - X1
  int step2A = analogRead(potsA); //read data from X1 potsA
  Serial.print(" ");
  Serial.print(step2A);
  int step2B = analogRead(potsB); //read data from X1 potsB
  Serial.print(" ");
  Serial.print(step2B);
//  delay(wait);
  
  PORTB=0b00000010;  //Data flow - X2
  int step3A = analogRead(potsA); //read data from X2 potsA
  Serial.print(" ");
  Serial.print(step3A);
  int step3B = analogRead(potsB); //read data from X2 potsB
  Serial.print(" ");
  Serial.print(step3B);
//  delay(wait);
  
  PORTB=0b00000011;  //Data flow - X3
  int step4A = analogRead(potsA); //read data from X3 potsA
  Serial.print(" ");
  Serial.print(step4A);
  int step4B = analogRead(potsB); //read data from X3 potsB
  Serial.print(" ");
  Serial.print(step4B);
//  delay(wait);

  PORTB=0b00000100;  //Data flow - X4
  int step5A = analogRead(potsA); //read data from X4 potsA
  Serial.print(" ");
  Serial.print(step5A);
  int step5B = analogRead(potsB); //read data from X4 potsB
  Serial.print(" ");
  Serial.print(step5B);
//  delay(wait);

  PORTB=0b00000101;  //Data flow - X5
  int step6A = analogRead(potsA); //read data from X5 potsA
  Serial.print(" ");
  Serial.print(step6A);
  int step6B = analogRead(potsB); //read data from X5 potsB
  Serial.print(" ");
  Serial.print(step6B);
//  delay(wait);

  PORTB=0b00000110;  //Data flow - X6
  int step7A = analogRead(potsA); //read data from X7 potsA
  Serial.print(" ");
  Serial.print(step7A);
  int step7B = analogRead(potsB); //read data from X7 potsB
  Serial.print(" ");
  Serial.print(step7B);
//  delay(wait);

  PORTB=0b00000111;  //Data flow - X7
  int step8A = analogRead(potsA); //read data from X8 potsA
  Serial.print(" ");
  Serial.print(step8A);
  int step8B = analogRead(potsB); //read data from X8 potsB
  Serial.print(" ");
  Serial.print(step8B);
//  delay(wait);
  
  digitalWrite(inhibitMux,HIGH);  // Inhibit / stop all data
  delay(wait);  
  digitalWrite(inhibitMux,LOW);  // Inhibit /  let data flow

  Serial.print(" ");
  int encoderButton = digitalRead(encoderButton);
  int scaledEncoder1 = (((setting1) - 2) / 4);
  Serial.print(scaledEncoder1);
  Serial.print(" ");
  int scaledEncoder2 = (((setting2) - 2) / 4);
  Serial.print(scaledEncoder2);
  Serial.print(" ");
  int scaledEncoder3 = (((setting3) - 2) / 4);
  Serial.print(scaledEncoder3);
  //Serial.print(encoderValue); //Rotary encoder value
  Serial.print(" ");
  //Serial.println(encoderButton); //Rotary encoder button
  Serial.println(setting); //Encoder button loop of 3 

//  if(setting > 0){
//  lcd.clear();
//  lcd.setCursor(0,0);
//  lcd.print(lcdMenu[setting]);
//  }
  
  if(setting == 1){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(lcdMenu[setting]);
    lcd.setCursor(0,1);
    lcd.print(scaledEncoder1);
  }

  if(setting == 2){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(lcdMenu[setting]);
    lcd.setCursor(0,1);
    lcd.print(scaledEncoder2);
  }

  if(setting == 3){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(lcdMenu[setting]);
    lcd.setCursor(0,1);
    lcd.print(scaledEncoder3);
  }
  }
}

void updateEncoder(){
  int MSB = digitalRead(encoderA); //MSB is most significant bit
  int LSB = digitalRead(encoderB); //LSB is least significant bit

  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number 
  int sum = (lastEncoded << 2) | encoded; //adding it to the previous encoded value 
  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue ++; //these values mean the encoder is turning clockwise
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue --; //these values mean the encoder is turning counter clockwise
  switch (setting) {
    case 1 :
      setting1 = encoderValue;
      break;
    case 2 :
      setting2 = encoderValue;
      break;
    case 3 :
      setting3 = encoderValue;
      break;
  }

  lastEncoded = encoded; //store this value for next time 
}

void establishContact() {
  while (Serial.available() < 0) {
    Serial.println("0,0,0");   // send an initial string
    delay(wait);
  }
}
