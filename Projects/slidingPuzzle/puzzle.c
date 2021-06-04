#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

#define W 4
#define H 3
#define M 2
#define NOTMOVE 12
#define POSSMOVES 4

typedef enum isequal
{
  equal,
  notequal
} isequal;

typedef enum
{
  false,
  true
} bool;

typedef struct node
{
  char square[H][W];

  /*Two dim int array containing a combination of possible moves*/
  int possiblemoves[W][M];

  /*Index of the parent in the list*/
  int parent;

  /*Number of possible moves*/
  int nummoves;

  /*Coordinate of the space*/
  int xspace, yspace;

  /*Index of the node in the linklist*/
  int index;

  /*Next node in the list*/
  struct node *next;

  /*Point at the node at pos 1, then 2,3 and so on*/
  struct node *lookat;
  /*Previous node in the list*/
  struct node *previous;

  /*Number of moves to find the solution*/
  int counter;

} Board;

Board *findsolution(Board *board);
void swap(Board *board, char square2[H][W], int n);
void printsquare(char square[H][W]);
void copysquare(char square[H][W], char squarenew[H][W]);
isequal checkifequal(char square[H][W], char square2[H][W]);
int exist(Board *board, char square[H][W]);
void test();
void nummoves(Board *board);
void possiblemoves(Board *board);
Board *createnewboard();
int inputsquare(char *string, char square[H][W]);
int createboards(Board *board, int i, int empty);
Board *nextmove(char input[H][W], char target[H][W]);
void freememory(Board *last);
int isempty(Board *board);
void printsolution(Board *solution, bool delay);
void printsolution(Board *solution, bool delay);

int main(int argc, char **argv)
{
  Board *last = NULL;
  Board *solution;
  char input[H][W];
  char target[H][W] = {
      {"123\0"},
      {"456\0"},
      {"78 \0"}};

  if (argc == 2)
  {

    inputsquare(argv[1], input);
    printf("wait...\n");
    last = nextmove(input, target);
    solution = findsolution(last);
    printsolution(solution, true);
    sleep(2);
    printsolution(solution, false);
    freememory(last);
  }
  else
  {
    printf("ERROR: Incorrect usage, try e.g. %s ''16 752843''\n", argv[0]);
  }
  /*test();*/
  return 0;
}

void freememory(Board *last)
{
  /*Free space allocated*/
  Board *tmp;

  while (last != NULL)
  {
    tmp = last;
    last = last->previous;
    free(tmp);
    tmp = NULL;
    assert(isempty(tmp));
  }
}

Board *nextmove(char input[H][W], char target[H][W])
{
  /*Calculate the moves until the sulution is found*/
  /*Head will point at the first node of the list*/
  Board *head = NULL, *board = NULL, *first;
  int i = 0, index = 0, n, moves, found = 0;
  char tempsquare[H][W];

  /*Allocate the space for the first node*/
  first = head = board = createnewboard();
  head->parent = -1;
  /*Set the parent */
  board->lookat = head;
  board->index = index;
  copysquare(input, head->square);

  while (!found)
  {
    /*Find the number of possible moves for the current board*/
    nummoves(board->lookat);
    /*Find the possible moves*/
    possiblemoves(board->lookat);
    /*Set the var moves to the number of possible moves*/
    moves = board->lookat->nummoves;

    /*If the solution is found exit from the loop*/
    if (checkifequal(board->square, target) == equal)
    {
      found = 1;
      board->index = i;
    }
    else
    {
      for (n = 0; n < moves; n++)
      {
        /*Generate the n move and store the result into tempsquare*/
        swap(board->lookat, tempsquare, n);

        /*If the move hasn't been done yet*/
        if (!exist(head, tempsquare))
        {

          /*Index of the next node*/
          index++;

          /*Allocate the space for the new node*/
          board->next = createnewboard();

          /*Point to the source for the next move*/
          board->next->lookat = board->lookat;

          /*Copy tempsquare inside the two dim array inside the node*/
          copysquare(tempsquare, board->next->square);

          /*Set the current node as the parent for the next one*/
          board->next->parent = i;

          /*Index of the next node*/
          board->next->index = index;
          /*Previous will point at the previous node in the list*/
          board->next->previous = board;
          board = board->next;
        }
      }

      /*Point the next board at the next element in the list*/
      board->lookat = first->next;
      first = first->next;
      i++;
    }
  }
  /*Return a pointer to the last node*/
  return board;
}

Board *findsolution(Board *board)
{
  char target[H][W] = {
      {"123\0"},
      {"456\0"},
      {"78 \0"}};
  Board *solution;
  int nextparent, counter = 0;

  /*Parent of the solution*/
  nextparent = board->parent;

  /*Allocate space for newboard*/
  solution = malloc(sizeof(Board));

  /*Copy the solution square at the first position*/
  copysquare(target, solution[counter].square);

  while (board != NULL)
  {

    /*If the board is parent of the previous one*/
    if (board->index == nextparent)
    {

      counter++;
      /*Copy it inside solition at index i*/
      copysquare(board->square, solution[counter].square);

      /*The next parent will be the parent of the next board*/
      nextparent = board->parent;
    }

    /*Go back to the previous board*/
    board = board->previous;
  }

  /*Counter rapresent the number of combination generated*/
  solution[0].counter = counter;

  return solution;
}
void printsolution(Board *solution, bool delay)
{

  int i = solution[0].counter;
  int index = 0;

  while (i >= 0)
  {
    /*Simple loop to print every board inside solution*/
    index++;
    printf("Index:%d\n", index);
    printsquare(solution[i].square);
    if (delay)
    {
      if (i != 0)
      {
        printf("\033[4A");
      }
    }
    sleep(1);
    fflush(stdout);
    i--;
  }
  if (delay)
  {
    printf("\nSOLVED\nBoard recap:\n");
    sleep(1);
  }
}

