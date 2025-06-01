
#pragma once
#include <Arduino.h>
#define ENCODER_DO_NOT_USE_INTERRUPTS
#include "pin_map.h"
#include <Encoder.h>

/**
 * @brief Clase para manejo de encoders rotativos del browser
 * Gestiona encoders izquierdo y derecho con debouncing y validación
 */
class BREncoder {
public:
  // Configuración de encoders
  struct EncoderConfig {
    uint8_t ccNumberCW;      // Control Change para rotación horaria
    uint8_t ccNumberCCW;     // Control Change para rotación antihoraria  
    uint8_t valueCW;         // Valor MIDI para rotación horaria
    uint8_t valueCCW;        // Valor MIDI para rotación antihoraria
    uint8_t midiChannel;     // Canal MIDI
    uint16_t debounceMs;     // Tiempo de debounce en ms
  };

  BREncoder();
  ~BREncoder();
  
  /**
   * @brief Inicializa los encoders
   * @return true si la inicialización fue exitosa
   */
  bool begin();
  
  /**
   * @brief Lee el estado de los encoders y envía MIDI si hay cambios
   * @param scc_func Función callback para enviar Control Change
   */
  void readEnc(void (*scc_func)(uint8_t, uint8_t, uint8_t));
  
  /**
   * @brief Configura los parámetros de un encoder específico
   * @param isLeft true para encoder izquierdo, false para derecho
   * @param config Configuración del encoder
   */
  void configureEncoder(bool isLeft, const EncoderConfig& config);
  
  /**
   * @brief Resetea las posiciones de los encoders
   */
  void reset();
  
  /**
   * @brief Obtiene la posición actual de un encoder
   * @param isLeft true para encoder izquierdo, false para derecho
   * @return Posición actual del encoder
   */
  long getPosition(bool isLeft) const;
  
  /**
   * @brief Verifica si el sistema está inicializado
   */
  bool isReady() const { return initialized; }

private:
  Encoder* leftEncoder;
  Encoder* rightEncoder;
  
  // Estados anteriores
  long oldLeft;
  long oldRight;
  
  // Timestamps para debouncing
  unsigned long lastLeftChange;
  unsigned long lastRightChange;
  
  // Configuraciones
  EncoderConfig leftConfig;
  EncoderConfig rightConfig;
  
  bool initialized;
  
  /**
   * @brief Procesa cambios en un encoder específico
   * @param isLeft true para encoder izquierdo
   * @param scc_func Función callback para MIDI
   */
  void processEncoder(bool isLeft, void (*scc_func)(uint8_t, uint8_t, uint8_t));
};
