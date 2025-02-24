#include <Servo.h>

Servo servo_motor;

int leitura_Joystick;

void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT); // LED funcionamento
  pinMode(4,OUTPUT);  // TRIG sensor ultrassônico
  pinMode(6,INPUT);   // ECHO sensor ultrassônico
  pinMode(8, OUTPUT);  // BUZZER
  servo_motor.attach(9); 
  servo_motor.write(10); 
  
  
  digitalWrite(2, LOW);
}


unsigned int Tdist;
void loop(){
  if(millis()-Tdist > 500){
    int distancia = sensor_proximidade(4,6);
    if(distancia <= 20){
      tone(8, 2000);
    }
    else{
      noTone(8);
    }
    Serial.println(distancia);
    Tdist = millis();
  }

}

int sensor_proximidade(int pinotrig,int pinoecho){
  digitalWrite(pinotrig,LOW);
  delayMicroseconds(2);
  digitalWrite(pinotrig,HIGH);
  delayMicroseconds(10);
  digitalWrite(pinotrig,LOW);

  return pulseIn(pinoecho,HIGH)/58;
}