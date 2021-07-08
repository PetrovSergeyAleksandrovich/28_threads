#include <iostream>
#include <vector>

int main()
{
    int x_size = 32;
    int y_size = 16;
    std::vector<char> screen = {};

    for(int i = 0; i < x_size; i++)
    {
        screen.push_back('.');
    }

    for(int i = 0; i < y_size; i++)
    {
        for(int j = 0; j < screen.size(); j++)
        {
            std::cout << screen[j] << " ";
        }
        std::cout << std::endl;
    }

}


