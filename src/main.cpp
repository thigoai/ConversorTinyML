#include <Arduino.h>
#include "NeuralNetwork.h"

NeuralNetwork *nn;

//  https://colab.research.google.com/drive/1azBPAoixkGswvUJpyAvLYKBaNBN1kBII?usp=sharing


bool estados[4] = {false, false, false, false};
bool ultimosEstados[4] = {true, true, true, true}; 
int pinos[4] = {2, 3, 4, 5};

void setup(){
    Serial.begin(115200);
    Serial.println("Inicializando modelo...");
    nn = new NeuralNetwork();
    Serial.println("Modelo inicializado!");
}

void loop(){
  for (int i = 0; i < 4; i++) {
    bool leitura = digitalRead(pinos[i]);
    if (ultimosEstados[i] == HIGH && leitura == LOW) {
      estados[i] = !estados[i];
    }
    ultimosEstados[i] = leitura;

    nn->getInputBuffer()[i] = estados[i] ? 1.0f : 0.0f;
  }

  // Faz a previsão
  nn->predict();

  // Lê as saídas
  float *output = nn->getOutpuBuffer();

  Serial.print("Saída: ");
  for (int i = 0; i < 7; i++) {
    Serial.print(output[i] > 0.5 ? "1" : "0"); 
    Serial.print(" ");
  }
  Serial.println();

  delay(100);
}

