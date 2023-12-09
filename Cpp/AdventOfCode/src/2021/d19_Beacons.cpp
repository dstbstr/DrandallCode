#include "2021/d19_Beacons.h"

#include <ranges>

SOLUTION(2021, 19) {
    using Beacon = Vec3<s32>;
    using RotationMatrix = std::array<std::array<s32, 3>, 3>;

    constexpr std::array<RotationMatrix, 24> GetRotations() {
        std::array<RotationMatrix, 24> result{};
        result[0] = { { {1, 0, 0}, { 0,1,0 }, { 0,0,1 }} };
        result[1] = { { {1, 0, 0}, { 0,0,-1 }, { 0,1,0 } } };
        result[2] = { { {1, 0, 0}, { 0,-1,0 }, { 0,0,-1 } } };
        result[3] = { { {1, 0, 0}, { 0,0,1 }, { 0, -1, 0 }} };
        result[4] = { {{0,-1,0},{1,0,0},{0,0,1}} };
        result[5] = { {{0,0,1},{1,0,0},{0,1,0}} };
        result[6] = { {{0,1,0},{1,0,0},{0,0,-1}} };
        result[7] = { {{0,0,-1},{1,0,0},{0,-1,0}} };
        result[8] = { {{-1,0,0},{0,-1,0},{0,0,1}} };
        result[9] = { {{-1,0,0},{0,0,-1},{0,-1, 0}} };
        result[10] = { {{-1,0,0},{0,1,0},{0,0,-1}} };
        result[11] = { {{-1,0,0},{0,0,1},{0,1,0}} };
        result[12] = { {{0,1,0},{-1,0,0},{0,0,1}} };
        result[13] = { {{0,0,1},{-1,0,0},{0,-1,0}} };
        result[14] = { {{0,-1,0},{-1,0,0},{0,0,-1}} };
        result[15] = { {{0,0,-1},{-1,0,0},{0,1,0}} };
        result[16] = { {{0,0,-1},{0,1,0},{1,0,0}} };
        result[17] = { {{0,1,0},{0,0,1},{1,0,0}} };
        result[18] = { {{0,0,1},{0,-1,0},{1,0,0}} };
        result[19] = { {{0,-1,0},{0,0,-1},{1,0,0}} };
        result[20] = { {{0,0,-1},{0,-1,0},{-1,0,0}} };
        result[21] = { {{0,-1,0},{0,0,1},{-1,0,0}} };
        result[22] = { {{0,0,1},{0,1,0},{-1,0,0}} };
        result[23] = { {{0,1,0},{0,0,-1},{-1,0,0}} };
        return result;
    }

    constexpr std::array<RotationMatrix, 24> Rotations = GetRotations();

    using BeaconPair = std::pair<Beacon, Beacon>;

    struct Scanner {
        std::vector<Beacon> Beacons;
        Constexpr::SmallMap<size_t, BeaconPair> DeltaMap;
        size_t id{ 0 };

        constexpr bool operator==(const Scanner& other) const {
            return Beacons == other.Beacons;
        }
        constexpr bool operator<(const Scanner& other) const {
            return Beacons[0] < other.Beacons[0];
        }
    };

    constexpr size_t GetDistance(const Beacon& lhs, const Beacon& rhs) {
        auto delta = lhs - rhs;
        delta *= delta;
        return delta.X + delta.Y + delta.Z;
    }

    constexpr void UpdateDeltaMap(Scanner& scanner) {
        scanner.DeltaMap.clear();
        for (size_t i = 0; i < scanner.Beacons.size(); i++) {
            for (size_t j = i + 1; j < scanner.Beacons.size(); j++) {
                auto lhs = scanner.Beacons[i];
                auto rhs = scanner.Beacons[j];
                auto d = GetDistance(lhs, rhs);
                scanner.DeltaMap[d] = std::make_pair(lhs, rhs);
            }
        }
    }

    constexpr void UpdateDeltaMap(Constexpr::BigMap<size_t, BeaconPair>& allDeltas, const std::vector<Beacon>& oldBeacons, const std::vector<Beacon>& newBeacons) {
        for (const auto& old : oldBeacons) {
            for (const auto& added : newBeacons) {
                auto d = GetDistance(old, added);
                allDeltas[d] = std::make_pair(old, added);
            }
        }

        for (size_t i = 0; i < newBeacons.size(); i++) {
            for (size_t j = i + 1; j < newBeacons.size(); j++) {
                auto lhs = newBeacons[i];
                auto rhs = newBeacons[j];
                auto d = GetDistance(lhs, rhs);
                allDeltas[d] = std::make_pair(lhs, rhs);
            }
        }
    }

    constexpr Scanner ParseScanner(const auto& lines) {
        Scanner result;
        for (const auto& line : lines | std::views::drop(1)) {
            result.Beacons.push_back(Beacon(line));
        }
        UpdateDeltaMap(result);
        return result;
    }

    constexpr size_t CountOverlap(const Constexpr::BigMap<size_t, BeaconPair>& allDeltas, const Scanner& toAdd) {
        auto keys = toAdd.DeltaMap.GetKeys();
        return std::count_if(keys.begin(), keys.end(), [&](size_t key) { return allDeltas.contains(key); });
    }

    constexpr bool FindScannerLocation(const BeaconPair& lhs, const BeaconPair& rhs, Beacon& outDelta) {
        if (lhs.first - rhs.first == lhs.second - rhs.second) {
            outDelta = lhs.first - rhs.first;
            return true;
        }
        else if (lhs.first - rhs.second == lhs.second - rhs.first) {
            outDelta = lhs.first - rhs.second;
            return true;
        }
        return false;
    }

    constexpr void Align(Constexpr::BigMap<size_t, BeaconPair>& allDeltas, std::vector<Beacon>& allBeacons, std::vector<Beacon>& allScanners, const Scanner& toAdd) {
        auto keys = toAdd.DeltaMap.GetKeys();
        BeaconPair a;
        BeaconPair b;
        BeaconPair aligned;
        Beacon translate;
        for (auto key : keys) {
            if (allDeltas.contains(key)) {
                a = allDeltas.at(key);
                b = toAdd.DeltaMap.at(key);
                for (const auto& rotation : Rotations) {
                    aligned = std::make_pair(DotProduct(b.first, rotation), DotProduct(b.second, rotation));
                    if (FindScannerLocation(a, aligned, translate)) {                       
                        std::vector<Beacon> transformed;
                        std::transform(toAdd.Beacons.begin(), toAdd.Beacons.end(), std::back_inserter(transformed), [rotation, translate](const Beacon& b) {
                            return DotProduct(b, rotation) + translate;
                            });
                        std::erase_if(transformed, [&](const Beacon& b) {
                            return std::find(allBeacons.begin(), allBeacons.end(), b) != allBeacons.end();
                            });
                        UpdateDeltaMap(allDeltas, allBeacons, transformed);
                        std::copy(transformed.begin(), transformed.end(), std::back_inserter(allBeacons));
                        allScanners.push_back(translate);
                        return;
                    }
                }
            }
        }
    }

    constexpr auto CombineScanners(const auto& lines) {
        auto groups = SplitInputIntoGroups(lines);
        std::vector<Scanner> all;
        for (const auto& group : groups) {
            auto scanner = ParseScanner(group);
            scanner.id = all.size();
            all.push_back(scanner);
        }

        Constexpr::BigMap<size_t, BeaconPair> allDeltas;
        for (const auto& [key, value] : all[0].DeltaMap) {
            allDeltas[key] = value;
        }
        std::vector<Beacon> allBeacons {all[0].Beacons};
        std::vector<Beacon> allScanners {{0, 0, 0}};
        std::vector<size_t> alignedIds {0};

        while (allScanners.size() < all.size()) {
            for (const auto& foo : all) {
                if (std::find(alignedIds.begin(), alignedIds.end(), foo.id) != alignedIds.end()) continue;
                if (CountOverlap(allDeltas, foo) >= 12) {
                    Align(allDeltas, allBeacons, allScanners, foo);
                    alignedIds.push_back(foo.id);
                }
            }
        }
        return std::make_pair(allScanners, allBeacons);
    }

    constexpr size_t SolvePartOne(const auto& lines) {
        auto [scanners, beacons] = CombineScanners(lines);
        return beacons.size();
    }

    PART_ONE() {
        return Constexpr::ToString(SolvePartOne(lines));
    }

    constexpr size_t SolvePartTwo(const auto& lines) {
        auto [scanners, beacons] = CombineScanners(lines);

        size_t best = 0;
        for (size_t i = 0; i < scanners.size(); i++) {
            for (size_t j = i + 1; j < scanners.size(); j++) {
                best = std::max(best, MDistance(scanners[i], scanners[j]));
            }
        }

        return best;
    }

    PART_TWO() {
        return Constexpr::ToString(SolvePartTwo(lines));
    }

    TESTS() {
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

        if (SolvePartOne(lines) != 79) return false;
        if (SolvePartTwo(lines) != 3621) return false;
        return true;
    }
}