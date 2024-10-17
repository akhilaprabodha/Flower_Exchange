// TradeBook

#include "TradeBook.h"

TradeBook::TradeBook(std::string Instrument) {
    // Set the instrument name
    this->Instrument = Instrument;
    // Instantiate the Sell Orderbook
}

TradeBook::~TradeBook() {
}

// Sell Orderbook Functions
bool TradeBook::isSellEmpty() {
    return sellHeap.empty();
}

double TradeBook::getSellMinVal() {
    // Get the price of the top vector in the sellHeap
    return stod(sellHeap.top()[4]);
}

std::vector<std::string> TradeBook::getSellMinVec() {
    // Get a copy of top vector in the sellHeap without popping it
    return sellHeap.top();
}

void TradeBook::popSellMinVec() {
    // Pop the top vector in the sellHeap
    sellHeap.pop();
}

void TradeBook::updateSellMinVec(const std::vector<std::string>& order) {
    // Update the top vector in the sellHeap by popping it
    sellHeap.pop();
    sellHeap.push(order);
}

void TradeBook::writeSell(const std::vector<std::string>& order) {
    // Write new order to the sellHeap
    sellHeap.push(order);
}

// Buy Orderbook Functions
bool TradeBook::isBuyEmpty() {
    return buyHeap.empty();
}

double TradeBook::getBuyMaxVal() {
    // Get the price of the top vector in the buyHeap
    return stod(buyHeap.top()[4]);
}

std::vector<std::string> TradeBook::getBuyMaxVec() {
    // Get a copy of top vector in the buyHeap without popping it
    return buyHeap.top();
}

void TradeBook::popBuyMaxVec() {
    // Pop the top vector in the buyHeap
    buyHeap.pop();
}

void TradeBook::updateBuyMaxVec(const std::vector<std::string>& order) {
    // Update the top vector in the buyHeap by popping it
    buyHeap.pop();
    buyHeap.push(order);
}

void TradeBook::writeBuy(const std::vector<std::string>& order) {
    // Write new order to the buyHeap
    buyHeap.push(order);
}

std::string TradeBook::getInstrument() {
    return Instrument;
}
