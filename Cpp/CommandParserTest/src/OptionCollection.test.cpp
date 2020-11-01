#include "CommandParser/OptionCollection.h"

#include "CommandParser/Option.h"
#include "TestCommon.h"

using ::testing::HasSubstr;

constexpr char IntOptionLong[]{"intOption"};
constexpr char StrOptionLong[]{"strOption"};
constexpr char FlagOptionLong[]{"flagOption"};
constexpr char UnusedOptionLong[]{"Unused"};

constexpr char IntOptionShort[]{"i"};
constexpr char StrOptionShort[]{"s"};
constexpr char FlagOptionShort[]{"f"};
constexpr char UnusedOptionShort[]{"z"};

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
            m_Options.Add(m_Int).Add(m_Str).Add(m_Bool);
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
            m_Options.Add(m_Int).Add(m_Str).Add(m_Bool);
        }

    protected:
        OptionCollection m_Options{"Optional options test"};
        IntOption m_Int{IntOptionShort, IntOptionLong, false};
        StringOption m_Str{StrOptionShort, StrOptionLong, false};
        BoolOption m_Bool{FlagOptionShort, FlagOptionLong, false};
    };

    TEST_F(RequiredOptionCollectionTest, ParsesLongOptions) {
        m_Options.Apply(LongOVPs);
        ASSERT_TRUE(m_Int.IsPopulated());
        ASSERT_TRUE(m_Str.IsPopulated());
        ASSERT_TRUE(m_Bool.IsPopulated());
    }

    TEST_F(RequiredOptionCollectionTest, ParsesShortOptions) {
        m_Options.Apply(ShortOVPs);
        ASSERT_TRUE(m_Int.IsPopulated());
        ASSERT_TRUE(m_Str.IsPopulated());
        ASSERT_TRUE(m_Bool.IsPopulated());
    }

    TEST_F(RequiredOptionCollectionTest, PrintsUsage) {
        std::stringstream ss;
        m_Options.PrintUsage(ss);
        auto result = ss.str();

        ASSERT_THAT(result, HasSubstr(IntOptionShort));
        ASSERT_THAT(result, HasSubstr(IntOptionLong));
        ASSERT_THAT(result, HasSubstr(StrOptionShort));
        ASSERT_THAT(result, HasSubstr(StrOptionLong));
        ASSERT_THAT(result, HasSubstr(FlagOptionShort));
        ASSERT_THAT(result, HasSubstr(FlagOptionLong));
        ASSERT_THAT(result, Not(HasSubstr(UnusedOptionShort)));
        ASSERT_THAT(result, Not(HasSubstr(UnusedOptionLong)));
    }

    TEST_F(RequiredOptionCollectionTest, ThrowsIfRequiredOptionIsMissing) {
        std::vector<OptionValuePair> args{IntShortOVP, StrShortOVP};
        ASSERT_ANY_THROW(m_Options.Apply(args));
    }

    TEST_F(OptionalOptionCollectionTest, ParsesLongOptions) {
        m_Options.Apply(LongOVPs);
        ASSERT_TRUE(m_Int.IsPopulated());
        ASSERT_TRUE(m_Str.IsPopulated());
        ASSERT_TRUE(m_Bool.IsPopulated());
    }

    TEST_F(OptionalOptionCollectionTest, ParsesShortOptions) {
        m_Options.Apply(ShortOVPs);
        ASSERT_TRUE(m_Int.IsPopulated());
        ASSERT_TRUE(m_Str.IsPopulated());
        ASSERT_TRUE(m_Bool.IsPopulated());
    }

    TEST_F(OptionalOptionCollectionTest, DoesNotThrowIfOptionIsMissing) {
        std::vector<OptionValuePair> args{IntShortOVP, StrShortOVP};
        ASSERT_NO_THROW(m_Options.Apply(args));
    }

    TEST_F(OptionalOptionCollectionTest, OnlyPopulatesProvidedOptions) {
        std::vector<OptionValuePair> args{IntShortOVP, StrShortOVP};
        m_Options.Apply(args);

        ASSERT_TRUE(m_Int.IsPopulated());
        ASSERT_TRUE(m_Str.IsPopulated());
        ASSERT_FALSE(m_Bool.IsPopulated());
    }

    TEST(OptionCollectionTest, ExtractsValuesFromBoolOptions) {
        BoolOption opt("a", "", false);
        VecStringOption restOpt(false);
        OptionValuePair pair("a", "foo");
        OptionCollection collection("Extracting value from bool option");
        collection.Add(opt).Add(restOpt);

        collection.Apply(std::vector<OptionValuePair>{pair});

        ASSERT_TRUE(opt.IsPopulated());
        ASSERT_TRUE(restOpt.IsPopulated());
        ASSERT_EQ(restOpt.GetValue().size(), 1);
        ASSERT_EQ(restOpt.GetValue()[0], "foo");
    }

} // namespace CommandParser