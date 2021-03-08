use crate::Direction;

pub struct Player {
    pub x: i32,               //                    Player Offset
    pub y: i32,               //                    Player Offset
    pub size: i32,            //                 Player Size
    pub travel: i32,          //               Player Speed
    pub direction: Direction, //            Player Direction
    pub lives: i32,
}

impl Player {
    pub fn pmove(mut self, direction: Direction) {
        match direction {
            Direction::North => {
                self.x += 1;
            }
            Direction::East => {
                self.y += 1;
            }
            Direction::South => {
                self.x -= 1;
            }
            Direction::West => {
                self.y -= 1;
            }
        }
    }
}
