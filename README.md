## Requirements:
- **arm-none-eabi-gcc** toolchain
- **pyocd** (install it from pip)
- **ninja** (optional)

## Usage:
- **Configure:** `cmake . -B build -DCMAKE_BUILD_TYPE=Release -G Ninja`
- **Build & Flash:** `ninja -C build flash`
