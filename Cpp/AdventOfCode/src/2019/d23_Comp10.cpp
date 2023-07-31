#include "2019/d23_Comp10.h"
#include "2019/Comp.h"
#include <queue>

SOLUTION(2019, 23) {
    struct Packet {
        size_t Destination = 0;
        s64 X = 0;
        s64 Y = 0;
    };

    struct Computer {
        std::vector<s64> Instructions;
        Args Args;
        s64 Id = 0;

        //std::queue<Packet> WorkQueue;
        Constexpr::Queue<Packet> WorkQueue;
    };

    constexpr void RunUntilInteract(Computer & computer) {
        RunUntilInteract(computer.Instructions, computer.Args);
    }

    constexpr bool NeedsInput(const Computer & computer) {
        return NeedsInput(computer.Instructions, computer.Args);
    }

    constexpr std::vector<Computer> InitializeComputers(const std::string & line) {
        auto instructions = ParseInstructions(line);
        Args args;
        std::vector<Computer> result;
        for (s64 i = 0; i < 50; i++) {
            Computer computer = { instructions, args, i };
            computer.Args.Inputs.push_back(i);
            Apply(computer.Instructions, computer.Args);
            result.push_back(computer);
        }
        return result;
    }

    constexpr bool AreQueuesEmpty(const std::vector<Computer>& computers) {
        return std::all_of(computers.begin(), computers.end(), [](const Computer& computer) {
            return computer.WorkQueue.is_empty() && HasConsumedAllInput(computer.Args);
        });
    }

    constexpr void SendPacket(std::vector<Computer>& computers, Packet packet) {
        if (packet.Destination < computers.size()) {
            computers[packet.Destination].WorkQueue.push(packet);
        }
    }

    constexpr auto ConsumeOutput(Computer & computer) {
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

        computer.Args.Output = Unset;
        return packet;
    }

    constexpr void ConsumeInput(Computer& computer) {
        if (computer.WorkQueue.is_empty()) {
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

    Packet NatPacket;
    s64 PreviousY = Unset;

    constexpr s64 InvokeNat(std::vector<Computer>& computers) {
        if (PreviousY == NatPacket.Y) {
            return NatPacket.Y;
        }
        PreviousY = NatPacket.Y;
        computers[0].WorkQueue.push(NatPacket);
        return 0;
    }

    PART_ONE() {
        auto computers = InitializeComputers(lines[0]);
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
                        return Constexpr::ToString(packet.Y);
                    }
                    else {
                        SendPacket(computers, packet);
                    }
                }
            }
        }

        return "Not Found";
    }

    PART_TWO() {
        auto computers = InitializeComputers(lines[0]);
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
                        return Constexpr::ToString(res);
                    }

                    emptyFrames = 0;
                }
            }
        }

        return "Not Found";
    }

    TESTS() {
        return true;
    }
}