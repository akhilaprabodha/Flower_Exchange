// TradeProcessor.cpp
#include "TradeProcessor.h"

//
TradeProcessor::TradeProcessor(std::string Instrument, ExecutionReportCSVWriter& writer) : writer(writer), orderBook(Instrument) {
	this->Instrument = Instrument;
}

TradeProcessor::~TradeProcessor()
{
}

void TradeProcessor::recordOrder(const std::vector<std::string>& order, int status, std::string quantity, double price)
{
	// print vec + status + quantity + price
	std::vector<std::string> orderDetails = { order[0], order[1], order[2], order[3], std::to_string(price), quantity, std::to_string(status), "_", getDateTime() };
	// OrderId, C_Id, Instrument, Side, Price, Quantity, Status, Reason, TimeStamp

	// Write order details to execution report writer
	writer.writeExecutionRecord(orderDetails);
}

void TradeProcessor::updateOrder(std::vector<std::string>& order, int quantity)
{
	// Update the order's quantity
	order[5] = std::to_string(quantity);
	// return order;
}

void TradeProcessor::processTrade(const std::vector<std::string>& ord)
{
	std::vector<std::string> order = ord;

	// For buy trades
	if (order[3] == "1") {
		bool temp = true;

		while (temp) {
			// check the Sell Order Book is empty
			if (!orderBook.isSellEmpty()) {
				// check minimum price in sell order book
				if (orderBook.getSellMinVal() <= stod(order[4])) { // SellMinVal <= BuyPrice
					std::vector<std::string> sellVec;
					sellVec = orderBook.getSellMinVec();
					// compare quantity
					if (stoi(sellVec[5]) == stoi(order[5])) {	// SellQuantity == BuyQuantity
						recordOrder(order, 2, sellVec[5], orderBook.getSellMinVal());	// record buy trade in execution table
						recordOrder(sellVec, 2, sellVec[5], orderBook.getSellMinVal());	// record sell trade in execution table
						orderBook.popSellMinVec();		// delete sell order from sell order book
						temp = false;
					}
					else if (stoi(sellVec[5]) > stoi(order[5])) {	// SellQuantity > BuyQuantity
						recordOrder(order, 2, order[5], orderBook.getSellMinVal());	// record buy trade in execution table
						recordOrder(sellVec, 3, order[5], orderBook.getSellMinVal());	// record sell trade in execution table
						updateOrder(sellVec, stoi(sellVec[5]) - stoi(order[5]));
						orderBook.updateSellMinVec(sellVec);		// Update Sell order book
						temp = false;
					}
					else { // SellQuantity < BuyQuantity
						recordOrder(order, 3, sellVec[5], orderBook.getSellMinVal());
						recordOrder(sellVec, 2, sellVec[5], orderBook.getSellMinVal());
						updateOrder(order, stoi(order[5]) - stoi(sellVec[5]));	// update selected buy order
						orderBook.popSellMinVec();
					}
				}
				else {
					orderBook.writeBuy(order);
					recordOrder(order, 0, order[5], stod(order[4]));
					temp = false;
				}
			}
			else {
				orderBook.writeBuy(order);
				recordOrder(order, 0, order[5], stod(order[4]));
				temp = false;
			}
		}
	}
	// For sell trades
	else if (order[3] == "2") {
		bool temp = true;
		while (temp) {
			// check the Buy Order Book is empty
			if (!orderBook.isBuyEmpty()) {
				// check maximum price in buy order book
				if (orderBook.getBuyMaxVal() >= stod(order[4])) {
					std::vector<std::string> buyVec;
					buyVec = orderBook.getBuyMaxVec();
					// compare quantity
					if (stoi(buyVec[5]) == stoi(order[5])) {	// SellQuantity == BuyQuantity
						recordOrder(order, 2, buyVec[5], orderBook.getBuyMaxVal());	// record sell trade in execution table
						recordOrder(buyVec, 2, buyVec[5], orderBook.getBuyMaxVal());	// record buy trade in execution table
						orderBook.popBuyMaxVec();		// delete buy order from buy order book
						temp = false;
					}
					else if (stoi(buyVec[5]) > stoi(order[5])) {	// BuyQuantity > SellQuantity
						recordOrder(order, 2, order[5], orderBook.getBuyMaxVal());	// record sell trade in execution table
						recordOrder(buyVec, 3, order[5], orderBook.getBuyMaxVal());	// record buy trade in execution table
						updateOrder(buyVec, stoi(buyVec[5]) - stoi(order[5]));	// update buy vector
						orderBook.updateBuyMaxVec(buyVec);		// update buy order book
						temp = false;
					}
					else { // SellQuantity > BuyQuantity
						recordOrder(order, 3, buyVec[5], orderBook.getBuyMaxVal());
						recordOrder(buyVec, 2, buyVec[5], orderBook.getBuyMaxVal());
						updateOrder(order, stoi(order[5]) - stoi(buyVec[5]));	// update selected sell order
						orderBook.popBuyMaxVec();
					}
				}
				else {
					orderBook.writeSell(order);
					recordOrder(order, 0, order[5], stod(order[4]));
					temp = false;
				}
			}
			else {
				orderBook.writeSell(order);
				recordOrder(order, 0, order[5], stod(order[4]));
				temp = false;
			}
		}
	}
}

