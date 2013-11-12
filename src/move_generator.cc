/*
 * Copyright (c) 2008-2013 Hao Cui <Hao.Cui@Tufts.edu>,
 *                         Liang Li <liliang010@gmail.com>,
 *                         Ruijian Wang <jeoygin@gmail.com>,
 *                         Siran Lin <linsiran@gmail.com>.
 *                         All rights reserved.
 *
 * This program is a free software; you can redistribute it and/or modify
 * it under the terms of the BSD license. See LICENSE.txt for details.
 *
 * Date: 2013/11/01
 *
 */

#include "tools.h"
#include "move_generator.h"

// Scores for the move generator.
#define score_1         15000
#define score_1_5       5005
#define score_2         5000
#define score_2_5       4995
#define score_2_6       2000
#define score_2_9       900
#define score_3         800
#define score_3_1       750
#define score_3_2       300
#define score_3_3       200
#define score_3_5       145
#define score_3_6       144
#define score_4         140
#define score_4_5       139
#define score_4_6       137
#define score_4_7       90
#define score_4_8       80
#define score_5         60
#define score_5_5       50
#define score_6         40
#define score_6_5       35
#define score_6_6       30
#define score_6_7       25
#define score_7         8
#define score_8         3
#define score_9         1

#define V               0
#define T               1


CMoveGenerator::CMoveGenerator()
{
    m_dead_four_plus = 0;

    m_time_get_moves = 0.0;
    m_time_set_score = 0.0;
    m_time_test = 0.0;
}

int CMoveGenerator::get_move_list(char ourColor , move_t* moveList, char board[][GRID_NUM])
{
    static move_one_t moveOne[GRID_COUNT ] ;
    static move_one_t moveOneCopy[GRID_COUNT ] ;
    static move_one_t moveTwo[GRID_COUNT ];
    int i,j,n,m,ii;
    int count = 0;
    int numOfOne = NUMOFONE;
    int numOfTwo ;

    move_one_t newMoveOne[GRID_COUNT];

    init_valuable_space(board);

    int tempScore = 0;

    clock_t beg,end;
    beg = clock();

    m_pos_to_update.clear();
    n = set_score(ourColor,1,moveOne,board);
    numOfOne = n;

    //print_board();
    //print_score(moveOne, n);

    if ( numOfOne > NUMOFONE )
        numOfOne = NUMOFONE;

    if (m_dead_four_plus == 1)
    {
        numOfOne = 3;
    }

    for(i = 0 ; i< numOfOne ; i++)
    {
        // Get the second stone for a move.
        m_pos_to_update_special.clear();

        board[moveOne[i].x][moveOne[i].y] = ourColor;
        extend_pos(moveOne[i].x,moveOne[i].y, board);

        memcpy(moveOneCopy,moveOne,(n + 1)*sizeof(move_one_t));

        tempScore = moveOneCopy[i].score;

        moveOneCopy[i].score = 0;

        // Set scores for the second stone.
        m = set_score(ourColor,2,newMoveOne,board);

        numOfTwo = sort_merge(moveTwo,moveOneCopy,n,newMoveOne,m);
        m = numOfTwo;

        if (m > NUMOFTWO)
            m = NUMOFTWO;

        if (m_dead_four_plus == 1)
        {
            m = 12;
        }


        board[moveOne[i].x][moveOne[i].y] = NOSTONE;

        // Take the second store to make the moves.
        int    moreTwoMove = 0;

        for(j = 0 ; j < m && moveTwo[j].score; j++)
        {
            ii = 0;

            if (moveTwo[j].score == moveTwo[m].score && moveTwo[j].score)
            {
                m++;
                moreTwoMove++;
            }
            while (ii < count)
            {
                if (moveList[ii].positions[1].x == moveOne[i].x && moveList[ii].positions[1].y == moveOne[i].y &&
                        moveList[ii].positions[0].x == moveTwo[j].x && moveList[ii].positions[0].y == moveTwo[j].y )
                    break;
                ii++;
            }
            if (ii < count)
            {
                moreTwoMove--;

                if (moreTwoMove < 0 && m < numOfTwo)
                {
                    m++;
                    moreTwoMove++;
                }
                continue;
            }
            moveList[count].positions[0].x = moveOne[i].x;
            moveList[count].positions[0].y = moveOne[i].y;
            moveList[count].positions[1].x = moveTwo[j].x;
            moveList[count].positions[1].y = moveTwo[j].y;
            moveList[count++].score = moveTwo[j].score + tempScore;
        }
    }

    end = clock();
    m_time_get_moves += (double)(end-beg)/CLOCKS_PER_SEC;

    return count;
}

int CMoveGenerator::init_valuable_space(char board[][GRID_NUM]) {
    memset(map,0,sizeof(map));
    int count = 0;
    int i,j,x,y;

    for ( x = 1 ; x < GRID_NUM - 1; x++)
    {
        for ( y = 1 ; y < GRID_NUM - 1; y++)
        {
            if (board[x][y] != NOSTONE)
            {
                i = x - 1;
                j = y;
                count = 0;
                while(count < 3 && board[i][j] == NOSTONE ) {
                    map[i][j] = 1;
                    i--;
                    count++;
                }
                i = x + 1;
                j = y;
                count = 0;
                while(count < 3 && board[i][j] == NOSTONE ) {
                    map[i][j] = 1;
                    i++;
                    count++;
                }

                i = x;
                j = y - 1;
                count = 0;
                while(count < 3 && board[i][j] == NOSTONE ) {
                    map[i][j] = 1;
                    j--;
                    count++;
                }
                i = x;
                j = y + 1;
                count = 0;
                while(count < 3 && board[i][j] == NOSTONE ) {
                    map[i][j] = 1;
                    j++;
                    count++;
                }

                i = x - 1;
                j = y - 1;
                count = 0;
                while(count < 3 && board[i][j] == NOSTONE ) {
                    map[i][j] = 1;
                    i--;
                    j--;
                    count++;
                }
                i = x + 1;
                j = y + 1;
                count = 0;
                while(count < 3 && board[i][j] == NOSTONE ) {
                    map[i][j] = 1;
                    i++;
                    j++;
                    count++;
                }

                i = x - 1;
                j = y + 1;
                count = 0;
                while(count < 3 && board[i][j] == NOSTONE ) {
                    map[i][j] = 1;
                    i--;
                    j++;
                    count++;
                }
                i = x + 1;
                j = y - 1;
                count = 0;
                while(count < 3 && board[i][j] == NOSTONE ) {
                    map[i][j] = 1;
                    i++;
                    j--;
                    count++;
                }
            } // if
        } // for y
    } // for x

    return 0;
}

void CMoveGenerator::add_new_pos_for_two(char x, char y)
{
    pos_t pos ;
    pos.x = x;
    pos.y = y;
    m_pos_to_update.push_back(pos);
}

void CMoveGenerator::add_new_pos_for_two_special(char x, char y)
{
    pos_t pos ;
    pos.x = x;
    pos.y = y;
    m_pos_to_update_special.push_back(pos);
}

bool CMoveGenerator::extend_pos(char x, char y, char board[][GRID_NUM])
{
    int i,j;
    int count;

    i = x;j = y;
    count = 0;
    while(count < 3)
    {
        i++;
        count++;
        if (board[i][j] == BORDER)
            break;
        if ( board[i][j] != NOSTONE)
        {
            count = 0;
            continue;
        }

        add_new_pos_for_two_special(i,j);
    }

    i = x;j = y;
    count = 0;
    while(count < 3)
    {
        i--;
        count++;
        if (board[i][j] == BORDER)
            break;
        if ( board[i][j] != NOSTONE)
        {
            count = 0;
            continue;
        }
        add_new_pos_for_two_special(i,j);
    }

    i = x;j = y;
    count = 0;
    while(count < 3)
    {
        j++;
        count++;
        if (board[i][j] == BORDER)
            break;
        if ( board[i][j] != NOSTONE)
        {
            count = 0;
            continue;
        }
        add_new_pos_for_two_special(i,j);
    }

    i = x;j = y;
    count = 0;
    while(count < 3)
    {
        j--;
        count++;
        if (board[i][j] == BORDER)
            break;
        if ( board[i][j] != NOSTONE)
        {
            count = 0;
            continue;
        }
        add_new_pos_for_two_special(i,j);
    }

    i = x;j = y;
    count = 0;
    while(count < 3)
    {
        i++;
        j++;
        count++;
        if (board[i][j] == BORDER)
            break;
        if ( board[i][j] != NOSTONE)
        {
            count = 0;
            continue;
        }
        add_new_pos_for_two_special(i,j);
    }

    i = x;j = y;
    count = 0;
    while(count < 3)
    {
        i--;
        j--;
        count++;
        if (board[i][j] == BORDER)
            break;
        if ( board[i][j] != NOSTONE)
        {
            count = 0;
            continue;
        }
        add_new_pos_for_two_special(i,j);
    }

    i = x;j = y;
    count = 0;
    while(count < 3)
    {
        i++;
        j--;
        count++;
        if (board[i][j] == BORDER)
            break;
        if ( board[i][j] != NOSTONE)
        {
            count = 0;
            continue;
        }
        add_new_pos_for_two_special(i,j);
    }

    i = x;j = y;
    count = 0;
    while(count < 3)
    {
        i--;
        j++;
        count++;
        if (board[i][j] == BORDER)
            break;
        if ( board[i][j] != NOSTONE)
        {
            count = 0;
            continue;
        }
        add_new_pos_for_two_special(i,j);
    }

    return true;
}

int CMoveGenerator::set_score( char ourColor , int step , move_one_t moveList[], char board[][GRID_NUM] )
{
    int count = 0 , score = 0;
    int i,j;
    int maxCount ;        // Return the max valid points in the board, sorted by their scores.

    int maxI;
    move_one_t tempMove;

    if ( step == 1 )
    {
        m_dead_four_plus = 0;
    }

    if (step == 1)
    {
        maxCount = NUMOFONE * 2;
        for (i = 1 ; i < GRID_NUM - 1 ; i++)
        {
            for (j = 1 ; j < GRID_NUM - 1 ; j++)
            {
                if (map[i][j] == 1)
                {
                    score = set_score_single( ourColor , i , j , step, board );

                    moveList[count].x = i;
                    moveList[count].y = j;
                    moveList[count++].score = score;
                }
            }
        }
    }else {
        maxCount = NUMOFTWO * 4;
        static char mapflag[GRID_NUM][GRID_NUM] ;
        memset(mapflag, 0, sizeof(mapflag));

        for (std::vector<pos_t>::iterator it = m_pos_to_update.begin(); it != m_pos_to_update.end() ; it++)
        {
            if (mapflag[it->x][it->y] || board[it->x][it->y] != NOSTONE)
            {
                continue;
            }
            else
            {
                mapflag[it->x][it->y] = 1;
            }
            score = set_score_single( ourColor , it->x , it->y , step, board );

            moveList[count].x = it->x;
            moveList[count].y = it->y;
            moveList[count++].score = score;
        }
        for (std::vector<pos_t>::iterator it = m_pos_to_update_special.begin(); it != m_pos_to_update_special.end() ; it++)
        {
            if (mapflag[it->x][it->y] || board[it->x][it->y] != NOSTONE)
            {
                continue;
            }
            else
            {
                mapflag[it->x][it->y] = 1;
            }
            score = set_score_single( ourColor , it->x , it->y , step, board );

            moveList[count].x = it->x;
            moveList[count].y = it->y;
            moveList[count++].score = score;
        }
    }

    clock_t beg,end;
    beg = clock();

    if (maxCount > count)
    {
        maxCount = count;
    }
    for(int i = 0 ; i < maxCount; i++)
    {
        maxI = i;
        for(int j = i+1 ; j < count ; j++)
        {
            if (moveList[maxI].score < moveList[j].score)
            {
                maxI = j;
            }
        }

        tempMove.score = moveList[maxI].score;
        tempMove.x = moveList[maxI].x;
        tempMove.y = moveList[maxI].y;
        moveList[maxI].score = moveList[i].score;
        moveList[maxI].x = moveList[i].x;
        moveList[maxI].y = moveList[i].y;
        moveList[i].x = tempMove.x;
        moveList[i].y = tempMove.y;
        moveList[i].score = tempMove.score;
    }

    end = clock();
    m_time_test += (double)(end-beg)/CLOCKS_PER_SEC;

    return count;
}

int CMoveGenerator::sort_merge(move_one_t list[],move_one_t listOne[],int oneN,move_one_t listTwo[],int twoN)
{
    int index[361] = {0};

    int count = 0,n;
    int i,j;

    for( i = 0 ; i < oneN ; i++)
    {
        index[(listOne[i].x - 1 ) * 19 + listOne[i].y - 1] = i ;
    }
    for( i = 0 ; i < twoN ; i++)
    {
        n = index[(listTwo[i].x - 1 ) * 19 + listTwo[i].y - 1];
        if (n + 1)
        {
            listOne[n].score = 0;
        }
    }
    for(  i = 0,j = 0 ; i < oneN &&j < twoN ; )
    {
        while(listOne[i].score == 0 && i < oneN)
        {
            i++;
        }

        if (listOne[i].score > listTwo[j].score)
        {
            list[count].score = listOne[i].score;
            list[count].x = listOne[i].x;
            list[count].y = listOne[i].y;
            i++;
            count++;
        }else
        {
            list[count].score = listTwo[j].score;
            list[count].x = listTwo[j].x;
            list[count].y = listTwo[j].y;
            j++;
            count++;
        }
        //if (count > 10)
        //    return ;
    }
    while (i < oneN)
    {
        if (listOne[i].score)
        {
            list[count].score = listOne[i].score;
            list[count].x = listOne[i].x;
            list[count].y = listOne[i].y;
            count++;
        }
        i++;
    }
    while (j < twoN)
    {
        list[count].score = listTwo[j].score;
        list[count].x = listTwo[j].x;
        list[count].y = listTwo[j].y;
        j++;
        count++;
    }

    return count;
}

int CMoveGenerator::set_score_single ( char ourColor , int x , int y , int step, char board[][GRID_NUM] )
{
    int score = 0;
    clock_t beg,end;
    beg = clock();

    score += set_by_direction1 ( ourColor , x , y , step, board );
    score += set_by_direction2 ( ourColor , x , y , step, board );
    score += set_by_direction3 ( ourColor , x , y , step, board );
    score += set_by_direction4 ( ourColor , x , y , step, board );
    score += g_board_base_score[x-1][y-1] + 1;

    end = clock();
    m_time_set_score += (double)(end-beg)/CLOCKS_PER_SEC;

    return score;
}

