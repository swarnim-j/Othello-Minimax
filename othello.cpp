#include <iostream>
#include <vector>
#include <array>
#include <string>

using namespace std;

// class for information about piece - coordinates and color
class Piece
{
public:
    int x, y; // x, y belong to [0, 7]
    bool white;

    Piece(int x, int y, bool white)
    {
        this->x = x;
        this->y = y;
        this->white = white;
    }

    Piece()
    {
        this->x = 0;
        this->y = 0;
        this->white = white;
    }
};

// class for information about board state - board array, last added piece, score
class Board
{
public:
    array<array<int, 8>, 8> board;
    Piece latestPiece;
    int score;

    Board(array<array<int, 8>, 8> brd, Piece latestPiece, int score)
    {
        this->board = brd;
        this->latestPiece = latestPiece;
        this->score = score;
    }
};

array<array<int, 8>, 8> board; // +1 for white, -1 for black, 0 for unoccupied
Piece lastPiece; // latest piece added to board

// function definitions
array<array<int, 8>, 8> updateState(Piece piece, array<array<int, 8>, 8> brd);
bool isLegal(Piece piece, array<array<int, 8>, 8> brd);
array<int, 2> countPieces(array<array<int, 8>, 8> brd);
void printBoard();
void printScore(array<array<int, 8>, 8> brd);
int checkState(bool white, array<array<int, 8>, 8> brd);
vector<Piece> legalMoves(bool white, array<array<int, 8>, 8> brd);
Board minimax(int depth, Board currentBoard);

