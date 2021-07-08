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

    std::thread sw0(swimming_in_process, swimmers_group.swimmers[0], swimmers_group.speed[0], distance);
    std::thread sw1(swimming_in_process, swimmers_group.swimmers[1], swimmers_group.speed[1], distance);
    std::thread sw2(swimming_in_process, swimmers_group.swimmers[2], swimmers_group.speed[2], distance);
    std::thread sw3(swimming_in_process, swimmers_group.swimmers[3], swimmers_group.speed[3], distance);
    std::thread sw4(swimming_in_process, swimmers_group.swimmers[4], swimmers_group.speed[4], distance);
    std::thread sw5(swimming_in_process, swimmers_group.swimmers[5], swimmers_group.speed[5], distance);

    std::cout << "THREADS CREATED. WAITING TO FINISH" << std::endl;

    sw0.join();
    sw1.join();
    sw2.join();
    sw3.join();
    sw4.join();
    sw5.join();

    std::cout << "THREADS JOINED. ALL FINISHED" << std::endl;

    return 0;
}

