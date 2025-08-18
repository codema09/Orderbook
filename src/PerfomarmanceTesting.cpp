#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <string>
#include <memory>

// Include all necessary headers
#include "include/OrderBook.hpp"
#include "include/Order.hpp"
#include "include/OrderType.hpp"
#include "include/OrderSide.hpp"
#include "include/Usings.hpp"
#include "include/constants.hpp"
#include "perf_utils/LatencyStats.hpp"

using namespace std;

// Helper function to get current time in nanoseconds (monotonic)
uint64_t get_time_nanoseconds() {
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
}

// LatencyStats helpers are now in perf_utils/LatencyStats.*

// Helper function to generate normal distribution with custom parameters
std::vector<double> generateNormalDistribution(std::mt19937& rng, double mid, double leftRange, double rightRange, int totalValues) {
    double min_val = mid - leftRange;
    double max_val = mid + rightRange;
    
    // Calculate standard deviation based on range (covers ~95% of values within range)
    double std_dev = (leftRange + rightRange) / 4.0;
    
    std::normal_distribution<double> dist(mid, std_dev);
    std::vector<double> values;
    values.reserve(totalValues);
    
    for (int i = 0; i < totalValues; ++i) {
        double value;
        do {
            value = dist(rng);
        } while (value < min_val || value > max_val); // Keep generating until in range
        
        values.push_back(value);
    }
    
    return values;
}

int main(){
// Random engine setup
std::mt19937 rng(std::random_device{}()); // Mersenne Twister
std::uniform_int_distribution<int> qty_dist(100, 1000);
std::uniform_int_distribution<int> side_dist(0, 1); // 0=buy, 1=sell

auto ob = OrderBook();

double start_sell_price = 125.0;
OrderId id = 0;
uint64_t populate_time_init = 0;
std::vector<uint64_t> init_sell_latencies;
init_sell_latencies.reserve(10000 * 100);
for (int level = 0; level < 10000; ++level) {
    double price = start_sell_price + (level/100.0); // e.g. 125, 125.01, ...
    for (int j = 0; j < 100; ++j) {
        id++;
        int quantity = qty_dist(rng);
        uint64_t start_t = get_time_nanoseconds();
        auto trades = ob.add_order(make_shared<Order>(OrderType::GoodTillCancel, OrderSide::Sell, id,price, quantity ));
        uint64_t end_t = get_time_nanoseconds();
        populate_time_init += end_t- start_t;
        init_sell_latencies.push_back(end_t - start_t);

    }
}

cout<<endl<<"Stats for initial SELL population:"<<endl;
auto init_sell_stats = computeLatencyStats(init_sell_latencies);
appendLatencyStatsToFile(init_sell_stats);

double start_buy_price = 123.0;

std::vector<uint64_t> init_buy_latencies;
init_buy_latencies.reserve(10000 * 100);
for (int level = 0; level < 10000; ++level) {
    double price = start_buy_price - (level/100.0); // e.g. 123, 122.99, ...
    for (int j = 0; j < 100; ++j) {
        id++;
        int quantity = qty_dist(rng);
        uint64_t start_t = get_time_nanoseconds();
        ob.add_order(make_shared<Order>(OrderType::GoodTillCancel, OrderSide::Buy, id,price, quantity ));
        uint64_t end_t = get_time_nanoseconds();
        populate_time_init += end_t- start_t;
        init_buy_latencies.push_back(end_t - start_t);
    }

    
}

cout<<endl<<"Stats for initial BUY population:"<<endl;
auto init_buy_stats = computeLatencyStats(init_buy_latencies);
appendLatencyStatsToFile(init_buy_stats);



{

const int NUM_LIMIT_ORDERS = 50000;
uint64_t total_limit_ns = 0;
std::vector<uint64_t> limit_latencies;
limit_latencies.reserve(NUM_LIMIT_ORDERS);

std::uniform_int_distribution<int> market_qty_dist(100, 2000);
// Generate prices using normal distribution around 124 with range [100, 150]
auto prices = generateNormalDistribution(rng, 124.0, 24.0, 26.0, NUM_LIMIT_ORDERS);

for (int i = 0; i < NUM_LIMIT_ORDERS; ++i) {
    // Random side (buy or sell)
    id++;
    OrderSide side = (side_dist(rng) == 0) ? OrderSide::Buy : OrderSide::Sell;
    int qty = market_qty_dist(rng);
    double price = prices[i]; // Use pre-generated price

    uint64_t start_t = get_time_nanoseconds();
    auto trades = ob.add_order(make_shared<Order>(OrderType::GoodTillCancel, side, id, price, qty));
    uint64_t end_t = get_time_nanoseconds();

    uint64_t duration = end_t - start_t;
    total_limit_ns += duration;
    limit_latencies.push_back(duration);
    // cout<<"trade "<<i<<": ";
    // trades.print_stats();
    // if(side == OrderSide::Sell)cout<<"Sell ";
    // else cout<<"Buy ";
    // cout<<price<<" "<<qty<<" "<<endl;;
    
}
double avg_limit_ns = static_cast<double>(total_limit_ns) / NUM_LIMIT_ORDERS;
    cout<<endl<<"Stats for "<<NUM_LIMIT_ORDERS<<" "<<"Limit Orders:"<<endl;

    auto limit_stats = computeLatencyStats(limit_latencies);
    appendLatencyStatsToFile(limit_stats);

}

{

    const int NUM_MARKET_ORDERS = 50000;
    uint64_t total_mkt_ns = 0;
    std::vector<uint64_t> market_latencies;
    market_latencies.reserve(NUM_MARKET_ORDERS);
    
    std::uniform_int_distribution<int> market_qty_dist(100, 2000);
    // Generate prices using normal distribution around 124 with range [100, 150]
    auto prices = generateNormalDistribution(rng, 124.0, 24.0, 26.0, NUM_MARKET_ORDERS);
    
    for (int i = 0; i < NUM_MARKET_ORDERS; ++i) {
        // Random side (buy or sell)
        id++;
        OrderSide side = (side_dist(rng) == 0) ? OrderSide::Buy : OrderSide::Sell;
        int qty = market_qty_dist(rng);
        double price = prices[i]; // Use pre-generated price
    
        uint64_t start_t = get_time_nanoseconds();
        auto trades = ob.add_order(make_shared<Order>(OrderType::Market, side, id, Constants::InvalidPrice, qty));
        uint64_t end_t = get_time_nanoseconds();
    
        uint64_t duration = end_t - start_t;
        total_mkt_ns += duration;
        market_latencies.push_back(duration);
        // cout<<"trade "<<i<<": ";
        // trades.print_stats();
        // if(side == OrderSide::Sell)cout<<"Sell ";
        // else cout<<"Buy ";
        // cout<<price<<" "<<qty<<" "<<endl;;
        
    }
    double avg_mkt_ns = static_cast<double>(total_mkt_ns) / NUM_MARKET_ORDERS;
        cout<<endl<<"Stats for "<<NUM_MARKET_ORDERS<<" "<<"Market Orders:"<<endl;
        auto market_stats = computeLatencyStats(market_latencies);
        appendLatencyStatsToFile(market_stats);
    }
    

}
