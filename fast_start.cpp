#include <iostream>
#include <thread>

void wait_for_call(int time, std::string name)
{
    std::this_thread::sleep_for(std::chrono::seconds(time));
    std::cout << "ID: " << std::this_thread::get_id() << std::endl;
    std::cout << "RING " <<  name << std::endl;
}

int main()
{

    int time = 3;
    //std::cin >> time;
    std::string name ="XYZ";
    //std::cin >> name;

    std::cout << "ID: " << std::this_thread::get_id() << std::endl;
    std::thread call(wait_for_call, time, name);
    std::thread call2(wait_for_call, 7, "SKILLBOX");
    call.detach();
    call2.detach();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "Ring friend" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(10));

    if(call.joinable()) call.join();
    if(call2.joinable()) call2.join();

    return 0;
}
