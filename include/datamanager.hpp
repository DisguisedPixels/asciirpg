#pragma once

#include <import.hpp>

struct LocationStruct
{

};

struct PlayerStruct
{
    unsigned int level;
};

struct SaveGameStruct
{
    std::string id;
    PlayerStruct player;
};

class DataManager
{
    private:
    public:
        SaveGameStruct game;
        Json::Value location;
        Json::Value item;
        Json::Value loot;
        
        int read_json(const std::string& PATH)
        {
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
            return 0;
        }

        int save_write(const std::string& savename)
        {
            return 0;
        }

        int save_read(const std::string& savename)
        {
            return 0;
        }

        void save_init(const std::string& savename)
        {
            game.id = savename;
            game.player.level = 0;
        }
};