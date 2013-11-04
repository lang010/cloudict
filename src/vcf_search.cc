/*
 * Copyright (c) 2008-2013 Hao Cui<>,
 *                         Liang Li<liliang010@gmail.com>,
 *                         Ruijian Wang<>,
 *                         Siran Lin<>.
 *                         All rights reserved.
 *
 * This program is a free software; you can redistribute it and/or modify
 * it under the terms of the BSD license. See LICENSE.txt for details.
 *
 * 2013/11/01
 *
 */

#include "vcf_search.h"
#include "tools.h"

CVCFSearch::CVCFSearch() {
    m_dx[0] = -1;
    m_dx[1] = -1;
    m_dx[2] = 0;
    m_dx[3] = 1;

    m_dy[0] = 0;
    m_dy[1] = -1;
    m_dy[2] = -1;
    m_dy[3] = -1;
    // m_dy[4]={0,-1,-1,-1};
    // m_dx[4]={-1,-1,0,1};        //定义方向数组
}

int CVCFSearch::init() {
    return m_dfa.dfa_init();
}

void CVCFSearch::init_game() {
    m_has_win = 0;
}

void CVCFSearch::before_search(char board[][GRID_NUM], char color){
    memcpy(m_board, board, sizeof(m_board));
    m_chess_type = color;
    m_vcf_node = 0;
}

bool CVCFSearch::vcf_judge(move_t * preMove)
{
    char Color=m_board[preMove->positions[0].x][preMove->positions[0].y];
    if (is_attack(m_board,Color,preMove)>=2)
    {
        return true;
    }
    return false;
}

int CVCFSearch::is_attack(char board[GRID_NUM][GRID_NUM],char Color, move_t * Move)            //判断该走法是否具有威胁
{
    int x, y, newx, newy;        
    int count;
    int i, d, j;
    char tmpboard[GRID_NUM][GRID_NUM];
    char tmpuse[GRID_NUM][GRID_NUM][4];
    int max, min, ans=0;

    ans=is_dlb_attack(board,Color,Move);
    if (ans>2)    //3个以上威胁
    {
        return ans;
    }

    //memset(use,0,sizeof(use));                    //清空所有评价标志
    memcpy(tmpboard,board,sizeof(tmpboard));    //创建临时棋盘
    memcpy(tmpuse,m_vcf_use,sizeof(tmpuse));

    for ( i = 0; i <= 1; i++ )
    {
        x = Move->positions[i].x;            //前一招法两点的坐标
        y = Move->positions[i].y;
        for ( d = 0; d < 4; d++  )            //对四个方向进行分析
        {
            //每个方向找出我方的区域
            newx = x;
            newy = y;
            for ( j = 1; j < 6 ; j++ )
            {
                newx += m_dx[d];
                newy += m_dy[d];
                if ( tmpboard[newx][newy]!=BORDER && tmpboard[newx][newy] != (Color^3) ) //如果该点不是对方棋子并且未评价过，继续
                {
                    if (tmpboard[newx][newy]==NOSTONE && m_vcf_mark[newx][newy])
                    {
                        break;
                    }
                } 
                else    //越出棋盘或有对方棋子退出
                {
                    break;
                }
            }
            max = j - 1;    //最大点

            newx = x;
            newy = y;
            for ( j = 1; j < 6 ; j++)
            {
                newx -= m_dx[d];
                newy -= m_dy[d];
                if ( tmpboard[newx][newy]!=BORDER && tmpboard[newx][newy] != (Color^3) ) //如果该点不是对方棋子并且未评价过，继续
                {
                    if (tmpboard[newx][newy]==NOSTONE&& m_vcf_mark[newx][newy])
                    {
                        break;
                    }
                } 
                else    //越出棋盘或有对方棋子退出    
                {
                    break;
                }
            }
            min = 1 - j;    //最小点

            if ( max - min + 1 < 6 )    //区域范围小于6
            {
                continue;
            }
            j = max - 5;            //计算连续6个点有几个我方棋子
            newx = x + m_dx[d] * j;
            newy = y + m_dy[d] * j;
            count=0;
            for (; j <= max; j++)
            {
                if (tmpboard[newx][newy]==Color)
                {
                    count++;
                }
                //use[newx][newy][d]=1;
                //mark[newx][newy]=1;
                newx += m_dx[d];
                newy += m_dy[d];
            }

            if ( count >= 4 )        //如果出现4个以上我方棋子，形成一威胁
            {
                ans++;
                for (j=0;j<6;j++)
                {
                    newx-=m_dx[d];
                    newy-=m_dy[d];
                    m_vcf_use[newx][newy][d]=i+1;
                    m_vcf_mark[newx][newy]=1;
                }
                continue;
            }

            j = max - 6;            //6个子的区域不断移动
            newx = x + m_dx[d] * j;
            newy = y + m_dy[d] * j;
            for (; j >= min; j--)
            {

                if (tmpboard[newx][newy]==Color)
                {
                    count++;
                }
                if (tmpboard[newx+m_dx[d]*6][newy+m_dy[d]*6])
                {
                    count--;
                }
                //use[newx][newy][d]=1;
                //mark[newx][newy]=1;
                /*if (tmpuse[newx+dx[d]*6][newy+dy[d]*6][d]==0)
                {
                use[newx+dx[d]*6][newy+dy[d]*6][d]=0;
                mark[newx+dx[d]*6][newy+dy[d]*6]=0;
                }*/

                if (count>=4)
                {
                    ans++;
                    for (int k=0;k<6;k++)
                    {
                        m_vcf_use[newx][newy][d]=i+1;
                        m_vcf_mark[newx][newy]=1;
                        newx+=m_dx[d];
                        newy+=m_dy[d];
                    }
                    break;
                }
                newx-=m_dx[d];
                newy-=m_dy[d];
            }

            if (count<4)
            {
                newx=x+m_dx[d]*min;
                newy=y+m_dy[d]*min;

            }

        }
    }
    return ans;
}

