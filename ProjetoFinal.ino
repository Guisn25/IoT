#include <Servo.h>
#include <Wire.h>

Servo servo_motor;

int leitura_Joystick;
int pos;

typedef struct{
  float InclinacaoX;
  float InclinacaoY;
  float InclinacaoZ;
  int Distancia;
  bool funcionamento;
}Tvalores;

typedef union{
  Tvalores Valor;
  byte Array[sizeof(Tvalores)];
}Tdado;

Tdado Dado;

void setup() {
  Serial.begin(9600);
  Serial2.begin(115200);
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
bool trava = 0;

unsigned long Tdist;
unsigned long Tdado;
void loop(){
  Wire.beginTransmission(0x68);
  Wire.write(0x43);
  Wire.endTransmission(false);

  Wire.requestFrom(0x68, 6, true);
  Dado.Valor.InclinacaoX = Wire.read() << 8 | Wire.read();  
  Dado.Valor.InclinacaoY = Wire.read() << 8  | Wire.read(); 
  Dado.Valor.InclinacaoZ = Wire.read() << 8 | Wire.read();

  Dado.Valor.InclinacaoX = Dado.Valor.InclinacaoX / 131.0; 
  Dado.Valor.InclinacaoY = Dado.Valor.InclinacaoY / 131.0;
  Dado.Valor.InclinacaoZ = Dado.Valor.InclinacaoZ / 131.0;
 
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
    Dado.Valor.Distancia = sensor_proximidade(4,6);
    if(Dado.Valor.Distancia <= 20){
      tone(8, 2000);
      Dado.Valor.Funcionamento = LOW; 
    }
    else{
      noTone(8);
      Dado.Valor.Funcionamento = HIGH;
    }
    digitalWrite(2, Dado.Valor.Funcionamento);
    Tdist = millis();
  }

  if(millis()-Tdado > 100){
    Serial2.write(Dado.Array[0], 4); 
    Serial2.write(Dado.Array[4], 4);
    Serial2.write(Dado.Array[8], 4);
    Serial2.write(Dado.Array[12], 2);
    Serial2.write(Dado.Array[14], 1);
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
