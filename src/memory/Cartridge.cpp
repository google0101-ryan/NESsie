#include "Cartridge.h"

#include <src/memory/mappers/nrom.h>

#include <fstream>

Mapper* mapper;

void Cartridge::Load(std::string name)
{
    std::ifstream file(name, std::ios::ate | std::ios::binary);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    uint8_t* data = new uint8_t[size];

    file.read((char*)data, size);

    struct NesHeader
    {
        char magic[4]; // 0x4E 0x45 0x53 0x1A
        uint8_t prg_rom;
        uint8_t chr_rom;
        uint8_t flags_6;
        uint8_t flags_7;
        uint8_t flags_8;
        uint8_t flags_9;
        uint8_t flags_10;
        uint8_t unused[5];
    };

    NesHeader* header = (NesHeader*)data;

    if (header->magic[0] != 0x4E || header->magic[1] != 0x45 || header->magic[2] != 0x53 || header->magic[3] != 0x1A)
    {
        printf("[Cart]: Invalid NES format!\n");
        exit(1);
    }

    size_t prg_rom = header->prg_rom * 0x4000;
    size_t chr_rom = header->chr_rom * 0x2000;

    printf("[Cart]: Cartridge contains 0x%lx bytes of prg rom, 0x%lx bytes of chr rom\n", prg_rom, chr_rom);

    uint8_t mapper_number = (header->flags_7 & 0xf0) | (header->flags_6 & 0xf);

    printf("[Cart]: Using mapper %d\n", mapper_number);

    switch (mapper_number)
    {
    case 0:
        mapper = new NROM(data + sizeof(NesHeader), prg_rom, chr_rom);
        break;
    default:
        printf("[Cart]: Unknown cartridge type %d\n", mapper_number);
        exit(1);
    }
}

uint8_t Cartridge::read8(uint16_t addr)
{
    return mapper->read8(addr);
}
