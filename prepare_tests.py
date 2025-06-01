#!/usr/bin/env python3
"""
Script to prepare test environments by copying necessary source files
"""

import os
import shutil
import argparse

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
    
    # Copy all mock files
    mock_dir = "test/mocks"
    if os.path.exists(mock_dir):
        for file in os.listdir(mock_dir):
            if file.endswith('.cpp'):
                src_path = os.path.join(mock_dir, file)
                shutil.copy2(src_path, test_dir)
                print(f"Copied mock {src_path} to {test_dir}")
    
    return True

def main():
    parser = argparse.ArgumentParser(description='Prepare test environments')
    parser.add_argument('--all', action='store_true', help='Prepare all test environments')
    parser.add_argument('--test', help='Prepare specific test environment')
    
    args = parser.parse_args()
    
    # Define which source files each test needs
    test_dependencies = {
        'brencoder': ['BREncoder'],
        'btnkit': ['BtnKit'],
        'deck': ['Deck', 'Muxer', 'Led', 'leds'],
        'integration': ['BREncoder', 'BtnKit', 'Deck', 'Led', 'leds', 'MDCore', 'MuxPots', 'NPKit', 'PotKit', 'SRKit', 'vumeters', 'vu'],
        'main': [],  # No specific dependencies
        'mdcore': ['MDCore', 'vumeters', 'vu', 'NPKit', 'Led', 'leds'],
        'muxpots': ['MuxPots', 'Mux', 'Muxer'],
        'npkit': ['NPKit', 'Led', 'leds'],
        'potkit': ['PotKit', 'Mux', 'Muxer']
    }
    
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
