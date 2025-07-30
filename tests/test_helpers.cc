#include <gtest/gtest.h>

#include "internal/helpers.h"


TEST(HelperTest, DetermineNodeType_BlockTypes) {
    using namespace md2rtf::internal::helpers;
    using  enum md2rtf::internal::markdown_ast::NodeType;

    // Heading
    EXPECT_EQ(DetermineBlockType("# Heading"), Heading);
    EXPECT_EQ(DetermineBlockType("## Heading2"), Heading);

    // Paragraph (default)
    EXPECT_EQ(DetermineBlockType("This is a paragraph."), Paragraph);

    // Blockquote
    EXPECT_EQ(DetermineBlockType("> Blockquote"), BlockQuote);

    // Unordered List
    EXPECT_EQ(DetermineBlockType("- Item"), List);
    EXPECT_EQ(DetermineBlockType("* Item"), List);
    EXPECT_EQ(DetermineBlockType("+ Item"), List);

    // Ordered List
    EXPECT_EQ(DetermineBlockType("1. Item"), List);
    EXPECT_EQ(DetermineBlockType("2. Item"), List);

    // Code Block
    EXPECT_EQ(DetermineBlockType("```cpp\nint main() { return 0;}\n```"), CodeBlock);
    EXPECT_EQ(DetermineBlockType("~~~\nCode block content\n~~~"), CodeBlock);
    EXPECT_EQ(DetermineBlockType("    Indented code block"), CodeBlock);
    EXPECT_EQ(DetermineBlockType("\tIndented code block"), CodeBlock);

    // Horizontal Rule
    EXPECT_EQ(DetermineBlockType("---"), HorizontalRule);
    EXPECT_EQ(DetermineBlockType("***"), HorizontalRule);
    EXPECT_EQ(DetermineBlockType("___"), HorizontalRule);

    // Table
    EXPECT_EQ(DetermineBlockType("| Name | Age |"), Table);
}