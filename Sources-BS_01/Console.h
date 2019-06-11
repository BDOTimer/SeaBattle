///----------------------------------------------------------------------------|
/// Процедуры для работы с Windows консолью.
///----------------------------------------------------------------------------|
#pragma once

#include <iostream>
#include <functional>
//#include "Delegate.h"

using std::cout;

#include <sstream>
template <typename T>
std::string toString(T val)
{   std::ostringstream oss;
    oss<< val;
    return oss.str();
}
template<typename T>
T fromString(const std::string& s)
{   std::istringstream iss(s);
    T res;
    iss >> res;
    return res;
}

#include <time.h>
int rrand(int range_min = 0, int range_max = 9)
{   return rand() % (range_max - range_min + 1) + range_min;
}

int colorr[10] = {4, 5, 6, 7, 11, 12, 13, 14, 3, 15};

enum ConsoleColor
{   Black         = 0,
    Blue          = 1,
    Green         = 2,
    Cyan          = 3,
    Red           = 4,
    Magenta       = 5,
    Brown         = 6,
    LightGray     = 7,
    DarkGray      = 8,
    LightBlue     = 9,
    LightGreen    = 10,
    LightCyan     = 11,
    LightRed      = 12,
    LightMagenta  = 13,
    Yellow        = 14,
    White         = 15
};

#include <Windows.h>
#define SHOW_CHAR(N, color)  \
    mySetColor(color);       \
    cout << N << "\n";       \
    mySetColor()
#define S(N,C) "";mySetColor(C);cout<<N;mySetColor();cout

void mySetColor(WORD text = 7, WORD background = 0) // #include <Windows.h>
{   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                          (background << 4) | text);
}

///ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
#include <stdio.h>
#include <windows.h>

void MyErrorExit(char *s)
{   printf("Fatal: %s\n", s);
    exit(1);
}

class sArbitrator;
struct cInput_system
{   cInput_system() : 
                 hStdin             (0),
                 hCon               (0),
                 cNumRead           (0),
                 fdwMode            (0),
                 fdwSaveOldMode     (0),
                 Cursor             (hCon),
                 press_mouse_button (false),
                 press_key          (false)
    {   hStdin = GetStdHandle(STD_INPUT_HANDLE);
        if (hStdin == INVALID_HANDLE_VALUE)
            MyErrorExit("GetStdHandle");

        // Save the current input mode, to be restored on exit.
        if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
            MyErrorExit("GetConsoleMode");

        hCon = GetStdHandle(STD_OUTPUT_HANDLE);

        memset(irInBuf, 0 , NUM_irInBuf * sizeof(irInBuf[0]));

        set_xy    (0, 0);
        set_Color (0x69);
    }
    
    HANDLE hStdin;
    HANDLE hCon;
    DWORD  cNumRead, fdwMode, fdwSaveOldMode;

    static const int NUM_irInBuf = 128;
    INPUT_RECORD irInBuf[NUM_irInBuf];
    std::function<void()> my_delegate_mouse_click;
    std::function<void(int)> my_delegate_keys_click;
    bool press_mouse_button;
    bool press_key;

    /*------------------------------------------------------------------------->
    Step-1:
    Disable 'Quick Edit Mode' option
    */
    //fdwMode = ENABLE_EXTENDED_FLAGS;
    //if (! SetConsoleMode(hStdin, fdwMode) )
    //    MyErrorExit("SetConsoleMode");
    /*
    Step-2:
    Enable the window and mouse input events,
    after you have already applied that 'ENABLE_EXTENDED_FLAGS'
    to disable 'Quick Edit Mode'
    */
    //fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
    //if (!SetConsoleMode(hStdin, fdwMode))
    //    MyErrorExit("SetConsoleMode");//---------------------------------------<