void printsquare(char square[H][W])
{
  /*Print a 3x3 square*/
  int i;

  for (i = 0; i < H; i++)
  {
    printf("%s\n", square[i]);
  }

  fflush(stdout);
}
int isempty(Board *board)
{
  return board == NULL;
}

int exist(Board *board, char square[H][W])
{
  /*Check if a combination already exists in the list */
  while (board != NULL)
  {

    if (checkifequal(board->square, square) == equal)
    {
      return 1;
    }
    board = board->next;
  }
  return 0;
}

Board *setmove(Board *board, int newy, int newx, int i, char tempsquare[H][W])
{
  board->possiblemoves[i][0] = newy;
  board->possiblemoves[i][1] = newx;
  tempsquare[newy][newx] = 'x';
  return board;
}

void possiblemoves(Board *board)
{
  int i;
  /*Coordinates of the white space*/
  int xspace = board->xspace;
  int yspace = board->yspace;

  char tempsquare[H][W];

  copysquare(board->square, tempsquare);

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
      board->possiblemoves[i][0] = NOTMOVE;
      board->possiblemoves[i][1] = NOTMOVE;
    }
  }
}

void swap(Board *board, char emptysquare[H][W], int n)
{
  /*Swap the space and one number*/

  /*Coordinates of the number to swap*/
  int y = board->possiblemoves[n][0];
  int x = board->possiblemoves[n][1];

  /*Coordinates of the space*/
  int yspace = board->yspace;
  int xspace = board->xspace;

  /*Copy board square inside the square pointed by emptysquare*/
  copysquare(board->square, emptysquare);

  /*Swap*/
  emptysquare[y][x] = ' ';
  emptysquare[yspace][xspace] = board->square[y][x];
}

void nummoves(Board *board)
{
  /*Find the number of possible moves for a cetrain board
  and the coordinates of the space*/
  /*Insert the values inside the corrispondent node*/

  int y, x, total;
  if (board->square[1][1] == ' ')
  {
    board->nummoves = 4;
    board->xspace = 1;
    board->yspace = 1;
    return;
  }

  for (y = 0; y < H; y++)
  {
    for (x = 0; x < H; x++)
    {
      if (board->square[y][x] == ' ')
      {
        total = x + y;
        /*If total is even*/
        if (total % 2 == 0)
        {
          board->nummoves = 2;
        }
        /*If total is odd*/
        else
        {
          board->nummoves = 3;
        }
        board->xspace = x;
        board->yspace = y;
      }
    }
  }
}

Board *createnewboard()
{
  /*Allocate the space for a new node and return a pointer to it*/
  Board *p;
  p = malloc(sizeof(Board));
  p->next = NULL;
  return p;
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
    square[y][3] = '\0';
    i--;
  }
  return 0;
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

void copysquare(char square[H][W], char squarenew[H][W])
{
  /*Copy a two dim array into another ine*/
  int x, y;
  for (y = 0; y < H; y++)
  {
    for (x = 0; x < H; x++)
    {
      squarenew[y][x] = square[y][x];
    }
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

  Board *board;
  board = createnewboard();

  /*Copy square inside emprty squarenew*/
  copysquare(square, squarenew);
  /*square-squarenew are equal*/
  assert(checkifequal(square, squarenew) == equal);

  /*square-square2 not equal*/
  assert(checkifequal(square, square2) == notequal);

  /*Convert string inputsquare in twodim array square*/
  inputsquare(input, square);
  /*square-square2 are now equal*/
  assert(checkifequal(square, square2) == equal);

  /*Copy square2 inside board->square*/
  copysquare(square2, board->square);

  /*board->square square2 equal*/
  assert(checkifequal(board->square, square2) == equal);

  /*Get the number of possible moves */
  nummoves(board);
  assert(board->nummoves == 3);

  /*The space inside square2 is at Y-1 X-2 */
  assert(board->yspace == 1);
  assert(board->xspace == 2);

  /*Get the possible moves*/
  possiblemoves(board);

  /*First possible move: move the space at Y-1 X-1*/
  assert(board->possiblemoves[0][0] == board->yspace);
  assert(board->possiblemoves[0][1] == 1);

  /*Second possible move: move the space at Y-0 X-2*/
  assert(board->possiblemoves[1][0] == 0);
  assert(board->possiblemoves[1][1] == 2);

  /*Only three moves are possible in this case*/
  assert(board->possiblemoves[3][0] == NOTMOVE);

  /*Look at the square inside the struct board*/
  /*Do the second possible move for this case*/
  /*Copy it inside the two dim emptysquare*/
  swap(board, emptysquare, 1);

  /*The square inside struct board should now be*/
  /*equal to square 2*/
  assert(exist(board, square2));
  assert(!exist(board, squarenew));
}
