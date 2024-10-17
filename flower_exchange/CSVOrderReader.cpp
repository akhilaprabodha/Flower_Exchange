// CSVOrderReader.cpp
#include "CSVOrderReader.h"

using namespace std;

CSVOrderReader::CSVOrderReader(const std::string& filename, int orderId) : filename(filename) {
    this->orderId = orderId;
    file.open(filename); // Open CSV file for reading
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        // Handle error, e.g., throw an exception
    }
    // Skip the header line of the CSV file
    string header;
    getline(file, header);
}

CSVOrderReader::~CSVOrderReader() {
    if (file.is_open()) {
        file.close(); // Close the file if it is open
    }
}

bool CSVOrderReader::getNextOrderLine(std::vector<std::string>& orderLine) {
    if (!file.eof()) {
        string line;
        if (getline(file, line)) {

            stringstream ss(line);
            string field;
            orderLine.clear();
            // Assign unique order ID prefix "ord" and increment the order ID
            orderLine.push_back("ord" + to_string(++orderId));
            while (getline(ss, field, ',')) {
                orderLine.push_back(field); // Split line into fields based on comma
            }
            return true;
        }
    }
    return false; // Return false if end of file is reached
}

void CSVOrderReader::changeFilename(const std::string& newFilename) {
    file.close(); // Close the current file
    filename = newFilename;
    file.open(filename); // Open new CSV file
}

void CSVOrderReader::getAllOrders(std::vector<std::vector<std::string>>& orders, std::mutex& readerMtx, bool& doneReading, std::condition_variable& cvReader) {
    std::vector<std::string> orderLine;
    while (getNextOrderLine(orderLine)) {
        {
            std::unique_lock<std::mutex> lock(readerMtx); // Lock mutex for safe access
            orders.push_back(orderLine); // Add the read order line to the orders list
            cvReader.notify_one(); // Notify that a new order is available
        }
    }
    {
        std::lock_guard<std::mutex> lock(readerMtx); // Lock mutex to update doneReading
        doneReading = true; // Mark reading as done
        cvReader.notify_one(); // Notify that reading is completed
    }
}

