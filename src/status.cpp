#include "status.h"

StatusBar::StatusBar()
/**
Editor Class Constructor

Vars:
    (int) width: Width of the command line
    (int) height: Height of the command line
    (int) cursorX: X-Index that the cursor is on in the command line
    (int) cursorY: Y-Index that the cursor is on in the command line (always zero)
    (string) line: Error being displayed in the command line
    (string) cmdTxt: Command being written into the command line
    (WINDOW*) commandWindow: Window for the command line

Returns:
    void
 */

{
}

StatusBar::StatusBar(int w, int h)
/**
Editor Class Constructor

Vars:
    (int) width: Width of the command line
    (int) height: Height of the command line
    (int) cursorX: X-Index that the cursor is on in the command line
    (int) cursorY: Y-Index that the cursor is on in the command line (always zero)
    (string) line: Error being displayed in the command line
    (string) cmdTxt: Command being written into the command line
    (WINDOW*) commandWindow: Window for the command line

Returns:
    void
 */

{
    width = w;
    height = h;
    cursorX = cursorY = 0;
    startX = lineX = 0;
    line = "";
    date = "";
    state = "";
    findTxt = "";
    replaceTxt = "";
    filenameTxt = "";
    confirmTxt = "";
    terminalTxt = "";
    matches = 0;
    error = false;
    modified = false;
    statusPad = newpad(100, width);
    statusHeight = 1;
    scroll = 0;
    currentCmd = 0;
    cmdHistory.push_back("");
}

int StatusBar::getWidth()
/**
Width Getter Function

Returns:
    int
 */

{
    return width;
}

void StatusBar::setWidth(int w)
/**
Width Setter Function

Args:
    (int) w: New width of the command line

Returns:
    void
 */

{
    width = w;
}

int StatusBar::getCursorX()
/**
CursorX Getter Function

Returns:
    int
 */

{
    return cursorX;
}

string StatusBar::getState()
/**
state Getter Function

Returns:
    string
 */

{
    return state;
}

void StatusBar::setState(string s)
/**
state Setter Function

Args:
    (string) s: New state of the command line

Returns:
    void
 */

{
    wbkgd(statusPad, COLOR_PAIR(1));
    werase(statusPad);
    wrefresh(statusPad);
    if (s == "terminal")
    {
        statusHeight = 6;
        scroll = terminalStack.size() + 1;
    }
    state = s;
}

string StatusBar::getFindTxt()
/**
findTxt Getter Function

Returns:
    string
 */

{
    return findTxt;
}

string StatusBar::getReplaceTxt()
/**
replaceTxt Getter Function

Returns:
    string
 */

{
    return replaceTxt;
}

void StatusBar::setMatches(int m)
/**
matches Setter Function

Args:
    (int) m: New m

Returns:
    void
 */

{
    matches = m;
}

string StatusBar::getFilename()
/**
CursorX Getter Function

Returns:
    int
 */

{
    return filenameTxt;
}

void StatusBar::setFilename(string filename)
/**
filename Setter Function

Args:
    (string) filename: New filename

Returns:
    void
 */

{
    filenameTxt = filename;
}

string StatusBar::getConfirm()
/**
confirm Getter Function

Returns:
    string
 */

{
    return confirmTxt;
}

void StatusBar::updateDimensions(int width, int height)
/**
Updates size of window if terminal has been changed

Returns:
    void
 */

{
    this->width = width;
    this->height = height;

    delwin(statusPad);
    statusPad = newpad(100, width);

    wbkgd(statusPad, COLOR_PAIR(1));
    keypad(statusPad, true);
    prefresh(statusPad, 0, 0, height - statusHeight, 0, height - 1, width - 1);

    if (state == "find")
    {
        cursorX = min((int)findTxt.size(), width / 3);
        lineX = findTxt.size();
        startX = 0;
    }
    else if (state == "replace")
    {
        cursorX = min((int)replaceTxt.size(), width / 3);
        lineX = replaceTxt.size();
        startX = 0;
    }
    else if (state == "save as")
    {
        cursorX = min((int)filenameTxt.size(), width - 15);
        lineX = filenameTxt.size();
        startX = 0;
    }
}

bool StatusBar::endOfLine()
/**
Checks if cursor is at the end of the line

Returns:
    bool
 */