// Set score by vertical direction.
int CMoveGenerator::set_by_direction1 ( char color , int x , int y , int step, char board[][GRID_NUM] )
{
    // Set the color of self and enemy.
    int self , enemy ,
        highLevel , lowLevel , i ,
        connectCountUp = 0 , connectCountDn = 0 , OcountUp = 0 , OcountDn = 0 ,
        score = 0 , middleCheck[2] = {0,0} , connectCount = 0 ,
        defSuc = 0 , defVT = 0 , canGoFive = 0 , edgeBlock = 0 , Ocount = 0;
    if ( color == 1 )
    {
        self = 1;
        enemy = 2;
    }
    else
    {
        self = 2;
        enemy = 1;
    }

    OcountUp = 0;
    OcountDn = 0;
    connectCountUp = 0;
    connectCountDn = 0;
    connectCount = 0;
    highLevel = x;
    lowLevel = x;

    // Set the bounds of attack and defance.
    i = x;
    while ( OcountUp < 3 )
    {
        i --;
        if ( board[i][y] == enemy )
        {
            connectCountUp ++;
            middleCheck[0] = 1;
            continue;
        }
        else
        {
            if ( board[i][y] == self || !IsValidPos(i,y) )
            {
                highLevel = i;
                break;
            }
            else
            {
                if ( board[i][y] == 0 )
                {
                    if ( board[i-1][y] == 0 && board[i-2][y] != enemy )
                    {
                        highLevel = i;
                        break;
                    }
                    else
                    {
                        OcountUp ++;
                        continue;
                    }
                }
            }
        }
    }
    if ( OcountUp == 3 )
    {
        highLevel = i;
        OcountUp = 2;
    }

    // Set the bounds of attack and defance.
    i = x;
    while ( OcountDn < 3 )
    {
        i ++;
        if ( board[i][y] == enemy )
        {
            connectCountDn ++;
            middleCheck[1] = 1;
            continue;
        }
        else
        {
            if ( board[i][y] == self || board[i][y] == 3 )
            {
                lowLevel = i;
                break;
            }
            else
            {
                if ( board[i][y] == 0 )
                {
                    if ( board[i+1][y] == 0 && board[i+2][y] != enemy )
                    {
                        lowLevel = i;
                        break;
                    }
                    else
                    {
                        OcountDn ++;
                        continue;
                    }
                }
            }
        }
    }
    if ( OcountDn == 3 )
    {
        lowLevel = i;
        OcountDn = 2;
    }
    if ( middleCheck[0] && middleCheck[1] )
    {
        if ( lowLevel - x <= 6 || x - highLevel <= 6 )
        {
            defSuc = 1;
        }
        else
        {
            defSuc = 0;
        }
        Ocount = OcountUp + OcountDn + 1;
    }
    if ( middleCheck[0] && !middleCheck[1] )
    {
        if ( x - highLevel <= 6 )
        {
            defSuc = 1;
        }
        else
        {
            defSuc = 0;
        }
        Ocount = OcountUp;
    }
    if ( middleCheck[1] && !middleCheck[0] )
    {
        if ( lowLevel - x <= 6 )
        {
            defSuc = 1;
        }
        else
        {
            defSuc = 0;
        }
        Ocount = OcountDn;
    }
    if ( ( board[highLevel][y] == 3 || board[highLevel][y] == self ) && ( board[lowLevel][y] == 3 || board[lowLevel][y] == self ) )
    {
        if ( lowLevel - highLevel <= 6 )
        {
            defSuc = 0;
        }
    }
    connectCount = connectCountUp + connectCountDn;
    if ( middleCheck[0] && middleCheck[1] )
    {
        if ( Ocount == 1 && board[highLevel][y] == 0 && board[lowLevel][y] == 0 )
        {
            defVT = T;
        }
        else
        {
            defVT = V;
        }
    }
    else
    {
        if ( middleCheck[0] && !middleCheck[1] )
        {
            if ( OcountUp == 0 && board[highLevel][y] == 0 )
            {
                defVT = T;
            }
            else
            {
                if ( OcountUp == 1 && board[x-1][y] == 0 )
                {
                    defVT = T;
                }
                else
                {
                    defVT = V;
                }
            }
        }
        if ( !middleCheck[0] && middleCheck[1] )
        {
            if ( OcountDn == 0 && board[lowLevel][y] == 0 )
            {
                defVT = T;
            }
            else
            {
                if ( OcountDn == 1 && board[x+1][y] == 0 )
                {
                    defVT = T;
                }
                else
                {
                    defVT = V;
                }
            }
        }
    }


    if ( defSuc )
    {
        if ( connectCount == 1 )
        {
            score += score_9;
        }

        // Block the four stones.
        if ( connectCount == 4 )
        {
            if ( middleCheck[0] && middleCheck[1] )
            {
                score += score_2;
                if ( step == 1 )
                {
                    if ( Ocount <= 2 )
                    {
                        m_dead_four_plus = 1;
                    }
                    add_new_pos_for_two(x,y);
                }
            }
            else
            {
                if ( step == 2 )
                {
                    if ( defVT == V )
                    {
                        score += score_2;
                    }
                }
                else
                {
                    score += score_2;
                    if ( defVT == V )
                    {
                        m_dead_four_plus = 1;
                    }
                    add_new_pos_for_two(x,y);
                }
            }
        }
        if ( connectCount == 1 )
        {
            if ( board[x-1][y] == 0 && board[x-2][y] == enemy && board[x-3][y] == 0 && board[x-4][y] == 0 && board[x-5][y] == enemy && board[x-6][y] == 0 && board[x-7][y] == 0 )
            {
                if ( step == 2 && m_dead_four_plus == 1 )
                {
                    score += score_4_7; //90
                }
                else
                {
                    score += score_5;
                    if ( step == 1 )
                    {
                        add_new_pos_for_two(x,y);
                    }
                }
            }
            if ( board[x+1][y] == 0 && board[x+2][y] == enemy && board[x+3][y] == 0 && board[x+4][y] == 0 && board[x+5][y] == enemy && board[x+6][y] == 0 && board[x+7][y] == 0 )
            {
                if ( step == 2 && m_dead_four_plus == 1 )
                {
                    score += score_4_7; //90
                }
                else
                {
                    score += score_5;
                    if ( step == 1 )
                    {
                        add_new_pos_for_two(x,y);
                    }
                }
            }
        }

        // Block the two stones.
        if ( connectCount == 2 )
        {
            if ( middleCheck[0] && middleCheck[1] && board[highLevel][y] == 0 && board[lowLevel][y] == 0 && Ocount <= 2 )
            {
                if ( step == 2 && m_dead_four_plus == 1 )
                {
                    score += score_4; //90
                }
                else
                {
                    score += score_5;
                    if ( step == 1 )
                    {
                        add_new_pos_for_two(x,y);
                    }
                }
            }
            if ( middleCheck[0] && !middleCheck[1] && board[highLevel][y] == 0 )
            {
                if ( board[x-1][y] == 0 && board[x-2][y] == 0 )
                {
                    score += 0;
                }
                else
                {
                    if ( board[x-1][y] == enemy && board[x-2][y] == 0 && board[x-3][y] == 0 && board[x-4][y] == enemy && board[x-5][y] == 0 && ( board[x-6][y] == self || board[x-6][y] == 3 ) )
                    {
                        score += 0;
                    }
                    else
                    {
                        if ( board[x-1][y] == 0 && board[x-2][y] == enemy && board[x-3][y] == 0 && board[x-4][y] == enemy && board[x-5][y] == 0 && ( board[x-6][y] == self || board[x-6][y] == 3 ) )
                        {
                            score += 0;
                        }
                        else
                        {
                            if ( board[x-1][y] == enemy && ( board[x+1][y] == self || board[x+1][y] == 3 ) )
                            {
                                score += 0;
                            }
                            else
                            {
                                if ( step == 2 && m_dead_four_plus == 1 )
                                {
                                    score += score_4; //90
                                }
                                else
                                {
                                    score += score_5;
                                    if ( step == 1 )
                                    {
                                        add_new_pos_for_two(x,y);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if ( middleCheck[1] && !middleCheck[0] && board[lowLevel][y] == 0 )
            {
                if ( board[x+1][y] == 0 && board[x+2][y] == 0 )
                {
                    score += 0;
                }
                else
                {
                    if ( board[x+1][y] == enemy && board[x+2][y] == 0 && board[x+3][y] == 0 && board[x+4][y] == enemy && board[x+5][y] == 0 && ( board[x+6][y] == self || board[x+6][y] == 3 ) )
                    {
                        score += 0;
                    }
                    else
                    {
                        if ( board[x+1][y] == 0 && board[x+2][y] == enemy && board[x+3][y] == 0 && board[x+4][y] == enemy && board[x+5][y] == 0 && ( board[x+6][y] == self || board[x+6][y] == 3 ) )
                        {
                            score += 0;
                        }
                        else
                        {
                            if ( board[x+1][y] == enemy && ( board[x-1][y] == self || board[x-1][y] == 3 ) )
                            {
                                score += 0;
                            }
                            else
                            {
                                if ( step == 2 && m_dead_four_plus == 1 )
                                {
                                    score += score_4; //90
                                }
                                else
                                {
                                    score += score_5;
                                    if ( step == 1 )
                                    {
                                        add_new_pos_for_two(x,y);
                                    }
                                }
                            }
                        }
                    }
                }
            }

        }
        // Block the three stones.
        if ( connectCount == 3 )
        {
            if ( middleCheck[0] && middleCheck[1] )
            {
                if ( defVT == T )
                {
                    if ( step == 2 && m_dead_four_plus == 1 )
                    {
                        score += score_3_3; // 200
                    }
                    else
                    {
                        score += score_4;
                        if ( step == 1 )
                        {
                            add_new_pos_for_two(x,y);
                        }
                    }
                }
                else
                {
                    if ( step == 2 && m_dead_four_plus == 1 )
                    {
                        score += score_4_6; //137
                    }
                    else
                    {
                        score += score_5_5;
                        if ( step == 1 )
                        {
                            add_new_pos_for_two(x,y);
                        }
                    }
                }
            }

            if ( middleCheck[0] && !middleCheck[1] && ( board[x-1][y] == enemy || board[x-2][y] == enemy ) )
            {
                if ( defVT == T )
                {
                    if ( step == 2 && m_dead_four_plus == 1 )
                    {
                        score += score_3_3; // 145
                    }
                    else
                    {
                        score += score_4;
                        if ( step == 1 )
                        {
                            add_new_pos_for_two(x,y);
                        }
                    }
                }
                else
                {
                    if ( step == 2 && m_dead_four_plus == 1 )
                    {
                        score += score_4_6; //80
                    }
                    else
                    {
                        score += score_5_5;
                        if ( step == 1 )
                        {
                            add_new_pos_for_two(x,y);
                        }
                    }
                }
            }

            if ( !middleCheck[0] && middleCheck[1] && ( board[x+1][y] == enemy || board[x+2][y] == enemy ) )
            {
                if ( defVT == T )
                {
                    if ( step == 2 && m_dead_four_plus == 1 )
                    {
                        score += score_3_3; // 145
                    }
                    else
                    {
                        score += score_4;
                        if ( step == 1 )
                        {
                            add_new_pos_for_two(x,y);
                        }
                    }
                }
                else
                {
                    if ( step == 2 && m_dead_four_plus == 1 )
                    {
                        score += score_4_6; //80
                    }
                    else
                    {
                        score += score_5_5;
                        if ( step == 1 )
                        {
                            add_new_pos_for_two(x,y);
                        }
                    }
                }
            }

        }
        // Block five stones.
        if ( connectCount == 5 )
        {
            score += score_2;
        }
        // Block six.
        if ( connectCount == 6 )
        {
            score += score_2;
        }
        // Block seven.
        if ( connectCount == 7 )
        {
            score += score_2;
        }
        // Block eight.
        if ( connectCount == 8 )
        {
            score += score_2;
        }
        // Block nine.
        if ( connectCount == 9 )
        {
            score += score_2;
        }
        // Block ten.
        if ( connectCount == 10 )
        {
            score += score_2;
        }
    }
    else
    {
        score = 0;
    }


    // Set the bounds of attack.
    connectCount = 0;
    Ocount = 0;
    highLevel = x;
    lowLevel = x;
    i = x;
    int sixDecreaseO_1 = 0 , sixDecreaseO_2 = 0 , sixDecreaseO = 0;
    while ( 1 )
    {
        if ( board[i][y] == self )
        {
            connectCount ++;
        }
        if ( board[i][y] == enemy )
        {
            highLevel = i;
            break;
        }
        else
        {
            if ( board[i][y] == 0 )
            {
                if ( !IsValidPos(i-1,y) )
                {
                    edgeBlock = 1;
                    canGoFive = 1;
                    highLevel = i-1;
                    sixDecreaseO_1 = 1;
                }
                else
                {
                    if ( !IsValidPos(i-2,y) )
                    {
                        edgeBlock = 1;
                        canGoFive = 1;
                        highLevel = i;
                        if ( i == x )
                        {
                            if ( board[i-1][y] == 0 )
                            {
                                sixDecreaseO_1 = 1;
                                Ocount ++;
                            }
                            if ( board[i-1][y] == enemy )
                            {
                                highLevel = i - 1;
                            }
                            if ( board[i-1][y] == self )
                            {
                                connectCount ++;
                            }
                        }
                        else
                        {
                            if ( board[i-1][y] == enemy )
                            {
                                highLevel = i - 1;
                                Ocount ++;
                            }
                            if ( board[i-1][y] == self )
                            {
                                connectCount ++;
                                Ocount ++;
                            }
                        }
                        break;
                    }
                }
                if ( board[i-1][y] == enemy )
                {
                    highLevel = i-1;
                    canGoFive = 1;
                    if ( i != x )
                    {
                        Ocount ++;
                        sixDecreaseO_1 = 1;
                    }
                    break;
                }
                if ( board[i-1][y] == 0 && board[i-2][y] == enemy )
                {
                    highLevel = i;
                    if ( i== x )
                    {
                        Ocount ++;
                        sixDecreaseO_1 = 1;
                    }

                    break;
                }

                if ( board[i-1][y] == 0 && board[i-2][y] == 0 )
                {
                    highLevel = i;
                    if ( i == x )
                    {
                        Ocount ++;
                    }
                    break;
                }
                Ocount ++;
            }
            else
            {
                if ( !IsValidPos(i,y) )
                {
                    edgeBlock = 1;
                    highLevel = i;
                    break;
                }
            }
        }
        i--;
    }
    if ( Ocount >= 1 )
    {
        Ocount --;
    }
    if ( board[x-1][y] == 0 && ( board[x-2][y] == enemy || board[x-2][y] == 3 ) )
    {
        highLevel = x-2;
    }

    i = x;
    while ( 1 )
    {
        if ( board[i][y] == self )
        {
            connectCount ++;
        }
        if ( board[i][y] == enemy )
        {
            lowLevel = i;
            break;
        }
        else
        {
            if ( board[i][y] == 0 )
            {
                if ( !IsValidPos(i+1,y) )
                {
                    edgeBlock = 1;  // Blocked by the edges.
                    canGoFive = 1;
                    sixDecreaseO_2 = 1;

                }
                else
                {
                    if ( !IsValidPos(i+2,y) )
                    {
                        edgeBlock = 1;  // Blocked by the edges.
                        canGoFive = 1;
                        lowLevel = i;
                        if ( i == x )
                        {
                            if ( board[i+1][y] == 0 )
                            {
                                sixDecreaseO_1 = 1;
                                Ocount ++;
                            }
                            if ( board[i+1][y] == enemy )
                            {
                                lowLevel = i + 1;
                            }
                            if ( board[i+1][y] == self )
                            {
                                connectCount ++;
                            }
                        }
                        else
                        {
                            if ( board[i+1][y] == enemy )
                            {
                                lowLevel = i + 1;
                                Ocount ++;
                            }
                            if ( board[i+1][y] == self )
                            {
                                connectCount ++;
                                Ocount ++;
                            }
                        }
                        break;
                    }
                }
                if ( board[i+1][y] == enemy )
                {
                    lowLevel = i+1;
                    canGoFive = 1;
                    if ( i != x )
                    {
                        sixDecreaseO_2 = 1;
                        Ocount ++;
                    }
                    break;
                }
                if ( board[i+1][y] == 0 && board[i+2][y] == enemy )
                {
                    lowLevel = i;
                    if ( i == x )
                    {
                        sixDecreaseO_2 = 1;
                        Ocount ++;
                    }
                    break;
                }
                if ( board[i+1][y] == 0 && board[i+2][y] == 0 )
                {
                    lowLevel = i;
                    if ( i == x )
                    {
                        Ocount ++;
                    }
                    break;
                }
                Ocount ++;
            }
            else
            {
                if ( !IsValidPos(i,y) )
                {
                    edgeBlock = 1;
                    highLevel = i;
                    break;
                }
            }
        }
        i++;
    }
    if ( Ocount >= 1 )
    {
        Ocount --;
    }
    if ( board[x+1][y] == 0 && ( board[x+2][y] == enemy || board[x+2][y] == 3 ) )
    {
        highLevel = x+2;
    }
    sixDecreaseO = sixDecreaseO_1 + sixDecreaseO_2;

    // Add scores for connected stones.
    if ( connectCount == 1 && board[lowLevel][y] == 0 && board[highLevel][y] == 0 )
    {
        if ( Ocount == 0 )
        {
            score += score_6;           // Connect for two.
        }
        else
        {
            if ( Ocount == 1 )
            {
                score += score_6_5;
            }
            else
            {
                if ( Ocount == 2)
                {
                    score += score_6_6;
                }
            }
        }
    }
    // For make three connection.
    if ( connectCount == 2 )
    {
        if ( board[highLevel][y] == 0 && board[lowLevel][y] == 0 )
        {
            if ( Ocount < 2 )
            {
                if ( step == 1 )
                {
                    score += score_3;

                    add_new_pos_for_two(x,y);
                }
                else
                {
                    score += score_5;
                }
            }
            if ( Ocount == 2 )
            {
                score += score_6_6;
            }
        }
        if ( ( board[highLevel][y] == 0 && board[lowLevel][y] != 0 && board[lowLevel][y] != self ) || ( board[highLevel][y] != 0 &&  board[highLevel][y] != self && board[lowLevel][y] == 0 ) )
        {
            score += score_6_6;
        }
        if ( board[lowLevel][y] != 0 && board[lowLevel][y] != self && board [highLevel][y] != 0 && board [highLevel][y] != self && lowLevel - highLevel >= 7 )
        {
            if ( Ocount == 0 )
            {
                if ( step == 1 )
                {
                    score += score_3;

                    add_new_pos_for_two(x,y);
                }
                else
                {
                    score += score_5;
                }
            }
            if ( Ocount >= 1 )
            {
                score += score_6_6;
            }
        }
    }
    // To make four.
    if ( connectCount == 3 )
    {
        if ( board[lowLevel][y] == 0 && board[highLevel][y] == 0 )
        {
            if ( Ocount == 0 )
            {
                if ( step == 1 )
                {
                    score += score_2_9;

                    add_new_pos_for_two(x,y);
                }
                else
                {
                    score += score_3;
                }
            }
            if ( Ocount >= 1 )
            {
                score += score_3_2;
            }
        }
        if ( ( board[lowLevel][y] == 0 && board[highLevel][y] != 0 && board[highLevel][y] != self ) || ( board[lowLevel][y] != 0 && board[lowLevel][y] != self && board[highLevel][y] == 0 ) )
        {
            if ( Ocount <= 2 )
            {
                score += score_3_2;
            }
        }
        if ( board[lowLevel][y] != 0 && board[lowLevel][y] != self && board[highLevel][y] != 0 && board[highLevel][y] != self && lowLevel - highLevel >= 7 )
        {
            if ( Ocount <= 2 )
            {
                score += score_3_2;
            }
        }
    }
    if ( connectCount >= 4 )
    {
        // To make five.
        if ( step == 1 )
        {
            // OOOO to make five.
            if ( board[x-1][y] == self && board[x-2][y] == self && board[x-3][y] == self && board[x-4][y] == self && ( board[x-5][y] == 0 || board[x+1][y] == 0 ) )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x+1][y] == self && board[x+2][y] == self && board[x+3][y] == self && board[x+4][y] == self && ( board[x+5][y] == 0 || board[x-1][y] == 0 ) )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            // OXOOO to make five.
            if ( board[x-1][y] == self && board[x-2][y] == 0 && board[x-3][y] == self && board[x-4][y] == self && board[x-5][y] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x+1][y] == self && board[x+2][y] == 0 && board[x+3][y] == self && board[x+4][y] == self && board[x+5][y] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x-1][y] == self && board[x+1][y] == self && board[x+2][y] == self && board[x+3][y] == self && ( board[x+4][y] == 0 || board[x-2][y] == 0 ) )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x+1][y] == self && board[x-1][y] == self && board[x-2][y] == self && board[x-3][y] == self && ( board[x-4][y] == 0 || board[x+2][y] == 0 ) )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x-1][y] == self && board[x-2][y] == self && board[x-3][y] == self && board[x-4][y] == 0 && board[x-5][y] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x+1][y] == self && board[x+2][y] == self && board[x+3][y] == self && board[x+4][y] == 0 && board[x+5][y] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            // OOXOO
            if ( board[x-1][y] == self && board[x-2][y] == self && board[x-3][y] == 0 && board[x-4][y] == self && board[x-5][y] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x+1][y] == self && board[x+2][y] == self && board[x+3][y] == 0 && board[x+4][y] == self && board[x+5][y] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x-1][y] == self && board[x-2][y] == self && board[x+1][y] == self && board[x+2][y] == self && ( board[x-3][y] == 0 || board[x+3][y] == 0 ) )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }

            // OXXOOO
            if ( board[x-1][y] == self && board[x+1][y] == 0 && board[x+2][y] == self && board[x+3][y] == self && board[x+4][y] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x+1][y] == self && board[x-1][y] == 0 && board[x-2][y] == self && board[x-3][y] == self && board[x-4][y] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x-1][y] == 0 && board[x-2][y] == self && board[x+1][y] == self && board[x+2][y] == self && board[x+3][y] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x+1][y] == 0 && board[x+2][y] == self && board[x-1][y] == self && board[x-2][y] == self && board[x-3][y] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            // OOXXOO
            if ( board[x-1][y] == self && board[x-2][y] == self && board[x+1][y] == 0 && board[x+2][y] == self && board[x+3][y] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x+1][y] == self && board[x+2][y] == self && board[x-1][y] == 0 && board[x-2][y] == self && board[x-3][y] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            // OXOXOO
            if ( board[x-1][y] == self && board[x+1][y] == self && board[x+2][y] == 0 && board[x+3][y] == self && board[x+4][y] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x+1][y] == self && board[x-1][y] == self && board[x-2][y] == 0 && board[x-3][y] == self && board[x-4][y] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x-1][y] == self && board[x-2][y] == 0 && board[x-3][y] == self && board[x+1][y] == self && board[x+2][y] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x+1][y] == self && board[x+2][y] == 0 && board[x+3][y] == self && board[x-1][y] == self && board[x-2][y] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            // OXOOXO
            if ( board[x-1][y] == self && board[x+1][y] == self && board[x+2][y] == self && board[x+3][y] == 0 && board[x+4][y] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x+1][y] == self && board[x-1][y] == self && board[x-2][y] == self && board[x-3][y] == 0 && board[x-4][y] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
        }
        else
        {
            if ( board[x+1][y] == 0 && board[x-1][y] == self && board[x-2][y] == self && board[x-3][y] == self && board[x-4][y] == self && board[x-5][y] == 0 && ( board[x-6][y] == enemy || board[x-6][y] == 3 ) )
            {
                score += score_3;
            }
            else
            {
                if ( board[x-1][y] == 0 && board[x+1][y] == self && board[x+2][y] == self && board[x+3][y] == self && board[x+4][y] == self && board[x+5][y] == 0 && ( board[x+6][y] == enemy || board[x+6][y] == 3 ) )
                {
                    score += score_3;
                }
                else
                {
                    score += score_6_6;
                }
            }
        }

        // OOOOO
        if ( board[x-1][y] == self && board[x-2][y] == self && board[x-3][y] == self && board[x-4][y] == self && board[x-5][y] == self )
        {
            score += score_1;
        }
        if ( board[x+1][y] == self && board[x+2][y] == self && board[x+3][y] == self && board[x+4][y] == self && board[x+5][y] == self )
        {
            score += score_1;
        }
        // OXOOOO
        if ( board[x-1][y] == self && board[x+1][y] == self && board[x+2][y] == self && board[x+3][y] == self && board[x+4][y] == self )
        {
            score += score_1;
        }
        if ( board[x+1][y] == self && board[x-1][y] == self && board[x-2][y] == self && board[x-3][y] == self && board[x-4][y] == self )
        {
            score += score_1;
        }
        // OOXOOO
        if ( board[x-1][y] == self && board[x-2][y] == self && board[x+1][y] == self && board[x+2][y] == self && board[x+3][y] == self )
        {
            score += score_1;
        }
        if ( board[x+1][y] == self && board[x+2][y] == self && board[x-1][y] == self && board[x-2][y] == self && board[x-3][y] == self )
        {
            score += score_1;
        }
    }
    return score;
}

