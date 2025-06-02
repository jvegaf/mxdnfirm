#!/usr/bin/env python3
"""
Script para limpiar archivos duplicados en los directorios de prueba
"""

import os
import argparse
import re

def clean_arduino_cpp(test_dir):
    """Elimina las redefiniciones de MockArduino en los archivos Arduino.cpp copiados"""
    arduino_file = os.path.join(test_dir, "Arduino.cpp")
    
    if not os.path.exists(arduino_file):
        print(f"No se encontró el archivo {arduino_file}")
        return False
    
    # Crear un nuevo archivo Arduino.cpp con solo las funciones básicas
    new_content = """// Archivo limpiado para evitar redefiniciones
#include "Arduino.h"
"""
    
    with open(arduino_file, 'w', encoding='utf-8') as f:
        f.write(new_content)
    
    with open(arduino_file, 'w', encoding='utf-8') as f:
        f.write(content)
    
    print(f"Archivo {arduino_file} limpiado.")
    return True

def main():
    parser = argparse.ArgumentParser(description='Limpia archivos duplicados en los directorios de prueba')
    parser.add_argument('--all', action='store_true', help='Limpia todos los directorios de prueba')
    parser.add_argument('--test', help='Limpia un directorio de prueba específico')
    
    args = parser.parse_args()
    
    test_dirs = []
    
    if args.all:
        # Buscar todos los directorios test_*
        for item in os.listdir('test'):
            if os.path.isdir(os.path.join('test', item)) and item.startswith('test_'):
                test_dirs.append(os.path.join('test', item))
    elif args.test:
        test_name = args.test
        if not test_name.startswith('test_'):
            test_name = f"test_{test_name}"
        test_dir = os.path.join('test', test_name)
        if os.path.isdir(test_dir):
            test_dirs.append(test_dir)
        else:
            print(f"Error: No se encontró el directorio {test_dir}")
            return
    else:
        parser.print_help()
        return
    
    for test_dir in test_dirs:
        print(f"Limpiando {test_dir}...")
        clean_arduino_cpp(test_dir)

if __name__ == "__main__":
    main()
