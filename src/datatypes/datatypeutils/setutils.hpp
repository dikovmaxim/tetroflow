#pragma once

#include "../Set.hpp"
#include "../Datatype.hpp"

#include <unordered_set>
#include <memory>
#include <vector>


inline std::shared_ptr<Set> SetIntersection(const std::shared_ptr<Set>& set1, const std::shared_ptr<Set>& set2) {
    std::unordered_set<std::shared_ptr<DataType>> set1_values = set1->get_value();
    std::unordered_set<std::shared_ptr<DataType>> set2_values = set2->get_value();
    std::unordered_set<std::shared_ptr<DataType>> intersection;
    for (auto it = set1_values.begin(); it != set1_values.end(); it++) {
        if (set2_values.find(*it) != set2_values.end()) {
            intersection.insert(*it);
        }
    }
    return std::make_shared<Set>(intersection);
}

inline std::shared_ptr<Set> SetUnion(const std::shared_ptr<Set>& set1, const std::shared_ptr<Set>& set2) {
    std::unordered_set<std::shared_ptr<DataType>> set1_values = set1->get_value();
    std::unordered_set<std::shared_ptr<DataType>> set2_values = set2->get_value();
    std::unordered_set<std::shared_ptr<DataType>> union_set = set1_values;
    for (auto it = set2_values.begin(); it != set2_values.end(); it++) {
        union_set.insert(*it);
    }
    return std::make_shared<Set>(union_set);
}

inline std::shared_ptr<Set> SetDifference(const std::shared_ptr<Set>& set1, const std::shared_ptr<Set>& set2) {
    std::unordered_set<std::shared_ptr<DataType>> set1_values = set1->get_value();
    std::unordered_set<std::shared_ptr<DataType>> set2_values = set2->get_value();
    std::unordered_set<std::shared_ptr<DataType>> difference;
    for (auto it = set1_values.begin(); it != set1_values.end(); it++) {
        if (set2_values.find(*it) == set2_values.end()) {
            difference.insert(*it);
        }
    }
    return std::make_shared<Set>(difference);
}

