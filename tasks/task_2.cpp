#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <cassert>

std::mutex mutex;
std::mutex input_mutex;
std::mutex m_train;
std::mutex m_terminal;

struct Info
{
    std::string names[3] = {"A", "B", "C"};
    double speeds[3] = {20.1f, 30.3f, 45.3f};
};

class Train
{
private:
    std::string name = "";
    double speed = 0.0f;
public:

    void setSpeed(double inSpeed)
    {
        assert(inSpeed > 0);
        if(inSpeed > 50) this->speed = 50;
        else speed = inSpeed;
    }

    void setName(std::string inName)
    {
        this->name = inName;
    }

    void move(std::string inName, double inSpeed)
    {
        setName(inName);
        setSpeed(inSpeed);
    }

    void getInfo()
    {
        std::cout << "Train " << this->name << " Speed " << this->speed << std::endl;
    }

    double getSpeed()
    {
        return this->speed;
    }

    std::string getName()
    {
        return this->name;
    }

};

class Terminal
{
private:
    bool hasTrain = false;
    Train* trainAtDepo = nullptr;
public:
    void getTrain(Train* inTrain)
    {
        trainAtDepo = inTrain;
    }

    void sendTrain()
    {
        trainAtDepo = nullptr;
    }

    Train* getTrainPtr()
    {
        return  trainAtDepo;
    }

};

void moving_to_depo(Train* inTrain, double inDistance, Terminal* inTerminal)
{

    bool flag = false;

    while(inDistance >= 0)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        if(inDistance > 0)
        {
            mutex.lock();
            std::cout << "TRAIN " << inTrain->getName() << " Distance " << inDistance << std::endl;
            mutex.unlock();
        }

        inDistance -= inTrain->getSpeed();
        if(inDistance <= 0)
        {

            inDistance = 0;
            if(inTerminal->getTrainPtr() == nullptr)
            {
                m_terminal.lock(); // access to Terminal
                inTerminal->getTrain(inTrain);

                std::cout << "Train " << inTrain->getName() << " at DEPO" << std::endl;
                std::cout << "Type 'd' to send Train from Depo: " << std::endl;
                std::string command = "d";
                std::string tmp;
                std::cin >> tmp;
                if(tmp == command)
                {
                    inTerminal->sendTrain();
                    std::cout << "Train " << inTrain->getName() << " left the Depo" << std::endl;
                    m_terminal.unlock(); // close access to Terminal after command
                    return;
                }

            }

            if(!flag)
            {
                if(inTerminal->getTrainPtr() != nullptr)
                {
                    mutex.lock();
                    std::cout << "Train " << inTrain->getName() << " is waiting to get to Depo" << std::endl;
                    mutex.unlock();
                    flag = true;
                }

            }
        }
    }
}

int main()
{
    double distance = 150.0f;
    Info info;
    Terminal terminal;
    Train* trains = new Train[3];

    for(int i = 0; i < 3; i++)
    {
        trains[i].move(info.names[i], info.speeds[i]);
    }

    std::thread* threads = new std::thread[3];
    for(int i = 0; i < 3; i++) {
        threads[i] = std::thread (moving_to_depo, (trains+i), distance, &terminal);
        m_train.lock();
        std::cout << "ID: " << (threads + i)->get_id() << std::endl;
        m_train.unlock();
    }
    std::cout << "THREADS CREATED. WAITING TO FINISH" << std::endl;

    for(int i = 0; i < 3; i++)
    {
        (threads+i)->join();
    }
    std::cout << "THREADS JOINED" << std::endl  << std::endl;



    delete[] trains;
    return 0;
}

