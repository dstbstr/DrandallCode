#include "2018/d13_MineCart.h"

#include "Facing.h"

SOLUTION(2018, 13) {
    enum struct TrackType { None, Verticle, Horizontal, CurveRight, CurveLeft, Intersection };
    constexpr TrackType ParseSpot(char spot) {
        switch (spot) {
        case '/': return TrackType::CurveRight;
        case '\\': return TrackType::CurveLeft;
        case '+': return TrackType::Intersection;
        case ' ': return TrackType::None;
        case '-': return TrackType::Horizontal;
        case '|': return TrackType::Verticle;
        case '>': return TrackType::Horizontal;
        case '<': return TrackType::Horizontal;
        case 'v': return TrackType::Verticle;
        case '^': return TrackType::Verticle;
        default: return TrackType::None;
        }
    }

    struct Cart {
        Facing CurrentFacing;
        UCoord Pos;
        Facing IntersectionAction;
        bool IsDead;
    };

    using Grid = std::vector<std::vector<TrackType>>;

    constexpr Grid ParseGrid(const std::vector<std::string>&lines) {
        Grid result;
        for (const auto& line : lines) {
            std::vector<TrackType> trackLine;
            for (auto c : line) {
                trackLine.push_back(ParseSpot(c));
            }

            result.push_back(trackLine);
        }

        return result;
    }

    constexpr std::vector<Cart> GetCarts(const std::vector<std::string>&lines) {
        std::vector<Cart> result;
        for (u32 row = 0; row < lines.size(); row++) {
            for (u32 col = 0; col < lines[row].size(); col++) {
                auto c = lines[row][col];
                if (!(c == '<' || c == '>' || c == '^' || c == 'v')) continue;

                Cart cart;
                cart.Pos = { col, row };
                cart.IntersectionAction = Facing::Left;
                cart.CurrentFacing = c == '<' ? Facing::Left :
                    c == '>' ? Facing::Right :
                    c == '^' ? Facing::Up : Facing::Down;
                cart.IsDead = false;

                result.push_back(cart);
            }
        }

        return result;
    }

    constexpr bool HasCollision(const std::vector<Cart>&carts, UCoord & collisionPos) {
        for (auto i = 0; i < carts.size(); i++) {
            for (auto j = i + 1; j < carts.size(); j++) {
                if (carts[i].Pos == carts[j].Pos && !carts[i].IsDead && !carts[j].IsDead) {
                    collisionPos = carts[i].Pos;
                    return true;
                }
            }
        }

        return false;
    }

    void PrintState(const Grid & grid, const std::vector<Cart>&carts) {
        system("cls"); //yuck
        for (u32 row = 0; row < grid.size(); row++) {
            for (u32 col = 0; col < grid[row].size(); col++) {
                auto pos = UCoord{ col, row };

                char spot = '?';
                bool hasCart = false;
                for (const auto& cart : carts) {
                    if (cart.Pos == pos) {
                        hasCart = true;
                        switch (cart.CurrentFacing) {
                        case Up: spot = '^'; break;
                        case Down: spot = 'v'; break;
                        case Left: spot = '<'; break;
                        case Right: spot = '>'; break;
                        }
                    }
                }
                if (!hasCart) {
                    switch (grid[row][col]) {
                    case TrackType::None: spot = ' '; break;
                    case TrackType::CurveLeft: spot = '\\'; break;
                    case TrackType::CurveRight: spot = '/'; break;
                    case TrackType::Intersection: spot = '+'; break;
                    case TrackType::Horizontal: spot = '-'; break;
                    case TrackType::Verticle: spot = '|'; break;
                    }
                }

                std::cout << spot;
            }
            std::cout << '\n';
        }
        std::cout << '\n';

        getchar();
    }

    constexpr void UpdateFacing(const Grid & grid, Cart & cart) {
        auto spot = grid.at(cart.Pos.Y).at(cart.Pos.X);
        if (spot == TrackType::Verticle || spot == TrackType::Horizontal) return;
        else if (spot == TrackType::CurveLeft) {
            switch (cart.CurrentFacing) {
            case Right: cart.CurrentFacing = Down; break;
            case Left: cart.CurrentFacing = Up; break;
            case Up: cart.CurrentFacing = Left; break;
            case Down: cart.CurrentFacing = Right; break;
            default: break;
            }
        }
        else if (spot == TrackType::CurveRight) {
            switch (cart.CurrentFacing) {
            case Right: cart.CurrentFacing = Up; break;
            case Left: cart.CurrentFacing = Down; break;
            case Up: cart.CurrentFacing = Right; break;
            case Down: cart.CurrentFacing = Left; break;
            default: break;
            }
        }
        else if (spot == TrackType::Intersection) {
            switch (cart.IntersectionAction) {
            case Left: cart.CurrentFacing = TurnLeft(cart.CurrentFacing); cart.IntersectionAction = Up; break;
            case Right: cart.CurrentFacing = TurnRight(cart.CurrentFacing); cart.IntersectionAction = Left; break;
            default: cart.IntersectionAction = Right; break;
            }
        }
    }

    constexpr auto SortFunc = [](const Cart& lhs, const Cart& rhs) {
        if (lhs.Pos.Y < rhs.Pos.Y) return true;
        if (lhs.Pos.Y > rhs.Pos.Y) return false;
        if (lhs.Pos.X < rhs.Pos.X) return true;
        return false;
    };

    constexpr void Tick(const Grid & grid, std::vector<Cart>&carts, UCoord & collision) {
        std::sort(carts.begin(), carts.end(), SortFunc);
        for (auto& cart : carts) {
            Move(cart.Pos, cart.CurrentFacing);
            if (HasCollision(carts, collision)) {
                break;
            }
            UpdateFacing(grid, cart);
        }
    }

    auto Part1(const std::vector<std::string>&lines) {
        auto grid = ParseGrid(lines);
        auto carts = GetCarts(lines);

        UCoord result;
        while (!HasCollision(carts, result)) {
            //PrintState(grid, carts);
            Tick(grid, carts, result);
        }

        return result;
    }

    constexpr void RemoveCrash(std::vector<Cart>&carts, UCoord pos) {
        for (auto& cart : carts) {
            if (cart.Pos == pos) {
                cart.IsDead = true;
            }
        }
    }

    constexpr size_t CountLiving(const std::vector<Cart>&carts) {
        return std::count_if(carts.cbegin(), carts.cend(), [](const Cart& cart) { return !cart.IsDead; });
    }

    constexpr void Tick2(const Grid & grid, std::vector<Cart>&carts) {
        std::sort(carts.begin(), carts.end(), SortFunc);
        UCoord collisionPos;
        for (auto& cart : carts) {
            Move(cart.Pos, cart.CurrentFacing);
            if (HasCollision(carts, collisionPos)) {
                RemoveCrash(carts, collisionPos);
            }
            UpdateFacing(grid, cart);
        }
    }


    auto Part2(const std::vector<std::string>&lines) {
        auto grid = ParseGrid(lines);
        auto carts = GetCarts(lines);

        while (CountLiving(carts) > 1) {
            Tick2(grid, carts);
        }

        for (const auto& cart : carts) {
            if (!cart.IsDead) {
                return cart.Pos;
            }
        }

        return UCoord{ 0, 0 };
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines));
        return Constexpr::ToString(Part2(lines));
    }

    bool RunTests() {
        std::vector<std::string> lines = {
            R"(/->-\        )",
            R"(|   |  /----\)",
            R"(| /-+--+-\  |)",
            R"(| | |  | v  |)",
            R"(\-+-/  \-+--/)",
            R"(  \------/   )"
        };

        if (Part1(lines) != UCoord{ 7, 3 }) return false;

        lines = {
            R"(/>-<\  )",
            R"(|   |  )",
            R"(| /<+-\)",
            R"(| | | v)",
            R"(\>+</ |)",
            R"(  |   ^)",
            R"(  \<->/)"
        };

        if (Part2(lines) != UCoord{ 6, 4 }) return false;

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