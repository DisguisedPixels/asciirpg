#include <import.hpp>
#include <terminal.hpp>
#include <datamanager.hpp>
#include <utils.hpp>

TerminalClass terminal;
DataManager data;
JsonData json;

void game_tick()
{
    while(terminal.running)
    {
        // Wait
        usleep(0.25*terminal.ms);

        // Logic
        terminal.tick += 1;

        switch(terminal.menu)
        {
            case 0: // MENU
            {
                break;
            }
        }

        // Refresh
        if(!terminal.pause)
        {
            terminal.print_scr(data.game, data.character_max);
        }
    }
}

int main()
{
    srand ( time(NULL) );
    
    std::string PATH = std::filesystem::current_path();
    if(json.read_json(PATH + "/bin/data/json"))
    {

    }

    terminal.setup();

    

    std::thread tick_thread (game_tick);
    tick_thread.detach();

    while(terminal.running)
    {
        terminal.print_scr(data.game, data.character_max);

        int input = terminal.get_key();
        if(input == KEY_F(1))
        {
            terminal.teardown();
            return 0;
        }
        else switch(terminal.menu)
        {
            case 0: // MENU
            {
                switch(terminal.submenu)
                {
                    case 0: // MAIN
                    {
                        switch(input)
                        {
                            case 27:
                            {
                                terminal.teardown();
                                return 0;
                            }
                            case 'p':
                            {
                                terminal.menu = 1;
                                terminal.submenu = 0;
                                break;
                            }
                        }
                        break;
                    }
                }
                break;
            }
            case 1: // GAME
            {
                switch(terminal.submenu)
                {
                    case 0: // MAIN
                    {
                        switch(input)
                        {
                            case 27:
                            {
                                terminal.menu = 0;
                                terminal.submenu = 0;
                                break;
                            }
                            case 'c':
                            {
                                terminal.submenu = 1;
                                break;
                            }
                            case 't':
                            {
                                terminal.submenu = 2;
                                break;
                            }
                            case 'i':
                            {
                                terminal.submenu = 3;
                                break;
                            }
                            case '\n':
                            {
                                std::string test = terminal.get_string();

                                std::vector<std::string> result;
                                std::istringstream iss(test);
                                for(std::string s; iss >> s; )
                                {
                                    result.push_back(s);
                                }
                                if(result.size() > 0)
                                {
                                if(result[0] == "c")
                                {   
                                    if(result.size() >= 3)
                                    {
                                        for (int i = 0; i < data.game.characters.size(); ++i)
                                        {
                                            if(data.game.characters[i].name == result[1])
                                            {
                                                if(result[2] == "select")
                                                {
                                                    data.game.current_char = i+1;
                                                }
                                                else if(result[2] == "remove" or result[2] == "delete")
                                                {
                                                    if(data.game.characters.size() > 1)
                                                    {
                                                        data.game.characters.erase(data.game.characters.begin()+i);
                                                    }
                                                }
                                                else if(result[2] == "rename" and result.size() >= 4)
                                                {
                                                    data.game.characters[i].name = result[3];
                                                }
                                                break;
                                            }
                                        }
                                    }
                                    else
                                    {
                                        terminal.submenu = 1;
                                    }
                                }
                                else if(result[0] == "t")
                                {
                                    if(result.size() >= 2)
                                    {
                                        Json::Value connections = json.location["data"][data.game.location]["connect"];
                                        for(int i = 0; i < connections.size(); i++)
                                        {
                                            if(connections[i][0] == result[1])
                                            {
                                                std::string name = json2str(json.location["data"][data.game.location]["connect"][i][0]);
                                                terminal.travel_to(data,name);
                                                break;
                                            }
                                        }
                                    }
                                    else
                                    {
                                        terminal.submenu = 2;
                                    }
                                }
                                break;
                                }
                            }
                        }
                        break;
                    }
                    case 1: // CHARACTER SELECT
                    {
                        switch(input)
                        {
                            case 27:
                            {
                                terminal.submenu = 0;
                                terminal.slot = 0;
                                break;
                            }
                            case KEY_UP:
                            {
                                if(terminal.slot > 0)
                                {
                                    terminal.slot -= 1;
                                }
                                break;
                            }
                            case KEY_DOWN:
                            {
                                if(terminal.slot < data.game.characters.size())
                                {
                                    terminal.slot += 1;
                                }
                                break;
                            }
                            case 's':
                            {
                                if(terminal.slot != 0 and terminal.slot != data.game.characters.size())
                                {
                                    std::swap(data.game.characters[terminal.slot],data.game.characters[terminal.slot-1]);
                                }
                                break;
                            }
                            case 'w':
                            {
                                if(terminal.slot >= 2)
                                {
                                    std::swap(data.game.characters[terminal.slot-1],data.game.characters[terminal.slot-2]);
                                }
                                break;
                            }
                            case 'c':
                            {
                                if(terminal.slot != 0)
                                {
                                    data.game.current_char = terminal.slot;
                                    terminal.submenu = 0;
                                    terminal.slot = 0;
                                }
                                break;
                            }
                            case 'd':
                            {
                                if(terminal.slot != 0)
                                {
                                    if(data.game.characters.size() > 1)
                                    {
                                        data.game.characters.erase(data.game.characters.begin()+terminal.slot-1);
                                    }
                                    terminal.slot = 0;
                                }
                                break;
                            }
                            case 'r':
                            {
                                if(terminal.slot != 0)
                                {
                                    std::string result = terminal.get_string();
                                    data.game.characters[terminal.slot-1].name = result;
                                }
                                break;
                            }
                        }
                        break;
                    }
                    case 2: // LOCATION SELECT
                    {
                        switch(input)
                        {
                            case 27:
                            {
                                terminal.submenu = 0;
                                terminal.slot = 0;
                                break;
                            }
                            case KEY_UP:
                            {
                                if(terminal.slot > 0)
                                {
                                    terminal.slot -= 1;
                                }
                                break;
                            }
                            case KEY_DOWN:
                            {
                                if(terminal.slot < json.location["data"][data.game.location]["connect"].size())
                                {
                                    terminal.slot += 1;
                                }
                                break;
                            }
                            case 't':
                            {
                                if(terminal.slot != 0)
                                {
                                    std::string name = json2str(json.location["data"][data.game.location]["connect"][terminal.slot-1][0]);
                                    terminal.travel_to(data,name);
                                }
                                break;
                            }
                        }
                        break;
                    }
                    case 3: // INVENTORY
                    {
                        switch(input)
                        {
                            case 27:
                            {
                                terminal.submenu = 0;
                                terminal.slot = 0;
                                break;
                            }
                            case KEY_UP:
                            {
                                if(terminal.slot > 0)
                                {
                                    terminal.slot -= 1;
                                }
                                break;
                            }
                            case KEY_DOWN:
                            {
                                if(terminal.slot < data.game.characters[data.game.current_char-1].inventory.size())
                                {
                                    terminal.slot += 1;
                                }
                                break;
                            }
                            case 's':
                            {
                                if(terminal.slot != 0 and terminal.slot != data.game.characters[data.game.current_char-1].inventory.size())
                                {
                                    std::swap(data.game.characters[data.game.current_char-1].inventory[terminal.slot],data.game.characters[data.game.current_char-1].inventory[terminal.slot-1]);
                                }
                                break;
                            }
                            case 'w':
                            {
                                if(terminal.slot >= 2)
                                {
                                    std::swap(data.game.characters[data.game.current_char-1].inventory[terminal.slot-2],data.game.characters[data.game.current_char-1].inventory[terminal.slot-1]);
                                }
                                break;
                            }
                            case 'm':
                            {
                                if(terminal.slot > 0)
                                {
                                    ItemStruct item = data.game.characters[data.game.current_char-1].inventory[terminal.slot-1];
                                    data.item_remove_slot(terminal.slot-1);
                                    data.item_add(item.id,item.amount,json2int(json.item[item.id]["stack"]));
                                    if(terminal.slot > data.game.characters[data.game.current_char-1].inventory.size())
                                    {
                                        terminal.slot-=1;
                                    }
                                }
                                break;
                            }
                            case 'l':
                            {
                                if(terminal.slot > 0 and data.game.characters[data.game.current_char-1].inventory.size() < data.game.characters[data.game.current_char-1].inventory_max)
                                {
                                    std::string val = terminal.get_string();
                                    if(terminal.check_int(val))
                                    {
                                        int val2 = stoi(val);
                                        if(data.game.characters[data.game.current_char-1].inventory[terminal.slot-1].amount > val2)
                                        {
                                            data.game.characters[data.game.current_char-1].inventory[terminal.slot-1].amount -= val2;
                                            data.game.characters[data.game.current_char-1].inventory.push_back({data.game.characters[data.game.current_char-1].inventory[terminal.slot-1].id,(unsigned int)val2});
                                            terminal.slot = data.game.characters[data.game.current_char-1].inventory.size();
                                        }
                                    }
                                }
                                break;
                            }
                            case 'x':
                            {
                                if(terminal.slot > 0)
                                {
                                    data.item_remove_slot(terminal.slot-1);
                                    terminal.slot -= 1;
                                }
                                break;
                            }
                            case 'k':
                            {
                                if(terminal.slot > 0)
                                {
                                    std::string character = terminal.get_string();
                                    for(int i = 0; i < data.game.characters.size(); i++)
                                    {
                                        if(data.game.characters[i].name == character)
                                        {
                                            ItemStruct item = data.game.characters[data.game.current_char-1].inventory[terminal.slot-1];
                                            if(data.item_add_char(item.id,item.amount,json2int(json.item[item.id]["stack"]),i))
                                            {
                                                data.item_remove_slot(terminal.slot-1);
                                                terminal.slot -= 1;
                                            }
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        break;
                    }
                    case 4: // COMBAT
                    {
                        switch(terminal.state)
                        {
                            case 0:
                            {
                                switch(input)
                                {
                                    case '\n':
                                    {
                                        terminal.combat_log.clear();
                                        terminal.state = 1;
                                        break;
                                    }
                                }
                                break;
                            }
                            case 1:
                            {
                                switch(input)
                                {
                                    case '\n':
                                    {
                                        if(data.game.enemies.size() < 1)
                                        {
                                            terminal.substate = 0;
                                            terminal.state = 3;
                                            break;
                                        }
                                        // Logic
                                        std::string say = "Party " + std::to_string(terminal.substate+1) + ": Attack ";
                                        terminal.combat_log.push_back(say);

                                        terminal.substate += 1;
                                        if(terminal.substate > data.game.characters.size()-1)
                                        {
                                            terminal.substate = 0;
                                            terminal.state = 2;
                                            break;
                                        }
                                        break;
                                    }
                                }
                                break;
                            }
                            case 2:
                            {
                                switch(input)
                                {
                                    case '\n':
                                    {
                                        if(data.game.enemies.size() < 1)
                                        {
                                            terminal.substate = 0;
                                            terminal.state = 3;
                                            break;
                                        }
                                        // Logic
                                        int selected = data.player_get_aggro();
                                        std::string say = "Enemy " + std::to_string(terminal.substate+1) + ": Attack " + data.game.characters[selected].name;
                                        terminal.combat_log.push_back(say);

                                        terminal.substate += 1;
                                        if(terminal.substate > data.game.enemies.size()-1)
                                        {
                                            terminal.substate = 0;
                                            terminal.state = 1;
                                            break;
                                        }
                                        break;
                                    }
                                }
                                break;
                            }
                            case 3:
                            {
                                switch(input)
                                {
                                    case '\n':
                                    {
                                        terminal.combat_log.clear();
                                        terminal.submenu = 0;
                                        terminal.substate = 0;
                                        terminal.state = 0;
                                        break;
                                    }
                                }
                                break;
                            }
                        }
                        break;
                    }
                }
                break;
            }
        }
    }
    return 0;
}