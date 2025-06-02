#!/usr/bin/env python3
"""
Script para preparar y ejecutar pruebas con el enfoque correcto
"""

import os
import subprocess
import argparse
import shutil
import re

def copy_arduino_mock(test_dir):
    """Copia el Arduino.h y Arduino.cpp del directorio mocks al directorio de prueba"""
    mock_dir = "test/mocks"
    
    # Copiar Arduino.h
    shutil.copy2(os.path.join(mock_dir, "Arduino.h"), os.path.join(test_dir, "Arduino.h"))
    print(f"Copiado Arduino.h a {test_dir}")
    
    # Copiar Arduino.cpp
    shutil.copy2(os.path.join(mock_dir, "Arduino.cpp"), os.path.join(test_dir, "Arduino.cpp"))
    print(f"Copiado Arduino.cpp a {test_dir}")
    
    return True

def fix_btnkit_debounce():
    """Arregla los problemas de debounce en btnkit"""
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
    """Ejecuta un test específico"""
    print(f"\nPreparando test_{test_name}...")
    
    test_dir = os.path.join("test", f"test_{test_name}")
    if not os.path.exists(test_dir):
        print(f"Error: Directorio {test_dir} no encontrado.")
        return False
    
    # Preparar el directorio de prueba
    copy_arduino_mock(test_dir)
    
    # Aplicar fixes específicos
    if test_name == "btnkit":
        fix_btnkit_debounce()
    
    # Ejecutar el test
    print(f"\nEjecutando test_{test_name}...")
    cmd = ["pio", "test", "-e", "native", "-f", f"test_{test_name}"]
    result = subprocess.run(cmd, capture_output=True, text=True)
    
    # Mostrar la salida del comando
    print(result.stdout)
    if result.stderr:
        print("Errores:")
        print(result.stderr)
    
    return result.returncode == 0

def run_all_tests():
    """Ejecuta todas las pruebas"""
    tests = ['brencoder', 'btnkit', 'deck', 'main', 'mdcore', 'potkit']
    results = {}
    
    for test in tests:
        success = run_test(test)
        results[test] = "ÉXITO" if success else "FALLO"
    
    print("\n=== RESUMEN DE PRUEBAS ===")
    success_count = 0
    failure_count = 0
    
    for test_name, result in results.items():
        print(f"test_{test_name}: {result}")
        if result == "ÉXITO":
            success_count += 1
        else:
            failure_count += 1
    
    print(f"\nTotal: {len(tests)}, Éxitos: {success_count}, Fallos: {failure_count}")

def main():
    parser = argparse.ArgumentParser(description='Preparar y ejecutar pruebas')
    parser.add_argument('--test', help='Ejecuta un test específico')
    parser.add_argument('--all', action='store_true', help='Ejecuta todos los tests')
    parser.add_argument('--copy-mock', help='Copia Arduino mock al directorio especificado')
    parser.add_argument('--fix-btnkit', action='store_true', help='Arregla problemas de debounce en btnkit')
    
    args = parser.parse_args()
    
    if args.test:
        run_test(args.test)
    elif args.all:
        run_all_tests()
    elif args.copy_mock:
        test_dir = os.path.join("test", args.copy_mock)
        if not os.path.exists(test_dir):
            test_dir = os.path.join("test", f"test_{args.copy_mock}")
            if not os.path.exists(test_dir):
                print(f"Error: Directorio {test_dir} no encontrado.")
                return
        copy_arduino_mock(test_dir)
    elif args.fix_btnkit:
        fix_btnkit_debounce()
    else:
        parser.print_help()

if __name__ == "__main__":
    main()
