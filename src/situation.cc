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

void CEvaluation::set_situation(char board[][GRID_NUM])
{
    //初始化
    int d[4];

    memset(m_w_detail,0,sizeof(m_w_detail));
    memset(m_b_detail,0,sizeof(m_b_detail));
    memset(m_w_situation,0,sizeof(m_w_situation));
    memset(m_b_situation,0,sizeof(m_b_situation));
    memset(m_visited_direction,-1,sizeof(m_visited_direction));

    memset(m_b_vir_detail,0,sizeof(m_b_vir_detail));
    memset(m_w_vir_detail,0,sizeof(m_w_vir_detail));
    memset(m_visited_virtual_direction,-1,sizeof(m_visited_virtual_direction));

    b_mean_point = w_mean_point = 0;

    m_w_cross = m_b_cross = 0;

    //对每个点算棋形
    int bd[4],wd[4];
    for (int x = 1 ; x < 20 ; x++)
    {
        for (int y = 1 ; y < 20 ; y++)
        {
            if (board[x][y] != NOSTONE) 
            {
                // 该位置有子 [10/28/2011 lang]
                // 计算棋型 [10/28/2011 lang]
                //上下方向
                d[0] = m_visited_direction[x][y][DUD];
                if (d[0] < 0)
                {
                    set_situation_for_one_direction(x,y,1,0,DUD,board);
                    d[0] = m_visited_direction[x][y][DUD];
                }

                //左右方向
                d[1] = m_visited_direction[x][y][DLR];
                if (d[1] < 0)
                {
                    set_situation_for_one_direction(x,y,0,1,DLR,board);
                    d[1] = m_visited_direction[x][y][DLR];
                }

                //右上方向
                d[2] = m_visited_direction[x][y][DRU];
                if (d[2] < 0)
                {
                    set_situation_for_one_direction(x,y,-1,1,DRU,board);
                    d[2] = m_visited_direction[x][y][DRU];
                }

                //右下方向
                d[3] = m_visited_direction[x][y][DRD];
                if (d[3] < 0)
                {
                    set_situation_for_one_direction(x,y,1,1,DRD,board);
                    d[3] = m_visited_direction[x][y][DRD];
                }

                if (board[x][y] == BLACK)
                {
                    for (int i = 0 ; i < 4 ; i++)
                    {
                        for (int j = i + 1 ; j < 4 ; j++)
                        {
                            if (d[i] >=0 && d[j] >= 0 && d[i] < wuxing && d[j] < wuxing && (d[i]!= sisi || d[j]!=sisi))
                            {
                                m_b_detail[d[j]][d[i]]++;
                                if (i != j)
                                    m_b_detail[d[i]][d[j]]++;

                                m_b_cross++;
                            }
                        }
                    }
                }else
                {
                    for (int i = 0 ; i < 4 ; i++)
                    {
                        for (int j = i + 1 ; j < 4 ; j++)
                        {
                            if (d[i] >=0 && d[j] >= 0 && d[i] < wuxing && d[j] < wuxing && (d[i]!= sisi || d[j]!=sisi))
                            {
                                m_w_detail[d[j]][d[i]]++;
                                if (i != j)
                                    m_w_detail[d[i]][d[j]]++;

                                m_w_cross++;
                            }
                        }
                    }
                }
            }else
            {
                // 该位置无子 [10/28/2011 lang]
                // 计算虚交叉 [10/28/2011 lang]
                for (int di = 0 ; di < 4 ; di++)
                {
                    bd[di] = -1;
                    wd[di] = -1;
                    if (m_visited_virtual_direction[x][y][di] >=black_fix)
                    {
                        bd[di] = m_visited_virtual_direction[x][y][di];
                    }else if (m_visited_virtual_direction[x][y][di] >=white_fix)
                    {
                        wd[di] = m_visited_virtual_direction[x][y][di];
                    }
                }
                for (int i = 0 ; i < 4 ; i++)
                {
                    for (int j = i + 1 ; j < 4 ; j++)
                    {
                        if (bd[i] >=0 && bd[j] >= 0 )
                        {
                            m_b_vir_detail[bd[j] - black_fix][bd[i] - black_fix]++;
                            if (i != j)
                                m_b_vir_detail[bd[i] - black_fix][bd[j] - black_fix]++;
                        }

                        if (wd[i] >=0 && wd[j] >= 0 )
                        {
                            m_w_vir_detail[wd[j] - white_fix][wd[i] - white_fix]++;
                            if (i != j)
                                m_w_vir_detail[wd[i]- white_fix][wd[j] - white_fix]++;
                        }
                    }
                }
            }
        }
    }

    return ;
}

