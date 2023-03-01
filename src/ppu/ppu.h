#pragma once

#include <cstdint>

namespace PPU
{
    void dump();

    void clock(int cycles);

    void write_ppuctrl(uint16_t addr, uint8_t data);
    void write_ppumask(uint16_t addr, uint8_t data);
    void write_scroll(uint8_t data);
    void write_ppuaddr(uint8_t data);
    void write_vram(uint8_t data);

    uint8_t read_ppustatus();
};