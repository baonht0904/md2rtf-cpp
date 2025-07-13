// C++
#include "internal/markdown_ast.h"
#include <gtest/gtest.h>

using namespace md2rtf::internal::markdown_ast;

// Headings
TEST(MD2AST_DETERMINE_NODE_TYPE, Heading) {
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("# Heading 1"), NodeType::Heading);
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("## Heading 2"), NodeType::Heading);
}

// Paragraphs
TEST(MD2AST_DETERMINE_NODE_TYPE, Paragraph) {
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("Some plain text."), NodeType::Paragraph);
    EXPECT_EQ(AstNodeFactory::DetermineNodeType(""), NodeType::Paragraph);
}

// Lists
TEST(MD2AST_DETERMINE_NODE_TYPE, List) {
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("- Item 1"), NodeType::List);
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("* Item 2"), NodeType::List);
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("+ Item 3"), NodeType::List);
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("1. Item 1"), NodeType::List);
}

// Horizontal rules
TEST(MD2AST_DETERMINE_NODE_TYPE, HorizontalRule) {
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("---"), NodeType::HorizontalRule);
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("___"), NodeType::HorizontalRule);
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("***"), NodeType::HorizontalRule);
}

// Blocks
TEST(MD2AST_DETERMINE_NODE_TYPE, Block) {
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("| Header 1 | Header 2 |\n| --- | --- |\n| Cell 1 | Cell 2 |"), NodeType::Table);
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("> Blockquote"), NodeType::Blockquote);
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("```cpp\nint main() {}\n```"), NodeType::CodeBlock);
}

// Inline-level
TEST(MD2AST_DETERMINE_NODE_TYPE, Inline) {
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("*Italic text*"), NodeType::Emphasis);
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("_Italic text_"), NodeType::Emphasis);
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("**Bold text**"), NodeType::Emphasis);
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("__Bold text__"), NodeType::Emphasis);
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("~~Strikethrough~~"), NodeType::Emphasis);
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("`inline code`"), NodeType::InlineCode);
}

// Links and images
TEST(MD2AST_DETERMINE_NODE_TYPE, LinkAndImage) {
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("[Link text](http://example.com)"), NodeType::Link);
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("![Alt text](image.png)"), NodeType::Image);
}