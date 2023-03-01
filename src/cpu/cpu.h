#pragma once

#include <cstdint>
#include <functional>
#include <unordered_map>

class CPU
{
private:
    uint8_t x, y, a, s, p;
    uint16_t pc;

    enum Flags
    {
        C_FLAG = (1 << 0),
        Z_FLAG = (1 << 1),
        I_FLAG = (1 << 2),
        D_FLAG = (1 << 3),
        B_FLAG = (1 << 4),
        V_FLAG = (1 << 6),
        N_FLAG = (1 << 7)
    };

    void SetFlag(Flags flag, bool set)
    {
        if (set)
            p |= flag;
        else
            p &= ~flag;
    }

    bool GetFlag(Flags flag)
    {
        return p & flag;
    }

    using Instr = std::function<int()>;
    std::unordered_map<uint8_t, Instr> opcodes;

    int BPL_rel(); // 0x10
    int CLC_imp(); // 0x18
    int JSR_abs(); // 0x20
    int AND_zp0(); // 0x25
    int AND_imm(); // 0x29
    int EOR_zp0(); // 0x45
    int PHA_imp(); // 0x48
    int EOR_imm(); // 0x49
    int LSR_imp(); // 0x4a
    int JMP_abs(); // 0x4C
    int RTS_imp(); // 0x60
    int ADC_zp0(); // 0x65
    int ROR_zp0(); // 0x66
    int PLA_imp(); // 0x68
    int ADC_imm(); // 0x69
    int SEI_imp(); // 0x78
    int STY_ZP0(); // 0x84
    int STA_ZP0(); // 0x85
    int DEY_imp(); // 0x88
    int TXA_imp(); // 0x8a
    int STA_abs(); // 0x8d
    int STA_idy(); // 0x91
    int TYA_imp(); // 0x98
    int TXS_imp(); // 0x9a
    int LDY_imm(); // 0xa0
    int LDX_imm(); // 0xa2
    int LDA_zp0(); // 0xa5
    int LDX_zp0(); // 0xa6
    int TAY_imp(); // 0xa8
    int LDA_imm(); // 0xa9
    int TAX_imp(); // 0xaa
    int LDA_abs(); // 0xad
    int LDA_iny(); // 0xb1
    int DEC_zp0(); // 0xc6
    int INY_imp(); // 0xc8
    int CMP_imm(); // 0xc9
    int DEX_imp(); // 0xca
    int BNE_rel(); // 0xd0
    int CLD_imp(); // 0xd8
    int BEQ_rel(); // 0xf0
public:
    CPU();
    void Dump();

    int clock();
};