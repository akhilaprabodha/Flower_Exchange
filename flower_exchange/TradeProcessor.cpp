// TradeProcessor

#include "TradeProcessor.h"

void TradeProcessor::ProcessOrder(const std::vector<std::string>& ord) {
    std::vector<std::string> order = ord;
    // For buy orders
    if (order[3] == "1") {
        bool temp = true;
        while (temp) {
            // Check if the Sell Order Book is empty
            if (!tradeBook.isSellEmpty()) {
                // Check minimum price in sell order book
                if (tradeBook.getSellMinVal() <= stod(order[4])) { // SellMinVal <= BuyPrice
                    std::vector<std::string> SellVec;
                    SellVec = tradeBook.getSellMinVec();
                    // Compare quantity
                    if (stoi(SellVec[5]) == stoi(order[5])) { // SellQuantity == BuyQuantity
                        recordOrder(order, 2, SellVec[5], tradeBook.getSellMinVal()); // Record buy order in execution table 
                        recordOrder(SellVec, 2, SellVec[5], tradeBook.getSellMinVal()); // Record sell order in execution table
                        tradeBook.popSellMinVec(); // Delete sell order from sell order book
                        temp = false;
                    } else if (stoi(SellVec[5]) > stoi(order[5])) { // SellQuantity > BuyQuantity
                        recordOrder(order, 2, order[5], tradeBook.getSellMinVal()); // Record buy order in execution table 
                        recordOrder(SellVec, 3, order[5], tradeBook.getSellMinVal()); // Record sell order in execution table
                        updateOrder(SellVec, stoi(SellVec[5]) - stoi(order[5]));
                        tradeBook.updateSellMinVec(SellVec); // Update Sell order book
                        temp = false;
                    } else { // SellQuantity < BuyQuantity
                        recordOrder(order, 3, SellVec[5], tradeBook.getSellMinVal());
                        recordOrder(SellVec, 2, SellVec[5], tradeBook.getSellMinVal());
                        updateOrder(order, stoi(order[5]) - stoi(SellVec[5])); // Update selected buy order
                        tradeBook.popSellMinVec();
                    }
                } else {
                    tradeBook.writeBuy(order);
                    recordOrder(order, 0, order[5], stod(order[4]));
                    temp = false;
                }
            } else {
                tradeBook.writeBuy(order);
                recordOrder(order, 0, order[5], stod(order[4]));
                temp = false;
            }
        }
    }
    // For sell orders
    else if (order[3] == "2") {
        bool temp = true;
        while (temp) {
            // Check if the Buy Order Book is empty
            if (!tradeBook.isBuyEmpty()) {
                // Check maximum price in buy order book
                if (tradeBook.getBuyMaxVal() >= stod(order[4])) {
                    std::vector<std::string> BuyVec;
                    BuyVec = tradeBook.getBuyMaxVec();
                    // Compare quantity
                    if (stoi(BuyVec[5]) == stoi(order[5])) { // SellQuantity == BuyQuantity
                        recordOrder(order, 2, BuyVec[5], tradeBook.getBuyMaxVal()); // Record sell order in execution table 
                        recordOrder(BuyVec, 2, BuyVec[5], tradeBook.getBuyMaxVal()); // Record buy order in execution table
                        tradeBook.popBuyMaxVec(); // Delete buy order from buy order book
                        temp = false;
                    } else if (stoi(BuyVec[5]) > stoi(order[5])) { // BuyQuantity > SellQuantity
                        recordOrder(order, 2, order[5], tradeBook.getBuyMaxVal()); // Record sell order in execution table 
                        recordOrder(BuyVec, 3, order[5], tradeBook.getBuyMaxVal()); // Record buy order in execution table
                        updateOrder(BuyVec, stoi(BuyVec[5]) - stoi(order[5])); // Update buy vector
                        tradeBook.updateBuyMaxVec(BuyVec); // Update buy order book
                        temp = false;
                    } else { // SellQuantity > BuyQuantity
                        recordOrder(order, 3, BuyVec[5], tradeBook.getBuyMaxVal());
                        recordOrder(BuyVec, 2, BuyVec[5], tradeBook.getBuyMaxVal());
                        updateOrder(order, stoi(order[5]) - stoi(BuyVec[5])); // Update selected sell order
                        tradeBook.popBuyMaxVec();
                    }
                } else {
                    tradeBook.writeSell(order);
                    recordOrder(order, 0, order[5], stod(order[4]));
                    temp = false;
                }
            } else {
                tradeBook.writeSell(order);
                recordOrder(order, 0, order[5], stod(order[4]));
                temp = false;
            }
        }
    }
}
