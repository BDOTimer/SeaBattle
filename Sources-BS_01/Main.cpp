///-Main-----------------------------------------------------------------------|
#include "SeaBatle.h"

cInput_system Input_system;

int main()
{   

    //hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    setlocale (0, "");
    setfont_01();
    showcursor(FALSE);
    system    ("mode con cols=80 lines=31");
    cout << "Привет, мир!";
    //test();
    
    resizing_the_console_buffer_to_a_smaller_size();

    cGame Game;
    Game.game();

    return 0;
}
