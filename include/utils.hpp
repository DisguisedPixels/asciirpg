#pragma once

#include <import.hpp>

class JsonData
{
    public:
        Json::Value location;
        Json::Value item;
        Json::Value entity;

        int read_json(const std::string& PATH)
        {
            // LOCATION
            std::ifstream json1(PATH + "/location.json");
            if (json1.fail())
            {
                return 1;
            }
            try
            {
                json1 >> location;
            }
            catch(const Json::RuntimeError& e)
            {
                return 2;
            }

            // ITEM
            std::ifstream json2(PATH + "/item.json");
            if (json2.fail())
            {
                return 1;
            }
            try
            {
                json2 >> item;
            }
            catch(const Json::RuntimeError& e)
            {
                return 2;
            }

            // ITEM
            std::ifstream json3(PATH + "/entity.json");
            if (json3.fail())
            {
                return 1;
            }
            try
            {
                json3 >> entity;
            }
            catch(const Json::RuntimeError& e)
            {
                return 2;
            }

            return 0;
        }
};

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