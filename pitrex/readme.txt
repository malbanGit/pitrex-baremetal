(see also: http://vide.malban.de/pitrex/pitrex-baremetal-knowledge
 and other pages "around" that one)


PiTrex – Baremetal knowledge

For a short write up on the boot process of the baremetal installation, please look at: Software description (bottom of the page)
(in order of “boot” order)
pitrex/software/pitrex/baremetal

This directory contains all files necessary to “boot up” baremetal piTrex.
There is no Makefile within this directory – because there is no program built here. These are a collection of support files, which are linked with the “C” file that should be executed as a baremetal image. Apart from the files described below, also some additional files are present (mostly redundant baremetal definition and access files for ARM IO definitions) – which I never really sorted into something sensible – because NEVER CHANGE A RUNNING SYSTEM!
(alltogether I use three different “sets” of accesses to the ARM:
a) lib-rpidmx512
b) some “left over” files from my first steps in “baremetal”
c) bcm2835 “lib” which Kevin uses to realize his low level accesses
One of these days I have to unify this into ONE usable access method… but that is in the future)

The (most) relevant files are:

    baremetalEntry.S
    As the name suggests, this is the first entry the “typical baremetal” user gets. Here we:
    – save our start registers,
    – configure our vectors (reset and exception handlers)
    – set the stack space
    – clear the later “bss” space for “C”
    – setup the coprocessor/and the MMU/ access to the cycle count register
    … and finaly jump to “kernalMain” our first “C” modul
    bareMetalMain.c
    – here we set our “clock” speed
    – initialize the UART interface
    – … continue to jump to our “main” of the to be executed “C” program
    cstubs.c
    diverse “c” stubs needed for some later included libraries to function.
    Most notable:
    – outbyte( char b )
    This is defined to output data to the UART interface. This enables as
    (amongst other things), to “printf(…)” to the UART.
    “libbcm2835”
    Diverse files from that library are linked to the package… e.g. the FAT file system stuff. Attention there were some tiny tweaks to this library I had to make, in order to fit my own memory layout! (So don’t just download a new version and overwrite this one!)

pitrex/software/pitrex/loader

This directory contains the programs first loaded by the pitrex, the above described stuff is linked to these.

pitrex.img

This is the “image” file the pitrex starts up with. It contains above boot files and a “short” main program. The only “C” program started is defined in:

    bootStrapMain.c
    This does only two things:
    1) Mount the first FAT partition of the SD
    2) load the file “loader.pit” (to a location given in the Makefile)
    … and start the program “loader.pit”

loader.pit

This is the first thing you can hear/see when starting up PiTrex baremetal. This things is the first “menu”. Apart from “libraries” and “objects” from other directories – this is build from two files:

    loaderEntry.S
    This is my way around adding an additional linker script. This file is the “start” of the loader. It is loaded to the correct address, clears the bss area and jumps to the “loaderMain” function
    loaderMain.c
    This is actually the first thing you “may” change and program.
    This file contains the menu system from which all other programs are loaded.
    It is linked with the above described “ARM” libs and the complete “vectrexInterface” (programmed by me) and the “pitrex IO” programmed by Kevin. So now and here you are able to draw vectors on the vectrex.

    Since the menu may change… it I will not describe it here further… With one notable exception:
    void loadAndStart(TCHAR FILE_NAME[], char *parameter)
    That function loads a “kernel” image file, passes parameters and starts it. More on that below!
    (After loading/starting the image, the loader stays in memory – and can be called again!)

pitrex/software/pitrex/pitrex

This directory contains the low level GPIO access routines Kevin provided. The files will be compiled to “objects” and linked with other programs. mainly two files are relevant:

    bcm2835.c
    This is a “library” (well, this one file + header) to access GPIO of the Pi.
    pitrex-gpio.c
    This contains Kevins low level access routines (which in turn use the above bcm2835 functions) to execute and time accesses to the Vectrex (VIA). These functions are highly timing critical, as they must synchronize exactily with the frequency the VIA chip inside the vectrex is running with. The functions have to watch the BUS and react on “ENABLE”, “READY” lines and such.
    From a low level user point of view more or less only “two functionalities” are important/usable:
    – SET(viaRegister, value);
    – GET(viaRegister);

pitrex/software/vectrex

This directory contains “my” vectrex interface. It uses Kevin’s low level routines to access the via, and to get the via timing correctly done. The whole package adds “abstraction” layers so the user/programmer has access to easy to use functions.
(e.g. drawVector(), printString() …)
Additionaly some further functionality was added to the “function collection” that might make daily life easier. Following file clusters are available:

    vectrexInterface
    This function collection provides the basic vectrex interface. For setting brightness, drawing vectors, adding sound… etc. For more description of this look at the files, they are somewhat documented.
    Also the PiTrex Wiki contains some paragraphs I wrote.
    osWrapper
    Contains some functions, that make it easier porting existing software to PiTrex, mainly file handling.
    baremetalUtil
    Contains some functionality for working with baremetal, enable/disable caches, cycle exact timing/counters, memcpy, string functions etc
    mathSupport
    Start of a collection to do vector rotation (only used with the PiTrex start screen)

A baremetal “program”

Let’s call the thing you loaded with the above described “loader.pit” a baremetal program. What is that?

These “programs” are bootable image files. If you replace the pitrex.img, with any of the created “program” image files… the pitrex will boot DIRECTLY into the program.

They are inherently functional complete baremetal programs.

These thought behind this was to:

    make it easily possible to have SD cards with specific games (if you want to)
    make it easy to boot into different images
    make it possible to even boot (select from the loader) raspbian OS 

As such… each of those created images contain alls stuff mentioned, each baremetal program contains:

    the code from: pitrex/software/pitrex/*.*
    plus its own code!

Most of that that is hidden in the Makefiles. If YOU want to write a baremetal program, you probably just copy one of the directories that already contains a Makefile, and delete all other files. Than you can create your own first “c” file (which you should add to the Makefile).

A simple hello world program than looks like:

If you add that to you Makefile … a “hello world” image file will be created and you can start a baremetal SD image with that.

Now… isn’t that COOL???
