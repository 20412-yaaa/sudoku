#include <iostream>
#include <windows.h>

using namespace std;

int board[4][4] = {
    {0, 0, 1, 0},
    {0, 4, 0, 0},
    {0, 0, 2, 0},
    {0, 1, 0, 0},
};

bool editable[4][4];

int cur_r = 0, cur_c = 0;

bool check_horizontal_conflict(int row)
{
    bool seen[5] = {false}; // ?¦r­S?¬O 1-4
    for (int col = 0; col < 4; ++col)
    {
        if (board[row][col] != 0)
        {
            if (seen[board[row][col]])
                return true;
            seen[board[row][col]] = true;
        }
    }
    return false;
}

bool check_vertical_conflict(int col)
{
    bool seen[5] = {false};
    for (int row = 0; row < 4; ++row)
    {
        if (board[row][col] != 0)
        {
            if (seen[board[row][col]])
                return true;
            seen[board[row][col]] = true;
        }
    }
    return false;
}

bool check_block_conflict(int start_row, int start_col)
{
    bool seen[5] = {false};
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            int val = board[start_row + i][start_col + j];
            if (val != 0)
            {
                if (seen[val])
                    return true;
                seen[val] = true;
            }
        }
    }
    return false;
}

void check_horizontal()
{
    for (int row = 0; row < 4; ++row)
    {
        if (check_horizontal_conflict(row))
            return;
    }
}

void check_vertical()
{
    for (int col = 0; col < 4; ++col)
    {
        if (check_vertical_conflict(col))
            return;
    }
}

void check_block()
{
    for (int i = 0; i < 4; i += 2)
    {
        for (int j = 0; j < 4; j += 2)
        {
            if (check_block_conflict(i, j))
                return;
        }
    }
}

bool is_invalid(int i, int j)
{
    return check_horizontal_conflict(i) || check_vertical_conflict(j) || check_block_conflict(i / 2 * 2, j / 2 * 2);
}

bool is_done(int i, int j)
{
    for (int k = 0; k < 4; ++k)
    {
        if (board[i][k] == 0 || board[k][j] == 0)
            return false;
    }
    int start_row = i / 2 * 2, start_col = j / 2 * 2;
    for (int x = 0; x < 2; ++x)
    {
        for (int y = 0; y < 2; ++y)
        {
            if (board[start_row + x][start_col + y] == 0)
                return false;
        }
    }
    return true;
}

bool check_win()
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (board[i][j] == 0 || is_invalid(i, j))
                return false;
        }
    }
    return true;
}

void fill_number(char c)
{
    if (!editable[cur_r][cur_c])
        return;

    if (c >= '0' && c <= '4')
    {
        board[cur_r][cur_c] = c - '0';
        check_horizontal();
        check_vertical();
        check_block();
    }
}

void move_cursor(char c)
{
    int dr = 0, dc = 0;

    if (c == 'W' || c == 'w')
        dr = -1;
    else if (c == 'S' || c == 's')
        dr = 1;
    else if (c == 'A' || c == 'a')
        dc = -1;
    else if (c == 'D' || c == 'd')
        dc = 1;

    int new_r = cur_r + dr;
    int new_c = cur_c + dc;

    while (new_r >= 0 && new_r < 4 && new_c >= 0 && new_c < 4 && !editable[new_r][new_c])
    {
        new_r += dr;
        new_c += dc;
    }

    if (new_r >= 0 && new_r < 4 && new_c >= 0 && new_c < 4 && editable[new_r][new_c])
    {
        cur_r = new_r;
        cur_c = new_c;
    }
}

string get_styled_text(string text, string style)
{
    string color = "", font = "";
    for (char c : style)
    {
        if (c == 'R')
            color = "31";
        if (c == 'G')
            color = "32";
        if (c == 'E')
            color = "41";
        if (c == 'C')
            color = "106";
        if (c == 'B')
            font = ";1";
    }
    return "\x1b[" + color + font + "m" + text + "\x1b[0m";
}

void print_board()
{
    cout << "\x1b[2J\x1b[1;1H";

    cout << get_styled_text("W/A/S/D: ", "B") << "move cursor" << endl;
    cout << get_styled_text("    1-4: ", "B") << "fill in number" << endl;
    cout << get_styled_text("      0: ", "B") << "clear the cell" << endl;
    cout << get_styled_text("      Q: ", "B") << "quit" << endl;
    cout << endl;

    for (int i = 0; i < 4; ++i)
    {
        if (i && i % 2 == 0)
            cout << "---------------" << endl;

        cout << "|";
        for (int j = 0; j < 4; ++j)
        {
            string style = "";

            if (cur_r == i && cur_c == j)
                style = "C";
            else if (is_invalid(i, j))
                style = "E";
            else if (is_done(i, j))
                style = "G";

            if (!editable[i][j])
                style += "B";

            if (board[i][j] == 0)
                cout << get_styled_text(" . ", style);
            else
                cout << get_styled_text(" " + to_string(board[i][j]) + " ", style);

            if ((j + 1) % 2 == 0)
                cout << "|";
        }
        cout << endl;
    }
}

void initialize()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            editable[i][j] = !board[i][j];

    print_board();
}

int main()
{
    char c;
    initialize();

    while (cin >> c)
    {
        if (c == 'Q' || c == 'q')
            break;

        if (c == 'W' || c == 'A' || c == 'S' || c == 'D' || c == 'w' || c == 'a' || c == 's' || c == 'd')
            move_cursor(c);
        else if (c >= '0' && c <= '4')
            fill_number(c);

        print_board();

        if (check_win())
        {
            cout << "YOU WIN!" << endl;
            break;
        }
    }

    return 0;
}


