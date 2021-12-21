
REBEL DEFENDER - README

1) Compiling from source files

After having unzipped "rdefen11.zip" (the one you are now in), enter this directory from the console and type "make" if you do not want to use the already compiled binary. Then the binary "rdefender" will be re-built.


2) Installing the already compiled game

In this same directory, type as root: "rdinstall".
This will install the files as follows:

- "snd1.wav" to "snd3.wav" in "/usr/share/games/defender/"

- "rdefender" in "/usr/games/"

You can proceed manually as you prefer.


3) Running the game

Type "rdefender" from the console or launch from a shortcut via your desktop.
If it fails because the sound card cannot be accessed, then try:
padsp rdefender

Hope you will enjoy !

--------------------------------------------------------------------------------------------------


Note: If you find that the game is too fast and difficult, you can do the following modifications:

In the rdefender.c source file,

- Look for this code part:

		/* Enemy regeneration */
		if (recoverdelay < 100)
			{
			recoverdelay += 1;
			}
		else
			{
			if (ntie < (gamelevel+4))
				{
				/* Create a new tie-fighter */
....

- Then replace "(ntie < (gamelevel+4))" by "(ntie < (gamelevel+3))"

- Finally, look for this code part:

		/* Delay instead of sound handling */
		while (Timer_msec(gt) < reftim) {}
	}
	reftim = Timer_msec(gt) + 12;
....

- Then replace 12 by 16.

- Save, rebuild (via "make") and replace "rdefender" by the new one.

Have more fun !


V.PORTET, Paris, FR.
vincentportet_95@hotmail.com
Version 1.2 of the 26th of June 2015
