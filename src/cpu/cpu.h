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

    int ORA_idx(); // 0x01
    int ORA_zp0(); // 0x05
    int ASL_zp0(); // 0x06
    int ORA_imm(); // 0x09
    int ASL_acc(); // 0x0a
    int BPL_rel(); // 0x10
    int CLC_imp(); // 0x18
    int JSR_abs(); // 0x20
    int AND_zp0(); // 0x25
    int PLP_imp(); // 0x28
    int AND_imm(); // 0x29
    int ROL_acc(); // 0x2a
    int BMI_rel(); // 0x30
    int AND_zpx(); // 0x35
    int SEC_imp(); // 0x38
    int RTI_imp(); // 0x40
    int EOR_zp0(); // 0x45
    int LSR_zp0(); // 0x46
    int PHA_imp(); // 0x48
    int EOR_imm(); // 0x49
    int LSR_imp(); // 0x4a
    int JMP_abs(); // 0x4C
    int RTS_imp(); // 0x60
    int ADC_zp0(); // 0x65
    int ROR_zp0(); // 0x66
    int PLA_imp(); // 0x68
    int ADC_imm(); // 0x69
    int ROR_acc(); // 0x6a
    int ADC_abs(); // 0x6d
    int SEI_imp(); // 0x78
    int STY_ZP0(); // 0x84
    int STA_ZP0(); // 0x85
    int STX_zp0(); // 0x86
    int DEY_imp(); // 0x88
    int TXA_imp(); // 0x8a
    int STY_abs(); // 0x8a
    int STA_abs(); // 0x8d
    int STX_abs(); // 0x8e
    int BCC_rel(); // 0x90
    int STA_idy(); // 0x91
    int STA_zpx(); // 0x95
    int TYA_imp(); // 0x98
    int TXS_imp(); // 0x9a
    int STA_abx(); // 0x9d
    int LDY_imm(); // 0xa0
    int LDX_imm(); // 0xa2
    int LDY_zp0(); // 0xa4
    int LDA_zp0(); // 0xa5
    int LDX_zp0(); // 0xa6
    int TAY_imp(); // 0xa8
    int LDA_imm(); // 0xa9
    int TAX_imp(); // 0xaa
    int LDA_abs(); // 0xad
    int LDX_abs(); // 0xae
    int BCS_rel(); // 0xf0
    int LDA_iny(); // 0xb1
    int LDY_zpx(); // 0xb4
    int LDA_zpx(); // 0xb5
    int DEC_zpx(); // 0xb6
    int LDA_aby(); // 0xb9
    int LDA_abx(); // 0xbd
    int CPY_zp0(); // 0xc4
    int DEC_zp0(); // 0xc6
    int INY_imp(); // 0xc8
    int CMP_imm(); // 0xc9
    int DEX_imp(); // 0xca
    int DEC_abs(); // 0xce
    int BNE_rel(); // 0xd0
    int CLD_imp(); // 0xd8
    int SBC_zp0(); // 0xe5
    int INC_zp0(); // 0xe6
    int INX_imp(); // 0xe8
    int INC_abs(); // 0xee
    int BEQ_rel(); // 0xf0
public:
    CPU();
    void Dump();

    int clock();
    void nmi();
    void SuspendCpu(bool suspended);
};