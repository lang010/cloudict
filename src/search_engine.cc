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
#include "search_engine.h"

CSearchEngine::CSearchEngine()
{}

void CSearchEngine::before_search(char board[][GRID_NUM], char color, int alphabeta_depth){

    memcpy(m_board, board, sizeof(m_board));
    m_chess_type = color;
    m_alphabeta_depth = alphabeta_depth;
    // For debug.
    m_total_nodes = 0;
    m_move_gernerator.m_time_get_moves = 0;
    m_move_gernerator.m_time_set_score = 0;
    m_move_gernerator.m_time_test = 0;
    m_evaluator.m_time_evalution = 0;

}

static int move_cmp(const void* move1,const void* move2)
{
    return (int)(((move_t*)move2)->score - ((move_t*)move1)->score);
}


double CSearchEngine::alpha_beta_search(int depth,double alpha,double beta,char ourColor ,move_t* bestMove,move_t* preMove)
{
    move_t moveList[NUMOFONE * NUMOFTWO * 2];
    move_t tempBest;

    double val = 0;
    int n = 0;
    int beg, end;

    m_total_nodes++;

    if (is_win_by_premove(m_board, preMove))
    {
        if (ourColor == m_chess_type)
        {
            // Opponent wins.
            return 0;
        } else
        {
            // Self wins.
            return MININT + 1;
        }
    }
    if (depth == 0)
    {
        if (m_alphabeta_depth & 1) 
            return -m_evaluator.evaluation(m_chess_type,ourColor,m_board);
        else return m_evaluator.evaluation(m_chess_type,ourColor,m_board);
    }

    // Get move list.
    n = m_move_gernerator.get_move_list(ourColor,moveList,m_board);
    if (n < 1)
    {
        bestMove->positions[0].x = 10;
        bestMove->positions[0].y = 10;
        bestMove->positions[1].x = 10;
        bestMove->positions[1].y = 10;
        return 0;
    }
    else
    {
        bestMove->positions[0].x = moveList[0].positions[0].x;
        bestMove->positions[0].y = moveList[0].positions[0].y;
        bestMove->positions[1].x = moveList[0].positions[1].x;
        bestMove->positions[1].y = moveList[0].positions[1].y;
    }
    qsort(moveList,n,sizeof(move_t),move_cmp);
    beg = 0;
    end = n;

    double pvs_beta = beta;

    for(int i = beg ; i< end ; i++)
    {
        if (depth == m_alphabeta_depth) {
	    printf("Searching %d/%d\n", i+1, end);
	    fflush(stdout);
        }
        m_board[moveList[i].positions[0].x][moveList[i].positions[0].y] = ourColor;
        m_board[moveList[i].positions[1].x][moveList[i].positions[1].y] = ourColor;

        // Alpha beta search.
        val = -alpha_beta_search(depth-1,-beta,-alpha,ourColor^(char)3,&tempBest,&moveList[i]);
        moveList[i].score = val;

        // Unmake the move.
        m_board[moveList[i].positions[0].x][moveList[i].positions[0].y] = NOSTONE;
        m_board[moveList[i].positions[1].x][moveList[i].positions[1].y] = NOSTONE;

        // Alpha beta prune.
        if (val >= beta)
        {
            return val;
        }

        pvs_beta = alpha + 1;

        if (val > alpha)
        {
            alpha = val;
            bestMove->positions[0].x = moveList[i].positions[0].x;
            bestMove->positions[0].y = moveList[i].positions[0].y;
            bestMove->positions[1].x = moveList[i].positions[1].x;
            bestMove->positions[1].y = moveList[i].positions[1].y;
            bestMove->score = alpha;
        }
    }

    return alpha;
}

