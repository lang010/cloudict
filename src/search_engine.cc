
#include "tools.h"
#include "search_engine.h"

CSearchEngine::CSearchEngine()
{}
//搜索引擎

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
            //对手赢
            return 0;
        } else
        {
            //自己赢
            return MININT + 1;
        }
    }
    if (depth == 0)
    {
        if (m_alphabeta_depth & 1) 
            return -m_evaluator.evaluation(m_chess_type,ourColor,m_board);
        else return m_evaluator.evaluation(m_chess_type,ourColor,m_board);
    }

    //招法生成
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
        //执行招法
        m_board[moveList[i].positions[0].x][moveList[i].positions[0].y] = ourColor;
        m_board[moveList[i].positions[1].x][moveList[i].positions[1].y] = ourColor;

        //递归搜索
        // PVS algorithm
        val = -alpha_beta_search(depth-1,-beta,-alpha,ourColor^(char)3,&tempBest,&moveList[i]);
        //if (alpha < val && val < beta && i > 0) {
        //    val = -alpha_beta_search(depth-1,-beta,-alpha,ourColor^(char)3,&tempBest,&moveList[i]);
        //}
        moveList[i].score = val;

        //撤销招法
        m_board[moveList[i].positions[0].x][moveList[i].positions[0].y] = NOSTONE;
        m_board[moveList[i].positions[1].x][moveList[i].positions[1].y] = NOSTONE;

        //剪枝
        if (val >= beta)
        {
            return val;
        }

        pvs_beta = alpha + 1;

        //已经找到必胜的招法
        if (val > alpha)
        {
            alpha = val;
            //memcpy(bestMove, &moveList[i], sizeof(move_t));
            bestMove->positions[0].x = moveList[i].positions[0].x;
            bestMove->positions[0].y = moveList[i].positions[0].y;
            bestMove->positions[1].x = moveList[i].positions[1].x;
            bestMove->positions[1].y = moveList[i].positions[1].y;
            bestMove->score = alpha;
        }
    }

    return alpha;
}

