#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "classes.h"

class GameOfLife : public olc::PixelGameEngine
{
public:
    GameOfLife()
    {
        sAppName = "G.o.G.o.L.";
    }
    
public:

    // Instance of Settings Container
    Settings S = Settings();

    // Instance of Level Data Container
    Levels L = Levels();

    // Instance of Spawner as Player
    Spawner player = Spawner();

    // Game States
    enum GAMESTATES
    {
    sPLAY=0, sSCORE=1, sMENU=2, sCREDIT=3,
    sINTRO=4, sHIGHS=5, sEND=6, sQUIT=7,
    };

    // Level Data
    enum LEVELDATA
    {
        lCONDITION=0, lIS_BLIND=1, lMIN=2, lMAX=3,
        lTOL=4, lW=5, lH=6, lTIME=7,
        lDENSE=8, lSIZE=9, lSPEED=10, lDIRECTION=11
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

    // Generational Matrices
    int matrix[512][256];//         matrix[width][height];
    int next[512][256];//           next[width][height];

    bool OnUserCreate() override
    {
        CleanSlate();
        S.state = sINTRO;
        return true;
    }

    // Drawing Routines

    void DrawMatrix()
    {
        Clear(olc::BLACK);
        int offset = (player.size+1)*32;

        if (!is_blind)
        {
            for (int x = 0; x < width; x++)
            {
                for (int y = 0; y < height; y++)
                {
                    if (matrix[x][y])
                    {
                        if (!S.danger && !S.safety) Draw(x, y+S.YOFF, olc::WHITE);
                        if (S.danger) Draw(x, y+S.YOFF, olc::RED);
                        if (S.safety) Draw(x, y+S.YOFF, olc::GREEN);
                    }
                }
            }
        }

        for (int s = 0; s < offset; s++)
        {
            for (int r = 0; r < 360; r+=1)
            {
                int lx = player.x+((s)+1)*cos(r);
                int ly = player.y+((s)+1)*sin(r);
                int grey;
                if ((lx  > -1 && lx < width) && (ly  > -1 && ly < height))
                {
                    if (is_blind) grey = int(255/(s+1));
                    if (!is_blind) grey = int(255-(255/(s+1)));
                    if (matrix[lx][ly])
                    {
                        if (S.safety) Draw(lx, ly+S.YOFF, olc::Pixel(0, grey, 0));
                        if (S.danger) Draw(lx, ly+S.YOFF, olc::Pixel(grey, 0, 0));
                        if (!S.danger && !S.safety) Draw(lx, ly+S.YOFF, olc::Pixel(grey, grey, grey));
                    }
                }
            }
        }
    }

    void DrawScore()
    {
        int value = int(points*S.counter);
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
            S.waiting = false; S.state = sPLAY;
        }
        Clear(olc::BLACK);

        std::string pt = "Points: "+std::to_string(points);
        std::string sc = "Score: "+std::to_string(score);

        float PT = GetOffset(pt, 8);
        float SC = GetOffset(sc, 8);

        DrawStringDecal({PT, 124}, pt, olc::YELLOW, {1.0f, 1.0f});
        DrawStringDecal({SC, 132}, sc, olc::YELLOW, {1.0f, 1.0f});
        if (S.level > L.total_levels) S.state = sEND;
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
        FillRect(0, 0, S.width, S.YOFF, olc::Pixel(16, 16, 16));
        FillRect(0, S.height+S.YOFF, S.width, S.height+S.YEND, olc::Pixel(16, 16, 16));
        int pop = GetPop();
        std::string level_text;
        if (keep_above && !keep_between) level_text = "> "+std::to_string(minimum);
        if (!keep_above && !keep_between) level_text = "< "+std::to_string(maximum);
        if (keep_between) level_text = "> "+std::to_string(minimum)+" && < "+std::to_string(maximum);
        level_text = level_text+" "+std::to_string(exceeded);

        // Upper HUD
        if (S.danger) DrawStringDecal({4, 4}, level_text, olc::RED, {1.5f, 1.5f});
        if (S.safety) DrawStringDecal({4, 4}, level_text, olc::GREEN, {1.5f, 1.5f});
        if (!S.danger && !S.safety) DrawStringDecal({4, 4}, level_text, olc::YELLOW, {1.5f, 1.5f});
        DrawStringDecal({416, 4}, "Time Left: "+std::to_string(int(timer)), olc::WHITE, {0.5f, 0.5f});
        DrawStringDecal({416, 12}, "Population: "+std::to_string(pop), olc::WHITE, {0.5f, 0.5f});
    
        // Lower HUD
        DrawStringDecal({0, float(S.height+S.YOFF+1)}, "x"+std::to_string(player.lives), olc::WHITE, {1.0f, 1.0f});
        DrawStringDecal({416, float(S.height+S.YOFF+1)}, "Score: "+std::to_string(points)+"/"+std::to_string(score), olc::WHITE, {1.0f, 1.0f});
    }

