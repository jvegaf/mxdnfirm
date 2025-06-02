# Instrucciones para las pruebas

## Problemas conocidos

Los tests en este proyecto enfrentan tres problemas principales:

1. **Redefiniciones de MockArduino**: Los tests incluyen copias individuales de Arduino.cpp, lo que causa redefiniciones.
2. **Problemas de debounce en BtnKit**: Algunos tests fallan porque no espera lo suficiente entre operaciones.
3. **Referencias a símbolos indefinidos**: Algunos tests no incluyen correctamente las implementaciones de mock.

## Cómo ejecutar las pruebas

### Paso 1: Limpiar y preparar el entorno

```bash
# Limpiar las carpetas de compilación
pio run -t clean
```

### Paso 2: Arreglar tests específicos

```bash
# Arreglar problemas de debounce en btnkit
python fix_tests.py --btnkit
```

### Paso 3: Ejecutar las pruebas

```bash
# Para ejecutar un test específico:
pio test -e native -f test_main

# Para ejecutar todos los tests estables:
pio test -e native
```

## Tests disponibles

- **test_brencoder**: Tests para los encoders rotativos.
- **test_btnkit**: Tests para los botones. Requiere fix_tests.py para solucionar problemas de debounce.
- **test_deck**: Tests para el componente Deck.
- **test_main**: Tests para las funciones principales. Estos tests son estables.
- **test_mdcore**: Tests para el núcleo MIDI. Algunos tests están temporalmente deshabilitados.
- **test_potkit**: Tests para potenciómetros.
- **test_muxpots**: Tests para potenciómetros multiplexados.
- **test_npkit**: Tests para NeoPixels.
- **test_integration**: Tests de integración. Estos tests requieren más trabajo.

## Soluciones implementadas

1. **Script test_setup.py**: Se ejecuta automáticamente antes de las pruebas para preparar los directorios.
2. **Script fix_tests.py**: Corrige problemas específicos en test_btnkit.
3. **Arduino.cpp simplificado**: Se crea un stub en cada directorio de prueba que solo incluye los encabezados.

## Comandos útiles

```bash
# Ver ayuda de platformio
pio --help

# Ver opciones de test
pio test --help

# Limpiar y ejecutar test con fix para btnkit
pio run -t clean && python fix_tests.py --btnkit && pio test -e native -f test_btnkit

# Ejecutar todos los tests estables
pio test -e native
```

## Estado actual de los tests

### Tests funcionando correctamente
- test_main: Todas las pruebas pasan
- Otros tests necesitan la implementación de las soluciones mencionadas

### Tests con problemas conocidos
- btnkit: Necesita fix_tests.py para resolver problemas de debounce
- muxpots: Fallan algunas pruebas (2 de 17)
- npkit: Fallan algunas pruebas (2 de 22)

## Próximos pasos

1. Integrar las soluciones en el proceso de CI/CD
2. Rehabilitar las pruebas comentadas en mdcore
3. Continuar mejorando los tests que aún fallan
- deck
- main
- mdcore (pruebas básicas)
- potkit

## Pruebas con problemas

Las siguientes pruebas tienen algunos problemas y necesitan ser revisadas:
- btnkit (5 pruebas fallan de 19)
- muxpots (2 pruebas fallan de 17)
- npkit (2 pruebas fallan de 22)

## Próximos pasos

1. Revisar y arreglar las pruebas que fallan en btnkit
2. Revisar y arreglar las pruebas que fallan en muxpots
3. Revisar y arreglar las pruebas que fallan en npkit
4. Rehabilitar las pruebas comentadas en mdcore
