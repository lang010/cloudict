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

#ifndef PATTERN_H
#define PATTERN_H

#include "defines.h"
#include <stdio.h>

//Pattern
typedef struct attrib
{
    int offset[2];
    /* mode */
    int mode;
} attrib_t;


/* DFA state. */
typedef struct state
{
    int att;
    int next[4];
} state_t;

/* DFA. */
typedef struct dfa
{
    /* File header. */
    char name[80];

    /* Transition graph. */
    state_t *states;
    int max_states;
    int last_state;

    /* Attributes sets. */
    attrib_t *indexes;
    int max_indexes;
    int last_index;
} dfa_t;

class CDFA {
    public:
        CDFA();
        /* Attribute list. */
        bool dfa_init();

        int pattern_match(char ourColor, move_t bestMove[], char board[][GRID_NUM]);

    private:
        bool dfa_create(dfa_t *pdfa, char str[]);
        void dfa_kill();
        void dfa_resize(dfa_t *pdfa, int max_states, int max_indexes);
        int change(int Color);
        int check(move_t bestMove[], move_t now);
        void new_match2(pos_t point, dfa_t *pdfa, move_t bestMove[], int direction);
        void new_match(pos_t point, move_t bestMove[], int ori_direction);
        void addpoint(move_t bestMove[], pos_t point);
        void match2(pos_t point, dfa_t *pdfa, move_t bestMove[], int direction, int dfa_num);
        void match(pos_t point, int direction, move_t * bestMove);
        int find(char temp);

    private:
        char    (*m_board)[GRID_NUM];
        int     m_dfa_index;
        dfa_t   m_dfa_array[10000];

        int     count, sim_c, my_color;
        FILE    *m_partin;

};

#endif