void TradeProcessor::processAllTrades(
	std::vector<std::vector<std::string>>& orderBuffer,
	std::string& writerBuffer,
	std::mutex& writerMtx,
	std::mutex& orderMtx,
	bool& doneOrder,
	int& doneWriting,
	std::condition_variable& cvWriter,
	std::condition_variable& cvInstrument)
{
	int countOut = 0;
	int count = 0;
	int orderCount = 0;
	std::vector<std::string> order;
	while (true) {
		{
			countOut++;
			std::unique_lock<std::mutex> lock(orderMtx);
			cvInstrument.wait(lock, [&] {return !orderBuffer.empty() || doneOrder; });
			if (doneOrder && orderBuffer.empty()) {
				break;
			}
			std::vector<std::vector<std::string>> orders = std::move(orderBuffer);
			lock.unlock();
			for (auto& order : orders) {
				orderCount++;
				processTrade(order, writerBuffer, writerMtx, cvWriter);
			}
		}
	}
	std::cout << "Trade Processor " << this->Instrument << " Trade Count : " << orderCount << " Execution Reports Written : " << nExecuted << std::endl;
	{
		std::unique_lock<std::mutex> lock6(writerMtx);
		doneWriting++;
		cvWriter.notify_one();
	}

}

std::string TradeProcessor::getDateTime()
{
	auto now = std::chrono::system_clock::now();
	auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
	auto epoch = now_ms.time_since_epoch();

	std::time_t now_c = std::chrono::duration_cast<std::chrono::seconds>(epoch).count();

	std::tm time_info;
#ifdef _WIN32
	localtime_s(&time_info, &now_c); // Use localtime_s on Windows
#else
	localtime_r(&now_c, &time_info); // Use localtime_r on non-Windows systems
#endif

	char buffer[80];
	std::strftime(buffer, sizeof(buffer), "%Y%m%d%H%M%S", &time_info);

	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(epoch) % 1000;
	sprintf_s(buffer, "%s.%03lld", buffer, ms.count());

	return buffer;
}

void TradeProcessor::processTrade(const std::vector<std::string>& ord, std::string& writerBuffer, std::mutex& writerMtx, std::condition_variable& cvWriter)
{
	std::vector<std::string> order = ord;

	// For buy trades
	if (order[3] == "1") {
		bool temp = true;

		while (temp) {
			// check the Sell Order Book is empty
			if (!orderBook.isSellEmpty()) {
				// check minimum price in sell order book
				if (orderBook.getSellMinVal() <= stod(order[4])) { // SellMinVal <= BuyPrice
					std::vector<std::string> sellVec;
					sellVec = orderBook.getSellMinVec();
					// compare quantity
					if (stoi(sellVec[5]) == stoi(order[5])) {	// SellQuantity == BuyQuantity
						recordOrder(order, 2, sellVec[5], orderBook.getSellMinVal(), std::ref(writer
