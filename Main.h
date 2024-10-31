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

struct OutputPos {
    OutputPos(unsigned short x_pos, unsigned short y_pos,
        unsigned short screen_width, unsigned short lenght) :X_Pos(x_pos), Y_Pos(y_pos), Screen_Width(screen_width), Lenght(lenght) {}

    unsigned short X_Pos;
    unsigned short Y_Pos;
    unsigned short Screen_Width;
    unsigned short Lenght;
};

//extern "C" void DrawLineHorizontal(CHAR_INFO* screen_buffer, OutputPos pos, CHAR_INFO symbol_info);
extern "C" void DrawLineHorizontal(CHAR_INFO* screen_buffer, OutputPos pos, LineInfo line_info);
extern "C" void DrawLineVertical(CHAR_INFO* screen_buffer, OutputPos pos, LineInfo line_info);
extern "C" void PrintColorPalette(CHAR_INFO* screen_buffer, OutputPos pos, CHAR_INFO symbol_info);