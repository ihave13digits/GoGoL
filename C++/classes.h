// Container For Game Settings
class Settings
{
public:
    bool safety = false;//      Shading Toggle
    bool danger = false;//      Shading Toggle
    bool paused = false;//      Game Updates
    bool waiting = false;//     Menu Updates
    
    float counter = 0.01;//     Score Updates
    
    int begin_calc = 10;//      Frames To Skip
    int state = 0;//            Game State
    int level = 0;//            Game Level
    int seed = 1;//             Game Seed
    int tick = 0;//             Game Tick
    int speed = 1;//            Tick Speed
    int width = 512;//          Horizontal Cells (World)
    int height = 256;//         Vertical Cells (World)
    int color = 255;//          UI Color For Fading
    int menu_index = 0;//       Selection Index
    int YOFF = 24;//            Upper HUD Height
    int YEND = 8;//             Lower HUD Height
};



// Container For Game Levels
class Levels
{
public:
    int total_levels = 15;
    
    int game_level[15][12] = {
        //CNDTN  BLIND    MIN     MAX      TOL    WIDTH   HEIGHT  TIME   DENSE    SIZE    SPD     DIR
        //{   2,     0,     5000,   7500,    500,    512,    256,    45,     10,     3,      1,      0    },// test level
        {   1,     0,     0,      0,       250,    512,    256,    15,     1,      0,      2,      0    },
        {   0,     0,     0,      750,     250,    512,    256,    15,     1,      1,      2,      0    },
        {   0,     1,     0,      500,     250,    512,    256,    15,     2,      0,      1,      0    },
        {   0,     0,     0,      250,     250,    512,    256,    15,     3,      0,      1,      0    },
        {   2,     0,     0,      500,     250,    512,    256,    15,     3,      0,      1,      0    },
        {   1,     1,     750,    0,       250,    512,    256,    30,     4,      0,      1,      0    },
        {   1,     0,     500,    0,       250,    512,    256,    30,     5,      1,      1,      0    },
        {   2,     1,     500,    1000,    500,    512,    256,    30,     5,      0,      1,      0    },
        {   0,     0,     0,      2000,    500,    512,    256,    30,     6,      1,      1,      0    },
        {   2,     0,     2000,   3000,    500,    512,    256,    30,     7,      2,      1,      0    },
        {   1,     0,     3000,   0,       500,    512,    256,    45,     8,      2,      1,      0    },
        {   0,     1,     0,      5000,    500,    512,    256,    45,     9,      2,      1,      0    },
        {   2,     0,     3000,   4000,    500,    512,    256,    45,     8,      3,      1,      0    },
        {   2,     0,     5000,   7500,    500,    512,    256,    45,     10,     3,      1,      0    },
        {   2,     0,     5500,   7500,    500,    512,    256,    60,     11,     4,      2,      0    }
    };
};



//
class Spawner
{
public:
    int x = 0;//                    Spawner Offset
    int y = 0;//                    Spawner Offset
    int size = 0;//                 Spawner Size
    int travel = 0;//               Spawner Speed
    int direction = 0;//            Spawner Direction
    int lives = 0;//                Spawner Respawns
};
