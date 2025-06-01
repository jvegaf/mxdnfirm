# 🎛️ Contributing to Mixduino

Thank you for your interest in contributing to the Mixduino project! This document provides guidelines for contributing to make the process smooth for everyone.

## 🚀 Quick Start

1. **Fork** the repository
2. **Clone** your fork locally
3. **Create** a feature branch: `git checkout -b feature/amazing-feature`
4. **Make** your changes
5. **Test** your changes: `python prepare_tests.py --all && pio test`
6. **Commit** your changes: `git commit -m 'feat: add amazing feature'`
7. **Push** to your branch: `git push origin feature/amazing-feature`
8. **Open** a Pull Request

## 📋 Development Setup

### Prerequisites
- **Python 3.7+** with PlatformIO
- **Arduino Mega 2560** for testing
- **Git** for version control

### Environment Setup
```bash
# Clone the repository
git clone https://github.com/your-username/mxdnfirm.git
cd mxdnfirm

# Install PlatformIO
pip install platformio

# Install dependencies
pio lib install

# Run tests
python prepare_tests.py --all
pio test -e native
```

## 🎯 Types of Contributions

### 🐛 Bug Fixes
- Fix existing issues
- Improve error handling
- Performance optimizations

### ✨ New Features
- MIDI functionality enhancements
- Hardware support
- User interface improvements

### 📚 Documentation
- Code documentation
- User guides
- Hardware setup instructions

### 🧪 Testing
- Unit tests
- Integration tests
- Hardware validation

## 📝 Coding Standards

### C++ Code Style
```cpp
// Use descriptive names
class MidiController {
public:
    // Public members first
    void processInput();
    
private:
    // Private members last
    uint8_t midiChannel;
};

// Constants in UPPER_CASE
const uint8_t MAX_MIDI_VALUE = 127;

// Functions in camelCase
void sendMidiMessage(uint8_t channel, uint8_t value);
```

### Commit Messages
Use [Conventional Commits](https://www.conventionalcommits.org/):

```
feat: add new MIDI mapping feature
fix: resolve encoder debouncing issue
docs: update installation instructions
test: add unit tests for potentiometer class
refactor: improve code organization
style: fix code formatting
chore: update dependencies
```

### Code Organization
```
src/
├── main.cpp          # Main application entry
├── Core/             # Core MIDI functionality
├── Hardware/         # Hardware abstraction
├── Controllers/      # Input controllers
└── Utils/           # Utility functions

include/
├── config.h         # Configuration constants
├── pin_map.h        # Pin mappings
└── midi_map.h       # MIDI mappings

test/
├── unit/            # Unit tests
├── integration/     # Integration tests
└── mocks/          # Mock objects
```

## 🧪 Testing Guidelines

### Running Tests
```bash
# Prepare test environment
python prepare_tests.py --all

# Run all tests
pio test -e native --verbose

# Run specific test
python prepare_tests.py --test muxpots
pio test -e native -f test_muxpots
```

### Writing Tests
```cpp
#include <gtest/gtest.h>
#include "YourClass.h"

class YourClassTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code
    }
    
    void TearDown() override {
        // Cleanup code
    }
};

TEST_F(YourClassTest, ShouldDoSomething) {
    // Arrange
    YourClass instance;
    
    // Act
    auto result = instance.doSomething();
    
    // Assert
    EXPECT_EQ(expected_value, result);
}
```

## 🔍 Code Review Process

### Before Submitting
- [ ] Code compiles without warnings
- [ ] All tests pass
- [ ] Code follows style guidelines
- [ ] Documentation is updated
- [ ] Commit messages are descriptive

### Review Criteria
- **Functionality**: Does it work as intended?
- **Performance**: Is it efficient?
- **Maintainability**: Is it easy to understand and modify?
- **Testing**: Are there adequate tests?
- **Documentation**: Is it well documented?

## 🏷️ Pull Request Guidelines

### PR Title
Use conventional commit format:
```
feat: add support for custom MIDI channels
fix: resolve potentiometer calibration issue
docs: update hardware setup guide
```

### PR Description Template
```markdown
## 🎯 Description
Brief description of changes

## 🔄 Type of Change
- [ ] Bug fix
- [ ] New feature
- [ ] Documentation update
- [ ] Performance improvement
- [ ] Code refactoring

## 🧪 Testing
- [ ] Unit tests added/updated
- [ ] Integration tests pass
- [ ] Manual testing completed

## 📋 Checklist
- [ ] Code follows style guidelines
- [ ] Self-review completed
- [ ] Documentation updated
- [ ] No merge conflicts
```

## 🎛️ Hardware Contributions

### Schematic Changes
- Use **KiCad** for schematics
- Include **Gerber files** for PCB designs
- Provide **BOM** (Bill of Materials)
- Test on actual hardware

### Component Selection
- Prefer widely available components
- Document alternative components
- Consider cost and availability
- Verify electrical specifications

## 📚 Documentation

### Code Documentation
```cpp
/**
 * @brief Processes MIDI input from controllers
 * @param channel MIDI channel (1-16)
 * @param value Control value (0-127)
 * @return true if message was processed successfully
 */
bool processMidiInput(uint8_t channel, uint8_t value);
```

### User Documentation
- **Clear instructions** for setup
- **Step-by-step** procedures
- **Troubleshooting** sections
- **Visual aids** (diagrams, photos)

## 🤝 Community Guidelines

### Be Respectful
- Use welcoming and inclusive language
- Respect different viewpoints and experiences
- Accept constructive criticism gracefully

### Be Helpful
- Help newcomers get started
- Share knowledge and resources
- Provide constructive feedback

### Be Collaborative
- Work together on solutions
- Share credit for contributions
- Support the community goals

## 🆘 Getting Help

### Channels
- **Issues**: For bugs and feature requests
- **Discussions**: For questions and ideas
- **Discord**: Real-time community chat (if available)

### Resources
- **Documentation**: Check existing docs first
- **Examples**: Look at existing code patterns
- **Tests**: See how features are tested

## 🏆 Recognition

Contributors are recognized in:
- **CONTRIBUTORS.md** file
- **Release notes** for significant contributions
- **GitHub contributors** page

---

Thank you for contributing to Mixduino! 🎛️✨
