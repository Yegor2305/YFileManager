#include "Panel.h"
#include "YApplication.h"
#include <vector>

using namespace std;

void HandleResize(HANDLE hConsole, CHAR_INFO* screen_buffer, COORD buffer_size, Panel &panel) {
    CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
    GetConsoleScreenBufferInfo(hConsole, &screen_buffer_info);
    COORD new_size;
    new_size.X = buffer_size.X;
    new_size.Y = screen_buffer_info.dwSize.Y;
    SetConsoleScreenBufferSize(hConsole, new_size);
    panel.Draw();
}

int main(void)
{
    HANDLE std_handle, screen_buffer_handle;
    CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info{};
    SMALL_RECT srctWriteRect;
    CHAR_INFO *screen_buffer;
    COORD screen_buffer_coord{};
    vector<INPUT_RECORD> input_buffer(128);
    DWORD num_events{};
    int buffer_size{};

    // Get a handle to the STDOUT screen buffer to copy from and create a new screen buffer to copy to.

    std_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    screen_buffer_handle = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);                   

    if (std_handle == INVALID_HANDLE_VALUE || screen_buffer_handle == INVALID_HANDLE_VALUE) {
        printf("CreateConsoleScreenBuffer failed - (%d)\n", GetLastError());
        return 1;
    }

    // Make the new screen buffer the active screen buffer.

    if (!SetConsoleActiveScreenBuffer(screen_buffer_handle)) {
        printf("SetConsoleActiveScreenBuffer failed - (%d)\n", GetLastError());
        return 1;
    }
    
    if (!GetConsoleScreenBufferInfo(screen_buffer_handle, &screen_buffer_info)) {

        printf("GetConsoleScreenBufferInfo failed - (%d)\n", GetLastError());
        return 1;
    }  

    buffer_size = screen_buffer_info.dwSize.X * screen_buffer_info.dwSize.Y;

    screen_buffer = new CHAR_INFO[buffer_size];
    memset(screen_buffer, 0, buffer_size * sizeof(CHAR_INFO));

    /*srctWriteRect.Top = 10;   
    srctWriteRect.Left = 0;
    srctWriteRect.Bottom = 11;
    srctWriteRect.Right = 79;*/

    YApplication app(screen_buffer_handle, screen_buffer, screen_buffer_info);
    Panel panel(screen_buffer_info.dwSize.X, screen_buffer_info.dwSize.Y);
    app.AddWidget(&panel);
    app.Draw();
    //panel.Draw();
    CHAR_INFO symbol_info{};
    symbol_info.Char.UnicodeChar = L'-';
    symbol_info.Attributes = 0x50;

    /*DrawLineHorizontal(screen_buffer, OutputPos(0, 0, screen_buffer_info.dwSize.X, screen_buffer_info.dwSize.X), symbol_info);
    symbol_info.Char.UnicodeChar = L'|';
    DrawLineVertical(screen_buffer, OutputPos(0, 0, screen_buffer_info.dwSize.X, screen_buffer_info.dwSize.Y), symbol_info);*/

    /*symbol_info.Char.UnicodeChar = L'X';
    PrintColorPalette(screen_buffer, OutputPos(0, 0, screen_buffer_info.dwSize.X, screen_buffer_info.dwSize.X), symbol_info);*/

    //screen_buffer[0].Char.UnicodeChar = L'і';
    //screen_buffer[0].Attributes = 0x50;

    if (!WriteConsoleOutput(screen_buffer_handle, screen_buffer, screen_buffer_info.dwSize, screen_buffer_coord, &screen_buffer_info.srWindow)) {
        printf("WriteConsoleOutput failed - (%d)\n", GetLastError());
        return 1;
    }

    while (true) {
        Sleep(100);

       
        app.Resize(screen_buffer_info);
    }

    if (!SetConsoleActiveScreenBuffer(std_handle)) {
        printf("SetConsoleActiveScreenBuffer failed - (%d)\n", GetLastError());
        return 1;
    }

    return 0;
}
