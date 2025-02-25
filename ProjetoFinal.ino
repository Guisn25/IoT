#include <Servo.h>
#include <Wire.h>

Servo servo_motor;

int leitura_Joystick;
int pos;

float giro_x;
float giro_y; 
float giro_z;

void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT); // LED funcionamento
  pinMode(4,OUTPUT);  // TRIG sensor ultrassônico
  pinMode(6,INPUT);   // ECHO sensor ultrassônico
  pinMode(8, OUTPUT);  // BUZZER
  
  servo_motor.attach(10); 
  servo_motor.write(30);
  
  Wire.begin();
  Wire.beginTransmission(0x68);
  Wire.write(0x6B); 
  Wire.write(0);
  Wire.endTransmission(true);

  digitalWrite(2, HIGH);
}
int levanta = 0;
bool trava = 0;

unsigned long Tdist;
void loop(){
  Wire.beginTransmission(0x68);
  Wire.write(0x43);
  Wire.endTransmission(false);

  Wire.requestFrom(0x68, 6, true);
  giro_x = Wire.read() << 8 | Wire.read();  
  giro_y = Wire.read() << 8  | Wire.read(); 
  giro_z = Wire.read() << 8 | Wire.read();

  giro_x = giro_x / 131.0; 
  giro_y = giro_y / 131.0;
  giro_z = giro_z / 131.0;
 
  if(analogRead(1) > 550){
    trava = 1;
  }else{
    trava = 0;
  }
  if(trava){
    leitura_Joystick = analogRead(0);
    pos = map(leitura_Joystick, 0, 1023, 10, 45);
    servo_motor.write(pos);
  }

  if(millis()-Tdist > 500){
    
    int distancia = sensor_proximidade(4,6);
    if(distancia <= 20){
      tone(8, 2000);
    }
    else{
      noTone(8);
    }
    Tdist = millis();
  }
}

int sensor_proximidade(int pinotrig,int pinoecho){
  digitalWrite(pinotrig,LOW);
  delayMicroseconds(2);
  digitalWrite(pinotrig,HIGH);
  delayMicroseconds(10);
  digitalWrite(pinotrig,LOW);
  int aux = pulseIn(pinoecho,HIGH)/58;
  return aux;
}
