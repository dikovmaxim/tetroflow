
#pragma once

#include <memory>
#include "../Datatype.hpp"

class Iterable {
public:
    virtual std::shared_ptr<DataType> get(int index) = 0;
    virtual void set(int index, std::shared_ptr<DataType> value) = 0;
    virtual int size() = 0;
};
 