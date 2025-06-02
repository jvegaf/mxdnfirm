#!/usr/bin/env python3
"""
Script to prepare test environments by regenerating from the master sources
"""

import os
import shutil
import argparse
import subprocess

def update_tests(test_name=None):
    """Run platformio to rebuild tests with the latest changes"""
    if test_name:
        print(f"Running tests with filter: test_{test_name}")
        subprocess.run(["pio", "test", "-e", "native", "-f", f"test_{test_name}"])
    else:
        print("Running all enabled tests")
        subprocess.run(["pio", "test", "-e", "native"])

def copy_source_files(test_name, source_files):
    """Copy source files and all mock files to a test directory"""
    test_dir = f"test/test_{test_name}"
    
    if not os.path.exists(test_dir):
        print(f"Error: Test directory {test_dir} does not exist")
        return False
    
    # Copy source files
    for source_file in source_files:
        src_path = f"src/{source_file}.cpp"
        if os.path.exists(src_path):
            shutil.copy2(src_path, test_dir)
            print(f"Copied {src_path} to {test_dir}")
        else:
            print(f"Warning: Source file {src_path} not found")
    
    # Copy only the needed mock files
    # Avoid copying Arduino.cpp/h to prevent redefinition errors
    mock_dir = "test/mocks"
    if os.path.exists(mock_dir):
        for file in os.listdir(mock_dir):
            if file.endswith('.cpp') and not file.startswith('Arduino'):
                src_path = os.path.join(mock_dir, file)
                shutil.copy2(src_path, test_dir)
                print(f"Copied mock {src_path} to {test_dir}")
    
    return True

def main():
    parser = argparse.ArgumentParser(description='Prepare test environments')
    parser.add_argument('--all', action='store_true', help='Prepare all test environments')
    parser.add_argument('--test', help='Prepare specific test environment')
    parser.add_argument('--update', action='store_true', help='Update and run tests')
    
    args = parser.parse_args()
    
    # Define which source files each test needs
    test_dependencies = {
        'brencoder': ['BREncoder'],
        'btnkit': ['BtnKit'],
        'deck': ['Deck', 'Muxer', 'Led', 'leds'],
        'integration': ['BREncoder', 'BtnKit', 'Deck', 'Led', 'leds', 'MDCore', 'MuxPots', 'NPKit', 'PotKit', 'SRKit', 'vumeters', 'vu'],
        'main': [],  # No specific dependencies
        'mdcore': ['MDCore', 'vumeters', 'vu', 'NPKit', 'Led', 'leds', 'Mux', 'Muxer'],
        'muxpots': ['MuxPots', 'Mux', 'Muxer'],
        'npkit': ['NPKit', 'Led', 'leds'],
        'potkit': ['PotKit', 'Mux', 'Muxer']
    }
    
    if args.update:
        # Run the tests directly
        update_tests(args.test)
        return
    
    if args.all:
        print("Preparing all test environments...")
        for test_name, sources in test_dependencies.items():
            print(f"\n--- Preparing test_{test_name} ---")
            copy_source_files(test_name, sources)
    elif args.test:
        test_name = args.test.replace('test_', '')  # Remove test_ prefix if provided
        if test_name in test_dependencies:
            print(f"Preparing test_{test_name}...")
            copy_source_files(test_name, test_dependencies[test_name])
        else:
            print(f"Error: Unknown test '{test_name}'. Available tests: {list(test_dependencies.keys())}")
    else:
        parser.print_help()

if __name__ == "__main__":
    main()
