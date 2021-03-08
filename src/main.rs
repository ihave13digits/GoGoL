pub mod game_options;
pub mod level;
pub mod player;
pub mod score_handling;
pub mod ui;

pub enum Direction {
    North = 0,
    East,
    South,
    West,
}

fn main() {
    let _scores = score_handling::Scores {
        highscore: 1,
        points: 1,
        score: 1,
    };
    let _game_options = game_options::GameOptions {
        danger: false,
        paused: false,
        waiting: false,
        counter: 1.0,
        begin_calc: 1,
        state: 1,
        level: 1,
        seed: 1,
        tick: 1,
        speed: 1,
        width: 1,
        height: 1,
        color: 1,
        menu_index: 1,
        yoff: 1,
    };
    println!("{}", ui::credits::JOHN);
    println!("{}", ui::credits::JAVID);
    println!("{}", ui::credits::DIGITS);
    println!("{}", ui::credits::ABLE);
}
