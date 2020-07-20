#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "RISC-V.hpp"
using namespace std;

RISCV_5stage riscv;

int main()
{
    /*char filename[10005]="testcases/tak.data";
    freopen(filename,"r",stdin);
    freopen("prediction outcome.txt","a+",stdout);*/
    riscv.input();
    riscv.run();
    //cout<<filename<<endl;
    cout << riscv.output() << endl;
    return 0;
}