    void DrawMenu()
    {
        Clear(olc::BLACK);
        S.waiting = true;

        std::string ng = "New Game";
        std::string sc = "Scores";
        std::string xt = "Exit";

        float NG = GetOffset(ng, 4);
        float SC = GetOffset(sc, 4);
        float XT = GetOffset(xt, 4);

        DrawStringDecal({NG, 115}, ng, olc::YELLOW, {0.5f, 0.5f});
        DrawStringDecal({SC, 135}, sc, olc::YELLOW, {0.5f, 0.5f});
        DrawStringDecal({XT, 155}, xt, olc::YELLOW, {0.5f, 0.5f});

        Draw({235, 116+(20*S.menu_index)}, olc::WHITE);
        Draw({236, 116+(20*S.menu_index)}, olc::WHITE);
        Draw({235, 117+(20*S.menu_index)}, olc::WHITE);
        Draw({236, 117+(20*S.menu_index)}, olc::WHITE);
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

        DrawStringDecal({tt, 80}, title, olc::Pixel(S.color, S.color, S.color), {1.0f, 2.0f});
        DrawStringDecal({dg, 140}, digits, olc::Pixel(S.color, S.color, S.color), {0.5f, 0.5f});
        DrawStringDecal({jd, 160}, javid, olc::Pixel(S.color, S.color, S.color), {0.5f, 0.5f});
        DrawStringDecal({jn, 180}, john, olc::Pixel(S.color, S.color, S.color), {0.5f, 0.5f});
    }

    void DrawIntro()
    {
        S.tick++;
        if (S.tick > S.speed)
        {
            S.tick = 0;
            if (S.color > 0)
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
                DrawStringDecal({tt, 90}, title, olc::Pixel(S.color, S.color, S.color), {1.0f, 2.0f});
                DrawStringDecal({au, 110}, author, olc::Pixel(S.color, S.color, S.color), {0.5f, 0.5f});
                DrawStringDecal({fl, 150}, flavor, olc::Pixel(S.color, S.color, S.color), {1.0f, 1.0f});
                DrawStringDecal({en, 170}, engine, olc::Pixel(S.color, S.color, S.color), {0.5f, 0.5f});
                S.color--;
            }
            if (S.color == 0)
            {
                S.color = 255;
                S.state = sMENU;
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
        srand(S.seed);
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
            player.lives--;
            if (player.lives < 0) S.state = sEND;
        }
        if (passed)
        {
            if (exceeded == 0) player.lives++;
            S.level++;
            S.state = sSCORE;
        }
        exceeded = 0;
        if (S.level < L.total_levels)
        {
            if (L.game_level[S.level][lCONDITION] == 0) keep_above = false; keep_between = false;
            if (L.game_level[S.level][lCONDITION] == 1) keep_above = true; keep_between = false;
            if (L.game_level[S.level][lCONDITION] == 2) keep_between = true;
            is_blind = bool(L.game_level[S.level][lIS_BLIND]);
            minimum = L.game_level[S.level][lMIN];
            maximum = L.game_level[S.level][lMAX];
            tolerance = L.game_level[S.level][lTOL];
            density = L.game_level[S.level][lDENSE];
            timer = float(L.game_level[S.level][lTIME]);

            player.size = L.game_level[S.level][lSIZE];
            S.speed = L.game_level[S.level][lSPEED];
            player.direction = L.game_level[S.level][lDIRECTION];
            player.x = int(L.game_level[S.level][lW]/2);
            player.y = int(L.game_level[S.level][lH]/2);

            ClearMatrix();
            Randomize();
        }
        else S.state = sSCORE;
    }

    void CleanSlate()
    {
        player.lives = 1;
        S.speed = 0;
        points = 0;
        score = 0;
        highscore = 0;
        S.level = 0;
    }

    void StartGame()
    {
        CleanSlate();
        S.state = sPLAY;
        StartLevel(false);
    }

