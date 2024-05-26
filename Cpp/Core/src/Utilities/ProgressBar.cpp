#include "Core/Utilities/ProgressBar.h"
#include "Core/Platform/Types.h"

#include <iostream>

constexpr u8 BarWidth = 80;
void ProgressBar::Update(size_t currentProgress) {
    f32 percentComplete = static_cast<f32>(currentProgress) / static_cast<f32>(m_FinalNumber);
    std::cout << "\r[";
    u8 progress = static_cast<u8>(BarWidth * percentComplete);
    for(u8 i = 0; i < progress - 1; i++) {
        std::cout << "=";
    }
    std::cout << ">";
    for(u8 i = progress + 1; i < BarWidth; i++) {
        std::cout << " ";
    }

    std::cout << "] " << static_cast<u16>(percentComplete * 100) << "%";
    std::cout.flush();
}

ProgressBar::~ProgressBar() {
    std::cout << "\r[";
    for(u8 i = 0; i < BarWidth - 1; i++) {
        std::cout << "=";
    }
    std::cout << ">] 100%\n";
}