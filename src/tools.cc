
#include "tools.h"

char g_log_file_name[] = "conn.log";

void init_board(char board[][GRID_NUM])
{
    // Board init. [10/28/2011 lang]
    for(int i = 0; i< 21 ; i++)
    {
        board[i][0] = board[0][i] = board[i][GRID_NUM-1] = board[GRID_NUM-1][i] = BORDER;
    }
    for(int i = 1 ; i< GRID_NUM-1 ; i++)
    {
        for(int j = 1 ; j < GRID_NUM-1 ; j++)
        {
            board[i][j] = NOSTONE;
        }
    }
}

void make_move(char board[][GRID_NUM], move_t* move, char color) {
    board[move->positions[0].x][move->positions[0].y] = color;
    board[move->positions[1].x][move->positions[1].y] = color;
}

void unmake_move(char board[][GRID_NUM], move_t* move)
{
    board[move->positions[0].x][move->positions[0].y] = NOSTONE;
    board[move->positions[1].x][move->positions[1].y] = NOSTONE;
}

bool is_win_by_premove(char board[][GRID_NUM], move_t *preMove)
{
    int count = 0,i,j,n,m;

    //ÅÐ¶ÏµÚÒ»¸öµã
    n = i = preMove->positions[0].x;
    m = j = preMove->positions[0].y;
    //Ë®Æ½·½Ïò
    count = 0;
    if (board[n][m] == BORDER
        || board[n][m] == NOSTONE)
    {
        return false;
    }
    while ( board[i][j] == board[n][m])
    {
        i++;
        count++;
    }
    i = n-1;
    while ( board[i][j] == board[n][m])
    {
        i--;
        count++;
    }
    if (count >= 6)
    {
        return true;
    }

    //Ð±·½Ïò
    count = 0;
    i = n;j = m;
    while ( board[i][j] == board[n][m])
    {
        i++;
        j++;
        count++;
    }
    i = n-1;
    j = m-1;
    while ( board[i][j] == board[n][m])
    {
        i--;
        j--;
        count++;
    }
    if (count >= 6)
    {
        return true;
    }

    //ÊúÖ±·½Ïò
    count = 0;
    i = n;j = m;
    while ( board[i][j] == board[n][m])
    {
        j++;
        count++;
    }
    j = m-1;
    while (board[i][j] == board[n][m])
    {
        j--;
        count++;
    }
    if (count >= 6)
    {
        return true;
    }

    //Ð±·½Ïò
    count = 0;
    i = n;j = m;
    while ( board[i][j] == board[n][m])
    {
        i++;
        j--;
        count++;
    }
    i = n-1;
    j = m+1;
    while ( board[i][j] == board[n][m])
    {
        i--;
        j++;
        count++;
    }
    if (count >= 6)
    {
        return true;
    }

    //ÅÐ¶ÏµÚ¶þ¸öµã
    n = i = preMove->positions[1].x;
    m = j = preMove->positions[1].y;
    if (board[n][m] == BORDER
        || board[n][m] == NOSTONE)
    {
        return false;
    }
    //Ë®Æ½·½Ïò
    count = 0;
    while ( board[i][j] == board[n][m])
    {
        i++;
        count++;
    }
    i = n-1;
    while (board[i][j] == board[n][m])
    {
        i--;
        count++;
    }
    if (count >= 6)
    {
        return true;
    }

    //Ð±·½Ïò
    count = 0;
    i = n;j = m;
    while ( board[i][j] == board[n][m])
    {
        i++;
        j++;
        count++;
    }
    i = n-1;
    j = m-1;
    while ( board[i][j] == board[n][m])
    {
        i--;
        j--;
        count++;
    }
    if (count >= 6)
    {
        return true;
    }

    //ÊúÖ±·½Ïò
    count = 0;
    i = n;j = m;
    while ( board[i][j] == board[n][m])
    {
        j++;
        count++;
    }
    j = m-1;
    while ( board[i][j] == board[n][m])
    {
        j--;
        count++;
    }
    if (count >= 6)
    {
        return true;
    }

    //Ð±·½Ïò
    count = 0;
    i = n;j = m;
    while ( board[i][j] == board[n][m])
    {
        i++;
        j--;
        count++;
    }
    i = n-1;
    j = m+1;
    while ( board[i][j] == board[n][m])
    {
        i--;
        j++;
        count++;
    }
    if (count >= 6)
    {
        return true;
    }

    //Ã»ÓÐÁ¬ÁùµÄ×Ó
    return false;
}

