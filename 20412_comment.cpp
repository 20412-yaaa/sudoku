#include <iostream>
#include <windows.h>  // 用於Windows系統的控制台處理

using namespace std;

// 初始化遊戲的棋盤，0 表示空格
int board[4][4] = {
    {0, 0, 1, 0},
    {0, 4, 0, 0},
    {0, 0, 2, 0},
    {0, 1, 0, 0},
};

// 用來標註哪些格子是可以編輯的
bool editable[4][4];

// 當前游標位置 (起始位置是 (0, 0))
int cur_r = 0, cur_c = 0;

// 檢查某一行是否有數字重複
bool check_horizontal_conflict(int row)
{
    bool seen[5] = {false}; // 標記已經出現過的數字，索引 1 到 4 是有效的數字
    for (int col = 0; col < 4; ++col)
    {
        if (board[row][col] != 0)  // 如果這個格子有填數字
        {
            if (seen[board[row][col]])  // 如果這個數字已經出現過
                return true;
            seen[board[row][col]] = true;  // 標記這個數字已經出現
        }
    }
    return false;
}

// 檢查某一列是否有數字重複
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

// 檢查 2x2 小區域內是否有數字重複
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

// 檢查整個棋盤的橫向衝突
void check_horizontal()
{
    for (int row = 0; row < 4; ++row)
    {
        if (check_horizontal_conflict(row))
            return;  // 一旦發現衝突就退出
    }
}

// 檢查整個棋盤的縱向衝突
void check_vertical()
{
    for (int col = 0; col < 4; ++col)
    {
        if (check_vertical_conflict(col))
            return;  // 一旦發現衝突就退出
    }
}

// 檢查整個棋盤的 2x2 小區域衝突
void check_block()
{
    for (int i = 0; i < 4; i += 2)  // 以 2 的步伐遍歷行
    {
        for (int j = 0; j < 4; j += 2)  // 以 2 的步伐遍歷列
        {
            if (check_block_conflict(i, j))
                return;  // 一旦發現衝突就退出
        }
    }
}

// 檢查特定格子 (i, j) 是否無效
bool is_invalid(int i, int j)
{
    return check_horizontal_conflict(i) || check_vertical_conflict(j) || check_block_conflict(i / 2 * 2, j / 2 * 2);
}

// 檢查特定格子 (i, j) 是否已完成，即該行、列和區域都已經填滿並無衝突
bool is_done(int i, int j)
{
    for (int k = 0; k < 4; ++k)
    {
        if (board[i][k] == 0 || board[k][j] == 0)  // 如果該行或列有空格
            return false;
    }
    int start_row = i / 2 * 2, start_col = j / 2 * 2;
    for (int x = 0; x < 2; ++x)  // 檢查區域內是否有空格
    {
        for (int y = 0; y < 2; ++y)
        {
            if (board[start_row + x][start_col + y] == 0)
                return false;
        }
    }
    return true;
}

// 檢查遊戲是否完成，即棋盤沒有空格且無衝突
bool check_win()
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (board[i][j] == 0 || is_invalid(i, j))
                return false;  // 如果有空格或者無效的格子
        }
    }
    return true;  // 所有格子已填滿並且無衝突
}

// 填寫數字到當前游標所在的格子
void fill_number(char c)
{
    if (!editable[cur_r][cur_c])  // 如果當前格子不可編輯
        return;

    if (c >= '0' && c <= '4')  // 如果按下的鍵是 0-4 數字
    {
        board[cur_r][cur_c] = c - '0';  // 把該數字填入棋盤
        check_horizontal();  // 檢查水平衝突
        check_vertical();    // 檢查垂直衝突
        check_block();       // 檢查區域衝突
    }
}

// 移動游標位置，根據按鍵方向
void move_cursor(char c)
{
    int dr = 0, dc = 0;

    if (c == 'W' || c == 'w')  // 上移
        dr = -1;
    else if (c == 'S' || c == 's')  // 下移
        dr = 1;
    else if (c == 'A' || c == 'a')  // 左移
        dc = -1;
    else if (c == 'D' || c == 'd')  // 右移
        dc = 1;

    int new_r = cur_r + dr;
    int new_c = cur_c + dc;

    // 移動游標，直到找到一個可以編輯的格子
    while (new_r >= 0 && new_r < 4 && new_c >= 0 && new_c < 4 && !editable[new_r][new_c])
    {
        new_r += dr;
        new_c += dc;
    }

    // 如果新的位置有效且可編輯，則移動游標
    if (new_r >= 0 && new_r < 4 && new_c >= 0 && new_c < 4 && editable[new_r][new_c])
    {
        cur_r = new_r;
        cur_c = new_c;
    }
}

// 用於在命令行中設定文本的樣式（顏色、字型等）
string get_styled_text(string text, string style)
{
    string color = "", font = "";
    for (char c : style)
    {
        if (c == 'R')
            color = "31";  // 紅色
        if (c == 'G')
            color = "32";  // 綠色
        if (c == 'E')
            color = "41";  // 背景紅色
        if (c == 'C')
            color = "106"; // 背景綠色
        if (c == 'B')
            font = ";1";    // 粗體字型
    }
    return "\x1b[" + color + font + "m" + text + "\x1b[0m";  // 返回帶樣式的文字
}

// 打印棋盤的當前狀態
void print_board()
{
    cout << "\x1b[2J\x1b[1;1H";  // 清屏

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


