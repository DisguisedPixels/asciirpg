#pragma once

#include <import.hpp>
#include <datamanager.hpp>
#include <utils.hpp>


class TerminalClass
{
    public:
    std::string PATH;

    int tick;

    bool running;
    bool pause;

    unsigned int menu;
    unsigned int submenu;
    unsigned int state;
    int substate;
    int slot;

    int gametime = 0;

    std::vector <std::string> combat_log {};

    const unsigned int ms = 1000000;

    TerminalClass()
    {
        
    }
    ~TerminalClass()
    {
        teardown();
    }

    int scr_width;
    int scr_height;

    WINDOW *win_bar_frame;
    WINDOW *win_bar;

    WINDOW *win_main_frame;
    WINDOW *win_main;

    WINDOW *win_side_frame;
    WINDOW *win_side;

    JsonData json;

    void setup()
    {
        initscr();
        noecho();
        curs_set(0);
        keypad(stdscr,TRUE);
        cbreak();
        //mousemask(ALL_MOUSE_EVENTS, NULL);


        getmaxyx(stdscr, scr_height, scr_width);
        resize_scr();

        running = true;
        pause = false;

        menu = 1;
        submenu = 5;
        state = 0;
        substate = 0;
        slot = 0;

        win_bar = newwin(2,scr_width-2,1,1);
        win_side = newwin(scr_height-5,28,4,1);
        win_main = newwin(scr_height-5,scr_width-31,4,30);

        PATH = std::filesystem::current_path();
        json.read_json(PATH + "/bin/data/json");
        tick = 0;
    }

    void teardown()
    {
        endwin();
        running = false;
    }

    void start_draw()
    {
        int tmp_x, tmp_y;
        getmaxyx(stdscr, tmp_y, tmp_x);
        if(scr_height != tmp_y or scr_width != tmp_x)
        {
            scr_height = tmp_y;
            scr_width = tmp_x;
            resize_scr();
        }

        werase(win_side);
        werase(win_main);
        werase(win_bar);
    }

    void end_draw()
    {
        refresh();
        
        wrefresh(win_side);
        wrefresh(win_main);
        wrefresh(win_bar);
    }

    void resize_scr()
    {
        win_main_frame = newwin(scr_height-3,scr_width-29,3,29);
        win_side_frame = newwin(scr_height-3,30,3,0);
        win_bar_frame = newwin(4,scr_width,0,0);

        wborder(win_main_frame,0,0,0,0,0,0,0,0);
        wborder(win_side_frame,0,0,0,0,0,0,0,0);
        wborder(win_bar_frame,0,0,0,0,0,0,0,0);

        refresh();

        wrefresh(win_main_frame);
        wrefresh(win_side_frame);
        wrefresh(win_bar_frame);

        wresize(win_side,scr_height-5,28);
        wresize(win_main,scr_height-5,scr_width-31);
        wresize(win_bar,2,scr_width-2);
    }

