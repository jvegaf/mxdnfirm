#!/usr/bin/env python3
"""
Script mejorado para arreglar los problemas de los tests

Este script resuelve dos problemas principales:
1. Evita redefiniciones de MockArduino
2. Asegura que las funciones de Arduino mock estén disponibles durante la compilación
"""

import os
import subprocess
import argparse
import shutil

def create_arduino_stub(test_dir):
    """
    Crea un archivo Arduino.cpp simplificado en el directorio de prueba
    que incluye el Arduino.h original pero sin redefinir clases
    """
    stub_content = """// Archivo Arduino.cpp generado automáticamente
#include "../mocks/Arduino.h"

// No se incluyen implementaciones aquí para evitar redefiniciones
// Las implementaciones están en test/mocks/Arduino.cpp
"""
    arduino_cpp_path = os.path.join(test_dir, "Arduino.cpp")
    with open(arduino_cpp_path, 'w', encoding='utf-8') as f:
        f.write(stub_content)
    print(f"Creado stub Arduino.cpp en {test_dir}")
    return True

def copy_arduino_mock_to_test(test_dir):
    """
    Copia Arduino.h y Arduino.cpp desde mocks a directorio de prueba
    asegurando que solo haya una implementación de MockArduino
    """
    # Verificar si ya existe Arduino.cpp y eliminarlo
    arduino_cpp_path = os.path.join(test_dir, "Arduino.cpp")
    if os.path.exists(arduino_cpp_path):
        os.remove(arduino_cpp_path)
        print(f"Eliminado Arduino.cpp duplicado en {test_dir}")
    
    # Crear un Arduino.cpp que solo incluya el Arduino.h
    create_arduino_stub(test_dir)
    
    return True

def prepare_tests():
    """Prepara todos los directorios de prueba"""
    test_base_dir = "test"
    test_dirs = []
    
    # Encontrar todos los directorios de prueba
    for item in os.listdir(test_base_dir):
        if item.startswith("test_") and os.path.isdir(os.path.join(test_base_dir, item)):
            test_dirs.append(os.path.join(test_base_dir, item))
    
    print(f"Preparando {len(test_dirs)} directorios de prueba...")
    
    for test_dir in test_dirs:
        copy_arduino_mock_to_test(test_dir)

def fix_btnkit_debounce():
    """Arregla los problemas de debounce en btnkit"""
    import re
    test_file = "test/test_btnkit/test_btnkit.cpp"
    backup_file = test_file + ".bak"
    
    # Crear copia de seguridad si no existe
    if not os.path.exists(backup_file):
        shutil.copy2(test_file, backup_file)
        print(f"Creada copia de seguridad: {backup_file}")
    
    with open(test_file, 'r', encoding='utf-8') as f:
        content = f.read()
    
    # 1. Fix ButtonPressDetection test
    pattern1 = r"(// Press button 0 \(set to LOW\)\n\s*MockArduino::setDigitalValue\(2, LOW\);\s*\n\s*EXPECT_TRUE\(btnKit->read\(testCallback, 1\)\);)"
    replacement1 = r"\1\n    \n    // Avanzar el tiempo para superar el debounce\n    MockArduino::addMillis(20);"
    content = re.sub(pattern1, replacement1, content)
    
    pattern2 = r"(// Release button 0 \(set to HIGH\)\n\s*MockArduino::setDigitalValue\(2, HIGH\);\s*\n\s*EXPECT_TRUE\(btnKit->read\(testCallback, 1\)\);)"
    replacement2 = r"\1\n    \n    // Avanzar el tiempo para superar el debounce\n    MockArduino::addMillis(20);"
    content = re.sub(pattern2, replacement2, content)
    
    # 2. Fix DebounceFiltering test
    pattern3 = r"(MockArduino::setDigitalValue\(2, LOW\);\s*\n\s*EXPECT_TRUE\(btnKit->read\(testCallback, 0\)\);)"
    replacement3 = r"\1\n    \n    // Avanzar el tiempo para superar el debounce\n    MockArduino::addMillis(101);"
    content = re.sub(pattern3, replacement3, content)
    
    pattern4 = r"(MockArduino::addMillis\(10\); // Only 10ms\s*\n\s*MockArduino::setDigitalValue\(2, LOW\);)"
    replacement4 = r"\1"
    content = re.sub(pattern4, replacement4, content)
    
    # 3. Fix InvertedLogicConfiguration test
    pattern5 = r"(MockArduino::setDigitalValue\(2, HIGH\);\s*\n\s*EXPECT_TRUE\(btnKit->read\(testCallback, 0\)\);)"
    replacement5 = r"\1\n    \n    // Avanzar el tiempo para superar el debounce\n    MockArduino::addMillis(20);"
    content = re.sub(pattern5, replacement5, content)
    
    # 4. Fix JustPressedAndReleasedDetection test
    pattern6 = r"(// Press button\s*\n\s*MockArduino::setDigitalValue\(2, LOW\);\s*\n\s*EXPECT_TRUE\(btnKit->read\(testCallback, 0\)\);)"
    replacement6 = r"\1\n    \n    // Avanzar el tiempo para superar el debounce\n    MockArduino::addMillis(20);"
    content = re.sub(pattern6, replacement6, content)
    
    pattern7 = r"(// Release button\s*\n\s*MockArduino::setDigitalValue\(2, HIGH\);\s*\n\s*EXPECT_TRUE\(btnKit->read\(testCallback, 0\)\);)"
    replacement7 = r"\1\n    \n    // Avanzar el tiempo para superar el debounce\n    MockArduino::addMillis(20);"
    content = re.sub(pattern7, replacement7, content)

    # Escribir contenido actualizado
    with open(test_file, 'w', encoding='utf-8') as f:
        f.write(content)
    
    print(f"Archivo {test_file} actualizado.")

