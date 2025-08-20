#pragma once
#include "Usings.hpp"
#include "OrderType.hpp"
#include "OrderSide.hpp"
#include<memory>
#include "CustomDLL.hpp"
#include <stdexcept>

class Order{

    public:
        Order(OrderType type_ip, OrderSide side, OrderId id, Price price, Quantity quantity):type_(type_ip),
        side_(side),id_(id), price_(price), quantity_order_(quantity), quantity_order_left_(quantity)
        {}

        bool is_filled(){
            return quantity_order_left_ == 0;
        }

        OrderType  get_order_type(){
            return type_;
        }

        OrderSide get_order_side(){
            return side_;
        }

        Quantity get_quantity(){
            return quantity_order_left_;
        }

        Price get_price(){
             return price_;
         }
         
         OrderId get_order_id(){
             return id_;
         }
         
         void fill_order(Quantity quantity){
             quantity_order_left_ -= quantity;
         }

         bool order_filled_partial_or_full(){
            return quantity_order_left_ < quantity_order_;
         }
         void market_normalize(){
            if(get_order_type() != OrderType::Market){
                throw std::runtime_error("Order type is not Market in market_to_gtc()");
            }
            // type_ = OrderType::GoodTillCancel;
            price_ = (get_order_side()==OrderSide::Buy)? 1e18:0;

         }

    private:

    OrderType type_;
    OrderSide side_;
    OrderId id_;
    Price price_;
    Quantity quantity_order_;
    Quantity quantity_order_left_;

};


using OrderPointer = std::shared_ptr<Order> ;
using OrderPointers = CustomLinkedList<OrderPointer> ;