#include "exceptions.hpp"

#ifndef REGISTER_HPP
#define REGISTER_HPP

class Register
{
    friend class RISCV_5stage;
    friend class InstructionFetch;
    friend class InstructionDecode;
    friend class Execution;
    friend class MemoryAccess;
    friend class WriteBack;
private:
    uint data[55];
    uint pc;

public:
    uint J_pc;

    Register(uint tpc)
    {
        memset(data, 0, sizeof(data));
        pc = tpc;
        J_pc = 0;
    }

    void revise(int pos, uint x)
    {
        if (pos == 0) //x0永远为0
            return;
        data[pos] = x;
    }

    uint operator[](int pos)
    {
        if (pos == 0)
            return 0;
        return data[pos];
    }

    uint load(uint pos, int li)
    {
        uint re = 0;
        for (int i = li - 1; i >= 0; i--)
            re = (re << 8) + memory[pos + i];
        return re;
    }

    void store(uint pos, uint x, int p)
    {
        for (int i = 0; i < p; i++)
        {
            memory[pos + i]=x & 255u;
            x >>= 8;
        }
    }

    uint getFEC()
    {
        uint re = load(pc, 4);
        pc += 4;
        return re;
    }
};

#endif