// Set score by left up direction.
int CMoveGenerator::set_by_direction2 ( char color , int x , int y , int step, char board[][GRID_NUM] )
{
    int self , enemy , highLevelx , lowLevelx , highLevely , lowLevely , i , j , connectCountUp = 0 , connectCountDn = 0 , OcountUp = 0 , OcountDn = 0 , score = 0 , middleCheck[2] = {0,0} , connectCount = 0 , defSuc = 0 , defVT = 0 , canGoFive = 0 , edgeBlock = 0 , Ocount = 0;

    if ( color == 1 )
    {
        self = 1;
        enemy = 2;
    }
    else
    {
        self = 2;
        enemy = 1;
    }

    OcountUp = 0;
    OcountDn = 0;
    connectCountUp = 0;
    connectCountDn = 0;
    connectCount = 0;
    highLevelx = x;
    highLevely = y;
    lowLevelx = x;
    lowLevely = y;

    i = x;
    j = y;
    while ( OcountUp < 3 )
    {
        i --;
        j ++;
        if ( board[i][j] == enemy )
        {
            connectCountUp ++;
            middleCheck[0] = 1;
            continue;
        }
        else
        {
            if ( board[i][j] == self || !IsValidPos(i,j) )
            {
                highLevelx = i;
                highLevely = j;
                break;
            }
            else
            {
                if ( board[i][j] == 0 )
                {
                    if ( board[i-1][j+1] == 0 && board[i-2][j+2] != enemy )
                    {
                        highLevelx = i;
                        highLevely = j;
                        break;
                    }
                    else
                    {
                        OcountUp ++;
                        continue;
                    }
                }
            }
        }
    }
    if ( OcountUp == 3 )
    {
        highLevelx = i;
        highLevely = j;
        OcountUp = 2;
    }
    i = x;
    j = y;
    while ( OcountDn < 3 )
    {
        i ++;
        j --;
        if ( board[i][j] == enemy )
        {
            connectCountDn ++;
            middleCheck[1] = 1;
            continue;
        }
        else
        {
            if ( board[i][j] == self || board[i][j] == 3 )
            {
                lowLevelx = i;
                lowLevely = j;
                break;
            }
            else
            {
                if ( board[i][j] == 0 )
                {
                    if ( board[i+1][j-1] == 0 && board[i+2][j-2] != enemy )
                    {
                        lowLevelx = i;
                        lowLevely = j;
                        break;
                    }
                    else
                    {
                        OcountDn ++;
                        continue;
                    }
                }
            }
        }
    }
    if ( OcountDn == 3 )
    {
        lowLevelx = i;
        lowLevely = j;
        OcountDn = 2;
    }
    if ( middleCheck[0] && middleCheck[1] )
    {
        if ( lowLevelx - x <= 6 || x - highLevelx <= 6 )
        {
            defSuc = 1;
        }
        else
        {
            defSuc = 0;
        }
        Ocount = OcountUp + OcountDn + 1;
    }
    if ( middleCheck[0] && !middleCheck[1] )
    {
        if ( x - highLevelx <= 6 )
        {
            defSuc = 1;
        }
        else
        {
            defSuc = 0;
        }
        Ocount = OcountUp;
    }
    if ( middleCheck[1] && !middleCheck[0] )
    {
        if ( lowLevelx - x <= 6 )
        {
            defSuc = 1;
        }
        else
        {
            defSuc = 0;
        }
        Ocount = OcountDn;
    }
    if ( ( board[highLevelx][highLevely] == 3 || board[highLevelx][highLevely] == self ) && ( board[lowLevelx][lowLevely] == 3 || board[lowLevelx][lowLevely] == self ) )
    {
        if ( lowLevelx - highLevelx <= 6 )
        {
            defSuc = 0;
        }
    }
    connectCount = connectCountUp + connectCountDn;
    if ( middleCheck[0] && middleCheck[1] )
    {
        if ( Ocount == 1 && board[highLevelx][highLevely] == 0 && board[lowLevelx][lowLevely] == 0 )
        {
            defVT = T;
        }
        else
        {
            defVT = V;
        }
    }
    else
    {
        if ( middleCheck[0] && !middleCheck[1] )
        {
            if ( OcountUp == 0 && board[highLevelx][highLevely] == 0 )
            {
                defVT = T;
            }
            else
            {
                if ( OcountUp == 1 && board[x-1][y+1] == 0 )
                {
                    defVT = T;
                }
                else
                {
                    defVT = V;
                }
            }
        }
        if ( !middleCheck[0] && middleCheck[1] )
        {
            if ( OcountDn == 0 && board[lowLevelx][lowLevely] == 0 )
            {
                defVT = T;
            }
            else
            {
                if ( OcountDn == 1 && board[x+1][y-1] == 0 )
                {
                    defVT = T;
                }
                else
                {
                    defVT = V;
                }
            }
        }
    }
    if ( defSuc )
    {
        if ( connectCount == 1 )
        {
            score += score_9;
        }
        if ( connectCount == 4 )
        {
            if ( middleCheck[0] && middleCheck[1] )
            {
                score += score_2;
                if ( step == 1 )
                {
                    if ( Ocount <= 2 )
                    {
                        m_dead_four_plus = 1;
                    }
                    add_new_pos_for_two(x,y);
                }
            }
            else
            {
                if ( step == 2 )
                {
                    if ( defVT == V )
                    {
                        score += score_2;
                    }
                }
                else
                {
                    score += score_2;
                    if ( defVT == V )
                    {
                        m_dead_four_plus = 1;
                    }
                    add_new_pos_for_two(x,y);
                }
            }
        }
        if ( connectCount == 1 )
        {
            if ( board[x-1][y+1] == 0 && board[x-2][y+2] == enemy && board[x-3][y+3] == 0 && board[x-4][y+4] == 0 && board[x-5][y+5] == enemy && board[x-6][y+6] == 0 && board[x-7][y+7] == 0 )
            {
                if ( step == 2 && m_dead_four_plus == 1 )
                {
                    score += score_4_7; //90
                }
                else
                {
                    score += score_5;
                    if ( step == 1 )
                    {
                        add_new_pos_for_two(x,y);
                    }
                }
            }
            if ( board[x+1][y-1] == 0 && board[x+2][y-2] == enemy && board[x+3][y-3] == 0 && board[x+4][y-4] == 0 && board[x+5][y-5] == enemy && board[x+6][y-6] == 0 && board[x+7][y-7] == 0 )
            {
                if ( step == 2 && m_dead_four_plus == 1 )
                {
                    score += score_4_7; //90
                }
                else
                {
                    score += score_5;
                    if ( step == 1 )
                    {
                        add_new_pos_for_two(x,y);
                    }
                }
            }
        }
        if ( connectCount == 2 )
        {
            if ( middleCheck[0] && middleCheck[1] && board[highLevelx][highLevely] == 0 && board[lowLevelx][lowLevely] == 0 && Ocount <= 2 )
            {
                if ( step == 2 && m_dead_four_plus == 1 )
                {
                }
                else
                {
                    score += score_5;
                    if ( step == 1 )
                    {
                        add_new_pos_for_two(x,y);
                    }
                }
            }
            if ( middleCheck[0] && !middleCheck[1] && board[highLevelx][highLevely] == 0 )
            {
                if ( board[x-1][y+1] == 0 && board[x-2][y+2] == 0 )
                {
                    score += 0;
                }
                else
                {
                    if ( board[x-1][y+1] == enemy && board[x-2][y+2] == 0 && board[x-3][y+3] == 0 && board[x-4][y+4] == enemy && board[x-5][y+5] == 0 && ( board[x-6][y+6] == self || board[x-6][y+6] == 3 ) )
                    {
                        score += 0;
                    }
                    else
                    {
                        if ( board[x-1][y+1] == 0 && board[x-2][y+2] == enemy && board[x-3][y+3] == 0 && board[x-4][y+4] == enemy && board[x-5][y+5] == 0 && ( board[x-6][y+6] == self || board[x-6][y+6] == 3 ) )
                        {
                            score += 0;
                        }
                        else
                        {
                            if ( board[x-1][y+1] == enemy && ( board[x+1][y-1] == self || board[x+1][y-1] == 3 ) )
                            {
                                score += 0;
                            }
                            else
                            {
                                if ( step == 2 && m_dead_four_plus == 1 )
                                {
                                    score += score_4; //90
                                }
                                else
                                {
                                    score += score_5;
                                    if ( step == 1 )
                                    {
                                        add_new_pos_for_two(x,y);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if ( middleCheck[1] && !middleCheck[0] && board[lowLevelx][lowLevely] == 0 )
            {
                if ( board[x+1][y-1] == 0 && board[x+2][y-2] == 0 )
                {
                    score += 0;
                }
                else
                {
                    if ( board[x+1][y-1] == enemy && board[x+2][y-2] == 0 && board[x+3][y-3] == 0 && board[x+4][y-4] == enemy && board[x+5][y-5] == 0 && ( board[x+6][y-6] == self || board[x+6][y-6] == 3 ) )
                    {
                        score += 0;
                    }
                    else
                    {
                        if ( board[x+1][y-1] == 0 && board[x+2][y-2] == enemy && board[x+3][y-3] == 0 && board[x+4][y-4] == enemy && board[x+5][y-5] == 0 && ( board[x+6][y-6] == self || board[x+6][y-6] == 3 ) )
                        {
                            score += 0;
                        }
                        else
                        {
                            if ( board[x+1][y-1] == enemy && ( board[x-1][y+1] == self || board[x-1][y+1] == 3 ) )
                            {
                                score += 0;
                            }
                            else
                            {
                                if ( step == 2 && m_dead_four_plus == 1 )
                                {
                                }
                                else
                                {
                                    score += score_5;
                                    if ( step == 1 )
                                    {
                                        add_new_pos_for_two(x,y);
                                    }
                                }
                            }
                        }
                    }
                }
            }

        }
        if ( connectCount == 3 )
        {
            if ( middleCheck[0] && middleCheck[1] )
            {
                if ( defVT == T )
                {
                    if ( step == 2 && m_dead_four_plus == 1 )
                    {
                        score += score_3_3; // 200
                    }
                    else
                    {
                        score += score_4;
                        if ( step == 1 )
                        {
                            add_new_pos_for_two(x,y);
                        }
                    }
                }
                else
                {
                    if ( step == 2 && m_dead_four_plus == 1 )
                    {
                        score += score_4_6; //137
                    }
                    else
                    {
                        score += score_5_5;
                        if ( step == 1 )
                        {
                            add_new_pos_for_two(x,y);
                        }
                    }
                }
            }

            if ( middleCheck[0] && !middleCheck[1] && ( board[x-1][y+1] == enemy || board[x-2][y+2] == enemy ) )
            {
                if ( defVT == T )
                {
                    if ( step == 2 && m_dead_four_plus == 1 )
                    {
                        score += score_3_3; // 145
                    }
                    else
                    {
                        score += score_4;
                        if ( step == 1 )
                        {
                            add_new_pos_for_two(x,y);
                        }
                    }
                }
                else
                {
                    if ( step == 2 && m_dead_four_plus == 1 )
                    {
                        score += score_4_6; //80
                    }
                    else
                    {
                        score += score_5_5;
                        if ( step == 1 )
                        {
                            add_new_pos_for_two(x,y);
                        }
                    }
                }
            }

            if ( !middleCheck[0] && middleCheck[1] && ( board[x+1][y-1] == enemy || board[x+2][y-2] == enemy ) )
            {
                if ( defVT == T )
                {
                    if ( step == 2 && m_dead_four_plus == 1 )
                    {
                        score += score_3_3; // 145
                    }
                    else
                    {
                        score += score_4;
                        if ( step == 1 )
                        {
                            add_new_pos_for_two(x,y);
                        }
                    }
                }
                else
                {
                    if ( step == 2 && m_dead_four_plus == 1 )
                    {
                        score += score_4_6; //80
                    }
                    else
                    {
                        score += score_5_5;
                        if ( step == 1 )
                        {
                            add_new_pos_for_two(x,y);
                        }
                    }
                }
            }

        }
        if ( connectCount == 5 )
        {
            score += score_2;
        }
        if ( connectCount == 6 )
        {
            score += score_2;
        }
        if ( connectCount == 7 )
        {
            score += score_2;
        }
        if ( connectCount == 8 )
        {
            score += score_2;
        }
        if ( connectCount == 9 )
        {
            score += score_2;
        }
        if ( connectCount == 10 )
        {
            score += score_2;
        }
    }
    else
    {
        score = 0;
    }

    i = x;
    j = y;
    highLevelx = x;
    lowLevelx = x;
    highLevely = y;
    lowLevely = y;
    connectCount = 0;
    Ocount = 0;
    int sixDecreaseO = 0 , sixDecreaseO_1 = 0 , sixDecreaseO_2 = 0;
    while ( 1 )
    {
        if ( board[i][j] == self )
        {
            connectCount ++;
        }
        if ( board[i][j] == enemy )
        {
            highLevelx = i;
            highLevely = j;
            break;
        }
        else
        {
            if ( board[i][j] == 0 )
            {
                if ( !IsValidPos(i-1,j+1) )
                {
                    edgeBlock = 1;
                    canGoFive = 1;
                    sixDecreaseO_1 = 0;
                }
                else
                {
                    if ( !IsValidPos(i-2,j+2) )
                    {
                        edgeBlock = 1;
                        canGoFive = 1;
                        highLevelx = i;
                        highLevely = j;
                        if ( i == x )
                        {
                            if ( board[i-1][j+1] == 0 )
                            {
                                sixDecreaseO_1 = 1;
                                Ocount ++;
                            }
                            if ( board[i-1][j+1] == enemy )
                            {
                                highLevelx = i - 1;
                                highLevely = j + 1;
                            }
                            if ( board[i-1][j+1] == self )
                            {
                                connectCount ++;
                            }
                        }
                        else
                        {
                            if ( board[i-1][j+1] == enemy )
                            {
                                highLevelx = i - 1;
                                highLevely = j + 1;
                                Ocount ++;
                            }
                            if ( board[i-1][j+1] == self )
                            {
                                connectCount ++;
                                Ocount ++;
                            }
                        }
                        break;
                    }
                }
                if ( board[i-1][j+1] == enemy )
                {
                    highLevelx = i-1;
                    highLevely = j+1;
                    canGoFive = 1;
                    if ( i != x )
                    {
                        sixDecreaseO_1 = 0;
                        Ocount ++;
                    }
                    break;
                }
                if ( board[i-1][j+1] == 0 && board[i-2][j+2] == enemy )
                {
                    highLevelx = i;
                    highLevely = j;
                    if ( i == x )
                    {
                        sixDecreaseO_1 = 0;
                        Ocount ++;
                    }
                    break;
                }
                if ( board[i-1][j+1] == 0 && board[i-2][j+2] == 0 )
                {
                    highLevelx = i;
                    highLevely = j;
                    if ( i == x )
                    {
                        Ocount ++;
                    }
                    break;
                }
                Ocount ++;
            }
            else
            {
                if ( !IsValidPos(i,j) )
                {
                    edgeBlock = 1;
                    highLevelx = i;
                    highLevely = j;
                    break;
                }
            }
        }
        i--;
        j++;
    }
    if ( Ocount >= 1 )
    {
        Ocount --;
    }
    if ( board[x-1][y+1] == 0 && ( board[x-2][y+2] == enemy || board[x-2][y+2] == 3 ) )
    {
        highLevelx = x-2;
        highLevely = y+2;
    }

    i = x;
    j = y;
    while ( 1 )
    {
        if ( board[i][j] == self )
        {
            connectCount ++;
        }
        if ( board[i][j] == enemy )
        {
            lowLevelx = i;
            lowLevely = j;
            break;
        }
        else
        {
            if ( board[i][j] == 0 )
            {
                if ( !IsValidPos(i+1,j-1) )
                {
                    edgeBlock = 1;
                    canGoFive = 1;
                    sixDecreaseO_2 = 1;
                }
                else
                {
                    if ( !IsValidPos(i+2,j-2) )
                    {
                        edgeBlock = 1;
                        canGoFive = 1;
                        lowLevelx = i;
                        lowLevely = j;
                        if ( i == x )
                        {
                            if ( board[i+1][y-1] == 0 )
                            {
                                sixDecreaseO_1 = 1;
                                Ocount ++;
                            }
                            if ( board[i+1][j-1] == enemy )
                            {
                                lowLevelx = i + 1;
                                lowLevely = j - 1;
                            }
                            if ( board[i+1][j-1] == self )
                            {
                                connectCount ++;
                            }
                        }
                        else
                        {
                            if ( board[i+1][j-1] == enemy )
                            {
                                highLevelx = i + 1;
                                highLevely = j - 1;
                                Ocount ++;
                            }
                            if ( board[i+1][j-1] == self )
                            {
                                connectCount ++;
                                Ocount ++;
                            }
                        }
                        break;
                    }
                }
                if ( board[i+1][j-1] == enemy )
                {
                    lowLevelx = i+1;
                    lowLevely = j-1;
                    canGoFive = 1;
                    if ( i != x )
                    {
                        sixDecreaseO_2 = 1;
                        Ocount ++;
                    }
                    break;
                }
                if ( board[i+1][j-1] == 0 && board[i+2][j-2] == enemy )
                {
                    lowLevelx = i;
                    lowLevely = j;
                    if ( i == x )
                    {
                        sixDecreaseO_2 = 1;
                        Ocount ++;
                    }
                    break;
                }
                if ( board[i+1][j-1] == 0 && board[i+2][j-2] == 0 )
                {
                    lowLevelx = i;
                    lowLevely = j;
                    if ( i == x )
                    {
                        Ocount ++;
                    }
                    break;
                }
                Ocount ++;
            }
            else
            {
                if ( !IsValidPos(i,j) )
                {
                    edgeBlock = 1;
                    lowLevelx = i;
                    lowLevely = j;
                    break;
                }
            }
        }
        i++;
        j--;
    }
    if ( Ocount >= 1 )
    {
        Ocount --;
    }
    if ( board[x+1][y-1] == 0 && ( board[x+2][y-2] == enemy || board[x+2][y-2] == 3 ) )
    {
        lowLevelx = x+2;
        lowLevely = y-2;
    }
    sixDecreaseO = sixDecreaseO_1 + sixDecreaseO_2;
    if ( connectCount == 1 && board[lowLevelx][lowLevely] == 0 && board[highLevelx][highLevely] == 0 )
    {
        if ( Ocount == 0 )
        {
            score += score_6;           //
        }
        else
        {
            if ( Ocount == 1 )
            {
                score += score_6_5;      //
            }
            else
            {
                if ( Ocount == 2)
                {
                    score += score_6_6; //
                }
            }
        }
    }
    if ( connectCount == 2 )
    {
        if ( board[highLevelx][highLevely] == 0 && board[lowLevelx][lowLevely] == 0 )
        {
            if ( Ocount < 2 )
            {
                if ( step == 1 )
                {
                    score += score_3;

                    add_new_pos_for_two(x,y);
                }
                else
                {
                    score += score_5;
                }
            }
            if ( Ocount == 2 )
            {
                score += score_6_6;
            }
        }
        if ( ( board[highLevelx][highLevely] == 0 && board[lowLevelx][lowLevely] != 0 && board[lowLevelx][lowLevely] != self  ) || ( board[highLevelx][highLevely] != 0 && board[highLevelx][highLevely] != self && board[lowLevelx][lowLevely] == 0 ) )
        {
            score += score_6_6;
        }
        if ( board[lowLevelx][lowLevely] != 0 && board[lowLevelx][lowLevely] != self && board[highLevelx][highLevely] != 0 && board[highLevelx][highLevely] != self && lowLevelx - highLevelx >= 7 )
        {
            if ( Ocount == 0 )
            {
                if ( step == 1 )
                {
                    score += score_3;

                    add_new_pos_for_two(x,y);
                }
                else
                {
                    score += score_5;
                }
            }
            if ( Ocount >= 1 )
            {
                score += score_6_6;
            }
        }
    }
    if ( connectCount == 3 )
    {
        if ( board[lowLevelx][lowLevely] == 0 && board[highLevelx][highLevely] == 0 )
        {
            if ( Ocount == 0 )
            {
                if ( step == 1 )
                {
                    score += score_2_9;

                    add_new_pos_for_two(x,y);
                }
                else
                {
                    score += score_3;
                }
            }
            if ( Ocount >= 1 )
            {
                score += score_3_2;
            }
        }
        if ( ( board[lowLevelx][lowLevely] == 0 && board[highLevelx][highLevely] != 0 && board[highLevelx][highLevely] != self ) || ( board[lowLevelx][lowLevely] != 0 && board[lowLevelx][lowLevely] != self && board[highLevelx][highLevely] == 0 ) )
        {
            if ( Ocount <= 2 )
            {
                score += score_3_2;
            }
        }
        if ( board[lowLevelx][lowLevely] != 0 && board[lowLevelx][lowLevely] != self && board[highLevelx][highLevely] != 0 && board[highLevelx][highLevely] != self && lowLevelx - highLevelx >= 7 )
        {
            if ( Ocount <= 2 )
            {
                score += score_3_2;
            }
        }
    }
    if ( connectCount >= 4 )
    {
        if ( step == 1 )
        {
            if ( board[x-1][y+1] == self && board[x-2][y+2] == self && board[x-3][y+3] == self && board[x-4][y+4] == self && ( board[x-5][y+5] == 0 || board[x+1][y-1] == 0 ) )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x+1][y-1] == self && board[x+2][y-2] == self && board[x+3][y-3] == self && board[x+4][y-4] == self && ( board[x+5][y-5] == 0 || board[x-1][y+1] == 0 ) )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x-1][y+1] == self && board[x-2][y+2] == 0 && board[x-3][y+3] == self && board[x-4][y+4] == self && board[x-5][y+5] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x+1][y-1] == self && board[x+2][y-2] == 0 && board[x+3][y-3] == self && board[x+4][y-4] == self && board[x+5][y-5] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x-1][y+1] == self && board[x+1][y-1] == self && board[x+2][y-2] == self && board[x+3][y-3] == self && ( board[x+4][y-4] == 0 || board[x-2][y+2] == 0 ) )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x+1][y-1] == self && board[x-1][y+1] == self && board[x-2][y+2] == self && board[x-3][y+3] == self && ( board[x-4][y+4] == 0 || board[x+2][y-2] == 0 ) )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x-1][y+1] == self && board[x-2][y+2] == self && board[x-3][y+3] == self && board[x-4][y+4] == 0 && board[x-5][y+5] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x+1][y-1] == self && board[x+2][y-2] == self && board[x+3][y-3] == self && board[x+4][y-4] == 0 && board[x+5][y-5] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x-1][y+1] == self && board[x-2][y+2] == self && board[x-3][y+3] == 0 && board[x-4][y+4] == self && board[x-5][y+5] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x+1][y-1] == self && board[x+2][y-2] == self && board[x+3][y-3] == 0 && board[x+4][y-4] == self && board[x+5][y-5] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x-1][y+1] == self && board[x-2][y+2] == self && board[x+1][y-1] == self && board[x+2][y-2] == self && ( board[x-3][y+3] == 0 || board[x+3][y-3] == 0 ) )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }

            if ( board[x-1][y+1] == self && board[x+1][y-1] == 0 && board[x+2][y-2] == self && board[x+3][y-3] == self && board[x+4][y-4] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x+1][y-1] == self && board[x-1][y+1] == 0 && board[x-2][y+2] == self && board[x-3][y+3] == self && board[x-4][y+4] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x-1][y+1] == 0 && board[x-2][y+2] == self && board[x+1][y-1] == self && board[x+2][y-2] == self && board[x+3][y-3] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x+1][y-1] == 0 && board[x+2][y-2] == self && board[x-1][y+1] == self && board[x-2][y+2] == self && board[x-3][y+3] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x-1][y+1] == self && board[x-2][y+2] == self && board[x+1][y-1] == 0 && board[x+2][y-2] == self && board[x+3][y-3] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x+1][y-1] == self && board[x+2][y-2] == self && board[x-1][y+1] == 0 && board[x-2][y+2] == self && board[x-3][y+3] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x-1][y+1] == self && board[x+1][y-1] == self && board[x+2][y-2] == 0 && board[x+3][y-3] == self && board[x+4][y-4] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x+1][y-1] == self && board[x-1][y+1] == self && board[x-2][y+2] == 0 && board[x-3][y+3] == self && board[x-4][y+4] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x-1][y+1] == self && board[x-2][y+2] == 0 && board[x-3][y+3] == self && board[x+1][y-1] == self && board[x+2][y-2] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x+1][y-1] == self && board[x+2][y-2] == 0 && board[x+3][y-3] == self && board[x-1][y+1] == self && board[x-2][y+2] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x-1][y+1] == self && board[x+1][y-1] == self && board[x+2][y-2] == self && board[x+3][y-3] == 0 && board[x+4][y-4] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x+1][y-1] == self && board[x-1][y+1] == self && board[x-2][y+2] == self && board[x-3][y+3] == 0 && board[x-4][y+4] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
        }
        else
        {
            if ( board[x+1][y-1] == 0 && board[x-1][y+1] == self && board[x-2][y+2] == self && board[x-3][y+3] == self && board[x-4][y+4] == self && board[x-5][y+5] == 0 && ( board[x-6][y+6] == enemy || board[x-6][y+6] == 3 ) )
            {
                score += score_3;
            }
            else
            {
                if ( board[x-1][y+1] == 0 && board[x+1][y-1] == self && board[x+2][y-2] == self && board[x+3][y-3] == self && board[x+4][y-4] == self && board[x+5][y-5] == 0 && ( board[x+6][y-6] == enemy || board[x+6][y-6] == 3 ) )
                {
                    score += score_3;
                }
                else
                {
                    score += score_6_6;
                }
            }
        }

        if ( board[x-1][y+1] == self && board[x-2][y+2] == self && board[x-3][y+3] == self && board[x-4][y+4] == self && board[x-5][y+5] == self )
        {
            score += score_1;
        }
        if ( board[x+1][y-1] == self && board[x+2][y-2] == self && board[x+3][y-3] == self && board[x+4][y-4] == self && board[x+5][y-5] == self )
        {
            score += score_1;
        }
        if ( board[x-1][y+1] == self && board[x+1][y-1] == self && board[x+2][y-2] == self && board[x+3][y-3] == self && board[x+4][y-4] == self )
        {
            score += score_1;
        }
        if ( board[x+1][y-1] == self && board[x-1][y+1] == self && board[x-2][y+2] == self && board[x-3][y+3] == self && board[x-4][y+4] == self )
        {
            score += score_1;
        }
        if ( board[x-1][y+1] == self && board[x-2][y+2] == self && board[x+1][y-1] == self && board[x+2][y-2] == self && board[x+3][y-3] == self )
        {
            score += score_1;
        }
        if ( board[x+1][y-1] == self && board[x+2][y-2] == self && board[x-1][y+1] == self && board[x-2][y+2] == self && board[x-3][y+3] == self )
        {
            score += score_1;
        }
    }


    return score;
}