int CVCFSearch::is_dlb_attack(char board[][GRID_NUM],char Color, move_t * Move)
{
    int x, y, newx, newy;        
    int count;
    int i, d, j;
    char tmpboard[GRID_NUM][GRID_NUM];
    int min, max;

    memset(m_vcf_use,0,sizeof(m_vcf_use));
    memset(m_vcf_mark,0,sizeof(m_vcf_mark));
    memcpy(tmpboard,board,sizeof(tmpboard));    //创建临时棋盘

    for ( i = 0; i <= 1; i++ )
    {
        x = Move->positions[i].x;            //前一招法两点的坐标
        y = Move->positions[i].y;
        for ( d = 0; d < 4; d++  )            //对四个方向进行分析
        {
            count = 1;                        //每个方向找出我方连续点的个数
            for ( j = 1; count < 6 ; j++ )
            {
                newx = x + m_dx[d] * j;
                newy = y + m_dy[d] * j;
                if ( tmpboard[newx][newy] == Color) //连续
                {
                    count++;
                } 
                else        //不连续退出
                {
                    break;
                }
            }
            max = j;
            for ( j = 1; count < 6 ; j++)
            {
                newx = x - m_dx[d] * j;
                newy = y - m_dy[d] * j;
                if ( tmpboard[newx][newy] == Color) //连续
                {
                    count++;
                } 
                else        //不连续退出
                {
                    break;
                }
            }
            min = - j;
            if ( count >= 6 )    //六连
            {
                return 100;
            }

            if (count<4)
            {
                continue;
            }

            //5连或4连
            int rightx = x + m_dx[d] * max;
            int righty = y + m_dy[d] * max;
            int leftx,lefty;
            if ( tmpboard[rightx][righty] == NOSTONE)//一方有气
            {
                leftx = x + m_dx[d] * min;
                lefty = y + m_dy[d] * min;
                if ( tmpboard[leftx][lefty] == NOSTONE)//另一方有气
                {
                    if (count==4&&(tmpboard[leftx-m_dx[d]][lefty-m_dy[d]]==BORDER||tmpboard[leftx-m_dx[d]][lefty-m_dy[d]]==(Color^3))
                        ||(tmpboard[rightx+m_dx[d]][righty+m_dy[d]]==BORDER||tmpboard[rightx+m_dx[d]][righty+m_dy[d]]==(Color^3)))
                    {
                        continue;
                    }
                    if (count==4)
                    {
                        m_vcf_mark[leftx-m_dx[d]][lefty-m_dy[d]]=1;
                        m_vcf_mark[rightx+m_dx[d]][righty+m_dy[d]]=1;
                    }
                    //活四或活五
                    for (j=min;j<=max;j++)
                    {
                        newx = x + m_dx[d] * j;
                        newy = y + m_dy[d] * j;
                        m_vcf_use[newx][newy][d]=i+1;
                        m_vcf_mark[newx][newy]=1;
                    }
                    return 2;
                }
            }

        }
    }
    return 0;
}

int CVCFSearch::is_three(char position[GRID_NUM][GRID_NUM],char Color, pos_t * Pos)
{
    int x, y, newx, newy;        
    int count;
    int  d, j;
    int min, max;

    char tmpboard[GRID_NUM][GRID_NUM];
    memcpy(tmpboard,position,sizeof(tmpboard));

    x=Pos->x;
    y=Pos->y;
    for ( d = 0; d < 4; d++  )            //对四个方向进行分析
    {
        count = 1;                        //每个方向找出我方连续的区域
        for ( j = 1; j < 6 ; j++ )
        {
            newx = x + m_dx[d] * j;
            newy = y + m_dy[d] * j;
            if ( tmpboard[newx][newy] == Color ) //连续
            {        
                count++;
            } 
            else if(tmpboard[newx][newy] != NOSTONE)        //不连续退出
            {
                break;
            }
        }
        max = j - 1;
        for ( j = 1; j < 6 ; j++)
        {
            newx = x - m_dx[d] * j;
            newy = y - m_dy[d] * j;
            if ( tmpboard[newx][newy] == Color) //连续
            {
                count++;
            } 
            else if(tmpboard[newx][newy] != NOSTONE)        //不连续退出
            {
                break;
            }
        }
        min = 1 - j;

        if ( max - min + 1 < 6 )    //区域范围小于6
        {
            continue;
        }
        if (count<3)
        {
            continue;
        }
        j = max - 5;            //计算连续6个点有几个我方棋子
        newx = x + m_dx[d] * j;
        newy = y + m_dy[d] * j;
        count=0;
        for (; j <= max; j++)
        {
            if (tmpboard[newx][newy]==Color)
            {
                count++;
            }
            newx += m_dx[d];
            newy += m_dy[d];
        }

        if ( count >= 3 )        //如果出现3个以上我方棋子
        {
            return 1;
        }

        j = max - 6;            //6个子的区域不断移动
        newx = x + m_dx[d] * j;
        newy = y + m_dy[d] * j;
        for (; j >= min; j--)
        {

            if (tmpboard[newx][newy]==Color)
            {
                count++;
            }
            if (tmpboard[newx+m_dx[d]*6][newy+m_dy[d]*6]==Color)
            {
                count--;
            }
            if (count>=3)
            {
                return 1;
            }
            newx-=m_dx[d];
            newy-=m_dy[d];
        }
    }    
    return 0;
}


