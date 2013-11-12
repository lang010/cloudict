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

#ifndef EVALUATION_H_
#define EVALUATION_H_

#include "defines.h"

// Shape names.
#define huoer           0
#define huosan          1
#define miansan         2
#define huosi           3
#define miansi          4
#define huowu           5
#define mianwu          6
#define liu             7
#define qi              8
#define ba              9
#define jiu             10
#define shi             11
#define shiyi           12
#define sisi            13
#define wuxing          14

#define black_fix 15
#define white_fix 1

// Four directions
#define DUD             0           // Up down.
#define DLR             1           // Left righ.
#define DRU             2           // Right Up.
#define DRD             3           // Right Down.

// Scores for shapes.
#define Threat          300
#define CROSSPLUS       300
#define CROSSJIAN       300

// Score index for the shapes.
#define base_1          0           //2000
#define base_2          1           //1000
#define base_3          2           //0
#define base_4          3           //-1000
#define base_5          4           //-2000
#define base_6          5           //-3000
#define base_7          6           //-4000
#define huoer_big       7           //15
#define huoer_lit       8           //10
#define huosan_big      9           //25
#define huosan_lit      10          //20
#define miansan_big     11          //10
#define miansan_lit     12          //6
#define cross_big       13          //15
#define cross_lit       14          //10

class CEvaluation {
    public:
        CEvaluation();

        double evaluation( char ourOrder , char nextStep, char board[][GRID_NUM]);

    private:
        void set_situation(char board[][GRID_NUM]);

        void set_situation_for_one_direction(int x, int y ,short countx,short county,int dir,char board[][GRID_NUM]);

    public:
        double      m_time_evalution;

    private:
        //Evaluation
        int         m_w_situation[15];
        int         m_b_situation[15];
        int         m_w_cross;
        int         m_b_cross;
        int         m_w_detail[15][15];         // Detail information for the white in the board.
        int         m_b_detail[15][15];         // Detail information for the black.
        char        m_visited_direction[20][20][4];

        int         m_w_vir_detail[15][15];     // Virtual cross information for white.
        int         m_b_vir_detail[15][15];
        char        m_visited_virtual_direction[20][20][4];

        int         b_mean_point;
        int         w_mean_point;

};

#endif

