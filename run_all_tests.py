#!/usr/bin/env python3
"""
Script completo para ejecutar y corregir todas las pruebas del proyecto

Este script:
1. Limpia la compilación anterior
2. Prepara los directorios de prueba
3. Aplica correcciones específicas (como el fix de debounce para btnkit)
4. Ejecuta las pruebas seleccionadas
5. Muestra un informe de resultados
"""

import os
import sys
import subprocess
import argparse
import re
import shutil
import time

def clean_build():
    """Limpia las compilaciones previas para asegurar un entorno limpio"""
    print("\n--- Limpiando compilaciones previas ---")
    cmd = ["pio", "run", "-t", "clean"]
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        print("Error al limpiar compilaciones:")
        print(result.stderr)
        return False
    print("Compilaciones limpiadas correctamente")
    return True

def prepare_test_dirs():
    """Prepara los directorios de test usando test_setup.py"""
    print("\n--- Preparando directorios de prueba ---")
    cmd = ["python", "test_setup.py"]
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        print("Error al preparar directorios:")
        print(result.stderr)
        return False
    print("Directorios de prueba preparados correctamente")
    return True

def fix_btnkit():
    """Aplica correcciones específicas a test_btnkit"""
    print("\n--- Aplicando correcciones a test_btnkit ---")
    cmd = ["python", "fix_tests.py", "--btnkit"]
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        print("Error al aplicar correcciones a btnkit:")
        print(result.stderr)
        return False
    print("Correcciones aplicadas a btnkit correctamente")
    return True

def run_test(test_name=None):
    """Ejecuta un test específico o todos los habilitados"""
    if test_name:
        print(f"\n--- Ejecutando test_{test_name} ---")
        cmd = ["pio", "test", "-e", "native", "-f", f"test_{test_name}"]
    else:
        print("\n--- Ejecutando todos los tests habilitados ---")
        cmd = ["pio", "test", "-e", "native"]
    
    start_time = time.time()
    result = subprocess.run(cmd, capture_output=False, text=True)
    elapsed_time = time.time() - start_time
    
    status = "ÉXITO" if result.returncode == 0 else "FALLO"
    print(f"\nEjecución completada con estado: {status}")
    print(f"Tiempo total: {elapsed_time:.2f} segundos")
    
    return result.returncode == 0

def parse_args():
    parser = argparse.ArgumentParser(description='Ejecuta y corrige pruebas del proyecto')
    parser.add_argument('--test', help='Ejecuta un test específico (sin el prefijo test_)')
    parser.add_argument('--skip-clean', action='store_true', help='Omite la limpieza de compilaciones')
    parser.add_argument('--skip-fix', action='store_true', help='Omite la aplicación de correcciones')
    parser.add_argument('--everything', action='store_true', help='Ejecuta absolutamente todas las pruebas')
    
    return parser.parse_args()

def list_available_tests():
    """Lista todos los tests disponibles en el proyecto"""
    test_dirs = []
    for item in os.listdir("test"):
        if item.startswith("test_") and os.path.isdir(os.path.join("test", item)):
            test_dirs.append(item.replace("test_", ""))
    
    print("\n--- Tests disponibles ---")
    for test in sorted(test_dirs):
        print(f"- {test}")
    
    return test_dirs

def main():
    args = parse_args()
    
    print("\n========== EJECUCIÓN DE PRUEBAS MXDNFIRM ==========\n")
    
    # Listar tests disponibles
    available_tests = list_available_tests()
    
    # Validar nombre de test si se especificó
    if args.test and args.test not in available_tests:
        print(f"Error: El test '{args.test}' no existe en el proyecto")
        print("Utilice uno de los tests disponibles listados arriba")
        return 1
    
    # Limpiar compilaciones previas
    if not args.skip_clean:
        if not clean_build():
            return 1
    
    # Preparar directorios de test
    if not prepare_test_dirs():
        return 1
    
    # Aplicar correcciones específicas
    if not args.skip_fix:
        if args.test == "btnkit" or not args.test:
            if not fix_btnkit():
                return 1
    
    # Modificar platformio.ini si se especificó --everything
    if args.everything:
        print("\n--- Configurando para ejecutar todas las pruebas ---")
        
        # TODO: Implementar modificación temporal de platformio.ini 
        # para quitar test_filter y test_ignore
        pass
    
    # Ejecutar los tests
    success = run_test(args.test)
    
    # Mostrar mensaje final
    if success:
        print("\n✅ Todas las pruebas ejecutadas correctamente")
    else:
        print("\n❌ Algunas pruebas han fallado")
    
    return 0 if success else 1

if __name__ == "__main__":
    sys.exit(main())
