#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <ctype.h>
#include <string.h>

#include "basson/color.h"
#include "basson/terminal.h"

#define MAX_ITERATION 100

Cursor *cursor;

void get_terminal_size();

/* -------------------- Cursor Operations --------------------  */

/**
 * This method initializes a variable for the Cursor struct
 * @return void
 * @author 1Pedro9
 */
void init_cursor()
{
    cursor = malloc(sizeof(Cursor));
    if (!cursor) return;

    cursor->col = 0,        cursor->row = 0;
    cursor->max_col = 0,    cursor->max_row = 0;
    cursor->type = 0;
    get_terminal_size();
}


/**
 * This method sets the cursor and the cursor struct's row and col to that of the parameters
 * @param row   - The y position of the cursor
 * @param col   - The x position of the cursor
 * @return void
 * @author 1Pedro9
 */
void set_cursor_pos(int row, int col)
{
    if (row < 1 || col < 0) {
        return;
    }
    
    if (cursor) {
        cursor->row = row;
        cursor->col = col;
    }
    
    fprintf(stdout, "\033[%d;%dH", row, col);
}


/**
 * This method relatively sets the cursor's position
 * @param row_rel   - This variable will take the current row position and add the row_rel to it
 * @param col_rel   - This variable will take the current column position and add the col_rel to it
 * @return void
 * @author 1Pedro9
 */
void set_cursor_pos_relative(int row_rel, int col_rel)
{
    set_cursor_pos(cursor->row + row_rel, cursor->col + col_rel);
}


/**
 * This function retrieves an integer value from a string
 * @param *file     - This is the file where to read from (i.e. stdin)
 * @return value    - The integer value retrieved
 * @author 1Pedro9
 */
int fgetint(FILE *file)
{
    int ch;
    int i = 0;
    int value = 0;
    int is_pos = 1;

    while ((ch = fgetc(file)) != EOF && i++ < MAX_ITERATION) {
        if (i == 1) {
            if (ch == '-') {
                is_pos = -1;
                continue;
            } else if (ch == '+') {
                is_pos = 1;
                continue;
            }
        }

        if (isdigit(ch)) {
            value *= 10;
            value += ch - 48;
        } else {
            break;
        }
    }

    return value * is_pos;
}


/**
 * The current cursor position is found and set to the cursor struct
 * @return void
 * @author 1Pedro9
 */
void get_cursor_pos()
{
    fprintf(stdout, "\033[6n");                         /* --- This request the terminal to give the cursor position --- */
    fgetc(stdin);                                       /* --- ESC char --- */
    fgetc(stdin);                                       /* --- [ char --- */
    cursor->row = fgetint(stdin);
    cursor->col = fgetint(stdin);
}

/**
 * This method changes the form of the cursor
 * @param type  - This is the 'enum' to set the cursor type
 *                  -> 1: Block cursor
 *                  -> 2: Thin cursor
 * @return void
 * @author 1Pedro9
 */
void set_cursor_type(int type)
{
    switch (type) {
        case 1:     printf("\033[2 q");     break;
        case 2:     printf("\033[6 q");     break;
        default:    break;
    }
}


/**
 * This method gets the terminal window's width and height
 * @return void
 * @author 1Pedro9
 */
void get_terminal_size()
{
    set_cursor_pos(999, 999);
    get_cursor_pos();
    cursor->max_row = cursor->row;
    cursor->max_col = cursor->col;
}


/**
 * This method returns the current cursor struct
 * @return *cursor  - The current cursor struct
 * @author 1Pedro9
 */
Cursor *get_cursor()
{
    return cursor;
}


/**
 * This method changes the terminal screen from normal to alternative and vise versa
 * @param type      - This is the type the terminal screen must change to
 * @return void
 * @author 1Pedro9
 */
void set_terminal_screen(int type)
{
    if (type == 0) fprintf(stdout, "\033[?1049l");
    else fprintf(stdout, "\033[?1049h");
}


/**
 * This method allows that the stdin sends data character by character and not line by line
 * @param *old      - This is the current mode of the terminal
 * @return void
 * @author 1Pedro9
 */
void enable_raw_mode(struct termios *old)
{
    struct termios raw;

    tcgetattr(STDIN_FILENO, old);
    raw = *old;

    raw.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}


/**
 * This method sets the stdin back to reading line by line
 * @param *old      - This is the struct to fall back to
 * @return void
 * @author 1Pedro9
 */
void disable_raw_mode(struct termios *old)
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, old);
}

/**
 * This method prints a string the exact length which specified
 * @param *text     - The string to print
 * @param length    - The exact length to print
 * @return void
 * @author 1Pedro9
 */
void print_exact_length(const char *text, int length) {
    if (strlen(text) >= length) {
        printf("%.*s", length, text);
    } else {
        printf("%-*s", length, text);
    }
}

/**
 * This method highlights the given text with a certain length
 * @param *text     - The string to print and highlight
 * @param length    - The length to print exactly
 * @return void
 * @author 1Pedro9
 */
void highlight(char *text, int length)
{
    printf(BG_GREEN);
    printf(COLOR_BLACK);

    print_exact_length(text, length);

    printf(RESET);
}

