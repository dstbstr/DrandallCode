#pragma once

#include "Common.h"

DECLARE_SOLUTION(2021, 20, R"RawInput_(##..#.#...##.####..#..###.#.#.#.#.####.#.##.....#####.##..#..#.#.##.......##.##.#.#...#....#.####..#.##.##....###..##.#####.##....##.#.#.#.#....####...##.#......#.#..##......##..#..###.#..####.###...##.#.##.#..##.##..#.#.#..###..##.####.#.#.#..#.##...#..##...##.#####..##.##..#...#..###.#.#...#..#..##...#.#..........#...#...#.#.#.#..#.###..##....####..#######.##.#.....#.#.###...##...###...#.##.##.##..#......#.###...#.#.#.#...##.#.#.##.#..###.#...##...##.......####.##..#.#....#.#####..#..#####.#.....#....#.#.

.##.#...##..##.#..........#####..#.##..######.##..#....#..#####.#.#####.#..#...#..#####..#.......##.
.#.#.#.#####..##.#.###..#.##.##.......#.####.#............#.#....####..#.#####.##.##.#.###..#..##...
.##.#.#...##....#.#..#...........##.#.#..#####...#..##.###..##.###..........#####..##..##.#.####....
##..####.####.###.......#....#.#####.##...#....###...##...###...#...##..####....####..#####..##..###
...###.##.....#......###.#.###..#..###...#.#..#.#######.##..#..#..#..##.##..#####.#####.###...#.###.
##.###.#....##..####.#..#.###.##.#.##..#...#...###.###.#..#..#.#.###.##.#..###.#.###....###.###.....
#.#.##..#.....#.....##....######...#........#.#.#.#.######..#...##.#.######...#.##########..#.###..#
.##.#...##.#......###..####.##.#...#..##.#.####...##..####..#####..##.###.#.###...#.#.######.#.#####
..#..##..#.#.#...####..###.#..#...#..#.#.#.#.###..####.#...#......#..#..####.#...#.#.#......#...#..#
#...###..#.#.###.#.##.##.#.#.#####....#....#.....#####...#..#..#.#####..##.....#..###.#..###.#..#...
..##.#.#..##.#.#...#...#.#.#..#.#.##....#####....#.##...#..#..#.#....###...##..#.#.##.##.#.......#.#
##.##.#######.###.#..#....#..##..#.###.....#.###..#.#..#....#.#..###.##.#.....#.#.#.##.#...#..##.#..
.....#.#.#.#.####.#..####.#....##.###.###...########.#.#....#..##.#.#####...##....##....#.......#.#.
##..#...#..#.#.####..#.#..####.#####.#####.###..#.#####.#..#...##.######.###.#..#.##.#.#..#..#.####.
#.##.##.#.####...#.#.#.#..####.#.#...###...#.#....##...####.#..#####.#.####.#..#.....###.#..#...#.#.
.#..#.##...####...##.####...###..####.##........#.#.##..#..#.##...#....##.#.#..#.#.###...##.......#.
..#..#..#.###.##...###....#.#..#..##..#...###.###...#..#.....#..#.####.#.#.#.##..###...#....##.#.###
###.##....##.#.##..####.#.###...###.###.#####..##.###.....#.#####....#.##..##.#####.##..###....#....
###..#...###..#....#..#.#...#....###.#.####.##..#.#..#.###..#..#.##.#.#.......#...#....#...###..#.##
#...#.#.#...#.####.##.#.#..#...##.....#..##....#..#.##..##.####.#.#.###..##..#.#.#.###.#.#..####..##
.###..###.##..###..#..#.##..#...#.##.#.......####.#..##.#...#.##..#..........##..####..##.###..###..
###.#.#...###..#...#####.####..#..#######.#.#.###.##.###.#.####.##.#####..###.#####.#...####.##..##.
.##..##..##.#...######.###...#.##.##....##.#...##.#####.#...##..#.#...#.#..#.##.#....##.####.#.##.#.
...####..#...#...####.#.#...#...###..#.##...####.#..######.##.....##..#...#...####.###.#..##.##..#..
#.#####.##.##..#..##.#.####..##.#.#...#..####.##..###.####.##.##...#.#...#.#.#####.####.#.###..#..#.
..#.....#..##..#...#####..##.###..##.#..##..##..####.#..#.####.###...#####.#.###.....#..#.####.#.##.
#####....###..##.#.######.#.##..##.#.#####.##########.#...##...###....###.#.....#.....###.#.#.#.##.#
##.....#.###.###.#.##.########....##.##.###...#######..##..#.#.#..#.##..####..#.###.#.###.#.#......#
..##..#...#.#.#.#.###.#..#####.#.#..#......####..#.###..###...##...##.##..#..#..#.##.#..###..##.#.#.
.#.#..#..###.##.#.#...#.#...##...#.###..#.#.##.#....#..####..#.#.##.#..#...##.#.....##...####.....#.
##.#..##...#..##.####.##....##..#####.###...#..######..#....#..#######..#..#.####.#...##.##..##..#..
..##.#.###....#..#.....#..#.#.#.#........#..##.#.#.......####.##...#.#.##.######..##..#....#.#......
.....#....#.#.##...#..######.###.###.##....##.#.##.###.....#..#.#....#####.#..##.#.....#..#.##....##
##.....#.#...####...##.##.#.##.#.##......#.#########.#.##..##...###.##.#..#....#..##.####..#######..
##...#.########.###.#....#...#..##.##.###....##...#####..##.####..##..#..#####.#..######.####...#.#.
##.####..##.#####..#..###...##.......#...#.###.#.#..#..#....#.#..##.....#.#....####.###...#.#..##.#.
#.#.#.#.###..#.#####.#...##...#.###.##..#.#####.....#...#.#.#..#...##....#...####.####.##.#.....####
..###.##.#......#######..#...##..###.##....#....#.#..###...#..###.#.#...#..##...####.###.###..#.#.##
.#..#.#.##..###########.#.#..#.###.######...###.##..#.###..##.#...#.#...###.#.##.##.#..#..#..#.#...#
.#..#.#..##....#....####.#..##....#.###.####..#.##.#.#..###.#.##.#........##.#...###.#.......##.#...
...###.#.#.##.##.##.....#.#.#..#..#.##.#######...##....#.#......##....##.#.####.####.###.#....##..#.
.#.##..#.##.##.####.....##...#..###.###.###..#..##.####..##...#.###.##..##..#####.#..###....###.##.#
#.###...#####.#..#.####...##.#..#...######.....#.#....##.#.#...##....#...........##.##.##.#...######
##..#..##.....##..#.#..##.##.#..#...####.######.......###..#..##.##..##..##..###..##.##.#.#.....#...
#......###..#####..#######.#.#..#.###..##.#..##.##.##.#..###.###..##.##.....#.#..####.#.##...##..###
.##...#..###..##..#.#####.#####.###...#.........#.##.##.###.#.#.#...###.#..###.##.##.#..#..#...###.#
#.###.###.###.#.###.#.##....##..##.......#....##.###.###.#.###..#..###.#.##.##..#.#...######.####..#
..#####.###.##.#..##.##.#..#.###..#.####....#...##.#.......#.#..##.#.......##.##.#######...######.##
#.##...###..#..##.....#.#.##.#..##...##.#####..#...##...#..#.####.#.##.##.#....##.###.#.##..##..#.#.
..#.###.#.##...#..##.#..####.###..#####.##.####.#.##..####.###..##..#...#.##...#.###....####...####.
.###.....###.#....######.#####..#..#..#.#.#..#..#.###.#........##.###...#...##..#..#...#.....#.###.#
..###.##.#.#####.#..########..##..##.#..###.#.##.#....#.###........#....#...#.#...##.#.##..#..##.##.
###.#.#.#...#..##....#.##....##.#######.#.##.#.##....#....###.#.#.....#.##..###..##..#..##..#.#..##.
.#.##..#.##..##.##.##..#.###.#..##.###.#.###.##......##.#...##.##..##...##.#...##.#..#..#.##..#.##..
####..####.###..#..#####...###.#....###.#...##........#.#...#..#.#.###..####.#.#.######...#..##.#.#.
#...#..#.##..#.#.#.#.#...##.#.##..#...#.###.###.#....##.#..##.#.###..##....#.###.#.##..#..##....#...
#.###.#.##.###.#..#...####.#....#..#.#####.#...#.###.##.##.###.###..#...#.#.####..###..........##.#.
##.##.#.###.#.##.####...###.#.###..###...####..#####.###...#.##...###.##..#.####.#.##...#....##.#.##
#....#..##.##.##......##...##.##..#####...#.##.###.##...##..##..###..#..#..#...###.##.#..#..#.#...##
###.####.....#.#....####..#.##.........#....##.....#....##.#..##.##..####..###.####.#...##....##.##.
#.......###.#..####.##..#..##...#....###...##.......##.....###.######.#..#..##..#....#.###.#..##.###
#.###.##..##..##.##...#.........####..####..#..##...##.#..#..#######.###...#......#.##.####.#..##...
#.####...##.#...#.#.###...##...#....#.##.##.####.#.#....###....#...#........##.##..#.#.####..#.#.#..
.#..###.#..#.....##..#..#.###..###.#.##...#.#.....#...#....#.##.#..#.#...#..#.#..###..#.#...###.....
.####....####....#.#.....#..#.#.##........#..#.####.##.##...#.#.####.#..#.##..##..#...#.#.##...#.#.#
#####.#...####.#.####.#...#..#...#.#.##.#..#...##..#.#...##.###.##.#.###.###.#...#..#.###.##.##...#.
#.#....#.###.##..##.##..##.#...#.###....###....###..#.####.###..#.#.##...#..####....#.####...###.#..
.#...#.###.#.####..#.#####...##...#..##........#....#..#.#.##..##.##.#######.#.#.#.##.#.#.#.##..#...
#....#....#.#.##.##.##..###..###.##..#.##.....###.###......#.#....#..#.##......#.###.#..#.#.#.#..###
#####.#####.##...#.#..#..##.####.#......######..####..##.##.##.#.......#...####..#..###.#..##.#.#.#.
#.##...##..##......###...#.#.##...#.#...#..##.###.#..#.##...#.#.#.##.###.##.#####....#...#.....##..#
..##....##..#..##..##...#..#.##.####.##........###..##.##.#.#.##..#..###...#..#...##.#.##.########.#
#...#.#####.###.#....#....#######.###..#..#.#..##..#.###.#.####.##.#..###..#....###.###....###.#..##
.......###.#.#.#..#.#..###...#.##..#..#...#..#....#..#...#.#####.#...##.#.###.###.#..#.##.#..##.....
....#.##...#.##.#.###..#####..#.######.###...###..##.#.#.##.###.##.####..#..#.....#.......#.######.#
##..####......##...##.#..#..#..#..##.#.#..##..####.#.######.#...##..#..#.##.##..###..#..#.#..#..#...
.##.#..#..##.#.##..#..#.#.#...##.##...##.#.########..##..#...#####.....#.#..###.#.##.##.###..#..#.##
..##...#.#..#.#.##.###..#....#..###...#..##....##.#..######..###.#.#####..#.###.##.###..#####...####
#.#..###.##.#...#..###..#.##.##...####.####.#.....##....###..#..#...####.####.###...#..#.#.#.#.##..#
###..##...#.#..##..#...##..###.#.....#...##.###.#..###...###.####..###.#..#.#...##.....#............
.#####..##...##....######...#..###.#.#......#...######.#..#...###.#...#..####.#......##..#..#.#...#.
.#..####.#.#..###..#.#.##.####..#.#....#.####..##...#..#.#.#..###.........##.#.#####...###...#.#..#.
.#.#....#..####..##..#########.#..#.##..###..#..#...###....##.....####...#..#.##....#######.####.##.
##.#.####..#...#..#.##.#..#....##.####..####..###.#.#...#######.#.###..###.###..#.###.##...##.##..#.
.#####.....##.#....##...##...#.#..####.##..#....##.###.#####.##.#.#.#...#...#.#.#.......##..##.##.#.
.##..#.#.###.#.#.#....##.#...#....#...#...#..#######....##.#.#.#####.#.###..#..#...#......#..#.##..#
...##.....#..#.####.###...#####.#.#....##.#.#.#.#...####.####.#..##.#.##.##.###..##...##.....#...###
####.###..###.####..###.##..#.#####.#...##.#..#.#.##.####.#..####..#..##....####.#....#...#.####.#.#
##....##.#.#.##.###..#####.##..##..#..##..##..#.#..###.##.#####...#..#.##.#.####.####..#...##..#..##
#.#..###..##..#..#..####..#..#.#..#.##...###.#.#.#.#...####.#.#...####.##..#.#.#..#.##..##..##.#....
##.#.####....##..##..####.#######..##....######.##.#....#..#...#.#..#.##.#...#.#.#.#.#..###....#.#.#
.##.###..#.###..##...####.###.#.##.###.#...#..#......######.##.##.##.####.##.##....#.#......####.#.#
#..###.####.#...#.###.###.#..##.###..#.#.#####..#.##.##.#.##.##..###.####...####..#..###...#.##.####
#...............#.##.###...#..#..#.##.##.#.##.#.#...#.#....#..#.######..#..#..##....#.##..#..#..#.##
###.###...#...#..#....##.......#.#..#...##...#....#..######..#.....##.####..#....#...##...######..##
..##..#..##.#.##..###.#...............####...##..##......#...#.....#..#....#.#...#.....##..#####..##
#.....##.#.##.###......###.#.##..###....###..#...#..###.#..#..##...#..#.##...#.#...#..#.....##...#.#
..##....#..#.#.#.####..#.#.#....#.#.#.#.##.#..#.#..#..#.#.##.####......##..#...##..#.#.##..#.....#..
.#..##....###.###..##....#####.#....####....#.#.##..#...#.###.##..#.....##........###.###.####..#.#.
.###.##..#..#...#.#.#######.##.#.#.#.#.###.#.#.#.#......#####.###.#.#.#......#.#..#.#.##..#.##.#.#.#)RawInput_");