#include <import.hpp>
#include <terminal.hpp>


int main()
{
    Terminal terminal;
    
    while(terminal.is_running())
    {
        /* FRAME INIT */
        terminal.frame_init();



        /* LOGIC      */
        


        /* RENDER     */
        terminal.start_draw();

        switch(terminal.menu)
        {
            case 0:
            {
                wresize(terminal.win_bar,4,terminal.win_width);
                wresize(terminal.win_main,terminal.win_height,terminal.win_width);

                terminal.startcolor(stdscr, "black", "white");
                wborder(terminal.win_main,0,0,0,0,0,0,0,0);
                wborder(terminal.win_bar,0,0,0,0,0,0,0,0);

                mvwprintw(terminal.win_bar,1,1,"asciirpg/menu/main");
                terminal.endcolor(stdscr);
                break;
            }
        }

        terminal.end_draw();



        /* INPUT      */
        int input = terminal.get_key();
        switch(input)
        {
            case KEY_F(1):
            {
                terminal.quit();
                break;
            }
            default:
            {
                switch(terminal.menu)
                {
                    case 0:
                    {
                        switch(input)
                        {
                            case 't':
                            {
                                terminal.menu = 1;
                            }
                        }
                        break;
                    }
                    case 1:
                    {
                        switch(input)
                        {
                            case 'q':
                            {
                                terminal.menu = 0;
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