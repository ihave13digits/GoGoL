use crate::Direction;

pub struct LevelData {
    pub condition: i32,
    pub is_blind: i32,
    pub min: i32,
    pub max: i32,
    pub tol: i32,
    pub width: i32,
    pub height: i32,
    pub time: i32,
    pub dense: i32,
    pub size: i32,
    pub speed: i32,
    pub direction: Direction,
}
