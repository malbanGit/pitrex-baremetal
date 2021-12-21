README.txt, for ICBM3D v.0.4

by Bill Kendrick
nbs@sonic.net
http://www.newbreedsoftware.com/

July 30, 1998
----------------------------------------------------------------------------

WHAT IS IT?
-----------
ICBM3D ("Inter-Continental Ballistic Missiles, 3D") is a 3D game of defense.

Like the original "Missile Command" and its clones, this game places you in
control of Anti-ICBM weapons which you use to destroy an onslaught of
missiles (and other nasties) which are dropping onto your nation.

The game ends when your cities have all been destroyed. You only gain
replacement cities by acheiving certain score thresholds during each attack
(level).

The main difference between this game and Missile Command is that ICBM3D,
as the name suggests, provides a 3D perspective. You take advantage
of X-Window's 3-button mouse to control your firing sight ("cross-hair")
in 3-dimensions, and change your viewpoint.


HISTORY
-------
  Changes Since 0.3
  -----------------
  Installation:
    * The "makefile" has been updated.
      Thanks to Tim Rightnour <root@garbled.net> for his contribution!
    * "keydefs.h" created so you can edit the controls to suit your
      needs (and your keyboard)
    * Multiple language support added.  Copy "lang_english.h" to
      "lang_YOUR-LANGUAGE.h", change the words, and recompile with
      "LANGUAGE=YOUR-LANGUAGE" on "make"'s command-line.
  Game:
    * Keyboard control is better.  You can now hold down multiple keys.

  Changes Since 0.2
  -----------------
  Game:
    * A plane has been added, which flys by, drops bombs, and takes off.
    * You can zoom in and out with the [I] and [O] keys.
  Other:
    * The title "effect" has been spruced up a little bit MORE!

  Changes Since 0.1
  -----------------
  Game:
    * When your crosshair pointer is close to a missile, it turns red.
      (This makes playing while in "Missile View" mode MUCH easier!)
    * When you fire Anti-ICBM bullets, small "X"'s appear at the position
      you fired to (ie, where the bullet will go off).
    * A sky (horizon) has been added.
  Other:
    * The title "effect" has been spruced up a little.
    * Some extra info. and commands appear when the game is over.
    * The "-lyrics" command-line option has been added.

  Changes Since 0.0
  -----------------
  * A title mode has been added.
  * The [K] command (Previous Level) has been added.
  * The "PAUSED" message now does a nifty sine-wave animation.
  * You can toggle between Complex (default) and Simple (faster) cities.
  * A "Half-Frame" mode has been added.
  * Wind has been added.


COMPILING THE GAME
------------------

To compile the game, type:

  make PREFIX=path-to-your-x-stuff INSTALLROOT=path-to-put-icbm3d

Where "path-to-your-x-stuff" is the location of your X-Window files
(ie, "/usr/X11" or "/usr/openwin", etc.) and "path-to-put-icbm3d"
is the place to put the program when the compilation is complete.

For example:

  make PREFIX=/usr/X11 INSTALLROOT=/usr/X11/games

or:

  make PREFIX=/usr/openwin INSTALLROOT=/home/kendrick/icbm3d


CONFIGURING
-----------