// Set score by horizon direction.
int CMoveGenerator::set_by_direction3 ( char color , int x , int y , int step, char board[][GRID_NUM] )
{
    int self , enemy , highLevel , lowLevel , j , connectCountUp = 0 , connectCountDn = 0 , OcountUp = 0 , OcountDn = 0 , score = 0 , middleCheck[2] = {0,0} , connectCount = 0 , defSuc = 0 , defVT = 0 , canGoFive = 0 , edgeBlock = 0 , Ocount = 0;

    if ( color == 1 )
    {
        self = 1;
        enemy = 2;
    }
    else
    {
        self = 2;
        enemy = 1;
    }

    OcountUp = 0;
    OcountDn = 0;
    connectCountUp = 0;
    connectCountDn = 0;
    connectCount = 0;
    highLevel = x;
    lowLevel = x;

    j = y;
    while ( OcountUp < 3 )
    {
        j ++;
        if ( board[x][j] == enemy )
        {
            connectCountUp ++;
            middleCheck[0] = 1;
            continue;
        }
        else
        {
            if ( board[x][j] == self || !IsValidPos(x,j) )
            {
                highLevel = j;
                break;
            }
            else
            {
                if ( board[x][j] == 0 )
                {
                    if ( board[x][j+1] == 0 && board[x][j+2] != enemy )
                    {
                        highLevel = j;
                        break;
                    }
                    else
                    {
                        OcountUp ++;
                        continue;
                    }
                }
            }
        }
    }
    if ( OcountUp == 3 )
    {
        highLevel = j;
        OcountUp = 2;
    }

    j = y;
    while ( OcountDn < 3 )
    {
        j --;
        if ( board[x][j] == enemy )
        {
            connectCountDn ++;
            middleCheck[1] = 1;
            continue;
        }
        else
        {
            if ( board[x][j] == self || board[x][j] == 3 )
            {
                lowLevel = j;
                break;
            }
            else
            {
                if ( board[x][j] == 0 )
                {
                    if ( board[x][j-1] == 0 && board[x][j-2] != enemy )
                    {
                        lowLevel = j;
                        break;
                    }
                    else
                    {
                        OcountDn ++;
                        continue;
                    }
                }
            }
        }
    }
    if ( OcountDn == 3 )
    {
        lowLevel = j;
        OcountDn = 2;
    }
    if ( middleCheck[0] && middleCheck[1] )
    {
        if ( y - lowLevel <= 6 || highLevel - y <= 6 )
        {
            defSuc = 1;
        }
        else
        {
            defSuc = 0;
        }
        Ocount = OcountUp + OcountDn + 1;
    }
    if ( middleCheck[0] && !middleCheck[1] )
    {
        if ( highLevel - y <= 6 )
        {
            defSuc = 1;
        }
        else
        {
            defSuc = 0;
        }
        Ocount = OcountUp;
    }
    if ( middleCheck[1] && !middleCheck[0] )
    {
        if ( y - lowLevel <= 6 )
        {
            defSuc = 1;
        }
        else
        {
            defSuc = 0;
        }
        Ocount = OcountDn;
    }
    if ( ( board[x][highLevel] == 3 || board[x][highLevel] == self ) && ( board[x][lowLevel] == 3 || board[x][lowLevel] == self ) )
    {
        if ( highLevel - lowLevel <= 6 )
        {
            defSuc = 0;
        }
    }
    connectCount = connectCountUp + connectCountDn;
    if ( middleCheck[0] && middleCheck[1] )
    {
        if ( Ocount == 1 && board[x][highLevel] == 0 && board[x][lowLevel] == 0 )
        {
            defVT = T;
        }
        else
        {
            defVT = V;
        }
    }
    else
    {
        if ( middleCheck[0] && !middleCheck[1] )
        {
            if ( OcountUp == 0 && board[x][highLevel] == 0 )
            {
                defVT = T;
            }
            else
            {
                if ( OcountUp == 1 && board[x][y+1] == 0 )
                {
                    defVT = T;
                }
                else
                {
                    defVT = V;
                }
            }
        }
        if ( !middleCheck[0] && middleCheck[1] )
        {
            if ( OcountDn == 0 && board[x][lowLevel] == 0 )
            {
                defVT = T;
            }
            else
            {
                if ( OcountDn == 1 && board[x][y-1] == 0 )
                {
                    defVT = T;
                }
                else
                {
                    defVT = V;
                }
            }
        }
    }
    if ( defSuc )
    {
        if ( connectCount == 1 )
        {
            score += score_9;
        }
        if ( connectCount == 4 )
        {
            if ( middleCheck[0] && middleCheck[1] )
            {
                score += score_2;
                if ( step == 1 )
                {
                    if ( Ocount <= 2 )
                    {
                        m_dead_four_plus = 1;
                    }
                    add_new_pos_for_two(x,y);
                }
            }
            else
            {
                if ( step == 2 )
                {
                    if ( defVT == V )
                    {
                        score += score_2;
                    }
                }
                else
                {
                    score += score_2;
                    if ( defVT == V )
                    {
                        m_dead_four_plus = 1;
                    }
                    add_new_pos_for_two(x,y);
                }
            }
        }
        if ( connectCount == 1 )
        {
            if ( board[x][y-1] == 0 && board[x][y-2] == enemy && board[x][y-3] == 0 && board[x][y-4] == 0 && board[x][y-5] == enemy && board[x][y-6] == 0 && board[x][y-7] == 0 )
            {
                if ( step == 2 && m_dead_four_plus == 1 )
                {
                    score += score_4_7; //90
                }
                else
                {
                    score += score_5;
                    if ( step == 1 )
                    {
                        add_new_pos_for_two(x,y);
                    }
                }
            }
            if ( board[x][y+1] == 0 && board[x][y+2] == enemy && board[x][y+3] == 0 && board[x][y+4] == 0 && board[x][y+5] == enemy && board[x][y+6] == 0 && board[x][y+7] == 0 )
            {
                if ( step == 2 && m_dead_four_plus == 1 )
                {
                    score += score_4_7; //90
                }
                else
                {
                    score += score_5;
                    if ( step == 1 )
                    {
                        add_new_pos_for_two(x,y);
                    }
                }
            }
        }
        if ( connectCount == 2 )
        {
            if ( middleCheck[0] && middleCheck[1] && board[x][highLevel] == 0 && board[x][lowLevel] == 0 && Ocount <= 2 )
            {
                if ( step == 2 && m_dead_four_plus == 1 )
                {
                    score += score_4; //90
                }
                else
                {
                    score += score_5;
                    if ( step == 1 )
                    {
                        add_new_pos_for_two(x,y);
                    }
                }
            }
            if ( middleCheck[0] && !middleCheck[1] && board[x][highLevel] == 0 )
            {
                if ( board[x][y+1] == 0 && board[x][y+2] == 0 )
                {
                    score += 0;
                }
                else
                {
                    if ( board[x][y+1] == enemy && board[x][y+2] == 0 && board[x][y+3] == 0 && board[x][y+4] == enemy && board[x][y+5] == 0 && ( board[x][y+6] == self || board[x][y+6] == 3 ) )
                    {
                        score += 0;
                    }
                    else
                    {
                        if ( board[x][y+1] == 0 && board[x][y+2] == enemy && board[x][y+3] == 0 && board[x][y+4] == enemy && board[x][y+5] == 0 && ( board[x][y+6] == self || board[x][y+6] == 3 ) )
                        {
                            score += 0;
                        }
                        else
                        {
                            if ( board[x][y+1] == enemy && ( board[x][y-1] == self || board[x][y-1] == 3 ) )
                            {
                                score += 0;
                            }
                            else
                            {
                                if ( step == 2 && m_dead_four_plus == 1 )
                                {
                                    score += score_4; //90
                                }
                                else
                                {
                                    score += score_5;
                                    if ( step == 1 )
                                    {
                                        add_new_pos_for_two(x,y);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if ( middleCheck[1] && !middleCheck[0] && board[x][lowLevel] == 0 )
            {
                if ( board[x][y-1] == 0 && board[x][y-2] == 0 )
                {
                    score += 0;
                }
                else
                {
                    if ( board[x][y-1] == enemy && board[x][y-2] == 0 && board[x][y-3] == 0 && board[x][y-4] == enemy && board[x][y-5] == 0 && ( board[x][y-6] == self || board[x][y-6] == 3 ) )
                    {
                        score += 0;
                    }
                    else
                    {
                        if ( board[x][y-1] == 0 && board[x][y-2] == enemy && board[x][y-3] == 0 && board[x][y-4] == enemy && board[x][y-5] == 0 && ( board[x][y-6] == self || board[x][y-6] == 3 ) )
                        {
                            score += 0;
                        }
                        else
                        {
                            if ( board[x][y-1] == enemy && ( board[x][y+1] == self || board[x][y+1] == 3 ) )
                            {
                                score += 0;
                            }
                            else
                            {
                                if ( step == 2 && m_dead_four_plus == 1 )
                                {
                                    score += score_4; //90
                                }
                                else
                                {
                                    score += score_5;
                                    if ( step == 1 )
                                    {
                                        add_new_pos_for_two(x,y);
                                    }
                                }
                            }
                        }
                    }
                }
            }

        }
        if ( connectCount == 3 )
        {
            if ( middleCheck[0] && middleCheck[1] )
            {
                if ( defVT == T )
                {
                    if ( step == 2 && m_dead_four_plus == 1 )
                    {
                        score += score_3_3; // 200
                    }
                    else
                    {
                        score += score_4;
                        if ( step == 1 )
                        {
                            add_new_pos_for_two(x,y);
                        }
                    }
                }
                else
                {
                    if ( step == 2 && m_dead_four_plus == 1 )
                    {
                        score += score_4_6; //137
                    }
                    else
                    {
                        score += score_5_5;
                        if ( step == 1 )
                        {
                            add_new_pos_for_two(x,y);
                        }
                    }
                }
            }

            if ( middleCheck[0] && !middleCheck[1] && ( board[x][y+1] == enemy || board[x][y+2] == enemy ) )
            {
                if ( defVT == T )
                {
                    if ( step == 2 && m_dead_four_plus == 1 )
                    {
                        score += score_3_3; // 145
                    }
                    else
                    {
                        score += score_4;
                        if ( step == 1 )
                        {
                            add_new_pos_for_two(x,y);
                        }
                    }
                }
                else
                {
                    if ( step == 2 && m_dead_four_plus == 1 )
                    {
                        score += score_4_6; //80
                    }
                    else
                    {
                        score += score_5_5;
                        if ( step == 1 )
                        {
                            add_new_pos_for_two(x,y);
                        }
                    }
                }
            }

            if ( !middleCheck[0] && middleCheck[1] && ( board[x][y-1] == enemy || board[x][y-2] == enemy ) )
            {
                if ( defVT == T )
                {
                    if ( step == 2 && m_dead_four_plus == 1 )
                    {
                        score += score_3_3; // 145
                    }
                    else
                    {
                        score += score_4;
                        if ( step == 1 )
                        {
                            add_new_pos_for_two(x,y);
                        }
                    }
                }
                else
                {
                    if ( step == 2 && m_dead_four_plus == 1 )
                    {
                        score += score_4_6; //80
                    }
                    else
                    {
                        score += score_5_5;
                        if ( step == 1 )
                        {
                            add_new_pos_for_two(x,y);
                        }
                    }
                }
            }

        }
        if ( connectCount == 5 )
        {
            score += score_2;
        }
        if ( connectCount == 6 )
        {
            score += score_2;
        }
        if ( connectCount == 7 )
        {
            score += score_2;
        }
        if ( connectCount == 8 )
        {
            score += score_2;
        }
        if ( connectCount == 9 )
        {
            score += score_2;
        }
        if ( connectCount == 10 )
        {
            score += score_2;
        }
    }
    else
    {
        score = 0;
    }
    j = y;
    highLevel = y;
    lowLevel = y;
    connectCount = 0;
    Ocount = 0;
    int sixDecreaseO = 0 , sixDecreaseO_1 = 0 , sixDecreaseO_2 = 0;
    while ( 1 )
    {
        if ( board[x][j] == self )
        {
            connectCount ++;
        }
        if ( board[x][j] == enemy )
        {
            highLevel = j;
            break;
        }
        else
        {
            if ( board[x][j] == 0 )
            {
                if ( !IsValidPos(x,j+1) )
                {
                    edgeBlock = 1;
                    canGoFive = 1;
                    sixDecreaseO_1 = 1;
                }
                else
                {
                    if ( !IsValidPos(x,j+2) )
                    {
                        edgeBlock = 1;
                        canGoFive = 1;
                        highLevel = j;
                        if ( j == y )
                        {
                            if ( board[x][j+1] == 0 )
                            {
                                sixDecreaseO_1 = 1;
                                Ocount ++;
                            }
                            if ( board[x][j+1] == enemy )
                            {
                                highLevel = j + 1;
                            }
                            if ( board[x][j+1] == self )
                            {
                                connectCount ++;
                            }
                        }
                        else
                        {
                            if ( board[x][j+1] == enemy )
                            {
                                highLevel = j + 1;
                                Ocount ++;
                            }
                            if ( board[x][j+1] == self )
                            {
                                connectCount ++;
                                Ocount ++;
                            }
                        }
                        break;
                    }
                }
                if ( board[x][j+1] == enemy )
                {
                    highLevel = j+1;
                    canGoFive = 1;
                    if ( j != y )
                    {
                        sixDecreaseO_1 = 1;
                        Ocount ++;
                    }
                    break;
                }
                if ( board[x][j+1] == 0 && board[x][j+2] == enemy )
                {
                    highLevel = j;
                    if ( j == y )
                    {
                        sixDecreaseO_1 = 1;
                        Ocount ++;
                    }
                    break;
                }
                if ( board[x][j+1] == 0 && board[x][j+2] == 0 )
                {
                    highLevel = j;
                    if ( j == y )
                    {
                        Ocount ++;
                    }
                    break;
                }
                Ocount ++;
            }
            else
            {
                if ( !IsValidPos(x,j) )
                {
                    edgeBlock = 1;
                    highLevel = j;
                    break;
                }
            }
        }
        j++;
    }
    if ( Ocount >= 1 )
    {
        Ocount --;
    }
    if ( board[x][y+1] == 0 && ( board[x][y+2] == enemy || board[x][y+2] == 3 ) )
    {
        highLevel = y+2;
    }

    j = y;
    while ( 1 )
    {
        if ( board[x][j] == self )
        {
            connectCount ++;
        }
        if ( board[x][j] == enemy )
        {
            lowLevel = j;
            break;
        }
        else
        {
            if ( board[x][j] == 0 )
            {
                if ( !IsValidPos(x,j-1) )
                {
                    edgeBlock = 1;
                    canGoFive = 1;
                    sixDecreaseO_2 = 1;
                }
                else
                {
                    if ( !IsValidPos(x,j-2) )
                    {
                        edgeBlock = 1;
                        canGoFive = 1;
                        lowLevel = j;
                        if ( j == y )
                        {
                            if ( board[x][j-1] == 0 )
                            {
                                sixDecreaseO_1 = 1;
                                Ocount ++;
                            }
                            if ( board[x][j-1] == enemy )
                            {
                                lowLevel = j - 1;
                            }
                            if ( board[x][j-1] == self )
                            {
                                connectCount ++;
                            }
                        }
                        else
                        {
                            if ( board[x][j-1] == enemy )
                            {
                                lowLevel = j - 1;
                                Ocount ++;
                            }
                            if ( board[x][j-1] == self )
                            {
                                connectCount ++;
                                Ocount ++;
                            }
                        }
                        break;
                    }
                }
                if ( board[x][j-1] == enemy )
                {
                    lowLevel = j-1;
                    canGoFive = 1;
                    if ( j != y )
                    {
                        sixDecreaseO_2 = 1;
                        Ocount ++;
                    }
                    break;
                }
                if ( board[x][j-1] == 0 && board[x][j-2] == enemy )
                {
                    lowLevel = j;
                    if ( j == y )
                    {
                        sixDecreaseO_2 = 1;
                        Ocount ++;
                    }
                    break;
                }
                if ( board[x][j-1] == 0 && board[x][j-2] == 0 )
                {
                    lowLevel = j;
                    if ( j == y )
                    {
                        Ocount ++;
                    }
                    break;
                }
                Ocount ++;
            }
            else
            {
                if ( !IsValidPos(x,j) )
                {
                    edgeBlock = 1;
                    lowLevel = j;
                    break;
                }
            }
        }
        j--;
    }
    if ( Ocount >= 1 )
    {
        Ocount --;
    }
    if ( board[x][y-1] == 0 && ( board[x][y-2] == enemy || board[x][y-2] == 3 ) )
    {
        lowLevel = y-2;
    }
    sixDecreaseO = sixDecreaseO_1 + sixDecreaseO_2;
    if ( connectCount == 1 && board[x][lowLevel] == 0 && board[x][highLevel] == 0 )
    {
        if ( Ocount == 0 )
        {
            score += score_6;           //
        }
        else
        {
            if ( Ocount == 1 )
            {
                score += score_6_5;      //
            }
            else
            {
                if ( Ocount == 2)
                {
                    score += score_6_6; //
                }
            }
        }
    }
    if ( connectCount == 2 )
    {
        if ( board[x][highLevel] == 0 && board[x][lowLevel] == 0 )
        {
            if ( Ocount < 2 )
            {
                if ( step == 1 )
                {
                    score += score_3;

                    add_new_pos_for_two(x,y);
                }
                else
                {
                    score += score_5;
                }
            }
            if ( Ocount == 2 )
            {
                score += score_6_6;
            }
        }
        if ( ( board[x][highLevel] == 0 && board[x][lowLevel] != 0 && board[x][lowLevel] != self ) || ( board[x][highLevel] != 0 && board[x][highLevel] != self && board[x][lowLevel] == 0 ) )
        {
            score += score_6_6;
        }
        if ( board[x][lowLevel] != 0 && board[x][lowLevel] != self && board [x][highLevel] != 0 && board [x][highLevel] != self && highLevel - lowLevel >= 7 )
        {
            if ( Ocount == 0 )
            {
                if ( step == 1 )
                {
                    score += score_3;

                    add_new_pos_for_two(x,y);
                }
                else
                {
                    score += score_5;
                }
            }
            if ( Ocount >= 1 )
            {
                score += score_6_6;
            }
        }
    }
    if ( connectCount == 3 )
    {
        if ( board[x][lowLevel] == 0 && board[x][highLevel] == 0 )
        {
            if ( Ocount == 0 )
            {
                if ( step == 1 )
                {
                    score += score_2_9;

                    add_new_pos_for_two(x,y);
                }
                else
                {
                    score += score_3;
                }
            }
            if ( Ocount >= 1 )
            {
                if ( Ocount <= 2 )
                {
                    score += score_3_2;
                }
            }
        }
        if ( ( board[x][lowLevel] == 0 && board[x][highLevel] != 0 && board[x][highLevel] != self ) || ( board[x][lowLevel] != 0 && board[x][lowLevel] != self && board[x][highLevel] == 0 ) )
        {
            if ( Ocount <= 2 )
            {
                score += score_3_2;
            }
        }
        if ( board[x][lowLevel] != 0 && board[x][lowLevel] != self && board[x][highLevel] != 0 && board[x][highLevel] != self && highLevel - lowLevel >= 7 )
        {
            score += score_3_2;
        }
    }
    if ( connectCount >= 4 )
    {
        if ( step == 1 )
        {
            if ( board[x][y-1] == self && board[x][y-2] == self && board[x][y-3] == self && board[x][y-4] == self && ( board[x][y-5] == 0 || board[x][y+1] == 0 ) )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x][y+1] == self && board[x][y+2] == self && board[x][y+3] == self && board[x][y+4] == self && ( board[x][y+5] == 0 || board[x][y-1] == 0 ) )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x][y-1] == self && board[x][y-2] == 0 && board[x][y-3] == self && board[x][y-4] == self && board[x][y-5] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x][y+1] == self && board[x][y+2] == 0 && board[x][y+3] == self && board[x][y+4] == self && board[x][y+5] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x][y-1] == self && board[x][y+1] == self && board[x][y+2] == self && board[x][y+3] == self && ( board[x][y+4] == 0 || board[x][y-2] == 0 ) )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x][y+1] == self && board[x][y-1] == self && board[x][y-2] == self && board[x][y-3] == self && ( board[x][y-4] == 0 || board[x][y+2] == 0 ) )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x][y-1] == self && board[x][y-2] == self && board[x][y-3] == self && board[x][y-4] == 0 && board[x][y-5] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x][y+1] == self && board[x][y+2] == self && board[x][y+3] == self && board[x][y+4] == 0 && board[x][y+5] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x][y-1] == self && board[x][y-2] == self && board[x][y-3] == 0 && board[x][y-4] == self && board[x][y-5] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x][y+1] == self && board[x][y+2] == self && board[x][y+3] == 0 && board[x][y+4] == self && board[x][y+5] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x][y-1] == self && board[x][y-2] == self && board[x][y+1] == self && board[x][y+2] == self && ( board[x][y-3] == 0 || board[x][y+3] == 0 ) )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x][y-1] == self && board[x][y+1] == 0 && board[x][y+2] == self && board[x][y+3] == self && board[x][y+4] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x][y+1] == self && board[x][y-1] == 0 && board[x][y-2] == self && board[x][y-3] == self && board[x][y-4] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x][y-1] == 0 && board[x][y-2] == self && board[x][y+1] == self && board[x][y+2] == self && board[x][y+3] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x][y+1] == 0 && board[x][y+2] == self && board[x][y-1] == self && board[x][y-2] == self && board[x][y-3] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x][y-1] == self && board[x][y-2] == self && board[x][y+1] == 0 && board[x][y+2] == self && board[x][y+3] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x][y+1] == self && board[x][y+2] == self && board[x][y-1] == 0 && board[x][y-2] == self && board[x][y-3] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x][y-1] == self && board[x][y+1] == self && board[x][y+2] == 0 && board[x][y+3] == self && board[x][y+4] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x][y+1] == self && board[x][y-1] == self && board[x][y-2] == 0 && board[x][y-3] == self && board[x][y-4] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x][y-1] == self && board[x][y-2] == 0 && board[x][y-3] == self && board[x][y+1] == self && board[x][y+2] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x][y+1] == self && board[x][y+2] == 0 && board[x][y+3] == self && board[x][y-1] == self && board[x][y-2] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x][y-1] == self && board[x][y+1] == self && board[x][y+2] == self && board[x][y+3] == 0 && board[x][y+4] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x][y+1] == self && board[x][y-1] == self && board[x][y-2] == self && board[x][y-3] == 0 && board[x][y-4] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
        }
        else
        {
            if ( board[x][y+1] == 0 && board[x][y-1] == self && board[x][y-2] == self && board[x][y-3] == self && board[x][y-4] == self && board[x][y-5] == 0 && ( board[x][y-6] == enemy || board[x][y-6] == 3 ) )
            {
                score += score_3;
            }
            else
            {
                if ( board[x][y-1] == 0 && board[x][y+1] == self && board[x][y+2] == self && board[x][y+3] == self && board[x][y+4] == self && board[x][y+5] == 0 && ( board[x][y+6] == enemy || board[x][y+6] == 3 ) )
                {
                    score += score_3;
                }
                else
                {
                    score += score_6_6;
                }
            }
        }

        if ( board[x][y-1] == self && board[x][y-2] == self && board[x][y-3] == self && board[x][y-4] == self && board[x][y-5] == self )
        {
            score += score_1;
        }
        if ( board[x][y+1] == self && board[x][y+2] == self && board[x][y+3] == self && board[x][y+4] == self && board[x][y+5] == self )
        {
            score += score_1;
        }
        if ( board[x][y-1] == self && board[x][y+1] == self && board[x][y+2] == self && board[x][y+3] == self && board[x][y+4] == self )
        {
            score += score_1;
        }
        if ( board[x][y+1] == self && board[x][y-1] == self && board[x][y-2] == self && board[x][y-3] == self && board[x][y-4] == self )
        {
            score += score_1;
        }
        if ( board[x][y-1] == self && board[x][y-2] == self && board[x][y+1] == self && board[x][y+2] == self && board[x][y+3] == self )
        {
            score += score_1;
        }
        if ( board[x][y+1] == self && board[x][y+2] == self && board[x][y-1] == self && board[x][y-2] == self && board[x][y-3] == self )
        {
            score += score_1;
        }
    }
    return score;
}

