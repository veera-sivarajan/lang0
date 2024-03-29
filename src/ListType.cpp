# include "../include/ListType.hpp"

void ListType::append(std::any value) {
    values.push_back(value);
}

std::any ListType::getEleAt(int index) {
    return values.at(index);
}

int ListType::length() {
    return values.size();
}

bool ListType::setAtIndex(int index, std::any value) {
     // allows inserting when length == index to append values to the list
    if (index == length()) {
        values.insert(values.begin() + index, value);
    } else if (index < length() && index >= 0) {
        values[index] = value;
    } else {
        return false;
    }
    return true;
}
