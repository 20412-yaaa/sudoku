#include <iostream>
#include <windows.h>  // �Ω�Windows�t�Ϊ�����x�B�z

using namespace std;

// ��l�ƹC�����ѽL�A0 ��ܪŮ�
int board[4][4] = {
    {0, 0, 1, 0},
    {0, 4, 0, 0},
    {0, 0, 2, 0},
    {0, 1, 0, 0},
};

// �ΨӼе����Ǯ�l�O�i�H�s�誺
bool editable[4][4];

// ��e��Ц�m (�_�l��m�O (0, 0))
int cur_r = 0, cur_c = 0;

// �ˬd�Y�@��O�_���Ʀr����
bool check_horizontal_conflict(int row)
{
    bool seen[5] = {false}; // �аO�w�g�X�{�L���Ʀr�A���� 1 �� 4 �O���Ī��Ʀr
    for (int col = 0; col < 4; ++col)
    {
        if (board[row][col] != 0)  // �p�G�o�Ӯ�l����Ʀr
        {
            if (seen[board[row][col]])  // �p�G�o�ӼƦr�w�g�X�{�L
                return true;
            seen[board[row][col]] = true;  // �аO�o�ӼƦr�w�g�X�{
        }
    }
    return false;
}

// �ˬd�Y�@�C�O�_���Ʀr����
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

// �ˬd 2x2 �p�ϰ줺�O�_���Ʀr����
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

// �ˬd��ӴѽL����V�Ĭ�
void check_horizontal()
{
    for (int row = 0; row < 4; ++row)
    {
        if (check_horizontal_conflict(row))
            return;  // �@���o�{�Ĭ�N�h�X
    }
}

// �ˬd��ӴѽL���a�V�Ĭ�
void check_vertical()
{
    for (int col = 0; col < 4; ++col)
    {
        if (check_vertical_conflict(col))
            return;  // �@���o�{�Ĭ�N�h�X
    }
}

// �ˬd��ӴѽL�� 2x2 �p�ϰ�Ĭ�
void check_block()
{
    for (int i = 0; i < 4; i += 2)  // �H 2 ���B��M����
    {
        for (int j = 0; j < 4; j += 2)  // �H 2 ���B��M���C
        {
            if (check_block_conflict(i, j))
                return;  // �@���o�{�Ĭ�N�h�X
        }
    }
}

// �ˬd�S�w��l (i, j) �O�_�L��
bool is_invalid(int i, int j)
{
    return check_horizontal_conflict(i) || check_vertical_conflict(j) || check_block_conflict(i / 2 * 2, j / 2 * 2);
}

// �ˬd�S�w��l (i, j) �O�_�w�����A�Y�Ӧ�B�C�M�ϰ쳣�w�g�񺡨õL�Ĭ�
bool is_done(int i, int j)
{
    for (int k = 0; k < 4; ++k)
    {
        if (board[i][k] == 0 || board[k][j] == 0)  // �p�G�Ӧ�ΦC���Ů�
            return false;
    }
    int start_row = i / 2 * 2, start_col = j / 2 * 2;
    for (int x = 0; x < 2; ++x)  // �ˬd�ϰ줺�O�_���Ů�
    {
        for (int y = 0; y < 2; ++y)
        {
            if (board[start_row + x][start_col + y] == 0)
                return false;
        }
    }
    return true;
}

// �ˬd�C���O�_�����A�Y�ѽL�S���Ů�B�L�Ĭ�
bool check_win()
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (board[i][j] == 0 || is_invalid(i, j))
                return false;  // �p�G���Ů�Ϊ̵L�Ī���l
        }
    }
    return true;  // �Ҧ���l�w�񺡨åB�L�Ĭ�
}

// ��g�Ʀr���e��ЩҦb����l
void fill_number(char c)
{
    if (!editable[cur_r][cur_c])  // �p�G��e��l���i�s��
        return;

    if (c >= '0' && c <= '4')  // �p�G���U����O 0-4 �Ʀr
    {
        board[cur_r][cur_c] = c - '0';  // ��ӼƦr��J�ѽL
        check_horizontal();  // �ˬd�����Ĭ�
        check_vertical();    // �ˬd�����Ĭ�
        check_block();       // �ˬd�ϰ�Ĭ�
    }
}

// ���ʴ�Ц�m�A�ھګ����V
void move_cursor(char c)
{
    int dr = 0, dc = 0;

    if (c == 'W' || c == 'w')  // �W��
        dr = -1;
    else if (c == 'S' || c == 's')  // �U��
        dr = 1;
    else if (c == 'A' || c == 'a')  // ����
        dc = -1;
    else if (c == 'D' || c == 'd')  // �k��
        dc = 1;

    int new_r = cur_r + dr;
    int new_c = cur_c + dc;

    // ���ʴ�СA������@�ӥi�H�s�誺��l
    while (new_r >= 0 && new_r < 4 && new_c >= 0 && new_c < 4 && !editable[new_r][new_c])
    {
        new_r += dr;
        new_c += dc;
    }

    // �p�G�s����m���ĥB�i�s��A�h���ʴ��
    if (new_r >= 0 && new_r < 4 && new_c >= 0 && new_c < 4 && editable[new_r][new_c])
    {
        cur_r = new_r;
        cur_c = new_c;
    }
}

// �Ω�b�R�O�椤�]�w�奻���˦��]�C��B�r�����^
string get_styled_text(string text, string style)
{
    string color = "", font = "";
    for (char c : style)
    {
        if (c == 'R')
            color = "31";  // ����
        if (c == 'G')
            color = "32";  // ���
        if (c == 'E')
            color = "41";  // �I������
        if (c == 'C')
            color = "106"; // �I�����
        if (c == 'B')
            font = ";1";    // ����r��
    }
    return "\x1b[" + color + font + "m" + text + "\x1b[0m";  // ��^�a�˦�����r
}

// ���L�ѽL����e���A
void print_board()
{
    cout << "\x1b[2J\x1b[1;1H";  // �M��

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


