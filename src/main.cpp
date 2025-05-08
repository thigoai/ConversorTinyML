#include <Arduino.h>
#include "NeuralNetwork.h"

NeuralNetwork *nn;

// Pinos do display de 7 segmentos (a, b, c, d, e, f, g)
const int segmentPins[7] = {23, 22, 21, 19, 18, 26, 5};

void setup() {
    Serial.begin(115200);
    while (!Serial) ;  // Espera porta serial abrir (opcional, para placas USB nativas)

    // Inicializa os pinos do display
    for (int i = 0; i < 7; i++) {
        pinMode(segmentPins[i], OUTPUT);
        digitalWrite(segmentPins[i], LOW); // Apaga o segmento (comum anodo)
    }
    
    Serial.println("Inicializando modelo...");
    nn = new NeuralNetwork();
    Serial.println("Modelo inicializado!");
}

void loop() {
    for (int valor = 0; valor < 16; valor++) {
        // Define entrada de 4 bits (MSB primeiro)
        for (int i = 0; i < 4; i++) {
            nn->getInputBuffer()[i] = (valor & (1 << (3 - i))) ? 1.0f : 0.0f;
        }

        nn->predict();
        float *output = nn->getOutpuBuffer();  // corrigido aqui

        Serial.print("Entrada: ");
        for (int i = 3; i >= 0; i--) {
            Serial.print((valor >> i) & 1);
        }

        Serial.print(" | Saída: ");
        for (int i = 0; i < 7; i++) {
            bool ligado = output[i] > 0.5;
            digitalWrite(segmentPins[i], ligado ? HIGH : LOW);  // LOW acende (anodo comum)
            Serial.print((output[i] > 0.5f) ? "1" : "0");
            Serial.print(" ");
        }

        
        Serial.println();
        delay(1000);
    }

   // while (true);  // trava após rodar 0-15
}
