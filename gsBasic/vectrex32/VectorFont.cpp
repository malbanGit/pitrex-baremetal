
#include <stdint.h>
#include "VectorFont.h"

#define MoveTo 0
#define DrawTo 1

// Define the ENTRY(characters. We define them within a rectangle that goes from
// {0, 0} (the bottom left corner) to {4, 6} (the top right).
//
// After defining the ENTRY(characters, you must add them to the 'table' variable,
// at the end of this function, in ASCII order.
//
// You may assume for each ENTRY(character that the pen is at (0, 0) when drawing starts.
static struct CharacterLine charSpace[] = {{MoveTo, 4, 0}};
static struct CharacterLine charExcl[] = {{MoveTo, 2, 0}, {DrawTo, 2, 0}, {MoveTo, 2, 2}, {DrawTo, 2, 6}};
static struct CharacterLine charQuote[] = {{MoveTo, 1, 6}, {DrawTo, 1, 4}, {MoveTo, 3, 6}, {DrawTo, 3, 4}};
static struct CharacterLine charHash[] = {{MoveTo, 1, 6}, {DrawTo, 1, 0}, {MoveTo, 3, 6}, {DrawTo, 3, 0}, 
            {MoveTo, 4, 3}, {DrawTo, 0, 3}, {MoveTo, 4, 1}, {DrawTo, 0, 1}};
static struct CharacterLine charDollar[] = {{MoveTo, 0, 1}, {DrawTo, 4, 1}, {DrawTo, 4, 3}, {DrawTo, 0, 3}, {DrawTo, 0, 5}, {DrawTo, 4, 5}, 
              {MoveTo, 2, 6}, {DrawTo, 2, 0}};
static struct CharacterLine charPercent[] = {{DrawTo, 4, 6}, {MoveTo, 4, 1}, {DrawTo, 4, 0}, {MoveTo, 0, 5}, {DrawTo, 0, 6}};
static struct CharacterLine charAmp[] = {{MoveTo, 4, 0}, {DrawTo, 1, 4}, {DrawTo, 1, 6}, {DrawTo, 3, 6}, {DrawTo, 3, 4}, 
           {DrawTo, 0, 2}, {DrawTo, 0, 0}, {DrawTo, 3, 0}, {DrawTo, 4, 2}};
