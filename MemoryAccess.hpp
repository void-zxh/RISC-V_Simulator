#include "WriteBack.hpp"
#include "Register.hpp"

#ifndef MEMORYACCESS_HPP
#define MEMORYACCESS_HPP

class MemoryAccess
{
    friend class RISCV_5stage;

private:
    Register *reg;

public:
    Instruction inst;

    MemoryAccess(Register *ri)
    {
        reg = ri;
    }

    void exe()
    {
        if (inst.type == NOP)
            return;
        switch (inst.type)
        {
        case LW:
            inst.result = reg->load(inst.src1, 4);
            break;
        case LH:
            inst.result = sext(reg->load(inst.src1, 2), 16);
            break;
        case LHU:
            inst.result = reg->load(inst.src1, 2);
            break;
        case LB:
            inst.result = sext(reg->load(inst.src1, 1), 8);
            break;
        case LBU:
            inst.result = reg->load(inst.src1, 1);
            break;
        case SW:
            reg->store(inst.src1, inst.src2, 4);
            break;
        case SH:
            reg->store(inst.src1, inst.src2, 2);
            break;
        case SB:
            reg->store(inst.src1, inst.src2, 1);
            break;
        default:
            break;
        }
    }

};

#endif