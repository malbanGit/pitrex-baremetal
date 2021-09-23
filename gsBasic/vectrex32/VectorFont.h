/* 
 * File:   VectorFont.h
 * Author: Bob
 *
 * Created on December 3, 2016, 12:40 PM
 */

#ifndef VECTORFONT_H
#define	VECTORFONT_H

#include "../Value.h"

// The characters as defined are really small. If we multiply by 6 and
// draw at a scale of 15 (which the Text2 sprites do) the text is comparable
// to a Vectrex text size of 40x5
#define FONT_MAGNIFICATION 6
    
struct CharacterLine
{
    uint8_t mode, x, y;
};

struct CharacterDefinition
{
    struct CharacterLine *lines;
    int linesCount;
};

struct VectorFont
{
    char firstChar, lastChar;
    uint8_t charWidth, charHeight;
    uint8_t advance, lineHeight, ascent;
    struct CharacterDefinition *characterDefinitions;
};

extern struct VectorFont vectorFontTable;

extern Value TextToLines(const char *str);

#endif	/* VECTORFONT_H */

