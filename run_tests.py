#!/usr/bin/env python3
"""
Script para ejecutar pruebas específicas de PlatformIO
"""

import argparse
import subprocess
import os

def run_tests(tests, prepare=False):
    """Ejecuta pruebas especificadas después de prepararlas si es necesario"""
    if not tests:
        print("No se especificaron pruebas. Ejecutando todas las pruebas estables.")
        tests = ["brencoder", "deck", "main", "mdcore", "potkit"]
    
    for test in tests:
        test_name = test.replace("test_", "")  # Eliminar prefijo si existe
        
        # Ejecutar la prueba
        print(f"\n=== Ejecutando test_{test_name} ===")
        subprocess.run(["pio", "test", "-e", "native", "-f", f"test_{test_name}"], check=True)

def main():
    parser = argparse.ArgumentParser(description='Ejecutar pruebas de PlatformIO')
    parser.add_argument('--tests', nargs='+', help='Lista de pruebas a ejecutar (sin el prefijo test_)')
    parser.add_argument('--all', action='store_true', help='Ejecutar todas las pruebas')
    parser.add_argument('--fix-debounce', action='store_true', help='Corregir problemas de debounce en tests')
    
    args = parser.parse_args()
    
    tests = args.tests
    
    # Si se especifica --all, ejecutar todas las pruebas
    if args.all:
        tests = ["brencoder", "btnkit", "deck", "integration", "main", "mdcore", "muxpots", "npkit", "potkit"]
    
    run_tests(tests)

if __name__ == "__main__":
    main()
