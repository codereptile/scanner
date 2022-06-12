//
// Created by codereptile on 07.11.2021.
//

#include "scanner.h"

SuspicionTest::SuspicionTest(std::ifstream& suspicious_files_settings_stream) {
    std::getline(suspicious_files_settings_stream, test_name_);
    if (test_name_.empty()) {
        StartRedCout();
        std::cout << "!!!!! Fatal error\n";
        std::cout << "Failed to read suspicion test\n\n";
        std::cout << "Abort\n";
        EndColorCout();
        exit(1);
    }
    std::string line;
    while (std::getline(suspicious_files_settings_stream, line)) {
        if (line == "E") {
            break;
        }
        try {
            file_types_regexes_.push_back(std::regex(line, std::regex::icase));
        } catch (std::exception& e) {
            StartRedCout();
            std::cout << "!!!!! Fatal error\n";
            std::cout << "\tCould not create regex from: " << line << "\n";
            std::cout << "\tError message:\n\t" << e.what() << "\n";
            std::cout << "Abort\n";
            EndColorCout();
            exit(1);
        }

        file_types_regexes_strings_.push_back(line);
    }
    while (std::getline(suspicious_files_settings_stream, line)) {
        if (line == "E") {
            break;
        }
        try {
            suspicion_regexes_.push_back(std::regex(line, std::regex::icase));
        } catch (std::exception& e) {
            StartRedCout();
            std::cout << "!!!!! Fatal error\n";
            std::cout << "\tCould not create regex from: " << line << "\n";
            std::cout << "\tError message:\n\t" << e.what() << "\n";
            std::cout << "Abort\n";
            EndColorCout();
            exit(1);
        }
        suspicion_regexes_strings_.push_back(line);
    }

    if (file_types_regexes_.empty() || suspicion_regexes_.empty()) {
        StartRedCout();
        std::cout << "!!!!! Fatal error\n";
        std::cout << "Failed to read suspicion test\n\n";
        std::cout << "Abort\n";
        EndColorCout();
        exit(1);
    }
}

void SuspicionTest::Print() const {
    std::cout << "Suspicion test: \"" << test_name_ << "\"\n";
    std::cout << "Files regexes:\n";
    for (const std::string& file_type_regex_string : file_types_regexes_strings_) {
        std::cout << "\t" << file_type_regex_string << "\n";
    }
    std::cout << "Suspicion regexes:\n";
    for (const std::string& suspicion_regex_string : suspicion_regexes_strings_) {
        std::cout << "\t" << suspicion_regex_string << "\n";
    }
    std::cout << "\n";
}

Settings::Settings(int argc, char** argv) {
    // default settings
    num_threads = std::thread::hardware_concurrency();
    bool print_help = false;
    per_file_short_info_ = false;
    per_file_long_info_ = false;
    suspicious_files_settings_path_ = "suspicious_files_settings.txt";
    recursive_ = false;
    // --------------------------

    // Read command-line arguments
    for (size_t i = 1; i < argc; ++i) {
        std::string argument = argv[i];
        if (argument[0] == '-') {  // an argument
            if (argument == "-h" || argument == "--help") {
                print_help = true;
            } else if (argument == "-f") {
                per_file_short_info_ = true;
            } else if (argument == "-v" || argument == "--verbose") {
                per_file_short_info_ = true;
                per_file_long_info_ = true;
            } else if (argument == "-j") {
                ++i;
                if (i >= argc) {
                    StartRedCout();
                    std::cout << "!!!!! Fatal error\n";
                    std::cout << "Failed to set -j\n";
                    EndColorCout();
                    PrintHelpBlock();
                    StartRedCout();
                    std::cout << "Abort\n";
                    EndColorCout();
                    exit(1);
                } else {
                    num_threads = std::atoi(argv[i]);
                    if (num_threads == 0 ||
                        num_threads > 128) {  // I don't think smb needs more than 128 threads
                        StartRedCout();
                        std::cout << "!!!!! Fatal error\n";
                        std::cout << "Invalid number of threads: " << num_threads << "!\n";
                        EndColorCout();
                        PrintHelpBlock();
                        StartRedCout();
                        std::cout << "Abort\n";
                        EndColorCout();
                        exit(1);
                    }
                }
            } else if (argument == "-s") {
                ++i;
                if (i >= argc) {
                    StartRedCout();
                    std::cout << "!!!!! Fatal error\n";
                    std::cout << "No settings given for -s\n";
                    EndColorCout();
                    PrintHelpBlock();
                    StartRedCout();
                    std::cout << "Abort\n";
                    EndColorCout();
                    exit(1);
                } else {
                    suspicious_files_settings_path_ = argv[i];
                }
            } else if (argument == "-r") {
                recursive_ = true;
            } else {
                StartRedCout();
                std::cout << "!!!!! Fatal error\n";
                std::cout << "Invalid arguments\n";
                EndColorCout();
                PrintHelpBlock();
                StartRedCout();
                std::cout << "Abort\n";
                EndColorCout();
                exit(1);
            }
        } else {  // directory name
            path_to_scanned_directories_.push_back(argument);
        }
    }
    // --------------------------
    // Check all arguments and execute immediate responses, if possible
    if (print_help) {
        PrintHelp();
        std::cout << "\n";
        exit(0);
    }
    if (path_to_scanned_directories_.empty()) {
        StartRedCout();
        std::cout << "Error: no directories to scan given\n";
        EndColorCout();
        PrintHelpBlock();
        StartRedCout();
        std::cout << "Abort\n";
        EndColorCout();
        exit(1);
    }
    // --------------------------

    // Read suspicious files settings
    std::ifstream suspicious_files_settings_stream(suspicious_files_settings_path_);
    if (suspicious_files_settings_stream.is_open()) {
        std::string line;
        while (std::getline(suspicious_files_settings_stream, line)) {
            if (line[0] != '#') {
                suspicion_tests_.emplace_back(new SuspicionTest(suspicious_files_settings_stream));
                // suspicion_tests_.back().Print();
            }
        }
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // make this printable
        // exit(1);
    } else {
        StartRedCout();
        std::cout << "!!!!! Fatal error\n";
        std::cout << "Could not open settings file: \"" << suspicious_files_settings_path_
                  << "\"\n";
        EndColorCout();
        PrintHelpBlock();
        StartRedCout();
        std::cout << "Abort\n";
        EndColorCout();
        exit(1);
    }
    // --------------------------
}

