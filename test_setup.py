#!/usr/bin/env python3
"""
Script para configurar el entorno de pruebas

Este script se ejecuta automáticamente antes de compilar los tests gracias a
la configuración extra_scripts en platformio.ini. Prepara todos los directorios
de test eliminando los archivos Arduino.cpp que causan redefiniciones y creando
stubs que usan nuestra implementación de ArduinoImpl.h.
"""

import os
import shutil

def prepare_test_dirs():
    """Prepara todos los directorios de prueba"""
    test_base_dir = "test"
    test_dirs = []
    
    # Encontrar todos los directorios de prueba
    for item in os.listdir(test_base_dir):
        if item.startswith("test_") and os.path.isdir(os.path.join(test_base_dir, item)):
            test_dirs.append(os.path.join(test_base_dir, item))
    
    print(f"Preparando {len(test_dirs)} directorios de prueba...")
    
    # Crear un nuevo Arduino.cpp para cada directorio de prueba
    for test_dir in test_dirs:
        # Eliminar Arduino.cpp si existe
        arduino_cpp = os.path.join(test_dir, "Arduino.cpp")
        if os.path.exists(arduino_cpp):
            os.remove(arduino_cpp)
            print(f"Eliminado Arduino.cpp en {test_dir}")
        
        # Crear un nuevo Arduino.cpp que define la implementación una sola vez
        stub_content = """// Arduino.cpp generado automáticamente para test
#define TEST_ARDUINO_IMPLEMENTATION
#include "../mocks/ArduinoImpl.h"
"""
        with open(arduino_cpp, 'w', encoding='utf-8') as f:
            f.write(stub_content)
        print(f"Creado stub Arduino.cpp en {test_dir}")
    
    return True

if __name__ == "__main__":
    print("Ejecutando configuración de tests manualmente...")
    prepare_test_dirs()
else:
    # Este script se ejecuta antes de la compilación como parte de extra_scripts
    prepare_test_dirs()
