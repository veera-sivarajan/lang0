# include "./ListType.hpp"

void ListType::append(std::any value) {
    values.push_back(value);
}

std::any ListType::getEleAt(int index) {
    return values.at(index);
}

int ListType::length() {
    return values.size();
}
