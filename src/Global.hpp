#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#define TABLE_INITIAL_SIZE 30

#include "storage/Storage.hpp"

// Declare storage as extern without initializing
extern Storage* storage;

// Declare the initStorage function
void initStorage();

#endif // GLOBAL_HPP