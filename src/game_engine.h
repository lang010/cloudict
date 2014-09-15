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

#include "search_engine.h"
#include "vcf_search.h"

class CGameEngine {
    public:
        CGameEngine();
        int init(char* name);       // Init by the engine's name.
        int run();

    private:
        // Init the game board before a game.
        void init_game();
        bool search_a_move(char ourColor,move_t* bestMove);

        void on_help();

    private:
        char                m_board[GRID_NUM][GRID_NUM];        // Game board for Connect6, 19*19 plus 2 borders.
        move_t              m_best_move;                        // The pre best move.
        int                 m_chess_type;                       // The engine's chess color, black or white.
        int                 m_alphabeta_depth;
	bool                m_vcf;

        CSearchEngine       m_search_engine;
        CVCFSearch          m_vcf_search;

        char                m_engine_name[MSG_LENGTH];

};

