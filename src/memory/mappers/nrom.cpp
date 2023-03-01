#include "nrom.h"

#include <cstring>

NROM::NROM(uint8_t *data, size_t prg_rom_size, size_t chr_rom_size)
{
    prg_rom = new uint8_t[prg_rom_size];
    chr_rom = new uint8_t[chr_rom_size];

    memcpy(prg_rom, data, prg_rom_size);
    memcpy(chr_rom, data+prg_rom_size, chr_rom_size);
}

uint8_t NROM::read8(uint16_t addr)
{
    if (addr >= 0x8000 && addr < 0xBFFF)
        return prg_rom[addr - 0x8000];
    else if (addr >= 0xC000 && addr < 0xFFFF)
        return prg_rom[addr - 0xC000];
}
