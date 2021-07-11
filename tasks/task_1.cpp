#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

std::mutex m;

struct Swim
{
    std::string swimmers[6] = {"John", "Bob", "Mark", "Carl", "Jack", "James"};
    double speed[6] = {1.18f, 0.83f, 0.98f, 0.78f, 0.92f, 1.13f};
};

void swimming_in_process(std::string name, double speed, double distance, std::vector<std::string>* winners)
{
    int total_second_to_win = 0;
    std::cout << "ID: " << std::this_thread::get_id() << std::endl;
    while(distance > 0)
    {
        total_second_to_win++;
        distance -= speed;
        if(distance < 0) distance = 0;
        //m.lock();
        std::cout << name << " left " << distance << "m," << " passed " << total_second_to_win << " sec" << std::endl;
        //m.unlock();

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    m.lock();
    winners->push_back(name);
    m.unlock();
    //m.lock();
    std::cout << name << " FINISHED at " << total_second_to_win << std::endl;
    //m.unlock();
}

int main()
{
    double distance = 10.0f;
    Swim swimmers_group;
    std::vector<std::string> winners;

    constexpr int threads_size = 6;
    std::thread* threads = new std::thread[threads_size];
    for(int i = 0; i < threads_size; i++) {
        threads[i] = std::thread(swimming_in_process, swimmers_group.swimmers[i], swimmers_group.speed[i], distance, &winners);
        //m.lock();
        std::cout << (threads + i)->get_id() << std::endl;
        //m.unlock();
    }
    std::cout << "THREADS CREATED. WAITING TO FINISH" << std::endl;

    for(int i = 0; i < 6; i++)
    {
        (threads+i)->join();
    }
    std::cout << "THREADS JOINED. ALL FINISHED" << std::endl << std::endl;

    std::cout << "Winners list:\n";
    for(int i = 0; i < winners.size(); i++)
    {
        std::cout << i+1 << ") " << winners[i] << std::endl;
    }

    delete[] threads;
    return 0;
}

