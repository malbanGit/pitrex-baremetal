
********************************************************

All files in this directory and subdirectories are
Copyright (c) 2021 Robert E. Alexander

Permission to use this software is granted under the 
MIT licence (see licence.txt).

********************************************************





PiTrex version of GS BASIC
##########################

Original Vectrex32 page: https://vectrex32.com/
Thank you Bob, for allowing me to port GS BASIC to PiTrex!

The PiTrex GS BASIC is based on Version 1.27 of GS BASIC for the Vectrex32!
Documentation for GS BASIC can be accessed at above mentioned page.

!!!!!!!!!!!!!!
The PiTrex version, although ported with Bobs blessings, is a seperate project!
Please don't perster Bob with support requests!

For PiTrex programming and questions/answers you might visit:
https://groups.io/g/pitrex-dev
!!!!!!!!!!!!!!


General:
The adaption of GS BASIC running on a PiTrex is using the "vectrexInterface" written by Malban.
The interface provides several functions and functionalities, with which programs running on a PiTrex can
access the Vectrex VIA.

For now GS BASIC runs on the baremetal version of PiTrex only.

The latest version (May 2021) of the interface implements an access mode called "IRQ Mode".
In general this means:
a) all accesses to the VIA within the interface are made during interrupts
b) the "application" that runs on the piTrex runs in the non IRQ environment (this is e.g. GSBASIC)

On an easy view you can say that the i/o to the vectrex is disjunct from the application running. 
For all user concerned relations you may think of the vectrexInterface as running in a different thread in a 
multitasking environment.
This is very much like the original GS BASIC on the Vectrex32 PIC cartridge.

There is "literaly" one big difference between the Vectrex32 and the PiTrex.
The PiTrex has 1Gb RAM.
As of now I allow user programs to be a size of about 60MB (one has to have some memory in reserve :-) ).
That also means, that BASIC programs could be of ENORMOUS size!


CHANGE IN MEANING OF: 
---------------------
SetFrameRate()
Per default the vectrexInterface always tries to display one frame each 1/50 of a second (50Hz).
From BASIC commands you cannot change that. 
The output might be slower than 50Hz, if you put to much on screen, but that is the only way
for a BASIC program to change the "visual" frame rate.

The BASIC command "SetFrameRate" sets (with the PiTrex version) the speed at which the BASIC program runs.
If you set SetFrameRate(30) for example, than the WaitForFrame() function will wait till 1/30 of a second
is passed before continuing. But the actual display Hz is not influences by that command!

It is very much possible for your BASIC program to internally "do a game round" in 10Hz, but remain at a 
steady display of 50Hz!

SOUND
-----
!The "old" sound commands:!
- MUSIC
- SOUND 
- EXPLOSION
Use the SetFrameRate() speed (or in case of Sound - how often you call it!). 
Thus if you set your framerate to 10Hz, the function Play(Music/Explosion) - will play a 10Hz version of the music/explosion!

!The "new" sound commands (see below):!
Do their output not from the BASIC application, but are called internally from the vectrexInterface, and are
thus called with the speed of the vectrex interface - hopefully 50Hz.

New BASIC commands
##################

Samples
-------
s = Sample(filename, rate)
**************************

Loads a sample file from disk, the sample rate determines the playback rate.
The rate is not "exact" and you have to use a bit of trial and error with your own program.
It also depends on the things displayed on screen. Much text e.g. will result in "raspier" playback.

The samples are RAW 8bit signed. "Easiest" way to create them is to use Vide!

PlaySample(s)
*************

Plays a sample.
PlaySample(nil) - stops current playback.
Samples can be played independendly of other sounds.
On so called "noBuzz" Vectrex - samples are barely audible! 

YM-Files
--------
ym = YMMusic(filename)
**********************

Loads a ymfile file from disk, the ym file must be "unpacked".
Nearly all ym files you find in the depth of the internet are packed. Use a Lharc/Lzh unpacker to
extract the original file. YM formats up to YM6 should be loadable.

To play ym files, use the normal "Play()" command. "MusicIsPlaying()" is also working correctly.
To stop a currently playing YM use "Play(nil)".


AYSFX
-----
These are little pieces of SFX sounds. They can be played "on top" of the other sounds 
(they capture their channel, when being played).

For more about the AYSFX format see: http://vectrex.malban.de/preliminary/ca8cc2e6.html (and the linked location of Shiru).

The downloadable AYSFX "creator" zip file contains dozens of examples you can use out of the box. 
Easiest way to convert them is Vide (export asm source and use the created data).

sfx = AYSFX(Array)
******************
To create a sfx. 
Example:

data1 = {  $ED, $E2, $00, $1E, $AE, $AA, $00, $AD, $E2, $00, _
  $AE, $AA, $00, $AF, $8F, $00, $AF, $86, $00, $AF, _
  $8F, $00, $AF, $86, $00, $AF, $8F, $00, $AF, $86, _
  $00, $AF, $8F, $00, $AE, $86, $00, $AE, $8F, $00, _
  $AD, $86, $00, $AD, $8F, $00, $AE, $86, $00, $AE, _
  $8F, $00, $AF, $71, $00, $AF, $55, $00, $AF, $47, _
  $00, $AF, $42, $00, $AF, $47, $00, $AF, $42, $00, _
  $AE, $47, $00, $AE, $42, $00, $AE, $47, $00, $AD, _
  $42, $00, $AD, $47, $00, $AD, $42, $00, $AC, $47, _
  $00, $AC, $42, $00, $AC, $47, $00, $AC, $42, $00, _
  $AB, $47, $00, $AB, $42, $00, $D0, $20}

shoot = AYSFX(data1)

(usually AYSFX are smaller than the above)

PlayAYSFX(sfx, channel)
***********************
Since the AYSFX format is channel independend you can chose for each playing the channel you wish to use.
channel 1, 2 or 3!

You can (as of now) not stop a sfx - usually they are very short!


-----

DEPRICATED WITH PITREX
######################
Following functions do not make sense in PiTrex environment – and are thus not implemented. 
(they are callable, but return “0” and give out a one time message, that they are not supported):

 static ErrorCode Upgrade()
 static ErrorCode Downgrade()
 static ErrorCode PatchCode()
 static ErrorCode Poke()
 static ErrorCode PokeW()
 static ErrorCode JSR()
 static ErrorCode CodeSprite()
 static ErrorCode GetFlashChecksum()
 static ErrorCode GetCompiledSpriteCount()
 static ErrorCode DumpMemory()
 static ErrorCode DumpDisk()
 static ErrorCode DumpDPRAM()
 static ErrorCode DumpDPRAMMirror()
 static ErrorCode DumpMem()
 static ErrorCode DumpFlashErrors()

 static ErrorCode Peek()

The last function (Peek()) can be used to determine whether the BASIC is running on a PiTrex. 
If you peek at an address that you KNOW is non zero the peek() returns 0 nonetheless – than the BASIC is running on a PiTrex :-).


