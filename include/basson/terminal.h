
#ifndef TERMINAL_H
#define TERMINAL_H

/**
 * col, row => The position of the cursor in the terminal
 * max_*    => The maximum position that the cursor can go in the current window size
 * type     => The type of cursor
 *              -> 01 = Block   [Solid]
 *              -> 02 = Line    [Solid]
 * @author 1Pedro9
 */
typedef struct {
    int col, row;
    int max_col, max_row;
    int type;
} Cursor;

void init_cursor();
void set_cursor_pos(int row, int col);
void get_cursor_pos();
Cursor *get_cursor();
void set_cursor_type(int type);

void enable_raw_mode();
void disable_raw_mode();
void set_terminal_screen(int type);
void highlight(char *text, int length);
void print_exact_length(const char *text, int length);

#endif
