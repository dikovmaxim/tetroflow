#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <stdexcept>

#include "../Global.hpp"
#include "../datatypes/Datatype.hpp"
#include "../datatypes/Error.hpp"
#include "../datatypes/Integer.hpp"
#include "../datatypes/List.hpp"
#include "../datatypes/String.hpp"
#include "../datatypes/Boolean.hpp"
#include "../datatypes/Float.hpp"
#include "../datatypes/Set.hpp"
#include "../datatypes/Sortedset.hpp"

inline std::shared_ptr<DataType> ListToSet(std::shared_ptr<List> list) {
    std::shared_ptr<Set> set = std::make_shared<Set>();
    for (auto& item : list->get_value()) {
        set->sadd(item);
    }
    return set;
}

inline std::shared_ptr<DataType> SetToList(std::shared_ptr<Set> set) {
    std::shared_ptr<List> list = std::make_shared<List>();
    for (auto& item : set->get_value()) {
        list->rpush(item);
    }
    return list;
}

inline std::shared_ptr<DataType> IntegerToFloat(std::shared_ptr<Integer> integer) {
    return std::make_shared<Float>(integer->get_value());
}

inline std::shared_ptr<DataType> FloatToInteger(std::shared_ptr<Float> f) {
    return std::make_shared<Integer>(f->get_value());
}

inline std::shared_ptr<DataType> StringToFloat(std::shared_ptr<String> s) {
    try {
        return std::make_shared<Float>(std::stof(s->get_value()));
    } catch (std::invalid_argument&) {
        return make_error("ERR value is not a valid float");
    }
}

inline std::shared_ptr<DataType> StringToInteger(std::shared_ptr<String> s) {
    try {
        return std::make_shared<Integer>(std::stoi(s->get_value()));
    } catch (std::invalid_argument&) {
        return make_error("ERR value is not a valid integer");
    }
}

inline std::shared_ptr<DataType> FloatToString(std::shared_ptr<Float> f) {
    return std::make_shared<String>(std::to_string(f->get_value()));
}

inline std::shared_ptr<DataType> IntegerToString(std::shared_ptr<Integer> i) {
    return std::make_shared<String>(std::to_string(i->get_value()));
}

inline std::shared_ptr<DataType> StringToBoolean(std::shared_ptr<String> s) {
    if (s->get_value() == "true") {
        return createBoolean(true);
    } else if (s->get_value() == "false") {
        return createBoolean(false);
    } else {
        return make_error("ERR value is not a valid boolean");
    }
}

inline std::shared_ptr<DataType> BooleanToString(std::shared_ptr<Boolean> b) {
    return std::make_shared<String>(b->get_value() ? "true" : "false");
}

inline std::shared_ptr<DataType> FloatToBoolean(std::shared_ptr<Float> f) {
    return createBoolean(f->get_value() != 0);
}

inline std::shared_ptr<DataType> IntegerToBoolean(std::shared_ptr<Integer> i) {
    return createBoolean(i->get_value() != 0);
}

inline std::shared_ptr<DataType> BooleanToFloat(std::shared_ptr<Boolean> b) {
    return std::make_shared<Float>(b->get_value() ? 1 : 0);
}

inline std::shared_ptr<DataType> BooleanToInteger(std::shared_ptr<Boolean> b) {
    return std::make_shared<Integer>(b->get_value() ? 1 : 0);
}

inline std::shared_ptr<DataType> StringToList(std::shared_ptr<String> s) {
    std::shared_ptr<List> list = std::make_shared<List>();
    for (char c : s->get_value()) {
        list->rpush(std::make_shared<String>(std::string(1, c)));
    }
    return list;
}

inline std::shared_ptr<DataType> ListToString(std::shared_ptr<List> list) {
    std::string s;
    for (auto& item : list->get_value()) {
        s += item->to_string();
    }
    return std::make_shared<String>(s);
}

