// OrderManager.h 
#pragma once
#ifndef ORDER_MANAGER_H
#define ORDER_MANAGER_H

#include <vector>
#include <string>
#include <iostream>
#include <queue>
#include <vector>

// Comparator for the Sell Orderbook (Min Heap)
struct CompareSell {
    bool operator()(const std::vector<std::string>& a, const std::vector<std::string>& b) {
        double priceA = std::stod(a[4]); // Extract price from vector 'a'
        double priceB = std::stod(b[4]); // Extract price from vector 'b'

        if (priceA == priceB) {
            // Time priority logic based on Client Order ID or timestamp (assuming ID here)
            return std::stoi(a[0].substr(3)) > std::stoi(b[0].substr(3)); // Lower Client Order ID has higher priority
        }

        return priceA > priceB; // Smaller price has higher priority in Sell Orderbook
    }
};

// Comparator for the Buy Orderbook (Max Heap)
struct CompareBuy {
    bool operator()(const std::vector<std::string>& a, const std::vector<std::string>& b) {
        double priceA = std::stod(a[4]); // Extract price from vector 'a'
        double priceB = std::stod(b[4]); // Extract price from vector 'b'

        if (priceA == priceB) {
            // Time priority logic based on Client Order ID or timestamp (assuming ID here)
            return std::stoi(a[0].substr(3)) > std::stoi(b[0].substr(3)); // Lower Client Order ID has higher priority
        }

        return priceA < priceB; // Larger price has higher priority in Buy Orderbook
    }
};

class OrderManager {
private:
    std::string Instrument; // Instrument for which the order book is being managed
    std::priority_queue<std::vector<std::string>, std::vector<std::vector<std::string>>, CompareBuy> buyHeap; // Max heap for buy orders
    std::priority_queue<std::vector<std::string>, std::vector<std::vector<std::string>>, CompareSell> sellHeap; // Min heap for sell orders

public:
    OrderManager(std::string Instrument); // Constructor to initialize with Instrument
    ~OrderManager(); // Destructor

    // SELL Orderbook Functions
    bool isSellEmpty(); // Check if sell heap is empty
    double getSellMinVal(); // Get minimum sell price
    std::vector<std::string> getSellMinVec(); // Get top order in the sell heap
    void popSellMinVec(); // Remove the top sell order
    void updateSellMinVec(const std::vector<std::string>& order); // Update the top sell order
    void writeSell(const std::vector<std::string>& order); // Insert new sell order

    // BUY Orderbook Functions
    bool isBuyEmpty(); // Check if buy heap is empty
    double getBuyMaxVal(); // Get maximum buy price
    std::vector<std::string> getBuyMaxVec(); // Get top order in the buy heap
    void popBuyMaxVec(); // Remove the top buy order
    void updateBuyMaxVec(const std::vector<std::string>& order); // Update the top buy order
    void writeBuy(const std::vector<std::string>& order); // Insert new buy order

    std::string getInstrument(); // Get the instrument name
};

#endif // ORDER_MANAGER_H