void CEvaluation::set_situation_for_one_direction(int x, int y ,short countx,short county,int dir, char board[][GRID_NUM])
{
    char currColor = board[x][y];
    int i = x,j = y;
    int colorN = 1,noneN = 0;
    int longContinue = 1;
    int longNone = 0;
    int rLongest = 0;
    int lLongest = 0;

    int tempLong = 1;
    int mid = 0;
    int rI,rJ,lI,lJ;
    //int total = 0;
    int rEnd = 0,lEnd = 0;

    pos_t posList[20] ;
    pos_t posRList[20] ;
    pos_t posLList[20] ;
    memset(posList, 0, sizeof(posList));
    memset(posRList, 0, sizeof(posRList));
    memset(posLList, 0, sizeof(posLList));
    posList[0].x = 1;
    posList[1].x = x;
    posList[1].y = y;
    posLList[0].x = 0;
    posRList[0].x = 0;
    int count= 0;

    //紧挨最后一个的棋的颜色
    int rSideColor = board[x-countx][y-county],lSideColor = board[x+countx][y+county];

    while ( rEnd < 5 &&  colorN  < 7 && noneN < 4)
    {
        i += countx;
        j += county;

        if (currColor == board[i][j])
        {
            //当前颜色与所求点颜色相同
            if (noneN + rEnd >3)
            {
                break;
            }
            if (longNone < rEnd)
            {
                longNone = rEnd;
            }
            noneN += rEnd;
            rEnd = 0;
            colorN++;

            tempLong++;

            //访问过改点
            posList[++posList[0].x].x = i;
            posList[posList[0].x].y = j;

            rSideColor = board[i-countx][j-county];

        } else if (board[i][j] == NOSTONE)
        {
            //当前点无子
            count++;
            if (rLongest == 0)
                rLongest = tempLong;

            rLongest = tempLong;

            if (tempLong > longContinue)
            {
                longContinue = tempLong;
            }
            if (mid == 0)
                mid = tempLong;
            tempLong = 0;
            rEnd++;

            //访问过改点
            posRList[++posRList[0].x].x = i;
            posRList[posRList[0].x].y = j;
        } else 
        {
            //结束
            if (rLongest == 0)
                rLongest = tempLong;

            if (tempLong > longContinue)
            {
                longContinue = tempLong;
            }
            break;
        }
    }
    rI = i;
    rJ = j;

    i = x;
    j = y;
    tempLong = mid;
    while (lEnd < 5 && colorN  < 7)
    {
        i -= countx;
        j -= county;
        
        if (currColor == board[i][j])
        {
            //当前颜色与所求点颜色相同
            if (noneN + lEnd >3)
            {
                break;
            }
            if (longNone < lEnd)
            {
                longNone = lEnd;
            }
            noneN += lEnd;
            lEnd = 0;
            colorN++;

            tempLong++;

            //访问过改点
            posList[++posList[0].x].x = i;
            posList[posList[0].x].y = j;

            lSideColor = board[i+countx][j+county];

        } else if (board[i][j] == NOSTONE)
        {
            //当前点无子
            if (lLongest == 0)
                lLongest = tempLong;
            lLongest = tempLong;

            if (tempLong > longContinue)
            {
                longContinue = tempLong;
            }
            if (count == 1)
            {
                //前面的最长值
                rLongest = tempLong;
                count = 0;
            }
            tempLong = 0;
            lEnd++;

            //访问过改点
            posLList[++posLList[0].x].x = i;
            posLList[posLList[0].x].y = j;
        } else 
        {
            //结束
            if (lLongest == 0)
                lLongest = tempLong;

            if (tempLong > longContinue)
            {
                longContinue = tempLong;
            }
            break;
        }
    }
    lI = i;
    lJ = j;

    for(int i = 0 ; i <= posList[0].x ; i++)
    {
        m_visited_direction[posList[i].x][posList[i].y][dir] = wuxing;
    }

    if (rEnd + lEnd + colorN + noneN < 6 && colorN != 4)
    {
        //一列中的总和小于六个不能形成棋形，返回
        return ;
    }else if (rEnd + lEnd + colorN + noneN > 5)
    {
        if (currColor == BLACK)
        {
            int last = posList[0].x - (rEnd < 2 ? 2-rEnd : 0) - (lEnd < 2 ? 2 - lEnd: 0);

            if (rSideColor == NOSTONE && rEnd == 0 || lSideColor == NOSTONE && lEnd == 0)
            {
                last++;
            }

            b_mean_point += last < 0 ? 0 : last;
        }else
        {
            int last = posList[0].x - (rEnd < 2 ? 2-rEnd : 0) - (lEnd < 2 ? 2 - lEnd: 0);

            if (rSideColor == NOSTONE && rEnd == 0 || lSideColor == NOSTONE && lEnd == 0)
            {
                last++;
            }

            w_mean_point += last < 0 ? 0 : last;
        }
    }

    switch (colorN)
    {
    case 1:        //color
        break;
    case 2:        //color
        if (rEnd > 1 && lEnd > 1 && rEnd + lEnd + noneN >= 6 && noneN < 3)
        {
            //活二
            if (currColor == BLACK)
            {
                m_b_situation[huoer]++;

                for (int i = 1 ; i <= posRList[0].x - rEnd + 2 ; i++)
                {
                    m_visited_virtual_direction[posRList[i].x][posRList[i].y][dir] = huoer + black_fix;
                }
                for (int i = 1 ; i <= posLList[0].x - lEnd + 2 ; i++)
                {
                    m_visited_virtual_direction[posLList[i].x][posLList[i].y][dir] = huoer + black_fix ;
                }
            }else
            {
                m_w_situation[huoer]++;

                for (int i = 1 ; i <= posRList[0].x - 2 ; i++)
                {
                    m_visited_virtual_direction[posRList[i].x][posRList[i].y][dir] = huoer + white_fix;
                }
                for (int i = 1 ; i <= posLList[0].x - 2 ; i++)
                {
                    m_visited_virtual_direction[posLList[i].x][posLList[i].y][dir] = huoer + white_fix;
                }
            }
            for (int i = 0 ; i <= posList[0].x ; i++)
            {
                m_visited_direction[posList[i].x][posList[i].y][dir] = huoer;
            }

        } else
        {
            //死二
            //if (currColor == BLACK)
            //{
            //    situation_b[huoer]++;
            //}else
            //{
            //    situation_w[huoer]++;
            //}
            //for (int i = 0 ; i <= posList[0].x ; i++)
            //{
            //    visitedDir[posList[i].x][posList[i].y][dir] = huoer;
            //}
        }
        break;
    case 3:        //color
        if (rEnd > 1 && lEnd > 1 && rEnd + lEnd + noneN >= 5 && noneN < 2)
        {
            //活三
            if (currColor == BLACK)
            {
                m_b_situation[huosan]++;

                for (int i = 1 ; i <= posRList[0].x - 2 ; i++)
                {
                    m_visited_virtual_direction[posRList[i].x][posRList[i].y][dir] = huosan + black_fix;
                }
                for (int i = 1 ; i <= posLList[0].x - 2 ; i++)
                {
                    m_visited_virtual_direction[posLList[i].x][posLList[i].y][dir] = huosan + black_fix ;
                }
            }else
            {
                m_w_situation[huosan]++;

                for (int i = 1 ; i <= posRList[0].x - 2 ; i++)
                {
                    m_visited_virtual_direction[posRList[i].x][posRList[i].y][dir] = huosan + white_fix;
                }
                for (int i = 1 ; i <= posLList[0].x - 2 ; i++)
                {
                    m_visited_virtual_direction[posLList[i].x][posLList[i].y][dir] = huosan + white_fix ;
                }
            }
            for (int i = 0 ; i <= posList[0].x ; i++)
            {
                m_visited_direction[posList[i].x][posList[i].y][dir] = huosan;
            }

        }else if ((rEnd == 1 && noneN < 3 && noneN + lEnd > 2)||( lEnd == 1 && noneN < 3 && noneN + rEnd > 2))
        {
            //相当于活二的三 0***000
            if (currColor == BLACK)
            {
                m_b_situation[huoer]++;

                for (int i = 1 ; i <= posRList[0].x + ( rEnd > 2 ? 2 - rEnd: 0 ) ; i++)
                {
                    m_visited_virtual_direction[posRList[i].x][posRList[i].y][dir] = huoer + black_fix;
                }
                for (int i = 1 ; i <= posLList[0].x + ( lEnd > 2 ? 2 - lEnd: 0 ) ; i++)
                {
                    m_visited_virtual_direction[posLList[i].x][posLList[i].y][dir] = huoer + black_fix ;
                }
            }else
            {
                m_w_situation[huoer]++;

                for (int i = 1 ; i <= posRList[0].x + ( rEnd > 2 ? 2 - rEnd: 0 ) ; i++)
                {
                    m_visited_virtual_direction[posRList[i].x][posRList[i].y][dir] = huoer + white_fix;
                }
                for (int i = 1 ; i <= posLList[0].x + ( lEnd > 2 ? 2 - lEnd: 0 ) ; i++)
                {
                    m_visited_virtual_direction[posLList[i].x][posLList[i].y][dir] = huoer + white_fix ;
                }
            }
            for (int i = 0 ; i <= posList[0].x ; i++)
            {
                m_visited_direction[posList[i].x][posList[i].y][dir] = huoer;
            }
            //死三
            if (currColor == BLACK)
            {
                m_b_situation[miansan]++;

                for (int i = 1 ; i <= posRList[0].x + ( rEnd > 2 ? 2 - rEnd: 0 ) ; i++)
                {
                    m_visited_virtual_direction[posRList[i].x][posRList[i].y][dir] = miansan + black_fix;
                }
                for (int i = 1 ; i <= posLList[0].x + ( lEnd > 2 ? 2 - lEnd: 0 ) ; i++)
                {
                    m_visited_virtual_direction[posLList[i].x][posLList[i].y][dir] = miansan + black_fix ;
                }
            }else
            {
                m_w_situation[miansan]++;

                for (int i = 1 ; i <= posRList[0].x + ( rEnd > 2 ? 2 - rEnd: 0 ) ; i++)
                {
                    m_visited_virtual_direction[posRList[i].x][posRList[i].y][dir] = miansan + white_fix;
                }
                for (int i = 1 ; i <= posLList[0].x + ( lEnd > 2 ? 2 - lEnd: 0 ) ; i++)
                {
                    m_visited_virtual_direction[posLList[i].x][posLList[i].y][dir] = miansan + white_fix ;
                }
            }
            for (int i = 0 ; i <= posList[0].x ; i++)
            {
                m_visited_direction[posList[i].x][posList[i].y][dir] = miansan;
            }
        } else
        {
            //三中含有活二,复杂
            if (( longNone == 1 && ( lLongest == 1 && rLongest == 1 && (rEnd > 2 || lEnd > 2)|| rEnd > 3 && rLongest == 2 || lEnd > 3 && lLongest ==2)  ) || 
                ( longNone == 2 && (lLongest == 1 && rLongest == 1 && (rEnd > 2 || lEnd > 2) || rEnd > 2 && rLongest == 2 || lEnd > 2 && lLongest ==2)  ) ||
                ( longNone == 3 && (rLongest == 2 && rEnd > 1 || lLongest == 2 && lEnd > 1)))
            {
                if (currColor == BLACK)
                {
                    m_b_situation[huoer]++;

                    for (int i = 1 ; i <= posRList[0].x + ( rEnd > 2 ? 2 - rEnd: 0 ) ; i++)
                    {
                        m_visited_virtual_direction[posRList[i].x][posRList[i].y][dir] = huoer + black_fix;
                    }
                    for (int i = 1 ; i <= posLList[0].x + ( lEnd > 2 ? 2 - lEnd: 0 ) ; i++)
                    {
                        m_visited_virtual_direction[posLList[i].x][posLList[i].y][dir] = huoer + black_fix ;
                    }

                }else
                {
                    m_w_situation[huoer]++;

                    for (int i = 1 ; i <= posRList[0].x + ( rEnd > 2 ? 2 - rEnd: 0 ) ; i++)
                    {
                        m_visited_virtual_direction[posRList[i].x][posRList[i].y][dir] = huoer + white_fix;
                    }
                    for (int i = 1 ; i <= posLList[0].x + ( lEnd > 2 ? 2 - lEnd: 0 ) ; i++)
                    {
                        m_visited_virtual_direction[posLList[i].x][posLList[i].y][dir] = huoer + white_fix ;
                    }
                }
                for (int i = 0 ; i <= posList[0].x ; i++)
                {
                    m_visited_direction[posList[i].x][posList[i].y][dir] = huoer;
                }
            }
            //死三
            if (currColor == BLACK)
            {
                m_b_situation[miansan]++;

                for (int i = 1 ; i <= posRList[0].x + ( rEnd > 2 ? 2 - rEnd: 0 ) ; i++)
                {
                    m_visited_virtual_direction[posRList[i].x][posRList[i].y][dir] = miansan + black_fix;
                }
                for (int i = 1 ; i <= posLList[0].x + ( lEnd > 2 ? 2 - lEnd: 0 ) ; i++)
                {
                    m_visited_virtual_direction[posLList[i].x][posLList[i].y][dir] = miansan + black_fix ;
                }
            }else
            {
                m_w_situation[miansan]++;

                for (int i = 1 ; i <= posRList[0].x + ( rEnd > 2 ? 2 - rEnd: 0 ) ; i++)
                {
                    m_visited_virtual_direction[posRList[i].x][posRList[i].y][dir] = miansan + white_fix;
                }
                for (int i = 1 ; i <= posLList[0].x + ( lEnd > 2 ? 2 - lEnd: 0 ) ; i++)
                {
                    m_visited_virtual_direction[posLList[i].x][posLList[i].y][dir] = miansan + white_fix ;
                }
            }
            for (int i = 0 ; i <= posList[0].x ; i++)
            {
                m_visited_direction[posList[i].x][posList[i].y][dir] = miansan;
            }

        }
        break;
    case 4:        //color
        if (rEnd + lEnd + colorN + noneN < 6)
        {
            //死四，非棉四
            if (currColor == BLACK)
            {
                m_b_situation[sisi]++;
            }else
            {
                m_w_situation[sisi]++;
            }
            for (int i = 0 ; i <= posList[0].x ; i++)
            {
                m_visited_direction[posList[i].x][posList[i].y][dir] = sisi;
            }
            break;

        }
        if (rEnd > 1 && lEnd > 1 && noneN == 0)
        {
            //活四
            if (currColor == BLACK)
            {
                m_b_situation[huosi]++;
            }else
            {
                m_w_situation[huosi]++;
            }
            for (int i = 0 ; i <= posList[0].x ; i++)
            {
                m_visited_direction[posList[i].x][posList[i].y][dir] = huosi;
            }
        }else if (noneN < 3)
        {
            //死四
            if (currColor == BLACK)
            {
                m_b_situation[miansi]++;
            }else
            {
                m_w_situation[miansi]++;
            }
            for (int i = 0 ; i <= posList[0].x ; i++)
            {
                m_visited_direction[posList[i].x][posList[i].y][dir] = miansi;
            }
        }else if (noneN == 3)
        {
            if (longNone == 1)
            {
                if (rEnd > 1 && lEnd > 1)
                {
                    //活三
                    if (currColor == BLACK)
                    {
                        m_b_situation[huosan]++;
                    }else
                    {
                        m_w_situation[huosan]++;
                    }
                    for (int i = 0 ; i <= posList[0].x ; i++)
                    {
                        m_visited_direction[posList[i].x][posList[i].y][dir] = huosan;
                    }
                }else 
                {
                    //棉三
                    if (currColor == BLACK)
                    {
                        m_b_situation[miansan]++;
                    }else
                    {
                        m_w_situation[miansan]++;
                    }
                    for (int i = 0 ; i <= posList[0].x ; i++)
                    {
                        m_visited_direction[posList[i].x][posList[i].y][dir] = miansan;
                    }
                }
            }else if (longNone == 2)
            {
                //四子空3，
                if (rLongest == 2 && rEnd < 2 || lLongest == 2 && lEnd < 2)
                {
                    //棉三
                    if (currColor == BLACK)
                    {
                        m_b_situation[miansan]++;
                    }else
                    {
                        m_w_situation[miansan]++;
                    }
                    for (int i = 0 ; i <= posList[0].x ; i++)
                    {
                        m_visited_direction[posList[i].x][posList[i].y][dir] = miansan;
                    }

                }else 
                {
                    //活三
                    if (currColor == BLACK)
                    {
                        m_b_situation[huosan]++;
                    }else
                    {
                        m_w_situation[huosan]++;
                    }
                    for (int i = 0 ; i <= posList[0].x ; i++)
                    {
                        m_visited_direction[posList[i].x][posList[i].y][dir] = huosan;
                    }
                }

            }else
            {
                if (rEnd < 2 && rLongest == 3 || lEnd < 2 && lLongest == 3)
                {
                    //棉三
                    if (currColor == BLACK)
                    {
                        m_b_situation[huosan]++;
                    }else
                    {
                        m_w_situation[huosan]++;
                    }
                    for (int i = 0 ; i <= posList[0].x ; i++)
                    {
                        m_visited_direction[posList[i].x][posList[i].y][dir] = huosan;
                    }
                }else
                {
                    //活三
                    if (currColor == BLACK)
                    {
                        m_b_situation[huosan]++;
                    }else
                    {
                        m_w_situation[huosan]++;
                    }
                    for (int i = 0 ; i <= posList[0].x ; i++)
                    {
                        m_visited_direction[posList[i].x][posList[i].y][dir] = huosan;
                    }
                }
            }
        }
        break;
    case 5:        //color
        switch (longContinue)
        {
        case 2:
            //棉五
            if (currColor == BLACK)
            {
                m_b_situation[mianwu]++;
            }else
            {
                m_w_situation[mianwu]++;
            }
            for (int i = 0 ; i <= posList[0].x ; i++)
            {
                m_visited_direction[posList[i].x][posList[i].y][dir] = mianwu;
            }

            break;
        case 3:
            if (longNone == 3)
            {
                //活二活三
                if (lEnd < 2 && rEnd < 2)
                {
                    //棉三
                    if (currColor == BLACK)
                    {
                        m_b_situation[miansan]++;
                    }else
                    {
                        m_w_situation[miansan]++;
                    }
                    for (int i = 0 ; i <= posList[0].x ; i++)
                    {
                        m_visited_direction[posList[i].x][posList[i].y][dir] = miansan;
                    }
                }else 
                {
                    if (lEnd > 1 && lLongest == 3 || rEnd > 1 && rLongest ==3)
                    {
                        //活三
                        if (currColor == BLACK)
                        {
                            m_b_situation[huosan]++;
                        }else
                        {
                            m_w_situation[huosan]++;
                        }
                        for (int i = 0 ; i <= posList[0].x ; i++)
                        {
                            m_visited_direction[posList[i].x][posList[i].y][dir] = huosan;
                        }
                    }
                }
            }else if (rLongest == 3 && lLongest == 3)
            {
                //活五
                if (currColor == BLACK)
                {
                    m_b_situation[huowu]++;
                }else
                {
                    m_w_situation[huowu]++;
                }
                for (int i = 0 ; i <= posList[0].x ; i++)
                {
                    m_visited_direction[posList[i].x][posList[i].y][dir] = huowu;
                }
            }else
            {
                //棉五
                if (currColor == BLACK)
                {
                    m_b_situation[mianwu]++;
                }else
                {
                    m_w_situation[mianwu]++;
                }
                for (int i = 0 ; i <= posList[0].x ; i++)
                {
                    m_visited_direction[posList[i].x][posList[i].y][dir] = mianwu;
                }
            }
            //if (rEnd > 0 && lEnd > 0 && noneN == 2)
            //{
            //    //活五
            //    if (currColor == BLACK)
            //    {
            //        situation_b[huowu]++;
            //    }else
            //    {
            //        situation_w[huowu]++;
            //    }
            //    for (int i = 0 ; i <= posList[0].x ; i++)
            //    {
            //        visitedDir[posList[i].x][posList[i].y][dir] = huowu;
            //    }
            //}else
            //{
            //    //死五
            //    if (currColor == BLACK)
            //    {
            //        situation_b[mianwu]++;
            //    }else
            //    {
            //        situation_w[mianwu]++;
            //    }
            //    for (int i = 0 ; i <= posList[0].x ; i++)
            //    {
            //        visitedDir[posList[i].x][posList[i].y][dir] = mianwu;
            //    }
            //}
            break;
        case 4:
            if (rEnd < 2 && rLongest == 4 || lEnd < 2 && lLongest == 4)
            {
                //棉五
                if (currColor == BLACK)
                {
                    m_b_situation[mianwu]++;
                }else
                {
                    m_w_situation[mianwu]++;
                }
                for (int i = 0 ; i <= posList[0].x ; i++)
                {
                    m_visited_direction[posList[i].x][posList[i].y][dir] = mianwu;
                }
            } else
            {
                //活五
                if (currColor == BLACK)
                {
                    m_b_situation[huowu]++;
                }else
                {
                    m_w_situation[huowu]++;
                }
                for (int i = 0 ; i <= posList[0].x ; i++)
                {
                    m_visited_direction[posList[i].x][posList[i].y][dir] = huowu;
                }
            }
            //if (rEnd > 0 && lEnd > 0)
            //{
            //    //活五
            //    if (currColor == BLACK)
            //    {
            //        situation_b[huowu]++;
            //    }else
            //    {
            //        situation_w[huowu]++;
            //    }
            //    for (int i = 0 ; i <= posList[0].x ; i++)
            //    {
            //        visitedDir[posList[i].x][posList[i].y][dir] = huowu;
            //    }
            //}else if (rEnd + lEnd == 1)
            //{
            //    //不定
            //    while (board[i][j] != currColor)
            //    {
            //        //返推
            //        i += countx;
            //        j += county;
            //    }
            //    if (board[i+countx][j+county] == board[i-countx][j-county])
            //    {
            //        //死五
            //        if (currColor == BLACK)
            //        {
            //            situation_b[mianwu]++;
            //        }else
            //        {
            //            situation_w[mianwu]++;
            //        }
            //        for (int i = 0 ; i <= posList[0].x ; i++)
            //        {
            //            visitedDir[posList[i].x][posList[i].y][dir] = mianwu;
            //        }
            //    }else 
            //    {
            //        //活五
            //        if (currColor == BLACK)
            //        {
            //            situation_b[huowu]++;
            //        }else
            //        {
            //            situation_w[huowu]++;
            //        }
            //        for (int i = 0 ; i <= posList[0].x ; i++)
            //        {
            //            visitedDir[posList[i].x][posList[i].y][dir] = huowu;
            //        }
            //    }
            //}else
            //{
            //    //死五
            //    if (currColor == BLACK)
            //    {
            //        situation_b[mianwu]++;
            //    }else
            //    {
            //        situation_w[mianwu]++;
            //    }
            //    for (int i = 0 ; i <= posList[0].x ; i++)
            //    {
            //        visitedDir[posList[i].x][posList[i].y][dir] = mianwu;
            //    }
            //}
            break;
        case 5:
            if (rEnd > 0 && lEnd > 0)
            {
                //活五
                if (currColor == BLACK)
                {
                    m_b_situation[huowu]++;
                }else
                {
                    m_w_situation[huowu]++;
                }
                for (int i = 0 ; i <= posList[0].x ; i++)
                {
                    m_visited_direction[posList[i].x][posList[i].y][dir] = huowu;
                }
            }else
            {
                //死五
                if (currColor == BLACK)
                {
                    m_b_situation[mianwu]++;
                }else
                {
                    m_w_situation[mianwu]++;
                }
                for (int i = 0 ; i <= posList[0].x ; i++)
                {
                    m_visited_direction[posList[i].x][posList[i].y][dir] = mianwu;
                }
            }
            break;
        default :
            //死五
            if (currColor == BLACK)
            {
                m_b_situation[mianwu]++;
            }else
            {
                m_w_situation[mianwu]++;
            }
            for (int i = 0 ; i <= posList[0].x ; i++)
            {
                m_visited_direction[posList[i].x][posList[i].y][dir] = mianwu;
            }
            break;
        }
        break;
    case 6:        //color
        if (noneN == 0)
        {
            //六，已经结束
            if (currColor == BLACK)
            {
                m_b_situation[liu]++;
            }else
            {
                m_w_situation[liu]++;
            }
            for (int i = 0 ; i <= posList[0].x ; i++)
            {
                m_visited_direction[posList[i].x][posList[i].y][dir] = liu;
            }
        }
        break;
    case 7:        //color
        switch (noneN)
        {
        case 0:        //noneN
            break;
        case 1:        //noneN
            break;
        case 2:        //noneN
            break;
        case 3:        //noneN
            break;
        default :        //noneN
            break;
        }
        break;
    case 8:        //color
        switch (noneN)
        {
        case 0:        //noneN
            break;
        case 1:        //noneN
            break;
        case 2:        //noneN
            break;
        case 3:        //noneN
            break;
        default :        //noneN
            break;
        }
        break;
    default :    //color
        switch (noneN)
        {
        case 0:        //noneN
            break;
        case 1:
            break;
        case 2:        //noneN
            break;
        case 3:        //noneN
            break;
        default :        //noneN
            break;
        }
        break;
    }

    return ;
}

