#include "Bus.h"

#include <src/memory/Cartridge.h>
#include <src/ppu/ppu.h>

uint8_t ram[0x800];

void Bus::LoadCartridge(std::string cart_name)
{
    Cartridge::Load(cart_name);
}

uint8_t Bus::read8(uint16_t addr)
{
    if (addr < 0x800)
        return ram[addr];
    if (addr >= 0x8000)
        return Cartridge::read8(addr);

    switch (addr)
    {
    case 0x2002:
        return PPU::read_ppustatus();
    case 0x4016:
    case 0x4017:
        return 0;
    }
    
    printf("[Bus]: Read from unknown address 0x%04x\n", addr);
    exit(1);
}

uint16_t Bus::read16(uint16_t addr)
{
    return read8(addr) | (read8(addr + 1) << 8);
}

void Bus::write8(uint16_t addr, uint8_t data)
{
    if (addr < 0x800)
    {
        ram[addr] = data;
        return;
    }

    switch (addr)
    {
    case 0x2000:
        PPU::write_ppuctrl(addr, data);
        return;
    case 0x2001:
        PPU::write_ppumask(addr, data);
        return;
    case 0x2003:
        PPU::write_oamaddr(data);
        return;
    case 0x2005:
        PPU::write_scroll(data);
        return;
    case 0x2006:
        PPU::write_ppuaddr(data);
        return;
    case 0x2007:
        PPU::write_vram(data);
        return;
    case 0x4014:
        PPU::write_oamdma(data);
        return;
    case 0x4015:
    case 0x4016:
    case 0x4017:
        return;
    }

    printf("[Bus]: Write to unknown address 0x%04x\n", addr);
    exit(1);
}
