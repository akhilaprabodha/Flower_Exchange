/*
 * ReportCSVWriter.cpp
 * Handles writing execution reports to a CSV file.
 * 
 * This module is responsible for generating and storing reports about processed transactions
 * in a structured CSV format.
 */

#include "ReportCSVWriter.h"
#include <fstream>

// Writes the execution report to a CSV file.
void ReportCSVWriter::writeReport(const std::vector<ExecutionReport>& reports) {
    std::ofstream csvFile("execution_report.csv");
    
    // Write header
    csvFile << "OrderID,Price,Quantity,Timestamp\n";
    
    // Iterate through reports and write each row to the file
    for (const auto& report : reports) {
        csvFile << report.orderID << "," << report.price << "," << report.quantity << "," << report.timestamp << "\n";
    }
}
