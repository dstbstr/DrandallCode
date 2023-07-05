#include "2021/d19_Beacons.h"

#include <ranges>

SOLUTION(2021, 19) {
    using Beacon = Vec3<s32>;

    struct Scanner {
        std::vector<Beacon> Beacons;
        std::vector<Beacon> Scanners;
        std::vector<std::vector<Beacon>> Deltas;
    };

    constexpr Scanner ParseScanner(const std::vector<std::string>&lines) {
        Scanner result;
        for (const auto& line : lines | std::views::drop(1)) {
            auto s = Constexpr::Split(line, ",");
            Beacon b;
            Constexpr::ParseNumber(s[0], b.X);
            Constexpr::ParseNumber(s[1], b.Y);
            Constexpr::ParseNumber(s[2], b.Z);

            result.Beacons.push_back(b);
        }

        std::sort(result.Beacons.begin(), result.Beacons.end());
        return result;
    }
    static_assert(ParseScanner({ "scanner id", "1,1,1" }).Beacons.size() == 1);

    constexpr std::vector<Scanner> ParseScanners(const std::vector<std::string>&lines) {
        auto groups = SplitInputIntoGroups(lines);
        std::vector<Scanner> scanners;
        for (const auto& group : groups) {
            scanners.push_back(ParseScanner(group));
        }

        return scanners;
    }

    constexpr void ForEachBeacon(Scanner & scanner, auto toApply) {
        for (auto& beacon : scanner.Beacons) {
            toApply(beacon);
        }
    }

    constexpr Scanner FlipX(Scanner scanner) {
        ForEachBeacon(scanner, [](auto& beacon) { beacon.X = -beacon.X; });
        return scanner;
    }

    constexpr Scanner FlipY(Scanner scanner) {
        ForEachBeacon(scanner, [](auto& beacon) { beacon.Y = -beacon.Y; });
        return scanner;
    }

    constexpr Scanner FlipZ(Scanner scanner) {
        ForEachBeacon(scanner, [](auto& beacon) { beacon.Z = -beacon.Z; });
        return scanner;
    }

    constexpr Scanner SwapXY(Scanner scanner) {
        ForEachBeacon(scanner, [](auto& beacon) { std::swap(beacon.X, beacon.Y); });
        return scanner;
    }

    constexpr Scanner SwapXZ(Scanner scanner) {
        ForEachBeacon(scanner, [](auto& beacon) { std::swap(beacon.X, beacon.Z); });
        return scanner;
    }

    constexpr Scanner SwapYZ(Scanner scanner) {
        ForEachBeacon(scanner, [](auto& beacon) { std::swap(beacon.Y, beacon.Z); });
        return scanner;
    }

    constexpr std::vector<Scanner> GetAllOrientations(const Scanner & scanner) {
        std::vector<Scanner> result;
        result.push_back(scanner);                                //[X=x, Y=y, Z=z]
        result.push_back(FlipY(FlipX(scanner)));                  //[X=-x, Y=-y, Z=z]
        result.push_back(SwapYZ(FlipX(FlipY(FlipZ(scanner)))));   //[X=-x, Y=-z, Z=-y]
        result.push_back(FlipZ(FlipX(scanner)));                  //[X=-x, Y=y, Z=-z]
        result.push_back(SwapYZ(FlipX(scanner)));                 //[X=-x, Y=z, Z=y]
        result.push_back(SwapXY(FlipX(FlipY(FlipZ(scanner)))));   //[X=-y, Y=-x, Z=-z]
        result.push_back(SwapXY(SwapXZ(FlipY(FlipZ(scanner)))));  //[X=-y, Y=-z, Z=x]
        result.push_back(SwapXY(FlipY(scanner)));                 //[X=-y, Y=x, Z=z]
        result.push_back(SwapXY(SwapXZ(FlipY(FlipX(scanner)))));  //[X=-y, Y=z, Z=-x]
        result.push_back(SwapYZ(SwapXZ(FlipX(FlipZ(scanner)))));  //[X=-z, Y=-x, Z=y]
        result.push_back(SwapXZ(FlipX(FlipY(FlipZ(scanner)))));   //[X=-z, Y=-y, Z=-x]
        result.push_back(SwapXZ(SwapXY(FlipZ(FlipY(scanner)))));  //[X=-z, Y=x, Z=-y]
        result.push_back(SwapXZ(FlipZ(scanner)));                 //[X=-z, Y=y, Z=x]
        result.push_back(FlipY(FlipZ(scanner)));                  //[X=x, Y=-y, Z=-z]
        result.push_back(SwapYZ(FlipZ(scanner)));                 //[X=x, Y=-z, Z=y]
        result.push_back(SwapYZ(FlipY(scanner)));                 //[X=x, Y=z, Z=-y]
        result.push_back(SwapXY(FlipX(scanner)));                 //[X=y, Y=-x, Z=z]
        result.push_back(SwapXY(SwapXZ(FlipX(FlipZ(scanner)))));  //[X=y, Y=-z, Z=-x]
        result.push_back(SwapXY(FlipZ(scanner)));                 //[X=y, Y=x, Z=-z]
        result.push_back(SwapXY(SwapXZ(scanner)));                //[X=y, Y=z, Z=x]
        result.push_back(SwapYZ(SwapXZ(FlipX(FlipY(scanner)))));  //[X=z, Y=-x, Z=-y]
        result.push_back(SwapXZ(FlipY(scanner)));                 //[X=z, Y=-y, Z=x]
        result.push_back(SwapXZ(SwapXY(scanner)));                //[X=z, Y=x, Z=y]
        result.push_back(SwapXZ(FlipX(scanner)));                 //[X=z, Y=y, Z=-x]

        for (auto& s : result) {
            std::sort(s.Beacons.begin(), s.Beacons.end());
        }
        return result;
    }

    constexpr std::vector<Vec3<s32>> GetDeltas(const std::vector<Vec3<s32>>&beacons, size_t beaconIndex) {
        std::vector<Vec3<s32>> result;

        for (int offset = -20; offset < 21; offset++) {
            if (offset == 0) continue;
            if (beaconIndex + offset >= 0 && beaconIndex + offset < beacons.size() - 1) {
                result.push_back(beacons[beaconIndex] - beacons[beaconIndex + offset]);
            }
        }
        return result;
    }

    constexpr bool DoesMatch(const std::vector<Vec3<s32>>&lhsDeltas, const std::vector<Vec3<s32>>&rhsDeltas) {
        for (const auto& lhsDelta : lhsDeltas) {
            for (const auto& rhsDelta : rhsDeltas) {
                if (lhsDelta == rhsDelta) return true;
            }
        }
        return false;
    }

    constexpr size_t CountMatchs(const Scanner & lhs, const Scanner & rhs) {
        size_t matches = 0;
        for (const auto& lhsDeltas : lhs.Deltas) {
            for (const auto& rhsDeltas : rhs.Deltas) {
                if (DoesMatch(lhsDeltas, rhsDeltas)) {
                    matches++;
                    break;
                }
            }
        }

        return matches;
    }


    constexpr void SetDeltas(Scanner & scanner) {
        scanner.Deltas.clear();
        for (size_t i = 0; i < scanner.Beacons.size(); i++) {
            scanner.Deltas.push_back(GetDeltas(scanner.Beacons, i));
        }
    }


    constexpr void AlignBeacons(const Scanner & all, Scanner & scanner) {
        Vec3<s32> offset;
        for (size_t i = 0; i < scanner.Deltas.size(); i++) {
            for (size_t j = 0; j < all.Deltas.size(); j++) {
                if (DoesMatch(scanner.Deltas[i], all.Deltas[j])) {
                    offset = all.Beacons[j] - scanner.Beacons[i];
                    break;
                }
            }
        }
        scanner.Scanners.push_back(offset);

        for (auto& beacon : scanner.Beacons) {
            beacon += offset;
        }
    }

    constexpr bool CanBePlaced(const Scanner & all, const Scanner & scanner, Scanner & outResult) {
        auto orientations = GetAllOrientations(scanner);
        for (auto& orientation : orientations) {
            SetDeltas(orientation);
            auto matchCount = CountMatchs(all, orientation);

            if (matchCount >= 12) {
                outResult = orientation;
                AlignBeacons(all, outResult);
                return true;
            }
        }
        return false;
    }

    constexpr void AddMissingBeacons(Scanner & all, const Scanner & scanner) {
        for (const auto& beacon : scanner.Beacons) {
            if (std::find(all.Beacons.begin(), all.Beacons.end(), beacon) == all.Beacons.end()) {
                all.Beacons.push_back(beacon);
            }
        }

        std::sort(all.Beacons.begin(), all.Beacons.end());
        SetDeltas(all);
        all.Scanners.push_back(scanner.Scanners[0]);
    }

    constexpr Scanner BuildScannerMap(const std::vector<std::string>&lines) {
        auto scanners = ParseScanners(lines);
        scanners[0].Scanners.push_back({ 0, 0, 0 });
        Scanner all;
        AddMissingBeacons(all, scanners[0]);

        std::vector<size_t> seenIndexes = { 0 };
        while (seenIndexes.size() < scanners.size()) {
            auto startSize = seenIndexes.size();
            for (size_t i = 0; i < scanners.size(); i++) {
                if (std::find(seenIndexes.cbegin(), seenIndexes.cend(), i) != seenIndexes.cend()) continue;
                Scanner scanner;
                if (CanBePlaced(all, scanners[i], scanner)) {
                    AddMissingBeacons(all, scanner);
                    seenIndexes.push_back(i);
                }
            }
            if (startSize == seenIndexes.size()) {
                throw "Made no progress";
            }
        }

        return all;
    }

    constexpr auto Part1(const std::vector<std::string>&lines) {
        auto all = BuildScannerMap(lines);
        return all.Beacons.size();
    }

    constexpr auto Part2(const std::vector<std::string>&lines) {
        auto all = BuildScannerMap(lines);

        size_t best = 0;
        for (size_t i = 0; i < all.Scanners.size(); i++) {
            for (size_t j = i + 1; j < all.Scanners.size(); j++) {
                best = std::max(best, MDistance(all.Scanners[i], all.Scanners[j]));
            }
        }

        return best;
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines));
        return Constexpr::ToString(Part2(lines));
    }

    bool RunTests() {
        std::vector<std::string> lines = {
            "--- scanner 0 ---",
            "404,-588,-901",
            "528,-643,409",
            "-838,591,734",
            "390,-675,-793",
            "-537,-823,-458",
            "-485,-357,347",
            "-345,-311,381",
            "-661,-816,-575",
            "-876,649,763",
            "-618,-824,-621",
            "553,345,-567",
            "474,580,667",
            "-447,-329,318",
            "-584,868,-557",
            "544,-627,-890",
            "564,392,-477",
            "455,729,728",
            "-892,524,684",
            "-689,845,-530",
            "423,-701,434",
            "7,-33,-71",
            "630,319,-379",
            "443,580,662",
            "-789,900,-551",
            "459,-707,401",
            "",
            "--- scanner 1 ---",
            "686,422,578",
            "605,423,415",
            "515,917,-361",
            "-336,658,858",
            "95,138,22",
            "-476,619,847",
            "-340,-569,-846",
            "567,-361,727",
            "-460,603,-452",
            "669,-402,600",
            "729,430,532",
            "-500,-761,534",
            "-322,571,750",
            "-466,-666,-811",
            "-429,-592,574",
            "-355,545,-477",
            "703,-491,-529",
            "-328,-685,520",
            "413,935,-424",
            "-391,539,-444",
            "586,-435,557",
            "-364,-763,-893",
            "807,-499,-711",
            "755,-354,-619",
            "553,889,-390",
            "",
            "--- scanner 2 ---",
            "649,640,665",
            "682,-795,504",
            "-784,533,-524",
            "-644,584,-595",
            "-588,-843,648",
            "-30,6,44",
            "-674,560,763",
            "500,723,-460",
            "609,671,-379",
            "-555,-800,653",
            "-675,-892,-343",
            "697,-426,-610",
            "578,704,681",
            "493,664,-388",
            "-671,-858,530",
            "-667,343,800",
            "571,-461,-707",
            "-138,-166,112",
            "-889,563,-600",
            "646,-828,498",
            "640,759,510",
            "-630,509,768",
            "-681,-892,-333",
            "673,-379,-804",
            "-742,-814,-386",
            "577,-820,562",
            "",
            "--- scanner 3 ---",
            "-589,542,597",
            "605,-692,669",
            "-500,565,-823",
            "-660,373,557",
            "-458,-679,-417",
            "-488,449,543",
            "-626,468,-788",
            "338,-750,-386",
            "528,-832,-391",
            "562,-778,733",
            "-938,-730,414",
            "543,643,-506",
            "-524,371,-870",
            "407,773,750",
            "-104,29,83",
            "378,-903,-323",
            "-778,-728,485",
            "426,699,580",
            "-438,-605,-362",
            "-469,-447,-387",
            "509,732,623",
            "647,635,-688",
            "-868,-804,481",
            "614,-800,639",
            "595,780,-596",
            "",
            "--- scanner 4 ---",
            "727,592,562",
            "-293,-554,779",
            "441,611,-461",
            "-714,465,-776",
            "-743,427,-804",
            "-660,-479,-426",
            "832,-632,460",
            "927,-485,-438",
            "408,393,-506",
            "466,436,-512",
            "110,16,151",
            "-258,-428,682",
            "-393,719,612",
            "-211,-452,876",
            "808,-476,-593",
            "-575,615,604",
            "-485,667,467",
            "-680,325,-822",
            "-627,-443,-432",
            "872,-547,-609",
            "833,512,582",
            "807,604,487",
            "839,-516,451",
            "891,-625,532",
            "-652,-548,-490",
            "30,-46,-14"
        };

        if (Part1(lines) != 79) return false;
        if (Part2(lines) != 3621) return false;
        return true;
    }

    PART_ONE() {
        return lines[0];
    }

    PART_TWO() {
        return lines[0];
    }

    TESTS() {
        return true;
    }
}