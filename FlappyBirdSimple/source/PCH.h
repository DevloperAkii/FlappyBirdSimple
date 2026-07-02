#pragma once
#include <print>

#define ASSERT(x, ...) if(x){ std::println("{} LINE - {} : {}", __FILE__, __LINE__, __VA_ARGS__); }