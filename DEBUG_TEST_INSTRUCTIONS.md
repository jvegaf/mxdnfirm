# Instrucciones para depurar las pruebas unitarias

## Problemas identificados

Durante el análisis de las pruebas que fallan, se han identificado los siguientes problemas:

1. **Problemas de tiempo (debounce) en BtnKit**
   - Las pruebas que fallan están relacionadas con el manejo del debounce
   - La función `millis()` en las pruebas no avanza lo suficiente entre lecturas

2. **Problemas con la lógica invertida en NPKit**
   - Algunas pruebas utilizan valores no válidos para la configuración
   - Hay pruebas con estados inconsistentes

3. **Problemas con el manejo de índices en MuxPots**
   - Las pruebas de boundary value fallan
   - El manejo de errores con índices fuera de rango no está funcionando correctamente

## Soluciones propuestas

### Para BtnKit

En los tests que fallan, añade código para avanzar manualmente el tiempo entre las pruebas:

```cpp
// Después de cambiar el estado de un botón:
MockArduino::addMillis(config.debounceDelay + 10);
```

### Para NPKit

Revisa las pruebas que están utilizando valores no válidos y asegúrate de que el comportamiento esperado sea coherente:

```cpp
// Ejemplo de modificación:
config.brightness = 255; // Usar un valor válido
```

### Para MuxPots

Verifica que los valores límite sean correctos:

```cpp
// Ejemplo de corrección:
MuxPotsTest.BoundaryValueTest - asegúrate de que los valores estén dentro del rango adecuado
```

## Próximos pasos

1. Soluciona las pruebas una a una, empezando por las más básicas.
2. Rehabilita gradualmente las pruebas en test_mdcore.
3. Una vez que todas las pruebas estén pasando, actualiza el test_filter en platformio.ini para incluir todas las pruebas.

## Comentarios sobre las implementaciones

- **BtnKit**: El problema principal parece ser que las pruebas están asumiendo que el tiempo avanza automáticamente, pero en las pruebas unitarias necesitamos avanzarlo manualmente.
- **NPKit**: Las pruebas están utilizando valores que deberían causar errores, pero la implementación no está manejando estos casos correctamente.
- **MuxPots**: El principal problema parece ser con el manejo de índices y límites.
