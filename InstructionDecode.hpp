#include "Execution.hpp"
#include "Register.hpp"
#include "prediction.hpp"

#ifndef INSTRUCTIONDECODE_HPP
#define INSTRUCTIONDECODE_HPP

class InstructionDecode
{
    friend class RISCV_5stage;

private:
    Register *reg;
    Prediction *pdt;

public:
    Instruction inst;

    InstructionDecode(Register *ri, Prediction *tpdt)
    {
        reg = ri;
        pdt = tpdt;
    }

    void exe()
    {
        J_suspend = 0;
        if (inst.type == NOP) //空指令（由JAL或MEM三周期产生）
            return;
        inst.getDEC();
        switch (inst.type)
        {
        case LW:
        case LH:
        case LHU:
        case LB:
        case LBU:
        case SW:
        case SH:
        case SB:
            waitMEM = 0;
            break;
        default:
            break;
        }
        if (!getSRC())
        {
            J_suspend = 1;
            return ;
        }
        prediction();
        if (inst.type == JALR)
            reg->J_pc = 1;
    }

    void reFetch()
    {
        inst.init();
        inst.num = reg->getFEC();
    }

    void prediction()
    {
        switch (inst.type)
        {
        case BEQ:
        case BNE:
        case BGE:
        case BLT:
        case BGEU:
        case BLTU:
            inst.pred = pdt->getPrediction(inst);
            if (inst.pred)
                reg->pc = inst.resultpc - 4 + inst.imm;
        default:
            break;
        }
    }

    bool getSRC()
    {
        switch (inst.type)
        {
        case AUIPC:
            if (reg->J_pc)
                return false;
            inst.src1 = reg->pc;
            break;
        case JAL:
            if (reg->J_pc)
                return false;
            inst.src1 = reg->pc;
            break;
        case JALR:
            if (reg->J_pc)
                return false;
            inst.src1 = (*reg)[inst.rs1];
            inst.result = reg->pc;
            break;
        case BEQ:
        case BNE:
        case BLT:
        case BLTU:
        case BGE:
        case BGEU:
            if (reg->J_pc)
                return false;
            inst.src1 = (*reg)[inst.rs1];
            inst.src2 = (*reg)[inst.rs2];
            inst.resultpc = reg->pc;
            break;
        case LB:
        case LH:
        case LW:
        case LBU:
        case LHU:
            inst.src1 = (*reg)[inst.rs1];
            break;
        case SW:
        case SH:
        case SB:
            inst.src1 = (*reg)[inst.rs1];
            inst.src2 = (*reg)[inst.rs2];
            break;
        case ADDI:
        case SLTI:
        case SLTIU:
        case ANDI:
        case ORI:
        case XORI:
            inst.src1 = (*reg)[inst.rs1];
            break;
        case SLLI:
        case SRLI:
        case SRAI:
            inst.src1 = (*reg)[inst.rs1];
            break;
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
            inst.src1 = (*reg)[inst.rs1];
            inst.src2 = (*reg)[inst.rs2];
            break;
        default:
            break;
        }
        return true;
    }
};

#endif