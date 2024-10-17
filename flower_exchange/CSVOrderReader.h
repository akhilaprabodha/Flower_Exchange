// CSVOrderReader.h

#ifndef CSV_ORDER_READER_H
#define CSV_ORDER_READER_H

#include <vector>
#include <string>
#include <fstream>
#include <mutex>
#include <iostream>
#include <sstream>
#include <condition_variable>

using namespace std;

class CSVOrderReader {
private:
    string filename; // Filename of the CSV file
    ifstream file; // File stream for reading the CSV
    int orderId; // Order ID for tracking unique orders

public:
    CSVOrderReader(const std::string& filename, int orderId = 0); // Constructor
    ~CSVOrderReader(); // Destructor

    bool getNextOrderLine(std::vector<std::string>& orderLine); // Read the next order line from CSV
    void changeFilename(const std::string& newFilename); // Change the file being read
    void getAllOrders(std::vector<std::vector<std::string>>& orders, std::mutex& readerMtx, bool& doneReading, std::condition_variable& cvReader); // Read all orders into a list
};

#endif // CSV_ORDER_READER_H
