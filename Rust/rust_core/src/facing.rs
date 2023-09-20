use crate::geometry::{Coord, UCoord};

pub enum Facing { Up, Right, Down, Left }

impl Facing {
    pub fn parse(c: char) -> Self {
        match c.to_lowercase().next().unwrap() {
            'u' => Facing::Up,
            'd' => Facing::Down,
            'l' => Facing::Left,
            'r' => Facing::Right,
            _ => panic!("Unable to parse character")
        }
    }
    pub fn parse_byte(b: u8) -> Self {
        match b {
            b'u' => Facing::Up,
            b'U' => Facing::Up,
            b'd' => Facing::Down,
            b'D' => Facing::Down,
            b'l' => Facing::Left,
            b'L' => Facing::Left,
            b'r' => Facing::Right,
            b'R' => Facing::Right,
            _ => panic!("Unable to parse byte")
            
        }
    }

    pub fn turn_right(&self) -> Self {
        match self {
            Facing::Up => Facing::Right,
            Facing::Right => Facing::Down,
            Facing::Down => Facing::Left,
            Facing::Left => Facing::Up
        }
    }

    pub fn turn_left(&self) -> Self {
        match self {
            Facing::Up => Facing::Left,
            Facing::Left => Facing::Down,
            Facing::Down => Facing::Right,
            Facing::Right => Facing::Up
        }
    }

    pub fn turn_around(&self) -> Self {
        match self {
            Facing::Up => Facing::Down,
            Facing::Down => Facing::Up,
            Facing::Left => Facing::Right,
            Facing::Right => Facing::Left
        }
    }

    pub fn move_coord(&self, pos: &mut Coord) {
        self.move_coord_by(pos, 1);
    }

    pub fn move_coord_by(&self, pos: &mut Coord, distance: isize) {
        match self {
            Facing::Up => pos.y -= distance,
            Facing::Down => pos.y += distance,
            Facing::Left => pos.x -= distance,
            Facing::Right => pos.x += distance
        };
    }

    pub fn move_ucoord(&self, pos: &mut UCoord) {
        self.move_ucoord_by(pos, 1);
    }

    pub fn move_ucoord_by(&self, pos: &mut UCoord, distance: usize) {
        match self {
            Facing::Up => pos.y -= distance,
            Facing::Down => pos.y += distance,
            Facing::Left => pos.x -= distance,
            Facing::Right => pos.x += distance
        };

    }
}

pub enum Direction { North, East, South, West }

impl Direction {
    pub fn parse(c: char) -> Self {
        match c.to_lowercase().next().unwrap() {
            'n' => Direction::North,
            'e' => Direction::East,
            's' => Direction::South,
            'w' => Direction::West,
            _ => panic!("Unable to parse character")
        }
    }

    pub fn parse_byte(b: u8) -> Self {
        match b {
            b'n' => Direction::North,
            b'N' => Direction::North,
            b'e' => Direction::East,
            b'E' => Direction::East,
            b's' => Direction::South,
            b'S' => Direction::South,
            b'w' => Direction::West,
            b'W' => Direction::West,
            _ => panic!("Unable to parse byte")
        }
    }

    pub fn turn_right(&self) -> Self {
        match self {
            Direction::North => Direction::East,
            Direction::East => Direction::South,
            Direction::South => Direction::West,
            Direction::West => Direction::North
        }
    }

    pub fn turn_left(&self) -> Self {
        match self {
            Direction::North => Direction::West,
            Direction::West => Direction::South,
            Direction::South => Direction::East,
            Direction::East => Direction::North
        }
    }

    pub fn turn_around(&self) -> Self {
        match self {
            Direction::North => Direction::South,
            Direction::South => Direction::North,
            Direction::West => Direction::East,
            Direction::East => Direction::West
        }
    }

    pub fn move_coord(&self, pos: &mut Coord) {
        self.move_coord_by(pos, 1);
    }

    pub fn move_coord_by(&self, pos: &mut Coord, distance: isize) {
        match self {
            Direction::North => pos.y -= distance,
            Direction::South => pos.y += distance,
            Direction::West => pos.x -= distance,
            Direction::East => pos.x += distance
        };
    }

    pub fn move_ucoord(&self, pos: &mut UCoord) {
        self.move_ucoord_by(pos, 1);
    }

    pub fn move_ucoord_by(&self, pos: &mut UCoord, distance: usize) {
        match self {
            Direction::North => pos.y -= distance,
            Direction::South => pos.y += distance,
            Direction::West => pos.x -= distance,
            Direction::East => pos.x += distance
        };
    }
}