ScanResult::ScanResult(const std::string& path_to_file)
    : path_to_file_(path_to_file), overall_status_(SCAN_NOT_LAUNCHED) {
}

void ScanFile(const Settings& settings, ScanResult& scan_result) {
    for (std::shared_ptr<SuspicionTest> test : settings.suspicion_tests_) {
        bool launch_test = false;
        for (const std::regex& file_type_regex : test->file_types_regexes_) {
            if (std::regex_match(scan_result.path_to_file_, file_type_regex)) {
                launch_test = true;
                break;
            }
        }

        if (launch_test) {
            bool failed_test = false;
            bool suspicious_test = false;
            for (const std::regex& suspicion_regex : test->suspicion_regexes_) {
                std::ifstream file_input_stream(scan_result.path_to_file_);
                if (file_input_stream.is_open()) {
                    std::string line;
                    while (getline(file_input_stream, line)) {
                        if (std::regex_search(line, suspicion_regex)) {
                            suspicious_test = true;
                            break;
                            // we break, since there's no point to fun other regexes, the file
                            // is already suspicious
                        }
                    }
                } else {
                    failed_test = true;
                }
            }
            if (suspicious_test) {
                scan_result.scan_statuses_[test.get()] = SCAN_SUSPICIOUS;
                scan_result.overall_status_ = SCAN_SUSPICIOUS;
            } else if (failed_test) {
                scan_result.scan_statuses_[test.get()] = SCAN_FAIL;
                if (scan_result.overall_status_ != SCAN_SUSPICIOUS) {
                    scan_result.overall_status_ = SCAN_FAIL;
                }
            } else {
                scan_result.scan_statuses_[test.get()] = SCAN_OK;
            }
        }
        // std::cout << "Thread for " << scan_result.path_to_file_ << " ";
    }

    if (scan_result.overall_status_ == SCAN_NOT_LAUNCHED) {
        scan_result.overall_status_ = SCAN_OK;
    }
}

Scanner::Scanner(const Settings& settings) {
    std::cout << "Staring folder mapping\n";
    for (const std::string& directory_path : settings.path_to_scanned_directories_) {
        try {
            if (settings.recursive_) {
                std::filesystem::recursive_directory_iterator directory(directory_path);
                try {
                    for (const auto& file_entry : directory) {
                        if (file_entry.is_directory()) {
                            continue;
                        }
                        scan_results_.emplace_back(file_entry.path());
                        for (std::shared_ptr<SuspicionTest> test : settings.suspicion_tests_) {
                            scan_results_.back().scan_statuses_[test.get()] = SCAN_NOT_LAUNCHED;
                        }
                    }
                } catch (std::exception& e) {
                    StartYellowCout();
                    std::cout << "!!!!! Warning\n";
                    std::cout << "\tScanner could not open the directory: " << directory->path()
                              << "\n";
                    std::cout << "\tError message:\n\t" << e.what() << "\n";
                    std::cout << "Abort\n";
                    EndColorCout();
                }
            } else {
                std::filesystem::directory_iterator directory(directory_path);
                try {
                    for (const auto& file_entry : directory) {
                        if (file_entry.is_directory()) {
                            continue;
                        }
                        scan_results_.emplace_back(file_entry.path());
                        for (std::shared_ptr<SuspicionTest> test : settings.suspicion_tests_) {
                            scan_results_.back().scan_statuses_[test.get()] = SCAN_NOT_LAUNCHED;
                        }
                    }
                } catch (std::exception& e) {
                    StartYellowCout();
                    std::cout << "!!!!! Warning\n";
                    std::cout << "\tScanner could not open the directory: " << directory->path()
                              << "\n";
                    std::cout << "\tError message:\n\t" << e.what() << "\n";
                    std::cout << "Abort\n";
                    EndColorCout();
                }
            }
        } catch (std::exception& e) {
            StartYellowCout();
            std::cout << "!!!!! Warning\n";
            std::cout << "\tScanner could not open the directory: " << directory_path << "\n";
            std::cout << "\tError message:\n\t" << e.what() << "\n";
            std::cout << "Abort\n";
            EndColorCout();
        }
    }
    std::cout << "Finished folder mapping\n\n";
}

void Scanner::LaunchScan(const Settings& settings) {
    std::vector<std::thread> threads;
    for (int i = 0; i < scan_results_.size(); ++i) {
        threads.emplace_back(ScanFile, settings, std::ref(scan_results_[i]));

        if ((i + 1) % settings.num_threads == 0) {
            for (std::thread& thread : threads) {
                thread.join();
            }
            threads.clear();
        }
    }
    for (std::thread& thread : threads) {
        thread.join();
    }
    threads.clear();
}