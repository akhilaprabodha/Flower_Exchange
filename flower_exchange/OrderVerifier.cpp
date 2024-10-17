// Verifier
#include "OrderVerifier.h"

OrderVerifier::OrderVerifier(ExecutionReportCSVWriter& writer) : writer(writer) {
    // ...
}

OrderVerifier::~OrderVerifier() {
    // ...
}

bool OrderVerifier::validateOrder(const std::vector<std::string>& order) {
    std::string reason = "";

    // check for required fields
    if (order.size() != 6) {
        reason = "Invalid number of fields";
    }
    else if (order[2] != "Rose" && order[2] != "Lavender" && order[2] != "Lotus" && order[2] != "Tulip" && order[2] != "Orchid") { // check for invalid instrument
        reason = "Invalid instrument";
    }
    else if (order[3] != "1" && order[3] != "2") { // check for invalid side
        reason = "Invalid side";
    }
    else if (stod(order[4]) <= 0) { // check for price greater than 0
        reason = "Price must be greater than 0";
    }
    else if (stoi(order[5]) % 10 != 0) { // check for quantity multiple of 10
        reason = "Quantity must be a multiple of 10";
    }
    else if (stoi(order[5]) < 10 || stoi(order[5]) > 1000) { // check for quantity in range
        reason = "Quantity must be in the range 10 to 1000";
    }
    else {
        return true;
    }
    recordRejectedOrder(order, reason);
    return false;
}

int OrderVerifier::newVerifier(const std::vector<std::string>& order) {
    std::string reason = "";
    // check for required fields
    if (order.size() != 6) {
        reason = "Invalid number of fields";
    }
    else if (order[3] != "1" && order[3] != "2") { // check for invalid side
        reason = "Invalid side";
    }
    else if (stod(order[4]) <= 0) { // check for price greater than 0
        reason = "Price must be greater than 0";
    }
    else if (stoi(order[5]) % 10 != 0) { // check for quantity multiple of 10
        reason = "Quantity must be a multiple of 10";
    }
    else if (stoi(order[5]) < 10 || stoi(order[5]) > 1000) { // check for quantity in range
        reason = "Quantity must be in the range 10 to 1000";
    }
    else {
        if (order[2] == "Rose") {
            return 1;
        }
        else if (order[2] == "Lavender") {
            return 2;
        }
        else if (order[2] == "Lotus") {
            return 3;
        }
        else if (order[2] == "Tulip") {
            return 4;
        }
        else if (order[2] == "Orchid") {
            return 5;
        }
        else {
            reason = "Invalid instrument";
        }
    }
    recordRejectedOrder(order, reason);
    return 0;
}

void OrderVerifier::recordRejectedOrder(const std::vector<std::string>& order, const std::string& reason) {
    std::vector<std::string> rejectedOrder;
    // Increment rejected orders (presumably)
    if (order.size() == 6) {
        rejectedOrder = { order[0], order[1], order[2], order[3], order[4], order[5], "1", reason, getDateTime() };
    }
    else {
        if (order.size() < 6) {
            for (int i = 0; i < order.size(); i++) {
                rejectedOrder.push_back(order[i]);
            }
            for (int i = 0; i < 6 - order.size(); i++) {
                rejectedOrder.push_back("");
            }
            rejectedOrder.push_back("1");
            rejectedOrder.push_back(reason);
            rejectedOrder.push_back(getDateTime());
        }
        else {
            for (int i = 0; i < 5; i++) {
                rejectedOrder.push_back(order[i]);
            }
            std::string txt = "";
            for (int i = 5; i < order.size(); i++) {
                txt += order[i] + " ";
            }
            rejectedOrder.push_back(txt);
            rejectedOrder.push_back("1");
            rejectedOrder.push_back(reason);
            rejectedOrder.push_back(getDateTime());
        }
    }

    writer.writeExecutionRecord(rejectedOrder); // Record the rejected order
    rejectedOrderIDs.push_back(order[0]); // Assuming order[0] is the Client Order ID
}