void CVCFSearch::vcf_make_move(char Color, move_t * move)            //下棋
{
    m_board[move->positions[0].x][move->positions[0].y]=Color;
    m_board[move->positions[1].x][move->positions[1].y]=Color;
}
void CVCFSearch::vcf_unmake_move(char Color, move_t * move)        //撤消招法
{
    m_board[move->positions[0].x][move->positions[0].y]=NOSTONE;
    m_board[move->positions[1].x][move->positions[1].y]=NOSTONE;
}


int CVCFSearch::vcf_get_move_list( char ourColor,char a_d, pos_t * canUse, int n_Pos, move_t * moveList, move_t * preMove)
{
    int n_MoveList=0, Count=0;
    int i, j, d, max, min;

    int x, y, newx, newy;    


    char tmpboard[GRID_NUM][GRID_NUM];
    memcpy(tmpboard,m_board,sizeof(tmpboard));    //创建临时棋盘

    if (a_d==1)//进攻
    {
        //我
        //SMove tmpMove;
        //for ( i=0;i<n_Pos;i++)
        //{
        //    tmpboard[canUse[i].x][canUse[i].y]=ourColor;
        //    tmpMove.StonePos[0]=canUse[i];
        //    
        //    if (isThree(tmpboard,ourColor,canUse+i)==0)
        //    {
        //        tmpboard[canUse[i].x][canUse[i].y]=NOSTONE;
        //        continue;
        //    }
        //    
        //    for (j=i+1;j<n_Pos;j++)            //根据当前可走点生成一些招法
        //    {
        //        tmpboard[canUse[j].x][canUse[j].y]=ourColor;
        //        tmpMove.StonePos[1]=canUse[j];        //创建临时招法并下子
        //        Count=IsAttack(tmpboard,ourColor,&tmpMove);    //返回临时招法的威胁度
        //        tmpboard[canUse[j].x][canUse[j].y]=NOSTONE;    //恢复棋盘

        //        if (Count>=50)
        //        {
        //            moveList[0].StonePos[0]=canUse[i];
        //            moveList[0].StonePos[1]=canUse[j];
        //            moveList[0].Score=Count;
        //            return 1;
        //        }

        //        if (Count>2)
        //        {
        //            moveList[n_MoveList]=moveList[0];
        //            moveList[0].StonePos[0]=canUse[i];
        //            moveList[0].StonePos[1]=canUse[j];
        //            moveList[0].Score=Count;
        //            n_MoveList++;
        //            
        //        }

        //        if (Count==2)        //如果双威胁添加招法
        //        {
        //            moveList[n_MoveList].StonePos[0]=canUse[i];
        //            moveList[n_MoveList].StonePos[1]=canUse[j];
        //            moveList[n_MoveList].Score=Count;
        //            n_MoveList++;
        //        } 

        //    }
        //    tmpboard[canUse[i].x][canUse[i].y]=NOSTONE;

        //}


        //崔皓
        /*int n_first,n_second;
        int MovePos[361],MovePos2[361];
        n_first=GetVCFMoveList(ourColor,1,canUse,n_Pos,MovePos,preMove,1);
        for (i=0;i<n_first;i++)
        {
        x=canUse[MovePos[i]].x;
        y=canUse[MovePos[i]].y;
        canUse[MovePos[i]]=canUse[n_Pos-1];
        board[x][y]=ourColor;
        n_second=GetVCFMoveList(ourColor,1,canUse,n_Pos-1,MovePos2,preMove,2);
        for (j=0;j<n_second;j++)
        {
        if (MovePos2[j]<MovePos[i])
        {
        continue;
        }
        moveList[n_MoveList].StonePos[0].x=x;
        moveList[n_MoveList].StonePos[0].y=y;
        moveList[n_MoveList].StonePos[1]=canUse[MovePos2[j]];
        board[canUse[MovePos2[j]].x][canUse[MovePos2[j]].y]=ourColor;
        if (IsAttack(board,ourColor,moveList+n_MoveList)>=2)
        {
        n_MoveList++;
        }
        board[canUse[MovePos2[j]].x][canUse[MovePos2[j]].y]=NOSTONE;
        }
        board[x][y]=NOSTONE;
        canUse[n_Pos-1]=canUse[MovePos[i]];
        canUse[MovePos[i]].x=x;
        canUse[MovePos[i]].y=y;
        }*/

        //林思然
        n_MoveList=m_dfa.pattern_match(ourColor,moveList);
        for (i=0,j=0;i<n_MoveList;i++)
        {
            move_t tmpMove, orgMove;
            int tmpCount;
            orgMove=moveList[i];
            tmpboard[moveList[i].positions[0].x][moveList[i].positions[0].y]=ourColor;
            tmpboard[moveList[i].positions[1].x][moveList[i].positions[1].y]=ourColor;
            tmpMove.positions[0] = moveList[i].positions[1];
            tmpMove.positions[1] = moveList[i].positions[0];
            Count=is_attack(tmpboard,ourColor,moveList+i);
            if ((tmpCount=is_attack(tmpboard,ourColor,&tmpMove)) > Count)
            {
                Count = tmpCount;
                moveList[i] = tmpMove;
            }

            if (Count>=2)
            {
                moveList[j]=moveList[i];
                moveList[j++].score=Count;
                if (Count>=50)
                {
                    tmpMove=moveList[j-1];
                    moveList[j-1]=moveList[0];
                    moveList[0]=tmpMove;
                }
            }

            tmpboard[orgMove.positions[0].x][orgMove.positions[0].y]=NOSTONE;
            tmpboard[orgMove.positions[1].x][orgMove.positions[1].y]=NOSTONE;
        }
        n_MoveList=j;
        for (i=0,j=0;i<n_MoveList;i++)
        {
            int k;
            for(k=i+1;k<n_MoveList;k++)
            {
                if(moveList[i].positions[0].x==moveList[k].positions[0].x&&moveList[i].positions[0].y==moveList[k].positions[0].y&&
                    moveList[i].positions[1].x==moveList[k].positions[1].x&&moveList[i].positions[1].y==moveList[k].positions[1].y ||
                    moveList[i].positions[0].x==moveList[k].positions[1].x&&moveList[i].positions[0].y==moveList[k].positions[1].y&&
                    moveList[i].positions[1].x==moveList[k].positions[0].x&&moveList[i].positions[1].y==moveList[k].positions[0].y)
                {
                    break;
                }
            }
            if (k>=n_MoveList)
            {
                moveList[j++]=moveList[i];
            }
        }
        n_MoveList=j;
        
    } 
    else        //防守
    {
        if (is_attack(tmpboard,ourColor^3,preMove)!=2)
        {
            return 0;
        }

        pos_t Pos[2][10];
        int nPos[2];
        nPos[0]=nPos[1]=0;
        int dir=0;
        for ( i = 0; i <= 1; i++ )
        {
            x = preMove->positions[i].x;            //前一招法两点的坐标
            y = preMove->positions[i].y;
            for ( d = 0; d < 4; d++  )            //对四个方向进行分析
            {

                if (!m_vcf_use[x][y][d])        //如果未评价过退出
                {
                    continue;
                }
                if (dir>=2)
                {
                    break;
                }
                //找出当前方向上已评价过的连续的点
                int n_Nostone=0;
                Count=0;
                for ( j = 1; ; j++ )
                {
                    newx = x + m_dx[d] * j;
                    newy = y + m_dy[d] * j;
                    if ( m_vcf_use[newx][newy][d] ) //连续并评价过
                    {
                        if (tmpboard[newx][newy]==NOSTONE)
                        {
                            n_Nostone++;
                            if (m_vcf_use[newx][newy][d]==i+1)
                            {
                                Pos[dir][nPos[dir]].x=newx;
                                Pos[dir][nPos[dir]++].y=newy;
                            }
                            else
                            {
                                break;
                            }
                        }

                    } 
                    else        //退出
                    {
                        break;
                    }
                }
                newx-=m_dx[d];
                newy-=m_dy[d];
                if (tmpboard[newx][newy]==NOSTONE)//&& tmpboard[newx+dx[d]][newy+dy[d]]!=BORDER
                    //&&tmpboard[newx+dx[d]][newy+dy[d]]!=ourColor)//如果边缘的点未落子
                {
                    Count++;
                }
                max=j-1;//保存最大点

                for ( j = 1; ; j++ )
                {
                    newx = x - m_dx[d] * j;
                    newy = y - m_dy[d] * j;
                    if ( m_vcf_use[newx][newy][d] ) //连续并评价过
                    {
                        if (tmpboard[newx][newy]==NOSTONE)
                        {
                            n_Nostone++;
                            if (m_vcf_use[newx][newy][d]==i+1)
                            {
                                Pos[dir][nPos[dir]].x=newx;
                                Pos[dir][nPos[dir]++].y=newy;
                            }
                            else
                            {
                                break;
                            }
                        }
                    } 
                    else        //退出
                    {
                        break;
                    }
                }

                newx+=m_dx[d];
                newy+=m_dy[d];
                if ( tmpboard[newx][newy]==NOSTONE)// && tmpboard[newx-dx[d]][newy-dy[d]]!=BORDER
                    //&&tmpboard[newx-dx[d]][newy-dy[d]]!=ourColor)//如果边缘的点未落子
                {
                    Count++;
                }
                min=1-j;//保存最小点

                if (max-min+1<6)
                {
                    continue;
                }

                dir++;


                if (Count==2&&n_Nostone==2)//如果边缘两个点是空子，即双威胁
                {
                    if (max-min==5)
                    {
                        newx=x+m_dx[d]*(max+1);
                        newy=y+m_dy[d]*(max+1);
                        if (tmpboard[newx][newy]==BORDER||tmpboard[newx][newy]==ourColor)
                        {
                            continue;
                        }
                        newx=x+m_dx[d]*(min-1);
                        newy=y+m_dy[d]*(min-1);
                        if (tmpboard[newx][newy]==BORDER||tmpboard[newx][newy]==ourColor)
                        {
                            continue;
                        }
                    }
                    n_MoveList=0;
                    newx=x+m_dx[d]*max;
                    newy=y+m_dy[d]*max;
                    moveList[n_MoveList].positions[0].x=newx;
                    moveList[n_MoveList].positions[0].y=newy;
                    newx=x+m_dx[d]*min;
                    newy=y+m_dy[d]*min;
                    moveList[n_MoveList].positions[1].x=newx;
                    moveList[n_MoveList].positions[1].y=newy;
                    n_MoveList++;
                    newx=x+m_dx[d]*(max+1);
                    newy=y+m_dy[d]*(max+1);
                    if ( tmpboard[newx][newy]==NOSTONE)
                    {
                        moveList[n_MoveList].positions[0].x=newx;
                        moveList[n_MoveList].positions[0].y=newy;
                        moveList[n_MoveList].positions[1]=moveList[0].positions[1];
                        n_MoveList++;
                    }
                    newx=x+m_dx[d]*(min-1);
                    newy=y+m_dy[d]*(min-1);
                    if ( tmpboard[newx][newy]==NOSTONE)
                    {
                        moveList[n_MoveList].positions[0].x=newx;
                        moveList[n_MoveList].positions[0].y=newy;
                        moveList[n_MoveList].positions[1]=moveList[0].positions[0];
                        n_MoveList++;
                    }
                    return n_MoveList;
                }

            }
        }

        n_MoveList=0;
        for (i=0;i<nPos[0];i++)
        {
            for (j=0;j<nPos[1];j++)  //招找到的点生成招法
            {
                if (Pos[0][i].x!=Pos[1][j].x||Pos[0][i].y!=Pos[1][j].y)
                {
                    moveList[n_MoveList].positions[0]=Pos[0][i];
                    moveList[n_MoveList].positions[1]=Pos[1][j];
                    n_MoveList++;
                }

            }
        }
    }
    return n_MoveList;
}

