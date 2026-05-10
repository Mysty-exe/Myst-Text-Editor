#include "menu.h"

Menu::Menu()
/**
Menu Class Constructor

Returns:
    void
 */

{
}

Menu::Menu(int w, int h, Settings &settings)
/**
Menu Class Constructor

Vars:
    (string) fact: random fact taken from file
    (int) maxHeight: Max Height of the editor
    (int) width: width of the editor
    (int) height: height of the editor
    (int) scroll: How scrolled the text editor is
    (int) currentMenu: current menu index
    (string) currentDirectory: current directory
    (vector<Button>) settingsButtons: Buttons vector in settings menu
    (vector<Button>) switchButtons: Buttons vector in switch file menu
    (vector<Button>) preferencesButtons: Buttons vector in preferences menu
    (ButtonsList) settingsList: ButtonsList for settings menu
    (ButtonsList) switchButtons: ButtonsList for switch file menu
    (ButtonsList) preferencesButtons: ButtonsList for preferences menu
    (WINDOW*) menuPad: Pad for the menu

Returns:
    void
 */

{
    fact = "";
    width = w;
    height = h;
    scroll = 0;
    currentMenu = 0;
    currentDirectory = filesystem::current_path();
    for (int i = currentDirectory.size(); i >= 0; i--)
    {
        if (currentDirectory[i] == '.')
        {
            currentDirectory = currentDirectory.substr(0, currentDirectory.find_last_of("/"));
            break;
        }
        if (currentDirectory[i] == '/')
            break;
    }

    settingsButtons = {Button("Back"), Button("Save"), Button("Save As"), Button("Switch File"), Button("Preferences"), Button("Quit")};
    switchButtons = {};
    preferencesButtons = {Button("Back"), Button("Line Numbers", false, settings.getLineNumbers()), Button("Tabsize", false, settings.getTabSize()), Button("Programming", false, settings.getProgrammingMode()), Button("Colour", false, settings.getColorScheme())};

    settingsList.newButtonList(settingsButtons);
    switchList.newButtonList(switchButtons);
    preferencesList.newButtonList(preferencesButtons);

    filesPad = newpad(height - 10, width - 20);
    menu = newwin(height - 10, width - 20, 5, 10);
    menuPanel = new_panel(menu);
}

int Menu::getCurrentMenu()
/**
currentMenu Getter Function

Returns:
    int
 */

{
    return currentMenu;
}

void Menu::setCurrentMenu(int current)
/**
currentMenu Setter Function

Args:
    (int) current: New currentMenu

Returns:
    void
 */

{
    currentMenu = current;
}

int Menu::getWidth()
/**
width Getter Function

Returns:
    int
 */

{
    return width;
}

void Menu::setWidth(int w)
/**
width Setter Function

Args:
    (int) w: New width

Returns:
    void
 */

{
    width = w;
}

int Menu::getHeight()
/**
height Getter Function

Returns:
    int
 */

{
    return height;
}

void Menu::setHeight(int h)
/**
height Setter Function

Args:
    (int) h: New height

Returns:
    void
 */

{
    height = h;
}

int Menu::getScroll()
/**
scroll Getter Function

Returns:
    int
 */

{
    return scroll;
}

void Menu::setScroll(int s)
/**
scroll Setter Function

Args:
    (int) s: New scroll

Returns:
    void
 */

{
    scroll = s;
}

string Menu::getCurrentDirectory()
/**
currentDirectory Getter Function

Returns:
    string
 */

{
    return currentDirectory;
}

void Menu::setCurrentDirectory(string dir)
/**
currentDirectory Setter Function

Args:
    (string) dir: New currentDirectory

Returns:
    void
 */

{
    error_code ec;
    if (!filesystem::is_directory(dir, ec))
        dir = dir.substr(0, dir.find_last_of("/"));

    currentDirectory = dir;
}

void Menu::updateDimensions(int width, int height)
/**
Updates dimensions if terminal size changes

Returns:
    void
 */

