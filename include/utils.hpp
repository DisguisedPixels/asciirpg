#pragma once

#include <import.hpp>

bool in_array(const std::string &value, const std::vector<std::string> &array)
{
    //return std::find(array.begin(), array.end(), value) != array.end();
    int numberofelements = sizeof(array)/sizeof(array[0]);

    bool found = false;
    for (int i = 0; i < numberofelements; ++i)
    {
        if (array[i] == value)
        {
            found = true;
        }
    }
    return found;

}