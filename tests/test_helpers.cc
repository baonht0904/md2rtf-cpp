#include "internal/helpers.h"
#include <gtest/gtest.h>

TEST(HelpersTest, StartsWith) {
    using namespace md2rtf::internal::helpers;

    EXPECT_TRUE(StartsWith("Hello, World!", "Hello"));
    EXPECT_FALSE(StartsWith("Hello, World!", "World"));
    EXPECT_TRUE(StartsWith("", ""));
    EXPECT_FALSE(StartsWith("Test", "Test123"));
}

TEST(HelpersTest, StartsWithAnyOf) {
    using namespace md2rtf::internal::helpers;

    EXPECT_TRUE(StartsWithAnyOf("Hello, World!", {"Hello", "World"}));
    EXPECT_FALSE(StartsWithAnyOf("Hello, World!", {"Goodbye", "World"}));
    EXPECT_TRUE(StartsWithAnyOf("", {})); // Empty prefixes should return false
    EXPECT_FALSE(StartsWithAnyOf("Test", {"Test123", "Test456"}));
}

TEST(HelpersTest, EndsWith) {
    using namespace md2rtf::internal::helpers;

    EXPECT_TRUE(EndsWith("Hello, World!", "World!"));
    EXPECT_FALSE(EndsWith("Hello, World!", "Hello"));
    EXPECT_TRUE(EndsWith("", ""));
    EXPECT_FALSE(EndsWith("Test", "Test123"));
}

TEST(HelpersTest, EndsWithAnyOf) {
    using namespace md2rtf::internal::helpers;

    EXPECT_TRUE(EndsWithAnyOf("Hello, World!", {"World!", "Hello"}));
    EXPECT_FALSE(EndsWithAnyOf("Hello, World!", {"Goodbye", "World"}));
    EXPECT_TRUE(EndsWithAnyOf("", {})); // Empty suffixes should return false
    EXPECT_FALSE(EndsWithAnyOf("Test", {"Test123", "Test456"}));
}

TEST(HelpersTest, Trim) {
    using namespace md2rtf::internal::helpers;

    EXPECT_EQ(Trim("   Hello, World!   "), "Hello, World!");
    EXPECT_EQ(Trim("Hello, World!"), "Hello, World!");
    EXPECT_EQ(Trim("   "), "");
    EXPECT_EQ(Trim(""), "");
    EXPECT_EQ(Trim("\n\t Hello \t\n"), "Hello");
}