inline int CVCFSearch::vcf_abs(int a)
{
    return a >= 0 ? a : -a;
}

inline int CVCFSearch::dist(pos_t p1, pos_t p2, pos_t pt)
{
    return (vcf_abs(p1.x-pt.x)+vcf_abs(p1.y-pt.y)+vcf_abs(p2.x-pt.x)+vcf_abs(p2.y-pt.y));
}

inline int CVCFSearch::vcf_min(int a, int b)
{
    return a <= b ? a : b;
}

static int cmp(const void * a, const void * b)
{
    ListNode aa=*(ListNode*)a, bb=*(ListNode*)b;
    if (aa.score!=bb.score)
    {
        return bb.score-aa.score;
    }
    
    return aa.dist-bb.dist;

}

void CVCFSearch::sort(move_t * moveList, int n_moveList, move_t * preMove)
{
    int i;

    for (i=0;i<n_moveList;i++)
    {
        m_tmp_move_list[i]=moveList[i];
        m_list_node[i].dist=vcf_min(dist(moveList[i].positions[0],moveList[i].positions[1],preMove->positions[0]),
                             dist(moveList[i].positions[0],moveList[i].positions[1],preMove->positions[1]));
        m_list_node[i].score= (int)moveList[i].score;
        m_list_node[i].pos=i;
    }
    
    qsort(m_list_node,n_moveList,sizeof(ListNode),cmp);
    for (i=0;i<n_moveList;i++)
    {
        moveList[i]=m_tmp_move_list[m_list_node[i].pos];
    }

}

