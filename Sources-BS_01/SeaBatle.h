///----------------------------------------------------------------------------|
/// Cкелетик пржэкта SeaBatle:
///-------------------------------------------------------статус:В разработке...
#include <iostream>
#include "Console.h"


///----------------------------------------------------------------------------|
const int NUMBERMAX_BOAT_TYPE  =   4; /// Количество типов кораблей.
const int MAX_PALUB            =   4; /// Mаксимальное количество палуб.
const int MAX_BOAT             =  10; /// Количество кораблей.
const int NUM_BOAT[MAX_BOAT]   = {4,3,3,2,2,2,1,1,1,1}; /// Палубы.

enum STATUS 
{   STATUS_EMPTY = 0,
    STATUS_LIVE, 
    STATUS_BOOM,
    STATUS_FORBOAT_INIT,
    STATUS_ZAPRET__INIT
};

///-Палуба:---oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo->
struct sPaluba
{   int x, y;
    int status; /// 0 - пусто.
                /// 1 - целый отсек корабля.
                /// 2 - подбитый отсек корабля.
    int n;      /// Номер корабля.
};

///-Корабль:---ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo->
class sBoat
{   
public: 
    sBoat() : amount_palub(0)
    {   for(int i = 0; i < MAX_PALUB; ++i)
        {   p_paluba[i] = NULL;
        };
    }

    ///----------------------------------------------------------------------var
    sPaluba* p_paluba[MAX_PALUB];
    int      amount_palub; /// Количество палуб.

    void show()
    {   cout << "\n";
        cout << "Для одного корабля:--------------------------------------->\n";
        for(int i = 0; i < amount_palub; i++)
        {   if(p_paluba[i] == NULL) cout << "Ахтунг! nullptr";
            else
            {   cout << "Палуба:------------------|||\n"
                     << "Номер корабля:" << p_paluba[i]->n      << "\n"
                     << "Координатa x: " << p_paluba[i]->x      << "\n"
                     << "Координатa y: " << p_paluba[i]->y      << "\n"
                     << "Статус:       " << p_paluba[i]->status << "\n"
                     << "Кол.палуб:    " << amount_palub        << "\n\n";
            }
        }
    }
    void showgraf(WORD field_x, WORD field_y)
    {   for(int i = 0; i < amount_palub; i++)
        {   print_xy_char(  p_paluba[i]->x*4+field_x+6,
                            p_paluba[i]->y*2+field_y+3,
                            p_paluba[i]->n);
        }
    }
};

///-Поле боя:---oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo->
class cField
{   
public:
    cField()
    {   init_poz_default();
    }
    
    static const int XMAX = 10;
    static const int YMAX = 10;
    sPaluba paluba[XMAX][YMAX]; ///<<<-----------------------------------------|

    COORD poz;
    void init_poz_default()
    {   poz.X = 17; //23
        poz.Y = 4;  //5
    }
    static const SHORT STEP_X = 4;
    static const SHORT STEP_Y = 2;

    void show()
    {   init_poz_default();
        SHORT x = poz.X;
        SHORT y = poz.Y;

        #define SETPOZ Input_system.set_xy(x, y); y++
        mySetColor();
        Input_system.set_xy(0, 0);
        cout << "Sea Battle...";
        //SETPOZ;       cout << "Sea Battle...";
        //system("pause");
        SETPOZ;cout << "|---|---------------------------------------|";
        SETPOZ;cout << "|   | A | B | C | D | E | F | G | J | K | L |";
        SETPOZ;cout << "|---|---------------------------------------|";
        SETPOZ;cout << "| 0 |   |   |   |   |   |   |   |   |   |   |";
        SETPOZ;cout << "|---|---------------------------------------|";
        SETPOZ;cout << "| 1 |   |   |   |   |   |   |   |   |   |   |";
        SETPOZ;cout << "|---|---------------------------------------|";
        SETPOZ;cout << "| 2 |   |   |   |   |   |   |   |   |   |   |";
        SETPOZ;cout << "|---|---------------------------------------|";
        SETPOZ;cout << "| 3 |   |   |   |   |   |   |   |   |   |   |";
        SETPOZ;cout << "|---|---------------------------------------|";
        SETPOZ;cout << "| 4 |   |   |   |   |   |   |   |   |   |   |";
        SETPOZ;cout << "|---|---------------------------------------|";
        SETPOZ;cout << "| 5 |   |   |   |   |   |   |   |   |   |   |";
        SETPOZ;cout << "|---|---------------------------------------|";
        SETPOZ;cout << "| 6 |   |   |   |   |   |   |   |   |   |   |";
        SETPOZ;cout << "|---|---------------------------------------|";
        SETPOZ;cout << "| 7 |   |   |   |   |   |   |   |   |   |   |";
        SETPOZ;cout << "|---|---------------------------------------|";
        SETPOZ;cout << "| 8 |   |   |   |   |   |   |   |   |   |   |";
        SETPOZ;cout << "|---|---------------------------------------|";
        SETPOZ;cout << "| 9 |   |   |   |   |   |   |   |   |   |   |";
        SETPOZ;cout << "|---|---------------------------------------|";
        //mess_gid.print("...do click!");
        #undef SETPOZ
    }