{
    this->width = width;
    this->height = height;

    wbkgd(menu, COLOR_PAIR(1));
    werase(menu);
    update_panels();
    doupdate();

    delwin(filesPad);
    del_panel(menuPanel);
    delwin(menu);
    menu = newwin(height - 10, width - 20, 5, 10);
    menuPanel = new_panel(menu);

    filesPad = newpad(height - 10, width - 20);
    wbkgd(filesPad, COLOR_PAIR(1));

    prefresh(filesPad,
             0, 0,
             5, 10,
             5 + (height - 10), 10 + longestFile);

    keypad(menu, true);
    // update_panels();
    // doupdate();
}

vector<Button> Menu::getButtons()
/**
Gets the vetor of buttons based on the current menu

Returns:
    vector<Button>
 */

{
    if (currentMenu == 0)
    {
        return settingsList.getButtons();
    }
    else if (currentMenu == 1)
    {
        return switchList.getButtons();
    }
    return preferencesList.getButtons();
}

void Menu::setButtons(vector<Button> buttons)
/**
buttons Setter Function

Args:
    (vector<Button>) buttons: New buttons

Returns:
    void
 */

{
    if (currentMenu == 0)
    {
        settingsList.setButtons(buttons);
    }
    else if (currentMenu == 2)
    {
        preferencesList.setButtons(buttons);
    }
}

void Menu::setFileButtons(string directory, string currentFile)
/**
Sets the buttons of the files based on the directory

Args:
    (string) directory: directory to get all the files from

Returns:
    void
 */

{
    numFiles = 0;
    longestFile = width / 6;
    switchButtons = {};
    error_code ec;
    if (filesystem::is_regular_file(directory, ec))
    {
        directory = std::filesystem::path(directory).parent_path();
    }
    for (auto &p : filesystem::directory_iterator(directory))
    {
        numFiles++;
        int l = 0;
        for (int i = p.path().string().size() - 1; i > 0; i--)
        {
            if (p.path().string()[i] != '/')
                l++;
            else
                break;
        }

        if (currentFile == p.path().string().substr(p.path().string().size() - l, l))
            switchList.setCurrentButton(numFiles - 1);

        if (l > longestFile)
            longestFile = (l > width - 20) ? width - 20 : l;

        switchButtons.push_back(Button(p.path(), p.is_directory()));
    }
    if (switchButtons.size() == 0)
    {
        switchButtons.push_back(Button("No Files In Folder"));
    }

    switchList.setButtons(switchButtons);

    delwin(filesPad);
    filesPad = newpad(numFiles + 10, width - 20);

    longestFile += 10;
}

void Menu::displayButtons()
/**
Displays all the buttons

Returns:
    void
 */

{
    if (currentMenu == 0)
    {
        settingsList.displayButtons(menu, height);
    }
    else if (currentMenu == 1)
    {
        switchList.displayFiles(filesPad);
    }
    else if (currentMenu == 2)
    {
        preferencesList.displayButtons(menu, height);
    }
}

vector<string> Menu::getMenuText(Editor &editor)
/**
Gets menu text

Returns:
    vector<string>
 */

{
    vector<string> text;
    string filename = (editor.getFile().getName() == "") ? "[Untitled File]" : editor.getFile().getName();
    text.push_back("File Information");
    text.push_back("File: " + filename);
    text.push_back("Lines: " + to_string(editor.getFile().getLines().size()));
    text.push_back("Col: " + to_string(editor.getCursorX() + 1));
    text.push_back("");
    text.push_back("Last Saved: " + editor.getLastSaved());
    text.push_back("");
    text.push_back("Shortcuts:");
    text.push_back("Ctrl-S   -   Save");
    text.push_back("Ctrl-F   -   Find/Replace");
    text.push_back("Ctrl-B   -   Previous File");
    text.push_back("Ctrl-N   -   Next File");
    text.push_back("Ctrl-E   -   Settings/Menu");
    text.push_back("Ctrl-T   -   Switch File");
    text.push_back("Ctrl-P   -   Preferences");
    text.push_back("Ctrl-Q   -   Quit");
    // text.push_back("");
    // text.push_back("Did You Know?");
    // text.push_back(fact);
    return text;
}

vector<string> Menu::getPreferencesText(Settings &settings)
/**
Gets preferences text

Returns:
    vector<string>
 */

