#include "internal/markdown_ast.h"
#include <gtest/gtest.h>

using namespace md2rtf::internal::markdown_ast;

TEST(AstNodeFactoryTest, DetermineNodeType_BlockTypes) {
    // Heading
    EXPECT_EQ(AstNodeFactory::DetermineBlockNodeType("# Heading"), NodeType::Heading);
    EXPECT_EQ(AstNodeFactory::DetermineBlockNodeType("## Heading2"), NodeType::Heading);

    // Paragraph (default)
    EXPECT_EQ(AstNodeFactory::DetermineBlockNodeType("This is a paragraph."), NodeType::Paragraph);

    // Blockquote
    EXPECT_EQ(AstNodeFactory::DetermineBlockNodeType("> Blockquote"), NodeType::BlockQuote);

    // Unordered List
    EXPECT_EQ(AstNodeFactory::DetermineBlockNodeType("- Item"), NodeType::List);
    EXPECT_EQ(AstNodeFactory::DetermineBlockNodeType("* Item"), NodeType::List);
    EXPECT_EQ(AstNodeFactory::DetermineBlockNodeType("+ Item"), NodeType::List);

    // Ordered List
    EXPECT_EQ(AstNodeFactory::DetermineBlockNodeType("1. Item"), NodeType::List);
    EXPECT_EQ(AstNodeFactory::DetermineBlockNodeType("2. Item"), NodeType::List);

    // Code Block
    EXPECT_EQ(AstNodeFactory::DetermineBlockNodeType("```cpp\nint main() { return 0;}\n```"), NodeType::CodeBlock);

    // Horizontal Rule
    EXPECT_EQ(AstNodeFactory::DetermineBlockNodeType("---"), NodeType::HorizontalRule);
    EXPECT_EQ(AstNodeFactory::DetermineBlockNodeType("***"), NodeType::HorizontalRule);
    EXPECT_EQ(AstNodeFactory::DetermineBlockNodeType("___"), NodeType::HorizontalRule);

    // Table
    EXPECT_EQ(AstNodeFactory::DetermineBlockNodeType("| Name | Age |"), NodeType::Table);
}

