#include "internal/markdown_ast.h"
#include <gtest/gtest.h>

TEST(MD2AST_BASIC_TEST, DetermineNodeType) {
    using namespace md2rtf::internal::markdown_ast;

    // Headings
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("# Heading 1"), NodeType::Heading);
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("## Heading 2"), NodeType::Heading);

    // Paragraphs
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("Some plain text."), NodeType::Paragraph);
    EXPECT_EQ(AstNodeFactory::DetermineNodeType(""), NodeType::Paragraph);
    
    // Lists
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("- Item 1"), NodeType::List);
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("* Item 2"), NodeType::List);
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("+ Item 3"), NodeType::List);
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("1. Item 1"), NodeType::List);
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("> Blockquote"), NodeType::Blockquote);
    
    // Horizontal rules
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("---"), NodeType::HorizontalRule);
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("___"), NodeType::HorizontalRule);
    
    // Tables
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("| Header 1 | Header 2 |\n| --- | --- |\n| Cell 1 | Cell 2 |"), NodeType::Table);
    
    // Code
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("```cpp\nint main() {}\n```"), NodeType::CodeBlock);

    // Inline-level
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("*Italic text*"), NodeType::Emphasis);
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("_Italic text_"), NodeType::Emphasis);
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("**Bold text**"), NodeType::Emphasis);
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("__Bold text__"), NodeType::Emphasis);
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("~~Strikethrough~~"), NodeType::Emphasis);
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("`inline code`"), NodeType::InlineCode);
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("[Link text](http://example.com)"), NodeType::Link);
    EXPECT_EQ(AstNodeFactory::DetermineNodeType("![Alt text](image.png)"), NodeType::Image);
}