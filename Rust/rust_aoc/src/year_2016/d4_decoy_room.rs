pub fn part_one(lines: &Vec<String>) -> String {
    lines.iter().map(|line| Room::parse(line))
    .filter(|room| room.is_real())
    .map(|room| room.id)
    .sum::<u32>()
    .to_string()
}
pub fn part_two(lines: &Vec<String>) -> String {
    lines.iter().map(|line| Room::parse(line))
    .filter(|room| room.is_real())
    .map(|room| (room.decrypt(), room.id))
    .filter(|(name, _)| name == "northpole object storage")
    .map(|(_, id)| id)
    .sum::<u32>()
    .to_string()
}

struct Room {
    parts: Vec<String>,
    id: u32,
    checksum: String
}

impl Room {
    fn parse(line: &str) -> Self {
        let mut s1 = line.split("[");
        let parts = s1.next().unwrap().split("-").collect::<Vec<_>>();
        let checksum = s1.next().unwrap().strip_suffix("]").unwrap();
        let (last, elements) = parts.split_last().unwrap();
        let id = last.parse::<u32>().unwrap();
        let parts = elements.iter().map(|s| s.to_string()).collect();

        Self {parts, id, checksum: checksum.to_string() }
    }

    fn decrypt(&self) -> String {
        let rot = (self.id % 26) as u8;
        self.parts.iter().map(|p| p.as_bytes().iter().map(|c| rotate(c, rot)).collect::<String>()).collect::<Vec<_>>().join(" ")
    }

    fn is_real(&self) -> bool {
        let combined = self.parts.join("");
        let mut counts = ('a'..='z')
        .map(|c| (c, combined.chars().filter(|c2| c == *c2).count()))
        .collect::<Vec<_>>();
        //map of (a,4) (b,0)
        counts.sort_by(|(_, lhs), (_, rhs)| rhs.cmp(lhs));
        for index in 0..5 {
            if self.checksum.as_bytes()[index] as char != counts[index].0 {
                return false;
            }
        }

        true
    }
}

fn rotate(c: &u8, rot: u8) -> char {
    (((c - b'a' + rot) % 26) + b'a') as char
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn is_real_test() {
        assert!(Room::parse("aaaaa-bbb-z-y-x-123[abxyz]").is_real());
        assert!(Room::parse("a-b-c-d-e-f-g-h-987[abcde]").is_real());
        assert!(Room::parse("not-a-real-room-404[oarel]").is_real());
        
        assert!(!Room::parse("totally-real-room-200[decoy]").is_real());
    }
    #[test]
    fn decrypt_test() {
        assert_eq!("very encrypted name", Room::parse("qzmt-zixmtkozy-ivhz-343[abcde]").decrypt());
    }
}