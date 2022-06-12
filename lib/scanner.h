//
// Created by codereptile on 07.11.2021.
//

#ifndef SCANNER_SCANNER_H
#define SCANNER_SCANNER_H

#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <regex>
#include <map>
#include <thread>
#include <mutex>
#include <chrono>

#include "utils.h"

// Represents one test to run on files and contains: name of test, regexes to search for file types
// and regexes to search for suspicious patterns
class SuspicionTest {
public:
    std::string test_name_;
    std::vector<std::regex> file_types_regexes_;
    std::vector<std::string> file_types_regexes_strings_;
    std::vector<std::regex> suspicion_regexes_;
    std::vector<std::string> suspicion_regexes_strings_;

    SuspicionTest(std::ifstream &suspicious_files_settings_stream);

    void Print() const;
};

// This class contains all the settings that could be set to the scanner
// and is capable of obtaining these from command line arguments and settings file
class Settings {
public:
    std::string path_to_directory_;
    std::string suspicious_files_settings_path_;
    bool per_file_short_info_;
    bool per_file_long_info_;
    std::vector<std::shared_ptr<SuspicionTest>> suspicion_tests_;
    unsigned int num_threads;

    Settings(int argc, char **argv);
};

enum ScanStatus {
    SCAN_OK,
    SCAN_FAIL,
    SCAN_SUSPICIOUS,
    SCAN_NOT_LAUNCHED,
};

// Contains scan results for all tests, for one file
struct ScanResult {
    std::string path_to_file_;
    std::map<const SuspicionTest *, ScanStatus> scan_statuses_;
    ScanStatus overall_status_;

    ScanResult(const std::string &path_to_file);
};

// Scans the file, using 'SuspicionTest's from settings and places the result into 'ScanResult' class
void ScanFile(const Settings &settings, ScanResult &scan_result);

// This class is responsible for scanning a whole directory and storing 'ScanResult's for every file
class Scanner {
public:
    std::vector<ScanResult> scan_results_;

    Scanner(const Settings &settings);

    void LaunchScan(const Settings &settings);
};

#endif  // SCANNER_SCANNER_H
