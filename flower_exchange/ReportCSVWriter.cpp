#include "ReportCSVWriter.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

// Function to get the current date and time with milliseconds
std::string ExecutionReportCSVWriter::getDateTime()
{
    // Get the current time in milliseconds since epoch
    auto currentTime = std::chrono::system_clock::now();
    auto currentTimeMs = std::chrono::time_point_cast<std::chrono::milliseconds>(currentTime);
    auto epochDuration = currentTimeMs.time_since_epoch();

    std::time_t currentTimeSeconds = std::chrono::duration_cast<std::chrono::seconds>(epochDuration).count();

    std::tm timeInfo;
#ifdef _WIN32
    localtime_s(&timeInfo, &currentTimeSeconds); // Use localtime_s on Windows
#else
    localtime_r(&currentTimeSeconds, &timeInfo); // Use localtime_r on non-Windows systems
#endif

    // Format the date and time
    char formattedTime[80];
    std::strftime(formattedTime, sizeof(formattedTime), "%Y%m%d%H%M%S", &timeInfo);

    // Add milliseconds to the formatted time
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(epochDuration) % 1000;
    sprintf_s(formattedTime, "%s.%03lld", formattedTime, milliseconds.count());

    return formattedTime;
}

// Constructor for the CSV writer class
ExecutionReportCSVWriter::ExecutionReportCSVWriter(const std::string& fileName) : fileName(fileName) {
    // Open the file in append mode
    file.open(fileName, std::ios_base::app);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        // Handle error, e.g., throw an exception
    }
}

// Destructor to close the file when the writer object is destroyed
ExecutionReportCSVWriter::~ExecutionReportCSVWriter() {
    if (file.is_open()) {
        file.close();
    }
}

// Function to write a single execution record to the CSV file
void ExecutionReportCSVWriter::writeExecutionRecord(const std::vector<std::string>& orderFields) {
    // Ensure the file is open before writing
    if (!file.is_open()) {
        std::cerr << "File is not open for writing: " << fileName << std::endl;
        return;
    }

    // Write each field to the file, separated by commas
    for (size_t i = 0; i < orderFields.size(); ++i) {
        file << orderFields[i];
        if (i < orderFields.size() - 1) {
            file << ",";
        }
    }
    file << "\n";
}

// Function to change the output file name
void ExecutionReportCSVWriter::changeFilename(const std::string& newFileName) {
    // Close the current file and open the new file in append mode
    if (file.is_open()) {
        file.close();
    }
    fileName = newFileName;
    file.open(fileName, std::ios_base::app);
}

// Function to get the list of order IDs
const std::vector<std::string>& ExecutionReportCSVWriter::getOrderIDs() const {
    return orderIDs;
}

// Function to write all buffered records with multi-threading control
void ExecutionReportCSVWriter::writeAllRecords(std::string& buffer, std::mutex& bufferMutex, int& writerStatus, std::condition_variable& writeCondition)
{
    int orderCount = 0;
    bool writingFinished = false;

    while (true) {
        {
            std::unique_lock<std::mutex> lock(bufferMutex);
            writingFinished = writerStatus > 5;
            // Wait for the buffer to fill up or writing to finish
            writeCondition.wait(lock, [&] { return (buffer.size() > 16000) || writingFinished; });

            if (writingFinished && buffer.empty()) {
                break;
            }

            std::string bufferedLines = std::move(buffer);
            lock.unlock();

            orderCount++;
            writeExecutionRecord(bufferedLines);  // Write the buffered lines to the file
        }
    }
    // std::cout << "Writer finished writing records" << std::endl;
}

// Helper function to write a single record to the file
void ExecutionReportCSVWriter::writeExecutionRecord(std::string singleRecord) {
    if (!file.is_open()) {
        std::cerr << "File is not open for writing: " << fileName << std::endl;
        return;
    }

    // Ensure the record ends with a newline character
    if (singleRecord.back() != '\n') {
        singleRecord += '\n';
    }
    file << singleRecord;
}
