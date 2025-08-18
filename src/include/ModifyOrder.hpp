#pragma once
#include "Usings.hpp"
#include "OrderType.hpp"
#include "OrderSide.hpp"
#include "Order.hpp"

class OrderModify{
    public:
        OrderModify(OrderType type, OrderSide side, OrderId id, Price price, Quantity quantity):
        type_(type),side_(side),id_(id), price_(price), quantity_order_(quantity)
        {}


        OrderSide get_order_side(){
            return side_;
        }

        OrderType get_order_type(){
            return type_;
        }

        Price get_price(){
             return price_;
         }
         
        OrderId get_order_id(){
             return id_;
         }

        OrderPointer to_order_ptr() {
            return std::make_shared<Order>(type_, side_, id_, price_, quantity_order_);
        }

    private:
    OrderType type_;
    OrderSide side_;
    OrderId id_;
    Price price_;
    Quantity quantity_order_;
};