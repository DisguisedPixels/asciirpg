#pragma once

#include <import.hpp>

bool in_array(const std::string &value, const std::vector<std::string> &array)
{
    return std::find(array.begin(), array.end(), value) != array.end();
}

int json2int(Json::Value &val)
{
    std::stringstream ss;
    ss << val;

    std::string tmp = ss.str();

    int num = atoi(tmp.c_str());

    return(num);
}

std::string json2str(Json::Value &val)
{
    std::stringstream ss;
    ss << val;

    std::string tmp = ss.str();
    tmp.erase(0, 1);
    tmp.erase(tmp.size() - 1);

    return(tmp);
}