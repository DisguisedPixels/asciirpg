#pragma once

#include <import.hpp>

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
        int win_height;
        int win_width;

        int menu;
        int state;
        int substate;

        WINDOW* win_bar;
        WINDOW* win_main;

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

            menu = 0;
            state = 0;
            substate = 0;

            

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
            wclear(win_bar);
            wclear(win_main);
            clear();
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

        int get_color(const std::string& background, const std::string& foreground)
        {
            std::string color_name = background + "-" + foreground;
            return m_colors[color_name];
        }
};