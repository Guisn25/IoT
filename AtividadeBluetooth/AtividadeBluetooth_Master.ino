#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

int sensor;
int grandeza;

bool sensorEscolha = 0;
bool grandezaEscolha = 0;

typedef union{
  float valor;
  byte bytesValor[4];
}TBytes;

TBytes Dado;

void setup() {
  Serial.begin(9600);
  SerialBT.begin("ESP32test",true); //mudar antes de compilar
  if(SerialBT.connect("Guilherme")){ // nome do HC-05 que será conectado
    Serial.println("Conectou!");
  }
}
int i = 0;
void loop(){
  if(!sensorEscolha){
    if(Serial.available()) {
      sensor = Serial.read();
      SerialBT.write(sensor);
      Serial.println(sensor);
      sensorEscolha = 1;
    }
  }else{
    if(!grandezaEscolha){
      if(Serial.available()){
        grandeza = Serial.read();
        SerialBT.write(grandeza);
        grandezaEscolha = 1;
      }
    }else{
      while(i<4){
        if (SerialBT.available()){ 
          Dado.bytesValor[i] = SerialBT.read();
          i++;
        }
      }
      Serial.println(Dado.valor); 
      sensorEscolha = 0;
      grandezaEscolha = 0;
    }
  }
}
