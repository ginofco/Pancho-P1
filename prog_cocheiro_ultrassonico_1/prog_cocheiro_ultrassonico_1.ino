//**** Cocheiro Ultrasonico ****//
// Recebe sinais dos sensores ultrassonicos.
// Transforma os sinais em distancias medidas.
// Raciocina qual decisao deve ser tomada (seguir ou desviar).
// Envia a decisão para o outro arduino (parelha_robotica).
// Utiliza o protocolo i2c.
//
// Carioca HackerSpace (CHS)
// Grupo de Robotica

#include <NewPing.h>
#include <Wire.h>

#define DIST_LIMIT    15  // Limite de deteccao de barreira pelo robo (inicia manobra)
#define MAX_DISTANCE 200  // Maximum distance we want to ping for (in centimeters).
                          // (Maximum sensor distance rated is at 400-500cm.)
#define ECHO_PIN_A     6  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define ECHO_PIN_B     7  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define ECHO_PIN_C     8  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define TRIGGER_PIN    9  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define pin_LEDA       3
#define pin_LEDB       4
#define pin_LEDC       5

int  distance      = 0;
int  detect_wall_A = 0;
int  detect_wall_B = 0;
int  detect_wall_C = 0;
byte state         = 0; // Robot state:  (Variavel global usada para comunicacao i2c.)
                        // 0 - stop
                        // 1 - forward
                        // 2 - turn left
                        // 3 - turn right
                
// NewPing setup of pins and maximum distance.
NewPing sonar_A(TRIGGER_PIN, ECHO_PIN_A, MAX_DISTANCE);
NewPing sonar_B(TRIGGER_PIN, ECHO_PIN_B, MAX_DISTANCE);
NewPing sonar_C(TRIGGER_PIN, ECHO_PIN_C, MAX_DISTANCE);

void setup() {
  Wire.begin(); // join i2c bus (address optional for master)
  pinMode(pin_LEDA, OUTPUT);
  pinMode(pin_LEDB, OUTPUT);
  pinMode(pin_LEDC, OUTPUT);
  Serial.begin(9600);
  //Serial.println("Inicio da leitura");
}

void loop() {
  unsigned int distA, distB, distC, uSA, uSB, uSC;
  Wire.beginTransmission(8); // transmit to device #8
  
 //Ultrasonico
  delay(100); // Aguarda 100ms entre um disparo e outro. 29ms deve ser o menor valor entre um disparo e outro.
  uSA = sonar_A.ping();     // Envia o disparo e recebe o tempo de ida e volta em microsegundos (uS).
  uSB = sonar_B.ping();
  uSC = sonar_C.ping();

  distA = distB = distC = MAX_DISTANCE;
  if (uSA>0) distA = uSA / US_ROUNDTRIP_CM ;  // Variavel para armazenar o valor ja convertido em centimetros
  if (uSB>0) distB = uSB / US_ROUNDTRIP_CM ;  // Variavel para armazenar o valor ja convertido em centimetros
  if (uSC>0) distC = uSC / US_ROUNDTRIP_CM ;
  //Serial.print("D1: ");
  //Serial.print(distA);
  //Serial.print(" - D2: ");
  //Serial.print(distB);
  //Serial.print(" - D3: ");
  //Serial.println(distC);  
  
  detect_wall_A = detect_wall_B = detect_wall_C = 0;
  if (distA>=DIST_LIMIT) digitalWrite(pin_LEDA, LOW);
  if (distB>=DIST_LIMIT) digitalWrite(pin_LEDB, LOW);
  if (distC>=DIST_LIMIT) digitalWrite(pin_LEDC, LOW);
    
  if (distA<DIST_LIMIT)  { digitalWrite(pin_LEDA, HIGH); detect_wall_A = 1; }
  if (distB<DIST_LIMIT)  { digitalWrite(pin_LEDB, HIGH); detect_wall_B = 1; }
  if (distC<DIST_LIMIT)  { digitalWrite(pin_LEDC, HIGH); detect_wall_C = 1; }


  // Central de decisao
  state = 1;  // Default: segue em frente

  // Deteccao de barreira aa direita: comanda virar aa direita (state->2)
  if (detect_wall_C == 1) state = 2;

  
  if (detect_wall_A == 1) state = 3;
  if ( (detect_wall_A == 1) & (detect_wall_C == 1) ) state = 0;
  if (detect_wall_B == 1) state = 0;
    
  Wire.write(state);         // sends one byte
  Wire.endTransmission();    // stop transmitting

}
