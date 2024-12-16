// Guilherme Silva do Nascimento 156554
// Atividade 2 com envio de múltiplos bytes via UART

#include "DHT.h"
DHT dht(2, DHT11);

int LDR[5] = {0};
int LM35[5] = {0};
float DHTtemp[5] = {0};
float DHThum[5] = {0};

typedef struct Svalor{
  float mediaLM35;
  float mediaLDR;
  float mediaDHTtemp;
  float mediaDHThum;
}Tvalor;

typedef union Uvalor{
  Tvalor valor;
  byte bytesValor[sizeof(Tvalor)];
}TBytes;

TBytes Dados;

bool sensor = 0;
int escolhaSensor;
int escolhaDado;

unsigned long T = 0; 

void setup(){
  Serial2.begin(115200);
  Serial.begin(115200);
  dht.begin();
}

void loop(){
  if(millis() - T > 200){
    LDR[0] = analogRead(0);
    LM35[0] = analogRead(1);
    DHTtemp[0] = dht.readTemperature();
    DHThum[0] = dht.readHumidity();
  
    Dados.valor.mediaLDR = (LDR[0] + LDR[1] + LDR[2] + LDR[3] + LDR[4])/5;
    Dados.valor.mediaLM35 = (LM35[0] + LM35[1] + LM35[2] + LM35[3] + LM35[4])/5;
    Dados.valor.mediaDHTtemp = (DHTtemp[0] + DHTtemp[1] + DHTtemp[2] + DHTtemp[3] + DHTtemp[4])/5;
    Dados.valor.mediaDHThum = (DHThum[0] + DHThum[1] + DHThum[2] + DHThum[3] + DHThum[4])/5;
    
    Dados.valor.mediaLM35 = Dados.valor.mediaLM35 * 0.48875855327;

    for(int i=4;i>0;i--){
      LDR[i] = LDR[i-1];
      LM35[i] = LM35[i-1];
      DHTtemp[i] = DHTtemp[i-1];
      DHThum[i] = DHThum[i-1];
    }
    T = millis();
  }
  
  if(Dados.valor.mediaLDR > 250){
    Serial2.write('A');
  }

  if(Dados.valor.mediaLM35 > 25 || Dados.valor.mediaDHTtemp > 25){
    Serial2.write('B');
  }
  
  if(!sensor){
    if(Serial2.available()){
      escolhaSensor = Serial2.read();
      sensor = 1;
    }
  }else{
    switch(escolhaSensor){
      case 49:
        if(Serial2.available()){
          escolhaDado = Serial2.read();
          if(escolhaDado == 50){
            sensor = 0;
          }
          if(escolhaDado == 49){
            Serial2.write(&Dados.bytesValor[0], 4);
            sensor = 0;
          }
        }
      break;

      case 50:
        if(Serial2.available()){
          escolhaDado = Serial2.read();
          if(escolhaDado == 50){
            sensor = 0;
          }
          if(escolhaDado == 49){
            Serial2.write(&Dados.bytesValor[4], 4);
            sensor = 0;
          }
        }
      break;

      case 51:
        if(Serial2.available()){
          escolhaDado = Serial2.read();
          if(escolhaDado == 51){
            sensor = 0;
          }
          if(escolhaDado == 49){
            Serial2.write(&Dados.bytesValor[8], 4);
            sensor = 0;
          }
          if(escolhaDado == 50){
            Serial2.write(&Dados.bytesValor[12], 4);
            sensor = 0;
          }
        }
      break;
    }
  }
}
