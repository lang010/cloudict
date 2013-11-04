# Copyright (c) 2008-2013 Hao Cui<>,
#                         Liang Li<liliang010@gmail.com>,
#                         Ruijian Wang<>,
#                         Siran Lin<>.
#                         All rights reserved.
#
# This program is a free software; you can redistribute it and/or modify
# it under the terms of the BSD license. See LICENSE.txt for details.
#
# 2013/11/01
#

CC    = g++
FILES = src/*.cc
OBJ   = gameEngine

all:
	${CC} ${FILES} -o ${OBJ}

