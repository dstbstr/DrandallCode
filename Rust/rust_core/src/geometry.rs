use std::fmt;
use std::ops::{Add, AddAssign, Sub, SubAssign, Mul, MulAssign};

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

    pub fn get_direct_neighbors(&self) -> Vec<Coord> {
        vec![Coord::new(self.x - 1, self.y), Coord::new(self.x + 1, self.y), Coord::new(self.x, self.y -1), Coord::new(self.x, self.y + 1)]
    }
    pub fn get_all_neighbors(&self) -> Vec<Coord> {
        let mut result: Vec<Coord> = Vec::new();
        for x in self.x - 1..=self.x + 1 {
            for y in self.y - 1..=self.y + 1 {
                let c = Coord::new(x, y);
                if c != *self {
                    result.push(c);
                }
            }
        }
        result
    }

    pub fn mdistance_origin(&self) -> isize {
        self.x.abs() + self.y.abs()
    }

    pub fn mdistance(&self, other: &Coord) -> isize {
        self.x.abs_diff(other.x) as isize + self.y.abs_diff(other.y) as isize
    }
}

impl fmt::Display for Coord {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{{{},{}}}", self.x, self.y)
    }
}

impl Add for Coord {
    type Output = Self;

    fn add(self, other: Self) -> Self {
        Self {
            x: self.x + other.x,
            y: self.y + other.y
        }
    }
}

impl AddAssign for Coord {
    fn add_assign(&mut self, other: Self) {
        *self = *self + other;
    }
}

impl Sub for Coord {
    type Output = Self;
    fn sub(self, other: Self) -> Self {
        Self {
            x: self.x - other.x,
            y: self.y - other.y
        }
    }
}

impl SubAssign for Coord {
    fn sub_assign(&mut self, other: Self) {
        *self = *self - other;
    }
}

impl Mul<Coord> for Coord {
    type Output = Self;
    fn mul(self, other: Self) -> Self {
        Self {
            x: self.x * other.x,
            y: self.y * other.y
        }
    }
}
impl Mul<isize> for Coord {
    type Output = Self;
    fn mul(self, scale: isize) -> Self {
        Self {
            x: self.x * scale,
            y: self.y * scale
        }
    }
}
impl MulAssign<Coord> for Coord {
    fn mul_assign(&mut self, other: Self) {
        *self = *self * other;
    }
}
impl MulAssign<isize> for Coord {
    fn mul_assign(&mut self, scale: isize) {
        *self = *self * scale;
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

    pub fn get_direct_neighbors(&self) -> Vec<UCoord> {
        vec![UCoord::new(self.x - 1, self.y), UCoord::new(self.x + 1, self.y), UCoord::new(self.x, self.y -1), UCoord::new(self.x, self.y + 1)]
    }
    pub fn get_all_neighbors(&self) -> Vec<UCoord> {
        let mut result: Vec<UCoord> = Vec::new();
        for x in self.x - 1..=self.x + 1 {
            for y in self.y - 1..=self.y + 1 {
                let c = UCoord::new(x, y);
                if c != *self {
                    result.push(c);
                }
            }
        }
        result
    }

    pub fn mdistance_origin(&self) -> usize {
        self.x + self.y
    }

    pub fn mdistance(&self, other: &UCoord) -> usize {
        self.x.abs_diff(other.x) + self.y.abs_diff(other.y) 
    }

}

impl fmt::Display for UCoord {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{{{},{}}}", self.x, self.y)
    }
}

impl Add for UCoord {
    type Output = Self;

    fn add(self, other: Self) -> Self {
        Self {
            x: self.x + other.x,
            y: self.y + other.y
        }
    }
}
impl AddAssign for UCoord {
    fn add_assign(&mut self, other: Self) {
        *self = *self + other;
    }
}

impl Sub for UCoord {
    type Output = Self;
    fn sub(self, other: Self) -> Self {
        Self {
            x: self.x - other.x,
            y: self.y - other.y
        }
    }
}

impl SubAssign for UCoord {
    fn sub_assign(&mut self, other: Self) {
        *self = *self - other;
    }
}

impl Mul<UCoord> for UCoord {
    type Output = Self;
    fn mul(self, other: Self) -> Self {
        Self {
            x: self.x * other.x,
            y: self.y * other.y
        }
    }
}
impl Mul<usize> for UCoord {
    type Output = Self;
    fn mul(self, scale: usize) -> Self {
        Self {
            x: self.x * scale,
            y: self.y * scale
        }
    }
}
impl MulAssign<UCoord> for UCoord {
    fn mul_assign(&mut self, other: Self) {
        *self = *self * other;
    }
}
impl MulAssign<usize> for UCoord {
    fn mul_assign(&mut self, scale: usize) {
        *self = *self * scale;
    }
}
