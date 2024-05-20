#include <Servo.h>

Servo Base; //Pins
Servo Alfa;
Servo Beta;
Servo Grip;
const int LRpin = A0;
const int UDpin = A1;
const int trigPin = 5; //trigger - fiolet (jest koło plusa)
const int echoPin = 6; //echo - niebieski (jest koło grounda)

bool AutomaticMode = false; //Mode

int posBase = 90;    //Manual
int posAlfa = 90;
int posBeta = 90;
int posGrip = 120;
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
float distance;
float distance_temp;
bool MoveSide = true;
bool objStart = false;
int posStart = 0;
int Accumulator = 0;

void move(Servo &servo, int val)
{
  if(servo.read() == val)
    return;
  Serial.println("read != val");
  Serial.println(servo.read());
  Serial.println(val);
  Serial.println("-------");
  if(servo.read() < val)
  {
    for(int i=servo.read()+1; i<=val; i++)
    {
      servo.write(i);
      Serial.println(i);
      delay(20);
    }
  }
  else
  {
    for(int i=servo.read()-1; i>=val; i--)
    {
      servo.write(i);
      Serial.println(i);
      delay(20);
    }
  }
}

//beta jest na 0, alfa jest na 1

int distance_int = 15;

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

void setup() {
  Base.write(90);
  Alfa.write(90);
  Beta.write(90);
  Grip.write(120);

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
/*
  //Alfa.write(angles[distance-5)][1]);
  //Beta.write(angles[9][0]);
  
  move(Base, 90);
  //Base.write(90);
  delay(1000);

  // move(Beta, angles[distance_int-5][0]);
  // move(Alfa, angles[distance_int-5][1]);
  // move(Grip, 120);

  // Alfa.write(angles[distance_int-5][1]);
  // Beta.write(angles[distance_int-5][0]);
  // Grip.write(120);
  // delay(2500);
  for(int i=119; i>=0; i--)
  {
    Grip.write(i);
    delay(10);
  }
  delay(2000);
      move(Alfa, 120);
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
}

*/
}

//wartosci odleglosci musza byc od 5cm do 17cm (12cm range)
//alfa granica to 135 stopni (dotyka ziemi)
//beta granica to 60 stopni (dotyka ziemi)