    void clear()
    {   for    (SHORT x = 0; x < XMAX; x++)
        {   for(SHORT y = 0; y < YMAX; y++)
            {   COORD Gon = { x*STEP_X + poz.X + 6,
                              y*STEP_Y + poz.Y + 3 };
                SetConsoleCursorPosition(Input_system.hCon, Gon);
                cout << ' ';
            }
        }
    }
    bool check()
    {   for        (SHORT x = 0; x < XMAX; x++)
        {   if(Input_system.x() == x*STEP_X + poz.X + 6)
            {   for(SHORT y = 0; y < YMAX; y++)
                    if(Input_system.y() == y*STEP_Y + poz.Y + 3)
                    {   return true;
                    }
            }
        }
        return false;
    }

    // Помечаем, куда можно пставить палубу:-----------------------------------|
    void set_forboat_init(SHORT X, SHORT Y)
    {
        const SHORT xx[3] = {-1, 0,  1};
        const SHORT yy[3] = { 1, 0, -1};
        for(SHORT i = 0; i < 3; i++)
        {   
            SHORT _x = X + xx[i];
            if(_x < 0 || _x >= XMAX) continue;

            for(SHORT j = 0; j < 3; j++)
            {   
                SHORT _y = Y + yy[j];
                if(_y < 0 || _y >= YMAX) continue;
                if(paluba[_x][_y].status == STATUS_EMPTY)
                {   paluba[_x][_y].status =  STATUS_FORBOAT_INIT;
                    show_status(_x, _y);
                }
            }
        }
        paluba[X][Y].status = STATUS_LIVE;
    }
    void set_finished_boat_init()
    {   for    (SHORT x = 0; x < XMAX; x++)
        {   for(SHORT y = 0; y < YMAX; y++)
            {   if(paluba[x][y].status == STATUS_FORBOAT_INIT)
                {   paluba[x][y].status =  STATUS_ZAPRET__INIT;
                    show_status(x, y);
                }
            }
        }
    }

private:
    void show_status(SHORT _x, SHORT _y)
    {       Input_system.set_xy (_x*STEP_X + poz.X + 6, _y*STEP_Y + poz.Y +3 );
            if(paluba[_x][_y].status == STATUS_FORBOAT_INIT)
             {  cout << '.';
             }

        else if(paluba[_x][_y].status == STATUS_ZAPRET__INIT)
             {  cout << '_';
             }
    }

};

///-Игрок:---ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo->
class cPlayer
{
public: 
    cPlayer()
    {   init_empty();
        computer_init_boat();
    }
    cPlayer(const char* str)
    {   init_empty();
    }

    void init_empty()
    {   for(int i = 0; i < 10; i++)
        {   for(int j = 0; j < 10; j++)
            Field.paluba[i][j].status = STATUS_EMPTY;
        }
        start_init_boat();
        User_init.p = this;
    }

    void start_init_boat()///------------------------------------------------foo
    {   // Инициализация boat[]
        // Количество палуб:
        for(size_t i = 0; i < MAX_BOAT; i++)
        {   boat[i].amount_palub = NUM_BOAT[i];
        }
    }
    void computer_init_boat()
    {   // Инициализация boat[]
        ///...
    }

    void stepI()
    {   ///...
    }
    void stepComp()
    {   ///...
    }

    ///----------------------------------------------------------------------var
    cField Field;
    sBoat  boat[MAX_BOAT];

    void show()
    {   cout << "\n===========================================Все корабли:===>";
        for(int i =0; i < MAX_BOAT; i++)
        {   boat[i].show();
            //getchar();
        }
    }

    void showgraf()
    {   for(int i = 0; i < MAX_BOAT; i++)
        {   boat[i].showgraf(Field.poz.X, Field.poz.Y);
        }
    }

    void user_init()///------------------------------------------------------foo
    {   if(Field.check())
        {   mySetColor(6,1);
            Input_system.set_char('A');

            // Test: ...........................................................
            COORD poz = User_init.get_xy_palub (
                Input_system.x(),
                Input_system.y() );
            mess.print   (poz.X);
        }
    }

//private:
    void generator()
    {   init_empty();
        
        int n  = 0;
        int nn;
        /// Начинаем с 0 корабля:
        for(nn = 0; nn < MAX_BOAT; nn++)
        {
            /// Количество NUM_BOAT[nn] палуб:
            int j = NUM_BOAT[nn];
            while(true)
            {   int xx = rrand();
                int yy = rrand();

                if (Field.paluba[xx][yy].status == STATUS_EMPTY)
                {   Field.paluba[xx][yy].x = xx;
                    Field.paluba[xx][yy].y = yy;

                    boat[nn].amount_palub  = j;

                    gen_add(Field.paluba[xx][yy], j, nn);
                    n += NUM_BOAT[nn];
                    break;
                }
            }
        }
        if(MAX_BOAT != nn)
        {   cout << "ахтунг!\n";
        }
    }