static struct CharacterLine charTick[] = {{MoveTo, 2, 4}, {DrawTo, 2, 6}};
static struct CharacterLine charLParen[] = {{MoveTo, 3, 0}, {DrawTo, 2, 1}, {DrawTo, 2, 5}, {DrawTo, 3, 6}};
static struct CharacterLine charRParen[] = {{MoveTo, 1, 0}, {DrawTo, 2, 1}, {DrawTo, 2, 5}, {DrawTo, 1, 6}};
static struct CharacterLine charAster[] = {{MoveTo, 0, 1}, {DrawTo, 4, 5}, {MoveTo, 2, 1}, {DrawTo, 2, 5}, {MoveTo, 4, 1}, {DrawTo, 0, 5}};
static struct CharacterLine charPlus[] = {{MoveTo, 0, 3}, {DrawTo, 4, 3}, {MoveTo, 2, 5}, {DrawTo, 2, 1}};
static struct CharacterLine charComma[] = {{MoveTo, 3, 2}, {DrawTo, 3, 1}, {DrawTo, 2, 0}};
static struct CharacterLine charDash[] = {{MoveTo, 1, 3}, {DrawTo, 3, 3}};
static struct CharacterLine charPeriod[] = {{MoveTo, 2, 0}, {DrawTo, 2, 0}};
static struct CharacterLine charSlash[] = {{DrawTo, 4, 6}};
static struct CharacterLine char0[] = {{MoveTo, 0, 0}, {DrawTo, 0, 6}, {DrawTo, 4, 6}, {DrawTo, 4, 0}, {DrawTo, 0, 0}};
static struct CharacterLine char1[] = {{MoveTo, 2, 0}, {DrawTo, 2, 6}, {DrawTo, 1, 5}};
static struct CharacterLine char2[] = {{MoveTo, 0, 6}, {DrawTo, 4, 6}, {DrawTo, 4, 3}, {DrawTo, 0, 3}, {DrawTo, 0, 0}, {DrawTo, 4, 0}};
static struct CharacterLine char3[] = {{DrawTo, 4, 0}, {DrawTo, 4, 6}, {DrawTo, 0, 6}, {MoveTo, 1, 3}, {DrawTo, 4, 3}};
static struct CharacterLine char4[] = {{MoveTo, 0, 6}, {DrawTo, 0, 3}, {DrawTo, 4, 3}, {MoveTo, 4, 6}, {DrawTo, 4, 0}};
static struct CharacterLine char5[] = {{DrawTo, 4, 0}, {DrawTo, 4, 3}, {DrawTo, 0, 3}, {DrawTo, 0, 6}, {DrawTo, 4, 6}};
static struct CharacterLine char6[] = {{MoveTo, 0, 6}, {DrawTo, 0, 0}, {DrawTo, 4, 0}, {DrawTo, 4, 3}, {DrawTo, 0, 3}};
static struct CharacterLine char7[] = {{DrawTo, 4, 6}, {DrawTo, 0, 6}};
static struct CharacterLine char8[] = {{DrawTo, 0, 6}, {DrawTo, 4, 6}, {DrawTo, 4, 0}, {DrawTo, 0, 0}, {MoveTo, 0, 3}, {DrawTo, 4, 3}};
static struct CharacterLine char9[] = {{MoveTo, 4, 0}, {DrawTo, 4, 6}, {DrawTo, 0, 6}, {DrawTo, 0, 3}, {DrawTo, 4, 3}};
static struct CharacterLine charColon[] = {{MoveTo, 2, 1}, {DrawTo, 2, 1}, {MoveTo, 2, 5}, {DrawTo, 2, 5}};
static struct CharacterLine charSemi[] = {{MoveTo, 3, 5}, {DrawTo, 3, 5}, {MoveTo, 3, 2}, {DrawTo, 3, 1}, {DrawTo, 2, 0}};
static struct CharacterLine charLT[] = {{MoveTo, 3, 1}, {DrawTo, 1, 3}, {DrawTo, 3, 5}};
static struct CharacterLine charEQ[] = {{MoveTo, 0, 2}, {DrawTo, 4, 2}, {MoveTo, 0, 4}, {DrawTo, 4, 4}};
static struct CharacterLine charGT[] = {{MoveTo, 1, 1}, {DrawTo, 3, 3}, {DrawTo, 1, 5}};
static struct CharacterLine charQues[] = {{MoveTo, 2, 0}, {DrawTo, 2, 0}, {MoveTo, 2, 2}, {DrawTo, 2, 3}, {DrawTo, 4, 3}, {DrawTo, 4, 6}, {DrawTo, 0, 6}};
static struct CharacterLine charAt[] = {{MoveTo, 4, 0}, {DrawTo, 0, 0}, {DrawTo, 0, 6}, {DrawTo, 4, 6}, {DrawTo, 4, 1}, 
          {DrawTo, 2, 1}, {DrawTo, 2, 5}, {DrawTo, 3, 5}, {DrawTo, 3, 3}, {DrawTo, 2, 3}};
static struct CharacterLine charA[] = {{DrawTo, 2, 6}, {DrawTo, 4, 0}, {MoveTo, 1, 3}, {DrawTo, 3, 3}};
static struct CharacterLine charB[] = {{DrawTo, 0, 6}, {DrawTo, 3, 6}, {DrawTo, 4, 5}, {DrawTo, 4, 4}, {DrawTo, 3, 3}, {DrawTo, 0, 3}, 
         {MoveTo, 3, 3}, {DrawTo, 4, 2}, {DrawTo, 4, 1}, {DrawTo, 3, 0}, {DrawTo, 0, 0}};
