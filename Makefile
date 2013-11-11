# Copyright (c) 2008-2013 Hao Cui<bit.cuihao@gmail.com>,
#                         Liang Li<liliang010@gmail.com>,
#                         Ruijian Wang<wrjchn@gmail.com>,
#                         Siran Lin<37863581@qq.com>.
#                         All rights reserved.
#
# This program is a free software; you can redistribute it and/or modify
# it under the terms of the BSD license. See LICENSE.txt for details.
#
# Authurs: Liang Li
# Date:    2013/11/01
#

CC    = g++
FILES = src/*.cc
OBJ   = gameEngine

all:
	${CC} ${FILES} -o ${OBJ}

