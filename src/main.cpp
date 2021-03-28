#include <import.hpp>
#include <terminal.hpp>
#include <datamanager.hpp>
#include <utils.hpp>

Terminal terminal;
DataManager data;

void tick_passed()
{
    while(terminal.is_running())
    {
        // Wait
        usleep(0.5*terminal.ms);
        
        // Logic
        terminal.tick += 1;

        switch(terminal.menu)
        {
            case 0:
            {
                break;
            }
            case 1:
            {
                break;
            }
        }

        if(!terminal.pause)
        {
            terminal.draw_refresh(data.game);
        }
    }
}

bool load_data()
{
    int result = terminal.read_json(terminal.PATH + "/bin/data/json");
    if(result == 1)
    {
        terminal.draw_refresh(data.game);
        mvwprintw(terminal.win_main,2,1,"Loading error: files not located");
        terminal.end_draw();
        getch();
        return false;
    }
    else if(result == 2)
    {
        terminal.draw_refresh(data.game);
        mvwprintw(terminal.win_main,2,1,"Loading error: file format error");
        terminal.end_draw();
        getch();
        return false;
    }

    return true;
}

int main()
{
    if(!load_data())
    {
        return 1;
    }

    data.save_init("Test");

    std::thread tick_thread (tick_passed);
    tick_thread.detach();

    Terminal* term = &terminal;

    while(terminal.is_running())
    {
        /* FRAME INIT */
        terminal.frame_init();



        /* LOGIC      */



        /* RENDER     */
        terminal.draw_refresh(data.game);



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
            // MENU
            case 0:
            {
            
            switch(terminal.submenu)
            {
                case 0:
                {
                    switch(input)
                    {
                        case 'p':
                        {
                            terminal.menu = 1;
                            break;
                        }
                    }
                    break;
                }
            }
            break;

            }
            // GAME
            case 1:
            {

            switch(terminal.submenu)
            {
                // MAIN
                case 0:
                {
                    switch(input)
                    {
                        case 27:
                        {
                            terminal.menu = 0;
                            break;
                        }
                        case 'c':
                        {
                            terminal.submenu = 1;
                            break;
                        }
                    }
                    break;
                }
                // SWITCH CHARACTER
                case 1:
                {
                    switch(input)
                    {
                        case 27:
                        {
                            terminal.submenu = 0;
                            break;
                        }
                        case '1':
                        case '2':
                        case '3':
                        case '4':
                        case '5':
                        {
                            int num;
                            switch(input)
                            {
                                case '1': {num = 1; break;};
                                case '2': {num = 2; break;};
                                case '3': {num = 3; break;};
                                case '4': {num = 4; break;};
                                case '5': {num = 5; break;};
                            }
                            if(num <= data.game.characters.size())
                            {
                                data.game.current_char = num;
                                terminal.submenu = 0;
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
        break;
        }
        }
    }

    return 0;
}