#include "md_2_rtf/md_2_rtf.h"
#include <gtest/gtest.h>

TEST(MD2RTFTest, BasicConversion) {
    EXPECT_EQ(convert_md_to_rtf("Hello, World!"), "{\\rtf1\\ansi Hello, World!}");
}
TEST(MD2RTFTest, EmptyString) {
    EXPECT_EQ(convert_md_to_rtf(""), "{\\rtf1\\ansi }");
}