def run_test(test_name):
    """Ejecuta un test específico utilizando platformio"""
    print(f"\nEjecutando test_{test_name}...")
    cmd = ["pio", "test", "-e", "native", "-f", f"test_{test_name}"]
    result = subprocess.run(cmd, capture_output=True, text=True)
    
    # Mostrar la salida del comando
    print(result.stdout)
    if result.stderr:
        print("Errores:")
        print(result.stderr)
    
    return result.returncode == 0

def main():
    parser = argparse.ArgumentParser(description='Script de arreglo de tests')
    parser.add_argument('--prepare', action='store_true', help='Prepara todos los directorios de prueba')
    parser.add_argument('--fix-btnkit', action='store_true', help='Arregla los problemas de debounce en btnkit')
    parser.add_argument('--run', help='Ejecuta un test específico')
    parser.add_argument('--run-all', action='store_true', help='Ejecuta todos los tests')
    parser.add_argument('--copy-arduino-h', action='store_true', help='Copia Arduino.h en todos los directorios de prueba')
    
    args = parser.parse_args()
    
    if args.prepare:
        prepare_tests()
    
    if args.fix_btnkit:
        fix_btnkit_debounce()
    
    if args.run:
        run_test(args.run)
    
    if args.run_all:
        tests = ['brencoder', 'deck', 'main', 'mdcore', 'potkit', 'btnkit', 'muxpots', 'npkit']
        results = {}
        
        for test_name in tests:
            results[test_name] = run_test(test_name)
        
        # Mostrar resumen
        print("\n=== RESUMEN DE PRUEBAS ===")
        for test_name, success in results.items():
            status = "ÉXITO" if success else "FALLO"
            print(f"test_{test_name}: {status}")
    
    if args.copy_arduino_h:
        # Este comando es sólo para depuración
        test_base_dir = "test"
        for item in os.listdir(test_base_dir):
            if item.startswith("test_") and os.path.isdir(os.path.join(test_base_dir, item)):
                test_dir = os.path.join(test_base_dir, item)
                src_file = os.path.join("test", "mocks", "Arduino.h")
                dst_file = os.path.join(test_dir, "Arduino.h")
                shutil.copy2(src_file, dst_file)
                print(f"Copiado Arduino.h a {test_dir}")

if __name__ == "__main__":
    main()
