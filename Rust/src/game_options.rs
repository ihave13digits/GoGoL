struct GameOptions {
    pub danger: bool,       //          Shading Toggle
    pub paused: bool,       //          Game Updates
    pub waiting: bool,      //          Menu Updates
    pub counter: f32,       //          Score Updates
    pub begin_calc: u32,    //          Frames To Skip
    pub state: u32,         //          Game State
    pub level: u32,         //          Game Level
    pub seed: u32,          //          Game Seed
    pub tick: u32,          //          Game Tick
    pub speed: u32,         //          Tick Speed
    pub width: u32,         //          Horizontal Cells (World)
    pub height: u32,        //          Vertical Cells (World)
    pub color: u32,         //          UI Color For Fading
    pub menu_index: u32,    //          Selection Index
    pub menu_height: u32,   //          Menu Height
}
