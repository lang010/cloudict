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

#ifndef DEFINE_H_
#define DEFINE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <vector>

// Value of the Connect6 game
#define GRID_NUM            21          // Number of the board, 19*19 plus edges.
#define GRID_COUNT          361         // Sum of the points in the board.
#define BLACK               1           // Black flag in the board.
#define WHITE               2           // White flag in the board.
#define BORDER              3           // Border flag in the board.
#define NOSTONE             0           // Empty flag.

#define MSG_LENGTH          512

// Max values in the evaluation.
#define MAXINT              20000
#define MININT              -20000

// Control the depth of the search.
#define VCFDEPTH            14          // VCF default search depth.
#define ANTIVCFDEPTH        10          // Anti-VCF define search depth.
#define ANTIVCFN            5           // Anti-VCF define search depth.
// Control the width of the search
#define NUMOFONE            9           // The width of the first point in a Move.
#define NUMOFTWO            4           // The width of the second point in a Move.

// Point in the board.
typedef struct _stoneposition
{
    int x;
    int y;
} pos_t;

// One Move.
typedef struct _stonemove
{
    pos_t       positions[2];           // Point.
    double      score;                  // Score of the move.
} move_t;

// One point and its value.
typedef struct chess
{
    int x , y;
    int score;
}move_one_t;

extern int g_board_base_score[GRID_NUM-2][GRID_NUM-2];             // Base scores for each points in the board, defined in evaluation.cc

#endif