{
    if (state == "find")
    {
        return lineX >= (int)findTxt.length();
    }
    else if (state == "replace")
    {
        return lineX >= (int)replaceTxt.length();
    }
    else if (state == "terminal")
    {
        return lineX >= (int)terminalTxt.length();
    }
    else if (state == "save as")
    {
        return lineX >= (int)filenameTxt.length();
    }
    else if (state == "save" || state == "quit")
    {
        return cursorX < 1;
    }

    return false;
}

void StatusBar::addCharacter(char character)
/**
Adds a character to the command line

Args:
    (char) character: Character you want to add

Returns:
    void
 */

{
    if (state == "find")
    {
        lineX += 1;
        cursorX += 1;
        findTxt = findTxt + character;
        if (cursorX > width / 3)
            cursorX = width / 3;
    }
    else if (state == "replace")
    {
        lineX += 1;
        cursorX += 1;
        replaceTxt = replaceTxt + character;
        if (cursorX > width / 3)
            cursorX = width / 3;
    }
    else if (state == "terminal")
    {
        lineX += 1;
        cursorX += 1;
        terminalTxt = terminalTxt + character;
        if (cursorX > width - 10)
            cursorX = width - 10;
    }
    else if (state == "save as")
    {
        lineX += 1;
        cursorX += 1;
        filenameTxt = filenameTxt + character;
        if (cursorX > (int)width - 15)
            cursorX = width - 15;
    }
    else if (state == "quit" || state == "save")
    {
        cursorX += 1;
        confirmTxt = character;
    }
}

void StatusBar::insertCharacter(char character)
/**
Insert a character in between two characters in the command line

Args:
    (char) character: Character to insert

Returns:
    void
 */

{
    if (state == "find")
    {
        findTxt = findTxt.substr(0, lineX) + character + findTxt.substr(lineX, findTxt.length());
        lineX += 1;
        if ((int)findTxt.size() <= (int)width / 3)
            cursorX += 1;
        if (cursorX == 0)
        {
            leftArrow();
            rightArrow();
        }
    }
    else if (state == "replace")
    {
        replaceTxt = replaceTxt.substr(0, lineX) + character + replaceTxt.substr(lineX, replaceTxt.length());
        lineX += 1;
        if ((int)replaceTxt.size() <= (int)width / 3)
            cursorX += 1;
        if (cursorX == 0)
        {
            leftArrow();
            rightArrow();
        }
    }
    else if (state == "terminal")
    {
        terminalTxt = terminalTxt.substr(0, lineX) + character + terminalTxt.substr(lineX, terminalTxt.length());
        lineX += 1;
        if ((int)terminalTxt.size() <= (int)width - 10)
            cursorX += 1;
        if (cursorX == 0)
        {
            leftArrow();
            rightArrow();
        }
    }
    else if (state == "save as")
    {
        filenameTxt = filenameTxt.substr(0, lineX) + character + filenameTxt.substr(lineX, filenameTxt.length());
        lineX += 1;
        if ((int)filenameTxt.size() <= (int)width - 15)
            cursorX += 1;
        if (cursorX == 0)
        {
            leftArrow();
            rightArrow();
        }
    }
}

void StatusBar::backspace()
/**
Deletes a character in the command line

Returns:
    void
 */

{
    if (state == "find")
    {
        if (lineX > 0)
        {
            lineX -= 1;
            findTxt = findTxt.erase(lineX, 1);
            if (cursorX > 0 && (int)findTxt.size() < (int)width / 3)
                cursorX -= 1;
        }
    }
    else if (state == "replace")
    {
        if (lineX > 0)
        {
            lineX -= 1;
            replaceTxt = replaceTxt.erase(lineX, 1);
            if (cursorX > 0 && (int)replaceTxt.size() < (int)width / 3)
                cursorX -= 1;
        }
    }
    else if (state == "terminal")
    {
        if (lineX > 0)
        {
            lineX -= 1;
            terminalTxt = terminalTxt.erase(lineX, 1);
            if (cursorX > 0 && (int)terminalTxt.size() < (int)width - 10)
                cursorX -= 1;
        }
    }
    else if (state == "save as")
    {
        if (lineX > 0)
        {
            lineX -= 1;
            filenameTxt = filenameTxt.erase(lineX, 1);
            if (cursorX > 0 && (int)filenameTxt.size() < (int)width - 15)
                cursorX -= 1;
        }
    }
    else if (state == "quit" || state == "save")
    {
        if (cursorX > 0)
        {
            cursorX -= 1;
            confirmTxt = confirmTxt.erase(cursorX, 1);
        }
    }
}

