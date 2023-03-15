#pragma once

#include "Common.h"
#include "Comp.h"
#include <queue>

constexpr s64 Consumed = -9919;

struct Packet {
    size_t Destination = 0;
    s64 X = 0;
    s64 Y = 0;
};

struct Computer {
    std::vector<s64> Instructions;
    Args Args;
    s64 Id = 0;

    std::queue<Packet> WorkQueue;
};

constexpr void RunUntilInteract(Computer& computer) {
    RunUntilInteract(computer.Instructions, computer.Args);
}

constexpr bool NeedsInput(const Computer& computer) {
    return NeedsInput(computer.Instructions, computer.Args);
}

std::vector<Computer> InitializeComputers(const std::string& line) {
    auto instructions = ParseInstructions(line);
    Args args;
    args.Output = Consumed;
    std::vector<Computer> result;
    for (auto i = 0; i < 50; i++) {
        Computer computer = { instructions, args, i };
        computer.Args.Inputs.push_back(static_cast<s64>(i));
        Apply(computer.Instructions, computer.Args);
        result.push_back(computer);
    }
    return result;
}

constexpr bool AreQueuesEmpty(const std::vector<Computer>& computers) {
    for (const auto& computer : computers) {
        if (!computer.WorkQueue.empty()) {
            return false;
        }
        if (!HasConsumedAllInput(computer.Args)) {
            return false;
        }
    }
    return true;
}

void SendPacket(std::vector<Computer>& computers, Packet packet) {
    if (packet.Destination < computers.size()) {
        computers[packet.Destination].WorkQueue.push(packet);
    }
}

auto ConsumeOutput(Computer& computer) {
    Packet packet;
    
    RunUntilInteract(computer);
    Apply(computer.Instructions, computer.Args);
    packet.Destination = computer.Args.Output;
    
    RunUntilInteract(computer);
    Apply(computer.Instructions, computer.Args);
    packet.X = computer.Args.Output;
    
    RunUntilInteract(computer);
    Apply(computer.Instructions, computer.Args);
    packet.Y = computer.Args.Output;

    computer.Args.Output = Consumed;
    return packet;
}

void ConsumeInput(Computer& computer) {
    if (computer.WorkQueue.empty()) {
        computer.Args.Inputs.push_back(-1);
        Apply(computer.Instructions, computer.Args);
    }
    else {
        auto packet = computer.WorkQueue.front();
        computer.WorkQueue.pop();
        computer.Args.Inputs.push_back(packet.X);
        computer.Args.Inputs.push_back(packet.Y);
        Apply(computer.Instructions, computer.Args);
        RunUntilInteract(computer.Instructions, computer.Args);
        Apply(computer.Instructions, computer.Args);
    }
}

auto PartOne(const std::string& line) {
    auto computers = InitializeComputers(line);
    Packet packet;
    while (true) {
        for (auto& computer : computers) {
            RunUntilInteract(computer);
            if (NeedsInput(computer)) {
                ConsumeInput(computer);
            }
            else {
                packet = ConsumeOutput(computer);
                if (packet.Destination == 255) {
                    return packet.Y;
                }
                else {
                    SendPacket(computers, packet);
                }
            }
        }
    }
}

Packet NatPacket;

s64 InvokeNat(std::vector<Computer>& computers) {
    static s64 PreviousY = -9919;

    if (PreviousY == NatPacket.Y) {
        return NatPacket.Y;
    }
    PreviousY = NatPacket.Y;
    computers[0].WorkQueue.push(NatPacket);
    return 0;
}

auto PartTwo(const std::string& line) {
    auto computers = InitializeComputers(line);
    u32 emptyFrames = 0;
    Packet packet;
    while (true) {
        u32 sendCount = 0;
        for (auto& computer : computers) {
            RunUntilInteract(computer);
            if (NeedsInput(computer)) {
                ConsumeInput(computer);
            }
            else {
                sendCount++;
                packet = ConsumeOutput(computer);
                if (packet.Destination == 255) {
                    NatPacket = packet;
                }
                else {
                    SendPacket(computers, packet);
                }
            }
        }

        if (sendCount == 0 && AreQueuesEmpty(computers)) {
            emptyFrames++;
            if (emptyFrames == 10) {
                auto res = InvokeNat(computers);
                if (res > 0) {
                    return res;
                }

                emptyFrames = 0;
            }
        }
    }
}

//12842 is too high
//12568 is wrong
std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne(lines[0]));
    return ToString(PartTwo(lines[0]));
}

bool RunTests() {
    return true;
}