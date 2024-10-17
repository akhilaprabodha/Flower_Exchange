// OrderFileReader

#include "OrderFileReader.h"
#include <fstream>
#include <sstream>
#include <iostream>

OrderFileReader::OrderFileReader(const std::string& filename) : filename(filename) {
}

void OrderFileReader::getAllOrders(std::vector<std::vector<std::string>>& orders, std::mutex& readerMtx, bool& doneReading, std::condition_variable& cvReader) {
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        std::vector<std::string> order;
        std::stringstream ss(line);
        std::string item;
        while (std::getline(ss, item, ',')) {
            order.push_back(item);
        }
        {
            std::lock_guard<std::mutex> lock(readerMtx);
            orders.push_back(order);
        }
    }
    doneReading = true;
    cvReader.notify_all();
}
