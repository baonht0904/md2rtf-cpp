#include "md_2_rtf.h"
#include <gtest/gtest.h>

TEST(MD2RTFTest, BasicConversion) {
    EXPECT_EQ(md2rtf::ConvertMdToRtf("Hello, World!"), "{\\rtf1\\ansi Hello, World!}");
}
TEST(MD2RTFTest, EmptyString) {
    EXPECT_EQ(md2rtf::ConvertMdToRtf(""), "{\\rtf1\\ansi }");
}