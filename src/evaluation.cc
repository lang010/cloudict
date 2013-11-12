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

#include "evaluation.h"
#include "tools.h"


int g_board_base_score[GRID_NUM-2][GRID_NUM-2] =             // Base scores for the board, used in move generator and evaluation.
{
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0},
    {0,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,0},
    {0,1,2,3,4,4,4,4,4,4,4,4,4,4,4,3,2,1,0},
    {0,1,2,3,4,5,5,5,5,5,5,5,5,5,4,3,2,1,0},
    {0,1,2,3,4,5,6,6,6,6,6,6,6,5,4,3,2,1,0},
    {0,1,2,3,4,5,6,7,7,7,7,7,6,5,4,3,2,1,0},
    {0,1,2,3,4,5,6,7,8,8,8,7,6,5,4,3,2,1,0},
    {0,1,2,3,4,5,6,7,8,9,8,7,6,5,4,3,2,1,0},
    {0,1,2,3,4,5,6,7,8,8,8,7,6,5,4,3,2,1,0},
    {0,1,2,3,4,5,6,7,7,7,7,7,6,5,4,3,2,1,0},
    {0,1,2,3,4,5,6,6,6,6,6,6,6,5,4,3,2,1,0},
    {0,1,2,3,4,5,5,5,5,5,5,5,5,5,4,3,2,1,0},
    {0,1,2,3,4,4,4,4,4,4,4,4,4,4,4,3,2,1,0},
    {0,1,2,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,0},
    {0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

int dna[16] = {2000 , 1000, 0 ,-1000 ,-2000 ,-3000 ,-4000 ,15 ,10 ,25 ,20, 10, 6 ,15 , 10,0};

CEvaluation::CEvaluation()
{
}

double CEvaluation::evaluation( char ourOrder , char nextStep, char board[][GRID_NUM])
{
    double score = 0;
    int i,j = 0;
    clock_t beg,end;
    beg = clock();

    set_situation(board);

    // Odd level evaluation
    if ( ourOrder == nextStep )
    {
        // Our is black
        if ( ourOrder == BLACK )
        {
            if (m_b_situation[huosi]*2 + m_b_situation[huowu]*2 + m_b_situation[miansi] + m_b_situation[mianwu] >= 1 || (m_b_situation[liu]+m_b_situation[qi]+m_b_situation[ba]+m_b_situation[jiu]!=0))
            {
                return MAXINT -1;
            }
            else
                if ( ( m_w_situation[huosi]*2 + m_w_situation[huowu]*2 + m_w_situation[mianwu] + m_w_situation[miansi] >= 3 ) )
                {
                    return 0;
                }
                else
                {
                    if(m_w_situation[huosi]*2 + m_w_situation[huowu]*2 + m_w_situation[mianwu] + m_w_situation[miansi] == 2)  //7-5
                    {
                        if(m_w_situation[huosan]>=2||(m_w_situation[huosan]==1&&m_w_situation[miansan]>=1))
                        {
                            return 1;
                        }
                        else
                        {
                            if(m_w_situation[huosan]>=1)   //7
                            {
                                score = dna[base_7];
                                score += m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                score -= m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                            }
                            else
                            {
                                if(m_w_situation[huoer]>=2&&(m_w_detail[huoer][huosan]+m_w_detail[huoer][huoer]+m_w_detail[huoer][miansan] + m_w_detail[huoer][huosi] + m_w_detail[huoer][sisi] + m_w_vir_detail[huoer][huoer] + m_w_vir_detail[huoer][miansan] )>0)  //7
                                {
                                    score = dna[base_7];
                                    score += m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                    score -= m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                }
                                else
                                {
                                    if (m_w_situation[huoer]>=1&&(m_w_detail[huoer][huosan]+m_w_detail[huoer][huoer]+m_w_detail[huoer][miansan] + m_w_detail[huoer][huosi] + m_w_detail[huoer][sisi] + m_w_vir_detail[huoer][huoer] + m_w_vir_detail[huoer][miansan] )>0)
                                    {
                                        score = dna[base_7];
                                        score += m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                        score -= m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                    }
                                    else
                                    {
                                        if( m_w_situation[miansan] >= 2 && ( m_w_detail[miansan][sisi] + m_w_detail[miansan][miansan] + m_w_vir_detail[miansan][miansan] + m_w_vir_detail[huoer][miansan] ) > 0 ) //7
                                        {
                                            score = dna[base_7];
                                            score += m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                            score -= m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                        }
                                        else
                                        {
                                            if(m_w_situation[miansan]>=2)   //6
                                         {
                                             score = dna[base_6];
                                             score += m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                             score -= m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                         }
                                        else
                                         {
                                             if( m_w_situation[huoer]>=1 )  //6
                                             {
                                                 score = dna[base_6];
                                                 score += m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                                 score -= m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                             }
                                             else //5
                                             {
                                                 if ( m_w_situation[miansan] > 0 && m_w_situation[huoer] == 0 )
                                                 {
                                                     if ( m_b_situation[miansan] >= m_w_situation[miansan] && m_b_situation[huoer] + m_b_situation[huosan] >= 3 && m_b_detail[huoer][huoer] + m_b_detail[huoer][miansan] + m_b_vir_detail[huoer][huoer] + m_b_vir_detail[huoer][miansan] > 0 )
                                                     {
                                                         score = dna[base_2];
                                                         score += m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                                         score -= m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                                     }
                                                     else
                                                     {
                                                         if ( m_b_situation[miansan] >= m_w_situation[miansan] && m_b_situation[huoer] + m_b_situation[huosan] == 2 && m_b_detail[huoer][huoer] + m_b_detail[huoer][miansan] + m_b_vir_detail[huoer][huoer] + m_b_vir_detail[huoer][miansan] > 0 )
                                                         {
                                                             if ( m_b_situation[huosan] > 0 )
                                                             {
                                                                 score = dna[base_2];
                                                             }
                                                             else
                                                             {
                                                                 score = dna[base_3];
                                                             }
                                                             score += m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                                             score -= m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                                         }
                                                         else
                                                         {
                                                             score = dna[base_6];
                                                             score += m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                                             score -= m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                                         }
                                                     }
                                                 }
                                                 else
                                                 {
                                                     if ( m_w_situation[miansan] > 0 )
                                                     {
                                                         score = dna[base_6];
                                                         score += m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                                         score -= m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                                     }
                                                     else
                                                     {
                                                         score = dna[base_5];
                                                         score += m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                                         score -= m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                                     }
                                                 }
                                             }
                                         }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        if(m_w_situation[huosi]*2 + m_w_situation[huowu]*2 + m_w_situation[mianwu] + m_w_situation[miansi] == 1)
                        {
                            if(m_b_situation[huosan]>=2)  //2
                            {
                                score = dna[base_1];
                                score += m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                score -= m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                            }
                            else
                            {
                                if ( m_b_situation[huosan] == 1 )
                                {
                                    if ( m_b_situation[huoer] >= 2 && ( m_b_detail[huoer][huoer] + m_b_detail[huoer][miansan] + m_b_vir_detail[huoer][huoer] + m_b_vir_detail[huoer][miansan] ) > 0 )
                                    {
                                        score = dna[base_1];
                                    }
                                    else
                                    {
                                        if ( m_b_situation[miansan] >= 2 && ( m_b_detail[miansan][miansan] + m_b_vir_detail[miansan][miansan] + m_b_detail[huoer][miansan] + m_b_vir_detail[huoer][miansan] ) > 0 )
                                        {
                                            score = dna[base_1];
                                        }
                                        else
                                        {
                                            if ( m_b_situation[huoer] > 0 || m_b_situation[miansan] >= 2 )
                                            {
                                                score = dna[base_3];
                                            }
                                            else
                                            {
                                                score = dna[base_5];
                                            }
                                        }
                                    }
                                    score += m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                    score -= m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                }
                                else
                                {
                                    if ( m_w_situation[huosan] >= 2 )
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        if ( m_w_situation[huosan] == 1 )
                                        {
                                            if ( m_w_situation[huoer] > 0 )
                                            {
                                                score = dna[base_7];
                                                score += m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                                score -= m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];

                                            }
                                            else
                                            {
                                                if ( m_w_situation[huosan] + m_w_situation[miansan] <= 1 )
                                                {
                                                    score = dna[base_3];
                                                    score += m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                                    score -= m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                                }
                                                else
                                                {
                                                    if ( m_b_situation[miansan] - m_w_situation[miansan] - m_w_situation[huosan] >= 0 && m_b_situation[huoer] >= 2 )
                                                    {
                                                        score = dna[base_3];
                                                        score += m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                                        score -= m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                                    }
                                                    else
                                                    {
                                                        if ( m_b_situation[miansan] < m_w_situation[huosan] + m_w_situation[miansan] )
                                                        {
                                                            score = dna[base_6];
                                                            score += m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                                            score -= m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                                        }
                                                        else
                                                        {
                                                            score = dna[base_5];
                                                            score += m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                                            score -= m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        else
                                        {
                                            if(m_b_situation[miansan]>=1)  //3
                                            {
                                                if ( m_b_situation[miansan] > m_w_situation[miansan] && m_b_situation[huoer] > 0 )
                                                {
                                                    score = dna[base_2];
                                                    score += m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                                    score -= m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                                }
                                                else
                                                {
                                                    if ( m_b_situation[miansan] >= m_w_situation[miansan] )
                                                    {
                                                        score = dna[base_3];
                                                        score += m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                                        score -= m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                                    }
                                                    else
                                                    {
                                                        if ( m_w_situation[miansan] > m_b_situation[miansan] && m_w_situation[huoer] > 0 )
                                                        {
                                                            score = dna[base_7];
                                                            score += m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                                            score -= m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                                        }
                                                        else
                                                        {
                                                            score = dna[base_5];
                                                            score += m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                                            score -= m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                                        }
                                                    }
                                                }


                                            }
                                            else
                                            {
                                                if ( m_w_situation[huoer] >= 2 )
                                                {
                                                    score = dna[base_7];score += m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                                    score -= m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                                }
                                                else
                                                {
                                                    if ( m_w_situation[miansan] >= 2 && m_b_situation[huoer] == 0 )
                                                    {
                                                        score = dna[base_6];
                                                        score += m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                                        score -= m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                                    }
                                                    else
                                                    {
                                                        if ( m_w_situation[miansan] == 1 && m_w_situation[huoer] == 1 )
                                                        {
                                                            score = dna[base_6];
                                                            score += m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                                            score -= m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                                        }
                                                        else
                                                        {
                                                            score = dna[base_5];
                                                            score += m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                                            score -= m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            if(m_b_situation[huosan]>=1) //1
                            {
                                score=dna[base_1];
                                score += m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                score -= m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                            }
                            else
                            {
                                if(m_b_situation[huoer] >= 2 && (m_b_detail[huoer][huoer] + m_b_detail[huoer][miansan] + m_b_detail[huoer][sisi] + m_b_vir_detail[huoer][huoer] + m_b_vir_detail[huoer][miansan] ) > 0 )  //1
                                {
                                    score = dna[base_1];
                                    score += m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                    score -= m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                }
                                else
                                {
                                    if ( m_b_situation[miansan] - m_w_situation[miansan] >= 1 && m_b_detail[huoer][huoer] + m_b_detail[huoer][miansan] + m_b_vir_detail[huoer][huoer] + m_b_vir_detail[huoer][miansan] > 0 && m_w_situation[huosan] == 0 && m_w_vir_detail[huoer][miansan] == 0 )
                                    {
                                        score = dna[base_3];
                                        score += m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                        score -= m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                    }
                                    else
                                    {
                                        if(m_b_situation[huoer]==1 &&( m_b_detail[huoer][sisi] +m_b_detail[huoer][miansan] + m_b_detail[huoer][miansan] + m_b_vir_detail[huoer][miansan])> 0) //2
                                        {
                                            score = dna[base_2];
                                            score += m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                            score -= m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                        }
                                        else
                                        {
                                            if( m_b_situation[miansan] >= 2 && m_b_detail[miansan][miansan] + m_b_vir_detail[miansan][miansan] > 0 )  //2
                                            {
                                                score = dna[base_2];
                                                score += m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                                score -= m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                            }
                                            else
                                            {
                                                if ( m_b_situation[huoer] >= 1 )  //3
                                                {
                                                    score = dna[base_3];
                                                    score += m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                                    score -= m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                                }
                                                else
                                                {
                                                    score = dna[base_5];
                                                    score += m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                                    score -= m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                                }
                                            }
                                        }
                                    }

                                }
                            }
                        }

                        for(i=1;i<=19;i++)
                            for(j=1;j<=19;j++)
                            {
                                if ( board[i][j] == ourOrder )
                                {
                                    score += g_board_base_score[i-1][j-1];
                                }
                                if ( board[i][j] == 2 )
                                {
                                    score -= g_board_base_score[i-1][j-1];
                                }
                            }
                    }
                }
        }
        // Our is white.
        else
        {
            if (m_w_situation[huosi]*2 + m_w_situation[huowu]*2 + m_w_situation[miansi] + m_w_situation[mianwu] >= 1 || (m_w_situation[liu]+m_w_situation[qi]+m_w_situation[ba]+m_w_situation[jiu]!=0))
            {
                return MAXINT -1;
            }
            else
                if ( ( m_b_situation[huosi]*2 + m_b_situation[huowu]*2 + m_b_situation[mianwu] + m_b_situation[miansi] >= 3 ) )
                {
                    return 0;
                }
                else
                {
                    if(m_b_situation[huosi]*2 + m_b_situation[huowu]*2 + m_b_situation[mianwu] + m_b_situation[miansi] == 2)  //7-5
                    {
                        if(m_b_situation[huosan]>=2||(m_b_situation[huosan]==1&&m_b_situation[miansan]>=1))
                        {
                            return 1;
                        }
                        else
                        {
                            if(m_b_situation[huosan]>=1)   //7
                            {
                                score = dna[base_7];
                                score += m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                score -= m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                            }
                            else
                            {
                                if(m_b_situation[huoer]>=2&&(m_b_detail[huoer][huosan]+m_b_detail[huoer][huoer]+m_b_detail[huoer][miansan] + m_b_detail[huoer][huosi] + m_b_detail[huoer][sisi] + m_b_vir_detail[huoer][huoer] + m_b_vir_detail[huoer][miansan] )>0)  //7
                                {
                                    score = dna[base_7];
                                    score += m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                    score -= m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                }
                                else
                                {
                                    if (m_b_situation[huoer]>=1&&(m_b_detail[huoer][huosan]+m_b_detail[huoer][huoer]+m_b_detail[huoer][miansan] + m_b_detail[huoer][huosi] + m_b_detail[huoer][sisi] + m_b_vir_detail[huoer][huoer] + m_b_vir_detail[huoer][miansan] )>0)
                                    {
                                        score = dna[base_7];
                                        score += m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                        score -= m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                    }
                                    else
                                    {
                                        if( m_b_situation[miansan] >= 2 && ( m_b_detail[miansan][sisi] + m_b_detail[miansan][miansan] + m_b_vir_detail[miansan][miansan] + m_b_vir_detail[huoer][miansan] ) > 0 ) //7
                                        {
                                            score = dna[base_7];
                                            score += m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                            score -= m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                        }
                                        else
                                        {
                                            if(m_b_situation[miansan]>=2)   //6
                                         {
                                             score = dna[base_6];
                                             score += m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                             score -= m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                         }
                                            else
                                         {
                                             if( m_b_situation[huoer]>=1 )  //6
                                             {
                                                 score = dna[base_6];
                                                 score += m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                                 score -= m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                             }
                                             else //5
                                             {
                                                 if ( m_b_situation[miansan] > 0 && m_b_situation[huoer] == 0 )
                                                 {
                                                     if ( m_w_situation[miansan] >= m_b_situation[miansan] && m_w_situation[huoer] + m_w_situation[huosan] >= 3 && m_w_detail[huoer][huoer] + m_w_detail[huoer][miansan] + m_w_vir_detail[huoer][huoer] + m_w_vir_detail[huoer][miansan] > 0 )
                                                     {
                                                         score = dna[base_2];
                                                         score += m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                                         score -= m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                                     }
                                                     else
                                                     {
                                                         if ( m_w_situation[miansan] >= m_b_situation[miansan] && m_w_situation[huoer] + m_w_situation[huosan] == 2 && m_w_detail[huoer][huoer] + m_w_detail[huoer][miansan] + m_w_vir_detail[huoer][huoer] + m_w_vir_detail[huoer][miansan] > 0 )
                                                         {
                                                             if ( m_w_situation[huosan] > 0 )
                                                             {
                                                                 score = dna[base_2];
                                                             }
                                                             else
                                                             {
                                                                 score = dna[base_3];
                                                             }
                                                             score += m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                                             score -= m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                                         }
                                                         else
                                                         {
                                                             score = dna[base_6];
                                                             score += m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                                             score -= m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                                         }
                                                     }
                                                 }
                                                 else
                                                 {
                                                     if ( m_b_situation[miansan] > 0 )
                                                     {
                                                         score = dna[base_6];
                                                         score += m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                                         score -= m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                                     }
                                                     else
                                                     {
                                                         score = dna[base_5];
                                                         score += m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                                         score -= m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                                     }
                                                 }
                                             }
                                         }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        if(m_b_situation[huosi]*2 + m_b_situation[huowu]*2 + m_b_situation[mianwu] + m_b_situation[miansi] == 1)
                        {
                            if(m_w_situation[huosan]>=2)  //2
                            {
                                score = dna[base_1];
                                score += m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                score -= m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                            }
                            else
                            {
                                if ( m_w_situation[huosan] == 1 )
                                {
                                    if ( m_w_situation[huoer] >= 2 && ( m_w_detail[huoer][huoer] + m_w_detail[huoer][miansan] + m_w_vir_detail[huoer][huoer] + m_w_vir_detail[huoer][miansan] ) > 0 )
                                    {
                                        score = dna[base_1];
                                    }
                                    else
                                    {
                                        if ( m_w_situation[miansan] >= 2 && ( m_w_detail[miansan][miansan] + m_w_vir_detail[miansan][miansan] + m_w_detail[huoer][miansan] + m_w_vir_detail[huoer][miansan] ) > 0 )
                                        {
                                            score = dna[base_1];
                                        }
                                        else
                                        {
                                            if ( m_w_situation[huoer] > 0 || m_w_situation[miansan] >= 2 )
                                            {
                                                score = dna[base_3];
                                            }
                                            else
                                            {
                                                score = dna[base_5];
                                            }
                                        }
                                    }
                                    score += m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                    score -= m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                }
                                else
                                {
                                    if ( m_b_situation[huosan] >= 2 )
                                    {
                                        return 1;
                                    }
                                    else
                                    {
                                        if ( m_b_situation[huosan] == 1 )
                                        {
                                            if ( m_b_situation[huoer] > 0 )
                                            {
                                                score = dna[base_7];
                                                score += m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                                score -= m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                            }
                                            else
                                            {
                                                if ( m_b_situation[huosan] + m_b_situation[miansan] <= 1 )
                                                {
                                                    score = dna[base_3];
                                                    score += m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                                    score -= m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                                }
                                                else
                                                {
                                                    if ( m_w_situation[miansan] - m_b_situation[miansan] - m_b_situation[huosan] >= 0 && m_w_situation[huoer] >= 2 )
                                                    {
                                                        score = dna[base_3];
                                                        score += m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                                        score -= m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                                    }
                                                    else
                                                    {
                                                        if ( m_w_situation[miansan] < m_b_situation[huosan] + m_b_situation[miansan] )
                                                        {
                                                            score = dna[base_6];
                                                            score += m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                                            score -= m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                                        }
                                                        else
                                                        {
                                                            score = dna[base_5];
                                                            score += m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                                            score -= m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        else
                                        {
                                            if(m_w_situation[miansan]>=1)  //3
                                            {
                                                if ( m_w_situation[miansan] > m_b_situation[miansan] && m_w_situation[huoer] > 0 )
                                                {
                                                    score = dna[base_2];
                                                    score += m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                                    score -= m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                                }
                                                else
                                                {
                                                    if ( m_w_situation[miansan] >= m_b_situation[miansan] )
                                                    {
                                                        score = dna[base_3];
                                                        score += m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                                        score -= m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                                    }
                                                    else
                                                    {
                                                        if ( m_b_situation[miansan] > m_w_situation[miansan] && m_b_situation[huoer] > 0 )
                                                        {
                                                            score = dna[base_7];
                                                            score += m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                                            score -= m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                                        }
                                                        else
                                                        {
                                                            score = dna[base_5];
                                                            score += m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                                            score -= m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                                        }
                                                    }
                                                }


                                            }
                                            else
                                            {
                                                if ( m_b_situation[huoer] >= 2 )
                                                {
                                                    score = dna[base_7];
                                                    score += m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                                    score -= m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                                }
                                                else
                                                {
                                                    if ( m_b_situation[miansan] >= 2 && m_w_situation[huoer] == 0 )
                                                    {
                                                        score = dna[base_6];
                                                        score += m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                                        score -= m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                                    }
                                                    else
                                                    {
                                                        if ( m_b_situation[miansan] == 1 && m_b_situation[huoer] == 1 )
                                                        {
                                                            score = dna[base_6];
                                                            score += m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                                            score -= m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                                        }
                                                        else
                                                        {
                                                            score = dna[base_5];
                                                            score += m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                                            score -= m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            if(m_w_situation[huosan]>=1) //1
                            {
                                score=dna[base_1];
                                score += m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                score -= m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                            }
                            else
                            {
                                if(m_w_situation[huoer] >= 2 && (m_w_detail[huoer][huoer] + m_w_detail[huoer][miansan] + m_w_detail[huoer][sisi] + m_w_vir_detail[huoer][huoer] + m_w_vir_detail[huoer][miansan] ) > 0 )  //1
                                {
                                    score = dna[base_1];
                                    score += m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                    score -= m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                }
                                else
                                {
                                    if ( m_w_situation[miansan] - m_b_situation[miansan] >= 1 && m_w_detail[huoer][huoer] + m_w_detail[huoer][miansan] + m_w_vir_detail[huoer][huoer] + m_w_vir_detail[huoer][miansan] > 0 && m_b_situation[huosan] == 0 && m_b_vir_detail[huoer][miansan] == 0 )
                                    {
                                        score = dna[base_3];
                                        score += m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                        score -= m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                    }
                                    else
                                    {
                                        if(m_w_situation[huoer]==1 &&( m_w_detail[huoer][sisi] +m_w_detail[huoer][miansan] + m_w_detail[huoer][miansan] + m_w_vir_detail[huoer][miansan])> 0) //2
                                        {
                                            score = dna[base_2];
                                            score += m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                            score -= m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                        }
                                        else
                                        {
                                            if( m_w_situation[miansan] >= 2 && m_w_detail[miansan][miansan] + m_w_vir_detail[miansan][miansan] > 0 )  //2
                                            {
                                                score = dna[base_2];
                                                score += m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                                score -= m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                            }
                                            else
                                            {
                                                if ( m_w_situation[huoer] >= 1 )  //3
                                                {
                                                    score = dna[base_3];
                                                    score += m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                                    score -= m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                                }
                                                else
                                                {
                                                    score = dna[base_5];
                                                    score += m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                                    score -= m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                                }
                                            }
                                        }
                                    }

                                }
                            }
                        }

                        for(i=1;i<=19;i++)
                            for(j=1;j<=19;j++)
                            {
                                if ( board[i][j] == ourOrder )
                                {
                                    if ( board[i][j] == ourOrder )
                                    {
                                        score += g_board_base_score[i-1][j-1];
                                    }
                                    if ( board[i][j] == 1 )
                                    {
                                        score -= g_board_base_score[i-1][j-1];
                                    }
                                }
                            }
                    }

                }
        }
        score += MAXINT/2;
    }
    // Odd level evaluation.
    else
    {
        // Our is white
        if ( ourOrder == WHITE )
        {
            if ( ( m_b_situation[huosi]*2 + m_b_situation[huowu]*2 + m_b_situation[miansi] + m_b_situation[mianwu] >= 1 || (m_b_situation[liu]+m_b_situation[qi]+m_b_situation[ba]+m_b_situation[jiu]!=0)) )
            {
                return 0;
            }
            else
                if (m_w_situation[huosi]*2 + m_w_situation[huowu]*2 + m_w_situation[mianwu] + m_w_situation[miansi] >= 3 || (m_w_situation[liu]+m_w_situation[qi]+m_w_situation[ba]+m_w_situation[jiu]!=0))
                {
                    return MAXINT - 1;
                }
                else
                {
                    if(m_w_situation[huosi]*2 + m_w_situation[huowu]*2 + m_w_situation[mianwu] + m_w_situation[miansi] == 2)
                    {
                        if(m_w_situation[huosan]>=1)   //1
                        {
                            score = dna[base_1];
                            score += m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                            score -= m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                        }
                        else
                        {
                            if(m_w_situation[huoer]>=2&&(m_w_detail[huoer][huosan]+m_w_detail[huoer][huoer]+m_w_detail[huoer][miansan] + m_w_detail[huoer][huosi] + m_w_detail[huoer][sisi]  + m_w_vir_detail[huoer][huoer] + m_w_vir_detail[huoer][miansan])>0)  //1
                            {
                                score = dna[base_1];
                                score += m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                score -= m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                            }
                            else
                            {
                                if ( m_w_situation[huoer] >= 2 )  //2
                                {
                                    score = dna[base_3];
                                    score += m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                    score -= m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                }
                                else
                                {
                                    if( m_w_situation[huoer]==1 && ( m_w_detail[huoer][huosan]+m_w_detail[huoer][huoer]+m_w_detail[huoer][miansan] + m_w_detail[huoer][huosi] + m_w_vir_detail[huoer][huoer] + m_w_vir_detail[huoer][miansan]) >0 )  //2
                                    {
                                        score = dna[base_2];
                                        score += m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                        score -= m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                    }
                                    else
                                    {
                                        if(m_w_situation[miansan]>=2&&(m_w_detail[miansan][sisi]+m_w_detail[miansan][miansan])>0) //2
                                        {
                                            score = dna[base_2];
                                            score += m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                            score -= m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                        }
                                        else
                                        {
                                            if( m_w_situation[miansan] - m_b_situation[miansan] >= 1 && m_b_situation[huosan] == 0 && m_b_vir_detail[huoer][miansan] == 0 )   //3
                                            {
                                                score = dna[base_3];
                                                score += m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                                score -= m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                            }
                                            else
                                            {
                                                if ( m_w_situation[huoer] == 1 )
                                                {
                                                    score = dna[base_3];
                                                    score += m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                                    score -= m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                                }


                                                else
                                                {
                                                    score = dna[base_6];
                                                    score += m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                                    score -= m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];


                                                }
                                            }

                                        }
                                    }
                                }
                            }
                        }
                    }
                    else
                        if(m_w_situation[huosi]*2 + m_w_situation[huowu]*2 + m_w_situation[mianwu] + m_w_situation[miansi] == 1)
                        {
                            if(m_b_situation[huosan]>=1)  //7
                            {
                                score = dna[base_7];
                                if(m_b_situation[huoer]>=3&&m_b_detail[huoer][huoer]>0)
                                {
                                    score -= m_b_situation[huoer]* 40;
                                }
                                else
                                {
                                    if(m_b_situation[huoer]>=2&&m_b_detail[huoer][huoer]>0)
                                    {
                                        score -= m_b_situation[huoer] * 30;
                                    }
                                    else
                                    {
                                        score -= m_b_situation[huoer] * 20;
                                    }
                                }
                                score += m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                score -= m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];


                            }
                            else
                            {
                                if(m_b_situation[miansan]>=1)
                                {
                                    if ( ( m_w_situation[miansan] >= m_b_situation[miansan] && m_w_situation[huoer] + m_w_situation[huosan] >= 2 ) && ( m_w_detail[huoer][miansi] + m_w_detail[huosan][miansi] + m_w_detail[huoer][huoer] + m_w_vir_detail[huoer][huoer]> 0 ) )
                                    {
                                        if ( m_b_vir_detail[huoer][miansan] > 0 )
                                        {
                                            score = dna[base_7];
                                            score += m_w_situation[huosan] * 20 + m_w_situation[huoer] * 10 + m_w_situation[miansan] * 6 +m_w_cross * 10;
                                            score -= m_b_situation[huosan] * 25 + m_b_situation[huoer] * 15 + m_b_situation[miansan] * 10 +m_b_cross * 15;
                                        }
                                        else
                                        {
                                            score = dna[base_2];
                                            score += m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                            score -= m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                        }
                                    }
                                    else
                                    {
                                        score = dna[base_6];
                                        score += m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                        score -= m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];

                                    }


                                }
                                else        //2
                                {
                                    if ( m_w_situation[miansan] + m_w_situation[huoer] + m_w_situation[huosan] >= 2 )
                                    {
                                        score = dna[base_2];
                                    }
                                    else
                                    {
                                        if ( m_w_situation[miansan] + m_w_situation[huoer] + m_w_situation[huosan] == 1 )
                                        {
                                            score = dna[base_3];
                                        }
                                        else
                                        {
                                            score = dna[base_6];
                                        }
                                    }
                                    score += m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                    score -= m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                }
                            }

                        }
                        else
                        {
                            if(m_b_situation[huosan]>=1) //7
                            {
                                score = dna[base_7];
                                if(m_b_situation[huoer]>=3&&m_b_detail[huoer][huoer]>0)
                                {
                                    score -= m_b_situation[huoer] *40;
                                }
                                else
                                {
                                    score -= m_b_situation[huoer] *20;
                                }
                                if(m_b_situation[huosan]>=2&&(m_b_detail[huoer][huosan]+m_b_detail[huoer][huoer])>0)
                                {
                                    score -= m_b_situation[huosan] *50;
                                }
                                else
                                {
                                    score -= m_b_situation[huosan] *40;
                                }
                                score += m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                score -= m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];

                            }
                            else
                            {
                                if(m_b_situation[huoer] >= 2 && (m_b_detail[huoer][huoer] + m_b_detail[huoer][miansan] + m_b_detail[huoer][sisi] + m_b_vir_detail[huoer][huoer] + m_b_vir_detail[huoer][miansan])>0)  //7
                                {
                                    score = dna[base_7];
                                    score += m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                    score -= m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];

                                }
                                else
                                {
                                    if( m_b_situation[huoer] >=2 ) //6
                                    {
                                        score = dna[base_6];
                                        score += m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                        score -= m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];

                                    }
                                    else
                                    {
                                        if(m_b_situation[huoer]>=1 &&( m_b_detail[huoer][sisi] +m_b_detail[huoer][miansan])> 0 + m_b_vir_detail[huoer][miansan]) //6
                                        {
                                            score = dna[base_7];
                                            score += m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                            score -= m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];

                                        }
                                        else
                                        {
                                            if(m_b_situation[miansan] >=2 &&m_b_detail[miansan][miansan] >0)  //6
                                            {
                                                score = dna[base_7];
                                                score += m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                                score -= m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];

                                            }
                                            else
                                            {
                                                if ( m_b_situation[huoer] >= 1 )
                                                {
                                                    score = dna[base_6];
                                                    score += m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                                    score -= m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];

                                                }
                                                else
                                                {
                                                    if ( m_b_situation[miansan] >= 1 && m_b_situation[huoer] == 0 )
                                                    {
                                                        if ( ( m_w_situation[miansan] >= m_b_situation[miansan] && m_w_situation[huoer] + m_w_situation[huosan] > 2 ) && ( m_w_detail[huoer][miansi] + m_w_detail[huosan][miansi] + m_w_detail[huoer][huoer] + m_w_vir_detail[huoer][huoer]> 0 ) )
                                                        {
                                                            score = dna[base_2];
                                                            score += m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                                            score -= m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                                        }
                                                        else
                                                        {
                                                            if ( ( m_w_situation[miansan] >= m_b_situation[miansan] && m_w_situation[huoer] + m_w_situation[huosan] == 2 ) && ( m_w_detail[huoer][miansi] + m_w_detail[huosan][miansi] + m_w_detail[huoer][huoer] + m_w_vir_detail[huoer][huoer]> 0 ) )
                                                            {
                                                                if ( m_w_situation[huosan] > 0 )
                                                                {
                                                                    score = dna[base_2];
                                                                    score += m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                                                    score -= m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                                                }
                                                                else
                                                                {
                                                                    score = dna[base_3];
                                                                    score += m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                                                    score -= m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                                                }
                                                            }
                                                            else
                                                            {
                                                                score = dna[base_6];
                                                                score += m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                                                score -= m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];

                                                            }
                                                        }
                                                    }
                                                    else
                                                    {
                                                        if ( m_b_situation[miansan] > 0 )
                                                        {
                                                            score = dna[base_6];
                                                            score += m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                                            score -= m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];

                                                        }
                                                        else    // 3
                                                        {
                                                            score = dna[base_5];
                                                            score += m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                                            score -= m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];

                                                        }
                                                    }
                                                }
                                            }

                                        }
                                    }
                                }
                            }
                        }

                        for(i=1;i<=19;i++)
                            for(j=1;j<=19;j++)
                            {
                                if ( board[i][j] == ourOrder )
                                {
                                    if ( board[i][j] == ourOrder )
                                    {
                                        score += g_board_base_score[i-1][j-1];
                                    }
                                    if ( board[i][j] == 1 )
                                    {
                                        score -= g_board_base_score[i-1][j-1];
                                    }
                                }
                            }
                }

        }
        // Our is black.
        else
        {
#ifdef _DEBUG_
            print_eval();
#endif
            if ((m_w_situation[huosi]*2 + m_w_situation[huowu]*2 + m_w_situation[miansi] + m_w_situation[mianwu] >= 1 || (m_w_situation[liu]+m_w_situation[qi]+m_w_situation[ba]+m_w_situation[jiu]!=0)))
            {
                return 0;
            }
            else
                if (m_b_situation[huosi]*2 + m_b_situation[huowu]*2 + m_b_situation[mianwu] + m_b_situation[miansi] >= 3 || (m_b_situation[liu]+m_b_situation[qi]+m_b_situation[ba]+m_b_situation[jiu]!=0))
                {
                    return MAXINT - 1;
                }
                else
                {
                    if(m_b_situation[huosi]*2 + m_b_situation[huowu]*2 + m_b_situation[mianwu] + m_b_situation[miansi] == 2)
                    {
                        if(m_b_situation[huosan]>=1)   //1
                        {
                            score = dna[base_1];
                            score += m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                            score -= m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                        }
                        else
                        {
                            if(m_b_situation[huoer]>=2&&(m_b_detail[huoer][huosan]+m_b_detail[huoer][huoer]+m_b_detail[huoer][miansan] + m_b_detail[huoer][huosi] + m_b_detail[huoer][sisi] + m_b_vir_detail[huoer][huoer] + m_b_vir_detail[huoer][miansan])>0)  //1
                            {
                                score = dna[base_1];
                                score += m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                score -= m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                            }
                            else
                            {
                                if ( m_b_situation[huoer] >= 2 )  //2
                                {
                                    score = dna[base_3];
                                    score += m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                    score -= m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                }
                                else
                                {
                                    if( m_b_situation[huoer]==1 && ( m_b_detail[huoer][huosan]+m_b_detail[huoer][huoer]+m_b_detail[huoer][miansan] + m_b_detail[huoer][huosi] + m_b_vir_detail[huoer][huoer] + m_b_vir_detail[huoer][miansan]) >0 )  //2
                                    {
                                        score = dna[base_2];
                                        score += m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                        score -= m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                    }
                                    else
                                    {
                                        if(m_b_situation[miansan]>=2&&(m_b_detail[miansan][sisi]+m_b_detail[miansan][miansan])>0) //2
                                        {
                                            score = dna[base_2];
                                            score += m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                            score -= m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                        }
                                        else
                                        {
                                            if( m_b_situation[miansan] - m_w_situation[miansan] >= 1 && m_w_situation[huosan] == 0 && m_w_vir_detail[huoer][miansan] == 0 )   //3
                                            {
                                                score = dna[base_3];
                                                score += m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                                score -= m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                            }
                                            else
                                            {
                                                if ( m_b_situation[huoer] == 1 )
                                                {
                                                    score = dna[base_3];
                                                    score += m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                                    score -= m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                                }
                                                else
                                                {
                                                    score = dna[base_6];
                                                    score += m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                                    score -= m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                                }

                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else
                        if(m_b_situation[huosi]*2 + m_b_situation[huowu]*2 + m_b_situation[mianwu] + m_b_situation[miansi] == 1)
                        {
                            if(m_w_situation[huosan]>=1)  //7
                            {
                                score = dna[base_7];
                                if(m_w_situation[huoer]>=3&&m_w_detail[huoer][huoer]>0)
                                {
                                    score -= m_w_situation[huoer]* 40;
                                }
                                else
                                {
                                    if(m_w_situation[huoer]>=2&&m_w_detail[huoer][huoer]>0)
                                    {
                                        score -= m_w_situation[huoer] * 30;
                                    }
                                    else
                                    {
                                        score -= m_w_situation[huoer] * 20;
                                    }
                                }
                                score += m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                score -= m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                            }
                            else
                            {
                                if(m_w_situation[miansan]>=1)
                                {
                                    if ( ( m_b_situation[miansan] >= m_w_situation[miansan] && m_b_situation[huoer] + m_b_situation[huosan] >= 2 ) && ( m_b_detail[huoer][miansi] + m_b_detail[huosan][miansi] + m_b_detail[huoer][huoer] + m_b_vir_detail[huoer][huoer]> 0 ) )
                                    {
                                        if ( m_w_vir_detail[huoer][miansan] > 0 )
                                        {
                                            score = dna[base_7];
                                            score += m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                            score -= m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                        }
                                        else
                                        {
                                            score = dna[base_2];
                                            score += m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                            score -= m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                        }
                                    }
                                    else
                                    {
                                        score = dna[base_6];
                                        score += m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                        score -= m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                    }
                                }
                                else        //2
                                {
                                    if ( m_b_situation[miansan] + m_b_situation[huoer] + m_b_situation[huosan] >= 2 )
                                    {
                                        score = dna[base_2];
                                    }
                                    else
                                    {
                                        if ( m_b_situation[miansan] + m_b_situation[huoer] + m_b_situation[huosan] == 1 )
                                        {
                                            score = dna[base_3];
                                        }
                                        else
                                        {
                                            score = dna[base_6];
                                        }
                                    }
                                    score += m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                    score -= m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                }
                            }

                        }
                        else
                        {
                            if(m_w_situation[huosan]>=1) //7
                            {
                                score = dna[base_7];
                                if(m_w_situation[huoer]>=3&&m_w_detail[huoer][huoer]>0)
                                {
                                    score -= m_w_situation[huoer] *40;
                                }
                                else
                                {
                                    score -= m_w_situation[huoer] *20;
                                }
                                if(m_w_situation[huosan]>=2&&(m_w_detail[huoer][huosan]+m_w_detail[huoer][huoer])>0)
                                {
                                    score -= m_w_situation[huosan] *50;
                                }
                                else
                                {
                                    score -= m_w_situation[huosan] *40;
                                }
                                score += m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                score -= m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];

                            }
                            else
                            {
                                if(m_w_situation[huoer] >= 2 && (m_w_detail[huoer][huoer] + m_w_detail[huoer][miansan] + m_w_detail[huoer][sisi] + m_w_vir_detail[huoer][huoer] + m_w_vir_detail[huoer][miansan])>0)  //7
                                {
                                    score = dna[base_7];
                                    score += m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                    score -= m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];

                                }
                                else
                                {
                                    if( m_w_situation[huoer] >=2 ) //6
                                    {
                                        score = dna[base_6];
                                        score += m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                        score -= m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];

                                    }
                                    else
                                    {
                                        if(m_w_situation[huoer]>=1 &&( m_w_detail[huoer][sisi] +m_w_detail[huoer][miansan] + m_w_vir_detail[huoer][miansan])> 0) //6
                                        {
                                            score = dna[base_7];
                                            score += m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                            score -= m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];

                                        }
                                        else
                                        {
                                            if(m_w_situation[miansan] >=2 &&m_w_detail[miansan][miansan] >0)  //6
                                            {
                                                score = dna[base_7];
                                                score += m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                                score -= m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];

                                            }

                                            else
                                            {
                                                if ( m_w_situation[huoer] >= 1 )
                                                {
                                                    score = dna[base_6];
                                                    score += m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                                    score -= m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];

                                                }
                                                else
                                                {
                                                    if ( m_w_situation[miansan] >= 1 && m_w_situation[huoer] == 0 )
                                                    {
                                                        if ( ( m_b_situation[miansan] >= m_w_situation[miansan] && m_b_situation[huoer] + m_b_situation[huosan] > 2 ) && ( m_b_detail[huoer][miansi] + m_b_detail[huosan][miansi] + m_b_detail[huoer][huoer] + m_b_vir_detail[huoer][huoer]> 0 ) )
                                                        {
                                                            score = dna[base_2];
                                                            score += m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                                            score -= m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                                        }
                                                        else
                                                        {
                                                            if ( ( m_b_situation[miansan] >= m_w_situation[miansan] && m_b_situation[huoer] + m_b_situation[huosan] == 2 ) && ( m_b_detail[huoer][miansi] + m_b_detail[huosan][miansi] + m_b_detail[huoer][huoer] + m_b_vir_detail[huoer][huoer]> 0 ) )
                                                            {
                                                                if ( m_b_situation[huosan] > 0 )
                                                                {
                                                                    score = dna[base_2];
                                                                    score += m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                                                    score -= m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                                                }
                                                                else
                                                                {
                                                                    score = dna[base_3];
                                                                    score += m_b_situation[huosan] * dna[huosan_big] + m_b_situation[huoer] * dna[huoer_big] + m_b_situation[miansan] * dna[miansan_big] +m_b_cross * dna[cross_big];
                                                                    score -= m_w_situation[huosan] * dna[huosan_lit] + m_w_situation[huoer] * dna[huoer_lit] + m_w_situation[miansan] * dna[miansan_lit] +m_w_cross * dna[cross_lit];
                                                                }
                                                            }
                                                            else
                                                            {
                                                                score = dna[base_6];
                                                                score += m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                                                score -= m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];

                                                            }
                                                        }
                                                    }
                                                    else
                                                    {
                                                        if ( m_w_situation[miansan] > 0 )
                                                        {
                                                            score = dna[base_6];
                                                            score += m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                                            score -= m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];

                                                        }


                                                        else    // 3
                                                        {
                                                            score = dna[base_5];
                                                            score += m_b_situation[huosan] * dna[huosan_lit] + m_b_situation[huoer] * dna[huoer_lit] + m_b_situation[miansan] * dna[miansan_lit] +m_b_cross * dna[cross_lit];
                                                            score -= m_w_situation[huosan] * dna[huosan_big] + m_w_situation[huoer] * dna[huoer_big] + m_w_situation[miansan] * dna[miansan_big] +m_w_cross * dna[cross_big];
                                                        }
                                                    }
                                                }
                                            }

                                        }
                                    }
                                }
                            }
                        }

                        for(i=1;i<=19;i++)
                            for(j=1;j<=19;j++)
                            {
                                if ( board[i][j] == ourOrder )
                                {
                                    score += g_board_base_score[i-1][j-1];
                                }
                                if ( board[i][j] == 2 )
                                {
                                    score -= g_board_base_score[i-1][j-1];
                                }
                            }
                }
        }


        score += MAXINT/2;
    }

    if (ourOrder == BLACK)
    {
        score += b_mean_point - w_mean_point;
    }else
    {
        score += w_mean_point - b_mean_point;
    }

    end = clock();
    m_time_evalution += (double)(end-beg)/CLOCKS_PER_SEC;

    return (score/10);
}