inline std::shared_ptr<DataType> ListToSortedset(std::shared_ptr<List> list) {
    std::shared_ptr<Sortedset> sortedset = std::make_shared<Sortedset>();
    for (auto& item : list->get_value()) {
        sortedset->zadd(item, 0);
    }
    return sortedset;
}

inline std::shared_ptr<DataType> SetToSortedset(std::shared_ptr<Set> set) {
    std::shared_ptr<Sortedset> sortedset = std::make_shared<Sortedset>();
    for (auto& item : set->get_value()) {
        sortedset->zadd(item, 0);
    }
    return sortedset;
}

inline std::shared_ptr<DataType> SortedsetToList(std::shared_ptr<Sortedset> sortedset) {
    std::shared_ptr<List> list = std::make_shared<List>();
    for (auto& item : sortedset->get_value()) {
        list->rpush(item);
    }
    return list;
}



inline std::shared_ptr<DataType> Cast(std::shared_ptr<DataType> value, DataTypeType type) {
    switch (type) {
        case DataTypeType::SET: {
            if (value->get_type() == DataTypeType::LIST) {
                return ListToSet(std::static_pointer_cast<List>(value));
            }
            break;
        }
        case DataTypeType::LIST: {
            if (value->get_type() == DataTypeType::SET) {
                return SetToList(std::static_pointer_cast<Set>(value));
            }
            if (value->get_type() == DataTypeType::STRING) {
                return StringToList(std::static_pointer_cast<String>(value));
            }
            if (value->get_type() == DataTypeType::SORTEDSET) {
                return SortedsetToList(std::static_pointer_cast<Sortedset>(value));
            }
            break;
        }
        case DataTypeType::FLOAT: {
            if (value->get_type() == DataTypeType::INTEGER) {
                return IntegerToFloat(std::static_pointer_cast<Integer>(value));
            }
            if (value->get_type() == DataTypeType::STRING) {
                return StringToFloat(std::static_pointer_cast<String>(value));
            }
            if (value->get_type() == DataTypeType::BOOLEAN) {
                return BooleanToFloat(std::static_pointer_cast<Boolean>(value));
            }
            break;
        }
        case DataTypeType::INTEGER: {
            if (value->get_type() == DataTypeType::FLOAT) {
                return FloatToInteger(std::static_pointer_cast<Float>(value));
            }
            if (value->get_type() == DataTypeType::STRING) {
                return StringToInteger(std::static_pointer_cast<String>(value));
            }
            if (value->get_type() == DataTypeType::BOOLEAN) {
                return BooleanToInteger(std::static_pointer_cast<Boolean>(value));
            }
            break;
        }
        case DataTypeType::STRING: {
            if (value->get_type() == DataTypeType::FLOAT) {
                return FloatToString(std::static_pointer_cast<Float>(value));
            }
            if (value->get_type() == DataTypeType::INTEGER) {
                return IntegerToString(std::static_pointer_cast<Integer>(value));
            }
            if (value->get_type() == DataTypeType::BOOLEAN) {
                return BooleanToString(std::static_pointer_cast<Boolean>(value));
            }
            if (value->get_type() == DataTypeType::LIST) {
                return ListToString(std::static_pointer_cast<List>(value));
            }
            break;
        }
        case DataTypeType::BOOLEAN: {
            if (value->get_type() == DataTypeType::FLOAT) {
                return FloatToBoolean(std::static_pointer_cast<Float>(value));
            }
            if (value->get_type() == DataTypeType::INTEGER) {
                return IntegerToBoolean(std::static_pointer_cast<Integer>(value));
            }
            if (value->get_type() == DataTypeType::STRING) {
                return StringToBoolean(std::static_pointer_cast<String>(value));
            }
            break;
        }

        case DataTypeType::SORTEDSET: {
            if (value->get_type() == DataTypeType::LIST) {
                return ListToSortedset(std::static_pointer_cast<List>(value));
            }
            if (value->get_type() == DataTypeType::SET) {
                return SetToSortedset(std::static_pointer_cast<Set>(value));
            }
            break;
        }

        default:
            break;
    }
    throw std::invalid_argument("Invalid cast");
}