#include "MuxPots.h"

// Configuración por defecto
const MuxPots::FilterConfig DEFAULT_FILTER_CONFIG = {
  .threshold = 8,
  .timeout = 50,
  .smoothingFactor = 15  // 15% de suavizado
};

MuxPots::MuxPots(const uint8_t sig, const uint8_t *el, const uint8_t t_el)
    : elements(el), tElements(t_el), filterConfig(DEFAULT_FILTER_CONFIG), initialized(false) {
  
  // Validar parámetros de entrada
  if (!el || t_el == 0) {
    return;
  }
  
  try {
    // Inicializar multiplexor
    mplexPots = new Multiplexer4067(muxPins[0], muxPins[1], muxPins[2], muxPins[3], sig);
    
    // Asignar memoria para arrays
    pTime = new uint32_t[t_el]();
    timer = new uint32_t[t_el]();
    potCState = new uint16_t[t_el]();
    potPState = new uint16_t[t_el]();
    smoothedValue = new uint16_t[t_el]();
    lastCcValue = new uint8_t[t_el]();
    minValues = new uint16_t[t_el]();
    maxValues = new uint16_t[t_el]();
    
    // Inicializar valores de calibración por defecto
    for (uint8_t i = 0; i < t_el; i++) {
      minValues[i] = 0;
      maxValues[i] = 1023;
      lastCcValue[i] = 255; // Valor inválido para forzar primer envío
    }
    
  } catch (...) {
    // Error en asignación de memoria
    initialized = false;
  }
}

MuxPots::~MuxPots() {
  delete mplexPots;
  delete[] pTime;
  delete[] timer;
  delete[] potCState;
  delete[] potPState;
  delete[] smoothedValue;
  delete[] lastCcValue;
  delete[] minValues;
  delete[] maxValues;
}

bool MuxPots::begin() {
  if (!mplexPots) {
    return false;
  }
  
  try {
    mplexPots->begin();
    initialized = true;
    return true;
  } catch (...) {
    initialized = false;
    return false;
  }
}

void MuxPots::read(void (*scc_func)(uint8_t, uint8_t, uint8_t), uint8_t midiCh) {
  if (!initialized || !scc_func) {
    return;
  }
  
  unsigned long currentTime = millis();
  
  for (uint8_t i = 0; i < tElements; i++) {
    // Leer valor actual del potenciómetro
    uint16_t rawValue = mplexPots->readChannel(elements[i]);
    
    // Aplicar filtro de suavizado
    uint16_t smoothedVal = applySmoothingFilter(i, rawValue);
    potCState[i] = smoothedVal;
    
    // Calcular variación desde última lectura
    uint16_t potVar = abs(potCState[i] - potPState[i]);
    
    if (potVar >= filterConfig.threshold) {
      pTime[i] = currentTime; // Actualizar tiempo de última actividad
    }
    
    timer[i] = currentTime - pTime[i];
    
    // Si el potenciómetro aún se está moviendo (dentro del timeout)
    if (timer[i] < filterConfig.timeout) {
      uint8_t ccValue = analogToMidi(i, potCState[i]);
      
      // Enviar solo si el valor MIDI ha cambiado
      if (lastCcValue[i] != ccValue) {
        scc_func(i, ccValue, midiCh);
        potPState[i] = potCState[i];
        lastCcValue[i] = ccValue;
      }
    }
  }
}

void MuxPots::setFilterConfig(const FilterConfig& config) {
  filterConfig = config;
  
  // Validar rangos
  if (filterConfig.smoothingFactor > 100) {
    filterConfig.smoothingFactor = 100;
  }
}

int MuxPots::getCurrentValue(uint8_t index) const {
  if (!isValidIndex(index) || !initialized) {
    return -1;
  }
  
  return analogToMidi(index, potCState[index]);
}

void MuxPots::reset() {
  if (!initialized) return;
  
  for (uint8_t i = 0; i < tElements; i++) {
    potCState[i] = 0;
    potPState[i] = 0;
    smoothedValue[i] = 0;
    lastCcValue[i] = 255; // Valor inválido
    pTime[i] = 0;
    timer[i] = 0;
  }
}

void MuxPots::calibratePot(uint8_t index, uint16_t minVal, uint16_t maxVal) {
  if (!isValidIndex(index)) return;
  
  // Validar que min < max
  if (minVal >= maxVal) {
    return;
  }
  
  minValues[index] = minVal;
  maxValues[index] = maxVal;
}

uint16_t MuxPots::applySmoothingFilter(uint8_t index, uint16_t newValue) {
  if (!isValidIndex(index)) return newValue;
  
  if (filterConfig.smoothingFactor == 0) {
    smoothedValue[index] = newValue;
  } else {
    // Filtro de media móvil exponencial
    uint32_t factor = filterConfig.smoothingFactor;
    smoothedValue[index] = ((100 - factor) * smoothedValue[index] + factor * newValue) / 100;
  }
  
  return smoothedValue[index];
}

uint8_t MuxPots::analogToMidi(uint8_t index, uint16_t analogValue) const {
  if (!isValidIndex(index)) return 0;
  
  // Aplicar calibración específica del potenciómetro
  uint16_t minVal = minValues[index];
  uint16_t maxVal = maxValues[index];
  
  // Clampear valor dentro del rango calibrado
  if (analogValue < minVal) analogValue = minVal;
  if (analogValue > maxVal) analogValue = maxVal;
  
  // Mapear a rango MIDI (0-127)
  return map(analogValue, minVal, maxVal, 0, 127);
}

bool MuxPots::isValidIndex(uint8_t index) const {
  return index < tElements;
}