// Set scores by right up direction.
int CMoveGenerator::set_by_direction4 ( char color , int x , int y , int step, char board[][GRID_NUM] )
{
    int self , enemy , highLevelx , lowLevelx , highLevely , lowLevely , i , j , connectCountUp = 0 , connectCountDn = 0 , OcountUp = 0 , OcountDn = 0 , score = 0 , middleCheck[2] = {0,0} , connectCount = 0 , defSuc = 0 , defVT = 0 , canGoFive = 0 , edgeBlock = 0 , Ocount = 0;

    if ( color == 1 )
    {
        self = 1;
        enemy = 2;
    }
    else
    {
        self = 2;
        enemy = 1;
    }

    OcountUp = 0;
    OcountDn = 0;
    connectCountUp = 0;
    connectCountDn = 0;
    connectCount = 0;
    highLevelx = x;
    highLevely = y;
    lowLevelx = x;
    lowLevely = y;

    i = x;
    j = y;
    while ( OcountUp < 3 )
    {
        i --;
        j --;
        if ( board[i][j] == enemy )
        {
            connectCountUp ++;
            middleCheck[0] = 1;
            continue;
        }
        else
        {
            if ( board[i][j] == self || !IsValidPos(i,j) )
            {
                highLevelx = i;
                highLevely = j;
                break;
            }
            else
            {
                if ( board[i][j] == 0 )
                {
                    if ( board[i-1][j-1] == 0 && board[i-2][j-2] != enemy )
                    {
                        highLevelx = i;
                        highLevely = j;
                        break;
                    }
                    else
                    {
                        OcountUp ++;
                        continue;
                    }
                }
            }
        }
    }
    if ( OcountUp == 3 )
    {
        highLevelx = i;
        highLevely = j;
        OcountUp = 2;
    }

    i = x;
    j = y;
    while ( OcountDn < 3 )
    {
        i ++;
        j ++;
        if ( board[i][j] == enemy )
        {
            connectCountDn ++;
            middleCheck[1] = 1;
            continue;
        }
        else
        {
            if ( board[i][j] == self || board[i][j] == 3 )
            {
                lowLevelx = i;
                lowLevely = j;
                break;
            }
            else
            {
                if ( board[i][j] == 0 )
                {
                    if ( board[i+1][j+1] == 0 && board[i+2][j+2] != enemy )
                    {
                        lowLevelx = i;
                        lowLevely = j;
                        break;
                    }
                    else
                    {
                        OcountDn ++;
                        continue;
                    }
                }
            }
        }
    }
    if ( OcountDn == 3 )
    {
        lowLevelx = i;
        lowLevely = j;
        OcountDn = 2;
    }
    if ( middleCheck[0] && middleCheck[1] )
    {
        if ( lowLevelx - x <= 6 || x - highLevelx <= 6 )
        {
            defSuc = 1;
        }
        else
        {
            defSuc = 0;
        }
        Ocount = OcountUp + OcountDn + 1;
    }
    if ( middleCheck[0] && !middleCheck[1] )
    {
        if ( ( board[highLevelx][highLevely] == 3 || board[highLevelx][highLevely] == self ) && ( board[lowLevelx][lowLevely] == 3 || board[lowLevelx][lowLevely] == self ) )
        {
            if ( lowLevelx - highLevelx <= 6 )
            {
                defSuc = 0;
            }
            else
            {
                defSuc = 1;
            }
        }
        else
        {
            if ( x - highLevelx <= 6 )
            {
                defSuc = 1;
            }
            else
            {
                defSuc = 0;
            }
        }
        Ocount = OcountUp;
    }
    if ( middleCheck[1] && !middleCheck[0] )
    {
        if ( lowLevelx - x <= 6 )
        {
            defSuc = 1;
        }
        else
        {
            defSuc = 0;
        }
        Ocount = OcountDn;
    }
    if ( ( board[highLevelx][highLevely] == 3 || board[highLevelx][highLevely] == self ) && ( board[lowLevelx][lowLevely] == 3 || board[lowLevelx][lowLevely] == self ) )
    {
        if ( lowLevelx - highLevelx <= 6 )
        {
            defSuc = 0;
        }
    }
    connectCount = connectCountUp + connectCountDn;
    if ( middleCheck[0] && middleCheck[1] )
    {
        if ( Ocount == 1 && board[highLevelx][highLevely] == 0 && board[lowLevelx][lowLevely] == 0 )
        {
            defVT = T;
        }
        else
        {
            defVT = V;
        }
    }
    else
    {
        if ( middleCheck[0] && !middleCheck[1] )
        {
            if ( OcountUp == 0 && board[highLevelx][highLevely] == 0 )
            {
                defVT = T;
            }
            else
            {
                if ( OcountUp == 1 && board[x-1][y-1] == 0 )
                {
                    defVT = T;
                }
                else
                {
                    defVT = V;
                }
            }
        }
        if ( !middleCheck[0] && middleCheck[1] )
        {
            if ( OcountDn == 0 && board[lowLevelx][lowLevely] == 0 )
            {
                defVT = T;
            }
            else
            {
                if ( OcountDn == 1 && board[x+1][y+1] == 0 )
                {
                    defVT = T;
                }
                else
                {
                    defVT = V;
                }
            }
        }
    }
    if ( defSuc )
    {
        if ( connectCount == 1 )
        {
            score += score_9;
        }
        if ( connectCount == 4 )
        {
            if ( middleCheck[0] && middleCheck[1] )
            {
                score += score_2;
                if ( step == 1 )
                {
                    if ( Ocount <= 2 )
                    {
                        m_dead_four_plus = 1;
                    }
                    add_new_pos_for_two(x,y);
                }
            }
            else
            {
                if ( step == 2 )
                {
                    if ( defVT == V )
                    {
                        score += score_2;
                    }
                }
                else
                {
                    score += score_2;
                    if ( defVT == V )
                    {
                        m_dead_four_plus = 1;
                    }
                    add_new_pos_for_two(x,y);
                }
            }
        }
        if ( connectCount == 1 )
        {
            if ( board[x-1][y-1] == 0 && board[x-2][y-2] == enemy && board[x-3][y-3] == 0 && board[x-4][y-4] == 0 && board[x-5][y-5] == enemy && board[x-6][y-6] == 0 && board[x-7][y-7] == 0 )
            {
                if ( step == 2 && m_dead_four_plus == 1 )
                {
                    score += score_4_7; //90
                }
                else
                {
                    score += score_5;
                    if ( step == 1 )
                    {
                        add_new_pos_for_two(x,y);
                    }
                }
            }
            if ( board[x+1][y+1] == 0 && board[x+2][y+2] == enemy && board[x+3][y+3] == 0 && board[x+4][y+4] == 0 && board[x+5][y+5] == enemy && board[x+6][y+6] == 0 && board[x+7][y+7] == 0 )
            {
                if ( step == 2 && m_dead_four_plus == 1 )
                {
                    score += score_4_7; //90
                }
                else
                {
                    score += score_5;
                    if ( step == 1 )
                    {
                        add_new_pos_for_two(x,y);
                    }
                }
            }
        }
        if ( connectCount == 2 )
        {
            if ( middleCheck[0] && middleCheck[1] && board[highLevelx][highLevely] == 0 && board[lowLevelx][lowLevely] == 0 && Ocount <= 2 )
            {
                if ( step == 2 && m_dead_four_plus == 1 )
                {
                    score += score_4; //90
                }
                else
                {
                    score += score_5;
                    if ( step == 1 )
                    {
                        add_new_pos_for_two(x,y);
                    }
                }
            }
            if ( middleCheck[0] && !middleCheck[1] && board[highLevelx][highLevely] == 0 )
            {
                if ( board[x-1][y-1] == 0 && board[x-2][y-2] == 0 )
                {
                    score += 0;
                }
                else
                {
                    if ( board[x-1][y-1] == enemy && board[x-2][y-2] == 0 && board[x-3][y-3] == 0 && board[x-4][y-4] == enemy && board[x-5][y-5] == 0 && ( board[x-6][y-6] == self || board[x-6][y-6] == 3 ) )
                    {
                        score += 0;
                    }
                    else
                    {
                        if ( board[x-1][y-1] == 0 && board[x-2][y-2] == enemy && board[x-3][y-3] == 0 && board[x-4][y-4] == enemy && board[x-5][y-5] == 0 && ( board[x-6][y-6] == self || board[x-6][y-6] == 3 ) )
                        {
                            score += 0;
                        }
                        else
                        {
                            if ( board[x-1][y-1] == enemy && ( board[x+1][y+1] == self || board[x+1][y+1] == 3 ) )
                            {
                                score += 0;
                            }
                            else
                            {
                                if ( step == 2 && m_dead_four_plus == 1 )
                                {
                                    score += score_4; //90
                                }
                                else
                                {
                                    score += score_5;
                                    if ( step == 1 )
                                    {
                                        add_new_pos_for_two(x,y);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if ( middleCheck[1] && !middleCheck[0] && board[lowLevelx][lowLevely] == 0 )
            {
                if ( board[x+1][y+1] == 0 && board[x+2][y+2] == 0 )
                {
                    score += 0;
                }
                else
                {
                    if ( board[x+1][y+1] == enemy && board[x+2][y+2] == 0 && board[x+3][y+3] == 0 && board[x+4][y+4] == enemy && board[x+5][y+5] == 0 && ( board[x+6][y+6] == self || board[x+6][y+6] == 3 ) )
                    {
                        score += 0;
                    }
                    else
                    {
                        if ( board[x+1][y+1] == 0 && board[x+2][y+2] == enemy && board[x+3][y+3] == 0 && board[x+4][y+4] == enemy && board[x+5][y+5] == 0 && ( board[x+6][y+6] == self || board[x+6][y+6] == 3 ) )
                        {
                            score += 0;
                        }
                        else
                        {
                            if ( board[x+1][y+1] == enemy && ( board[x-1][y-1] == self || board[x-1][y-1] == 3 ) )
                            {
                                score += 0;
                            }
                            else
                            {
                                if ( step == 2 && m_dead_four_plus == 1 )
                                {
                                    score += score_4; //90
                                }
                                else
                                {
                                    score += score_5;
                                    if ( step == 1 )
                                    {
                                        add_new_pos_for_two(x,y);
                                    }
                                }
                            }
                        }
                    }
                }
            }

        }
        if ( connectCount == 3 )
        {
            if ( middleCheck[0] && middleCheck[1] )
            {
                if ( defVT == T )
                {
                    if ( step == 2 && m_dead_four_plus == 1 )
                    {
                        score += score_3_3; // 200
                    }
                    else
                    {
                        score += score_4;
                        if ( step == 1 )
                        {
                            add_new_pos_for_two(x,y);
                        }
                    }
                }
                else
                {
                    if ( step == 2 && m_dead_four_plus == 1 )
                    {
                        score += score_4_6; //137
                    }
                    else
                    {
                        score += score_5_5;
                        if ( step == 1 )
                        {
                            add_new_pos_for_two(x,y);
                        }
                    }
                }
            }

            if ( middleCheck[0] && !middleCheck[1] && ( board[x-1][y-1] == enemy || board[x-2][y-2] == enemy ) )
            {
                if ( defVT == T )
                {
                    if ( step == 2 && m_dead_four_plus == 1 )
                    {
                        score += score_3_3; // 145
                    }
                    else
                    {
                        score += score_4;
                        if ( step == 1 )
                        {
                            add_new_pos_for_two(x,y);
                        }
                    }
                }
                else
                {
                    if ( step == 2 && m_dead_four_plus == 1 )
                    {
                        score += score_4_6; //80
                    }
                    else
                    {
                        score += score_5_5;
                        if ( step == 1 )
                        {
                            add_new_pos_for_two(x,y);
                        }
                    }
                }
            }

            if ( !middleCheck[0] && middleCheck[1] && ( board[x+1][y+1] == enemy || board[x+2][y+2] == enemy ) )
            {
                if ( defVT == T )
                {
                    if ( step == 2 && m_dead_four_plus == 1 )
                    {
                        score += score_3_3; // 145
                    }
                    else
                    {
                        score += score_4;
                        if ( step == 1 )
                        {
                            add_new_pos_for_two(x,y);
                        }
                    }
                }
                else
                {
                    if ( step == 2 && m_dead_four_plus == 1 )
                    {
                        score += score_4_6; //80
                    }
                    else
                    {
                        score += score_5_5;
                        if ( step == 1 )
                        {
                            add_new_pos_for_two(x,y);
                        }
                    }
                }
            }

        }
        if ( connectCount == 5 )
        {
            score += score_2;
        }
        if ( connectCount == 6 )
        {
            score += score_2;
        }
        if ( connectCount == 7 )
        {
            score += score_2;
        }
        if ( connectCount == 8 )
        {
            score += score_2;
        }
        if ( connectCount == 9 )
        {
            score += score_2;
        }
        if ( connectCount == 10 )
        {
            score += score_2;
        }
    }
    else
    {
        score = 0;
    }
    i = x;
    j = y;
    highLevelx = x;
    lowLevelx = x;
    highLevely = y;
    lowLevely = y;
    connectCount = 0;
    Ocount = 0;
    int sixDecreaseO = 0 , sixDecreaseO_1 = 0 , sixDecreaseO_2 = 0;
    while ( 1 )
    {
        if ( board[i][j] == self )
        {
            connectCount ++;
        }
        if ( board[i][j] == enemy )
        {
            highLevelx = i;
            highLevely = j;
            break;
        }
        else
        {
            if ( board[i][j] == 0 )
            {
                if ( !IsValidPos(i-1,j-1) )
                {
                    edgeBlock = 1;
                    canGoFive = 1;
                    sixDecreaseO_1 = 1;
                }
                else
                {
                    if ( !IsValidPos(i-2,j-2) )
                    {
                        edgeBlock = 1;
                        canGoFive = 1;
                        highLevelx = i;
                        highLevely = j;
                        if ( i == x )
                        {
                            if ( board[i-1][j-1] == 0 )
                            {
                                sixDecreaseO_1 = 1;
                                Ocount ++;
                            }
                            if ( board[i-1][j-1] == enemy )
                            {
                                highLevelx = i - 1;
                                highLevely = j - 1;
                            }
                            if ( board[i-1][j-1] == self )
                            {
                                connectCount ++;
                            }
                        }
                        else
                        {
                            if ( board[i-1][j-1] == enemy )
                            {
                                highLevelx = i - 1;
                                highLevely = j - 1;
                                Ocount ++;
                            }
                            if ( board[i-1][j-1] == self )
                            {
                                connectCount ++;
                                Ocount ++;
                            }
                        }
                        break;
                    }
                }
                if ( board[i-1][j-1] == enemy )
                {
                    highLevelx = i-1;
                    highLevely = j-1;
                    canGoFive = 1;
                    if ( i != x )
                    {
                        sixDecreaseO_1 = 1;
                        Ocount ++;
                    }
                    break;
                }
                if ( board[i-1][j-1] == 0 && board[i-2][j-2] == enemy )
                {
                    highLevelx = i;
                    highLevely = j;
                    if ( i == x )
                    {
                        sixDecreaseO_1 = 1;
                        Ocount ++;
                    }
                    break;
                }
                if ( board[i-1][j-1] == 0 && board[i-2][j-2] == 0 )
                {
                    highLevelx = i;
                    highLevely = j;
                    if ( i == x )
                    {
                        Ocount ++;
                    }
                    break;
                }
                Ocount ++;
            }
            else
            {
                if ( !IsValidPos(i,j) )
                {
                    edgeBlock = 1;
                    highLevelx = i;
                    highLevely = j;
                    break;
                }
            }
        }
        i--;
        j--;
    }
    if ( Ocount >= 1 )
    {
        Ocount --;
    }
    if ( board[x-1][y-1] == 0 && ( board[x-2][y-2] == enemy || board[x-2][y-2] == 3 ) )
    {
        highLevelx = x-2;
        highLevely = y-2;
    }

    i = x;
    j = y;
    while ( 1 )
    {
        if ( board[i][j] == self )
        {
            connectCount ++;
        }
        if ( board[i][j] == enemy )
        {
            lowLevelx = i;
            lowLevely = j;
            break;
        }
        else
        {
            if ( board[i][j] == 0 )
            {
                if ( !IsValidPos(i+1,j+1) )
                {
                    edgeBlock = 1;
                    canGoFive = 1;
                    sixDecreaseO_2 = 1;
                }
                else
                {
                    if ( !IsValidPos(i+2,j+2) )
                    {
                        edgeBlock = 1;
                        canGoFive = 1;
                        lowLevelx = i;
                        lowLevely = j;
                        if ( i == x )
                        {
                            if ( board[i+1][j+1] == 0 )
                            {
                                sixDecreaseO_1 = 1;
                                Ocount ++;
                            }
                            if ( board[i+1][j+1] == enemy )
                            {
                                lowLevelx = i + 1;
                                lowLevely = j + 1;
                            }
                            if ( board[i+1][j+1] == self )
                            {
                                connectCount ++;
                            }
                        }
                        else
                        {
                            if ( board[i+1][j+1] == enemy )
                            {
                                lowLevelx = i + 1;
                                lowLevely = j + 1;
                                Ocount ++;
                            }
                            if ( board[i+1][j+1] == self )
                            {
                                connectCount ++;
                                Ocount ++;
                            }
                        }
                        break;
                    }
                }
                if ( board[i+1][j+1] == enemy )
                {
                    lowLevelx = i+1;
                    lowLevely = j+1;
                    canGoFive = 1;
                    if ( i != x )
                    {
                        sixDecreaseO_2 = 1;
                        Ocount ++;
                    }
                    break;
                }
                if ( board[i+1][j+1] == 0 && board[i+2][j+2] == enemy )
                {
                    lowLevelx = i;
                    lowLevely = j;
                    if ( i == x )
                    {
                        sixDecreaseO_2 = 1;
                        Ocount ++;
                    }
                    break;
                }
                if ( board[i+1][j+1] == 0 && board[i+2][j+2] == 0 )
                {
                    lowLevelx = i;
                    lowLevely = j;
                    if ( i == x )
                    {
                        Ocount ++;
                    }
                    break;
                }
                Ocount ++;
            }
            else
            {
                if ( !IsValidPos(i,j) )
                {
                    edgeBlock = 1;
                    lowLevelx = i;
                    lowLevely = j;
                    break;
                }
            }
        }
        i++;
        j++;
    }
    if ( Ocount >= 1 )
    {
        Ocount --;
    }
    if ( board[x+1][y+1] == 0 && ( board[x+2][y+2] == enemy || board[x+2][y+2] == 3 ) )
    {
        lowLevelx = x+2;
        lowLevely = y+2;
    }
    sixDecreaseO = sixDecreaseO_1 + sixDecreaseO_2 ;
    if ( connectCount == 1 && board[lowLevelx][lowLevely] == 0 && board[highLevelx][highLevely] == 0 )
    {
        if ( Ocount == 0 )
        {
            score += score_6;           //
        }
        else
        {
            if ( Ocount == 1 )
            {
                score += score_6_5;      //
            }
            else
            {
                if ( Ocount == 2)
                {
                    score += score_6_6; //
                }
            }
        }
    }
    if ( connectCount == 2 )
    {
        if ( board[highLevelx][highLevely] == 0 && board[lowLevelx][lowLevely] == 0 )
        {
            if ( Ocount < 2 )
            {
                if ( step == 1 )
                {
                    score += score_3;

                    add_new_pos_for_two(x,y);
                }
                else
                {
                    score += score_5;
                }
            }
            if ( Ocount == 2 )
            {
                score += score_6_6;
            }
        }
        if ( ( board[highLevelx][highLevely] == 0 && board[lowLevelx][lowLevely] != 0 && board[lowLevelx][lowLevely] != self ) || ( board[highLevelx][highLevely] != 0 && board[highLevelx][highLevely] != self && board[lowLevelx][lowLevely] == 0 ) )
        {
            score += score_6_6;
        }
        if ( board[lowLevelx][lowLevely] != 0 && board[lowLevelx][lowLevely] != self && board[highLevelx][highLevely] != 0 && board[highLevelx][highLevely] != self && lowLevelx - highLevelx >= 7 )
        {
            if ( Ocount == 0 )
            {
                if ( step == 1 )
                {
                    score += score_3;

                    add_new_pos_for_two(x,y);
                }
                else
                {
                    score += score_5;
                }
            }
            if ( Ocount >= 1 )
            {
                score += score_6_6;
            }
        }
    }
    if ( connectCount == 3 )
    {
        if ( board[lowLevelx][lowLevely] == 0 && board[highLevelx][highLevely] == 0 )
        {
            if ( Ocount == 0 )
            {
                if ( step == 1 )
                {
                    score += score_2_9;

                    add_new_pos_for_two(x,y);
                }
                else
                {
                    score += score_3;
                }
            }
            if ( Ocount >= 1 )
            {
                score += score_3_2;
            }
        }
        if ( ( board[lowLevelx][lowLevely] == 0 && board[highLevelx][highLevely] != 0 && board[highLevelx][highLevely] != self ) || ( board[lowLevelx][lowLevely] != 0 && board[lowLevelx][lowLevely] != self && board[highLevelx][highLevely] == 0 ) )
        {
            if ( Ocount <= 2 )
            {
                score += score_3_2;
            }
        }
        if ( board[lowLevelx][lowLevely] != 0 && board[lowLevelx][lowLevely] != self && board[highLevelx][highLevely] != 0 && board[highLevelx][highLevely] != self && lowLevelx - highLevelx >= 7 )
        {
            if ( Ocount <= 2 )
            {
                score += score_6_6;
            }
        }
    }
    if ( connectCount >= 4 )
    {
        if ( step == 1 )
        {
            if ( board[x-1][y-1] == self && board[x-2][y-2] == self && board[x-3][y-3] == self && board[x-4][y-4] == self && ( board[x-5][y-5] == 0 || board[x+1][y+1] == 0 ) )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x+1][y+1] == self && board[x+2][y+2] == self && board[x+3][y+3] == self && board[x+4][y+4] == self && ( board[x+5][y+5] == 0 || board[x-1][y-1] == 0 ) )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x-1][y-1] == self && board[x-2][y-2] == 0 && board[x-3][y-3] == self && board[x-4][y-4] == self && board[x-5][y-5] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x+1][y+1] == self && board[x+2][y+2] == 0 && board[x+3][y+3] == self && board[x+4][y+4] == self && board[x+5][y+5] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x-1][y-1] == self && board[x+1][y+1] == self && board[x+2][y+2] == self && board[x+3][y+3] == self && ( board[x+4][y+4] == 0 || board[x-2][y-2] == 0 ) )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x+1][y+1] == self && board[x-1][y-1] == self && board[x-2][y-2] == self && board[x-3][y-3] == self && ( board[x-4][y-4] == 0 || board[x+2][y+2] == 0 ) )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x-1][y-1] == self && board[x-2][y-2] == self && board[x-3][y-3] == self && board[x-4][y-4] == 0 && board[x-5][y-5] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x+1][y+1] == self && board[x+2][y+2] == self && board[x+3][y+3] == self && board[x+4][y+4] == 0 && board[x+5][y+5] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x-1][y-1] == self && board[x-2][y-2] == self && board[x-3][y-3] == 0 && board[x-4][y-4] == self && board[x-5][y-5] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x+1][y+1] == self && board[x+2][y+2] == self && board[x+3][y+3] == 0 && board[x+4][y+4] == self && board[x+5][y+5] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x-1][y-1] == self && board[x-2][y-2] == self && board[x+1][y+1] == self && board[x+2][y+2] == self && ( board[x-3][y-3] == 0 || board[x+3][y+3] == 0 ) )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }

            if ( board[x-1][y-1] == self && board[x+1][y+1] == 0 && board[x+2][y+2] == self && board[x+3][y+3] == self && board[x+4][y+4] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x+1][y+1] == self && board[x-1][y-1] == 0 && board[x-2][y-2] == self && board[x-3][y-3] == self && board[x-4][y-4] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x-1][y-1] == 0 && board[x-2][y-2] == self && board[x+1][y+1] == self && board[x+2][y+2] == self && board[x+3][y+3] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x+1][y+1] == 0 && board[x+2][y+2] == self && board[x-1][y-1] == self && board[x-2][y-2] == self && board[x-3][y-3] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x-1][y-1] == self && board[x-2][y-2] == self && board[x+1][y+1] == 0 && board[x+2][y+2] == self && board[x+3][y+3] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x+1][y+1] == self && board[x+2][y+2] == self && board[x-1][y-1] == 0 && board[x-2][y-2] == self && board[x-3][y-3] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x-1][y-1] == self && board[x+1][y+1] == self && board[x+2][y+2] == 0 && board[x+3][y+3] == self && board[x+4][y+4] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x+1][y+1] == self && board[x-1][y-1] == self && board[x-2][y-2] == 0 && board[x-3][y-3] == self && board[x-4][y-4] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x-1][y-1] == self && board[x-2][y-2] == 0 && board[x-3][y-3] == self && board[x+1][y+1] == self && board[x+2][y+2] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x+1][y+1] == self && board[x+2][y+2] == 0 && board[x+3][y+3] == self && board[x-1][y-1] == self && board[x-2][y-2] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x-1][y-1] == self && board[x+1][y+1] == self && board[x+2][y+2] == self && board[x+3][y+3] == 0 && board[x+4][y+4] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
            if ( board[x+1][y+1] == self && board[x-1][y-1] == self && board[x-2][y-2] == self && board[x-3][y-3] == 0 && board[x-4][y-4] == self )
            {
                score += score_1;
                add_new_pos_for_two(x,y);
            }
        }
        else
        {
            if ( board[x+1][y+1] == 0 && board[x-1][y-1] == self && board[x-2][y-2] == self && board[x-3][y-3] == self && board[x-4][y-4] == self && board[x-5][y-5] == 0 && ( board[x-6][y-6] == enemy || board[x-6][y-6] == 3 ) )
            {
                score += score_3;
            }
            else
            {
                if ( board[x-1][y-1] == 0 && board[x+1][y+1] == self && board[x+2][y+2] == self && board[x+3][y+3] == self && board[x+4][y+4] == self && board[x+5][y+5] == 0 && ( board[x+6][y+6] == enemy || board[x+6][y+6] == 3 ) )
                {
                    score += score_3;
                }
                else
                {
                    score += score_6_6;
                }
            }
        }

        if ( board[x-1][y-1] == self && board[x-2][y-2] == self && board[x-3][y-3] == self && board[x-4][y-4] == self && board[x-5][y-5] == self )
        {
            score += score_1;
        }
        if ( board[x+1][y+1] == self && board[x+2][y+2] == self && board[x+3][y+3] == self && board[x+4][y+4] == self && board[x+5][y+5] == self )
        {
            score += score_1;
        }
        if ( board[x-1][y-1] == self && board[x+1][y+1] == self && board[x+2][y+2] == self && board[x+3][y+3] == self && board[x+4][y+4] == self )
        {
            score += score_1;
        }
        if ( board[x+1][y+1] == self && board[x-1][y-1] == self && board[x-2][y-2] == self && board[x-3][y-3] == self && board[x-4][y-4] == self )
        {
            score += score_1;
        }
        if ( board[x-1][y-1] == self && board[x-2][y-2] == self && board[x+1][y+1] == self && board[x+2][y+2] == self && board[x+3][y+3] == self )
        {
            score += score_1;
        }
        if ( board[x+1][y+1] == self && board[x+2][y+2] == self && board[x-1][y-1] == self && board[x-2][y-2] == self && board[x-3][y-3] == self )
        {
            score += score_1;
        }
    }
    return score;
}

