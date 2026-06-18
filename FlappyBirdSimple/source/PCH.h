#pragma once
#include <print>

#define ASSERT(x, ...) if(x){ std::print("{} LINE - {} : {}", __FILE__, __VA_ARGS__, __LINE__); }