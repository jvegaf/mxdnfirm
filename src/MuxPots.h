
#pragma once
#include "pin_map.h"
#include <Arduino.h>
#include <Multiplexer4067.h>

/**
 * @brief Clase para manejo de potenciómetros multiplexados
 * Gestiona lectura de múltiples potenciómetros con filtrado y debouncing
 */
class MuxPots {
public:
  // Configuración de filtrado y timing
  struct FilterConfig {
    uint16_t threshold;        // Umbral mínimo de cambio
    uint16_t timeout;          // Timeout para considerar movimiento finalizado
    uint16_t smoothingFactor;  // Factor de suavizado (0-100)
  };

  /**
   * @brief Constructor
   * @param sig Pin de señal del multiplexor
   * @param el Array de elementos/canales a leer
   * @param t_el Total de elementos
   */
  MuxPots(const uint8_t sig, const uint8_t *el, const uint8_t t_el);
  
  /**
   * @brief Destructor - libera memoria asignada
   */
  ~MuxPots();

  /**
   * @brief Inicializa el multiplexor
   * @return true si la inicialización fue exitosa
   */
  bool begin();
  
  /**
   * @brief Lee todos los potenciómetros y envía cambios MIDI
   * @param scc_func Función callback para enviar Control Change
   * @param midiCh Canal MIDI a usar
   */
  void read(void (*scc_func)(uint8_t, uint8_t, uint8_t), uint8_t midiCh);
  
  /**
   * @brief Configura parámetros de filtrado
   * @param config Nueva configuración de filtrado
   */
  void setFilterConfig(const FilterConfig& config);
  
  /**
   * @brief Obtiene el valor actual de un potenciómetro específico
   * @param index Índice del potenciómetro
   * @return Valor MIDI (0-127) o -1 si el índice es inválido
   */
  int getCurrentValue(uint8_t index) const;
  
  /**
   * @brief Resetea todos los valores almacenados
   */
  void reset();
  
  /**
   * @brief Verifica si el sistema está inicializado
   */
  bool isReady() const { return initialized; }
  
  /**
   * @brief Calibra un potenciómetro específico
   * @param index Índice del potenciómetro
   * @param minVal Valor mínimo leído
   * @param maxVal Valor máximo leído
   */
  void calibratePot(uint8_t index, uint16_t minVal, uint16_t maxVal);

private:
  // Configuración
  const uint8_t *elements;
  const uint8_t tElements;
  FilterConfig filterConfig;
  
  // Arrays dinámicos para estado
  uint8_t *lastCcValue;      // Último valor MIDI enviado
  uint16_t *potCState;       // Estado actual
  uint16_t *potPState;       // Estado anterior
  uint16_t *smoothedValue;   // Valor suavizado
  uint32_t *pTime;           // Tiempo de última actividad
  uint32_t *timer;           // Timer calculado
  
  // Calibración
  uint16_t *minValues;       // Valores mínimos calibrados
  uint16_t *maxValues;       // Valores máximos calibrados
  
  Multiplexer4067 *mplexPots;
  bool initialized;
  
  /**
   * @brief Aplica suavizado a un valor
   * @param index Índice del potenciómetro
   * @param newValue Nuevo valor leído
   * @return Valor suavizado
   */
  uint16_t applySmoothingFilter(uint8_t index, uint16_t newValue);
  
  /**
   * @brief Convierte valor analógico a MIDI con calibración
   * @param index Índice del potenciómetro
   * @param analogValue Valor analógico leído
   * @return Valor MIDI (0-127)
   */
  uint8_t analogToMidi(uint8_t index, uint16_t analogValue) const;
  
  /**
   * @brief Valida que un índice esté en rango válido
   * @param index Índice a validar
   * @return true si el índice es válido
   */
  bool isValidIndex(uint8_t index) const;
};
