struct GameOptions {
    pub danger: bool,    //      Shading Toggle
    pub paused: bool,    //      Game Updates
    pub waiting: bool,   //     Menu Updates
    pub counter: f32,    //     Score Updates
    pub begin_calc: u32, //      Frames To Skip
    pub state: u32,      //            Game State
    pub level: u32,      //            Game Level
    pub seed: u32,       //             Game Seed
    pub tick: u32,       //             Game Tick
    pub speed: u32,      //            Tick Speed
    pub WIDTH: u32,      //          Horizontal Cells (World)
    pub HEIGHT: u32,     //         Vertical Cells (World)
    pub COLOR: u32,      //          UI Color For Fading
    pub menu_index: u32, //       Selection Index
    pub YOFF: u32,       //            Y Offset
}
