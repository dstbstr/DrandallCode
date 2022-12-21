#pragma once

#include "Common.h"

#include <iostream>

struct Instruction {
    virtual ~Instruction() = default;
    Instruction(s32 offset, u32 regIndex) : Offset(offset), RegIndex(regIndex){}

    virtual void Apply(u32& instNumber, u64& reg) const = 0;

    s32 Offset;
    u32 RegIndex;
};

struct Half : Instruction {
    Half(s32 offset, u32 regIndex) : Instruction(offset, regIndex){}
    void Apply(u32& instNumber, u64& reg) const override {
        reg /= 2;
        instNumber++;
    }
};
struct Triple : Instruction {
    Triple(s32 offset, u32 regIndex) : Instruction(offset, regIndex){}
    void Apply(u32& instNumber, u64& reg) const override {
        reg *= 3;
        instNumber++;
    }
};
struct Inc : Instruction {
    Inc(s32 offset, u32 regIndex) : Instruction(offset, regIndex){}
    void Apply(u32& instNumber, u64& reg) const override {
        reg++;
        instNumber++;
    }
};
struct Jump : Instruction {
    Jump(s32 offset, u32 regIndex) : Instruction(offset, regIndex){}
    void Apply(u32& instNumber, u64&) const override {
        instNumber += Offset;
    }
};
struct JumpEven : Instruction {
    JumpEven(s32 offset, u32 regIndex) : Instruction(offset, regIndex){}
    void Apply(u32& instNumber, u64& reg) const override {
        if(reg % 2 == 0) {
            instNumber += Offset;
        } else {
            instNumber++;
        }
    }
};
struct JumpIfOne : Instruction {
    JumpIfOne(s32 offset, u32 regIndex) : Instruction(offset, regIndex){}
    void Apply(u32& instNumber, u64& reg) const override {
        if(reg == 1) {
            instNumber += Offset;
        } else {
            instNumber++;
        }
    }
};

//hlf r -> r >> 1
//tpl r -> r *= 3
//inc r -> r++
//jmp [+/-]offset -> Run instruction at offset
//jie r, [+/-]offset -> jmp if r is even
//jio r, [+/-]offset -> jmp if r is ONE

struct Program {
    std::vector<std::unique_ptr<Instruction>> Instructions{};
    std::vector<u64> Registers{1, 0};

    void ParseInstruction(const std::string& line) {
        auto name = line.substr(0, 3);
        s32 offset;
        u32 rIndex;
        if(name == "jmp") {
            rIndex = 0;
            auto sign = line[4];
            ParseNumber(line.substr(5), offset);
            if(sign == '-') {
                offset *= -1;
            }
        } else if(name == "jie" || name == "jio" ) {
            rIndex = line[4] == 'a' ? 0 : 1;
            auto sign = line[7];
            ParseNumber(line.substr(8), offset);
            if(sign == '-') {
                offset *= -1;
            }
        } else {
            rIndex = line[4] == 'a' ? 0 : 1;
            offset = 0;
        }

        if(name == "hlf") {
            Instructions.push_back(std::make_unique<Half>(offset, rIndex));
        } else if(name == "tpl") {
            Instructions.push_back(std::make_unique<Triple>(offset, rIndex));
        } else if(name == "inc") {
            Instructions.push_back(std::make_unique<Inc>(offset, rIndex));
        } else if(name == "jmp") {
            Instructions.push_back(std::make_unique<Jump>(offset, rIndex));
        } else if(name == "jie") {
            Instructions.push_back(std::make_unique<JumpEven>(offset, rIndex));
        } else if(name == "jio") {
            Instructions.push_back(std::make_unique<JumpIfOne>(offset, rIndex));
        }
    }

    void Run() {
        u32 current = 0;
        while(current < Instructions.size()) {
            const auto& inst = Instructions[current];
            inst->Apply(current, Registers[inst->RegIndex]);
        }
    }
};

#define ASSERT(cond) if(!(cond)) return false;
bool RunTests() {
    Program program;
    program.ParseInstruction("jio a, +18");
    program.ParseInstruction("jio b, +18");
    program.ParseInstruction("inc a");
    program.ParseInstruction("tpl b");
    program.ParseInstruction("jmp +22");
    program.ParseInstruction("jie a, +4");
    program.ParseInstruction("jmp -7");

    ASSERT(program.Instructions[0]->RegIndex == 0);
    ASSERT(program.Instructions[1]->RegIndex == 1);
    ASSERT(program.Instructions[0]->Offset == 18);
    ASSERT(program.Instructions[2]->RegIndex == 0);
    ASSERT(program.Instructions[3]->RegIndex == 1);
    ASSERT(program.Instructions[4]->Offset == 22);
    ASSERT(program.Instructions[5]->RegIndex == 0);
    ASSERT(program.Instructions[5]->Offset == 4);
    ASSERT(program.Instructions[6]->Offset == -7);

    return true;
}