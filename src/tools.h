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

#ifndef DEBUGPRINT_H_
#define DEBUGPRINT_H_

#include "defines.h"

// Point (x, y) if in the valid position of the board.
#define IsValidPos(x,y)        ((x>0&&x<GRID_NUM-1 )&&(y>0&&y<GRID_NUM-1))

void init_board(char board[][GRID_NUM]);
bool is_win_by_premove(char board[][GRID_NUM], move_t* preMove);

void make_move(char board[][GRID_NUM], move_t* move, char color);
void unmake_move(char board[][GRID_NUM], move_t* move);

int log_to_file(char* msg);
int msg2move(char* msg, move_t* move);
int move2msg(move_t* move, char* msg);

int send_msg_to_slave(char* buf, int slave = 0);
int send_msg_to_master(char* buf);
int recv_msg_from_slave(char* buf,int len, int slave = 0);
int recv_msg_from_master(char* buf, int len);

int get_msg(char* buf, int maxLen);

void print_board(char tempboard[][GRID_NUM], move_t* preMove);
void print_score(move_one_t *moveList,int n);
void print_eval();

#endif

