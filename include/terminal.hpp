#pragma once

#include <import.hpp>
#include <datamanager.hpp>

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
        int state;
        int substate;

        WINDOW* win_bar;
        WINDOW* win_main;

        int ms = 1000000;
        int second = 0;

        bool pause;


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
            state = 0;
            substate = 0;

            //nodelay(stdscr, TRUE);
            // use this to break the game!!

            pause = false;

            getmaxyx(stdscr, win_height, win_width);

            win_bar = newwin(4,win_width,0,0);
            win_main = newwin(win_height,win_width,0,0);
        }

        void teardown()
        {
            endwin();
        }

        void start_draw()
        {
            werase(win_bar);
            werase(win_main);
            //erase();
        }

        void end_draw()
        {
            refresh();
            wrefresh(win_main);
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

                    startcolor(stdscr, "black", "white");
                    wborder(win_main,0,0,0,0,0,0,0,0);
                    wborder(win_bar,0,0,0,0,0,0,0,0);

                    mvwprintw(win_bar,1,1,"Name: %s | Level: %d", data.id.c_str(), second);
                    endcolor(stdscr);
                    break;
                }
            }

            end_draw();
        }
};