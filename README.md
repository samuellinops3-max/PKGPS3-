# SlitherPS3

Clone offline estilo Slither.io para PlayStation 3 (Homebrew)

## Requisitos
- PS3 com **CFW** ou **HEN**
- Toolchain PSL1GHT instalado

## Como compilar

```bash
export PS3DEV=/usr/local/ps3dev
export PSL1GHT=$PS3DEV
export PATH=$PS3DEV/bin:$PS3DEV/ppu/bin:$PS3DEV/spu/bin:$PATH

make
make pkg