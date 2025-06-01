# 🎛️ Mixduino Firmware - Controlador MIDI Avanzado

Un controlador MIDI completo basado en Arduino con características profesionales para DJs y productores musicales.

## ✨ Características

- **🎚️ 3 Canales Mixer completos**
- **🎵 2 Decks independientes** 
- **📚 Navegador de biblioteca** con función de preview
- **🎪 16 Pads de control** con LEDs RGB (8 por deck)
- **🎭 Controles de efectos (FX)**
- **🎪 Controles de pitch/tempo** de 75mm
- **👆 Barras multitáctiles** en lugar de jog wheels tradicionales

## 🏗️ Arquitectura del Proyecto

```
src/
├── main.cpp          # Loop principal y setup
├── MDCore.cpp/.h     # Core del sistema MIDI
├── Deck.cpp/.h       # Control de decks
├── Pad.cpp/.h        # Control de pads
├── BREncoder.cpp/.h  # Encoders rotatorios
├── MuxPots.cpp/.h    # Potenciómetros multiplexados
└── ...otros módulos

include/
├── base.h           # Definiciones base
├── midi_map.h       # Mapeo MIDI
├── pin_map.h        # Mapeo de pines
└── ...configuraciones

test/
├── mocks/           # Mocks para testing
├── test_mdcore/     # Tests del core
├── test_deck/       # Tests de decks
└── test_main/       # Tests del main
```

## 🚀 Compilación y Deployment

### Instalación rápida
```shell
# Clonar el repositorio
git clone <tu-repo>
cd mxdnfirm

# Instalar dependencias
make install-deps

# Compilar
make build

# Subir al Arduino
make upload
```

### Comandos principales
```shell
make build          # Compilar proyecto
make upload         # Subir al Arduino
make test           # Ejecutar tests unitarios
make monitor        # Monitor serial
make clean          # Limpiar compilación
```

Ver todos los comandos disponibles:
```shell
make help
```

## 🧪 Testing Unitario

El proyecto incluye un sistema completo de testing unitario usando **Google Test**.

### Ejecutar todos los tests
```shell
make test
```

### Ejecutar tests específicos
```shell
make test-mdcore    # Tests del core MIDI
make test-deck      # Tests de control de decks
make test-main      # Tests de funciones principales
```

### Tests con output detallado
```shell
make test-verbose
```

### Estructura de testing
- **Mocks completos** para Arduino, MIDI, y threads
- **Tests de validación** de datos MIDI
- **Tests de robustez** y manejo de errores
- **Tests de inicialización** y estados
- **Cobertura** de funciones críticas

## 🧪 Testing

The project includes comprehensive unit tests and integration tests using Google Test framework.

### Running Tests

```bash
# Run all tests
make test

# Run specific component tests
make test-mdcore      # Core system tests
make test-deck        # Deck control tests
make test-brencoder   # Rotary encoder tests
make test-muxpots     # Multiplexed potentiometer tests
make test-potkit      # Potentiometer kit tests
make test-btnkit      # Button kit tests
make test-npkit       # NeoPixel LED tests

# Run integration tests
make test-integration

# Run complete test suite
make test-full

# Run performance benchmarks
make benchmark
```

### Test Coverage

The testing framework covers:

- **Unit Tests**: Individual component functionality
- **Integration Tests**: Multi-component interactions
- **Performance Tests**: Stress testing and timing validation
- **Memory Tests**: Memory allocation and cleanup verification
- **Error Handling**: Comprehensive error condition testing

### Test Structure

```
test/
├── mocks/                 # Mock implementations for Arduino/MIDI
├── test_main.cpp         # Main test runner
├── test_mdcore/          # Core system tests
├── test_deck/            # Deck control tests
├── test_brencoder/       # Rotary encoder tests
├── test_muxpots/         # Multiplexed potentiometer tests
├── test_potkit/          # Potentiometer kit tests
├── test_btnkit/          # Button kit tests
├── test_npkit/           # NeoPixel LED tests
└── test_integration/     # Integration tests
```

## 🔧 Desarrollo

### Configuración del entorno
El proyecto usa **PlatformIO** con las siguientes dependencias:

```ini
[env:mega]
platform = atmelavr
board = megaatmega2560
framework = arduino
lib_deps = 
    fortyseveneffects/MIDI Library
    adafruit/Adafruit NeoPixel
    ivanseidel/ArduinoThread
    # ...otras dependencias
```

### Mejoras implementadas

#### 🚀 Optimizaciones de rendimiento:
- **Gestión de tiempo mejorada** con debouncing
- **Validación de datos MIDI** (0-127)
- **Configuración optimizada** del baud rate
- **Threading inteligente** con prioridades

#### 🛡️ Robustez y validación:
- **Manejo de errores** con códigos específicos
- **Validación de parámetros** en todas las funciones
- **Estados de inicialización** controlados
- **Timeout protection** para MIDI

#### 🔧 Mantenimiento:
- **Código modular** y bien documentado
- **Constantes configurables** 
- **Tests unitarios** comprehensivos
- **Separación clara** de responsabilidades

## 🐛 Debug y Monitorización

### Debug MIDI en Linux
```shell
ttymidi -s /dev/ttyACM0 -v
```

### Debug MIDI en Windows
```shell
# Usar herramientas como MIDI Monitor o similar
```

### Monitor serial
```shell
make monitor        # Monitor con formato
make serial-raw     # Monitor sin formato
```

### Análisis de memoria
```shell
make size          # Muestra uso de memoria
make lint          # Análisis estático de código
```

## 📊 Estado del Proyecto

### ✅ Implementado
- [x] Core MIDI con validaciones
- [x] Control de decks con error handling
- [x] Sistema de testing unitario
- [x] Optimizaciones de rendimiento
- [x] Documentación completa

### 🚧 En desarrollo
- [ ] Calibración automática de potenciómetros
- [ ] Modo de configuración MIDI
- [ ] Interface web para configuración
- [ ] Logging avanzado

### 🎯 Planificado
- [ ] Soporte para múltiples dispositivos
- [ ] Integración con DAWs específicos
- [ ] Profiles de usuario guardados
- [ ] Firmware OTA updates

## 🤝 Contribución

1. Fork el proyecto
2. Crear branch para feature (`git checkout -b feature/nueva-caracteristica`)
3. **Ejecutar tests** (`make test`)
4. Commit cambios (`git commit -am 'Añadir nueva característica'`)
5. Push al branch (`git push origin feature/nueva-caracteristica`)
6. Crear Pull Request

### Estándares de código
- **Usar clang-format**: `make format`
- **Ejecutar tests**: Todos los tests deben pasar
- **Documentar funciones**: Comentarios descriptivos
- **Validar entrada**: Siempre validar parámetros MIDI

## 📝 Licencia

[Especificar licencia]

## 🙏 Agradecimientos

- Comunidad Arduino y PlatformIO
- Librerías de terceros utilizadas
- Testers y colaboradores

---

**¿Necesitas ayuda?** Abre un issue o contacta al equipo de desarrollo.

With [MidiView](https://hautetechnique.com/midi/midiview/)

## 🔧 Development Tools

### Code Quality

```bash
# Static analysis
make lint

# Memory usage analysis
make memory-check

# Code coverage (if available)
make coverage
```

### Build System

```bash
# Build for Arduino Mega
make build

# Upload to device
make upload

# Serial monitor
make monitor

# Clean build artifacts
make clean
make clean-all    # Includes test artifacts
```

### Documentation

```bash
# Generate documentation (requires Doxygen)
make docs

# Show all available commands
make help
```

## 📊 Performance Characteristics

### Timing Specifications

- **Main Loop**: Target 1ms cycle time
- **MIDI Processing**: < 100μs per message
- **LED Updates**: < 50μs per pixel
- **Button Debouncing**: 20ms default, configurable
- **Potentiometer Timeout**: 50ms default, configurable

### Memory Usage

- **RAM**: ~2KB for core functionality
- **Flash**: ~15KB for complete feature set
- **EEPROM**: ~512B for configuration storage

### Error Handling

All components implement comprehensive error reporting:

```cpp
// Example error handling pattern
if (!component.begin()) {
    ComponentError error = component.getLastError();
    // Handle specific error conditions
    component.clearError();
}
```
