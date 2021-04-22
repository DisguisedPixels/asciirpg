#pragma once

#include <import.hpp>
#include <utils.hpp>

struct ItemStruct
{
    std::string id;
    unsigned int amount;
};

struct LocationStruct
{
    std::vector <std::string> creatures {};
};

struct PlayerStatsStruct
{
    int health;
    unsigned int health_max;
    int mana;
    unsigned int mana_max;

    int strength;
    int true_attack;
    int crit_chance;
    int crit_damage;
    int piercing;
    int defence;
    int true_defence;
    int aggro; // AKA creatures will attack more (or fierce)
};

struct PlayerStruct
{
    std::string name;
    unsigned int level;
    std::vector <ItemStruct> inventory {};
    ItemStruct hotbar[6];
    std::map <std::string, ItemStruct> equipment {};
    unsigned int inventory_max;
    PlayerStatsStruct stats;
};

struct EnemyStruct
{
    std::string id;
    int health;
    int strength;
};

struct SaveGameStruct
{
    std::string id;
    std::vector <PlayerStruct> characters {};
    std::vector <EnemyStruct> enemies {};
    std::map <std::string, LocationStruct> locations {};
    unsigned int current_char;
    std::string location;
    std::string location_old;
    int temp_data[10];
};

class DataManager
{
    public:
        int character_max = 5;

        JsonData json;

        SaveGameStruct game;

        DataManager()
        {
            std::string PATH = std::filesystem::current_path();
            if(!json.read_json(PATH + "/bin/data/json") == 0)
            {
                endwin();
            }
            save_init("Test");
        }

        int save_write(const std::string& savename)
        {
            return 0;
        }

        int save_read(const std::string& savename)
        {
            return 0;
        }

        bool create_character(const std::string &name)
        {
            if(game.characters.size() < character_max)
            {
                game.characters.push_back((struct PlayerStruct){name,1});
                return true;
            }
            else
            {
                return false;
            }
        }

        bool summon_creature(std::string &location, const std::string &id)
        {
            game.locations[location].creatures.push_back({id});
            return true;
        }

        bool combat_enemy(const std::string &id)
        {
            int health = json2int(json.entity[id]["health"]);
            int strength = json2int(json.entity[id]["strength"]);
            game.enemies.push_back({id,health,strength});
            return true;
        }

        int enemy_check()
        {
            int aggro = 0;
            for(int i = 0; i < game.characters.size(); i++)
            {
                if(aggro < game.characters[i].stats.aggro)
                {
                    aggro = game.characters[i].stats.aggro;
                }
            }

            int found = false;
            for(int i = 0; i < game.locations[game.location].creatures.size(); i++)
            {
                if(aggro >= json2int(json.entity[game.locations[game.location].creatures[i]]["aggro"]))
                {
                    found = true;
                    combat_enemy(game.locations[game.location].creatures[i]);
                }
            }
            return found;
        }

