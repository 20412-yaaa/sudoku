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
bool row_has_conflict[4] = {false};
bool col_has_conflict[4] = {false};
bool block_has_conflict[4] = {false};


int cur_r = 0, cur_c = 0;

void check_horizontal(char c)
{
	for (int i = 0; i < 4; ++i) {
        bool visited[5] = {false};
        row_has_conflict[i] = false;
        for (int j = 0; j < 4; ++j) {
            int c = board[i][j];
            if (c > 0 && visited[c]) {
                row_has_conflict[i] = true;
                break;
            }
            if (c > 0)
                visited[c] = true;
        }
    }
    /* TODO: Check if a horizontal line has conflict number, or is finished. */
}

void check_vertical(char c)
{
	
    for (int j = 0; j < 4; ++j) {
        bool visited[5] = {false};
        col_has_conflict[j] = false;
        for (int i = 0; i < 4; ++i) {
            int num = board[i][j];
            if (num > 0 && visited[num]) {
                col_has_conflict[j] = true;
                break;
            }
            if (num > 0)
                visited[num] = true;
        }
    }
    /* TODO: Check if a vertical line has conflict number, or is finished. */
}

void check_block(char c)
{
	  for (int block = 0; block < 4; ++block) {
        bool visited[5] = {false};
        block_has_conflict[block] = false;
        for (int i = (block / 2) * 2; i < (block / 2) * 2 + 2; ++i) {
            for (int j = (block % 2) * 2; j < (block % 2) * 2 + 2; ++j) {
                int num = board[i][j];
                if (num > 0 && visited[num]) {
                    block_has_conflict[block] = true;
                    break;
                }
                if (num > 0)
                    visited[num] = true;
            }
        }
    }	
    /* TODO: Check if a block has conflict number, or is finished. */
}

void fill_number(char c)
{
    // �u���\��g���Ī��Ʀr (1-4)
    int n = c - '0'; // �N�r���ର�������Ʀr
    if (n >= 1 && n <= 4) {
        board[cur_r][cur_c] = n;  // ��J�Ʀr
    }
    else {
        cout << "�L�Ī��Ʀr�I�п�J 1 �� 4 ���Ʀr�C" << endl;
    }
}

void move_cursor(char c)
{
    // �O�s���ʪ���V
    int dr = 0, dc = 0;

    if (c == 'W' || c == 'w') {
        dr = -1;  // �V�W��
    }
    else if (c == 'S' || c == 's') {
        dr = 1;   // �V�U��
    }
    else if (c == 'A' || c == 'a') {
        dc = -1;  // �V����
    }
    else if (c == 'D' || c == 'd') {
        dc = 1;   // �V�k��
    }

    // ���ժu�۵��w����V���ʥ��СA���L�w�g��R�Ʀr����l
    int new_r = cur_r + dr;
    int new_c = cur_c + dc;

    // �p�G�ؼЦ�m�O�w��R�Ʀr����l�A�~��u�۷��e��V���ʡA������Ů�
    while (new_r >= 0 && new_r < 4 && new_c >= 0 && new_c < 4 && editable[new_r][new_c] == 0) {
        new_r += dr;
        new_c += dc;
    }

    // �p�G�s��m���M�b�ѽL�d�򤺡A�N��s���Ц�m
    if (new_r >= 0 && new_r < 4 && new_c >= 0 && new_c < 4) {
        cur_r = new_r;
        cur_c = new_c;
    }
}

bool is_invalid(int i, int j)
{
	int block_index = (i / 2) * 2 + (j / 2);
    return row_has_conflict[i] || col_has_conflict[j] || block_has_conflict[block_index];
    /* TODO: Check if board[i][j] is in a line that has conflict numbers. */
    return false;
}

bool is_done(int i, int j)
{
    /* TODO: Check if board[i][j] is in a line that has finished. */
    return false;
}

bool check_win()
{
    /* TODO: Check if the game is set. That is, every cell is finished. */
    return false;
}

bool is_moving_action(char c)
{
    return (c == 'W' || c == 'w' || c == 'S' || c == 's' ||
            c == 'A' || c == 'a' || c == 'D' || c == 'd');
}

bool is_filling_action(char c)
{
    return (c >= '1' && c <= '4');  // �u���\��J 1 �� 4 ���Ʀr
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
    // Flush the screen
    cout << "\x1b[2J\x1b[1;1H";

    // Print usage hint.
    cout << get_styled_text("W/A/S/D: ", "B") << "move cursor" << endl;
    cout << get_styled_text("    1-4: ", "B") << "fill in number" << endl;
    cout << get_styled_text("      0: ", "B") << "clear the cell" << endl;
    cout << get_styled_text("      Q: ", "B") << "quit" << endl;
    cout << endl;

    // Iterate through and print each cell.
    for (int i = 0; i < 4; ++i)
    {
        // Print horizontal divider.
        if (i && i % 2 == 0)
            cout << "---------------" << endl;

        // Print the first vertical divider in each line.
        cout << "|";
        for (int j = 0; j < 4; ++j)
        {
            // Set text style based on the state of the cell.
            string style = "";

            // Set style for the cell the cursor pointing to.
            if (cur_r == i && cur_c == j)
                style = "C";
            // Set style for the cell in an invalid line.
            else if (is_invalid(i, j))
                style = "E";
            // Set style for the cell in a finished line.
            else if (is_done(i, j))
                style = "G";

            // Print the cell out in styled text.
            // If the content is 0, print a dot, else print the number.
            if (board[i][j] == 0)
                cout << get_styled_text(" �P ", style);
            else
                cout << get_styled_text(" " + to_string(board[i][j]) + " ", style);

            // Print the vertical divider for each block.
            if ((j + 1) % 2 == 0)
                cout << "|";
        }
        cout << endl;
    }
}

void initialize()
{
    // Set up styled text for Windows.
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);

    // Mark editable cells
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            editable[i][j] = true;  // �Ҧ���l���i�H�s��

    // Print the initial board out.
    print_board();
}

int main()
{
    char c;
    bool action_ok;

    initialize();
    while (cin >> c)
    {
        action_ok = false;
        if (is_moving_action(c))
        {
            action_ok = true;
            move_cursor(c);
        }

        if (is_filling_action(c))
        {
            action_ok = true;
            fill_number(c);
        }

        if (c == 'Q' || c == 'q')
            break;

        print_board();

        if (check_win())
        {
            cout << "YOU WIN!" << endl;
            break;
        }

        if (!action_ok)
            cout << get_styled_text("!!! Invalid action !!!", "R");
    }

    return 0;
}