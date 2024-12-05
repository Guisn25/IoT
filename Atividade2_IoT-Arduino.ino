#include "DHT.h"
DHT dht(2, DHT11);

int LDR[5] = {0};
int LM35[5] = {0};
float DHTtemp[5] = {0};
float DHThum[5] = {0};

typedef struct Svalor{
  float mediaLDR;
  float mediaLM35;
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
    if(Dados.valor.mediaLDR > 250){
      Serial2.write('A');
    }

    if(Dados.valor.mediaLM35 > 27 || Dados.valor.mediaDHTtemp > 30){
      Serial2.write('B');
    }

    for(int i=4;i>0;i--){
      LDR[i] = LDR[i-1];
      LM35[i] = LM35[i-1];
      DHTtemp[i] = DHTtemp[i-1];
      DHThum[i] = DHThum[i-1];
    }
    T = millis();
  }
  for(int i=0; i<4; i++){
   Serial.print(Dados.valor.mediaLM35);
   Serial.print("\t");
   Serial2.write(Dados.bytesValor[i+4]);
  }
  Serial.println();
  /*
  if(!sensor){
    if(Serial2.available()){
      escolhaSensor = Serial2.parseInt();
      sensor = 1;
    }
  }else{
    switch(escolhaSensor){
      case 1:
        if(Serial2.available()){
          escolhaDado = Serial2.parseInt();
          if(escolhaDado == 2){
            sensor = 0;
          }
          if(escolhaDado == 1){
            for(int i=0; i<4; i++){
              Serial2.write(Dados.bytesValor[i]);
            }
          }
        }
      break;

      case 2;
        if(Serial2.available()){
          escolhaDado = Serial2.parseInt();
          if(escolhaDado == 2){
            sensor = 0;
          }
          if(escolhaDado == 1){
            Serial2.write(Dados.bytesValor[i+4]);
          }
        }
      break;

      case 3:
        if(Serial2.available()){
          escolhaDado = Serial2.parseInt();
          if(escolhaDado == 3){
            sensor = 0;
          }
          if(escolhaDado == 1){
            Serial2.write(Dados.bytesValor[i+8]);
          }
          if(escolhaDado == 2){
            Serial2.write(Dados.bytesValor[i+12]);
          }
        }
      break;
    }
  }
  */
}
