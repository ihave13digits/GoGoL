#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class GameOfLife : public olc::PixelGameEngine
{
public:
    GameOfLife()
    {
        sAppName = "G.o.G.o.L.";
    }

public:

    // Game Variables
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
    int WIDTH = 512;//          Horizontal Cells (World)
    int HEIGHT = 256;//         Vertical Cells (World)
    int COLOR = 255;//          UI Color For Fading
    int menu_index = 0;//       Selection Index
    int YOFF = 20;//            Y Offset

    // Game States
    int sPLAY = 0;
    int sSCORE = 1;
    int sMENU = 2;
    int sCREDIT = 3;
    int sINTRO = 4;
    int sHIGHS = 5;
    int sEND = 6;
    int sQUIT = 7;

    // Level Data
    int lCONDITION = 0;
    int lIS_BLIND = 1;
    int lMIN = 2;
    int lMAX = 3;
    int lTOL = 4;
    int lW = 5;
    int lH = 6;
    int lTIME = 7;
    int lDENSE = 8;
    int lSIZE = 9;
    int lSPEED = 10;
    int lDIRECTION = 11;

    // Game Levels
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

    // Level Variables
    bool is_blind = false;//        Dark Level
    bool keep_between = false;//    Level Condition
    bool keep_above = false;//      Level Condition
    int exceeded = 0;//             Track Conditions
    int tolerance = 0;//            Maximum Overage
    int generations = 0;//          Level Generations
    int maximum = 1000;//           Tolerance For Population
    int minimum = 0;//              Tolerance For Population
    int density = 2;//              Random Chance (percent of screen to set true)
    int width = 512;//              Horizontal Cells (Screen)
    int height = 256;//             Vertical Cells (Screen)
    float timer = 10.0f;//          Level Timeout

    // Score Accumulation
    int points = 0;//               Level Score
    int score = 0;//                Game Score
    int highscore = 0;//            Highscore Placeholder
    int highscores[10] = {
        10000000,
        5000000,
        1000000,
        500000,
        100000,
        50000,
        10000,
        5000,
        1000,
        500
    };

    // Player Variables
    int X = 0;//                    Player Offset
    int Y = 0;//                    Player Offset
    int size = 0;//                 Player Size
    int travel = 0;//               Player Speed
    int direction = 0;//            Player Direction
    int lives = 0;

    // Generational Matrices
    int matrix[512][256];//         matrix[width][height];
    int next[512][256];//           next[width][height];

    bool OnUserCreate() override
    {
        CleanSlate();
        state = sINTRO;
        return true;
    }

    // Drawing Routines

    void DrawMatrix()
    {
        Clear(olc::BLACK);
        int offset = (size+1)*32;

        if (!is_blind)
        {
            for (int x = 0; x < width; x++)
            {
                for (int y = 0; y < height; y++)
                {
                    if (matrix[x][y])
                    {
                        if (!danger && !safety) Draw(x, y+YOFF, olc::WHITE);
                        if (danger) Draw(x, y+YOFF, olc::RED);
                        if (safety) Draw(x, y+YOFF, olc::GREEN);
                    }
                }
            }
        }

        for (int s = 0; s < offset; s++)
        {
            for (int r = 0; r < 360; r+=1)
            {
                int lx = X+((s)+1)*cos(r);
                int ly = Y+((s)+1)*sin(r);
                int grey;
                if ((lx  > -1 && lx < width) && (ly  > -1 && ly < height))
                {
                    if (is_blind) grey = int(255/(s+1));
                    if (!is_blind) grey = int(255-(255/(s+1)));
                    if (matrix[lx][ly])
                    {
                        if (safety) Draw(lx, ly+YOFF, olc::Pixel(0, grey, 0));
                        if (danger) Draw(lx, ly+YOFF, olc::Pixel(grey, 0, 0));
                        if (!danger && !safety) Draw(lx, ly+YOFF, olc::Pixel(grey, grey, grey));
                    }
                }
            }
        }
    }

    void DrawScore()
    {
        int value = int(points*counter);
        if (points >= value)
        {
            if (value > 0)
            {
                points -= value;
                score += value;
            }
            else
            {
                points--;
                score++;
            }
        }
        if (points <= 0)
        {
            waiting = false; state = sPLAY;
        }
        Clear(olc::BLACK);

        std::string pt = "Points: "+std::to_string(points);
        std::string sc = "Score: "+std::to_string(score);

        float PT = GetOffset(pt, 8);
        float SC = GetOffset(sc, 8);

        DrawStringDecal({PT, 124}, pt, olc::YELLOW, {1.0f, 1.0f});
        DrawStringDecal({SC, 132}, sc, olc::YELLOW, {1.0f, 1.0f});
        if (level > total_levels) state = sEND;
    }

    void DrawScores()
    {
        DrawStringDecal({236, 40}, "Highscores:", olc::YELLOW, {0.5f, 0.5f});
        for (int i = 0; i < 10; i++)
        {
            std::string sc = std::to_string(highscores[i]);
            float SC = GetOffset(sc, 4);
            DrawStringDecal({SC, float((i*10)+80)}, sc, olc::YELLOW, {0.5f, 0.5f});
        }
    }

    void DrawHUD()
    {
        FillRect(0, 0, WIDTH, YOFF, olc::Pixel(16, 16, 16));
        int pop = GetPop();
        std::string level_text;
        if (keep_above && !keep_between) level_text = "> "+std::to_string(minimum);
        if (!keep_above && !keep_between) level_text = "< "+std::to_string(maximum);
        if (keep_between) level_text = "> "+std::to_string(minimum)+" && < "+std::to_string(maximum);

        DrawStringDecal({0, 0}, "x"+std::to_string(lives), olc::WHITE, {1.5f, 1.5f});
        DrawStringDecal({32, 0}, level_text, olc::YELLOW, {1.5f, 1.5f});
        DrawStringDecal({416, 0}, "Score: "+std::to_string(points)+"/"+std::to_string(score), olc::WHITE, {0.5f, 0.5f});
        DrawStringDecal({416, 5}, "Time Left: "+std::to_string(int(timer)), olc::WHITE, {0.5f, 0.5f});
        DrawStringDecal({416, 10}, "Population: "+std::to_string(pop), olc::WHITE, {0.5f, 0.5f});
    }

    void DrawMenu()
    {
        Clear(olc::BLACK);
        waiting = true;

        std::string ng = "New Game";
        std::string sc = "Scores";
        std::string xt = "Exit";

        float NG = GetOffset(ng, 4);
        float SC = GetOffset(sc, 4);
        float XT = GetOffset(xt, 4);

        DrawStringDecal({NG, 115}, ng, olc::YELLOW, {0.5f, 0.5f});
        DrawStringDecal({SC, 135}, sc, olc::YELLOW, {0.5f, 0.5f});
        DrawStringDecal({XT, 155}, xt, olc::YELLOW, {0.5f, 0.5f});

        Draw({235, 116+(20*menu_index)}, olc::WHITE);
        Draw({236, 116+(20*menu_index)}, olc::WHITE);
        Draw({235, 117+(20*menu_index)}, olc::WHITE);
        Draw({236, 117+(20*menu_index)}, olc::WHITE);
    }

    void DrawCredits()
    {
        std::string title = "G.o.G.o.L";
        std::string john = "In memory of John Conway.  Thank you for your contributions to this crazy thing we call life.";
        std::string javid = "Thanks to javidx9 for olcPixelGameEngine, and the many lessons he's taught.";
        std::string digits = "Written by ihave13digits with olcPixelGameEngine";

        float tt = GetOffset(title, 8);
        float jn = GetOffset(john, 4);
        float jd = GetOffset(javid, 4);
        float dg = GetOffset(digits, 4);

        DrawStringDecal({tt, 80}, title, olc::Pixel(COLOR, COLOR, COLOR), {1.0f, 2.0f});
        DrawStringDecal({dg, 140}, digits, olc::Pixel(COLOR, COLOR, COLOR), {0.5f, 0.5f});
        DrawStringDecal({jd, 160}, javid, olc::Pixel(COLOR, COLOR, COLOR), {0.5f, 0.5f});
        DrawStringDecal({jn, 180}, john, olc::Pixel(COLOR, COLOR, COLOR), {0.5f, 0.5f});
    }

    void DrawIntro()
    {
        tick++;
        if (tick > speed)
        {
            tick = 0;
            if (COLOR > 0)
            {
                std::string title = "G.o.G.o.L";
                std::string author = "by digits";
                std::string flavor = "Powered by";
                std::string engine = "olcPixelGameEngine";
                
                float tt = GetOffset(title, 8);
                float au = GetOffset(author, 4);
                float fl = GetOffset(flavor, 8);
                float en = GetOffset(engine, 4);

                Clear(olc::BLACK);
                DrawStringDecal({tt, 90}, title, olc::Pixel(COLOR, COLOR, COLOR), {1.0f, 2.0f});
                DrawStringDecal({au, 110}, author, olc::Pixel(COLOR, COLOR, COLOR), {0.5f, 0.5f});
                DrawStringDecal({fl, 150}, flavor, olc::Pixel(COLOR, COLOR, COLOR), {1.0f, 1.0f});
                DrawStringDecal({en, 170}, engine, olc::Pixel(COLOR, COLOR, COLOR), {0.5f, 0.5f});
                COLOR--;
            }
            if (COLOR == 0)
            {
                COLOR = 255;
                state = sMENU;
            }
        }
    }

    // Helper Routines

    int GetOffset(std::string txt, int w)
    {
        int total = int(txt.length()*w);
        float offset = width/2-total/2;
        return offset;
    }

    // Matrix Routines
    void ClearMatrix()
    {
        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                matrix[x][y] = false;
                next[x][y] = false;
            }
        }
    }

    void Randomize()
    {
        srand(seed);
        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                int chance = rand() % 100;
                if (chance < density) matrix[x][y] = true;
            }
        }
    }

    int GetPop()
    {
        int pop = 0;
        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                if (matrix[x][y]) pop++;
            }
        }
        return pop;
    }

    void Progress()
    {
        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                // Get Neighbors
                int count = 0;
                for (int i = -1; i < 2; i++)
                {
                    for (int j = -1; j < 2; j++)
                    {
                        int X = x+i; int Y = y+j;
                        if ((X > 0 && X < width) && ((Y > 0 && Y < height))) if (matrix[X][Y]) count++;
                    }
                }
                if (matrix[x][y]) count--;
                // Apply Rules
                if ((count < 2 || count > 3) && matrix[x][y]) next[x][y] = false;
                if ((count == 2 || count == 3) && matrix[x][y]) next[x][y] = true;
                if (count == 3 && !matrix[x][y]) next[x][y] = true;
            }
        }

        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                matrix[x][y] = next[x][y];
            }
        }
        generations++;
    }

    // Game Routines

    void SortScores()
    {
        for (int i = 0; i < 9; i++)
        {
            for (int i = 0; i < 9; i++)
            {
                if (highscores[i] < highscores[i+1])
                {
                    int replacement = highscores[i];
                    highscores[i] = highscores[i+1];
                    highscores[i+1] = replacement;
                }
            }
        }
    }

    void UpdateScores()
    {
        // Update Scores
        for (int i = 0; i < 10; i++)
        {
            if (highscores[i] < highscore)
            {
                highscores[i] = highscore;
                break;
            }
        }
        SortScores();
    }

    void StartLevel(bool passed)
    {
        generations = 0;
        Clear(olc::BLACK);
        if (!passed)
        {
            points = 0;
            lives--;
            if (lives < 0) state = sEND;
        }
        if (passed)
        {
            if (exceeded == 0) lives++;
            level++;
            state = sSCORE;
        }
        exceeded = 0;
        if (level < total_levels)
        {
            if (game_level[level][lCONDITION] == 0) keep_above = false; keep_between = false;
            if (game_level[level][lCONDITION] == 1) keep_above = true; keep_between = false;
            if (game_level[level][lCONDITION] == 2) keep_between = true;
            is_blind = bool(game_level[level][lIS_BLIND]);
            minimum = game_level[level][lMIN];
            maximum = game_level[level][lMAX];
            tolerance = game_level[level][lTOL];
            density = game_level[level][lDENSE];
            timer = float(game_level[level][lTIME]);

            size = game_level[level][lSIZE];
            speed = game_level[level][lSPEED];
            direction = game_level[level][lDIRECTION];
            X = int(game_level[level][lW]/2);
            Y = int(game_level[level][lH]/2);

            ClearMatrix();
            Randomize();
        }
        else state = sSCORE;
    }

    void CleanSlate()
    {
        lives = 1;
        speed = 0;
        points = 0;
        score = 0;
        highscore = 0;
        level = 0;
    }

    void StartGame()
    {
        CleanSlate();
        state = sPLAY;
        StartLevel(false);
    }

    void EndGame()
    {
        Clear(olc::BLACK);
        int value = int(score*counter);
        if (score > value && value > 0)
        {
            highscore += value;
            score -= value;
        }
        else
        {
            highscore++;
            score--;
        }
        if (score == 0)
        {
            waiting = false;
            UpdateScores();
            state = sCREDIT;
        }

        std::string go = "GAME OVER";
        std::string sc = "Score: "+std::to_string(score);
        std::string hs = "Highscore: "+std::to_string(highscore);

        float GO = GetOffset(go, 8);
        float SC = GetOffset(sc, 4);
        float HS = GetOffset(hs, 4);

        DrawStringDecal({GO, 120}, go, olc::YELLOW, {1.0f, 1.0f});
        DrawStringDecal({SC, 140}, sc, olc::WHITE, {0.5f, 0.5f});
        DrawStringDecal({HS, 150}, hs, olc::WHITE, {0.5f, 0.5f});
    }

    // Game Logic
    void GameFrame()
    {
        bool passed = false;
        int pop = GetPop();
        // Check Level Conditions
        danger = false;
        safety = false;
        if (generations > begin_calc)
        {
            if (keep_above && !keep_between)
            {
                if (pop < minimum)
                {
                    if (points > 0) points--;
                    if (exceeded < tolerance) exceeded++;
                    danger = true;
                }
                if (pop < int(minimum+(minimum/3)))
                {
                    if (exceeded > 0) exceeded--;
                    points++;
                    safety = true;
                }
            }
            if (!keep_above && !keep_between)
            {
                if (pop > maximum)
                {
                    if (points > 0) points--;
                    if (exceeded < tolerance) exceeded++;
                    danger = true;
                }
                if (pop > int(maximum/3) && pop < int(maximum-(maximum/3)))
                {
                    if (exceeded > 0) exceeded--;
                    points++;
                    safety = true;
                }
            }
            if (keep_between)
            {
                int diff = maximum-minimum;
                if (pop < minimum || pop > maximum)
                {
                    if (points > 0) points--;
                    if (exceeded < tolerance) exceeded++;
                    danger = true;
                }
                if (pop > int(minimum+(diff/3)) && pop < int(maximum-(diff/3)))
                {
                    if (exceeded > 0) exceeded--;
                    points++;
                    safety = true;
                }
            }
        }
        if (exceeded < tolerance) passed = true;
        if (timer < 0.0f) StartLevel(passed);

        // Game Updates
        if (tick > speed)
        {
            if (generations > begin_calc)
            {
                int value = 0;
                // Update Potential Score
                if (keep_above && !keep_between) value = int((pop-maximum)*0.01f);
                if (!keep_above && !keep_between) value = int((pop-minimum)*0.01f);
                if (keep_between) value = int(pop*0.01f);
                points += value;
            }
            // Update Game
            tick = 0;
            Progress();
            DrawMatrix();
        }
        // Player Updates
        if (tick > travel)
        {
            if (direction == 0 && X < width-size-1) X++;
            if (direction == 2 && X > size) X--;
            if (direction == 1 && Y < height-size-1) Y++;
            if (direction == 3 && Y > size) Y--;
        }
        // Draw Player
        for (int s = 0; s < size; s++)
        {
            for (int r = 0; r < 360; r++)
            {
                int lx = X+(s+1)*cos(r);
                int ly = Y+(s+1)*sin(r);
                int mod = int(s*(size+1));
                matrix[lx][ly] = true;
                Draw(lx, ly+YOFF, olc::Pixel(255-mod, 255-mod, 255-mod, 255-mod*32));
            }
        }
        matrix[X][Y] = true; Draw(X, Y+YOFF, olc::WHITE);
        DrawHUD();
    }

    void UpdateGame(float delta)
    {
        // Increment Game Tick
        if (!paused) tick++;

        if (state == sPLAY)
        {
            // Update Timeout
            if (!paused) timer -= delta;
            GameFrame();
        }

        if (state == sSCORE)
        {
            DrawScore();
            if (waiting) timer -= delta;
        }
        if (state == sMENU)
        {
            DrawMenu();
            if (waiting)
            {
                if (GetKey(olc::Key::W).bPressed || GetKey(olc::Key::UP).bPressed) if (menu_index > 0) menu_index -= 1;
                if (GetKey(olc::Key::S).bPressed || GetKey(olc::Key::DOWN).bPressed) if (menu_index < 2) menu_index += 1;

                if (GetKey(olc::Key::ENTER).bPressed)
                {
                    if (menu_index == 0) state = sPLAY; StartGame();
                    if (menu_index == 1) state = sHIGHS;
                    if (menu_index == 2) state = sQUIT;
                }
            }
        }
        if (state == sHIGHS)
        {
            DrawScores();
            if (waiting)
            {
                if (GetKey(olc::Key::ENTER).bReleased) state = sMENU;
            }
        }
        if (state == sINTRO)
        {
            DrawIntro();
            if (GetKey(olc::Key::ENTER).bReleased) COLOR = 1;
        }
        if (state == sCREDIT)
        {
            DrawCredits();
            if (GetKey(olc::Key::ENTER).bReleased) state = sMENU;
        }
        if (state == sEND)
        {
            EndGame();
        }
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        // Pause Game
        if (GetKey(olc::Key::P).bPressed)
        {
            if (paused) paused = false;
            else paused = true;
        }
        // Set Player Direction
        if (GetKey(olc::Key::D).bPressed || GetKey(olc::Key::RIGHT).bPressed) direction = 0;
        if (GetKey(olc::Key::S).bPressed || GetKey(olc::Key::DOWN).bPressed) direction = 1;
        if (GetKey(olc::Key::A).bPressed || GetKey(olc::Key::LEFT).bPressed) direction = 2;
        if (GetKey(olc::Key::W).bPressed || GetKey(olc::Key::UP).bPressed) direction = 3;
        // Update Game Loop
        UpdateGame(fElapsedTime);
        // Quit
        if (state == sQUIT) return false;
        return !GetKey(olc::Key::ESCAPE).bPressed;
    }
};



int main()
{
    GameOfLife GOL;
    if (GOL.Construct(512, 276, 2, 2, false, false))
        GOL.Start();

    return 0;
}
