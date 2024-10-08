#include "Global.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <queue>

#include "log/Logger.hpp"
#include "table/Table.hpp"

#include "Global.hpp"

#include "datatypes/String.hpp"
#include "datatypes/Integer.hpp"
#include "datatypes/Float.hpp"
#include "datatypes/List.hpp"

#include "transactions/Transaction.hpp"


// Define the coreTable variable (shared pointer to a Table object, defined as extern in Global.hpp)
std::shared_ptr<Table> coreTable;

// Define the initStorage function
void initStorage() {
    // Create a new Table object
    coreTable = std::make_shared<Table>(100);
    // Log that the storage has been initialized
    log(LOG_INFO, "Storage initialized");
}