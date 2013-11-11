/*
 * Copyright (c) 2008-2013 Hao Cui<bit.cuihao@gmail.com>,
 *                         Liang Li<liliang010@gmail.com>,
 *                         Ruijian Wang<wrjchn@gmail.com>,
 *                         Siran Lin<37863581@qq.com>.
 *                         All rights reserved.
 *
 * This program is a free software; you can redistribute it and/or modify
 * it under the terms of the BSD license. See LICENSE.txt for details.
 *
 * Date: 2013/11/01
 *
 */

#include "game_engine.h"

// Main function for the app, taking the first argument as the Engine's name.
int main(int argc, char* args[])
{
    CGameEngine* gameEngine = new CGameEngine();
    gameEngine->init(args[1]);

    gameEngine->run();

    delete gameEngine;
    return 0;
}


