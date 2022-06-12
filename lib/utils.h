//
// Created by codereptile on 07.11.2021.
//

#ifndef SCANNER_UTILS_H
#define SCANNER_UTILS_H

#include <iostream>
#include <chrono>

// Console color
void StartRedCout();
void StartGreenCout();
void StartBlueCout();

void EndColorCout();

// Prints info about program usage
void PrintHelp();

// Prints info about program usage in a block for easier distinguishing from other messages
void PrintHelpBlock();

// Timer class is capable of measuring execution time of some code and outputting result in ms
class Timer {
private:
    std::chrono::time_point<std::chrono::steady_clock> start_;
    std::chrono::time_point<std::chrono::steady_clock> end_;

public:
    Timer() = default;

    void Start();
    void End();
    double GetTimeMs();
};

#endif  // SCANNER_UTILS_H
