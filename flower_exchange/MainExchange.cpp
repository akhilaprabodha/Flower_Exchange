// MainExchange

#include "OrderFileReader.h"
#include "ExecutionReportWriter.h"
#include "OrderChecker.h"
#include "TradeBook.h"
#include "TradeProcessor.h"
#include "OrderSplitter.h"
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <chrono>
#include <mutex>
#include <thread>
#include <condition_variable>

int main() {
    // Measure time at the start of the program
    auto start = std::chrono::system_clock::now();
    OrderFileReader reader("data/orders.csv"); // Read orders from orders.csv
    ExecutionReportWriter writer("data/Execution_rep.csv"); // Write execution report to Execution_rep.csv
    OrderChecker validator(writer); // Initialize order validator
    TradeProcessor roseOrderprocessor("Rose", writer),
        lavenderOrderprocessor("Lavender", writer),
        lotusOrderprocessor("Lotus", writer),
        tulipOrderprocessor("Tulip", writer),
        orchidOrderprocessor("Orchid", writer);

    // Reader buffer
    std::vector<std::vector<std::string>> newOrders, roseOrders, lavenderOrders, lotusOrders, tulipOrders, orchidOrders;
    // Writer buffer
    std::string executionReports;
    std::mutex readerMtx, roseMtx, lotusMtx, tulipMtx, orchidMtx, writerMtx, lavenderMtx;
    bool doneReading = false;
    bool doneRose = false;
    bool doneLavender = false;
    bool doneLotus = false;
    bool doneTulip = false;
    bool doneOrchid = false;
    int doneWriting = 0;
    std::condition_variable cvReader, cvRose, cvLavender, cvLotus, cvTulip, cvOrchid, cvWriter;

    std::thread readerThread([&reader](std::vector<std::vector<std::string>>& orders, std::mutex& readerMtx, bool& doneReading, std::condition_variable& cvReader) {
        reader.getAllOrders(orders, readerMtx, doneReading, cvReader);
    }, std::ref(newOrders), std::ref(readerMtx), std::ref(doneReading), std::ref(cvReader));

    std::thread roseThread([&roseOrderprocessor](std::vector<std::vector<std::string>>& roseOrders, std::string& executionReports, std::mutex& writerMtx, std::mutex& roseMtx, bool& doneRose, int& doneWriting, std::condition_variable& cvWriter, std::condition_variable& cvInstrument) {
        roseOrderprocessor.ProcessAllOrders(roseOrders, executionReports, writerMtx, roseMtx, doneRose, doneWriting, cvWriter, cvInstrument);
    }, std::ref(roseOrders), std::ref(executionReports), std::ref(writerMtx), std::ref(roseMtx), std::ref(doneRose), std::ref(doneWriting), std::ref(cvWriter), std::ref(cvRose));

    std::thread lavenderThread([&lavenderOrderprocessor](std::vector<std::vector<std::string>>& lavenderOrders, std::string& executionReports, std::mutex& writerMtx, std::mutex& lavenderMtx, bool& doneLavender, int& doneWriting, std::condition_variable& cvWriter, std::condition_variable& cvInstrument) {
        lavenderOrderprocessor.ProcessAllOrders(lavenderOrders, executionReports, writerMtx, lavenderMtx, doneLavender, doneWriting, cvWriter, cvInstrument);
    }, std::ref(lavenderOrders), std::ref(executionReports), std::ref(writerMtx), std::ref(lavenderMtx), std::ref(doneLavender), std::ref(doneWriting), std::ref(cvWriter), std::ref(cvLavender));

    // Additional threads for other processors...

    readerThread.join();
    roseThread.join();
    lavenderThread.join();
    // Join additional threads...

    // Measure time at the end of the program
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Elapsed time: " << elapsed_seconds.count() << "s\n";

    return 0;
}