unsigned long CVCFSearch::vcf_hash_board(char board[GRID_NUM][GRID_NUM])
{
    unsigned long ans=0;
    int i, j;
    for (i=1;i<=19;i++)
    {
        for (j=1;j<=19;j++)
        {
            ans=(ans<<4)+board[i][j];
            unsigned long g=ans & 0xf0000000L;
            if (g)
            {
                ans^=g>>24;
            }
            ans &= ~g;
        }
    }
    return ans;
}

int CVCFSearch::vcf_hash_check(HashNode node)
{
    int i, t;
    int dep=node.dep;
    unsigned int hash=node.hash;
    int p=hash%HASHSIZE;
    char tmpboard[GRID_NUM][GRID_NUM];
    memcpy(tmpboard,m_org_board,sizeof(tmpboard));

    tmpboard[node.move.positions[0].x][node.move.positions[0].y]=node.color;
    tmpboard[node.move.positions[1].x][node.move.positions[1].y]=node.color;
    i=node.pre;
    while (i>=0)
    {
        tmpboard[m_hash_que[i].move.positions[0].x][m_hash_que[i].move.positions[0].y]=m_hash_que[i].color;
        tmpboard[m_hash_que[i].move.positions[1].x][m_hash_que[i].move.positions[1].y]=m_hash_que[i].color;
        i=m_hash_que[i].pre;
    }

    for (i=0;i< (int)(m_hash_map[dep][p].size());i++)
    {
        if (m_hash_que[m_hash_map[dep][p][i]].hash==hash)
        {
            t=m_hash_map[dep][p][i];
            while (t>=0)
            {
                if (tmpboard[m_hash_que[t].move.positions[0].x][m_hash_que[t].move.positions[0].y]!=m_hash_que[t].color||
                    tmpboard[m_hash_que[t].move.positions[1].x][m_hash_que[t].move.positions[1].y]!=m_hash_que[t].color)
                {
                    break;
                }
                t=m_hash_que[t].pre;
            }
            if (t<0)
            {
                t=m_hash_map[dep][p][i];
                return t;
            }
        }
    }
    return -1;
}

