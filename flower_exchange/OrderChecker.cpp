// OrderChecker

#include "OrderChecker.h"
#include <iostream>

OrderChecker::OrderChecker(ExecutionReportWriter& writer) : writer(writer) {
}

bool OrderChecker::validateOrder(const std::vector<std::string>& order) {
    // Validate order logic
    if (order.size() != 6) {
        std::cerr << "Invalid order size" << std::endl;
        return false;
    }
    // Additional validation logic
    return true;
}