    enum eINPUT {INPUT_NOTHING, INPUT_MOUSE_CLICK, INPUT_KEYS};
    int loop()
    {   // Enable the window and mouse input events.
        fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
        if (!SetConsoleMode(hStdin, fdwMode))
            MyErrorExit("SetConsoleMode");
        
        while(true) // Wait for the events.
        {   
            if (!ReadConsoleInputA(
                hStdin,      // input buffer handle
                irInBuf,     // buffer to read into
                128,         // size of read buffer
                &cNumRead))  // number of records read
                MyErrorExit("ReadConsoleInput");

            // Dispatch the events to the appropriate handler.
            for (size_t i = 0; i < cNumRead; i++)
            {
                switch (irInBuf[i].EventType)
                {
                case KEY_EVENT: // keyboard input
                {   //KeyEventProc(irInBuf[i].Event.KeyEvent);
                    int key = (int)irInBuf[i].Event.KeyEvent.uChar.AsciiChar;
                    if(key == VK_ESCAPE)
                        return VK_ESCAPE;

                    press_key = !press_key;
                    if(press_key)
                        my_delegate_keys_click(key);
                    cout << key;
                    break;
                }

                case MOUSE_EVENT: // mouse input
                    Cursor.MouseEventProc(irInBuf[i].Event.MouseEvent);

                    if (irInBuf[i].EventType==MOUSE_EVENT) 
                    {                    
                        switch (irInBuf[i].Event.MouseEvent.dwEventFlags)
                        {
                        case MOUSE_MOVED: //cout<<"Moved";
                            break;
                        case DOUBLE_CLICK:  //cout << "Double Click";
                            break;
                        case MOUSE_WHEELED: //cout << "Wheel";
                            break;
                        default: //cout << "Click"; //(pressed or released);
                            press_mouse_button = !press_mouse_button;
                            if(press_mouse_button) my_delegate_mouse_click();
                            //return INPUT_MOUSE_CLICK;
                        }
                        //printf(" (X=%d, Y=%d)\n", 
                        //irInBuf[i].Event.MouseEvent.dwMousePosition.X,
                        //irInBuf[i].Event.MouseEvent.dwMousePosition.Y);
                    }
                    break;

                case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing
                    ResizeEventProc(
                        irInBuf[i].Event.WindowBufferSizeEvent);
                    break;

                case FOCUS_EVENT:  // disregard focus events
                    //cout << "#";

                case MENU_EVENT:   // disregard menu events
                    //cout << "O";
                    break;

                default:
                    MyErrorExit("unknown event type");
                    break;
                }
            }
        }
        return INPUT_NOTHING;
    }

    VOID ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD dummy) { return; };
    VOID KeyEventProc(KEY_EVENT_RECORD dummy)
    {   //cout << "X";
        //cout << (int)dummy.uChar.AsciiChar;
        return; 
    }
    VOID GetInputEvents(VOID) { return; };

    void set_xy(SHORT x, SHORT y)
    {   COORD coord = {x, y};
        SetConsoleCursorPosition(hCon, coord);
    }
    void set_char(char ch)
    {   SetConsoleCursorPosition(hCon, Cursor.coord);
        cout << ch;
        Cursor.set_char_old(ch);
    }

    void set_Color(WORD _c) // #include <Windows.h>
    {   SetConsoleTextAttribute(hCon, _c);
    }

    SHORT x(){return Cursor.coord.X;}
    SHORT y(){return Cursor.coord.Y;}

private:
    ///-Курсор-----------------------------------------------------------------|
    class cCursor
    {
    public://------------------------------------------------------------------|
        cCursor(HANDLE& hCon) : _hcon(hCon)
        {   coord.X = coord.Y = 0;
        }

        COORD   coord;
        VOID MouseEventProc(MOUSE_EVENT_RECORD ir)
        {   if (ir.dwEventFlags == MOUSE_MOVED)
            {   coord.X = ir.dwMousePosition.X;
                coord.Y = ir.dwMousePosition.Y;
                my_drawcursor();
            }
        }
        void set_char_old(char ch)
        {   char_old = ch;
        }

    private://-----------------------------------------------------------------|
        HANDLE& _hcon;
        char     char_old;

        void my_drawcursor()
        {   COORD& _c = coord;

            static COORD coord_old = {-1, 0};
            static WORD  atr_old;

            if((_c.X != coord_old.X) ||  (_c.Y != coord_old.Y))
            {   SetConsoleCursorPosition(_hcon, coord_old);
                SetConsoleTextAttribute (_hcon, atr_old);

                cout << static_cast<char>(char_old);

                atr_old  = read_atrib(_c);
                char_old = read_char (_c);
            }

            SetConsoleCursorPosition(_hcon, _c);
            coord_old = _c;
            mySetColor(4,7);
            cout << static_cast<char>(read_char (_c));
            mySetColor();
        }

        char read_char (COORD& pos)
        {   const int SIZE = 1;
            char  buff[SIZE];
            DWORD cbRead = 0;
            ReadConsoleOutputCharacter (_hcon, buff, SIZE, pos, &cbRead);
            return buff[0];
        }
    public:
        WORD read_atrib(COORD& pos)
        {   const int sz = 1;
            WORD  buff[1];
            DWORD cbRead = 0;
            ReadConsoleOutputAttribute(_hcon, buff, sz, pos, &cbRead);
            return buff[0];
        }
    }Cursor;
    ///------------------------------------------------------------------------|
