pub enum Direction {
    North = 0,
    East,
    South,
    West,
}

struct Player {
    X: i32,               //                    Player Offset
    Y: i32,               //                    Player Offset
    size: i32,            //                 Player Size
    travel: i32,          //               Player Speed
    direction: Direction, //            Player Direction
    lives: i32,
}
