#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <cassert>

std::mutex mutex;
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

    void getTrainAtTerminal()
    {
        if(trainAtDepo != nullptr)
        {
            std::cout << "check_void getTrainAtTerminal()" << std::endl;
            std::cout << trainAtDepo->getName() << std::endl;
        }
    }
};

void moving_to_depo(Train* inTrain, double inDistance, Terminal* inTerminal)
{
    while(inDistance >= 0)
    {
        inDistance -= inTrain->getSpeed();
        if(inDistance <= 0)
        {
            inDistance = 0;
            if(inTerminal->getTrainPtr() == nullptr)
            {
                m_terminal.lock();
                inTerminal->getTrain(inTrain);
                std::string command = "d";
                while(true)
                {
                    std::string tmp;
                    std::cout << "Train " << inTrain->getName() << " at DEPO" << std::endl;
                    std::cout << "Type 'd' to send it: ";
                    std::cin >> tmp;

                    if(tmp == command)
                    {
                        inTerminal->sendTrain();
                        m_terminal.unlock();
                        return;
                    }
                }
            }
            else if(inTerminal->getTrainPtr() != nullptr)
            {
                mutex.lock();
                std::cout << "Train " << inTrain->getName() << " is waiting to get to Depo" << std::endl;
                mutex.unlock();
            }
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
        if(inDistance > 0)
        {
            mutex.lock();
            std::cout << "TRAIN " << inTrain->getName() << " Distance " << inDistance << std::endl;
            mutex.unlock();
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

    std::cout << "check_1" << std::endl;

    delete[] trains;
    return 0;
}