// updates the board after a move has been made
array<array<int, 8>, 8> updateState(Piece piece, array<array<int, 8>, 8> brd)
{
    bool white = piece.white; // white move if true, black move if false
    int x = piece.x, y = piece.y;

    array<array<int, 8>, 8> brd_c; // creating a copy of brd[][] to manipulate later
    copy(brd.begin(), brd.end(), brd_c.begin());

    int a = white ? 1 : -1;
    int b = -1 * a;

    brd[x][y] = a;
    brd_c[x][y] = b; // putting opposite value in brd_c[][] for easier computation

    // top (north)
    if (y > 1)
    {
        for (int i = y; i > 0; i--)
        {
            if (brd_c[x][i] == 0)
            {
                break;
            }
            else if (brd_c[x][i] == brd_c[x][i - 1])
            {
                continue;
            }
            else if (brd_c[x][i - 1] != 0 && i != y)
            {
                for (int j = i; j < y; j++)
                {
                    brd[x][j] = a;
                    brd_c[x][j] = a;
                }
            }
            else
            {
                break;
            }
        }
    }

    // top right (north-east)
    if (y > 1 && x < 6)
    {
        if ((7 - x) <= y)
        { // distance between x and edge < y and edge (top right edges)
            for (int i1 = x, i2 = y; i1 < 7; i1++, i2--)
            {
                if (brd_c[i1][i2] == 0)
                {
                    break;
                }
                else if (brd_c[i1][i2] == brd_c[i1 + 1][i2 - 1])
                {
                    continue;
                }
                else if (brd_c[i1 + 1][i2 - 1] != 0 && i1 != x)
                {
                    for (int j1 = i1, j2 = i2; j1 > x; j1--, j2++)
                    {
                        brd[j1][j2] = a;
                        brd_c[j1][j2] = a;
                    }
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            for (int i1 = x, i2 = y; i2 > 0; i1++, i2--)
            {
                if (brd_c[i1][i2] == 0)
                {
                    break;
                }
                else if (brd_c[i1][i2] == brd_c[i1 + 1][i2 - 1])
                {
                    continue;
                }
                else if (brd_c[i1 + 1][i2 - 1] != 0 && i1 != x)
                {
                    for (int j1 = i1, j2 = i2; j2 < y; j1--, j2++)
                    {
                        brd[j1][j2] = a;
                        brd_c[j1][j2] = a;
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }

    // right (east)
    if (x < 6)
    {
        for (int i = x; i < 7; i++)
        {
            if (brd_c[i][y] == 0)
            {
                break;
            }
            else if (brd_c[i][y] == brd_c[i + 1][y])
            {
                continue;
            }
            else if (brd_c[i + 1][y] != 0 && i != x)
            {
                for (int j = i; j > x; j--)
                {
                    brd[j][y] = a;
                    brd_c[j][y] = a;
                }
            }
            else
            {
                break;
            }
        }
    }

    // bottom right (south-east)
    if (y < 6 && x < 6)
    {
        if ((7 - x) <= (7 - y))
        { // distance between x and edge < y and edge (bottom right edges)
            for (int i1 = x, i2 = y; i1 < 7; i1++, i2++)
            {
                if (brd_c[i1][i2] == 0)
                {
                    break;
                }
                else if (brd_c[i1][i2] == brd_c[i1 + 1][i2 + 1])
                {
                    continue;
                }
                else if (brd_c[i1 + 1][i2 + 1] != 0 && i1 != x)
                {
                    for (int j1 = i1, j2 = i2; j1 > x; j1--, j2--)
                    {
                        brd[j1][j2] = a;
                        brd_c[j1][j2] = a;
                    }
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            for (int i1 = x, i2 = y; i2 < 7; i1++, i2++)
            {
                if (brd_c[i1][i2] == 0)
                {
                    break;
                }
                else if (brd_c[i1][i2] == brd_c[i1 + 1][i2 + 1])
                {
                    continue;
                }
                else if (brd_c[i1 + 1][i2 + 1] != 0 && i1 != x)
                {
                    for (int j1 = i1, j2 = i2; j2 > y; j1--, j2--)
                    {
                        brd[j1][j2] = a;
                        brd_c[j1][j2] = a;
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }

    // bottom (south)
    if (y < 6)
    {
        for (int i = y; i < 7; i++)
        {
            if (brd_c[x][i] == 0)
            {
                break;
            }
            else if (brd_c[x][i] == brd_c[x][i + 1])
            {
                continue;
            }
            else if (brd_c[x][i + 1] != 0 && i != y)
            {
                for (int j = i; j > y; j--)
                {
                    brd[x][j] = a;
                    brd_c[x][j] = a;
                }
            }
            else
            {
                break;
            }
        }
    }

    // bottom left (south-west)
    if (y < 6 && x > 1)
    {
        if (x <= (7 - y))
        { // distance between x and edge < y and edge (bottom right edges)
            for (int i1 = x, i2 = y; i1 > 0; i1--, i2++)
            {
                if (brd_c[i1][i2] == 0)
                {
                    break;
                }
                else if (brd_c[i1][i2] == brd_c[i1 - 1][i2 + 1])
                {
                    continue;
                }
                else if (brd_c[i1 - 1][i2 + 1] != 0 && i1 != x)
                {
                    for (int j1 = i1, j2 = i2; j1 < x; j1++, j2--)
                    {
                        brd[j1][j2] = a;
                        brd_c[j1][j2] = a;
                    }
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            for (int i1 = x, i2 = y; i2 < 7; i1--, i2++)
            {
                if (brd_c[i1][i2] == 0)
                {
                    break;
                }
                else if (brd_c[i1][i2] == brd_c[i1 - 1][i2 + 1])
                {
                    continue;
                }
                else if (brd_c[i1 - 1][i2 + 1] != 0)
                {
                    for (int j1 = i1, j2 = i2; j2 > y; j1++, j2--)
                    {
                        brd[j1][j2] = a;
                        brd_c[j1][j2] = a;
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }

    // left (west)
    if (x > 1)
    {
        for (int i = x; i > 0; i--)
        {
            if (brd_c[i][y] == 0)
            {
                break;
            }
            else if (brd_c[i][y] == brd_c[i - 1][y])
            {
                continue;
            }
            else if (brd_c[i - 1][y] != 0 && i != x)
            {
                for (int j = i; j < x; j++)
                {
                    brd[j][y] = a;
                    brd_c[j][y] = a;
                }
            }
            else
            {
                break;
            }
        }
    }

    // top left (north-west)
    if (y > 1 && x > 1)
    {
        if (x <= y)
        { // distance between x and edge < y and edge (bottom right edges)
            for (int i1 = x, i2 = y; i1 > 0; i1--, i2--)
            {
                if (brd_c[i1][i2] == 0)
                {
                    break;
                }
                else if (brd_c[i1][i2] == brd_c[i1 - 1][i2 - 1])
                {
                    continue;
                }
                else if (brd_c[i1 - 1][i2 - 1] != 0 && i1 != x)
                {
                    for (int j1 = i1, j2 = i2; j1 < x; j1++, j2++)
                    {
                        brd[j1][j2] = a;
                        brd_c[j1][j2] = a;
                    }
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            for (int i1 = x, i2 = y; i2 > 0; i1--, i2--)
            {
                if (brd_c[i1][i2] == 0)
                {
                    break;
                }
                else if (brd_c[i1][i2] == brd_c[i1 - 1][i2 - 1])
                {
                    continue;
                }
                else if (brd_c[i1 - 1][i2 - 1] != 0 && i1 != x)
                {
                    for (int j1 = i1, j2 = i2; j2 < y; j1++, j2++)
                    {
                        brd[j1][j2] = a;
                        brd_c[j1][j2] = a;
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }

    return brd;
}

// checks whether a particular move is legal or not
bool isLegal(Piece piece, array<array<int, 8>, 8> brd)
{
    bool white = piece.white; // white move if true, black move if false
    int x = piece.x, y = piece.y;

    if (brd[x][y] != 0)
    {
        return false;
    }

    array<array<int, 8>, 8> brd_c;
    copy(brd.begin(), brd.end(), brd_c.begin());

    int a = white ? 1 : -1;
    int b = -1 * a;

    brd_c[x][y] = b;

    // top (north)
    if (y > 1)
    {
        for (int i = y; i > 0; i--)
        {
            if (brd_c[x][i] == 0)
            {
                break;
            }
            else if (brd_c[x][i] == brd_c[x][i - 1])
            {
                continue;
            }
            else if (brd_c[x][i - 1] != 0 && i != y)
            {
                for (int j = i; j < y; j++)
                {
                    if (brd_c[x][j] != a)
                    {
                        return true;
                    }
                }
            }
            else
            {
                break;
            }
        }
    }

    // top right (north-east)
    if (y > 1 && x < 6)
    {
        if ((7 - x) <= y)
        { // distance between x and edge < y and edge (top right edges)
            for (int i1 = x, i2 = y; i1 < 7; i1++, i2--)
            {
                if (brd_c[i1][i2] == 0)
                {
                    break;
                }
                else if (brd_c[i1][i2] == brd_c[i1 + 1][i2 - 1])
                {
                    continue;
                }
                else if (brd_c[i1 + 1][i2 - 1] != 0 && i1 != x)
                {
                    for (int j1 = i1, j2 = i2; j1 > x; j1--, j2++)
                    {
                        if (brd_c[j1][j2] != a)
                        {
                            return true;
                        }
                    }
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            for (int i1 = x, i2 = y; i2 > 0; i1++, i2--)
            {
                if (brd_c[i1][i2] == 0)
                {
                    break;
                }
                else if (brd_c[i1][i2] == brd_c[i1 + 1][i2 - 1])
                {
                    continue;
                }
                else if (brd_c[i1 + 1][i2 - 1] != 0 && i1 != x)
                {
                    for (int j1 = i1, j2 = i2; j2 < y; j1--, j2++)
                    {
                        if (brd_c[j1][j2] != a)
                        {
                            return true;
                        }
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }

    // right (east)
    if (x < 6)
    {
        for (int i = x; i < 7; i++)
        {
            if (brd_c[i][y] == 0)
            {
                break;
            }
            else if (brd_c[i][y] == brd_c[i + 1][y])
            {
                continue;
            }
            else if (brd_c[i + 1][y] != 0 && i != x)
            {
                for (int j = i; j > x; j--)
                {
                    if (brd_c[j][y] != a)
                    {
                        return true;
                    }
                }
            }
            else
            {
                break;
            }
        }
    }

    // bottom right (south-east)
    if (y < 6 && x < 6)
    {
        if ((7 - x) <= (7 - y))
        { // distance between x and edge < y and edge (bottom right edges)
            for (int i1 = x, i2 = y; i1 < 7; i1++, i2++)
            {
                if (brd_c[i1][i2] == 0)
                {
                    break;
                }
                else if (brd_c[i1][i2] == brd_c[i1 + 1][i2 + 1])
                {
                    continue;
                }
                else if (brd_c[i1 + 1][i2 + 1] != 0 && i1 != x)
                {
                    for (int j1 = i1, j2 = i2; j1 > x; j1--, j2--)
                    {
                        if (brd_c[j1][j2] != a)
                        {
                            return true;
                        }
                    }
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            for (int i1 = x, i2 = y; i2 < 7; i1++, i2++)
            {
                if (brd_c[i1][i2] == 0)
                {
                    break;
                }
                else if (brd_c[i1][i2] == brd_c[i1 + 1][i2 + 1])
                {
                    continue;
                }
                else if (brd_c[i1 + 1][i2 + 1] != 0 && i1 != x)
                {
                    for (int j1 = i1, j2 = i2; j2 > y; j1--, j2--)
                    {
                        if (brd_c[j1][j2] != a)
                        {
                            return true;
                        }
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }

    // bottom (south)
    if (y < 6)
    {
        for (int i = y; i < 7; i++)
        {
            if (brd_c[x][i] == 0)
            {
                break;
            }
            else if (brd_c[x][i] == brd_c[x][i + 1])
            {
                continue;
            }
            else if (brd_c[x][i + 1] != 0 && i != y)
            {
                for (int j = i; j > y; j--)
                {
                    if (brd_c[x][j] != a)
                    {
                        return true;
                    }
                }
            }
            else
            {
                break;
            }
        }
    }

    // bottom left (south-west)
    if (y < 6 && x > 1)
    {
        if (x <= (7 - y))
        { // distance between x and edge < y and edge (bottom right edges)
            for (int i1 = x, i2 = y; i1 > 0; i1--, i2++)
            {
                if (brd_c[i1][i2] == 0)
                {
                    break;
                }
                else if (brd_c[i1][i2] == brd_c[i1 - 1][i2 + 1])
                {
                    continue;
                }
                else if (brd_c[i1 - 1][i2 + 1] != 0 && i1 != x)
                {
                    for (int j1 = i1, j2 = i2; j1 < x; j1++, j2--)
                    {
                        if (brd_c[j1][j2] != a)
                        {
                            return true;
                        }
                    }
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            for (int i1 = x, i2 = y; i2 < 7; i1--, i2++)
            {
                if (brd_c[i1][i2] == 0)
                {
                    break;
                }
                else if (brd_c[i1][i2] == brd_c[i1 - 1][i2 + 1])
                {
                    continue;
                }
                else if (brd_c[i1 - 1][i2 + 1] != 0 && i1 != x)
                {
                    for (int j1 = i1, j2 = i2; j2 > y; j1++, j2--)
                    {
                        if (brd_c[j1][j2] != a)
                        {
                            return true;
                        }
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }

    // left (west)
    if (x > 1)
    {
        for (int i = x; i > 0; i--)
        {
            if (brd_c[i][y] == 0)
            {
                break;
            }
            else if (brd_c[i][y] == brd_c[i - 1][y])
            {
                continue;
            }
            else if (brd_c[i - 1][y] != 0 && i != x)
            {
                for (int j = i; j < x; j++)
                {
                    if (brd_c[j][y] != a)
                    {
                        return true;
                    }
                }
            }
            else
            {
                break;
            }
        }
    }

    // top left (north-west)
    if (y > 1 && x > 1)
    {
        if (x <= y)
        { // distance between x and edge < y and edge (bottom right edges)
            for (int i1 = x, i2 = y; i1 > 0; i1--, i2--)
            {
                if (brd_c[i1][i2] == 0)
                {
                    break;
                }
                else if (brd_c[i1][i2] == brd_c[i1 - 1][i2 - 1])
                {
                    continue;
                }
                else if (brd_c[i1 - 1][i2 - 1] != 0 && i1 != x)
                {
                    for (int j1 = i1, j2 = i2; j1 < x; j1++, j2++)
                    {
                        if (brd_c[j1][j2] != a)
                        {
                            return true;
                        }
                    }
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            for (int i1 = x, i2 = y; i2 > 0; i1--, i2--)
            {
                if (brd_c[i1][i2] == 0)
                {
                    break;
                }
                else if (brd_c[i1][i2] == brd_c[i1 - 1][i2 - 1])
                {
                    continue;
                }
                else if (brd_c[i1 - 1][i2 - 1] != 0 && i1 != x)
                {
                    for (int j1 = i1, j2 = i2; j2 < y; j1++, j2++)
                    {
                        if (brd_c[j1][j2] != a)
                        {
                            return true;
                        }
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }

    return false;
}

// counts pieces of each color on current board position
array<int, 2> countPieces(array<array<int, 8>, 8> brd)
{

    array<int, 2> count = {0, 0}; // {white, black}

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (brd[i][j] == 1)
            {
                count[0]++;
            }
            else if (brd[i][j] == -1)
            {
                count[1]++;
            }
        }
    }

    return count;
}

// prints the current board after each move
void printBoard()
{
    int n = 1;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            string s = n < 10 ? "0" + to_string(n) : to_string(n);
            cout << s << " ";
            n++;
        }

        cout << "  ";

        for (int j = 0; j < 8; j++)
        {
            if (board[j][i] == 1)
            {
                cout << "W ";
            }
            else if (board[j][i] == -1)
            {
                cout << "B ";
            }
            else
            {
                cout << "- ";
            }
        }
        cout << endl;
    }

    return;
}

// prints the score after each move
void printScore()
{
    int white = countPieces(board)[0];
    int black = countPieces(board)[1];

    cout << "White: " << white << ", Black: " << black << endl
         << endl;

    return;
}

// checks whether the game has ended and if so, what the result is
int checkState(bool white, array<array<int, 8>, 8> brd)
{
    // -3: continue
    // -2: black passes
    // -1: white passes
    //  0: draw
    //  1: white wins
    //  2: black wins

    int n_white = countPieces(brd)[0];
    int n_black = countPieces(brd)[1];

    if (legalMoves(white, brd).empty() && legalMoves(!white, brd).empty())
    {
        if (n_white > n_black)
        {
            return 1;
        }
        else if (n_white < n_black)
        {
            return 2;
        }
        return 0;
    }
    else
    {
        if (white)
        {
            if (legalMoves(true, brd).empty()) // white passes
            {
                return -1;
            }
        }
        else
        {
            if (legalMoves(false, brd).empty()) // black passes
            {
                return -2;
            }
        }
    }

    return -3; // continue
}

// finds legal moves for white/black and returns them in a vector
vector<Piece> legalMoves(bool white, array<array<int, 8>, 8> brd)
{
    vector<Piece> moves;

    int a = white ? 1 : -1;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            Piece piece(i, j, white);
            if (isLegal(piece, brd))
            {
                moves.push_back(piece);
            }
        }
    }

    return moves;
}

// finds best move for maximizing/minimizing score by search
Board minimax(int depth, Board currentBoard)
{
    array<array<int, 8>, 8> brd = currentBoard.board;
    Piece latestPiece = currentBoard.latestPiece;
    int sc = currentBoard.score;

    bool col_next = !(latestPiece.white);

    vector<Piece> moves = legalMoves(col_next, brd);

    array<array<int, 8>, 8> best_brd = currentBoard.board;
    Piece bestPiece(0, 0, true);
    int best_score = sc;

    if (depth == 0)
    {
        brd = updateState(latestPiece, brd);
        int temp_score = countPieces(brd)[0] - countPieces(brd)[1];
        Board tempBoard(brd, latestPiece, temp_score);
        return tempBoard;
    }

    if (col_next) // searching for white moves
    {
        int maxVal = -100;
        int pruneScore = -100;
        for (Piece move : moves)
        {
            array<array<int, 8>, 8> brd_c = updateState(move, brd);
            int score = countPieces(brd_c)[0] - countPieces(brd_c)[1];

            if (score < pruneScore)
            {
                continue;
            }

            Board tempBoard(brd_c, move, score);
            Board newBoard = minimax(depth - 1, tempBoard);

            if (newBoard.score > maxVal)
            {
                maxVal = best_score = newBoard.score;
                bestPiece = move;
            }
        }
    }
    else
    {
        int minVal = 100;
        int pruneScore = 100;
        for (Piece move : moves)
        {
            array<array<int, 8>, 8> brd_c = updateState(move, brd);
            int score = countPieces(brd_c)[0] - countPieces(brd_c)[1];

            if (score > pruneScore)
            {
                continue;
            }

            Board tempBoard(brd_c, move, score);
            Board newBoard = minimax(depth - 1, tempBoard);

            if (newBoard.score < minVal)
            {
                minVal = best_score = newBoard.score;
                bestPiece = move;
            }
        }
    }

    Board bestBoard(brd, bestPiece, best_score);

    return bestBoard;
}

// main function
int main()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            board[j][i] = 0;
        }
    }

    board[3][3] = -1;
    board[3][4] = 1;
    board[4][3] = 1;
    board[4][4] = -1;

    printBoard();
    cout << endl;

    bool isWhite = true;

    while (true)
    {
        int state = checkState(isWhite, board);

        if (state == 2)
        {
            cout << "Black wins!" << endl;
            break;
        }
        else if (state == 1)
        {
            cout << "White wins!" << endl;
            break;
        }
        else if (state == 0)
        {
            cout << "Draw!" << endl;
            break;
        }
        else if (state == -1)
        {
            cout << "White passes" << endl;
            isWhite = !isWhite;
            continue;
        }
        else if (state == -2)
        {
            cout << "Black passes" << endl;
            isWhite = !isWhite;
            continue;
        }

        int n, x, y;

        if (isWhite) // white move
        {
            cout << "Enter move (W): " << endl;
            cin >> n;
            x = (n - 1) % 8;
            y = (n - 1) / 8;
        }
        else
        {
            Board currentBoard(board, lastPiece, countPieces(board)[0] - countPieces(board)[1]);
            Board currentState = minimax(5, currentBoard); // depth = 5
            x = currentState.latestPiece.x, y = currentState.latestPiece.y;

            int state = checkState(!isWhite, board);

            if (state == 2)
            {
                cout << "Black wins!" << endl;
                break;
            }
            else if (state == 1)
            {
                cout << "White wins!" << endl;
                break;
            }
            else if (state == 0)
            {
                cout << "Draw!" << endl;
                break;
            }
            else if (state == -1)
            {
                cout << "White passes" << endl;
                isWhite = !isWhite;
                continue;
            }
            else if (state == -2)
            {
                cout << "Black passes" << endl;
                isWhite = !isWhite;
                continue;
            }
        }

        Piece piece(x, y, isWhite);

        if (!isLegal(piece, board))
        {
            cout << "Invalid move!" << endl;
            continue;
        }

        Piece tempPiece(x, y, isWhite);
        lastPiece = tempPiece;

        array<array<int, 8>, 8> board_tmp = updateState(piece, board);
        copy(board_tmp.begin(), board_tmp.end(), board.begin());
        printBoard();
        if (!isWhite)
        {
            cout << "Computer played!" << endl;
        }
        printScore();

        isWhite = !isWhite;
    }

    return 0;
}