    void print_scr(SaveGameStruct data, int char_max)
    {
        start_draw();

        switch(menu)
        {
            case 0: // MENU
            {
                mvwprintw(win_bar,0,0,"ASCII RPG");
                mvwprintw(win_bar,1,0,"MENU");

                switch(submenu)
                {
                    case 0: // MAIN
                    {
                        mvwprintw(win_side,1,1,"ESC: Exit");
                        mvwprintw(win_side,2,1,"P: Play");
                        break;
                    }
                }
                break;
            }
            case 1: // GAME
            {
                // MAIN
                PlayerStruct current_char = data.characters[data.current_char-1];

                mvwprintw(win_bar,0,0,"ASCII RPG | Profile: %s | Party: (%d/%d)", data.id.c_str(),data.characters.size(),char_max);
                mvwprintw(win_bar,1,0,"%s LVL%d | Hotbar: %s, %s, %s, %s, %s, %s", current_char.name.c_str(),current_char.level,current_char.hotbar[0].id.c_str(),current_char.hotbar[1].id.c_str(),current_char.hotbar[2].id.c_str(),current_char.hotbar[3].id.c_str(),current_char.hotbar[4].id.c_str(),current_char.hotbar[5].id.c_str());

                std::string location_1 = json2str(json.location["data"][data.location]["name"][0]);
                std::string location_2 = json2str(json.location["data"][data.location]["name"][1]);
                std::string location_3 = json2str(json.location["data"][data.location]["name"][2]);
                unsigned int size = location_1.size() + location_2.size() + location_3.size() + 8;

                mvwprintw(win_bar,0,scr_width-(std::to_string(tick).length() + std::to_string(gametime).length())-20,"Turn: %d | Elapsed: %d",gametime,tick);
                mvwprintw(win_bar,1,scr_width-size,"%s / %s / %s",location_1.c_str(),location_2.c_str(),location_3.c_str());

                // DETAIL
                switch(submenu)
                {
                    case 0: // MAIN
                    {
                        mvwprintw(win_side,0,0,"Main");

                        mvwprintw(win_side,2,0,"ESC: Menu");
                        mvwprintw(win_side,3,0,"ENTER: Terminal");
                        mvwprintw(win_side,4,0,"SPACE: Wait");
                        mvwprintw(win_side,5,0,"C: Character Menu");
                        mvwprintw(win_side,6,0,"T: Travel Menu");
                        mvwprintw(win_side,7,0,"I: Inventory");
                        mvwprintw(win_side,8,0,"E: Equipment");
                        break;
                    }
                    case 1: // CHARACTER SELECT
                    {
                        mvwprintw(win_side,0,0,"Character Select");

                        mvwprintw(win_side,2,0,"ESC: Back");
                        mvwprintw(win_side,3,0,"/\\: Up");
                        mvwprintw(win_side,4,0,"\\/: Down");
                        mvwprintw(win_side,5,0,"W: Item Up");
                        mvwprintw(win_side,6,0,"S: Item Down");
                        mvwprintw(win_side,7,0,"S: Select");
                        mvwprintw(win_side,8,0,"R: Rename");
                        mvwprintw(win_side,9,0,"D: Delete");

                        mvwprintw(win_main,0,1,"Character List:");
                        for (int i = 0; i < data.characters.size(); i++)
                        {
                            if(i == data.characters.size()-1)
                            {
                                mvwprintw(win_main,1+i,1,"\\ %s LVL%d",data.characters[i].name.c_str(),data.characters[i].level);
                            }
                            else
                            {
                                mvwprintw(win_main,1+i,1,"| %s LVL%d",data.characters[i].name.c_str(),data.characters[i].level);
                            }
                        }
                        mvwprintw(win_main,0+slot,0,">");
                        break;
                    }
                    case 2: // LOCATION SELECT
                    {
                        mvwprintw(win_side,0,0,"Travel Select");

                        mvwprintw(win_side,2,0,"ESC: Back");
                        mvwprintw(win_side,3,0,"/\\: Up");
                        mvwprintw(win_side,4,0,"\\/: Down");
                        mvwprintw(win_side,5,0,"T: Travel");

                        mvwprintw(win_main,0,1,"Location List:");
                        Json::Value connections = json.location["data"][data.location]["connect"];
                        for (int i = 0; i < connections.size(); i++)
                        {
                            if(i == connections.size()-1)
                            {
                                mvwprintw(win_main,1+i,1,"\\ %s %s (%s)",json2str(json.location["data"][json2str(connections[i][0])]["name"][1]).c_str(),json2str(json.location["data"][json2str(connections[i][0])]["name"][2]).c_str(),json2str(connections[i][0]).c_str());
                            }
                            else
                            {
                                mvwprintw(win_main,1+i,1,"| %s %s (%s)",json2str(json.location["data"][json2str(connections[i][0])]["name"][1]).c_str(),json2str(json.location["data"][json2str(connections[i][0])]["name"][2]).c_str(),json2str(connections[i][0]).c_str());
                            }
                        }
                        mvwprintw(win_main,0+slot,0,">");
                        break;
                    }
                    case 3: // INVENTORY
                    {
                        mvwprintw(win_side,0,0,"Inventory");

                        mvwprintw(win_side,2,0,"ESC: Back");
                        mvwprintw(win_side,3,0,"/\\: Up");
                        mvwprintw(win_side,4,0,"\\/: Down");
                        mvwprintw(win_side,5,0,"W: Item Up");
                        mvwprintw(win_side,6,0,"S: Item Down");
                        mvwprintw(win_side,7,0,"M: Merge Stack");
                        mvwprintw(win_side,8,0,"L: Split Stack");
                        mvwprintw(win_side,9,0,"X: Remove Stack");
                        mvwprintw(win_side,10,0,"K: Transfer Stack");

                        std::vector <ItemStruct> items = data.characters[data.current_char-1].inventory;
                        mvwprintw(win_main,0,1,"Item List:");
                        mvwprintw(win_main,0,20-(count_digit(items.size())+count_digit(data.characters[data.current_char-1].inventory_max)),"(%d/%d)",items.size(),data.characters[data.current_char-1].inventory_max);
                        for (int i = 0; i < items.size(); i++)
                        {
                            if(i == items.size()-1)
                            {
                                mvwprintw(win_main,1+i,1,"\\ %s",json2str(json.item[items[i].id]["name"]).c_str());
                            }
                            else
                            {
                                mvwprintw(win_main,1+i,1,"| %s",json2str(json.item[items[i].id]["name"]).c_str());
                            }
                            mvwprintw(win_main,1+i,22-count_digit(items[i].amount),"x%d",items[i].amount);
                        }
                        mvwprintw(win_main,0+slot,0,">");
                        break;
                    }
                    case 4: // COMBAT
                    {
                        /*
                        wmove(win_main,0,0);
                        
                        int size = scr_height-4;
                        int logs = size;
                        if(combat_log.size()<size)
                        {
                            logs = combat_log.size();
                        }
                        for(int i = size-logs; i < logs; i++)
                        {
                            wprintw(win_main,"%s %c",combat_log[i].c_str(), '\n');
                        }
                        */

                        int pos = scr_height-6;
                        for(int i = combat_log.size()-1; i >= 0 ; i--)
                        {
                            if(pos >= 0)
                            {
                                std::string tmp = combat_log[i];
                                if(tmp.size() > scr_width-31)
                                {
                                    tmp.erase(scr_width-31);
                                }
                                mvwprintw(win_main,pos,0,"%s",tmp.c_str());
                                // cut before it gets to \n
                                pos -= 1;
                            }
                            else
                            {
                                break;
                            }
                        }
                        

                        switch(state)
                        {
                            case 0: // INIT
                            {
                                mvwprintw(win_side,0,0,"Combat: Start");

                                mvwprintw(win_side,2,0,"\\n: Continue");
                                break;
                            }
                            case 1: // PARTY
                            {
                                mvwprintw(win_side,0,0,"Combat: Party (%d/%d)",substate+1,data.characters.size());
                                mvwprintw(win_side,1,1,"| %s [%d/%d]",data.characters[substate].name.c_str(),data.characters[substate].stats.health,data.characters[substate].stats.health_max);
                                mvwprintw(win_side,3,1,"\\n:Skip");
                                mvwprintw(win_side,4,1,"E:Excape");
                                mvwprintw(win_side,4,1,"A:Attack");
                                break;
                            }
                            case 2: // ENEMY
                            {
                                mvwprintw(win_side,0,0,"Combat: Enemy (%d/%d)", substate+1, data.enemies.size());
                                mvwprintw(win_side,1,1,"| %s [%d/%d]",json2str(json.entity[data.enemies[substate].id]["name"]).c_str(),data.enemies[substate].health,json2int(json.entity[data.enemies[substate].id]["health"]));
                                mvwprintw(win_side,3,1,"\\n:Continue");
                                break;
                            }
                            case 3: // DEINIT
                            {
                                mvwprintw(win_side,0,0,"Combat: Complete");

                                mvwprintw(win_side,2,0,"\\n: Continue");
                                break;
                            }
                        }
                        break;
                    }
                    case 5: // EQUIPMENT
                    {
                        mvwprintw(win_side,0,0,"Equipment");

                        mvwprintw(win_side,2,0,"ESC: Back");
                        mvwprintw(win_side,3,0,"h: Remove Hotbar Slot");
                        mvwprintw(win_side,4,0,"e: Remove Equipment Slot");
                        

                        mvwprintw(win_main,0,0,"Hotbar:");
                        for(int i = 0; i < 6; i++)
                        {
                            if(data.characters[data.current_char-1].hotbar[i].id != "0")
                            {
                                mvwprintw(win_main,1+i,0,"%d: %s",i+1,json2str(json.item[data.characters[data.current_char-1].hotbar[i].id]["name"]).c_str());

                            mvwprintw(win_main,1+i,21-count_digit(data.characters[data.current_char-1].hotbar[i].amount),"x%d",data.characters[data.current_char-1].hotbar[i].amount);
                            }
                            else
                            {
                                mvwprintw(win_main,1+i,0,"%d: None",i+1);
                            }
                        }


                        std::map <std::string, ItemStruct> equipment = data.characters[data.current_char-1].equipment;
                        mvwprintw(win_main,0,24,"Equipment:");
                        mvwprintw(win_main,1,24,"1: Head:  %s",json2str(json.item[equipment["head"].id]["name"]).c_str());
                        mvwprintw(win_main,2,24,"2: Chest: %s",json2str(json.item[equipment["chest"].id]["name"]).c_str());
                        mvwprintw(win_main,3,24,"3: Legs:  %s",json2str(json.item[equipment["legs"].id]["name"]).c_str());
                        mvwprintw(win_main,4,24,"4: Feet:  %s",json2str(json.item[equipment["feet"].id]["name"]).c_str());
                        mvwprintw(win_main,5,24,"5: Waist: %s",json2str(json.item[equipment["waist"].id]["name"]).c_str());
                        break;
                    }
                }
                break;
            }
        }
        
        end_draw();
    }

    bool check_int(std::string &str)
    {
        for(int i = 0; i < str.length()-1; i++)
        {
            if(!isdigit(str[i]))
            {
                return false;
            }
        }
        return true;
    }

    int count_digit(int number)
    {
        return int(log10(number) + 1);
    }

    int get_key()
    {
        return wgetch(stdscr);
    }

    std::string get_string()
    {
        std::string input;

        nocbreak();
        echo();
        curs_set(1);
        pause = true;

        move(scr_height-2,30);
        int ch = getch();

        while ( ch != '\n' )
        {
            input.push_back( ch );
            ch = getch();
        }

        cbreak();
        noecho();
        curs_set(0);
        pause = false;

        return input;
    }

    void travel_to(DataManager &data, std::string &name)
    {
        data.game.location_old = data.game.location;
        Json::Value current_location = json.location["data"][name];
        data.game.location = json2str(current_location["location"][2]);
        //data.enemy_check(data.game.locations[data.game.location]);
        if(data.enemy_check())
        {
            slot = 0;
            submenu = 4;
        }
        else
        {
            slot = 0;
            submenu = 0;
        }
    }
};