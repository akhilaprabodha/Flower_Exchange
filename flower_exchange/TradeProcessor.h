#pragma once
#ifndef TRADE_PROCESSOR_H
#define TRADE_PROCESSOR_H

#include <vector>
#include <string>
#include "ReportCSVWriter.h"  // Updated include for the CSV writer
#include "OrderManager.h"      // Updated include for the order manager
#include <mutex>
#include <condition_variable>

class TradeProcessor {  // Updated class name
private:
    // Private variables
    ReportCSVWriter& writer;  // Updated writer type
    int nExecuted;  // Number of executed trades

    OrderManager orderBook;  // Updated order book type
    //std::vector<std::string> rejectedOrderIDs;  // Potential future use

    // Private functions
    void recordOrder(const std::vector<std::string>& order, int status, std::string quantity, double price);
    void updateOrder(std::vector<std::string>& order, int quantity);
    std::string getDateTime();  // Get current date and time
    void ProcessTrade(const std::vector<std::string>& ord, std::string& writerBuffer, std::mutex& writerMtx, std::condition_variable& cvWriter);  // Updated method name
    void recordOrder(const std::vector<std::string>& order, int status, std::string quantity, double price, std::string& writerBuffer, std::mutex& writerMtx, std::condition_variable& cvWriter);  // Updated method signature

public:
    std::string Instrument;  // Instrument for trading
    // Public functions and variables
    TradeProcessor(std::string Instrument, ReportCSVWriter& writer);  // Updated constructor
    ~TradeProcessor();  // Updated destructor

    void ProcessOrder(const std::vector<std::string>& order);  // Process individual order

    void ProcessAllOrders(
        std::vector<std::vector<std::string>>& orderBuffer,
        std::string& writerBuffer,
        std::mutex& writerMtx,
        std::mutex& orderMtx,
        bool& doneOrder,
        int& doneWriting,
        std::condition_variable& cvWriter,
        std::condition_variable& cvInstrument);  // Process multiple orders

};

#endif // TRADE_PROCESSOR_H
