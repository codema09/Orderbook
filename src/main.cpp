#include"include/OrderBook.hpp"

int main(){

    OrderBook ob;
    OrderId id_ = 1;

    auto trades = ob.add_order(std::make_shared<Order>(OrderType::GoodTillCancel, OrderSide::Sell, id_, Price{100}, Quantity{3000}));
    // trades.print_all_trades();
    auto levelsinfo = ob.get_order_book();
    levelsinfo.display_all_levels();

    trades = ob.add_order(std::make_shared<Order>(OrderType::GoodTillCancel, OrderSide::Buy, OrderId{2}, Price{90}, Quantity{200}));
    levelsinfo = ob.get_order_book();
    levelsinfo.display_all_levels();

    trades = ob.add_order(std::make_shared<Order>(OrderType::GoodTillCancel, OrderSide::Buy, OrderId{3}, Price{95}, Quantity{1000}));
    levelsinfo = ob.get_order_book();
    levelsinfo.display_all_levels();

    trades = ob.add_order(std::make_shared<Order>(OrderType::GoodTillCancel, OrderSide::Buy, OrderId{4}, Price{105}, Quantity{2500}));
    levelsinfo = ob.get_order_book();
    levelsinfo.display_all_levels();
    trades.print_all_trades();

    trades = ob.add_order(std::make_shared<Order>(OrderType::GoodTillCancel, OrderSide::Buy, OrderId{5}, Price{104}, Quantity{700}));
    levelsinfo = ob.get_order_book();
    levelsinfo.display_all_levels();
    trades.print_all_trades();

    trades = ob.add_order(std::make_shared<Order>(OrderType::GoodForDay, OrderSide::Buy, OrderId{6}, Price{72}, Quantity{700}));
    levelsinfo = ob.get_order_book();
    levelsinfo.display_all_levels();
    trades.print_all_trades();

    trades = ob.add_order(std::make_shared<Order>(OrderType::FillOrKill, OrderSide::Sell, OrderId{7}, Price{10}, Quantity{4000}));
    levelsinfo = ob.get_order_book();
    levelsinfo.display_all_levels();
    trades.print_all_trades();

    trades = ob.add_order(std::make_shared<Order>(OrderType::FillOrKill, OrderSide::Sell, OrderId{8}, Price{0}, Quantity{1500}));
    levelsinfo = ob.get_order_book();
    levelsinfo.display_all_levels();
    trades.print_all_trades();
}