public:
    WORD read_atrib(COORD& pos)
    {   return Cursor.read_atrib(pos);
    }
};

///ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
extern cInput_system Input_system;
///ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo

void print_xy_char(SHORT x, SHORT y, char c = 'O')
{   COORD Gon = { x, y };
    SetConsoleCursorPosition(Input_system.hCon, Gon);
    SHOW_CHAR(static_cast<char>(c+64), colorr[c]);
}

void calibr()
{   COORD Gon = { 19, 3 };
    SetConsoleCursorPosition(Input_system.hCon, Gon);
    cout << 'W';
}

typedef BOOL (WINAPI *FN_SETCONSOLEFONT)(HANDLE, DWORD);
FN_SETCONSOLEFONT SetConsoleFont;
void setfont_01()
{   HMODULE hm;// = GetModuleHandleW(L"KERNEL32.DLL");

    int retCode =::GetModuleHandleExA(0, "KERNEL32.DLL", &hm);

    SetConsoleFont = reinterpret_cast<FN_SETCONSOLEFONT>
                                     (GetProcAddress (hm, "SetConsoleFont") );
    SetConsoleFont(Input_system.hCon, 9);
    //6 в Windows 7 ставит Lucida 12
}

void showcursor(BOOL b = TRUE)
{   CONSOLE_CURSOR_INFO info;
    info.dwSize   = 100;
    info.bVisible =   b;
    SetConsoleCursorInfo( Input_system.hCon, &info );
}

///ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
/// Вывод мессаги.
///ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
#include <string>
class cMess
{
public:
    cMess(
        std::string _s, 
        SHORT        x,          /// Позиция Х на экране.
        SHORT        y,          /// Позиция Y на экране.
        WORD         color_fg,   /// Цвет текста знакоместа.
        WORD         color_bg,   /// Цвет заднего фона.
        size_t       len         /// Ограничение длины строки на экране.
        )
        : length_max(len),
          SS(_s)
    {   ccc.X = x;
        ccc.Y = y;
        now_color = (color_bg << 4) | color_fg;
        bg = color_bg;
        str.reserve(80);
    }

    template<class T>
    void print(T _s)
    {   mem_cursor_foo();
        
        str = SS + toString(_s);
        if(str.length() > length_max)
        {   str = str.substr   (0, length_max); // усекаем до length_max
            // error
            // Выести сообщение:
            str[length_max-1] = '.';
            str[length_max-2] = '.';
            str[length_max-3] = '.';
        }
        else
        {   size_t i = length_max - str.length();
            for (; i != 0; i--)
            {   str += ' ';
            }
        }
        cout << str;
        SetConsoleCursorPosition(Input_system.hCon, mem_cursor);
        SetConsoleTextAttribute (Input_system.hCon, mem_color);
    }

private:
    COORD ccc, mem_cursor;
    WORD  now_color, mem_color, bg; 
    const std::string SS;
          std::string str;
    size_t length_max;
    inline void mem_cursor_foo()
    {   mem_cursor.X = Input_system.x();
        mem_cursor.Y = Input_system.y();
        SetConsoleCursorPosition(Input_system.hCon, ccc);

        mem_color = Input_system.read_atrib(mem_cursor);
        SetConsoleTextAttribute(Input_system.hCon, now_color);
    }
};
cMess mess("coord56789012345", 0, 1, 15, 1, 16);
cMess mess_gid("Gid: ", 2, 29, 15, 1, 76);
cMess mess_Err("Gid: ", 2, 30, 15, 1, 76);

///.............................................................................
inline int resizing_the_console_buffer_to_a_smaller_size()
{
    HANDLE hOut;
    CONSOLE_SCREEN_BUFFER_INFO SBInfo;
    COORD NewSBSize;
    int Status;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleScreenBufferInfo(hOut, &SBInfo);
    NewSBSize.X = SBInfo.dwSize.X - 2;
    NewSBSize.Y = SBInfo.dwSize.Y;

    Status = SetConsoleScreenBufferSize(hOut, NewSBSize);
    if (Status == 0)
    {
        Status = GetLastError();
        cout << "SetConsoleScreenBufferSize() failed! Reason : " << Status 
             << std::endl;
        //exit(Status);
    }

    GetConsoleScreenBufferInfo(hOut, &SBInfo);

    cout << "Screen Buffer Size : ";
    cout << SBInfo.dwSize.X << " x ";
    cout << SBInfo.dwSize.Y << std::endl;

    return 0;
}