int log_to_file(char* msg) 
{
    FILE* file = fopen(g_log_file_name, "a");
  static char buf[32];
    if (file == NULL)
    {
    printf("Error: Can't open log file - %s\n", g_log_file_name);
        return -1;
    }
    time_t tm = time(NULL);
  char* ptr;
  ptr = ctime(&tm);
  ptr[strlen(ptr) - 1] = 0;
    fprintf(file, "[%s] - %s\n", ptr, msg);
    fclose(file);

    return 0;
}

int move2msg(move_t* move, char* msg)
{
    if (move->positions[0].x == move->positions[1].x
        && move->positions[0].y == move->positions[1].y)
    {
        msg[0] = 'S' - move->positions[0].x + 1;
        msg[1] = move->positions[0].y + 'A' - 1;
        msg[2] = 0;
        return 1;
    } 
    else
    {
        msg[1] = 'S' - move->positions[0].x + 1;
        msg[0] = move->positions[0].y + 'A' - 1;
        msg[3] = 'S' - move->positions[1].x + 1;
        msg[2] = move->positions[1].y + 'A' - 1;

        //msg[0] = move->positions[0].x + 'A' - 1;
        //msg[1] = move->positions[0].y + 'A' - 1;
        //msg[2] = move->positions[1].x + 'A' - 1;
        //msg[3] = move->positions[1].y + 'A' - 1;
        msg[4] = 0;
    }
    return 2;
}

int msg2move(char* msg, move_t* move)
{
    if (msg[2] == 0)
    {
        move->positions[0].x = move->positions[1].x = 'S' - msg[1] + 1;
        move->positions[0].y = move->positions[1].y = msg[0] - 'A' + 1;
        move->score = 0;
        return 1;
    } 
    else
    {
        move->positions[0].x = 'S' - msg[1] + 1;
        move->positions[0].y = msg[0] - 'A' + 1;
        move->positions[1].x = 'S' - msg[3] + 1;
        move->positions[1].y = msg[2] - 'A' + 1;

        //move->positions[0].x = msg[0] - 'A' + 1;
        //move->positions[0].y = msg[1] - 'A' + 1;
        //move->positions[1].x = msg[2] - 'A' + 1;
        //move->positions[1].y = msg[3] - 'A' + 1;
        move->score = 0;
    }
    return 2;
}

void print_board(char board[21][21], move_t* preMove)
{
    printf("  ");
    for (int i = 1 ; i < GRID_NUM - 1 ; i++)
    {
        printf("%2d",i);
    }
    printf("\n");
    int x,y;
    for (int i = 1 ; i < GRID_NUM - 1 ; i++)
    {
        printf("%2d", i);
        for (int j = 1 ; j < GRID_NUM - 1; j++)
        {
            //x = j;
            //y = GRID_NUM - i - 1;
            x = i;
            y = j;
            if (preMove) {
                if (x == preMove->positions[0].x 
                        && y == preMove->positions[0].y 
                        && board[x][y] != NOSTONE
                        || x == preMove->positions[1].x 
                        && y == preMove->positions[1].y 
                        && board[x][y] != NOSTONE )
                {
                    printf(" X");
                    continue;
                }
            }
            switch (board[x][y])
            {
                case  WHITE:
                    printf(" O");
                    break;
                case BLACK:
                    printf(" *");
                    break;
                case NOSTONE:
                    printf(" -");
                    break;
            }
        }
        printf("\n");
    }
}

void print_score(move_one_t *moveList,int n)
{
    int board[GRID_NUM][GRID_NUM];
    memset(board, 0, sizeof(board));
    for (int i = 0 ; i < n ; i++)
    {
        board[moveList[i].x][moveList[i].y] = moveList[i].score;
    }

    printf("  ");
    for (int i = 1 ; i < GRID_NUM - 1 ; i++)
    {
        printf("%4d",i);
    }
    printf("\n");
    int score = 0,x,y;
    for (int i = 1 ; i < GRID_NUM - 1 ; i++)
    {
        printf("%2d", i);
        for (int j = 1 ; j < GRID_NUM - 1; j++)
        {
            //x = j;
            //y = GRID_NUM - i - 1;
            x = i;
            y = j;
            score = board[x][y];
            if (score == 0)
            {
                printf("   -");
            } 
            else
            {
                printf("%4d", score);
            }
        }
        printf("\n");
    }
}