Keyboard:

  You may decide that you don't like the default keyboard controls.
  If not, there's a small file called "keydefs.h" which you can edit
  (it's pretty obvious what to change).  Recompile to make your changes
  happen.

Language:

  You also may decide to translate the game into a different language.
  To do this, copy the file "lang_english.h" into a new file, say,
  "lang_french.h".  Edit that new file, and when you compile the game,
  add "LANGUAGE=french" to "make"'s command-line.

  For example:

    make PREFIX=/usr/X11 INSTALLROOT=/home/kendrick/icbm3d LANGUAGE=swahili

  Note: an interim file named "language.h" is created during the build.
  Don't edit THIS file, as it gets clobbered each time you rebuild!
  (If you don't specify "LANGUAGE=..." to "make", it will default to
  "english".)

Double-Buffering:

  If, when you run the game, you receive an error about "Pixmaps", or if
  you wish to run the game without double-buffering (which is used to
  reduce flicker), there's a "#define" in the "icbm3d.c" source code file
  called "DOUBLE_BUFFER" which you can set to "False".

Etc.:

  There are also many other "#define"'s and constants defined at the
  top of the "icbm3d.c" source code file which you may enjoy playing with.
  I won't cover those in this document, however.


RUNNING THE GAME
----------------

Quick-And-Easy:

  Run the game by executing "icbm3d" from your shell while logged into an
  X-Window server.  For example, at the "%" command prompt:

    % ./icbm3d


What Display:

  You can optionally specify the display to play the game on.  If you don't,
  the program will attempt to determine your display by looking at the
  "DISPLAY" environment variable:

    % ./icbm3d my.computer.edu:0.0


Selecting a Level:

  To start out at a specific level, append "-L", followed (without a space)
  by the level number you want, to the end of the command line:

    % ./icbm3d -L5
    % ./icbm3d my.computer.edu:0.0 -l10


Getting Help:

  If you wish to be reminded of the usage and keyboard controls, use the
  "-help" switch:

    % ./icbm3d -help

  Note: the help display shows you the current keyboard mappings, as they
  were compiled into the game.


Version:

  If you want to verify which version of ICBM3D you have, you can use
  the "-version" switch:

    % ./icbm3d -version


Lyrics (?):

  For some fun (and very appropriate) lyrics to a hilarious Tom Leher
  song, use the "-lyrics" switch (or just "-l").  You'll probably want
  to pipe it through "more" or "less", since it's longer than 24 lines
  (the typical terminal size), or to "lpr" to print it, or redirect it
  to a file with your shell's ">" command.

    % ./icbm3d -lyrics | more
    % ./icbm3d -lyrics > lyrics.txt

Note: The game does not begin when you ask for "-help", "-version"
or "-lyrics"...


TITLE SCREEN
------------

When the game starts, the ICBM3D window will appear, and the ICBM3D
title will appear.  Press [Space] to begin the game at the selected
level, or use [L] and [K] to change the current level.

(NOTE: Key commands listed in this document represent the default keys.
See run "./icbm3d -help" and/or view "keydefs.h" to see what keys
your copy of the game is set up to use.)


PLAYING THE GAME
----------------

The game is played with the mouse and keyboard.  (Their specific
controls are described below.)

The main components of the screen are as follows:

  * Cross-hairs

    This is used to aim your defensive "Anti-ICBM" weapons.
    It is a white three-dimesional cross-hair ("+"-shape).
    There is a grey "shadow" of your cross-hair directly below it on
    the ground, as a reference.
    NOTE: Your cross-hair turns red when it's close to a missile.

  * Cities

    You start the game with 16 cities.  They are represented by blue
    clusters of buildings layed out evenly in a 4x4 pattern on the ground.
    (If you're in "Simple City" mode, your cities actually look like
    pyramids, not clusters of buildings.)
    (The ground just around your cities is a green square).

  * Defense Bases

    You also start the game with 4 defense bases.  This is where your
    AICBM Bullets are launched from.  They are represented by green
    buildings on the outer edge of your cities (on the corners).
    Each Defense Base starts with a limited number of bullets.
    Once the bullets run out, or if the Defense Base is destroyed,
    no more bullets can come from that position.  YES!  All four of your
    Defense Bases can be destroyed and/or run out of bullets, leaving you
    defenseless!  But don't worry, if you survive the level, all of your
    Bases will be rebuilt and rearmed.  "Empty" bases (without bullets)
    appear as a slightly different shape than "active" bases.

  * Missiles (ICBM's)

    "The Enemy" (tm) is attacking your cities by launching ICBM's
    (Inter-Continental Ballistic Missiles).  These appear high in the
    sky and drop down towards your cities.  If one hits a city, the city is
    destroyed.  (There is a white flash, the city disappears, and rubble
    is left in its place.)  Missiles leave a trail of smoke behind them.
    Although unrealistic, when a missile is destroyed or hits the ground,
    its smoke-trail disappears.  This is to make the game a little easier,
    and is also what happens in the original Missile Command.

    WIND: Wind can blow smoke around, making it harder to determine
    where a missile is actually going.  Be careful!

    SCORE: You get higher points the closer missiles are to the ground.

  * Planes

    "The Enemy" (tm) also sends bomber planes which fly over your cities,
    launch a missile, and then fly away.

    SCORE: Planes which aren't "making their escape" are worth 200 points.
    Planes which are (they have already dropped their missile) are worth 500.

  * AICBM "Bullets"

    Bullets are launched from your defense bases and fired toward the
    position that the cross-hair was at when you fired.  They explode
    when they reach their destination.  They are represented by purple
    "plasma" balls.  Small "X"'s mark where each bullet is to explode
    (ie, the position where your crosshair was when you fired).

  * Explosions

    While you may think explosions are not very useful to describe,
    they actually are very important.  Like the original "Missile Command"
    game, the explosions (which start out small, grow to a certain size,
    and then retract) are what actually destroys missiles.

    When you launch an AICBM bullet, if its explosion makes contact with
    a missile, that missile is destroyed in mid-air.  (This is of course
    the object of the game.)  The missile then explodes, which may in turn
    cause another incoming missile to be destroyed.  In "ICBM3D", explosions
    are represented with red diamond-like shapes.

  * Level

    The level you're currently on is displayed on the upper left corner.

  * Score

    Your score is displayed in the center of the top of the window.
    You gain score by destroying missiles with AICBM's.  The closer the
    ICBM is to the ground when you destroy it, the more points you get.
    If you pass a 10000-point mark during the current level, and don't
    have all 16 of your cities, you gain a new city.  This means that
    even if all of your cities are destroyed in the middle of a level,
    you may still "make a comeback" by destroying enough ICBM's or getting
    enough bonus points at the end of the level!

  * Anti-ICBM Bullet Counts

    As stated above, each of your Defense Bases has a limited number of
    AICBM bullets.  The number of bullets each base has appears in the
    center of the base.

  * Other Debris

    When cities or defense bases blow up, large "chunks" of them go flying
    and then disappear.  It's mean not only to look cool, and as extra
    visual feedback, but also to make the game harder. >:^)

    When cities explode, small red bits of debris fly up for a while from
    where the city was.  Note that, like with smoke, wind can affect the
    direction these bits go.


Controlling the cross-hair (and view)... with the mouse:

  ICBM3D has a relatively steep learning curve when it comes to getting
  used to the controls.  Don't be discouraged... it IS kind of difficult
  to control until you've played the game a few times!  Eventually, you'll
  be able to play without thinking about the controls.

  The following controls are available with the mouse:

               Left-Clicking       Middle-Clicking    Right-Clicking
  -----------+--------------------------------------------------------------
  Left/Right | Move Left/Right     Move Left/Right    Rotate View Left/Right
  Up/Down    | Move Up/Down        Move Away/Towards  Rotate View Up/Down

  So in other words, you click the left-button and drag left/right and
  up/down to move the cross-hair left/right and up/down...

  you click the middle-button and drag left/right and up/down to move
  the cross-hair left/right and away/towards...

  and you click the right-button and drag to change the viewpoint.


Controlling the cross-hair... with the keyboard:

  If you wish instead to use the keyboard to control the cross-hair,
  use the following keys:

  [Left]/[Right] - Move Left/Right
  [A]/[Z]        - Move Up/Down
  [Up]/[Down]    - Move Away/Towards


Firing AICBM Bullets:

  [Space] - When you want to launch an AICBM, you press the [Space] key.

            A bullet will be launched towards the cross-hair's position at
            the time you pressed space.  A small "X" will appear where
            you fired, reminding you where the bullet's destination is.

  When you're trying to aim for a missile, take into consideration the
  fact that the bullet takes a moment to get from the defense base, so
  you may need to aim just "ahead of" (below and in front of) the missile
  you wish to destroy.


Game-play Controls:

  There are also other commands available in ICBM3D:

  [P] - Pause and unpause the game.  ([Space] also unpauses.)
        When you pause, the word "PAUSED" will fly onto the screen
        (in the same way "LEVEL 1" does when you start out).

        Note that you can still affect certain things in the game
        (change your viewpoint for example), but all bullets, missiles
        and explosions, and your cross-hair are "frozen" until the game
        is unpaused.

  [X] - Abort.  This aborts your game, as if you lost the game.  (Game over.)

  [Q] - Quit.  This quits the program completely.  The window closes.

  [L] - Select Next Level.  The game must be over to select the level.
        (If you haven't died yet, use the [X]-Abort command.)
        The next level will be selected.  Press [Space] to start a new game
        at the selected level.

  [K] - Select Previous Level.  This is identical to [L], except it selects
        the previous level instead of the next one.


Other Viewing Controls:

  [I]   - Zoom In.

  [O]   - Zoom Out.

  [T]   - Targetting Lines On/Off.
          By default, a dotted line is drawn between each missile and
          the spot on the ground directly below it.  The purpose of this
          is to let you more easily determine whether your cross-hair is
          actually lined up with a missile (or if it just looks like it
          is, due to perspective and foreshortening).

          (Use your cross-hair's grey "shadow" on the ground to determine if
          you're at the right spot, then move up or down to get to the
          right altitude... and FIRE!)

          Unfortunately, having lots of dotted lines on the screen can
          become confusing.  To overcome this, you can turn the
          targetting lines off with the [T] key.

          Note, however, that targetting lines DO still appear for any
          missiles that are close to your crosshair.  This lets you
          retain the advantage of the targetting lines without cluttering
          your screen.  (Note: Your height and the missile's height are
          irrelavent in this determination of "closeness.")

  [F]   - Toggle Fast-Draw Mode.
          If you'd like the program to run a little faster, you can switch
          into "fast-draw" mode, where every other line that is supposed
          to be drawn in the window is not, every other frame.  This makes
          for some potentially annoying flickering, but the program is
          doing 1/2 as many 3D->2D calculations, and is drawing 1/2 as many
          lines in the window.  This may make the game more playable on
          some slower machines.

          Note: If you'd like this mode to be on by default whenever you
          start the game, set the value of the "#define" named
          "FASTDRAW_BY_DEFAULT" to "1" in the "icbm3d.c" source code file,
          and recompile with "make".  Set it to "0" if you don't want the
          game to start out in "fast-draw" mode...

  [H]   - Toggle Half-Frame Mode.
          Similar to Fast-Draw mode, this mode draws 1/2 as often.
          The frame-update-rate is cut in half (although the game is
          still going the same speed).

  [C]   - Toggle Simple/Complex Cities.
          This toggles the drawing of complex cities (the default), which
          are clusters of rectangular solids and pyramids, and simple
          cities (which are drawn much more quickly), which are single
          pyramids.  Simple mode also has less clutter on the screen.

  Many of the above commands can have their defaults set in "icbm3d.c".
  Look for "#define"'s which contain the text "_BY_DEFAULT".  Set the
  values to "0" to have them off, or "1" to have them on by default,
  and recompile.  You can of course still use the keyboard commands to
  toggle these options during a game.

  The standard viewing mode is "freeview" (similar to "freeview" mode in
  my "3D Pong" game for X-Window).  You can also select other viewing modes.
  I don't suggest using these much to play, as it's not as easy to
  see what's going on, but they are interesting and fun to look at!

  [M]   - Missile View.
          You are looking down at the city from just behind a missle.

  [Tab] - Select Missile.
          This selects which missile you're looking from.  It rotates
          through all active missiles.


END-OF-LEVEL BONUS
------------------

At the end of each level (after the wave of missiles has ended), you
gain bonus points for each of the following:

  * Bullets - One point each.
              Each Defense Base's bullet-count is counted down as
              your score increases.
              The Defense Base being tallied "pops" to a large size
              while the score is being totalled.

  * Cities -  100 points each.
              Each city that's not destroyed counts for 100 points at
              the end of the level.
              Each city "flattens" (a la the Atari 2600 Missile Command)
              when it's score is totalled.  All cities "pop" back into
              shape at the beginning of the next level.

As stated earlier, if you pass the 10,000-mark at the end of the level,
and don't have all 16 of your cities, you gain one extra city.
(For example, if you started the level with 5000 points and ended it
with 11000 points, you get a city.)

Cities are randomly placed and appear when the next level begins.


GAME VARIATIONS
---------------

Different levels of ICBM3D present different variations of the game.

* There are two variations on missiles (meaning there are a total of four
  types of missiles):

  * Splitters

    Some missiles split into three at a certain altitude.  This means
    that it's extra-important to destroy them quickly before they become
    three times as dangerous.

  * Spirallers

    Spirallers don't travel in straight lines.  As the name suggests,
    they spin around as they move downwards, which makes aiming much
    harder.

  Splitters occur more and more often throughout the game.
  Spirallers occur more often on even-numbered levels.

* Each level of the game has more missiles to destroy.

* Missiles go faster on higher levels.

* Wind is introduced in levels 50 and beyond.  This blows the missiles'
  smoke (and destroyed cities' debris) around randomly, confusing you!


CREDITS
-------

ICBM3D was written by Bill Kendrick,
(c) copyright "New Breed Software," March 1998 - July 1998.

ICBM3D was based on Missile Command, by Dave Theurer,
(c) copyright Atari, 1981.

Ideas for and comments about ICBM3D were obtained from many folks,
all listed at the top of "icbm3d.c".
