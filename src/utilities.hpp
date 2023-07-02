#pragma once
#include "string"
#include "fstream"
#include "iostream"

class Utilities
{
public:
    static std::string readFile(const char *filePath);
};