#include <iostream>
#include <mutex>
#include <thread>

std::mutex m;

void my_thread()
{

    int count = 10;

    m.lock();
    std::cout << "m.lock() in my_thread" << std::endl;
    for(int i = 0; i < count; i++)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "." << " ";
    }
    std::cout << std::endl;
    m.unlock();
    std::cout << "m.unlock() in my_thread" << std::endl;
};

int main()
{
    std::thread thrd(my_thread);
    thrd.detach();
    std::cout << "thrd.detach() in main" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    m.lock();
    std::cout << " m.lock() in main" << std::endl;
}