void StatusBar::tab()
/**
Handles if tab is pressed

Returns:
    void
 */

{
    if (state == "find")
    {
        cursorX = min((int)replaceTxt.size(), width / 3);
        lineX = replaceTxt.size();
        startX = 0;

        state = "replace";
    }
    else if (state == "replace")
    {
        cursorX = min((int)findTxt.size(), width / 3);
        lineX = findTxt.size();
        startX = 0;

        state = "find";
    }
}

void StatusBar::ctrlV()
{
    if (state == "quit")
        return;

    FILE *pipe = popen("xclip -selection clipboard -o", "r");
    if (pipe)
    {
        char buffer[128];
        string result;
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
        {
            result += buffer;
        }

        pclose(pipe);

        for (int x = 0; x < (int)result.length(); x++)
        {
            if (result[x] == '\n')
            {
                addCharacter(' ');
            }
            else if (result[x] != '\r')
            {
                if (endOfLine())
                    addCharacter(result[x]);
                else
                    insertCharacter(result[x]);
            }
        }
    }
}

bool StatusBar::enter()
/**
Passes the command to "parseCommand()" and deletes it

Returns:
    void
 */

{
    if (state == "replace")
    {
        return true;
    }
    return false;
}

void StatusBar::leftArrow()
/**
Moves cursor to the left if its not already at the beginning of the command line

Returns:
    void
 */

{
    if (lineX > 0)
    {
        lineX--;
        cursorX--;
        if (cursorX < 0)
        {
            startX += 1;
            cursorX = 0;
        }
    }
}

void StatusBar::rightArrow()
/**
Moves cursor to the right of the lin if cursor isn't already at the end of the command line

Returns:
    void
 */

{
    if (state == "find")
    {
        if (lineX <= (int)findTxt.length() - 1)
        {
            lineX++;
            cursorX++;
            if (cursorX > width / 3)
            {
                startX -= 1;
                cursorX = width / 3;
            }
        }
    }
    else if (state == "replace")
    {
        if (lineX <= (int)replaceTxt.length() - 1)
        {
            lineX++;
            cursorX++;
            if (cursorX > width / 3)
            {
                startX -= 1;
                cursorX = width / 3;
            }
        }
    }
    else if (state == "terminal")
    {
        if (lineX <= (int)terminalTxt.length() - 1)
        {
            lineX++;
            cursorX++;
            if (cursorX > width - 10)
            {
                startX -= 1;
                cursorX = width - 10;
            }
        }
    }
    else if (state == "save as")
    {
        if (lineX <= (int)filenameTxt.length() - 1)
        {
            lineX++;
            cursorX++;
            if (cursorX > width / 3)
            {
                startX -= 1;
                cursorX = width / 3;
            }
        }
    }
}

string StatusBar::getDateString()
/**
date Getter Function

Returns:
    string
 */

{
    return date;
}

void StatusBar::getDate()
/**
Gets the current date

Returns:
    void
 */

{
    std::time_t currentTime = std::time(nullptr);
    std::tm *localTime = std::localtime(&currentTime);
    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%A %d, %Y %H:%M:%S", localTime);
    if (date != string(buffer))
    {
        modified = true;
        date = string(buffer);
    }
}

void StatusBar::displayDate()
/**
Display the date

Returns:
    void
 */

{
    curs_set(0);
    mvwprintw(statusPad, statusHeight - 1, width - (int)date.length(), date.c_str(), "%s");
}

void StatusBar::setInfo(string text, bool err = false)
/**
Displays an info message with colours in the command line

Args:
    (string) text: Text to display in the info message

Returns:
    void
 */

{
    error = err;
    if (line != text)
    {
        modified = true;
        line = text;
    }
}

void StatusBar::update(bool forceUpdate)
/**
Updates the statusbar

Returns:
    void
 */

{
    leaveok(statusPad, true);

    getDate();
    if (modified || forceUpdate)
    {
        werase(statusPad);
        wrefresh(statusPad);

        modified = false;
        displayDate();
        if (error)
            wattron(statusPad, COLOR_PAIR(12));
        else
            wattron(statusPad, COLOR_PAIR(1));

        mvwprintw(statusPad, statusHeight - 1, 0, line.c_str(), "%s");
        wattroff(statusPad, COLOR_PAIR(1));
        wattroff(statusPad, COLOR_PAIR(12));

        prefresh(statusPad, 0, 0, height - statusHeight, 0, height - 1, width - 1);
    }
}

