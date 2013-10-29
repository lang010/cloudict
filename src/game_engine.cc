
#include "game_engine.h"
#include "tools.h"


CGameEngine::CGameEngine() {
    strcpy(m_engine_name, "cloudict.connect6");
}

int CGameEngine::init() {

    m_alphabeta_depth = 6;

    m_vcf_search.init(); //VCFÄ£Ê½³õÊ¼»¯
    init_game();

    return 0;
}

void CGameEngine::init_game() {
    init_board(m_board);
    m_vcf_search.init_game();
}

int CGameEngine::run()
{
    char msg[MSG_LENGTH] = {0};        //±£´æ½ÓÊÕµ½µÄÏûÏ¢

    while (1)
    {
        //Ñ­»·½ÓÊÕ²ÃÅÐÆ½Ì¨·¢ËÍµÄÏûÏ¢
        //×¢ÒâÐèÒª·¢ËÍµÄ×Ö·û´®Ó¦¸ÃÒÔ'\n'½áÊø£¬²ÃÅÐÆ½Ì¨²Å»áÈÏÎªÊÇÒ»´ÎÍêÕûµÄÊäÈë
        //·¢ËÍÍêÐèÒªµ÷ÓÃfflush(stdout)Çå¿ÕÊä³ö»º³åÇø£¬Ê¹×Ö·û´®Á¢¿ÌÊä³öµ½²ÃÅÐÆ½Ì¨
        // msg: name?, exit/quit, print, new, next, black XXXX, white XXXX, move XXXX
        //        back, forward...
        memset(msg, 0, sizeof(msg));
        fgets(msg, MSG_LENGTH, stdin);
        log_to_file(msg);
        if (strcmp(msg,"name?") == 0)
        {
            //name?
            printf("%s\n", m_engine_name);
            fflush(stdout);
            continue;
        }
        if (strcmp(msg, "exit") == 0 
                || strcmp(msg, "quit") == 0)
        {
            break;
        }
        if (strcmp(msg, "print") == 0)
        {
            print_board(m_board, &m_best_move);
        }

        //µ÷ÊÔÓÃ
        if (strncmp(msg,"black", 5) == 0)
        {
            msg2move(&msg[6], &m_best_move);
            make_move(m_board, &m_best_move, BLACK);
            m_chess_type = BLACK;
            continue;
        }
        if (strncmp(msg,"white", 5) == 0)
        {
            msg2move(&msg[6], &m_best_move);
            make_move(m_board, &m_best_move, WHITE);
            m_chess_type = WHITE;
            continue;
        }

        if (strcmp(msg,"next") == 0)
        {
            m_chess_type = m_chess_type ^ 3;
            if (search_a_move(m_chess_type, &m_best_move))
            {
                make_move(m_board, &m_best_move, m_chess_type);
                strcpy(msg, "move ");
                move2msg(&m_best_move, &msg[5]);
                printf("%s\n",msg);
            }
            continue;
        }

        if (strncmp(msg,"new", 3) == 0)
        {
            //new
            init_game();//³õÊ¼»¯ÆåÅÌ
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
        }
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
                //»ñµÃ×Å·¨µÄ×ø±ê
                move2msg(&m_best_move, &msg[5]);
                make_move(m_board, &m_best_move, m_chess_type);
                printf("%s\n",msg);
                fflush(stdout);
            }
        }//move
    }
    return 0;
}

bool CGameEngine::search_a_move(char ourColor,move_t* bestMove)
{
    double score = 0;
    bool vcf = false;

    //VCFËÑË÷²¿·Ö£¬ËÑË÷³É¹¦·µ»ØÕÐ·¨
    clock_t start,end;
    start = clock();
    m_vcf_search.before_search(m_board, m_chess_type);
    vcf = m_vcf_search.vcf_search(0,ourColor,bestMove,bestMove,0,-1);
    end = clock();

    printf("VCF time : %.3lf\n", (double)(end - start)/CLOCKS_PER_SEC);
    printf("VCF node : %d\n\n", m_vcf_search.m_vcf_node);

    if (vcf)
    {
        printf ( " Win by VCF.\n");
        return true;
    }

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

void move_back()
{
    return ;
}

