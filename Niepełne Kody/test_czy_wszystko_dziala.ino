#include <Servo.h>

Servo Base; //Pins
Servo Alfa;
Servo Beta;
Servo Grip;
const int LRpin = A0;
const int UDpin = A1;
const int trigPin = 5;
const int echoPin = 6;

bool AutomaticMode = true; //Mode

int posBase = 90;    //Manual
int posAlfa = 90;
int posBeta = 90;
int LR;
int UD;
int deadzone = 1;

const int buttonPinWhite = 12;  // the pin that the pushbutton is attached to
const int ledPinWhite = 13;    // the pin that the LED is attached to

const int buttonPinRed = 7;  // the pin that the pushbutton is attached to
const int ledPinRed = 8;    // the pin that the LED is attached to

int buttonStateWhite = 0;        // current state of the button
int lastButtonStateWhite = 0;    // previous state of the button
int StateWhite = 0;             // Binary state

int buttonStateRed = 0;        // current state of the button
int lastButtonStateRed = 0;    // previous state of the button
int StateRed = 0;             // Binary state

bool AlfaTime = true;

long duration;  //Automatic
int distance;

bool MoveSide = false;
bool objStart = false;
int posStart = 0;
int Accumulator = 0;

int angles[13][2] = {  

65, 135,
70, 137,
78, 140,
83, 142,
90, 143,

100, 149,
105, 151,
109, 154,
123, 160,
145, 170,

151, 170,
164, 176,
172, 180


};

void move(Servo &servo, int val)
{
  if(servo.read() == val)
    return;
  // Serial.println("read != val");
  // Serial.println(servo.read());
  // Serial.println(val);
  // Serial.println("-------");
  if(servo.read() < val)
  {
    for(int i=servo.read()+1; i<=val; i++)
    {
      servo.write(i);
      // Serial.println(i);
      delay(10);
    }
  }
  else
  {
    for(int i=servo.read()-1; i>=val; i--)
    {
      servo.write(i);
      // Serial.println(i);
      delay(10);
    }
  }
}

void resetValues(){
  posBase = 90;
  posAlfa = 90;
  posBeta = 90;
  deadzone = 1;
  buttonStateWhite = 0;
  lastButtonStateWhite = 0;
  StateWhite = 0;
  buttonStateRed = 0;
  lastButtonStateRed = 0;
  StateRed = 0;
  AlfaTime = true;
  MoveSide = false; 
  objStart = false;
  posStart = 0;
  distance = 1000;
}

void Automatic(){
  int curr = Base.read();

  //DISTANCE
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = round(duration * 0.034 / 2);
  if (distance > 4 && distance < 18){
    
    Serial.print("Distance: ");
    Serial.println(distance);
    
    if(!objStart){
      objStart = true;
      posStart = curr;
      Serial.println("START");
    }
  }
  else{
    if(objStart){
      Serial.println("FINISH");
      objStart = false;
      int MiddleOfObj;
      MiddleOfObj = (posStart + curr)/2;
      move(Base, MiddleOfObj);
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      duration = pulseIn(echoPin, HIGH);
      distance = round(duration * 0.034 / 2);
      
      Serial.print("Distance 2: ");
      Serial.println(distance);

      if(!(distance>4&&distance<18)){return;}
      move(Beta, angles[distance-5][0]);
      move(Alfa, angles[distance-5][1]);
      move(Grip, 120);

      /*Alfa.write(angles[distance_int-5][1]);
      Beta.write(angles[distance_int-5][0]);
      Grip.write(120);*/
      delay(2500);
      move(Grip, 0);
      /*for(int i=119; i>=0; i--)
      {
        Grip.write(i);
        delay(10);
      }*/
      delay(2000);
      move(Alfa, 120);
      delay(500);
      move(Beta, 90);
      //Alfa.write(90);
      //Beta.write(90);
      delay(1000);
      move(Base, 180);
      //Base.write(180);
      delay(1000);
      move(Grip, 120);
      //Grip.write(120);
      delay(1000);
      move(Base, 0);
      MoveSide = false;
      delay(20);
      return;
    }
  }

  //MOVEMENT
  if (!MoveSide){
    if(curr > 0){
      Base.write(curr - 1);
    }
    else{
      MoveSide = true;
    }
  }
  else{
      if(curr < 180){
      Base.write(curr + 1);
    }
    else{
      MoveSide = false;
    }
  }

  delay(20);
}

