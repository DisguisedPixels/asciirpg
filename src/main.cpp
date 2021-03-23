#include <main.hpp>
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

        terminal.startcolor(stdscr, "green", "black");
        mvprintw(2,2,"gg!");
        terminal.endcolor(stdscr);

        terminal.end_draw();



        /* INPUT      */
        int input = terminal.get_key();
        switch(input)
        {
            case 'p':
            {
                terminal.quit();
                break;
            }
            default:
            {
                break;
            }
        }
    }

    return 0;
}