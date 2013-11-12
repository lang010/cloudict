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

#ifndef MOVEGENERATER_H_
#define MOVEGENERATER_H_

#include "defines.h"

class CSearchEngine;

class CMoveGenerator {
    public:
        CMoveGenerator();

        int get_move_list(char ourColor , move_t* moveList, char board[][GRID_NUM]);

    private:
        int init_valuable_space(char board[][GRID_NUM]);
        int sort_merge(move_one_t list[],move_one_t listOne[],int oneN,move_one_t listTwo[],int twoN);
        bool extend_pos(char x, char y, char board[][GRID_NUM]);
        void add_new_pos_for_two(char x, char y);
        void add_new_pos_for_two_special(char x, char y);

        int set_score( char ourColor , int step , move_one_t moveList[] , char board[][GRID_NUM]  );
        int set_score_single ( char ourColor, int x, int y, int step, char board[][GRID_NUM] );
        int set_by_direction1 ( char color, int x, int y, int step, char board[][GRID_NUM] );
        int set_by_direction2 ( char color, int x, int y, int step, char board[][GRID_NUM] );
        int set_by_direction3 ( char color, int x, int y, int step, char board[][GRID_NUM] );
        int set_by_direction4 ( char color, int x, int y, int step, char board[][GRID_NUM] );

    public:
        double m_time_get_moves;
        double m_time_set_score;
        double m_time_test;

    private:
        int m_dead_four_plus;
        std::vector<pos_t> m_pos_to_update;
        std::vector<pos_t> m_pos_to_update_special;

        int map[GRID_NUM][GRID_NUM];
};

#endif

