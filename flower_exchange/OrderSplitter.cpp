// OrderSplitter

#include "OrderSplitter.h"
#include <sstream>

std::vector<std::string> OrderSplitter::splitOrder(const std::string& order) {
    std::vector<std::string> result;
    std::stringstream ss(order);
    std::string item;
    while (std::getline(ss, item, ',')) {
        result.push_back(item);
    }
    return result;
}
