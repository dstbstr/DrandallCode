#include "CommandParser/OptionCollection.h"
#include "CommandParser/Option.h"
#include "TestCommon.h"


constexpr char IntOptionLong[]{"intOption"};
constexpr char StrOptionLong[]{"strOption"};
constexpr char FlagOptionLong[]{"flagOption"};
constexpr char UnusedOptionLong[]{"Unused"};

constexpr char IntOptionShort[]{"i"};
constexpr char StrOptionShort[]{"s"};
constexpr char FlagOptionShort[]{"f"};
constexpr char UnusedOptionShort[]{"u"};

namespace CommandParser {
    static const OptionValuePair IntLongOVP(IntOptionLong, "1");
    static const OptionValuePair IntShortOVP(IntOptionShort, "2");
    static const OptionValuePair StrLongOVP(StrOptionLong, "hello");
    static const OptionValuePair StrShortOVP(StrOptionShort, "world");
    static const OptionValuePair FlagLongOVP(FlagOptionLong);
    static const OptionValuePair FlagShortOVP(FlagOptionShort);
    static const OptionValuePair UnusedLongOVP(UnusedOptionLong, "4");
    static const OptionValuePair UnusedShortOVP(UnusedOptionShort, "5");

    static const std::vector<OptionValuePair> LongOVPs{IntLongOVP, StrLongOVP, FlagLongOVP, UnusedLongOVP};
    static const std::vector<OptionValuePair> ShortOVPs{IntShortOVP, StrShortOVP, FlagShortOVP, UnusedShortOVP};

    class RequiredOptionCollectionTest : public ::testing::Test {
    public:
        void SetUp() override {
            m_Options.Add(m_Int)
                .Add(m_Str)
                .Add(m_Bool);
        }

    protected:
        IntOption m_Int{IntOptionShort, IntOptionLong, true};
        StringOption m_Str{StrOptionShort, StrOptionLong, true};
        BoolOption m_Bool{FlagOptionShort, FlagOptionLong, true};
        OptionCollection m_Options{"Required Options Test"};
    };

    class OptionalOptionCollectionTest : public ::testing::Test {
    public:
        void SetUp() override {
            m_Options.Add(m_Int)
                .Add(m_Str)
                .Add(m_Bool);
        }

    protected:
        OptionCollection m_Options{"Optional options test"};
        IntOption m_Int{IntOptionShort, IntOptionLong, false};
        StringOption m_Str{StrOptionShort, StrOptionLong, false};
        BoolOption m_Bool{FlagOptionShort, FlagOptionLong, false};
    };

    TEST_F(RequiredOptionCollectionTest, ParsesLongOptions) {
        auto result = m_Options.Apply(LongOVPs);

        ASSERT_EQ(result.size(), 3);
    }

    TEST_F(RequiredOptionCollectionTest, ParsesShortOptions) {
        auto result = m_Options.Apply(ShortOVPs);

        ASSERT_EQ(result.size(), 3);
    }

    TEST_F(OptionalOptionCollectionTest, ParsesLongOptions) {
        auto result = m_Options.Apply(LongOVPs);

        ASSERT_EQ(result.size(), 3);
    }

    TEST_F(OptionalOptionCollectionTest, ParsesShortOptions) {
        auto result = m_Options.Apply(ShortOVPs);

        ASSERT_EQ(result.size(), 3);
    }
} // namespace CommandParser