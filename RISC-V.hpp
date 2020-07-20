#include <iostream>
#include <string.h>
#include "Register.hpp"
#include "InstructionFetch.hpp"
#include "InstructionDecode.hpp"
#include "Execution.hpp"
#include "MemoryAccess.hpp"
#include "WriteBack.hpp"
#include "prediction.hpp"
using namespace std;

#ifndef RISCV_HPP
#define RISCV_HPP

class RISCV_5stage
{
private:
    Register reg;
    InstructionFetch IF;
    InstructionDecode ID;
    Execution EX;
    MemoryAccess MEM;
    WriteBack WB;
    Prediction pdt;

    void Forward(Execution &EX, InstructionDecode &ID)
    {
        if (EX.inst.type == NOP)
            return;
        if (EX.inst.num & 127 != 99 && EX.inst.num & 127 != 35 && EX.inst.num & 127 != 3) //不为B类指令、L类指令与S类指令
        {
            if (ID.inst.rs1 == EX.inst.rd && ID.inst.rs1 != 0)
                ID.inst.src1 = EX.inst.result;
            if (ID.inst.rs2 == EX.inst.rd && ID.inst.rs2 != 0)
                ID.inst.src2 = EX.inst.result;
        }
    }

    void Forward(MemoryAccess &MA, InstructionDecode &ID)
    {
        if (MA.inst.type == NOP)
            return;
        if (MA.inst.num & 127 != 99 && MA.inst.num & 127 != 35) //不为B类指令与S类指令
        {
            if (ID.inst.rs1 == MA.inst.rd && ID.inst.rs1 != 0)
                ID.inst.src1 = MA.inst.result;
            if (ID.inst.rs2 == MA.inst.rd && ID.inst.rs2 != 0)
                ID.inst.src2 = MA.inst.result;
        }
    }

    void Forward(MemoryAccess &MA, Execution &EX)
    {
        if (MA.inst.type == NOP)
            return;
        if (MA.inst.num & 127 != 99 && MA.inst.num & 127 != 35) //不为B类指令与S类指令
        {
            if (EX.inst.rs1 == MA.inst.rd && EX.inst.rs1 != 0)
                EX.inst.src1 = MA.inst.result;
            if (EX.inst.rs2 == MA.inst.rd && EX.inst.rs2 != 0)
                EX.inst.src2 = MA.inst.result;
        }
    }

    void Prediction_verify()
    {
        if (EX.inst.type == NOP)
            return;
        if (EX.prediction_result() == 0)
            ID.reFetch();
    }

    void Forward_towards_ID()
    {
        if (!J_suspend)
        {
            Forward(MEM, ID);
            Forward(EX, ID);
            if (ID.inst.type == JAL) //JAR直接跳转的优化
            {
                reg.revise(ID.inst.rd, ID.inst.src1);
                reg.pc = ID.inst.src1 - 4 + ID.inst.imm;
                ID.inst.type = NOP;
            }
        }
    }

    void passafter()
    {
        WB.inst = MEM.inst;
        MEM.inst = EX.inst;
        EX.inst = ID.inst;
        if (J_suspend)
            EX.inst.type = NOP;
        if (!J_suspend)
        {
            ID.inst = IF.inst;
            if (Fetch_suspend)
                ID.inst.type = NOP;
        }
    }

public:
    RISCV_5stage() : reg(0), pdt(), IF(&reg), ID(&reg, &pdt), EX(&reg, &pdt), MEM(&reg), WB(&reg) {}

    void run()
    {
        while (1)
        {
            WB.exe();
            MEM.exe();
            Forward(MEM, EX);
            EX.exe();
            Prediction_verify();
            ID.exe();
            Forward_towards_ID();
            if (!J_suspend)
                IF.exe();
            if (ID.inst.num == 0xff00513) //终止条件
                break;
            passafter();
        }
    }

    void input()
    {
        char s[105];
        memset(s, 0, sizeof(s));
        int addr = 0;
        while (~scanf("%s", s))
        {
            if (s[0] == '@')
                addr = decodeHEX(s + 1);
            else
            {
                memory[addr] = uint(decodeHEX(s));
                addr++;
            }
        }
    }

    uint output()
    {
        //pdt.out();
        return (reg[10] & 255u);
    }
};

#endif