static struct CharacterLine charC[] = {{MoveTo, 4, 6}, {DrawTo, 0, 6}, {DrawTo, 0, 0}, {DrawTo, 4, 0}};
static struct CharacterLine charD[] = {{DrawTo, 0, 6}, {DrawTo, 3, 6}, {DrawTo, 4, 5}, {DrawTo, 4, 1}, {DrawTo, 3, 0}, {DrawTo, 0, 0}};
static struct CharacterLine charE[] = {{MoveTo, 4, 6}, {DrawTo, 0, 6}, {DrawTo, 0, 0}, {DrawTo, 4, 0}, {MoveTo, 0, 3}, {DrawTo, 3, 3}};
static struct CharacterLine charF[] = {{MoveTo, 4, 6}, {DrawTo, 0, 6}, {DrawTo, 0, 0}, {MoveTo, 0, 3}, {DrawTo, 3, 3}};
static struct CharacterLine charG[] = {{MoveTo, 4, 6}, {DrawTo, 0, 6}, {DrawTo, 0, 0}, {DrawTo, 4, 0}, {DrawTo, 4, 3}, {DrawTo, 3, 3}};
static struct CharacterLine charH[] = {{DrawTo, 0, 6}, {MoveTo, 4, 6}, {DrawTo, 4, 0}, {MoveTo, 0, 3}, {DrawTo, 4, 3}};
static struct CharacterLine charI[] = {{DrawTo, 4, 0}, {MoveTo, 2, 0}, {DrawTo, 2, 6}, {MoveTo, 0, 6}, {DrawTo, 4, 6}};
static struct CharacterLine charJ[] = {{MoveTo, 0, 1}, {DrawTo, 0, 0}, {DrawTo, 4, 0}, {DrawTo, 4, 6}};
static struct CharacterLine charK[] = {{DrawTo, 0, 6}, {MoveTo, 4, 6}, {DrawTo, 0, 3}, {DrawTo, 4, 0}};
static struct CharacterLine charL[] = {{MoveTo, 0, 6}, {DrawTo, 0, 0}, {DrawTo, 4, 0}};
static struct CharacterLine charM[] = {{DrawTo, 0, 6}, {DrawTo, 2, 3}, {DrawTo, 4, 6}, {DrawTo, 4, 0}};
static struct CharacterLine charN[] = {{DrawTo, 0, 6}, {DrawTo, 4, 0}, {DrawTo, 4, 6}};
static struct CharacterLine charO[] = {{DrawTo, 0, 6}, {DrawTo, 4, 6}, {DrawTo, 4, 0}, {DrawTo, 0, 0}};
static struct CharacterLine charP[] = {{DrawTo, 0, 6}, {DrawTo, 4, 6}, {DrawTo, 4, 3}, {DrawTo, 0, 3}};
static struct CharacterLine charQ[] = {{DrawTo, 0, 6}, {DrawTo, 4, 6}, {DrawTo, 4, 0}, {DrawTo, 0, 0}, {MoveTo, 2, 3}, {DrawTo, 4, 0}};
static struct CharacterLine charR[] = {{DrawTo, 0, 6}, {DrawTo, 4, 6}, {DrawTo, 4, 3}, {DrawTo, 0, 3}, {DrawTo, 4, 0}};
static struct CharacterLine charS[] = {{DrawTo, 4, 0}, {DrawTo, 4, 3}, {DrawTo, 0, 3}, {DrawTo, 0, 6}, {DrawTo, 4, 6}};
static struct CharacterLine charT[] = {{MoveTo, 2, 0}, {DrawTo, 2, 6}, {DrawTo, 0, 6}, {DrawTo, 4, 6}};
static struct CharacterLine charU[] = {{MoveTo, 0, 6}, {DrawTo, 0, 0}, {DrawTo, 4, 0}, {DrawTo, 4, 6}};
static struct CharacterLine charV[] = {{MoveTo, 0, 6}, {DrawTo, 2, 0}, {DrawTo, 4, 6}};
static struct CharacterLine charW[] = {{MoveTo, 0, 6}, {DrawTo, 0, 0}, {DrawTo, 2, 3}, {DrawTo, 4, 0}, {DrawTo, 4, 6}};
static struct CharacterLine charX[] = {{DrawTo, 4, 6}, {MoveTo, 0, 6}, {DrawTo, 4, 0}};
static struct CharacterLine charY[] = {{MoveTo, 0, 6}, {DrawTo, 2, 3}, {MoveTo, 4, 6}, {DrawTo, 2, 3}, {DrawTo, 2, 0}};
static struct CharacterLine charZ[] = {{MoveTo, 0, 6}, {DrawTo, 4, 6}, {DrawTo, 0, 0}, {DrawTo, 4, 0}};
static struct CharacterLine charLBrack[] = {{MoveTo, 3, 0}, {DrawTo, 1, 0}, {DrawTo, 1, 6}, {DrawTo, 3, 6}};
static struct CharacterLine charBackSl[] = {{MoveTo, 0, 6}, {DrawTo, 4, 0}};
static struct CharacterLine charRBrack[] = {{DrawTo, 3, 0}, {DrawTo, 3, 6}, {DrawTo, 1, 6}};
static struct CharacterLine charExp[] = {{MoveTo, 0, 3}, {DrawTo, 2, 6}, {DrawTo, 4, 3}};
static struct CharacterLine charUnder[] = {{DrawTo, 4, 0}};
static struct CharacterLine charGrave[] = {{MoveTo, 1, 6}, {DrawTo, 3, 4}};
static struct CharacterLine charUp[] = {{MoveTo, 2, 0}, {DrawTo, 2, 6}, {MoveTo, 0, 3}, {DrawTo, 2, 6}, {DrawTo, 4, 3}};
static struct CharacterLine charDown[] = {{MoveTo, 0, 3}, {DrawTo, 2, 0}, {DrawTo, 4, 3}, {MoveTo, 2, 6}, {DrawTo, 2, 0}};

#define ENTRY(x) {x, sizeof(x) / sizeof((x)[0])}

