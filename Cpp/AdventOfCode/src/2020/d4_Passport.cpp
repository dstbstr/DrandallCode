#include "2020/d4_Passport.h"

SOLUTION(2020, 4) {
    struct Passport {
        std::string BirthYear;
        std::string IssueYear;
        std::string ExpirationYear;
        std::string PassportId;
        std::string CountryId;
        std::string Height;
        std::string HairColor;
        std::string EyeColor;

        std::ostream& operator<<(std::ostream& stream) const {
            stream << "Birth Year: " << (BirthYear.empty() ? "(Missing)" : BirthYear) << "\n";
            stream << "Issue Year: " << (IssueYear.empty() ? "(Missing)" : IssueYear) << "\n";
            stream << "Expiration Year: " << (ExpirationYear.empty() ? "(Missing)" : ExpirationYear) << "\n";
            stream << "Passport Id: " << (PassportId.empty() ? "(Missing)" : PassportId) << "\n";
            stream << "Country Id: " << (CountryId.empty() ? "(Missing)" : CountryId) << "\n";
            stream << "Height: " << (Height.empty() ? "(Missing)" : Height) << "\n";
            stream << "Hair Color: " << (HairColor.empty() ? "(Missing)" : HairColor) << "\n";
            stream << "Eye Color: " << (EyeColor.empty() ? "(Missing)" : EyeColor) << "\n";
            stream << "\n";
            return stream;
        }
    };

    constexpr void TryParseField(std::string_view field, Passport & passport) {
        auto split = Constexpr::Split(field, ":");
        auto key = split[0];
        auto value = std::string(split[1]);

        if (key == "byr") {
            passport.BirthYear = value;
        }
        else if (key == "iyr") {
            passport.IssueYear = value;
        }
        else if (key == "eyr") {
            passport.ExpirationYear = value;
        }
        else if (key == "pid") {
            passport.PassportId = value;
        }
        else if (key == "cid") {
            passport.CountryId = value;
        }
        else if (key == "hgt") {
            passport.Height = value;
        }
        else if (key == "hcl") {
            passport.HairColor = value;
        }
        else if (key == "ecl") {
            passport.EyeColor = value;
        }
    }

    constexpr Passport ParsePassport(const std::vector<std::string>&lines) {
        Passport passport;

        for (const auto& line : lines) {
            auto split = Constexpr::Split(line, " ");
            for (const auto& field : split) {
                TryParseField(field, passport);
            }
        }

        return passport;
    }

    constexpr bool ValidatePassport(const Passport & passport) {
        return(!(
            passport.EyeColor.empty() ||
            passport.PassportId.empty() ||
            passport.ExpirationYear.empty() ||
            passport.HairColor.empty() ||
            passport.BirthYear.empty() ||
            passport.IssueYear.empty() ||
            passport.Height.empty()));
    }

    constexpr bool VerifyPassport(const Passport & passport) {
        size_t birthYear, issueYear, expirationYear, id;
        if (!Constexpr::ParseNumber(passport.BirthYear, birthYear)) return false;
        if (!Constexpr::ParseNumber(passport.IssueYear, issueYear)) return false;
        if (!Constexpr::ParseNumber(passport.ExpirationYear, expirationYear)) return false;
        if (!Constexpr::ParseNumber(passport.PassportId, id)) return false;
        if (birthYear < 1920 || birthYear > 2002) return false;
        if (issueYear < 2010 || issueYear > 2020) return false;
        if (expirationYear < 2020 || expirationYear > 2030) return false;
        if (passport.PassportId.size() != 9) return false;

        auto units = passport.Height.substr(passport.Height.size() - 2);
        if (units != "in" && units != "cm") return false;
        size_t height;
        if (!Constexpr::ParseNumber(passport.Height.substr(0, passport.Height.size() - 2), height)) return false;
        if (units == "in") {
            if (height < 59 || height > 76) return false;
        }
        else {
            if (height < 150 || height > 193) return false;
        }

        if (passport.HairColor[0] != '#') return false;
        if (passport.HairColor.size() != 7) return false;
        for (auto i = 1; i < passport.HairColor.size(); i++) {
            auto c = passport.HairColor[i];
            if (c >= 'a' && c <= 'f') continue;
            if (c >= '0' && c <= '9') continue;
            return false;
        }

        std::vector<std::string> validEyeColors = {
            "amb", "blu", "brn", "gry", "grn", "hzl", "oth"
        };
        if (std::find(validEyeColors.cbegin(), validEyeColors.cend(), passport.EyeColor) == validEyeColors.cend()) return false;

        return true;
    }

    PART_ONE() {
        auto groups = SplitInputIntoGroups(lines);
        return Constexpr::ToString(std::count_if(groups.begin(), groups.end(), [](const auto& pLines) {
            return ValidatePassport(ParsePassport(pLines));
            }));
    }

    PART_TWO() {
        auto groups = SplitInputIntoGroups(lines);
        return Constexpr::ToString(std::count_if(groups.begin(), groups.end(), [](const auto& pLines) {
            auto p = ParsePassport(pLines);
            return ValidatePassport(p) && VerifyPassport(p);
        }));
    }

    constexpr bool TestParsePassport() {
        std::vector<std::string> lines = {
            "ecl:gry pid:860033327 eyr:2020 hcl:#fffffd",
            "byr:1937 iyr:2017 cid:147 hgt:183cm"
        };

        auto passport = ParsePassport(lines);
        if (passport.EyeColor != "gry") return false;
        if (passport.PassportId != "860033327") return false;
        if (passport.ExpirationYear != "2020") return false;
        if (passport.HairColor != "#fffffd") return false;
        if (passport.BirthYear != "1937") return false;
        if (passport.IssueYear != "2017") return false;
        if (passport.CountryId != "147") return false;
        if (passport.Height != "183cm") return false;
        return true;
    }

    constexpr bool TestValidatePassport() {
        std::vector<std::string> lines = {
            "hcl:#ae17e1 iyr:2013",
            "eyr:2024",
            "ecl:brn pid:760753108 byr:1931",
            "hgt:179cm"
        };

        return ValidatePassport(ParsePassport(lines));
    }

    TESTS() {
        static_assert(TestParsePassport());
        static_assert(TestValidatePassport());

        std::vector<std::string> lines = {
            "pid:087499704 hgt:74in ecl:grn iyr:2012 eyr:2030 byr:1980",
            "hcl:#623a2f"
        };

        if (PartTwo(lines) != "1") return false;

        return true;
    }
}