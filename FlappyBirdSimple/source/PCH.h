#pragma once
#include <print>

#define ASSERT(x, ...) if(x){ std::println("{} LINE - {} : {}", __FILE__, __LINE__, __VA_ARGS__); }

int glCheckError_(const char* file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__) 