void StatusBar::findAndReplace()
/**
Handles when ctrl-f is pressed

Returns:
    void
 */

{
    leaveok(statusPad, false);

    clear();

    wmove(statusPad, statusHeight - 1, 0);
    wattron(statusPad, COLOR_PAIR(1));
    wprintw(statusPad, "Find: ");

    wattroff(statusPad, COLOR_PAIR(1));
    if ((int)findTxt.size() > width / 3)
        wprintw(statusPad, findTxt.substr((int)findTxt.size() - (width / 3) - startX, width / 3).c_str(), "%s");
    else
        wprintw(statusPad, findTxt.c_str(), "%s");
    wprintw(statusPad, "     ");

    wattron(statusPad, COLOR_PAIR(1));
    wprintw(statusPad, "Replace: ");

    wattroff(statusPad, COLOR_PAIR(1));
    if ((int)replaceTxt.size() > width / 3)
        wprintw(statusPad, replaceTxt.substr((int)replaceTxt.size() - (width / 3) - startX, width / 3).c_str(), "%s");
    else
        wprintw(statusPad, replaceTxt.c_str(), "%s");

    wattron(statusPad, COLOR_PAIR(1));
    mvwprintw(statusPad, statusHeight - 1, width - (9 + to_string(matches).length()), "Matches: ");

    wattroff(statusPad, COLOR_PAIR(1));
    mvwprintw(statusPad, statusHeight - 1, width - to_string(matches).length(), to_string(matches).c_str(), "%s");

    if (state == "find")
    {
        wmove(statusPad, statusHeight - 1, cursorX + 6);
    }
    else if (state == "replace")
    {
        wmove(statusPad, statusHeight - 1, cursorX + min((int)findTxt.size(), width / 3) + 20);
    }
    prefresh(statusPad, 0, 0, height - statusHeight, 0, height - 1, width - 1);
}

void StatusBar::resetStatus()
/**
Resets the status bar

Returns:
    void
 */

{
    state = "";
    findTxt = "";
    replaceTxt = "";
    filenameTxt = "";
    confirmTxt = "";
    terminalTxt.clear();
    terminalStack.clear();
    cmdHistory.clear();
    currentCmd = 0;
    matches = 0;
    cursorX = 0;
    lineX = 0;
    startX = 0;
    statusHeight = 1;
    cmdHistory.push_back("");
}

void StatusBar::saveAs()
/**
Save as function to save as a new file in statusbar

Returns:
    void
 */

{
    leaveok(statusPad, false);

    clear();
    wmove(statusPad, statusHeight - 1, 0);
    wattron(statusPad, COLOR_PAIR(1));
    wprintw(statusPad, "File Name: ");
    wattroff(statusPad, COLOR_PAIR(1));
    if ((int)filenameTxt.size() > (int)width - 15)
        mvwprintw(statusPad, statusHeight - 1, 11, filenameTxt.substr((int)filenameTxt.size() - (width - 15) - startX, width - 15).c_str(), "%s");
    else
        mvwprintw(statusPad, statusHeight - 1, 11, filenameTxt.c_str(), "%s");

    wmove(statusPad, statusHeight - 1, cursorX + 11);

    prefresh(statusPad, 0, 0, height - statusHeight, 0, height - 1, width - 1);
}

void StatusBar::confirm()
/**
confirm function to confirm an action in statusbar

Returns:
    void
 */

{
    leaveok(statusPad, false);

    clear();
    wattron(statusPad, COLOR_PAIR(1));
    if (state == "quit")
    {
        wmove(statusPad, statusHeight - 1, 0);
        wprintw(statusPad, "Quit Without Saving? (y/n): ");
        wattroff(statusPad, COLOR_PAIR(1));
        mvwprintw(statusPad, statusHeight - 1, 28, confirmTxt.c_str(), "%s");
        wmove(statusPad, statusHeight - 1, cursorX + 28);
    }
    else if (state == "save")
    {
        wmove(statusPad, statusHeight - 1, 0);
        wprintw(statusPad, "Overwrite File? (y/n): ");
        wattroff(statusPad, COLOR_PAIR(1));
        mvwprintw(statusPad, statusHeight - 1, 23, confirmTxt.c_str(), "%s");
        wmove(statusPad, statusHeight - 1, cursorX + 23);
    }

    prefresh(statusPad, 0, 0, height - statusHeight, 0, height - 1, width - 1);
}

