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

#include "move_generator.h"
#include "evaluation.h"

class CSearchEngine {
    public:
        CSearchEngine();

        void before_search(char board[][GRID_NUM], char color, int m_alphabeta_depth);
        double alpha_beta_search(int depth,double alpha,double beta,char ourColor, move_t* bestMove,move_t* preMove);

    private:

    public:
        int m_total_nodes;

    private:
        char                m_board[GRID_NUM][GRID_NUM];                    // The board in the search engine.
        char                m_chess_type;
        int                 m_alphabeta_depth;
        CMoveGenerator      m_move_gernerator;
        CEvaluation         m_evaluator;

};

