pub enum Direction {
    North = 0,
    East,
    South,
    West,
}

struct Spawner {
    x: i32,                 //              Spawner Offset
    y: i32,                 //              Spawner Offset
    size: i32,              //              Spawner Size
    delay: i32,             //              Spawner Speed
    direction: Direction,   //              Spawner Direction
    lives: i32,             //              Spawner Lives
}
