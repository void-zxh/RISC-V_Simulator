#include "InstructionDecode.hpp"
#include "Register.hpp"
#include "Instruction.hpp"

#ifndef INSTRUCTIONFETCH_HPP
#define INSTRUCTIONFETCH_HPP

class InstructionFetch
{
    friend class RISCV_5stage;

private:
    Register *reg;

public:
    Instruction inst;

    InstructionFetch(Register *ri)
    {
        reg = ri;
    }

    void exe()
    {
        if (waitMEM)
        {
            waitMEM--;
            inst.type = NOP;
            return;
        }
        if (reg->J_pc)
        {
            Fetch_suspend = 1;
            return ;
        }
        Fetch_suspend = 0;
        inst.init();
        inst.num = reg->getFEC();
    }
};

#endif