        bool item_check(std::string id, unsigned int amount)
        {
            int found = 0;
            for(int i=0;i<game.characters[game.current_char-1].inventory.size();i++)
            {
                if(game.characters[game.current_char-1].inventory[i].id == id)
                {
                    found += game.characters[game.current_char-1].inventory[i].amount;
                }
            }
            if(found >= amount)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        bool item_add(std::string id, unsigned int amount)
        {
            unsigned int stack = json2int(json.item[id]["stack"]);
            unsigned int free = 0;
            free += (game.characters[game.current_char-1].inventory_max - game.characters[game.current_char-1].inventory.size())*stack;
            for(int i = 0;i<game.characters[game.current_char-1].inventory.size(); i++)
            {
                if(game.characters[game.current_char-1].inventory[i].id == id)
                {
                    free += (stack - game.characters[game.current_char-1].inventory[i].amount);
                }
            }
            if(free >= amount)
            {
                int returned = amount;
                for(int j = 0;j<game.characters[game.current_char-1].inventory.size(); j++)
                {
                    if(game.characters[game.current_char-1].inventory[j].id == id)
                    {
                        if(returned >= (stack - game.characters[game.current_char-1].inventory[j].amount))
                        {
                            returned -= (stack - game.characters[game.current_char-1].inventory[j].amount);
                            game.characters[game.current_char-1].inventory[j].amount = stack;
                        }
                        else
                        {
                            game.characters[game.current_char-1].inventory[j].amount += returned;
                            returned = 0;
                        }
                    }
                }
                while(returned > 0)
                {
                    if(returned >= stack)
                    {
                        game.characters[game.current_char-1].inventory.push_back({id,stack});
                        returned -= stack;
                    }
                    else
                    {
                        game.characters[game.current_char-1].inventory.push_back({id,(unsigned int)returned});
                        returned = 0;
                    }
                }
                return true;
            }
            else
            {
                return false;
            }
        }

        bool item_add_char(std::string id, unsigned int amount, unsigned int stack, int character)
        {
            unsigned int free = 0;
            free += (game.characters[character].inventory_max - game.characters[character].inventory.size())*stack;
            for(int i = 0;i<game.characters[character].inventory.size(); i++)
            {
                if(game.characters[character].inventory[i].id == id)
                {
                    free += (stack - game.characters[character].inventory[i].amount);
                }
            }
            if(free >= amount)
            {
                int returned = amount;
                for(int j = 0;j<game.characters[character].inventory.size(); j++)
                {
                    if(game.characters[character].inventory[j].id == id)
                    {
                        if(returned >= (stack - game.characters[character].inventory[j].amount))
                        {
                            returned -= (stack - game.characters[character].inventory[j].amount);
                            game.characters[character].inventory[j].amount = stack;
                        }
                        else
                        {
                            game.characters[character].inventory[j].amount += returned;
                            returned = 0;
                        }
                    }
                }
                while(returned > 0)
                {
                    if(returned >= stack)
                    {
                        game.characters[character].inventory.push_back({id,stack});
                        returned -= stack;
                    }
                    else
                    {
                        game.characters[character].inventory.push_back({id,(unsigned int) returned});
                        returned = 0;
                    }
                }
                return true;
            }
            else
            {
                return false;
            }
        }

        bool item_remove(std::string id, unsigned int amount)
        {
            int found = 0;
            for(int i=0;i<game.characters[game.current_char-1].inventory.size();i++)
            {
                if(game.characters[game.current_char-1].inventory[i].id == id)
                {
                    found += game.characters[game.current_char-1].inventory[i].amount;
                }
            }
            if(found >= amount)
            {
                int num2 = amount;
                for(int j=game.characters[game.current_char-1].inventory.size()-1;j>-1;j--)
                {
                    if(game.characters[game.current_char-1].inventory[j].id == id and num2 > 0)
                    {
                        if(num2 >= game.characters[game.current_char-1].inventory[j].amount)
                        {
                            num2 -= game.characters[game.current_char-1].inventory[j].amount;
                            game.characters[game.current_char-1].inventory.erase(game.characters[game.current_char-1].inventory.begin()+j);
                        }
                        else
                        {
                            game.characters[game.current_char-1].inventory[j].amount -= num2;
                            num2 = 0;
                        }
                    }
                }
                return true;
            }
            else
            {
                return false;
            }
        }

        bool item_remove_slot(unsigned int slot)
        {
            game.characters[game.current_char-1].inventory.erase(game.characters[game.current_char-1].inventory.begin()+slot);
            return(true);
        }

        int player_get_aggro()
        {
            int sum_of_weight = 0;
            for(int i=0; i<game.characters.size(); i++)
            {
                sum_of_weight += game.characters[i].stats.aggro;
            }
            int rnd = random() % sum_of_weight;
            for(int i=0; i<game.characters.size(); i++)
            {
                if(rnd < game.characters[i].stats.aggro)
                {
                    return i;
                }
                rnd -= game.characters[i].stats.aggro;
            }
            return -1;
        }

        void save_init(const std::string& savename)
        {
            game.id = savename;
            game.characters.push_back((struct PlayerStruct){"Guy1",1,{{"wood",15},{"test",1},{"wood",4}},{{"wood",1},{"sword",1},{"0",1},{"sword",1},{"0",1},{"wood",1}},{{"head",{"sword",1}},{"chest",{"0",1}},{"legs",{"0",1}},{"feet",{"sword",1}},{"waist",{"0",1}}},5,{20,20,20,20,1,0,30,30,0,1,0,50}});

            for(int i = 0; i < json.location["other"].size(); i++)
            {
                std::string name = json2str(json.location["other"][i]["id"]);
                game.locations[name];
                game.locations[name].creatures;
                for(int j = 0; j < json.location["other"][i]["creatures"].size(); j++)
                {
                    summon_creature(name,json2str(json.location["other"][i]["creatures"][j]));
                }
            }

            game.current_char = 1;
            game.location = "village_entrance";
            game.location_old = "village_entrance";
        }
};