  cmake_minimum_required(VERSION 3.5.2)
project(code)

set(CMAKE_CXX_STANDARD 14)

add_executable(code main.cpp exceptions.hpp Execution.hpp Instruction.hpp InstructionDecode.hpp InstructionFetch.hpp MemoryAccess.hpp prediction.hpp Register.hpp RISC-V.hpp WriteBack.hpp)