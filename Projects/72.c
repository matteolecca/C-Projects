#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#define W 4
#define H 3
#define M 2
#define MAXBOARDS 100000
#define NOTMOVE 12
#define POSSMOVES 4

typedef enum isequal
{
  equal,
  notequal
} isequal;

typedef struct board
{

  char square[H][W];

  /*Two dim int array containing a combination of possible moves*/
  int pmoves[W][M];

  /*Index of the parent in the list*/
  int parent;

  /*Number of possible moves*/
  int nummoves;

  /*Coordinate of the space*/
  int xspace, yspace;

  /*Number of moves to find the solution*/
  int counter;
} Board;

void swap(Board board, char square2[H][W], int n);
void printsquare(char square[H][W]);
void copysquare(char square[H][W], char squarenew[H][W]);
isequal checkifequal(char square[H][W], char square2[H][W]);
int nummissplace(char square[H][W]);
int exist(Board boardarray[MAXBOARDS], char square[H][W], int count);
void test();
void iteratesolution(Board boardarray[MAXBOARDS]);
int createboards(Board boardarray[MAXBOARDS], int i, int empty);
Board nummoves(Board board);
Board possiblemoves(Board board);
int inputsquare(char *string, char square[H][W]);
Board *findsolution(Board boardarray[MAXBOARDS]);
Board setmove(Board board, int newy, int newx, int i, char tempsquare[H][W]);
void printsolution(Board *solution);

int main(int argc, char **argv)
{
  char input[H][W];
  static Board boardarray[MAXBOARDS];
  Board *solution;

  if (argc == 2)
  {
    /*Convert the string in a two dim array*/
    inputsquare(argv[1], input);

    /*Copy it inside the board at index 0*/
    copysquare(input, boardarray[0].square);
    /*Flag to find the first node */
    boardarray[0].parent = -1;

    printf("wait...\n");
    /*Try to find the moves*/
    iteratesolution(boardarray);

    solution = findsolution(boardarray);
    printsolution(solution);
  }
  else
  {
    printf("ERROR: Incorrect usage, try e.g. %s ''16 752843''\n", argv[0]);
  }

  /*test();*/
  return 0;
}

void iteratesolution(Board boardarray[MAXBOARDS])
{

  int i = 0, empty = 1;
  /*Set the firs board at -1 to know when to end the loop*/
  boardarray[i].parent = -1;

  while (empty)
  {
    /*Do while createboards don't returns 0*/
    if (i < MAXBOARDS)
    {
      empty = createboards(boardarray, i, empty);
    }
    else
    {
      printf("Unabe to find the solution\n");
    }

    /*Increase the index of the node*/
    i++;
  }
}

int createboards(Board boardarray[MAXBOARDS], int i, int empty)
{
  char target[H][W] = {
      {"123\0"},
      {"456\0"},
      {"78 \0"}};
  int n, c = 0;
  char tempsquare[H][W];

  /*Found the solution, return 0*/
  if (checkifequal(boardarray[i].square, target) == equal)
  {
    return 0;
  }

  /*Find the number of possible moves for the current board*/
  boardarray[i] = nummoves(boardarray[i]);

  /*Find the possible moves*/
  boardarray[i] = possiblemoves(boardarray[i]);

  for (n = 0; n < boardarray[i].nummoves; n++)
  {

    /*Generate the n move and store the result into tempsquare*/
    swap(boardarray[i], tempsquare, n);

    /*If the move hasn't been done yet*/
    if (!exist(boardarray, tempsquare, c + empty))
    {

      /*Copy tempsquare inside the two dim array square of the node*/
      copysquare(tempsquare, boardarray[c + empty].square);

      /*The parent of the n board generated is always the one at index i*/
      boardarray[c + empty].parent = i;
      c++;
    }
  }

  /*Return the index of the next empty node in the list*/
  return c + empty;
}

isequal checkifequal(char square[H][W], char square2[H][W])
{
  /*Check if two square are equal*/

  int x, y;
  for (y = 0; y < H; y++)
  {
    for (x = 0; x < H; x++)
    {
      if (square[y][x] != square2[y][x])
      {
        return notequal;
      }
    }
  }
  return equal;
}
void swap(Board board, char emptysquare[H][W], int n)
{
  /*Swap the space and one number*/

  /*Coordinates of the number to swap*/
  int y = board.pmoves[n][0];
  int x = board.pmoves[n][1];

  /*Coordinates of the space*/
  int yspace = board.yspace;
  int xspace = board.xspace;

  /*Copy board square inside the square pointed by emptysquare*/
  copysquare(board.square, emptysquare);

  /*Swap*/
  emptysquare[y][x] = ' ';
  emptysquare[yspace][xspace] = board.square[y][x];
}
void printsquare(char square[H][W])
{
  int i;
  for (i = 0; i < H; i++)
  {
    printf("%s\n", square[i]);
  }
  printf("\n");
}
void copysquare(char square[H][W], char squarenew[H][W])
{
  int x, y;
  for (y = 0; y < H; y++)
  {
    for (x = 0; x < W; x++)
    {
      squarenew[y][x] = square[y][x];
    }
  }
}