std::vector<std::string> OrderVerifier::getRejectedReport(const std::vector<std::string>& order, const std::string& reason) {
    std::vector<std::string> rejectedOrder;
    if (order.size() == 6) {
        rejectedOrder = { order[0], order[1], order[2], order[3], order[4], order[5], "1", reason, getDateTime() };
    }
    else {
        if (order.size() < 6) {
            for (int i = 0; i < order.size(); i++) {
                rejectedOrder.push_back(order[i]);
            }
            for (int i = 0; i < 6 - order.size(); i++) {
                rejectedOrder.push_back("");
            }
            rejectedOrder.push_back("1");
            rejectedOrder.push_back(reason);
            rejectedOrder.push_back(getDateTime());
        }
        else {
            for (int i = 0; i < 5; i++) {
                rejectedOrder.push_back(order[i]);
            }
            std::string txt = "";
            for (int i = 5; i < order.size(); i++) {
                txt += order[i] + " ";
            }
            rejectedOrder.push_back(txt);
            rejectedOrder.push_back("1");
            rejectedOrder.push_back(reason);
            rejectedOrder.push_back(getDateTime());
        }
    }
    return rejectedOrder;
}

void OrderVerifier::getRejectedOrderIDs(std::vector<std::string>& orderIDs) {
    orderIDs = rejectedOrderIDs;
}

void OrderVerifier::validateAllOrders(
    std::vector<std::vector<std::string>>& readerBuffer,
    std::string& writerBuffer,
    std::vector<std::vector<std::string>>& roseOrders,
    std::vector<std::vector<std::string>>& lavenderOrders,
    std::vector<std::vector<std::string>>& lotusOrders,
    std::vector<std::vector<std::string>>& tulipOrders,
    std::vector<std::vector<std::string>>& orchidOrders,
    std::mutex& readerMtx, std::mutex& writerMtx, std::mutex& roseMtx,
    std::mutex& lavenderMtx, std::mutex& lotusMtx, std::mutex& tulipMtx, std::mutex& orchidMtx, bool& doneReading,
    bool& doneRose, bool& doneLavender, bool& doneLotus, bool& doneTulip, bool& doneOrchid, int& doneWriting,
    std::condition_variable& cvReader, std::condition_variable& cvWriter, std::condition_variable& cvRose,
    std::condition_variable& cvLavender, std::condition_variable& cvLotus, std::condition_variable& cvTulip, std::condition_variable& cvOrchid) {
    
    int countOut = 0;
    int count = 0;
    int orderCount = 0;
    bool flag = false;
    bool emt = false;
    while (true) {
        countOut++;
        std::unique_lock<std::mutex> lock(readerMtx);
        cvReader.wait(lock, [&] {return !readerBuffer.empty() || doneReading; });

        if (doneReading && readerBuffer.empty()) {
            break;
        }
        std::vector<std::vector<std::string>> orders = std::move(readerBuffer);
        lock.unlock();
        count++;
        for (auto& order : orders) {
            orderCount++;
            int status = threadVerifier(order, writerBuffer, writerMtx, cvWriter);
            if (status == 1) {
                std::lock_guard<std::mutex> lock1(roseMtx);
                roseOrders.push_back(order);
                cvRose.notify_one();
            }
            else if (status == 2) {
                std::lock_guard<std::mutex> lock2(lavenderMtx);
                lavenderOrders.push_back(order);
                cvLavender.notify_one();
            }
            else if (status == 3) {
                std::lock_guard<std::mutex> lock3(lotusMtx);
                lotusOrders.push_back(order);
                cvLotus.notify_one();
            }
            else if (status == 4) {
                std::lock_guard<std::mutex> lock4(tulipMtx);
                tulipOrders.push_back(order);
                cvTulip.notify_one();
            }
            else if (status == 5) {
                std::lock_guard<std::mutex> lock5(orchidMtx);
                orchidOrders.push_back(order);
                cvOrchid.notify_one();
            }
        }
    }

    std::cout << "Order Verifier || OrderCount: " << orderCount << " || Orders Processed: " << count << " || Orders Rejected: " << rejectedOrderIDs.size() << std::endl;
    doneRose = true;
    doneLavender = true;
    doneLotus = true;
    doneTulip = true;
    doneOrchid = true;
    cvRose.notify_one();
    cvLavender.notify_one();
    cvLotus.notify_one();
    cvTulip.notify_one();
    cvOrchid.notify_one();
    doneWriting = 1;
    cvWriter.notify_one();
}

int OrderVerifier::threadVerifier(const std::vector<std::string>& order, std::string& writerBuffer, std::mutex& writerMtx, std::condition_variable& cvWriter) {
    bool valid = validateOrder(order);
    if (valid) {
        return newVerifier(order);
    }
    else {
        return 0;
    }
}

std::string OrderVerifier::getDateTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    return std::ctime(&now_c);
}
