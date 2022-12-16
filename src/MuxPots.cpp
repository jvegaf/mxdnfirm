#include "MuxPots.h"

void MuxPots::begin() { mplexPots->begin(); }

void MuxPots::read(void (*scc_func)(uint8_t, uint8_t, uint8_t),
                   uint8_t midiCh) {
  for (uint8_t i = 0; i < t_elements; i++) {
    uint16_t potVar = abs(
        potCState[i] - potPState[i]); // calcula a variacao da porta analogica
    potCState[i] = mplexPots->readChannel(elements[i]);

    if (potVar >= kThreshold) {
      pTime[i] = millis(); // armazena o tempo previo
    }
    timer[i] = millis() - pTime[i]; // reseta o timer
    if (timer[i] <
        kTimeout) { // se o timer for menor que o tempo maximo permitido
                    // significa que o potenciometro ainda esta se movendo
      uint8_t ccValue = map(potCState[i], 0, 1023, 0, 127);

      if (lastCcValue[i] != ccValue) {
        scc_func(i, ccValue, midiCh); // envia Control Change (numero do CC,
                                      // valor do CC, canal midi)
        potPState[i] =
            potCState[i]; // armazena a leitura atual do potenciometro para
        lastCcValue[i] = ccValue;
        // comparar com a proxima
      }
    }
  }
}
