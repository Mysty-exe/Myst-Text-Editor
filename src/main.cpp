#include <ncurses.h>
#include <iostream>
#include <filesystem>
#include <unistd.h>
#include <limits.h>
#include <string>
#include <cstring>
#include "app.h"

std::filesystem::path getExecutableDir()
{
    char buffer[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);

    if (len != -1)
    {
        buffer[len] = '\0';
        return std::filesystem::path(buffer).parent_path();
    }

    return std::filesystem::current_path();
}

std::string getProjectPath()
{
    auto base = getExecutableDir();
    auto projectRoot = base.parent_path();
    return projectRoot;
}

int main(int argc, char **argv)
{
    std::string projectPath = getProjectPath();
    if (projectPath.empty())
    {
        std::cout << "Project path is not set in the environment variable." << std::endl;
    }

    cout << "\033]0;Myst Editor\007";

    // Setup ncurses
    initscr();

    refresh();

    int w, h;
    getmaxyx(stdscr, h, w);

    noecho();
    cbreak();
    raw();
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);

    use_default_colors();
    start_color();
    set_escdelay(25);

    App app(w, h, projectPath);
    app.init();

    if (argc < 3)
    {
        // Get file from arguments passed when program is run
        string file = argc > 1 ? argv[1] : "";
        if (file == "-help")
        {
            endwin();
            string text;
            fstream readFile(projectPath + "/resources/help.txt");
            while (getline(readFile, text))
            {
                cout << text << endl;
            }
        }

        else if (File::fileError(file) && file != "")
        {
            cout << "Unknown File Location." << endl;
        }

        else
        {
            app.displayLogo();
            app.stateLoop(file);
        }
    }
    else
    {
        endwin();
        cout << "Unknown Command." << endl;
    }

    endwin();

    printf("\033[?1003l\n");

    return 0;
}