bool CVCFSearch::anti_vcf_search(int depth,char ourColor,move_t * bestMove,move_t * preMove, int preNode, int prePos)
{
    int Color = ourColor;            //当前轮到谁下子
    int i,j, t;
    int n_pos=0, n_moveList;
    pos_t canUse[GRID_NUM*GRID_NUM];                //可落子的点
    move_t * moveList = m_vcf_move_list[depth];
    bool flag;
    int NodeID;
    int CurPos=m_vcf_node++;
    char tmpboard[GRID_NUM][GRID_NUM];

    memcpy(tmpboard,m_board,sizeof(tmpboard));

    if (depth==0)
    {
        memcpy(m_org_board,tmpboard,sizeof(tmpboard));
        for (i=0;i<VCFDEPTH+1;i++)
        {
            for (j=0;j<HASHSIZE;j++)
            {
                m_hash_map[i][j].clear();
            }
        }
        m_vcf_total_node=0;
    }

    m_hash_que[CurPos].move=*preMove;
    m_hash_que[CurPos].pre=prePos;
    m_hash_que[CurPos].hash=vcf_hash_board(tmpboard);
    m_hash_que[CurPos].dep=depth;
    m_hash_que[CurPos].color=Color;
    if ((t=vcf_hash_check(m_hash_que[CurPos]))!=-1)
    {
        return m_hash_que[t].res;
    }


    //return false;

    if (depth>=ANTIVCFDEPTH)   //超过预定的层数，VCF失败
    {
        m_hash_que[CurPos].res=false;
        m_hash_map[depth][m_hash_que[CurPos].hash%HASHSIZE].push_back(CurPos);
        return false;
    }

    if (is_win_by_premove(m_board, preMove)) //如果有一方胜利，退出当前搜索
    {
        m_hash_que[CurPos].res=(Color!=(m_chess_type));
        m_hash_map[depth][m_hash_que[CurPos].hash%HASHSIZE].push_back(CurPos);
        return Color!=(m_chess_type);
    }

    if (Color==((m_chess_type)^3) && is_attack(m_board,Color^3,preMove))
    {
        n_moveList=vcf_get_move_list(Color,1,canUse,n_pos,moveList, preMove); //生成招法

        sort(moveList,n_moveList,preMove);

        for (i=0;i<n_moveList;i++)
        {
            if (moveList[i].score>=50)
            {
                NodeID=++m_vcf_total_node;
                if (depth==0)
                {
                    *bestMove=moveList[i];
                    m_vcf_now_pos=NodeID;
                }

                m_hash_que[CurPos].res=true;
                m_hash_map[depth][m_hash_que[CurPos].hash%HASHSIZE].push_back(CurPos);

                return true;
            }
            vcf_make_move(Color,moveList+i);    //下子
            if (is_attack(m_board,Color^3,preMove))
            {
                vcf_unmake_move(Color,moveList+i);    //撤子
                continue;
            }
            NodeID=++m_vcf_total_node;
            flag = anti_vcf_search(depth+1,ourColor^3,bestMove,moveList+i,NodeID,CurPos); //返回下一步VCF的结果
            vcf_unmake_move(Color,moveList+i);    //撤子
            if (flag)    //如果成功，当前VCF成功并返回
            {
                if (depth==0)
                {
                    *bestMove=moveList[i];
                    m_vcf_now_pos=NodeID;
                }

                m_hash_que[CurPos].res=true;
                m_hash_map[depth][m_hash_que[CurPos].hash%HASHSIZE].push_back(CurPos);

                return true;
            }
            m_vcf_total_node=NodeID-1;
        }
        m_hash_que[CurPos].res=false;
        m_hash_map[depth][m_hash_que[CurPos].hash%HASHSIZE].push_back(CurPos);
        return false;        //如果所有招法无法成功则VCF失败
    }

    if (Color==(m_chess_type)&& is_attack(m_board,Color^3,preMove)>2)
    {
        m_hash_que[CurPos].res=true;
        m_hash_map[depth][m_hash_que[CurPos].hash%HASHSIZE].push_back(CurPos);
        return true;
    }

        

    if ( Color==((m_chess_type)^3) )
    {
        //n_moveList=PatternMatch(ourColor,moveList);

        n_moveList=vcf_get_move_list(Color,1,canUse,n_pos,moveList, preMove); //生成招法

        sort(moveList,n_moveList,preMove);

        for (i=0;i<n_moveList;i++)
        {    
            NodeID=++m_vcf_total_node;
            vcf_make_move(Color,moveList+i);    //下子
            flag = anti_vcf_search(depth+1,ourColor^3,bestMove,moveList+i,NodeID,CurPos); //返回下一步VCF的结果
            vcf_unmake_move(Color,moveList+i);    //撤子
            if (flag)    //如果成功，当前VCF成功并返回
            {
                if (depth==0)
                {

                    *bestMove=moveList[i];
                    m_vcf_now_pos=NodeID;
                }
                m_hash_que[CurPos].res=true;
                m_hash_map[depth][m_hash_que[CurPos].hash%HASHSIZE].push_back(CurPos);
                return true;
            }
            m_vcf_total_node=NodeID-1;
        }
        m_hash_que[CurPos].res=false;
        m_hash_map[depth][m_hash_que[CurPos].hash%HASHSIZE].push_back(CurPos);
        return false;        //如果所有招法无法成功则VCF失败
    }
    else
    {

        n_moveList=vcf_get_move_list(Color,0,canUse,n_pos,moveList, preMove);//生成招法
        j=m_vcf_total_node;
        for (i=0;i<n_moveList;i++)
        {
            NodeID=++m_vcf_total_node;
            vcf_make_move(Color,moveList+i);//下子
            flag = anti_vcf_search(depth+1,ourColor^3,bestMove,moveList+i,NodeID,CurPos);//返回下一步VCF的结果
            vcf_unmake_move(Color,moveList+i);    //撤子
            if ( !flag )//如果失败，当前VCF失败并返回
            {
                m_vcf_total_node=j;
                m_hash_que[CurPos].res=false;
                m_hash_map[depth][m_hash_que[CurPos].hash%HASHSIZE].push_back(CurPos);
                return false;
            }

        }
        m_hash_que[CurPos].res=true;
        m_hash_map[depth][m_hash_que[CurPos].hash%HASHSIZE].push_back(CurPos);
        return true;        //如果所有招法成功则VCF成功
    }
}

