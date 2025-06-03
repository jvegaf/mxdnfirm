# 📋 Tareas Pendientes - Mixduino Firmware

**Fecha:** 3 de Junio, 2025  
**Estado:** En desarrollo activo  
**Prioridad:** Arreglar tests críticos antes del release

---

## 🚨 Tareas Críticas (Alta Prioridad)

### 1. **Corregir Segmentation Faults**
- **Estado:** 🔴 CRÍTICO
- **Archivos afectados:** 
  - `test/test_mdcore/`
  - `test/test_integration/`
- **Descripción:** Tests crasheando con SIGSEGV durante la ejecución
- **Posibles causas:**
  - Punteros null o no inicializados
  - Acceso a memoria fuera de límites
  - Problemas en mocks de hardware (Shifter)
- **Comandos para reproducir:**
  ```bash
  make test-mdcore
  make test-integration
  ```

### 2. **Reparar BtnKit Tests (5 tests fallando)**
- **Estado:** 🔴 CRÍTICO
- **Tests fallando:**
  - `BtnKitTest.ButtonPressDetection`
  - `BtnKitTest.DebounceFiltering`
  - `BtnKitTest.InvertedLogicConfiguration`
  - `BtnKitTest.JustPressedAndReleasedDetection`
  - `BtnKitTest.GetLastPressTime`
- **Problemas identificados:**
  - Callback count esperado vs real (0 vs 1)
  - lastChannel y lastVelocity no se están configurando
  - Problemas con detección de estado de botones
- **Archivos:** `test/test_btnkit/test_btnkit.cpp`, `src/BtnKit.cpp`

### 3. **Corregir NPKit Configuration Tests**
- **Estado:** 🟡 ALTA
- **Tests fallando:**
  - `NPKitTest.InvalidConfiguration`
  - `NPKitTest.PixelOperationsWithoutInitialization`
- **Problemas:**
  - Error codes no se están configurando correctamente
  - Operaciones permitidas sin inicialización cuando deberían fallar
- **Archivos:** `test/test_npkit/test_npkit.cpp`, `src/NPKit.cpp`

---

## 🔧 Tareas de Media Prioridad

### 4. **Arreglar MuxPots Boundary Test**
- **Estado:** 🟡 MEDIA
- **Test fallando:** `MuxPotsTest.BoundaryValueTest`
- **Problema:** getCurrentValue(3) retorna -1 en lugar del valor esperado 0xFFFF
- **Archivo:** `test/test_muxpots/test_muxpots.cpp`

### 5. **Optimizar Mock Sistema**
- **Estado:** 🟡 MEDIA
- **Descripción:** Mejorar robustez de los mocks para evitar crashes
- **Archivos afectados:**
  - `test/mocks/Arduino.h`
  - `test/mocks/Shifter.cpp`
  - `test/mocks/MIDI.cpp`

---

## ✅ Componentes Funcionando Correctamente

### Tests que pasan exitosamente:
- ✅ **test_main** (7/7 tests) - Funciones principales MIDI
- ✅ **test_deck** (6/6 tests) - Control de decks
- ✅ **test_potkit** (18/18 tests) - Kit de potenciómetros
- ✅ **test_brencoder** - Encoders rotativos

---

## 📊 Estadísticas Actuales

| Componente | Estado | Tests Pasando | Tests Fallando | Total |
|------------|--------|---------------|----------------|-------|
| test_main | ✅ PASS | 7 | 0 | 7 |
| test_deck | ✅ PASS | 6 | 0 | 6 |
| test_potkit | ✅ PASS | 18 | 0 | 18 |
| test_brencoder | ✅ PASS | ? | 0 | ? |
| test_btnkit | 🔴 ERROR | 14 | 5 | 19 |
| test_npkit | 🔴 ERROR | 20 | 2 | 22 |
| test_muxpots | 🔴 ERROR | 16 | 1 | 17 |
| test_mdcore | 🔴 SEGFAULT | 2 | CRASH | ? |
| test_integration | 🔴 SEGFAULT | 0 | CRASH | ? |

**Total general:** 83 tests pasando, 8 fallando, 2 crasheando

---

## 🛠️ Plan de Acción Recomendado

### Fase 1: Estabilización (Semana 1)
1. **Día 1-2:** Corregir segmentation faults en mdcore e integration
2. **Día 3-4:** Arreglar BtnKit tests (callback y estado de botones)
3. **Día 5:** Corregir NPKit configuration tests

### Fase 2: Optimización (Semana 2)
1. **Día 1:** Arreglar MuxPots boundary test
2. **Día 2-3:** Mejorar sistema de mocks
3. **Día 4-5:** Tests adicionales y validación

### Fase 3: Validación Final
1. Ejecutar suite completa de tests
2. Validar en hardware real
3. Documentación y cleanup

---

## 📝 Comandos Útiles para Debugging

```bash
# Ejecutar tests específicos con output detallado
make test-verbose

# Tests individuales para debugging
make test-mdcore
make test-btnkit  
make test-npkit
make test-integration

# Solo tests que funcionan
make test-working

# Preparar entornos de test
python prepare_tests.py --all

# Limpiar y reconstruir
make clean && make test
```

---

## 🔍 Notas Técnicas

### Problemas Comunes Identificados:
1. **Mock State Management:** Los mocks no están manteniendo estado consistente entre tests
2. **Callback Registration:** Problemas con registro y ejecución de callbacks
3. **Error Code Propagation:** Los códigos de error no se están configurando correctamente
4. **Memory Management:** Posibles leaks o accesos inválidos en componentes hardware

### Archivos Clave a Revisar:
- `src/BtnKit.cpp` - Lógica de botones y callbacks
- `src/NPKit.cpp` - Manejo de errores y validación
- `src/MDCore.cpp` - Core MIDI (segfault)
- `test/mocks/` - Sistema de mocks
- `test/test_integration/` - Tests de integración

---

**Última actualización:** 3 de Junio, 2025 22:45  
**Próxima revisión:** Después de completar Fase 1
