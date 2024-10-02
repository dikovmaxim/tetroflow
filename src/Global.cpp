#include "Global.hpp"
#include <stdio.h>

// Define the storage variable (initialized as nullptr)
Storage* storage = nullptr;

// Define the initStorage function
void initStorage() {
    storage = new Storage();
    printf("Storage initialized\n");
}