{
    vector<string> text;
    text.push_back(string("1. Line Numbers ") + "[" + settings.getLineNumbers() + "]");
    text.push_back("Show or hide line numbers in the text editor.");
    text.push_back("Useful for coding or navigating large files.");
    text.push_back("");
    text.push_back(string("2. Tab Size ") + "[" + settings.getTabSize() + "]");
    text.push_back("Set the number of spaces for each tab.");
    text.push_back("Choose a size that suits your workflow.");
    text.push_back("");
    text.push_back(string("3. Programming Mode ") + "[" + settings.getProgrammingMode() + "]");
    text.push_back("Enable features like autocomplete and autotab.");
    text.push_back("Ideal for editing code.");
    text.push_back("");
    text.push_back(string("4. Colour Scheme ") + "[" + settings.getColorScheme() + "]");
    text.push_back("Select the editor's colour scheme.");
    return text;
}

void Menu::displayText(vector<string> texts)
/**
Display text in the menu

Returns:
    void
 */

{
    int line = height / 2 - 12;
    for (string &text : texts)
    {
        if (currentMenu == 0)
        {
            if (line == (height / 2 - 12) || line == (height / 2 - 12) + 5 || line == (height / 2 - 12) + 7 || line == (height / 2 - 12) + 17)
            {
                wattron(menu, COLOR_PAIR(1));
            }
        }
        else if (currentMenu == 2)
        {
            if ((line - (height / 2 - 12)) % 4 == 0)
            {
                wattron(menu, COLOR_PAIR(1));
            }
        }
        mvwprintw(menu, line, width / 2, text.c_str(), "%s");
        wattroff(menu, COLOR_PAIR(1));
        line += 1;
    }
}

void Menu::upArrow()
/**
Handles when up arrow is pressed

Returns:
    void
 */

{
    if (currentMenu == 0)
    {
        settingsList.upArrow(scroll);
    }
    else if (currentMenu == 1)
    {
        switchList.upArrow(scroll);
    }
    else if (currentMenu == 2)
    {
        preferencesList.upArrow(scroll);
    }
}

void Menu::downArrow()
/**
Handles when down arrow is pressed

Returns:
    void
 */

{
    if (currentMenu == 0)
    {
        settingsList.downArrow(height, scroll);
    }
    else if (currentMenu == 1)
    {
        switchList.downArrow(height - 20, scroll);
    }
    else if (currentMenu == 2)
    {
        preferencesList.downArrow(height, scroll);
    }
}

int Menu::enter(File &file)
/**
Handles when enter is pressed

Returns:
    void
 */

{
    if (currentMenu == 0)
    {
        return settingsList.enter();
    }
    else if (currentMenu == 1)
    {
        try
        {
            int num = switchList.enter();
            error_code ec;
            const filesystem::path path(switchList.getButtons()[num].getText());
            if (filesystem::is_directory(path, ec))
            {
                switchList.setCurrentButton(0);
                scroll = 0;
                currentDirectory = switchList.getButtons()[num].getText();
                try
                {
                    setFileButtons(path);
                }
                catch (const std::exception &e)
                {
                    currentDirectory = currentDirectory.substr(0, currentDirectory.find_last_of("/"));
                }
            }
            else if (filesystem::is_regular_file(path, ec))
            {
                if (File::overwrite(path))
                    file.open(path);

                return 1;
            }
            return 0;
        }
        catch (exception &e)
        {
            return 1;
        }
    }

    return preferencesList.enter();
}

void Menu::back()
/**
Handles when back arrow is pressed in the switch file menu

Returns:
    void
 */

{
    currentDirectory = currentDirectory.substr(0, currentDirectory.find_last_of("/"));
    error_code ec;
    const filesystem::path path(currentDirectory);
    if (filesystem::is_directory(path, ec))
    {
        switchList.setCurrentButton(0);
        scroll = 0;
        setFileButtons(path);
    }
}

void Menu::resetPad()
/**
Resets the pad

Returns:
    void
 */

{
    currentMenu = 0;
    scroll = 0;
    settingsList.setCurrentButton(0);
    switchList.setCurrentButton(0);
    preferencesList.setCurrentButton(0);
}
