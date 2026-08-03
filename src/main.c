#include <ncurses.h>

int main(void) {
    initscr();                  // start ncurses mode
    cbreak();                   // disable line buffering
    noecho();                   // don't echo typed characters
    keypad(stdscr, TRUE);       // enable function keys / arrows

    printw("Hello from Roxy + ncurses!\n");
    printw("Press any key to exit...");
    refresh();                  // show the text

    getch();                    // wait for a key

    endwin();                   // restore terminal
    return 0;
}
