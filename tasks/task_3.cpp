#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <cassert>
#include <ctime>

std::mutex print;
std::mutex access_list_of_orders;
std::mutex access_to_ready_orders;

void getOrder(std::vector<std::string>* inList, int *inTotalDeliveries)
{
    while(true)
    {
        if(*inTotalDeliveries >= 9) break;
        std::srand(time(nullptr));
        int rand_dish = (1 + (rand()) % 5);

        std::string new_order;
        if(rand_dish == 1) new_order = "PIZZA";
        else if(rand_dish == 2) new_order = "SOUP";
        else if(rand_dish == 3) new_order = "STEAK";
        else if(rand_dish == 4) new_order = "SALAD";
        else if(rand_dish == 5) new_order = "SYSHI";

        access_list_of_orders.lock();
        inList->push_back(new_order);
        access_list_of_orders.unlock();

        print.lock();
        std::cout << "New Order: " << new_order << std::endl;
        print.unlock();

        std::srand(time(nullptr));
        int rand_delay = (5 + (rand()) % 6);
        std::this_thread::sleep_for(std::chrono::seconds(rand_delay));
    }
}

void kitchen(std::vector<std::string>* inOrders, std::vector<std::string>* inToDeliver, int *inTotalDeliveries)
{
    while(true)
    {
        if(*inTotalDeliveries >= 9) break;
        print.lock();
        std::cout << "Current list of orders: " << inOrders->size() << std::endl;
        for(int i = 0; i < inOrders->size(); i++)
        {
            std::cout << i+1 << ") " << inOrders->at(i) << std::endl;
        }
        print.unlock();

        std::srand(time(nullptr));
        int rand_cooking = (5 + (rand()) % 11);
        std::this_thread::sleep_for(std::chrono::seconds(rand_cooking));
        access_list_of_orders.lock();

        print.lock();
        std::cout << "Order " << *inOrders->begin() << " ready" << std::endl;
        print.unlock();

        access_to_ready_orders.lock();
        inToDeliver->push_back(*inOrders->begin());
        access_to_ready_orders.unlock();

        inOrders->erase(inOrders->begin() + 0);
        access_list_of_orders.unlock();
    }
}

void deliveryBoy(std::vector<std::string> *inToDeliver, int *inTotalDeliveries)
{
    while (true)
    {
        if(*inTotalDeliveries >= 10) break;
        std::this_thread::sleep_for(std::chrono::seconds(10));
        if(inToDeliver->size() == 0) continue;
        access_to_ready_orders.lock();
        print.lock();
        std::cout << "List to deliver:" << std::endl;
        for(int i = 0; i < inToDeliver->size(); i++)
        {
            std::cout << i+1 << ") " << inToDeliver->at(i) << std::endl;
        }
        print.unlock();
        inToDeliver->clear();
        access_to_ready_orders.unlock();
        (*inTotalDeliveries)++;
        print.lock();
        std::cout << "Total deliveries " << *inTotalDeliveries << std::endl;
        print.unlock();
    }
}

int main()

{
    std::vector<std::string> list_of_orders;
    std::vector<std::string> list_to_deliver;
    int total_delivers = 0;

    std::thread getting_orders(getOrder, &list_of_orders, &total_delivers);
    std::thread cooking(kitchen, &list_of_orders, &list_to_deliver, &total_delivers);
    std::thread delivering(deliveryBoy, &list_to_deliver, &total_delivers);

    getting_orders.detach();
    cooking.join();
    delivering.join();

    if(total_delivers == 10)
    {
        std::cout << std::endl << "Restaurant Closed" << std::endl;
        return 0;
    }

}

