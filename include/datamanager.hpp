#pragma once

#include <import.hpp>

struct LocationStruct
{

};

struct PlayerStruct
{
    std::string name;
    unsigned int level;
};

struct SaveGameStruct
{
    std::string id;
    std::vector <PlayerStruct> characters {};
    unsigned int current_char;
    std::string location;
};

class DataManager
{
    private:
    public:
        SaveGameStruct game;

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
            game.characters.push_back((struct PlayerStruct){"Guy 1",1});
            game.characters.push_back((struct PlayerStruct){"Guy 2",2});
            game.current_char = 1;
            game.location = "village_entrance";
        }
};