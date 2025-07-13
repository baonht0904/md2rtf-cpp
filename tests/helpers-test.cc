#include "internal/helpers.h"
#include <gtest/gtest.h>

TEST(HelpersTest, IsStartWith) {
    using namespace md2rtf::internal::helpers;

    EXPECT_TRUE(IsStartWith("Hello, World!", "Hello"));
    EXPECT_FALSE(IsStartWith("Hello, World!", "World"));
    EXPECT_TRUE(IsStartWith("", ""));
    EXPECT_FALSE(IsStartWith("Test", "Test123"));
}

TEST(HelpersTest, IsEndWith) {
    using namespace md2rtf::internal::helpers;

    EXPECT_TRUE(IsEndWith("Hello, World!", "World!"));
    EXPECT_FALSE(IsEndWith("Hello, World!", "Hello"));
    EXPECT_TRUE(IsEndWith("", ""));
    EXPECT_FALSE(IsEndWith("Test", "Test123"));
}

TEST(HelpersTest, Trim) {
    using namespace md2rtf::internal::helpers;

    EXPECT_EQ(Trim("   Hello, World!   "), "Hello, World!");
    EXPECT_EQ(Trim("Hello, World!"), "Hello, World!");
    EXPECT_EQ(Trim("   "), "");
    EXPECT_EQ(Trim(""), "");
    EXPECT_EQ(Trim("\n\t Hello \t\n"), "Hello");
}