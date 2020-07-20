#include "Register.hpp"
#include "exceptions.hpp"
#include "Instruction.hpp"

#ifndef WRITEBACK_HPP
#define WRITEBACK_HPP

class WriteBack
{
    friend class RISCV_5stage;

private:
    Register *reg;

public:
    Instruction inst;

    WriteBack(Register *ri)
    {
        reg = ri;
    }

    void exe()
    {
        if (inst.type == NOP)
            return;
        switch (inst.type)
        {
        case JAL:
        case JALR:
            reg->revise(inst.rd, inst.result);
            reg->pc = inst.resultpc;
            break;
        case LUI:
        case AUIPC:
        case LB:
        case LW:
        case LH:
        case LBU:
        case LHU:
        case ADDI:
        case SLTI:
        case SLTIU:
        case ANDI:
        case ORI:
        case XORI:
        case SLLI:
        case SRLI:
        case SRAI:
        case ADD:
        case SUB:
        case SLL:
        case SLT:
        case SLTU:
        case XOR:
        case SRL:
        case SRA:
        case OR:
        case AND:
            reg->revise(inst.rd, inst.result);
            break;
        default:
            break;
        }
        if (inst.type == JALR)
            reg->J_pc = 0;
    }
};

#endif