    class cUser_init///----------------------------------------------------class
    {
    public:
        cUser_init(){}
        cPlayer* p;

    //private:
        size_t num_boat;
        size_t num_palub;

        //---------------------------------------------------------------------|
        // Получаем индексы палубы в массиве field
        // по экранным координатам курсора:
        COORD get_xy_palub(SHORT screen_x, SHORT screen_y)
        {   COORD ccc = {   (screen_x - p->Field.poz.X - 6)/
                                        p->Field.STEP_X,

                            (screen_y - p->Field.poz.Y - 3)/
                                        p->Field.STEP_Y
            };
            return ccc;
        }
    }User_init;

private:///--------------------------------------------------------------private
    void gen_add(sPaluba pp, int amount_paluba, int _nn)
    {
        initpaluba(pp, _nn, 0);

        //if(1 == amount_paluba) return;
        for(int i = 1; i < amount_paluba; i++)
        {   //cout << "#";
            bool done = true;
            sPaluba _pp = pp;
            int ZAS     = 0;           /// Защита от загнанного цикла...
            do{
                if(ZAS++ > 100) break; /// Теряем палубы...
                done  = true;
                pp    = _pp;
                int r = rrand(0, 3);
                //cout << "\r" << r ;
                switch(r)
                {   case 0:
                        if(pp.x != 9) // На берег не вылазеем!!!
                        {   pp.x++;
                            done = false;
                        }
                        break;
                    case 1:
                        if(pp.x != 0)
                        {   pp.x--;
                            done = false;
                        }
                        break;
                    case 2:
                        if(pp.y != 9)
                        {   pp.y++;
                            done = false;
                        }
                        break;
                    case 3:
                        if(pp.y != 0)
                        {   pp.y--;
                            done = false;
                        }
                        break;
                    default:
                        cout << "Рандом сломался!!!\n";
                }
            }while((Field.paluba[pp.x][pp.y].status != STATUS_EMPTY) || done
                   ); // Повторить если ячейка занята!
            /// Внимание цикл может уйти в бесконечность где-нить в углу!
            initpaluba(pp, _nn, i);
        }
    }
    void initpaluba(sPaluba& P, int _nn, int _pal)
    {   //cout << "\n@@@@@\n";
        Field.paluba[P.x][P.y].status = STATUS_LIVE;
        Field.paluba[P.x][P.y].n      = _nn; // Номер корабля

        /// Клетка поля хранит собственные координаты:
        Field.paluba[P.x][P.y].x      = P.x;
        Field.paluba[P.x][P.y].y      = P.y;

        boat[_nn].p_paluba[_pal]  = &Field.paluba[P.x][P.y];
    }
};

///-Арбитр:---oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo->
class sArbitrator
{
public: 
    sArbitrator() : PlayerI("myname")
    {   done = true;
    }

    cPlayer PlayerComp,
            PlayerI;
    
    bool done;
    bool pro()//---------------------------------------------------------------|
    {   ///...
        PlayerI.Field.show();
        while(done)
        {   // Обойдемся без полиформизма, хотя....
            PlayerI.stepI();
            PlayerComp.stepComp();

            ///(это будет не здесь)Тест:
            PlayerComp.generator  ();
            PlayerComp.Field.clear();
            PlayerComp.showgraf   ();

            //PlayerI.user_init();
            //calibr();

            if(Input_system.loop() == VK_ESCAPE) break;
            getchar();///<<<---временно...
        }
        return false;
    }

    void callback_mouseclick()///<<<-------------------------------------------|
    {   cout << "\a";
        PlayerI.user_init();
        //PlayerI.Field.set_forboat_init   (9,9);///............................

        //getchar   ();
    }
    void callback_keysclick(int key)
    {   Input_system.set_xy(0, 0);
        cout << "\a" << key << "        ";

        if(key == VK_RETURN) 
        {   PlayerComp.generator  ();
            PlayerComp.Field.clear();
            PlayerComp.showgraf   ();
        }
        //getchar   ();
    }
};

///-Игра:---oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo->
class cGame
{
public:
    cGame()
    {   
    }

    void init_game()///--------------------------------------------------------|
    {   
    }

    void game()///-------------------------------------------------------------|
    {
        srand(static_cast<unsigned int>(time(NULL)));

        sArbitrator arbitrator;

        Input_system.my_delegate_mouse_click =
            std::bind(&sArbitrator::callback_mouseclick, &arbitrator);
        Input_system.my_delegate_keys_click =
            std::bind(&sArbitrator::callback_keysclick,  &arbitrator, 
                                                         std::placeholders::_1);
        arbitrator.pro();
    }

private:

};

