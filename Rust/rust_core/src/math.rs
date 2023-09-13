use itertools::Itertools;

use num::{PrimInt};
use std::hash::Hash;

pub fn get_divisors<T>(input: T) -> impl Iterator<Item = T>
where T: PrimInt + Hash {
    num::range(T::one(), input + T::one())
    .take_while(move |x| *x * *x <= input)
    .filter(move |x| input % *x == T::zero())
    .flat_map(move |x| [x, input / x])
    .unique()
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_get_divisors_i32() {
        let divisors = get_divisors(24);
        assert!(get_divisors(24).all(|x| 24 % x == 0));
        assert_eq!(8, divisors.collect::<Vec<i32>>().len()); //1 24 2 12 3 8 4 6

        let divisors = get_divisors(9);
        assert!(get_divisors(9).all(|x| 9 % x == 0));
        assert_eq!(3, divisors.collect::<Vec<i32>>().len()); //1 3 9

        let divisors = get_divisors(1).collect::<Vec<i32>>();
        assert_eq!(1, divisors.len());
        assert_eq!(1, divisors[0]);
    }

    #[test]
    fn test_get_divisors_u32() {
        let divisors = get_divisors(24_u32);
        assert!(get_divisors(24_u32).all(|x| 24_u32 % x == 0));
        assert_eq!(8, divisors.collect::<Vec<u32>>().len());
    }

    #[test]
    fn test_get_divisors_i8() {
        let divisors = get_divisors(24_i8);
        assert!(get_divisors(24_i8).all(|x| 24_i8 % x == 0));
        assert_eq!(8, divisors.collect::<Vec<i8>>().len());
    }
}