void StatusBar::terminal()
{
    leaveok(statusPad, false);

    statusHeight = 6;

    clear();
    int i = 0;
    for (auto &l : terminalStack)
    {
        mvwprintw(statusPad, statusHeight + i, 0, l.c_str(), "%s");
        i++;
    }
    mvwprintw(statusPad, statusHeight + i, 0, "> ");
    if ((int)terminalTxt.size() > width - 10)
        wprintw(statusPad, terminalTxt.substr((int)terminalTxt.size() - (width - 10) - startX, width - 10).c_str(), "%s");
    else
        wprintw(statusPad, terminalTxt.c_str(), "%s");

    wmove(statusPad, statusHeight + i, cursorX + 2);

    if ((int)terminalStack.size() >= statusHeight)
    {
        scroll = (scroll < statusHeight) ? statusHeight : scroll;
        scroll = ((scroll - statusHeight) > ((int)(terminalStack.size() + 1) - statusHeight)) ? terminalStack.size() + 1 : scroll;
    }

    prefresh(statusPad, scroll, 0, height - statusHeight, 0, height - 1, width - 1);
}

void StatusBar::runCommand(std::string dir)
{
    error_code ec;
    if (!filesystem::is_directory(dir, ec))
        dir = dir.substr(0, dir.find_last_of("/"));

    for (int i = 0; i < (1 + floor((int)terminalTxt.size() / (int)(width - 10))); i++)
    {
        if (i == 0)
            terminalStack.push_back("> " + terminalTxt.substr(0 + (width - 10) * i, width - 10));
        else
            terminalStack.push_back("  " + terminalTxt.substr(0 + (width - 10) * i, width - 10));
    }

    cmdHistory.pop_back();
    cmdHistory.push_back(terminalTxt);
    cmdHistory.push_back("");

    if (terminalTxt == "clear" || terminalTxt == "cls")
    {
        terminalStack.clear();
        scroll = terminalStack.size() + 1;
        terminalTxt = "";
        cursorX = 0;
        startX = 0;
        lineX = 0;
        return;
    }

    currentCmd = cmdHistory.size() - 1;
    std::string copyCmd = "cd " + dir + " && " + terminalTxt;
    FILE *pipe = popen((copyCmd + " 2>&1").c_str(), "r");

    if (!pipe)
    {
        terminalStack.push_back("Failed to start command");
        return;
    }
    else
    {
        char buffer[256];

        while (fgets(buffer, sizeof(buffer), pipe))
        {
            std::string cmd(buffer);
            for (int i = 0; i < (1 + floor((int)cmd.size() / (int)(width - 10))); i++)
                terminalStack.push_back(cmd.substr(0 + (width - 10) * i, width - 10));
        }
    }

    pclose(pipe);

    scroll = terminalStack.size() + 1;
    terminalTxt = "";
    cursorX = 0;
    startX = 0;
    lineX = 0;
}

void StatusBar::previousCmd()
{
    currentCmd = (currentCmd > 0) ? currentCmd - 1 : 0;
    terminalTxt = cmdHistory[currentCmd];
    cursorX = min((int)terminalTxt.size(), width - 10);
    lineX = terminalTxt.size();
    startX = 0;
}

void StatusBar::nextCmd()
{
    currentCmd = (currentCmd < (int)cmdHistory.size() - 1) ? currentCmd + 1 : cmdHistory.size() - 1;
    terminalTxt = cmdHistory[currentCmd];
    cursorX = min((int)terminalTxt.size(), width - 10);
    lineX = terminalTxt.size();
    startX = 0;
}

void StatusBar::scrollUpTerminal()
{
    if ((int)terminalStack.size() >= statusHeight)
    {
        scroll -= 1;
    }
}

void StatusBar::scrollDownTerminal()
{
    if ((int)terminalStack.size() >= statusHeight)
    {
        scroll += 1;
    }
}

void StatusBar::clear()
/**
Clears all commands from the command line

Returns:
    void
 */

{
    werase(statusPad);
}
