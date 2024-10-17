// OrderVerifier.h
#pragma once
#ifndef ORDER_VERIFIER_H
#define ORDER_VERIFIER_H

#include "ReportCSVWriter.h"  // Updated include to ReportCSVWriter
#include <vector>
#include <string>
#include <iostream>
#include <ctime>
#include <chrono>
#include <mutex>
#include <condition_variable>

class OrderVerifier {  // Class name changed to OrderVerifier
private:
    ReportCSVWriter& writer;  // Updated to ReportCSVWriter
    std::vector<std::string> rejectedOrderIDs;
    int nRejected;

    // Increments the count of rejected orders
    void incrementRejectedOrders(std::string& orderID);
    
    // Records a rejected order with a specified reason
    void recordRejectedOrder(const std::vector<std::string>& order, const std::string& reason);
    
    // Gets a report of rejected orders
    std::vector<std::string> getRejectedReport(const std::vector<std::string>& order, const std::string& reason);
    
    // Gets the current date and time
    std::string getDateTime();

public:
    // Constructor initializing with ReportCSVWriter
    OrderVerifier(ReportCSVWriter& writer);
    ~OrderVerifier();

    // Validates an order and returns the result
    bool validateOrder(const std::vector<std::string>& order);
    
    // Processes a new order verification
    int newVerifier(const std::vector<std::string>& order);
    
    // Retrieves the IDs of rejected orders
    void getRejectedOrderIDs(std::vector<std::string>& orderIDs);
    
    // Retrieves the rejected orders
    void getRejectedOrders(std::vector<std::string>& orders);

    // Validates all orders from the reader buffer
    void validateAllOrders(
        std::vector<std::vector<std::string>>& readerBuffer,
        std::string& writerBuffer,
        std::vector<std::vector<std::string>>& roseOrders,
        std::vector<std::vector<std::string>>& lavenderOrders,
        std::vector<std::vector<std::string>>& lotusOrders,
        std::vector<std::vector<std::string>>& tulipOrders,
        std::vector<std::vector<std::string>>& orchidOrders,
        std::mutex& readerMtx,
        std::mutex& writerMtx,
        std::mutex& roseMtx,
        std::mutex& lavenderMtx,
        std::mutex& lotusMtx,
        std::mutex& tulipMtx,
        std::mutex& orchidMtx,
        bool& doneReading,
        bool& doneRose,
        bool& doneLavender,
        bool& doneLotus,
        bool& doneTulip,
        bool& doneOrchid,
        int& doneWriting,
        std::condition_variable& cvReader,
        std::condition_variable& cvWriter,
        std::condition_variable& cvRose,
        std::condition_variable& cvLavender,
        std::condition_variable& cvLotus,
        std::condition_variable& cvTulip,
        std::condition_variable& cvOrchid);

    // Validator for a thread
    int threadVerifier(const std::vector<std::string>& order, std::string& writerBuffer, std::mutex& writerMtx, std::condition_variable& cvWriter);
    
    // Gets the rejected order line for reporting
    std::string getRejectedOrderLine(const std::vector<std::string>& order, const std::string& reason);
};

#endif // ORDER_VERIFIER_H

