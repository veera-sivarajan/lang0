# pragma once

# include <any>
# include <vector>

# include "./Expression.hpp"

class ListType {
public:
    std::vector<std::any> values;
    void append(std::any value);
    std::any getEleAt(int index);
};
