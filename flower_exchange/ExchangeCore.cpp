// ExchangeCore

#include "OrderCSVReader.h"
#include "ReportCSVWriter.h"
#include "OrderVerifier.h"
#include "TradeProcessor.h"
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <chrono>
#include <mutex>
#include <thread>
#include <condition_variable>

int main() {
    // Measure the start time of the program
    auto startTime = std::chrono::system_clock::now();

    // Initialize the CSV reader for order data
    OrderCSVReader orderReader("data/orders.csv"); // Adjusted to orders.csv for 100000 inputs

    // Initialize the CSV writer for execution reports
    ReportCSVWriter reportWriter("data/Execution_rep.csv");

    // Initialize the order validator
    OrderVerifier orderVerifier(reportWriter);

    // Initialize order processors for different flowers
    TradeProcessor roseOrderProcessor("Rose", reportWriter),
        lavenderOrderProcessor("Lavender", reportWriter),
        lotusOrderProcessor("Lotus", reportWriter),
        tulipOrderProcessor("Tulip", reportWriter),
        orchidOrderProcessor("Orchid", reportWriter);

    // Buffers to store orders and execution reports
    std::vector<std::vector<std::string>> allOrders, roseOrders, lavenderOrders, lotusOrders, tulipOrders, orchidOrders;
    std::string executionReportBuffer;

    // Mutexes for synchronizing access to resources
    std::mutex orderReaderMutex, roseMutex, lotusMutex, tulipMutex, orchidMutex, reportWriterMutex, lavenderMutex;

    // Flags and condition variables for managing thread execution
    bool doneReading = false;
    bool doneRose = false;
    bool doneLavender = false;
    bool doneLotus = false;
    bool doneTulip = false;
    bool doneOrchid = false;
    int doneWriting = 0;
    std::condition_variable cvOrderReader, cvRose, cvLavender, cvLotus, cvTulip, cvOrchid, cvWriter;

    // Reader thread: Read all orders from the CSV
    std::thread readerThread([&orderReader](std::vector<std::vector<std::string>>& ordersBuffer, std::mutex& readerMutex, bool& doneReadingFlag, std::condition_variable& cvReader) {
        orderReader.getAllOrders(ordersBuffer, readerMutex, doneReadingFlag, cvReader);
    }, std::ref(allOrders), std::ref(orderReaderMutex), std::ref(doneReading), std::ref(cvOrderReader));

    // Processor threads for each type of flower
    std::thread roseThread([&roseOrderProcessor](std::vector<std::vector<std::string>>& roseOrdersBuffer, std::string& reportBuffer, std::mutex& writerMutex, std::mutex& roseMutex, bool& doneRoseFlag, int& writingDoneFlag, std::condition_variable& cvWriter, std::condition_variable& cvProcessor) {
        roseOrderProcessor.ProcessAllOrders(roseOrdersBuffer, reportBuffer, writerMutex, roseMutex, doneRoseFlag, writingDoneFlag, cvWriter, cvProcessor);
    }, std::ref(roseOrders), std::ref(executionReportBuffer), std::ref(reportWriterMutex), std::ref(roseMutex), std::ref(doneRose), std::ref(doneWriting), std::ref(cvWriter), std::ref(cvRose));

    std::thread lavenderThread([&lavenderOrderProcessor](std::vector<std::vector<std::string>>& lavenderOrdersBuffer, std::string& reportBuffer, std::mutex& writerMutex, std::mutex& lavenderMutex, bool& doneLavenderFlag, int& writingDoneFlag, std::condition_variable& cvWriter, std::condition_variable& cvProcessor) {
        lavenderOrderProcessor.ProcessAllOrders(lavenderOrdersBuffer, reportBuffer, writerMutex, lavenderMutex, doneLavenderFlag, writingDoneFlag, cvWriter, cvProcessor);
    }, std::ref(lavenderOrders), std::ref(executionReportBuffer), std::ref(reportWriterMutex), std::ref(lavenderMutex), std::ref(doneLavender), std::ref(doneWriting), std::ref(cvWriter), std::ref(cvLavender));

    std::thread lotusThread([&lotusOrderProcessor](std::vector<std::vector<std::string>>& lotusOrdersBuffer, std::string& reportBuffer, std::mutex& writerMutex, std::mutex& lotusMutex, bool& doneLotusFlag, int& writingDoneFlag, std::condition_variable& cvWriter, std::condition_variable& cvProcessor) {
        lotusOrderProcessor.ProcessAllOrders(lotusOrdersBuffer, reportBuffer, writerMutex, lotusMutex, doneLotusFlag, writingDoneFlag, cvWriter, cvProcessor);
    }, std::ref(lotusOrders), std::ref(executionReportBuffer), std::ref(reportWriterMutex), std::ref(lotusMutex), std::ref(doneLotus), std::ref(doneWriting), std::ref(cvWriter), std::ref(cvLotus));

    std::thread tulipThread([&tulipOrderProcessor](std::vector<std::vector<std::string>>& tulipOrdersBuffer, std::string& reportBuffer, std::mutex& writerMutex, std::mutex& tulipMutex, bool& doneTulipFlag, int& writingDoneFlag, std::condition_variable& cvWriter, std::condition_variable& cvProcessor) {
        tulipOrderProcessor.ProcessAllOrders(tulipOrdersBuffer, reportBuffer, writerMutex, tulipMutex, doneTulipFlag, writingDoneFlag, cvWriter, cvProcessor);
    }, std::ref(tulipOrders), std::ref(executionReportBuffer), std::ref(reportWriterMutex), std::ref(tulipMutex), std::ref(doneTulip), std::ref(doneWriting), std::ref(cvWriter), std::ref(cvTulip));

    std::thread orchidThread([&orchidOrderProcessor](std::vector<std::vector<std::string>>& orchidOrdersBuffer, std::string& reportBuffer, std::mutex& writerMutex, std::mutex& orchidMutex, bool& doneOrchidFlag, int& writingDoneFlag, std::condition_variable& cvWriter, std::condition_variable& cvProcessor) {
        orchidOrderProcessor.ProcessAllOrders(orchidOrdersBuffer, reportBuffer, writerMutex, orchidMutex, doneOrchidFlag, writingDoneFlag, cvWriter, cvProcessor);
    }, std::ref(orchidOrders), std::ref(executionReportBuffer), std::ref(reportWriterMutex), std::ref(orchidMutex), std::ref(doneOrchid), std::ref(doneWriting), std::ref(cvWriter), std::ref(cvOrchid));

    // Writer thread: Write all execution reports
    std::thread writerThread([&reportWriter](std::string& reportBuffer, std::mutex& writerMutex, int& writingDoneFlag, std::condition_variable& cvWriter) {
        reportWriter.writeAllRecords(reportBuffer, writerMutex, writingDoneFlag, cvWriter);
    }, std::ref(executionReportBuffer), std::ref(reportWriterMutex), std::ref(doneWriting), std::ref(cvWriter));

    // Validate all orders and categorize them into the respective buffers
    orderVerifier.validateAllOrders(std::ref(allOrders), std::ref(executionReportBuffer), std::ref(roseOrders), std::ref(lavenderOrders), std::ref(lotusOrders), std::ref(tulipOrders), std::ref(orchidOrders),
        std::ref(orderReaderMutex), std::ref(reportWriterMutex), std::ref(roseMutex), std::ref(lavenderMutex), std::ref(lotusMutex), std::ref(tulipMutex), std::ref(orchidMutex),
        std::ref(doneReading), std::ref(doneRose), std::ref(doneLavender), std::ref(doneLotus), std::ref(doneTulip), std::ref(doneOrchid), std::ref(doneWriting),
        std::ref(cvOrderReader), std::ref(cvRose), std::ref(cvLavender), std::ref(cvLotus), std::ref(cvTulip), std::ref(cvOrchid), std::ref(cvWriter)
    );

    // Wait for all threads to finish
    readerThread.join();
    roseThread.join();
    lavenderThread.join();
    lotusThread.join();
    tulipThread.join();
    orchidThread.join();
    writerThread.join();

    // Measure the end time of the program and calculate elapsed time
    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsedTime = endTime - startTime;
    std::cout << "Time taken to process all orders: " << elapsedTime.count() << " seconds.\n";

    return 0;
}