    void EndGame()
    {
        Clear(olc::BLACK);
        int value = int(score*S.counter);
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
            S.waiting = false;
            UpdateScores();
            S.state = sCREDIT;
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
        S.danger = false;
        S.safety = false;
        if (generations > S.begin_calc)
        {
            if (keep_above && !keep_between)
            {
                if (pop < minimum)
                {
                    if (points > 0) points--;
                    if (exceeded < tolerance) exceeded++;
                    S.danger = true;
                }
                if (pop < int(minimum+(minimum/3)))
                {
                    if (exceeded > 0) exceeded--;
                    points++;
                    S.safety = true;
                }
            }
            if (!keep_above && !keep_between)
            {
                if (pop > maximum)
                {
                    if (points > 0) points--;
                    if (exceeded < tolerance) exceeded++;
                    S.danger = true;
                }
                if (pop > int(maximum/3) && pop < int(maximum-(maximum/3)))
                {
                    if (exceeded > 0) exceeded--;
                    points++;
                    S.safety = true;
                }
            }
            if (keep_between)
            {
                int diff = maximum-minimum;
                if (pop < minimum || pop > maximum)
                {
                    if (points > 0) points--;
                    if (exceeded < tolerance) exceeded++;
                    S.danger = true;
                }
                if (pop > int(minimum+(diff/3)) && pop < int(maximum-(diff/3)))
                {
                    if (exceeded > 0) exceeded--;
                    points++;
                    S.safety = true;
                }
            }
        }
        if (exceeded < tolerance) passed = true;
        if (timer < 0.0f) StartLevel(passed);

        // Game Updates
        if (S.tick > S.speed)
        {
            if (generations > S.begin_calc)
            {
                int value = 0;
                // Update Potential Score
                if (keep_above && !keep_between) value = int((pop-maximum)*0.01f);
                if (!keep_above && !keep_between) value = int((pop-minimum)*0.01f);
                if (keep_between) value = int(pop*0.01f);
                points += value;
            }
            // Update Game
            S.tick = 0;
            Progress();
            DrawMatrix();
        }
        // Player Updates
        if (S.tick > player.travel)
        {
            if (player.direction == 0)
            {
                if (player.x < width-player.size-1) player.x++;
                if (player.x == width-player.size-1) StartLevel(false);
            }
            if (player.direction == 2)
            {
                if (player.x > player.size) player.x--;
                if (player.x == player.size) StartLevel(false);
            }
            if (player.direction == 1)
            {
                if (player.y < height-player.size-1) player.y++;
                if (player.y == height-player.size-1) StartLevel(false);
            }
            if (player.direction == 3)
            {
                if (player.y > player.size) player.y--;
                if (player.y == player.size) StartLevel(false);
            }
        }
        // Draw Player
        for (int s = 0; s < player.size; s++)
        {
            for (int r = 0; r < 360; r++)
            {
                int lx = player.x+(s+1)*cos(r);
                int ly = player.y+(s+1)*sin(r);
                int mod = int(s*(player.size+1));
                matrix[lx][ly] = true;
                Draw(lx, ly+S.YOFF, olc::Pixel(255-mod, 255-mod, 255-mod, 255-mod*32));
            }
        }
        matrix[player.x][player.y] = true; Draw(player.x, player.y+S.YOFF, olc::WHITE);
        DrawHUD();
    }

    void UpdateGame(float delta)
    {
        // Increment Game Tick
        if (!S.paused) S.tick++;

        if (S.state == sPLAY)
        {
            // Update Timeout
            if (!S.paused) timer -= delta;
            GameFrame();
        }

        if (S.state == sSCORE)
        {
            DrawScore();
            if (S.waiting) timer -= delta;
        }
        if (S.state == sMENU)
        {
            DrawMenu();
            if (S.waiting)
            {
                if (GetKey(olc::Key::W).bPressed || GetKey(olc::Key::UP).bPressed) if (S.menu_index > 0) S.menu_index -= 1;
                if (GetKey(olc::Key::S).bPressed || GetKey(olc::Key::DOWN).bPressed) if (S.menu_index < 2) S.menu_index += 1;
                
                if (GetKey(olc::Key::ENTER).bPressed)
                {
                    if (S.menu_index == 0) S.state = sPLAY; StartGame();
                    if (S.menu_index == 1) S.state = sHIGHS;
                    if (S.menu_index == 2) S.state = sQUIT;
                }
            }
        }
        if (S.state == sHIGHS)
        {
            DrawScores();
            if (S.waiting)
            {
                if (GetKey(olc::Key::ENTER).bReleased) S.state = sMENU;
            }
        }
        if (S.state == sINTRO)
        {
            DrawIntro();
            if (GetKey(olc::Key::ENTER).bReleased) S.color = 1;
        }
        if (S.state == sCREDIT)
        {
            DrawCredits();
            if (GetKey(olc::Key::ENTER).bReleased) S.state = sMENU;
        }
        if (S.state == sEND)
        {
            EndGame();
        }
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        // Pause Game
        if (GetKey(olc::Key::P).bPressed)
        {
            if (S.paused) S.paused = false;
            else S.paused = true;
        }
        // Set Player Direction
        if (GetKey(olc::Key::D).bPressed || GetKey(olc::Key::RIGHT).bPressed) player.direction = 0;
        if (GetKey(olc::Key::S).bPressed || GetKey(olc::Key::DOWN).bPressed) player.direction = 1;
        if (GetKey(olc::Key::A).bPressed || GetKey(olc::Key::LEFT).bPressed) player.direction = 2;
        if (GetKey(olc::Key::W).bPressed || GetKey(olc::Key::UP).bPressed) player.direction = 3;
        // Update Game Loop
        UpdateGame(fElapsedTime);
        // Quit
        if (S.state == sQUIT) return false;
        return !GetKey(olc::Key::ESCAPE).bPressed;
    }
};



int main()
{
    GameOfLife GOL;
    if (GOL.Construct(512, 288, 2, 2, false, false))
        GOL.Start();

    return 0;
}
