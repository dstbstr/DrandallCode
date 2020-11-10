#include "IncludeCounter/Extractors/TypeDataExtractor.h"

#include "TestCommon.h"

namespace IncludeCounter {
    namespace Extractors {
        TEST(TypeExtractorTest, EmptyStructRecognizedAsType) {
            ASSERT_TRUE(TypeDataExtractor::IsAType("struct Foo {};"));
        }
        TEST(TypeExtractorTest, EmptyClassRecognizedAsType) {
            ASSERT_TRUE(TypeDataExtractor::IsAType("class Foo {};"));
        }

        TEST(TypeExtractorTest, EmptyUnionRecognizedAsType) {
            ASSERT_TRUE(TypeDataExtractor::IsAType("union Foo {};"));
        }

        TEST(TypeExtractorTest, EmptyEnumRecognizedAsType) {
            ASSERT_TRUE(TypeDataExtractor::IsAType("enum Foo {};"));
        }

        TEST(TypeExtractorTest, OpeningStructRecognizedAsType) {
            ASSERT_TRUE(TypeDataExtractor::IsAType("struct Foo {"));
        }
    } // namespace Extractors
} // namespace IncludeCounter