/*
攻方只要有一种招法能使VCF成功则当前VCF成功，否则失败
防方只要有一种招法能使VCF失败则当前VCF失败，否则成功

*/
bool CVCFSearch::vcf_search(int depth,char ourColor,move_t * bestMove,move_t * preMove, int preNode, int prePos)
{
    int Color = ourColor;            //当前轮到谁下子
    int i,j, t;
    int nRoundi0,nRoundi1,nRoundj0,nRoundj1;
    int n_pos=0, n_moveList;
    pos_t canUse[GRID_NUM*GRID_NUM];                //可落子的点
    move_t * moveList = m_vcf_move_list[depth];
    bool flag;
    int NodeID;
    int CurPos=m_vcf_node++;
    char tmpboard[GRID_NUM][GRID_NUM];

    memcpy(tmpboard,m_board,sizeof(tmpboard));

    if (m_has_win)
    {
        for (int i=m_vcf_now_pos+1;i<=m_vcf_total_node;i++)
        {
            if (m_vcf_move_table[i].pre==m_vcf_now_pos&&(m_vcf_move_table[i].p1.x==preMove->positions[0].x&&
                m_vcf_move_table[i].p1.y==preMove->positions[0].y&&m_vcf_move_table[i].p2.x==preMove->positions[1].x&&
                m_vcf_move_table[i].p2.y==preMove->positions[1].y||m_vcf_move_table[i].p2.x==preMove->positions[0].x&&
                m_vcf_move_table[i].p2.y==preMove->positions[0].y&&m_vcf_move_table[i].p1.x==preMove->positions[1].x&&
                m_vcf_move_table[i].p1.y==preMove->positions[1].y))
            {
                bestMove->positions[0]=m_vcf_move_table[m_vcf_move_table[i].next].p1;
                bestMove->positions[1]=m_vcf_move_table[m_vcf_move_table[i].next].p2;
                m_vcf_now_pos=m_vcf_move_table[i].next;
                return true;
            }
        }
        m_has_win=0;
    }
    
    if (depth==0)
    {
        memcpy(m_org_board,tmpboard,sizeof(tmpboard));
        for (i=0;i<VCFDEPTH+1;i++)
        {
            for (j=0;j<HASHSIZE;j++)
            {
                m_hash_map[i][j].clear();
            }
        }
        m_vcf_total_node=0;
    }
    
    m_hash_que[CurPos].move=*preMove;
    m_hash_que[CurPos].pre=prePos;
    m_hash_que[CurPos].hash=vcf_hash_board(tmpboard);
    m_hash_que[CurPos].dep=depth;
    m_hash_que[CurPos].color=Color;
    if ((t=vcf_hash_check(m_hash_que[CurPos]))!=-1)
    {
        return m_hash_que[t].res;
    }


    //return false;

    if (depth>=VCFDEPTH)   //超过预定的层数，VCF失败
    {
        m_hash_que[CurPos].res=false;
        m_hash_map[depth][m_hash_que[CurPos].hash%HASHSIZE].push_back(CurPos);
        return false;
    }

    if (depth>=10&&m_vcf_node>500000)
    {
        m_hash_que[CurPos].res=false;
        m_hash_map[depth][m_hash_que[CurPos].hash%HASHSIZE].push_back(CurPos);
        return false;
    }

    if (is_win_by_premove(m_board, preMove)) //如果有一方胜利，退出当前搜索
    {
        m_hash_que[CurPos].res=(Color!=(m_chess_type));
        m_hash_map[depth][m_hash_que[CurPos].hash%HASHSIZE].push_back(CurPos);
        return Color!=(m_chess_type);
    }

    if (Color==(m_chess_type) && is_attack(m_board,Color^3,preMove))
    {

        //找出棋盘上已落子的点向外招展两圈可下的点
        //开始---------------------------------------------------------------

        nRoundi0 = 19;
        nRoundj0 = 19;
        nRoundi1 = 1;
        nRoundj1 = 1;

        for (i = 1; i <= 19; i++)
        {
            for (j = 1; j <= 19; j++)
            {
                if (m_board[i][j] != NOSTONE)
                {
                    if (i < nRoundi0)
                    {
                        nRoundi0 = i;
                    }
                    if (i > nRoundi1)
                    {
                        nRoundi1 = i;
                    }
                    if (j < nRoundj0)
                    {
                        nRoundj0 = j;
                    }
                    if (j > nRoundj1)
                    {
                        nRoundj1 = j;
                    }
                }
            }
        }
        nRoundi0 -= 2;
        nRoundj0 -= 2;
        nRoundi1 += 2;
        nRoundj1 += 2;
        if (nRoundi0 < 1)
        {
            nRoundi0 = 1;
        }
        if (nRoundi1 > 19)
        {
            nRoundi1 = 19;
        }
        if (nRoundj0 < 1)
        {
            nRoundj0 = 1;
        }
        if (nRoundj1 > 19)
        {
            nRoundj1 = 19;
        }


        for (i = nRoundi0; i <= nRoundi1; i++)
        {
            for (j = nRoundj0; j <= nRoundj1; j++)
            {
                if (m_board[i][j] == NOSTONE)
                {
                    canUse[n_pos].x=i;
                    canUse[n_pos].y=j;
                    n_pos++;
                }
            }
        }

        //结束---------------------------------------------------------------

        n_moveList=vcf_get_move_list(Color,1,canUse,n_pos,moveList, preMove); //生成招法

        sort(moveList,n_moveList,preMove);

        for (i=0;i<n_moveList;i++)
        {    
            if (moveList[i].score>=50)
            {
                NodeID=++m_vcf_total_node;
                if (depth==0)
                {
                    *bestMove=moveList[i];
                    m_has_win=1;
                    m_vcf_now_pos=NodeID;
                }
                m_vcf_move_table[NodeID].pre=preNode;
                m_vcf_move_table[preNode].next=NodeID;
                m_vcf_move_table[NodeID].p1=moveList[i].positions[0];
                m_vcf_move_table[NodeID].p2=moveList[i].positions[1];

                m_hash_que[CurPos].res=true;
                m_hash_map[depth][m_hash_que[CurPos].hash%HASHSIZE].push_back(CurPos);

                return true;
            }
            vcf_make_move(Color,moveList+i);    //下子
            if (is_attack(m_board,Color^3,preMove))
            {
                vcf_unmake_move(Color,moveList+i);    //撤子
                continue;
            }
            NodeID=++m_vcf_total_node;
            flag = vcf_search(depth+1,ourColor^3,bestMove,moveList+i,NodeID,CurPos); //返回下一步VCF的结果
            vcf_unmake_move(Color,moveList+i);    //撤子
            if (flag)    //如果成功，当前VCF成功并返回
            {
                if (depth==0)
                {
                    *bestMove=moveList[i];
                    m_has_win=1;
                    m_vcf_now_pos=NodeID;
                }
                m_vcf_move_table[NodeID].pre=preNode;
                m_vcf_move_table[preNode].next=NodeID;
                m_vcf_move_table[NodeID].p1=moveList[i].positions[0];
                m_vcf_move_table[NodeID].p2=moveList[i].positions[1];

                m_hash_que[CurPos].res=true;
                m_hash_map[depth][m_hash_que[CurPos].hash%HASHSIZE].push_back(CurPos);

                return true;
            }
            m_vcf_total_node=NodeID-1;
        }
        m_hash_que[CurPos].res=false;
        m_hash_map[depth][m_hash_que[CurPos].hash%HASHSIZE].push_back(CurPos);
        return false;        //如果所有招法无法成功则VCF失败
    }

    if ((Color^3)==(m_chess_type)&& is_attack(m_board,Color^3,preMove)>2)
    {
        m_hash_que[CurPos].res=true;
        m_hash_map[depth][m_hash_que[CurPos].hash%HASHSIZE].push_back(CurPos);
        return true;
    }

    if (Color==(m_chess_type))
    {
        //找出棋盘上已落子的点向外招展两圈可下的点
        //开始---------------------------------------------------------------

        nRoundi0 = 19;
        nRoundj0 = 19;
        nRoundi1 = 1;
        nRoundj1 = 1;

        for (i = 1; i <= 19; i++)
        {
            for (j = 1; j <= 19; j++)
            {
                if (m_board[i][j] != NOSTONE)
                {
                    if (i < nRoundi0)
                    {
                        nRoundi0 = i;
                    }
                    if (i > nRoundi1)
                    {
                        nRoundi1 = i;
                    }
                    if (j < nRoundj0)
                    {
                        nRoundj0 = j;
                    }
                    if (j > nRoundj1)
                    {
                        nRoundj1 = j;
                    }
                }
            }
        }
        nRoundi0 -= 2;
        nRoundj0 -= 2;
        nRoundi1 += 2;
        nRoundj1 += 2;
        if (nRoundi0 < 1)
        {
            nRoundi0 = 1;
        }
        if (nRoundi1 > 19)
        {
            nRoundi1 = 19;
        }
        if (nRoundj0 < 1)
        {
            nRoundj0 = 1;
        }
        if (nRoundj1 > 19)
        {
            nRoundj1 = 19;
        }


        for (i = nRoundi0; i <= nRoundi1; i++)
        {
            for (j = nRoundj0; j <= nRoundj1; j++)
            {
                if (m_board[i][j] == NOSTONE)
                {
                    canUse[n_pos].x=i;
                    canUse[n_pos].y=j;
                    n_pos++;
                }
            }
        }

        //结束---------------------------------------------------------------
    }

    if ( Color==m_chess_type )
    {
        //n_moveList=PatternMatch(ourColor,moveList);

        n_moveList=vcf_get_move_list(Color,1,canUse,n_pos,moveList, preMove); //生成招法

        sort(moveList,n_moveList,preMove);

        for (i=0;i<n_moveList;i++)
        {    
            NodeID=++m_vcf_total_node;
            vcf_make_move(Color,moveList+i);    //下子
            flag = vcf_search(depth+1,ourColor^3,bestMove,moveList+i,NodeID,CurPos); //返回下一步VCF的结果
            vcf_unmake_move(Color,moveList+i);    //撤子
            if (flag)    //如果成功，当前VCF成功并返回
            {
                if (depth==0)
                {

                    *bestMove=moveList[i];
                    m_has_win=1;
                    m_vcf_now_pos=NodeID;
                }
                m_vcf_move_table[NodeID].pre=preNode;
                m_vcf_move_table[preNode].next=NodeID;
                m_vcf_move_table[NodeID].p1=moveList[i].positions[0];
                m_vcf_move_table[NodeID].p2=moveList[i].positions[1];
                m_hash_que[CurPos].res=true;
                m_hash_map[depth][m_hash_que[CurPos].hash%HASHSIZE].push_back(CurPos);
                return true;
            }
            m_vcf_total_node=NodeID-1;
        }
        m_hash_que[CurPos].res=false;
        m_hash_map[depth][m_hash_que[CurPos].hash%HASHSIZE].push_back(CurPos);
        return false;        //如果所有招法无法成功则VCF失败
    }
    else
    {

        n_moveList=vcf_get_move_list(Color,0,canUse,n_pos,moveList, preMove);//生成招法
        j=m_vcf_total_node;
        for (i=0;i<n_moveList;i++)
        {
            NodeID=++m_vcf_total_node;
            vcf_make_move(Color,moveList+i);//下子
            flag = vcf_search(depth+1,ourColor^3,bestMove,moveList+i,NodeID,CurPos);//返回下一步VCF的结果
            vcf_unmake_move(Color,moveList+i);    //撤子
            if ( !flag )//如果失败，当前VCF失败并返回
            {
                m_vcf_total_node=j;
                m_hash_que[CurPos].res=false;
                m_hash_map[depth][m_hash_que[CurPos].hash%HASHSIZE].push_back(CurPos);
                return false;
            }
            m_vcf_move_table[NodeID].pre=preNode;
            m_vcf_move_table[NodeID].p1=moveList[i].positions[0];
            m_vcf_move_table[NodeID].p2=moveList[i].positions[1];
        }
        m_hash_que[CurPos].res=true;
        m_hash_map[depth][m_hash_que[CurPos].hash%HASHSIZE].push_back(CurPos);
        return true;        //如果所有招法成功则VCF成功
    }
}

