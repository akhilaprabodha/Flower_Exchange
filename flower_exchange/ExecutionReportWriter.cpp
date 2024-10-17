// ExecutionReportWriter

#include "ExecutionReportWriter.h"
#include <fstream>
#include <iostream>

ExecutionReportWriter::ExecutionReportWriter(const std::string& filename) : filename(filename) {
}

void ExecutionReportWriter::writeReport(const std::string& report) {
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        file << report << std::endl;
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}
