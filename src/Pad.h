#pragma once

#include "Muxer.h"
#include <Arduino.h>

/**
 * @brief Clase para control de pads MIDI
 * Maneja pads con validación y estados de inicialización
 */
class PadControl {
public:
  /**
   * @brief Constructor
   * @param m Puntero al multiplexor a usar
   */
  PadControl(Muxer *m) : muxer(m), isInitialized(false), lastReadTime(0) {}

  /**
   * @brief Inicializa el control de pads
   * @return true si la inicialización fue exitosa
   */
  bool begin();
  
  /**
   * @brief Lee el estado de los pads y ejecuta callback si hay cambios
   * @param func Función callback para envío MIDI
   * @param midiCh Canal MIDI a usar
   */
  void read(void (*func)(uint8_t, uint8_t, uint8_t), uint8_t midiCh);
  
  /**
   * @brief Verifica si el sistema está listo
   * @return true si está inicializado y listo
   */
  bool isReady() const { return isInitialized && muxer != nullptr; }
  
  /**
   * @brief Obtiene el tiempo desde la última lectura
   * @return Tiempo en milisegundos desde última lectura
   */
  unsigned long getTimeSinceLastRead() const;
  
  /**
   * @brief Resetea el estado interno
   */
  void reset();

private:
  Muxer *muxer;
  bool isInitialized;
  unsigned long lastReadTime;
  
  /**
   * @brief Valida parámetros antes de procesar
   * @param func Función a validar
   * @return true si los parámetros son válidos
   */
  bool validateParameters(void (*func)(uint8_t, uint8_t, uint8_t)) const;
};
