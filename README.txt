Copyright (c) 2008-2013 Hao Cui <Hao.Cui@Tufts.edu>,
                        Liang Li <liliang010@gmail.com>,
                        Ruijian Wang <jeoygin@gmail.com>,
                        Siran Lin <linsiran@gmail.com>.
                        All rights reserved.
License: BSD-style

================================================================================

About Cloudict
--------------

Cloudict is a game engine for the game Connect6, developed by four computer
fellows named Hao Cui, Liang Li, Ruijian Wang, Siran Lin.
For the computer game Connect6, please see:

        http://en.wikipedia.org/wiki/Connect6

Using some technologies, including alpha-beta search, VCF, preprunning, etc.
the game engine has relatively high AI and won some honors in Computer Olympiad
with different names in different periods:

        GOLD MEDAL as Cloudict in 16th Computer Olympiad, Connect6, 2011
        http://www.grappa.univ-lille3.fr/icga/tournament.php?id=244

        GOLD MEDAL as Bit in 14th Computer Olympiad, Connect6, 2009
        http://www.grappa.univ-lille3.fr/icga/tournament.php?id=201

        SILVER MEDAL as Bitstronger in 13th Computer Olympiad, Connect6, 2008
        http://www.grappa.univ-lille3.fr/icga/tournament.php?id=186


    Have fun! :-)

================================================================================

Compile Notes
-------------

* For Linux Family, just
    make
  It will generate the runnable program gameEngine, with the pattern file
  patterns.in.

* For Windows Family,
    chose your preferred compilers.

================================================================================

Runtime Notes
-------------

After compiled, there will be a runnable program gameEngine. It will take the
file patterns.in in current direction for VCF search.
Run the program with or without a argument; it will take the string as engine
name, while default is Cloudict.Connect6.
The command list as follows:

        name        - print the name of the Game Engine.
        print       - print the board.
        exit/quit   - quit the game.
        black XXXX  - place the black stone on the position XXXX in the board.
        white XXXX  - place the write stone on the XXXX in the board, X is the A-S.
        next        - the engine will search the move for next step.
        move XXXX   - tell the engine that the opponent take the move XXXX,
                        and the engine will search the move for next step.
        new black   - start a new game and set the engine to Black player.
        new white   - start a new game and set it to White.
        depth d     - set the alpha beta search depth, default is 6.
	vcf   	    - set vcf search.
	unvcf	    - set none vcf search.
        help        - print this help.