static struct CharacterDefinition characterDefinitions[] = {
        ENTRY(charSpace), 
        ENTRY(charExcl), 
        ENTRY(charQuote), 
        ENTRY(charHash), 
        ENTRY(charDollar), 
        ENTRY(charPercent), 
        ENTRY(charAmp), 
        ENTRY(charTick), 
        ENTRY(charLParen), 
        ENTRY(charRParen), 
        ENTRY(charAster), 
        ENTRY(charPlus), 
        ENTRY(charComma), 
        ENTRY(charDash), 
        ENTRY(charPeriod), 
        ENTRY(charSlash), 
        ENTRY(char0), 
        ENTRY(char1), 
        ENTRY(char2), 
        ENTRY(char3), 
        ENTRY(char4), 
        ENTRY(char5), 
        ENTRY(char6), 
        ENTRY(char7), 
        ENTRY(char8), 
        ENTRY(char9), 
        ENTRY(charColon), 
        ENTRY(charSemi), 
        ENTRY(charLT), 
        ENTRY(charEQ), 
        ENTRY(charGT), 
        ENTRY(charQues), 
        ENTRY(charAt), 
        ENTRY(charA), 
        ENTRY(charB), 
        ENTRY(charC), 
        ENTRY(charD), 
        ENTRY(charE), 
        ENTRY(charF), 
        ENTRY(charG), 
        ENTRY(charH), 
        ENTRY(charI), 
        ENTRY(charJ), 
        ENTRY(charK), 
        ENTRY(charL), 
        ENTRY(charM), 
        ENTRY(charN), 
        ENTRY(charO), 
        ENTRY(charP), 
        ENTRY(charQ), 
        ENTRY(charR), 
        ENTRY(charS), 
        ENTRY(charT), 
        ENTRY(charU), 
        ENTRY(charV), 
        ENTRY(charW), 
        ENTRY(charX), 
        ENTRY(charY), 
        ENTRY(charZ), 
        ENTRY(charLBrack), 
        ENTRY(charBackSl), 
        ENTRY(charRBrack), 
        ENTRY(charExp), 
        ENTRY(charUnder), 
        ENTRY(charGrave), 
        ENTRY(charUp), 
        ENTRY(charQues), 
        ENTRY(charDown)
    };
    
// Put all the ENTRY(characters into a table spanning from ASCII space to ~
struct VectorFont vectorFontTable = {
    ' ', 'c',
    4, 6, // Character width and height
    7, 11, 7, // advance, line height, ascent
    characterDefinitions
    };

Value TextToLines(const char *str)
{
    // Figure out how big an array we'll need
    const char *p = str;
    size_t lineCount = 0;
    
    // Count the number of lines we'll generate
    while (*p)
    {
        if (*p >= vectorFontTable.firstChar && *p <= vectorFontTable.lastChar)
        {
            const CharacterDefinition &charDef = vectorFontTable.characterDefinitions[*p - vectorFontTable.firstChar];
            lineCount += charDef.linesCount;
            // If the first action in a character is a DrawTo, we'll need to add a MoveTo
            if (charDef.lines[0].mode == DrawTo)
                ++lineCount;
        }
        
        ++p;
    }
    
    uint16_t dims[] = {uint16_t(lineCount), 3};
    Value lines(dims, 2);
    size_t lineIndex = 0;
    int charPosX = 0, charPosY = 0;
    p = str;
    // Generate the lines
    while (*p)
    {
        if (*p >= vectorFontTable.firstChar && *p <= vectorFontTable.lastChar)
        {
            const CharacterDefinition &charDef = vectorFontTable.characterDefinitions[*p - vectorFontTable.firstChar];
            // If the first action in a character is a DrawTo, we need to add a MoveTo
            if (charDef.lines[0].mode == DrawTo)
            {
                lines[lineIndex++] = MoveTo;
                lines[lineIndex++] = charPosX * FONT_MAGNIFICATION;
                lines[lineIndex++] = charPosY * FONT_MAGNIFICATION;
            }
            
            for (int i = 0; i < charDef.linesCount; ++i)
            {
                lines[lineIndex++] = charDef.lines[i].mode;
                lines[lineIndex++] = (charDef.lines[i].x + charPosX) * FONT_MAGNIFICATION;
                lines[lineIndex++] = (charDef.lines[i].y + charPosY) * FONT_MAGNIFICATION;
            }
                
            charPosX += vectorFontTable.advance;
        }
        
        else if (*p == '\r')
            charPosX = 0;
        
        else if (*p == '\n')
            charPosY -= vectorFontTable.lineHeight;
        
        ++p;
    }
    
    return lines;
}
