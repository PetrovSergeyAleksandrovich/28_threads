#include <iostream>
#include <thread>
#include <mutex>

std::mutex m;

struct Swim
{
    std::string swimmers[6] = {"John", "Bob", "Mark", "Carl", "Jack", "James"};
    double speed[6] = {1.2f, 0.8f, 1.0f, 0.7f, 0.95f, 1.1f};
};

void swimming_in_process(std::string name, double speed, double distance)
{
    int total_second_to_win = 0;
    std::cout << "ID: " << std::this_thread::get_id() << std::endl;
    while(distance > 0)
    {
        total_second_to_win++;
        distance -= speed;
        if(distance < 0) distance = 0;
        m.lock();
        std::cout << name << " left " << distance << "m," << " passed " << total_second_to_win << " sec" << std::endl;
        m.unlock();

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    m.lock();
    std::cout << name << " FINISHED at " << total_second_to_win << std::endl;
    m.unlock();
}

int main()
{
    double distance = 100.0f;
    Swim swimmers_group;
    int threads_size = 6;
    std::thread* threads = new std::thread[threads_size];

    for(int i = 0; i < threads_size; i++) {
        threads[i] = std::thread(swimming_in_process, swimmers_group.swimmers[i], swimmers_group.speed[i], distance);
        m.lock();
        std::cout << (threads + i)->get_id() << std::endl;
        m.unlock();
    }
    std::cout << "THREADS CREATED. WAITING TO FINISH" << std::endl;

    for(int i = 0; i < 6; i++)
    {
        (threads+i)->join();
    }
    std::cout << "THREADS JOINED. ALL FINISHED" << std::endl;

    delete[] threads;

    return 0;
}

