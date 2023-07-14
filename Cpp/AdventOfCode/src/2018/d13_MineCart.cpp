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
        Facing CurrentFacing{ Up };
        UCoord Pos{ 0, 0 };
        Facing IntersectionAction{ Up };
        bool IsDead{ false };
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
        for (size_t row = 0; row < lines.size(); row++) {
            for (size_t col = 0; col < lines[row].size(); col++) {
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

    PART_ONE() {
        auto grid = ParseGrid(lines);
        auto carts = GetCarts(lines);

        UCoord result;
        while (!HasCollision(carts, result)) {
            Tick(grid, carts, result);
        }

        return Constexpr::ToString(result);
    }

    PART_TWO() {
        auto grid = ParseGrid(lines);
        auto carts = GetCarts(lines);

        while (CountLiving(carts) > 1) {
            Tick2(grid, carts);
        }

        for (const auto& cart : carts) {
            if (!cart.IsDead) {
                return Constexpr::ToString(cart.Pos);
            }
        }

        return "Not Found";
    }

    TESTS() {
        std::vector<std::string> lines = {
                R"(/->-\        )",
                R"(|   |  /----\)",
                R"(| /-+--+-\  |)",
                R"(| | |  | v  |)",
                R"(\-+-/  \-+--/)",
                R"(  \------/   )"
            };

        if (PartOne(lines) != "{7,3}") return false;

        lines = {
            R"(/>-<\  )",
            R"(|   |  )",
            R"(| /<+-\)",
            R"(| | | v)",
            R"(\>+</ |)",
            R"(  |   ^)",
            R"(  \<->/)"
        };

        if (PartTwo(lines) != "{6,4}") return false;

        return true;
    }
}