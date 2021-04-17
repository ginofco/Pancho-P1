//**** Parelha Robotica ****//
// Recebe decisao de seguir ou manobrar do outro arduino
// (Cocheiro Ultrassonico)
// Executa o comando acionando os motores pela ponte H
// Utiliza o protocolo i2c
//
// Carioca HackerSpace (CHS)
// Grupo de Robotica

#include <Wire.h>

#define PIN_OUT1               4
#define PIN_OUT2               5
#define PIN_OUT3               6
#define PIN_OUT4               7

byte   state=1;

void setup() {
  pinMode(PIN_OUT1, OUTPUT);
  pinMode(PIN_OUT2, OUTPUT);
  pinMode(PIN_OUT3, OUTPUT);
  pinMode(PIN_OUT4, OUTPUT);
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  //while (1 < Wire.available()) { // loop through all but the last
  //  char c = Wire.read(); // receive byte as a character
  //  Serial.print(c);         // print the character
  //}
  //int x = Wire.read();    // receive byte as an integer
  //Serial.println(x);         // print the integer
  //state = x;
  state = Wire.read();
}


void loop() {
  switch (state) {
     case 0:
         full_stop_and_return();
         break;
     case 1:
         go_ahead();
         break;
     case 2:
         turn_right();
         break;
     case 3:
         turn_left();
         break;
     default:
         full_stop_and_return();
  }
}

void full_stop_and_return() {
  digitalWrite(PIN_OUT1, LOW);
  digitalWrite(PIN_OUT2, LOW);
  digitalWrite(PIN_OUT3, LOW);
  digitalWrite(PIN_OUT4, LOW);
  delay(500);
  digitalWrite(PIN_OUT1, LOW);
  digitalWrite(PIN_OUT2, HIGH);
  digitalWrite(PIN_OUT3, LOW);
  digitalWrite(PIN_OUT4, HIGH);
  delay(1000);
  digitalWrite(PIN_OUT1, LOW);
  digitalWrite(PIN_OUT2, HIGH);
  digitalWrite(PIN_OUT3, HIGH);
  digitalWrite(PIN_OUT4, LOW);
  delay(1500);
  digitalWrite(PIN_OUT1, LOW);
  digitalWrite(PIN_OUT2, LOW);
  digitalWrite(PIN_OUT3, LOW);
  digitalWrite(PIN_OUT4, LOW);
  delay(500);
}

void go_ahead() {
  digitalWrite(PIN_OUT1, HIGH);
  digitalWrite(PIN_OUT2, LOW);
  digitalWrite(PIN_OUT3, HIGH);
  digitalWrite(PIN_OUT4, LOW);
  delay(100);
}

void turn_left() {
  digitalWrite(PIN_OUT1, LOW); // Stop for awhile
  digitalWrite(PIN_OUT2, LOW);
  digitalWrite(PIN_OUT3, LOW);
  digitalWrite(PIN_OUT4, LOW);
  delay(2000);
  digitalWrite(PIN_OUT1, HIGH); // Make turn
  digitalWrite(PIN_OUT2, LOW);
  digitalWrite(PIN_OUT3, LOW);
  digitalWrite(PIN_OUT4, HIGH);
  delay(500);
  digitalWrite(PIN_OUT1, LOW); // Stop for awhile
  digitalWrite(PIN_OUT2, LOW);
  digitalWrite(PIN_OUT3, LOW);
  digitalWrite(PIN_OUT4, LOW);
  delay(2000);
}

void turn_right() {
  digitalWrite(PIN_OUT1, LOW); // Stop for awhile
  digitalWrite(PIN_OUT2, LOW);
  digitalWrite(PIN_OUT3, LOW);
  digitalWrite(PIN_OUT4, LOW);
  delay(2000);
  digitalWrite(PIN_OUT1, LOW); // Make turn
  digitalWrite(PIN_OUT2, HIGH);
  digitalWrite(PIN_OUT3, HIGH);
  digitalWrite(PIN_OUT4, LOW);
  delay(500);
  digitalWrite(PIN_OUT1, LOW); // Stop for awhile
  digitalWrite(PIN_OUT2, LOW);
  digitalWrite(PIN_OUT3, LOW);
  digitalWrite(PIN_OUT4, LOW);
  delay(2000);
}
