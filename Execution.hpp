#include "MemoryAccess.hpp"
#include "Register.hpp"
#include "exceptions.hpp"
#include "prediction.hpp"

#ifndef EXCUTION_HPP
#define EXCUTION_HPP

class Execution
{
	friend class RISCV_5stage;

private:
	Register *reg;
	Prediction *pdt;

public:
	Instruction inst;

	Execution(Register *ri, Prediction *tpdt)
	{
		reg = ri;
		pdt = tpdt;
	}

	void exe()
	{
		if (inst.type == NOP)
			return;
		switch (inst.type)
		{
		case LUI:
			inst.result = inst.imm;
			break;
		case AUIPC:
			inst.result = inst.src1 - 4 + inst.imm;
			break;
		case JALR:
			inst.resultpc = inst.src1 + inst.imm;
			inst.resultpc = (inst.resultpc) & (~1);
			break;
		case BEQ:
			inst.result = uint(inst.src1 == inst.src2);
			break;
		case BNE:
			inst.result = uint(inst.src1 != inst.src2);
			break;
		case BLTU:
			inst.result = uint(inst.src1 < inst.src2);
			break;
		case BGEU:
			inst.result = uint(inst.src1 >= inst.src2);
			break;
		case BLT:
			inst.result = uint(inst.src1 < inst.src2);
			break;
		case BGE:
			inst.result = uint(inst.src1 >= inst.src2);
			break;
		case LB:
		case LW:
		case LH:
		case LHU:
		case LBU:
			inst.src1 = inst.src1 + sext(inst.imm, 12);
			break;
		case SB:
		case SW:
		case SH:
			inst.src1 = inst.src1 + sext(inst.imm, 12);
			break;
		case ADDI:
			inst.result = inst.src1 + inst.imm;
			break;
		case SLTI:
			inst.result = uint(inst.src1 < inst.imm);
			break;
		case SLTIU:
			inst.result = uint(inst.src1 < inst.imm);
			break;
		case ANDI:
			inst.result = (inst.src1 & inst.imm);
			break;
		case ORI:
			inst.result = (inst.src1 | inst.imm);
			break;
		case XORI:
			inst.result = (inst.src1 ^ inst.imm);
			break;
		case SLLI:
			inst.result = (inst.src1 << inst.imm);
			break;
		case SRLI:
			inst.result = (inst.src1 >> inst.imm);
			break;
		case SRAI:
			inst.result = ((inst.src1 >> inst.imm) | (inst.src1 >> 31 << 31));
			break;
		case ADD:
			inst.result = inst.src1 + inst.src2;
			break;
		case SUB:
			inst.result = inst.src1 - inst.src2;
			break;
		case SLL:
			inst.result = (inst.src1 << inst.src2);
			break;
		case SRL:
			inst.result = (inst.src1 >> inst.src2);
			break;
		case SRA:
			inst.result = ((inst.src1 >> inst.src2) | (inst.src1 >> 31 << 31));
			break;
		case SLT:
			inst.result = uint(inst.src1 < inst.src2);
			break;
		case SLTU:
			inst.result = uint(inst.src1 < inst.src2);
			break;
		case XOR:
			inst.result = (inst.src1 ^ inst.src2);
			break;
		case OR:
			inst.result = (inst.src1 | inst.src2);
			break;
		case AND:
			inst.result = (inst.src1 & inst.src2);
			break;
		default:
			break;
		}
	}

	bool prediction_result()
	{
		if (inst.type == BEQ || inst.type == BNE || inst.type == BGE || inst.type == BLT || inst.type == BGEU || inst.type == BLTU)
		{
			if (inst.pred != inst.result)
			{
				pdt->update(inst);
				if (inst.pred == 0)
					reg->pc = inst.resultpc - 4 + inst.imm;
				else
					reg->pc = inst.resultpc;
				return false;
			}
			else
				pdt->update(inst);
			return true;
		}
		return true;
	}
};

#endif