void Manual(){
  UD = analogRead(UDpin);
  UD -= 515;
  UD = UD / 200;
  if (abs(UD)>=deadzone){
    posBase += UD;
  }
  if (posBase > 180) {
    posBase = 180;
  }
  if (posBase < 0) {
    posBase = 0;
  }

  if(AlfaTime){
    LR = analogRead(LRpin);

    LR -= 515;
    LR = LR / 200;
    if (abs(LR)>=deadzone){
      posAlfa += LR;
    }
    if (posAlfa > 179) {
      posAlfa = 179;
    }
    if (posAlfa < 50) {
      posAlfa = 50;
    }
  }
  else{
    LR = analogRead(LRpin);

    LR -= 515;
    LR = LR / 200;
    if (abs(LR)>=deadzone){
      posBeta += LR;
    }
    if (posBeta > 180) {
      posBeta = 180;
    }
    if (posBeta < 58) {
      posBeta = 58;
    }
  }

  // Serial.print("Beta");
  // Serial.println(Beta.read());

  // Serial.print("Alfa");
  // Serial.println(Alfa.read());
  
  // Serial.print("Base");
  // Serial.println(Base.read());

  

  Base.write(int(posBase));
  Alfa.write(int(posAlfa));
  Beta.write(int(posBeta));

  if(StateWhite == 2){
    if (digitalRead(ledPinWhite) == HIGH){
      digitalWrite(ledPinWhite,LOW);
      AlfaTime = true;
    }
    else{
      digitalWrite(ledPinWhite,HIGH);
      AlfaTime = false;
    }
  }

  if(StateRed == 2){
    if (digitalRead(ledPinRed) == LOW){
      digitalWrite(ledPinRed,HIGH);
      for(int i = 180; i>=0; i--){
        Grip.write(i);
        delay(10);
      }
    }
    else{
      digitalWrite(ledPinRed,LOW);
      for(int i = 0; i<=180; i++){
        Grip.write(i);
        delay(10);
      }
    }
  }

  //delay(75);
}

void setup() {
  Base.write(90);
  Alfa.write(90);
  Beta.write(90);
  Grip.write(180);

  Base.attach(9); 
  Alfa.attach(10);
  Beta.attach(11);
  Grip.attach(3);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(buttonPinWhite, INPUT_PULLUP);
  pinMode(ledPinWhite, OUTPUT);

  pinMode(buttonPinRed, INPUT_PULLUP);
  pinMode(ledPinRed, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  buttonStateRed = digitalRead(buttonPinRed);
  StateRed = 1*buttonStateRed + 2*lastButtonStateRed;
  lastButtonStateRed = buttonStateRed;

  buttonStateWhite = digitalRead(buttonPinWhite);
  StateWhite = 1*buttonStateWhite + 2*lastButtonStateWhite;
  lastButtonStateWhite = buttonStateWhite;

  if( (StateRed == 2 || StateRed == 0) && (StateWhite == 2 || StateWhite == 0) ){
    Accumulator = Accumulator + 1;
    Serial.print("Accumulator ");
    Serial.println(Accumulator);
    if(Accumulator == 200){
      AutomaticMode = !AutomaticMode;
      Serial.print("MODE CHANGE ");
      Serial.println(AutomaticMode);
      Accumulator = 0;
  
      digitalWrite(ledPinRed,LOW);
      digitalWrite(ledPinWhite,LOW);
      delay(200);
      digitalWrite(ledPinRed,HIGH);
      digitalWrite(ledPinWhite,HIGH);
      delay(200);
      digitalWrite(ledPinRed,LOW);
      digitalWrite(ledPinWhite,LOW);
      delay(200);
      digitalWrite(ledPinRed,HIGH);
      digitalWrite(ledPinWhite,HIGH);
      delay(200);
      digitalWrite(ledPinRed,LOW);
      digitalWrite(ledPinWhite,LOW);
      resetValues();
      Base.write(90);
      Alfa.write(90);
      Beta.write(90);
      Grip.write(180);
    }
    return;
  }
  Accumulator = 0;
  
  if(AutomaticMode){
    Automatic();
  }
  else{
    Manual();
  }
  
  delay(10);
}