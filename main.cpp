#include "lib/scanner.h"
#include "lib/utils.h"

// TO-DO: make function more scalable:
// 1 - make cross-file checks available
// 2 - make catch for incorrect regrex
// 3 - make per-file actions
// --------------------------
// Additional:
// 6 - multi-directory support
// 7 - make per-file, not per-line option
// 8 - switch to library-based command line arguments parsing
// 9 - switch to .ini config files

int main(int argc, char **argv) {
    // First, we create the settings for out scanner.
    // All setting data will be set in the constructor, we only need to pass the command line arguments
    Settings settings(argc, argv);

    // Info-print
    std::cout << "\nLaunching scanner on directory: " << settings.path_to_directory_ << " in " << settings.num_threads << " thread(s)\n\n";

    // Now, we need to create a scanner, to process the directory and a timer, to measure execution time
    Scanner scanner(settings);
    Timer timer;

    // Time to launch our scanner under timer
    timer.Start();
    scanner.LaunchScan(settings);
    timer.End();

    // A few variables for short statistics
    int total_files = 0;
    int failed = 0;
    int suspicious = 0;

    // Go through all the files and update the statistics variables
    // Also, if the user requested so, we will print short/long info in this block
    for (const ScanResult &scan_result : scanner.scan_results_) {
        ++total_files;
        if (scan_result.overall_status_ == SCAN_OK) {
            // do nothing
        } else if (scan_result.overall_status_ == SCAN_FAIL) {
            ++failed;
        } else if (scan_result.overall_status_ == SCAN_SUSPICIOUS) {
            ++suspicious;
        }
        if (settings.per_file_short_info_) {
            std::cout << "File: " << scan_result.path_to_file_ << "\n";
            if (scan_result.overall_status_ == SCAN_OK) {
                StartGreenCout();
                std::cout << "All tests good\n";
                EndColorCout();
            } else {
                if (settings.per_file_long_info_) {
                    for (std::pair<const SuspicionTest *, ScanStatus> scan_status :
                         scan_result.scan_statuses_) {
                        std::cout << scan_status.first->test_name_ << "\t";
                        if (scan_status.second == SCAN_OK) {
                            StartGreenCout();
                            std::cout << "Test good\n";
                            EndColorCout();
                        } else if (scan_status.second == SCAN_FAIL) {
                            StartBlueCout();
                            std::cout << "Test failed\n";
                            EndColorCout();
                        } else if (scan_status.second == SCAN_SUSPICIOUS) {
                            StartRedCout();
                            std::cout << "Test reported suspicious\n";
                            EndColorCout();
                        } else if (scan_status.second == SCAN_NOT_LAUNCHED) {
                            StartGreenCout();
                            std::cout << "Scan not launched\n";
                            EndColorCout();
                        }
                    }
                } else {
                    if (scan_result.overall_status_ == SCAN_FAIL) {
                        StartBlueCout();
                        std::cout << "Some tests failed\n";
                        EndColorCout();
                    } else if (scan_result.overall_status_ == SCAN_SUSPICIOUS) {
                        StartRedCout();
                        std::cout << "Some tests reported suspicious";
                        EndColorCout();
                    }
                }
            }
            std::cout << "\n\n";
        }
    }

    // All we have to do now, is to print the short statistics block:
    std::cout << "\n\n------------------------------------\n";
    std::cout << "\t\tReport:\n";
    std::cout << "Total files scanned:\t" << total_files << "\n";
    std::cout << "Failed to scan:\t" << failed << "\n";
    std::cout << "Suspicious files:\t" << suspicious << "\n";
    std::cout << "Execution time:\t" << timer.GetTimeMs() << " ms\n";
    std::cout << "------------------------------------\n\n";

    return 0;
}