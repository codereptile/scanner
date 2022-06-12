//
// Created by codereptile on 07.11.2021.
//

#include "utils.h"

void StartRedCout() {
    std::cout << "\033[31m";  // for linux red text
}
void StartGreenCout() {
    std::cout << "\033[32m";  // for linux green text
}
void StartBlueCout() {
    std::cout << "\033[35m";  // for linux green text
}
void EndColorCout() {
    std::cout << "\033[0m";
}
void PrintHelp() {
    std::cout << "Usage: ./scanner [OPTIONS] [DIRECTORY]\n\n";
    std::cout
        << "Searches for 'suspicious' files by rules, specified in suspicious_files_settings.txt\n";
    std::cout << "Search is done using regex. Search is not recursive.\n";

    std::cout << "Available options:\n";

    std::cout << "   -h, --help       - print this help and exit\n";
    std::cout << "   -f               - print short status for every file\n";
    std::cout << "   -v, --verbose    - print per-suspicion-group status for every file\n";
    std::cout << "   -j [NUM THREADS] - set the number of threads to run in, by default it's the "
                 "number of supported threads\n";
    std::cout << "   -s [SETTINGS]    - change suspicious files setting file. By default - "
                 "suspicious_files_settings.txt\n";
}
void PrintHelpBlock() {
    std::cout << "\n----------------------------------------------------------\n";
    PrintHelp();
    std::cout << "----------------------------------------------------------\n\n";
}
void Timer::Start() {
    start_ = std::chrono::steady_clock::now();
}
void Timer::End() {
    end_ = std::chrono::steady_clock::now();
}
double Timer::GetTimeMs() {
    return std::chrono::duration<double, std::milli>(end_ - start_).count();
}
