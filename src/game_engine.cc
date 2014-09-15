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

#include "game_engine.h"
#include "tools.h"


CGameEngine::CGameEngine() {
    strcpy(m_engine_name, "Cloudict.Connect6");
}

int CGameEngine::init(char* name) {

    if (name != NULL && strlen(name) > 0) {
        if (strlen(name) < MSG_LENGTH) {
            strcpy(m_engine_name, name);
        }else {
            printf("To long Engine Name: %s, should less than: %d\n",
                    name, MSG_LENGTH);
        }
    }

    m_alphabeta_depth = 6;
    m_vcf = true;

    m_vcf_search.init(); // VCF engine init the dfa, only once when engine started.
    init_game();

    return 0;
}

void CGameEngine::init_game() {
    init_board(m_board);
    m_vcf_search.init_game();
}

void CGameEngine::on_help() {
    printf(
        "On help for GameEngine %s\n\
            name        - print the name of the Game Engine.\n\
            print       - print the board.\n\
            exit/quit   - quit the game.\n\
            black XXXX  - place the black stone on the position XXXX in the board.\n\
            white XXXX  - place the write stone on the XXXX in the board, X is the A-S.\n\
            next        - the engine will search the move for next step.\n\
            move XXXX   - tell the engine that the opponent take the move XXXX,\n\
                            and the engine will search the move for next step.\n\
            new black   - start a new game and set the engine to Black player.\n\
            new white   - start a new game and set it to White.\n\
            depth d     - set the alpha beta search depth, default is 6.\n\
            vcf         - set vcf search.\n\
            unvcf       - set none vcf search.\n\
            help        - print this help.\n", m_engine_name);
}

int CGameEngine::run()
{
    char msg[MSG_LENGTH] = {0};

    on_help();
    while (1)
    {
        // Take in the commands.
        memset(msg, 0, sizeof(msg));
        get_msg(msg, MSG_LENGTH);
        log_to_file(msg);
        if (strcmp(msg,"name") == 0)
        {
            //name
            printf("name %s\n", m_engine_name);
            fflush(stdout);
            continue;
        } else
        if (strcmp(msg, "exit") == 0 
                || strcmp(msg, "quit") == 0)
        {
            break;
        } else
        if (strcmp(msg, "print") == 0)
        {
            print_board(m_board, &m_best_move);
        } else if (strcmp(msg, "vcf") == 0) {
	    m_vcf = true;
	} else if (strcmp(msg, "unvcf") == 0) {
	    m_vcf = false;
	} else
        if (strncmp(msg,"black", 5) == 0)
        {
            // Set the position by black stone
            msg2move(&msg[6], &m_best_move);
            make_move(m_board, &m_best_move, BLACK);
            m_chess_type = BLACK;
            continue;
        } else
        if (strncmp(msg,"white", 5) == 0)
        {
            // Set the position by white stone
            msg2move(&msg[6], &m_best_move);
            make_move(m_board, &m_best_move, WHITE);
            m_chess_type = WHITE;
            continue;
        } else
        if (strcmp(msg,"next") == 0)
        {
            m_chess_type = m_chess_type ^ 3;
            if (search_a_move(m_chess_type, &m_best_move))
            {
                make_move(m_board, &m_best_move, m_chess_type);
                strcpy(msg, "move ");
                move2msg(&m_best_move, &msg[5]);
                printf("%s\n",msg);
		fflush(stdout);
            }
            continue;
        } else
        if (strncmp(msg,"new", 3) == 0)
        {
            //New game.
            init_game();// Begin a new game.
            if (strcmp(&msg[4],"black") == 0)
            {
                //new black
                msg2move("JJ", &m_best_move);
                make_move(m_board, &m_best_move, BLACK);
                m_chess_type = BLACK;
                printf("move JJ\n");
                fflush(stdout);
                continue;
            }
            else
            {
                //new white
                m_chess_type = WHITE;
                continue;
            }

            continue;
        } else
        if (strncmp(msg,"move", 4) == 0)
        {
            //move
            msg2move(&msg[5], &m_best_move);
            make_move(m_board, &m_best_move, m_chess_type^3);
            if (is_win_by_premove(m_board, &m_best_move))
            {
                printf("We loss!");
                fflush(stdout);
                continue;
            }
            if (search_a_move(m_chess_type,&m_best_move))
            {
                // Search for a good move and take it.
                move2msg(&m_best_move, &msg[5]);
                make_move(m_board, &m_best_move, m_chess_type);
                printf("%s\n",msg);
                fflush(stdout);
            }
        } else
        if (strncmp(msg, "depth", 5) == 0) {
            fflush(stdin);
            fflush(stdout);
            int d = 0;
            sscanf(&msg[5], "%d", &d);
            if (d > 0 && d < 10) {
                m_alphabeta_depth = d;
            }
            fflush(stdout);
            printf("Set the search depth to %d.\n\n", m_alphabeta_depth);
            fflush(stdout);
        } else
        if (strncmp(msg, "help", 4) == 0) {
            on_help();
        }
    }
    return 0;
}

bool CGameEngine::search_a_move(char ourColor,move_t* bestMove)
{
    double score = 0;
    clock_t start,end;

    // VCF Search first, for the position.
    if (m_vcf) {
        bool vcf = false;
	start = clock();
	m_vcf_search.before_search(m_board, m_chess_type);
	vcf = m_vcf_search.vcf_search(0,ourColor,bestMove,bestMove,0,-1);
	end = clock();

	printf("VCF time : %.3lf\n", (double)(end - start)/CLOCKS_PER_SEC);
	// Check if wins.
	if (vcf) {
	    printf("Win by VCF.\n");
	    return true;
	}

	printf("VCF node : %d\n\n", m_vcf_search.m_vcf_node);
    }


    // Alpha beta search continue, if VCF search failed.
    start = clock();
    m_search_engine.before_search(m_board, m_chess_type, m_alphabeta_depth);
    score = m_search_engine.alpha_beta_search(m_alphabeta_depth,MININT,MAXINT,ourColor,bestMove,bestMove);
    end = clock();

    printf("AB Time :\t%.3lf\n",(double)(end - start)/CLOCKS_PER_SEC);
    //printf("Get Moves Time :\t%.3lf\n", m_time_get_moves);
    //printf("Set Score Time :\t%.3lf\n", m_time_set_score);
    //printf("Test Time :\t%.3lf\n", m_time_test);
    //printf("Evaluate Time :\t%.3lf\n", m_time_evalution);
    printf("Node :\t%d\n\n", m_search_engine.m_total_nodes);
    printf("Score :\t%.3lf\n",score);

    return true;
}


