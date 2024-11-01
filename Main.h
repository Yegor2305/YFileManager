#pragma once
#include <iostream>
#include <windows.h>
#include <stdio.h>

struct LineInfo {
    union {
        WCHAR UnicodeChar;
        CHAR   AsciiChar;
    } FirstChar;
    union {
        WCHAR UnicodeChar;
        CHAR   AsciiChar;
    } MediumChar;
    union {
        WCHAR UnicodeChar;
        CHAR   AsciiChar;
    } LastChar;
    WORD Attributes;
};

struct LabelInfo
{
    LabelInfo(unsigned short x_pos, unsigned short y_pos,
        unsigned short screen_width, WORD attributes) :X_Pos(x_pos), Y_Pos(y_pos), Screen_Width(screen_width), Attributes(attributes) {}

    unsigned short X_Pos;
    unsigned short Y_Pos;
    unsigned short Screen_Width;
    WORD Attributes;
};

struct OutputPos {
    OutputPos(unsigned short x_pos, unsigned short y_pos,
        unsigned short screen_width, unsigned short length) :X_Pos(x_pos), Y_Pos(y_pos), Screen_Width(screen_width), Length(length) {}

    unsigned short X_Pos;
    unsigned short Y_Pos;
    unsigned short Screen_Width;
    unsigned short Length;
};

extern "C" void DrawLineHorizontal(CHAR_INFO* screen_buffer, OutputPos pos, LineInfo line_info);
extern "C" void DrawLineVertical(CHAR_INFO* screen_buffer, OutputPos pos, LineInfo line_info);
extern "C" void DrawLabel(CHAR_INFO* screen_buffer, LabelInfo label_info, const char* text);
extern "C" void PrintColorPalette(CHAR_INFO* screen_buffer, OutputPos pos, CHAR_INFO symbol_info);