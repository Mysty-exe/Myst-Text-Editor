#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cmath>
#include <algorithm>
#include <ctime>
#include <format>
#include <vector>
#include <curses.h>

using namespace std;

class StatusBar
/**
 * Class that represents a text editor
 */

{
private:
    string line, date;
    string state, findTxt, replaceTxt, filenameTxt, confirmTxt, terminalTxt;
    int width, height, statusHeight;
    int cursorX, cursorY, scroll;
    int startX, lineX;
    int matches;
    int currentCmd;
    bool error;
    bool modified;
    vector<string> terminalStack, cmdHistory;

public:
    WINDOW *statusPad;

    StatusBar();
    StatusBar(int w, int h);

    int getWidth();
    void setWidth(int w);
    int getStatusHeight() { return statusHeight; };
    int getCursorX();
    string getState();
    void setState(string s);
    string getFindTxt();
    string getReplaceTxt();
    void setMatches(int m);
    string getFilename();
    void setFilename(string filename);
    string getConfirm();

    void updateDimensions(int width, int height);
    bool endOfLine();
    void addCharacter(char character);
    void insertCharacter(char character);
    void backspace();
    void tab();
    void ctrlV();
    bool enter();
    void leftArrow();
    void rightArrow();
    string getDateString();
    void getDate();
    void displayDate();
    void setInfo(string text, bool err);
    void update(bool forceUpdate = false);
    void findAndReplace();
    void resetStatus();
    void saveAs();
    void confirm();
    void terminal();
    void runCommand(std::string dir);
    void previousCmd();
    void nextCmd();
    void scrollUpTerminal();
    void scrollDownTerminal();
    void clear();
};