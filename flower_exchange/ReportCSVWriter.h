#pragma once
#ifndef EXECUTION_REPORT_CSV_WRITER_H
#define EXECUTION_REPORT_CSV_WRITER_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <ctime>
#include <chrono>
#include <mutex>
#include <condition_variable>

class ExecutionReportCSVWriter {
private:
    std::string fileName;                    // Name of the CSV file
    std::ofstream file;                      // File stream for writing the CSV
    std::vector<std::string> orderIDs;       // List of order IDs

    // Function to get the current date and time in a formatted string
    std::string getDateTime();

public:
    // Constructor: Initializes the writer with the given file name
    ExecutionReportCSVWriter(const std::string& fileName);

    // Destructor: Closes the file stream
    ~ExecutionReportCSVWriter();

    // Function to write an execution record (a vector of strings) to the CSV file
    void writeExecutionRecord(const std::vector<std::string>& orderFields);

    // Function to change the CSV file name
    void changeFilename(const std::string& newFileName);

    // Function to get the list of order IDs
    const std::vector<std::string>& getOrderIDs() const;

    // Function to write all buffered records in a thread-safe manner
    void writeAllRecords(std::string& buffer, std::mutex& bufferMutex, int& writerStatus, std::condition_variable& writeCondition);

    // Function to write a single execution record (string) to the CSV file
    void writeExecutionRecord(std::string singleRecord);
};

#endif // EXECUTION_REPORT_CSV_WRITER_H
