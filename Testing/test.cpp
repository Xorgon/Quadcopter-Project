//
// Created by Elijah on 14/02/2017.
//

#include <iostream>
#include <cstring>

int main() {
    std::cout << 15501 % 60000 << std::endl;
    int center = 5;
    for (int i = 0; i < 5; i++) {
        int led1 = center + i;
        int led2 = center - i;
    }
    return 0;
}
