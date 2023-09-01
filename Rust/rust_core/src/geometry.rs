use std::fmt;

//#[derive(Debug)]
pub struct Coord {
    pub x: i64,
    pub y: i64
}
impl Coord {
    pub fn new(x: i64, y: i64) -> Self {
        Self{
            x,
            y
        }
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