int exist(Board boardarray[MAXBOARDS], char square[H][W], int count)
{
  /*Check if a combination already exists in the array */

  int i;
  for (i = 0; i < count; i++)
  {
    if (checkifequal(boardarray[i].square, square) == equal)
    {
      return 1;
    }
  }
  return 0;
}
Board nummoves(Board board)
{
  /*Find the number of possible moves for a square
  and the coordinates of the space*/
  /*Insert the values inside the corrispondent node*/

  int y, x, total;
  if (board.square[1][1] == ' ')
  {
    board.nummoves = 4;
    board.xspace = 1;
    board.yspace = 1;
    return board;
  }

  for (y = 0; y < H; y++)
  {
    for (x = 0; x < H; x++)
    {
      if (board.square[y][x] == ' ')
      {

        total = x + y;

        /*If total is even*/
        if (total % 2 == 0)
        {
          board.nummoves = 2;
        }

        /*If total is odd*/
        else
        {
          board.nummoves = 3;
        }

        board.xspace = x;
        board.yspace = y;
      }
    }
  }
  return board;
}
Board setmove(Board board, int newy, int newx, int i, char tempsquare[H][W])
{
  board.pmoves[i][0] = newy;
  board.pmoves[i][1] = newx;
  tempsquare[newy][newx] = 'x';
  return board;
}
Board possiblemoves(Board board)
{
  int i;

  /*Coordinates of the white space*/
  int xspace = board.xspace;
  int yspace = board.yspace;

  char tempsquare[H][W];

  copysquare(board.square, tempsquare);

  for (i = 0; i < POSSMOVES; i++)
  {

    /*If can swap left*/
    if ((xspace > 0) && (xspace <= 2) && (tempsquare[yspace][xspace - 1] != 'x'))
    {
      board = setmove(board, yspace, xspace - 1, i, tempsquare);
    }

    /*If can swap right*/
    else if ((xspace >= 0) && (xspace < 2) && (tempsquare[yspace][xspace + 1] != 'x'))
    {
      board = setmove(board, yspace, xspace + 1, i, tempsquare);
    }

    /*If can swap up*/
    else if ((yspace > 0) && (yspace <= 2) && (tempsquare[yspace - 1][xspace] != 'x'))
    {
      board = setmove(board, yspace - 1, xspace, i, tempsquare);
    }

    /*If can swap down*/
    else if ((yspace >= 0) && (yspace < 2) && (tempsquare[yspace + 1][xspace] != 'x'))
    {
      board = setmove(board, yspace + 1, xspace, i, tempsquare);
    }

    else
    {
      board.pmoves[i][0] = NOTMOVE;
      board.pmoves[i][1] = NOTMOVE;
    }
  }
  return board;
}
int inputsquare(char *string, char square[H][W])
{
  /*Convert the imput string into a 3x3 square*/

  int y, x, i = 0;

  for (y = 0; y < H; y++)
  {
    for (x = 0; x < W; x++)
    {
      square[y][x] = string[i];
      i++;
    }
    square[y][H] = '\0';
    i--;
  }
  return 0;
}
Board *findsolution(Board boardarray[MAXBOARDS])
{

  Board *solution;
  char target[H][W] = {
      {"123\0"},
      {"456\0"},
      {"78 \0"}};
  int i = 0, counter = 0;

  while (checkifequal(boardarray[i].square, target) != equal)
  {
    i++;
  }

  /*Allocate space for the newboard*/
  solution = malloc(sizeof(Board));

  /*Until found the first combination insered*/
  while (boardarray[i].parent != -1)
  {

    /*Copy the parent of the current board inside the array solution*/
    copysquare(boardarray[boardarray[i].parent].square, solution[counter].square);
    counter++;

    /*Change the index to index of the parent of the board*/
    i = boardarray[i].parent;
  }

  /*Store the number of moves made to get the result*/
  solution[0].counter = counter;

  return solution;
}
void printsolution(Board *solution)
{

  int i = solution[0].counter;
  int index = 0;

  while (i >= 0)
  {
    /*Simple loop to print every board inside solution*/
    index++;
    printf("Index:%d\n", index);
    printsquare(solution[i].square);
    i--;
  }
}

void test()
{
  char square[H][W] = {
      {"12 \0"},
      {"463\0"},
      {"578\0"}};
  char square2[H][W] = {
      {"123\0"},
      {"46 \0"},
      {"578\0"}};
  char squarenew[H][W];
  char emptysquare[H][W];
  char input[] = {"12346 578"};

  static Board boardarray[MAXBOARDS];

  /*Copy square inside empty squarenew*/
  copysquare(square, squarenew);
  /*square-squarenew are equal*/
  assert(checkifequal(square, squarenew) == equal);

  /*square-square2 not equal*/
  assert(checkifequal(square, square2) == notequal);

  /*Convert string inputsquare in twodim array square*/
  inputsquare(input, square);
  /*square-square2 are now equal*/
  assert(checkifequal(square, square2) == equal);

  /*Copy square2 inside board[0].square*/
  copysquare(square2, boardarray[0].square);

  /*board[0].square square2 equal*/
  assert(checkifequal(boardarray[0].square, square2) == equal);

  /*Get the number of possible moves */
  boardarray[0] = nummoves(boardarray[0]);
  assert(boardarray[0].nummoves == 3);

  /*The space inside square2 is at Y-1 X-2 */
  assert(boardarray[0].yspace == 1);
  assert(boardarray[0].xspace == 2);

  /*Get the possible moves*/
  boardarray[0] = possiblemoves(boardarray[0]);

  /*First possible move: move the space at Y-1 X-1*/
  assert(boardarray[0].pmoves[0][0] == boardarray[0].yspace);
  assert(boardarray[0].pmoves[0][1] == 1);

  /*Second possible move: move the space at Y-0 X-2*/
  assert(boardarray[0].pmoves[1][0] == 0);
  assert(boardarray[0].pmoves[1][1] == 2);

  /*Only three moves are possible in this case*/
  assert(boardarray[0].pmoves[3][0] == NOTMOVE);

  /*Look at the square inside the first struct inside the array*/
  /*Do the second possible move for this case*/
  /*Copy it inside the two dim emptysquare*/
  swap(boardarray[0], emptysquare, 1);

  /*The square inside struct board should now be*/
  /*equal to square 2*/
  assert(exist(boardarray, square2, 1));
}
