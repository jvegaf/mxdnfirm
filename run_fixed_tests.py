#!/usr/bin/env python3
"""
Script para eliminar los archivos Arduino.cpp duplicados y ejecutar las pruebas
"""

import os
import sys
import subprocess
import argparse

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

def prepare_test_dirs(test_dirs):
    """Prepara los directorios de prueba creando stubs de Arduino.cpp"""
    for test_dir in test_dirs:
        arduino_file = os.path.join(test_dir, "Arduino.cpp")
        if os.path.exists(arduino_file):
            os.remove(arduino_file)
            print(f"Eliminado original: {arduino_file}")
        create_arduino_stub(test_dir)

def run_tests(tests):
    """Ejecuta las pruebas especificadas y devuelve un diccionario con los resultados"""
    results = {}
    success_count = 0
    failure_count = 0
    
    for test in tests:
        test_name = test.replace("test_", "")
        cmd = ["pio", "test", "-e", "native", "-f", f"test_{test_name}"]
        print(f"\nEjecutando: {' '.join(cmd)}")
        result = subprocess.run(cmd, capture_output=True, text=True)
        
        # Mostrar la salida del comando
        print(result.stdout)
        if result.stderr:
            print("Errores:")
            print(result.stderr)
        
        # Almacenar el resultado
        if result.returncode == 0:
            success_count += 1
            results[test_name] = "ÉXITO"
        else:
            failure_count += 1
            results[test_name] = "FALLO"
    
    # Mostrar resumen
    print("\n=== RESUMEN DE PRUEBAS ===")
    for test_name, result in results.items():
        print(f"test_{test_name}: {result}")
    
    print(f"\nTotal: {success_count + failure_count}, Éxitos: {success_count}, Fallos: {failure_count}")
    
    return success_count, failure_count, results

def main():
    parser = argparse.ArgumentParser(description='Ejecutar pruebas eliminando archivos Arduino.cpp duplicados')
    parser.add_argument('--all', action='store_true', help='Ejecutar todas las pruebas habilitadas')
    parser.add_argument('--everything', action='store_true', help='Ejecutar absolutamente todas las pruebas')
    parser.add_argument('--tests', nargs='+', help='Lista de pruebas específicas a ejecutar')
    parser.add_argument('--debug', action='store_true', help='Mostrar información adicional de depuración')
    
    args = parser.parse_args()
    
    print("Iniciando script de pruebas arregladas...")
    
    # Determinar qué pruebas ejecutar
    if args.everything:
        tests = [
            "test_brencoder", 
            "test_deck", 
            "test_main", 
            "test_mdcore", 
            "test_potkit",
            "test_btnkit",
            "test_muxpots",
            "test_npkit",
            "test_integration"
        ]
        print(f"Modo: todas las pruebas disponibles ({len(tests)} pruebas)")
    elif args.all:
        tests = [
            "test_brencoder", 
            "test_deck", 
            "test_main", 
            "test_mdcore", 
            "test_potkit"
        ]
        print(f"Modo: todas las pruebas estables ({len(tests)} pruebas)")
    elif args.tests:
        tests = [f"test_{t}" if not t.startswith("test_") else t for t in args.tests]
        print(f"Modo: pruebas específicas ({len(tests)} pruebas): {', '.join(tests)}")
    else:
        print("Debe especificar --all, --everything o --tests")
        sys.exit(1)
    
    # Obtener directorios de prueba
    test_dirs = [os.path.join("test", t) for t in tests]
    
    # Verificar que los directorios existen
    for test_dir in test_dirs:
        if not os.path.exists(test_dir):
            print(f"Error: Directorio no encontrado: {test_dir}")
            sys.exit(1)
        elif args.debug:
            print(f"Directorio encontrado: {test_dir}")
      # Preparar los directorios de prueba
    print("\nPreparando directorios de prueba...")
    prepare_test_dirs(test_dirs)
    
    # Ejecutar las pruebas
    print("\nEjecutando pruebas...")
    run_tests(tests)

if __name__ == "__main__":
    main()
