#include "OrderManager.h"

OrderManager::OrderManager(std::string instrument) {
    // Set the instrument name
    this->instrument = instrument;
}

OrderManager::~OrderManager() {}

// Sell Orderbook Functions
bool OrderManager::isSellEmpty() {
    return sellHeap.empty();
}

double OrderManager::getSellMinVal() {
    return stod(sellHeap.top()[4]);
}

std::vector<std::string> OrderManager::getSellMinVec() {
    return sellHeap.top();
}

void OrderManager::popSellMinVec() {
    sellHeap.pop();
}

void OrderManager::updateSellMinVec(const std::vector<std::string>& order) {
    sellHeap.pop();
    sellHeap.push(order);
}

void OrderManager::writeSell(const std::vector<std::string>& order) {
    sellHeap.push(order);
}

// Buy Orderbook Functions
bool OrderManager::isBuyEmpty() {
    return buyHeap.empty();
}

double OrderManager::getBuyMaxVal() {
    return stod(buyHeap.top()[4]);
}

std::vector<std::string> OrderManager::getBuyMaxVec() {
    return buyHeap.top();
}

void OrderManager::popBuyMaxVec() {
    buyHeap.pop();
}

void OrderManager::updateBuyMaxVec(const std::vector<std::string>& order) {
    buyHeap.pop();
    buyHeap.push(order);
}

void OrderManager::writeBuy(const std::vector<std::string>& order) {
    buyHeap.push(order);
}

std::string OrderManager::getInstrument() {
    return instrument;
}

