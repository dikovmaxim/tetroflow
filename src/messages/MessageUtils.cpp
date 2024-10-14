#include "MessageUtils.hpp"
#include <random>

#define node_id 777

//generate a 15 digit unique id
int generate_unique_id() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 9);

    int id = 0;
    for (int i = 0; i < 15; i++) {
        id = id * 10 + dis(gen);
    }

    id = id * 10 + node_id;

    return id;
}