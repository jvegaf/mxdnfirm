# MIDI Controller Firmware Makefile
# CREATED BY VIM-PIO

# Build targets
all:
	platformio -f -c vim run

upload:
	platformio -f -c vim run --target upload

clean:
	platformio -f -c vim run --target clean

program:
	platformio -f -c vim run --target program

uploadfs:
	platformio -f -c vim run --target uploadfs

compiledb:
	pio run -t compiledb

# Testing targets
test:
	pio test -e native

test-all:
	python prepare_tests.py --all
	pio test -e native

test-prepare:
	python prepare_tests.py --all

# Individual test targets
test-brencoder:
	python prepare_tests.py --test test_brencoder
	pio test -e native --filter test_brencoder

test-btnkit:
	python prepare_tests.py --test test_btnkit
	pio test -e native --filter test_btnkit

test-deck:
	python prepare_tests.py --test test_deck
	pio test -e native --filter test_deck

test-integration:
	python prepare_tests.py --test test_integration
	pio test -e native --filter test_integration

test-main:
	python prepare_tests.py --test test_main
	pio test -e native --filter test_main

test-mdcore:
	python prepare_tests.py --test test_mdcore
	pio test -e native --filter test_mdcore

test-muxpots:
	python prepare_tests.py --test test_muxpots
	pio test -e native --filter test_muxpots

test-npkit:
	python prepare_tests.py --test test_npkit
	pio test -e native --filter test_npkit

test-potkit:
	python prepare_tests.py --test test_potkit
	pio test -e native --filter test_potkit

# Working tests only
test-working:
	pio test -e native --filter "test_brencoder|test_main|test_deck"

# Verbose testing
test-verbose:
	pio test -e native -v

# Help target
help:
	@echo "Available targets:"
	@echo "  all                 - Build firmware"
	@echo "  upload              - Upload firmware to device"
	@echo "  clean               - Clean build files"
	@echo "  program             - Program device"
	@echo "  uploadfs            - Upload filesystem"
	@echo "  compiledb           - Generate compile commands"
	@echo ""
	@echo "Testing targets:"
	@echo "  test                - Run all native tests"
	@echo "  test-all            - Prepare and run all tests"
	@echo "  test-prepare        - Prepare all test environments"
	@echo "  test-working        - Run only working tests"
	@echo "  test-verbose        - Run tests with verbose output"
	@echo ""
	@echo "Individual test targets:"
	@echo "  test-brencoder      - Test BREncoder component"
	@echo "  test-btnkit         - Test BtnKit component"
	@echo "  test-deck           - Test Deck component"
	@echo "  test-integration    - Test integration scenarios"
	@echo "  test-main           - Test main MIDI controller functions"
	@echo "  test-mdcore         - Test MDCore component"
	@echo "  test-muxpots        - Test MuxPots component"
	@echo "  test-npkit          - Test NPKit component"
	@echo "  test-potkit         - Test PotKit component"

.PHONY: all upload clean program uploadfs compiledb test test-all test-prepare test-working test-verbose test-brencoder test-btnkit test-deck test-integration test-main test-mdcore test-muxpots test-npkit test-potkit help
