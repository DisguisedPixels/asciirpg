#pragma once

#include <import.hpp>
#include <datamanager.hpp>
#include <utils.hpp>

class Terminal
{
    private:
        bool running;
        std::map<std::string, int> m_colors;
        std::string last_bg;
        std::string last_fg;

        void setup_colors()
        {
            std::string color_names[] = {
                "black", "red", "green", "blue", "yellow", "magenta", "cyan", "white"
            };

            /* fg = foreground || bg = background */
            int counter = 0;
            for(int fg = 0; fg < 8; fg++)
            {
                for(int bg = 0; bg < 8; bg++)
                {
                    std::string color_name = color_names[bg] + "-" + color_names[fg];
                    init_pair(counter,fg,bg);

                    m_colors[color_name] = counter;
                    counter++;
                }
            }
        }

    public:
        std::string PATH;
        int win_height;
        int win_width;

        int menu;
        int submenu;

        int character_max;

        WINDOW* win_bar;
        WINDOW* win_main;
        WINDOW* win_side;

        int ms = 1000000;
        int tick = 0;

        bool pause;

        Json::Value location;
        
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

        Terminal()
        {
            setup();
        }

        ~Terminal()
        {
            teardown();
        }

        void setup()
        {
            initscr();
            noecho();
            //raw();
            curs_set(0);
            keypad(stdscr, TRUE);
            mousemask(ALL_MOUSE_EVENTS, NULL);

            if(has_colors())
            {
                start_color();
                setup_colors();
            }

            running = true;
            last_bg = "";
            last_fg = "";

            PATH = std::filesystem::current_path();

            menu = 0;
            submenu = 0;

            character_max = 5;

            nodelay(stdscr, FALSE);
            // use this to break the game!!

            pause = false;

            getmaxyx(stdscr, win_height, win_width);

            win_main = newwin(win_height,win_width,0,0);
            win_side = newwin(win_height-3,30,3,0);
            win_bar = newwin(4,win_width,0,0);
        }

        void teardown()
        {
            endwin();
        }

        void start_draw()
        {
            werase(win_main);
            werase(win_side);
            werase(win_bar);
            //erase();
        }

        void end_draw()
        {
            refresh();
            wrefresh(win_main);
            wrefresh(win_side);
            wrefresh(win_bar);
        }

        void startcolor(WINDOW * win, const std::string& background, const std::string& foreground)
        {
            if(last_bg != "")
            {
                endcolor(win);
            }

            last_bg = background;
            last_fg = foreground;

            wattron(win, COLOR_PAIR(get_color(background,foreground)));
        }

        void endcolor(WINDOW * win)
        {
            wattroff(win, COLOR_PAIR(get_color(last_bg,last_fg)));
            last_bg = "";
            last_fg = "";
        }

        bool is_running()
        {
            return running;
        }

        void quit()
        {
            running = false;
            teardown();
        }

        void frame_init()
        {
            getmaxyx(stdscr, win_height, win_width);
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

            move(win_height-2,1);
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

        int get_color(const std::string& background, const std::string& foreground)
        {
            std::string color_name = background + "-" + foreground;
            return m_colors[color_name];
        }

        void draw_refresh(SaveGameStruct &data)
        {
            start_draw();

            switch(menu)
            {
                case 0:
                {
                    wresize(win_bar,4,win_width);
                    wresize(win_main,win_height,win_width);
                    wresize(win_side,win_height-3,30);

                    startcolor(stdscr, "black", "white");
                    wborder(win_main,0,0,0,0,0,0,0,0);
                    wborder(win_side,0,0,0,0,0,0,0,0);
                    wborder(win_bar,0,0,0,0,0,0,0,0);

                    mvwprintw(win_bar,1,1,"ASCII RPG");
                    switch(submenu)
                    {
                        case 0:
                        {
                            break;
                        }
                    }
                    endcolor(stdscr);
                    break;
                }
                case 1:
                {
                    wresize(win_bar,4,win_width);
                    wresize(win_main,win_height,win_width);
                    wresize(win_side,win_height-3,30);

                    startcolor(stdscr, "black", "white");
                    wborder(win_main,0,0,0,0,0,0,0,0);
                    wborder(win_side,0,0,0,0,0,0,0,0);
                    wborder(win_bar,0,0,0,0,0,0,0,0);

                    PlayerStruct current_char = data.characters[data.current_char-1];

                    mvwprintw(win_bar,1,1,"ASCII RPG | Profile: %s", data.id.c_str());
                    mvwprintw(win_bar,2,1,"%s LVL%d | Party: (%d/%d)", current_char.name.c_str(), current_char.level,data.characters.size(),character_max);
                    std::string location_1 = json2str(location[data.location]["name"][0]);
                    std::string location_2 = json2str(location[data.location]["name"][1]);
                    std::string location_3 = json2str(location[data.location]["name"][2]);
                    unsigned int size = location_1.size() + location_2.size() + location_3.size() + 8;
                    mvwprintw(win_bar,1,win_width-size,"%s / %s / %s",location_1.c_str(),location_2.c_str(),location_3.c_str());
                    switch(submenu)
                    {
                        case 0:
                        {
                            mvwprintw(win_side,1,1,"Main");
                            mvwprintw(win_side,3,1,"ESC:Exit");
                            mvwprintw(win_side,4,1,"c:Select Character");
                            break;
                        }
                        case 1:
                        {
                            mvwprintw(win_side,1,1,"Select Character");
                            mvwprintw(win_side,3,1,"ESC:Back");
                            mvwprintw(win_side,4,1,"1-%d: Select Character",data.characters.size());

                            for (int i = 0; i < data.characters.size(); i++)
                            {
                                mvwprintw(win_main,4+i,30,"%d | %s LVL%d",i+1,data.characters[i].name.c_str(),data.characters[i].level);
                            }

                            break;
                        }
                    }
                    endcolor(stdscr);
                    break;
                }
            }

            end_draw();
        }
};