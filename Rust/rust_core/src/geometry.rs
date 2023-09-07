use std::fmt;

#[derive(Hash, Eq, PartialEq, Clone, Copy)]
pub struct Coord {
    pub x: isize,
    pub y: isize
}
impl Coord {
    pub fn new(x: isize, y: isize) -> Self {
        Self { x, y }
    }
    pub fn from_str(input: &str) -> Self {
        let mut split = input.split(',');
        Self {
            x: split.next().expect("Missing X").parse().expect("Failed to parse value"),
            y: split.next().expect("Missing Y").parse().expect("Failed to parse value")
        }
    }
}

impl fmt::Display for Coord {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{{{},{}}}", self.x, self.y)
    }
}

#[derive(Hash, Eq, PartialEq, Clone, Copy)]
pub struct UCoord {
    pub x: usize,
    pub y: usize
}
impl UCoord {
    pub fn new(x: usize, y: usize) -> Self {
        Self { x, y }
    }
    pub fn from_str(input: &str) -> Self {
        let mut split = input.split(',');
        Self { 
            x: split.next().expect("Missing X").parse().expect("Failed to parse value"),
            y: split.next().expect("Missing Y").parse().expect("Failed to parse value"),
        }
    }
}

impl fmt::Display for UCoord {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{{{},{}}}", self.x, self.y)
    }
}