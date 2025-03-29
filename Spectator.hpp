#pragma once
#include <iostream>
#include <vector>
#include <array>
#include "Player.hpp"
#include "LocalPlayer.hpp"


struct Spectator {
    LocalPlayer* Myself;
    std::vector<Player*>* Players;
    int TotalSpectators = 0;
    std::vector<std::string> SpectatorsNames;
    std::chrono::milliseconds LastUpdateTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

    Spectator(std::vector<Player*>* Players, LocalPlayer* Myself) {
        this->Players = Players;
        this->Myself = Myself;
    }

    void Update() {
        std::chrono::milliseconds Now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
        if (Now >= LastUpdateTime + std::chrono::milliseconds(1500))
        {
            int TempTotalSpectators = 0;
            std::vector<std::string> TempSpectatorsNames;

            for (int i = 0; i < Players->size(); i++)
            {
                Player* p = Players->at(i);
                if (!mem.IsValidPointer(p->BasePointer))
                    continue;

                if (p->BasePointer == Myself->BasePointer)
                    continue;

                if (!p->IsPlayer())
                    continue;

                uint64_t SpectatorList = mem.Read<uint64_t>(mem.OFF_BASE + OFF_OBSERVER_LIST);
                int PlayerData = mem.Read<int>(p->BasePointer + 0x38);
                int SpecIndex = mem.Read<int>(SpectatorList + PlayerData * 8 + OFF_OBSERVER_LIST_IN_ARRAY);
                uint64_t SpectatorAddr = mem.Read<uint64_t>(mem.OFF_BASE + OFF_ENTITY_LIST + ((SpecIndex & 0xFFFF) << 5));

                if (SpectatorAddr == Myself->BasePointer)
                {
                    std::cout << "SPECTATOR!" << std::endl;
                    TempTotalSpectators++;
                    TempSpectatorsNames.push_back(p->Name);
                }
            }

            TotalSpectators = TempTotalSpectators;
            SpectatorsNames = TempSpectatorsNames;
            LastUpdateTime = Now;
        }
    }
};