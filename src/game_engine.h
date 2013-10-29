
#include "search_engine.h"
#include "vcf_search.h"

class CGameEngine {
    public:
        CGameEngine();
        int init();
        int run();

    private:
        // Init the game board before a game.
        void init_game();
        bool search_a_move(char ourColor,move_t* bestMove);

    private:
        char                m_board[GRID_NUM][GRID_NUM];                    //´æ´¢Æå¾Ö
        move_t              m_best_move;                    //·µ»ØµÄ×îºÃÕÐ·¨
        int                 m_chess_type;                    //¼ÇÂ¼Æå×ÓÑÕÉ«
        int                 m_alphabeta_depth;

        CSearchEngine       m_search_engine;
        CVCFSearch          m_vcf_search;

        char                m_engine_name[MSG_LENGTH];

};

