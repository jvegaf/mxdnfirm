import os
try:
    from SCons.Script import Import
    # Import the PlatformIO environment
    Import("env")
except ImportError:
    # Cuando se ejecuta flake8, definimos una variable env falsa para que no falle el linting
    class DummyEnv:
        def Append(self, **kwargs):
            pass
        def Object(self, source):
            return source
    env = DummyEnv()

# Define the source directories we want to include
src_dirs = [
    "src",
    "test/mocks"
]

# Get all .cpp files from the source directories
sources = []
for src_dir in src_dirs:
    if os.path.exists(src_dir):
        for root, dirs, files in os.walk(src_dir):
            for file in files:
                if file.endswith('.cpp') and file != 'main.cpp':
                    sources.append(os.path.join(root, file))

# Add the sources to the PIOBUILD_SOURCES directly
for source in sources:
    if os.path.exists(source):
        env.Append(PIOBUILD_SOURCES=env.Object(source))

print(f"Added {len(sources)} source files to native build:")
for source in sources:
    print(f"  - {source}")
