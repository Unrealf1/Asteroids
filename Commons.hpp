#pragma once

#include <memory>

struct position_t {
    float x;
    float y;
};

template<typename OBJ_T>
using obj_container = std::vector<std::shared_ptr<OBJ_T>>;
