#ifndef GAMEART_H
#define GAMEART_H

#include "globals.h"

#define ART_START(name)  static const struct artworks name[] = {
#define ART_LOAD(zipfile, filename, type, target) { zipfile, filename, type, target },
#define ART_END {NULL, NULL, 0}};
ART_START(omegarace_art)
 ART_LOAD("omegrace.zip", "omegbkdp3.png",ART_TEX, 0)
 ART_LOAD("omegrace.zip", "omegrace_overlay.png", ART_TEX, 1)
 ART_LOAD("omegrace.zip", "omegbezlcroped.png", ART_TEX, 3)
ART_END

//
//Artwork Setting.
//Backdrop : layer 0
//Overlay : layer 1
// Bezel Mask : layer 2
// Bezel : layer 3
//Screen burn layer 4: Yea I know, but...
ART_START(noart)
ART_END

#endif