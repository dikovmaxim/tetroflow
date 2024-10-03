#include "Global.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <string>

#include "log/Logger.hpp"

// Define the storage variable (initialized as nullptr)
Storage* storage = nullptr;

// Define the initStorage function
void initStorage() {
    storage = new Storage();
    log(LOG_INFO, "Storage initialized");
}
