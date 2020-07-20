#include <iostream>
#include <cstring>

#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

typedef unsigned int uint;

enum instructiontype
{
    NONE,
    NOP,
    LUI,
    AUIPC,
    JAL,
    JALR,
    BEQ,
    BNE,
    BLT,
    BGE,
    BLTU,
    BGEU,
    LB,
    LH,
    LW,
    LBU,
    LHU,
    SB,
    SH,
    SW,
    ADDI,
    SLTI,
    SLTIU,
    XORI,
    ORI,
    ANDI,
    SLLI,
    SRLI,
    SRAI,
    ADD,
    SUB,
    SLL,
    SLT,
    SLTU,
    XOR,
    SRL,
    SRA,
    OR,
    AND
};

uint waitMEM = 0;
uint memory[10000005]; //模拟内存,按字节为单位
uint Fetch_suspend;//抓取中断
uint J_suspend;//跳转中断

uint cHEX(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    else
        return c - 'A' + 10;
}

uint decodeHEX(char *s)
{
    uint re = 0;
    for (int i = 0; s[i] != '\0'; i++)
        re = (re << 4) + cHEX(s[i]);
    return re;
}

uint sext(uint x, int bl)//有符号位拓展,bl为数的二进制位数
{
    bl--;
    if ((x >> bl) & 1)
        x |= ((0xffffffff >> bl) << bl);
    return x;
}

#endif