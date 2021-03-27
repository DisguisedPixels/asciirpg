#include <import.hpp>
#include <terminal.hpp>
#include <datamanager.hpp>
#include <utils.hpp>

Terminal terminal;
DataManager data;

void tick_passed()
{
    // Wait
    usleep(0.5*terminal.ms);

    // Logic
    if(terminal.is_running())
    {
        terminal.second += 1;
        if(!terminal.pause)
        {
            terminal.draw_refresh(data.game);
        }
    }
    else
    {
        std::terminate();
    }

    // Replay
    tick_passed();
}

bool load_data()
{
    int result = data.read_json(terminal.PATH + "/bin/data/json");
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
            case 0:
            {
                switch(input)
                {
                    case 't':
                    {
                        data.game.id = terminal.get_string();
                        break;
                    }
                    case 'q':
                    {
                        if(in_array("a",{"a","test"}));
                        {
                            return 0;
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