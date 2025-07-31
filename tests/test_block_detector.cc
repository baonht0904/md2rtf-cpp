#include <gtest/gtest.h>
#include "internal/block_detector.h"

using md2rtf::internal::markdown_ast::BlockDetector;
using md2rtf::internal::markdown_ast::BlockData;
using md2rtf::internal::markdown_ast::NodeType;

TEST(BlockDetectorTest, NextBlock_Empty) {
    {
        // Empty input
        BlockDetector detector("");
        auto block = detector.NextBlock();
        EXPECT_TRUE(block.IsEmpty());
        EXPECT_EQ(block.GetType(), NodeType::Paragraph);
    }

    {
        // Input with only newlines
        BlockDetector detector("\n\n\n");
        auto block = detector.NextBlock();
        EXPECT_TRUE(block.IsEmpty());
        EXPECT_EQ(block.GetType(), NodeType::Paragraph);
    }
}

TEST(BlockDetectorTest, NextBlock_Paragraph) {
    {
        // Normal paragraph with multiple lines
        BlockDetector detector("This is a paragraph.\nNext line.");
        auto block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::Paragraph);
        ASSERT_EQ(block.GetLines().size(), 2);
        EXPECT_EQ(block.GetLines()[0], "This is a paragraph.");
        EXPECT_EQ(block.GetLines()[1], "Next line.");
    }

    {
        // Single line paragraph
        BlockDetector detector("Single line paragraph.");
        auto block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::Paragraph);
        ASSERT_EQ(block.GetLines().size(), 1);
        EXPECT_EQ(block.GetLines()[0], "Single line paragraph.");
    }

    {
        // Empty paragraph
        BlockDetector detector("\n\n\r\n");
        auto block = detector.NextBlock();
        EXPECT_TRUE(block.IsEmpty());
        EXPECT_EQ(block.GetType(), NodeType::Paragraph);
    }

    {
        // Paragraph with leading empty lines
        BlockDetector detector("\n\nThis is a paragraph.\nNext line.");
        auto block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::Paragraph);
        ASSERT_EQ(block.GetLines().size(), 2);
        EXPECT_EQ(block.GetLines()[0], "This is a paragraph.");
        EXPECT_EQ(block.GetLines()[1], "Next line.");
    }

    {
        // Paragraph with trailing empty lines
        BlockDetector detector("This is a paragraph.\nNext line.\n\n");
        auto block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::Paragraph);
        ASSERT_EQ(block.GetLines().size(), 2);
        EXPECT_EQ(block.GetLines()[0], "This is a paragraph.");
        EXPECT_EQ(block.GetLines()[1], "Next line.");
    }

    {
        // Two paragraphs with an empty line in between
        BlockDetector detector("This is a paragraph.\nSame paragraph.\n\nNext paragraph.");
        auto block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::Paragraph);
        ASSERT_EQ(block.GetLines().size(), 2);
        EXPECT_EQ(block.GetLines()[0], "This is a paragraph.");
        EXPECT_EQ(block.GetLines()[1], "Same paragraph.");

        auto next_block = detector.NextBlock();
        ASSERT_EQ(next_block.GetType(), NodeType::Paragraph);
        ASSERT_EQ(next_block.GetLines().size(), 1);
        EXPECT_EQ(next_block.GetLines()[0], "Next paragraph.");
    }

}

TEST(BlockDetectorTest, NextBlock_Heading) {
    {
        // Single heading without trailing empty lines
        BlockDetector detector("# Heading\nNext line.");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::Heading);
        ASSERT_EQ(block.GetLines().size(), 1);
        EXPECT_EQ(block.GetLines()[0], "# Heading");
    }

    {
        // Single heading with trailing empty lines
        BlockDetector detector("# Heading\n\nNext line.\n");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::Heading);
        ASSERT_EQ(block.GetLines().size(), 1);
        EXPECT_EQ(block.GetLines()[0], "# Heading");
    }

    {
        // Heading with leading empty lines
        BlockDetector detector("\n\n# Heading with leading empty lines\nNext line.");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::Heading);
        ASSERT_EQ(block.GetLines().size(), 1);
        EXPECT_EQ(block.GetLines()[0], "# Heading with leading empty lines");
    }

    {
        // Heading with multiple lines
        BlockDetector detector("# Heading 1\n## Heading 2\nNext line.");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::Heading);
        ASSERT_EQ(block.GetLines().size(), 1);
        EXPECT_EQ(block.GetLines()[0], "# Heading 1");
    }

    {
        // Multiple headings with an empty line in between
        BlockDetector detector("# Heading 1\n\n# Heading 2\nNext line.");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::Heading);
        ASSERT_EQ(block.GetLines().size(), 1);
        EXPECT_EQ(block.GetLines()[0], "# Heading 1");

        auto next_block = detector.NextBlock();
        ASSERT_EQ(next_block.GetType(), NodeType::Heading);
        ASSERT_EQ(next_block.GetLines().size(), 1);
        EXPECT_EQ(next_block.GetLines()[0], "# Heading 2");
    }
}

TEST(BlockDetectorTest, NextBlock_BlockQuote) {
    {
        // Single block quote
        BlockDetector detector("> This is a block quote.");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::BlockQuote);
        ASSERT_EQ(block.GetLines().size(), 1);
        EXPECT_EQ(block.GetLines()[0], "> This is a block quote.");
    }

    {
        // Block quote with multiple lines
        BlockDetector detector("> Line 1\n> Line 2\n> Line 3");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::BlockQuote);
        ASSERT_EQ(block.GetLines().size(), 3);
        EXPECT_EQ(block.GetLines()[0], "> Line 1");
        EXPECT_EQ(block.GetLines()[1], "> Line 2");
        EXPECT_EQ(block.GetLines()[2], "> Line 3");
    }

    {
        // Block quote with leading empty lines
        BlockDetector detector("\n\n> Block quote with leading empty lines\nNext line.");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::BlockQuote);
        ASSERT_EQ(block.GetLines().size(), 1);
        EXPECT_EQ(block.GetLines()[0], "> Block quote with leading empty lines");
    }

    {
        // Block quote with trailing empty lines
        BlockDetector detector("> Block quote with trailing empty lines\n\nNext line.");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::BlockQuote);
        ASSERT_EQ(block.GetLines().size(), 1);
        EXPECT_EQ(block.GetLines()[0], "> Block quote with trailing empty lines");
    }

    {
        // Multiple block quotes with an empty line in between
        BlockDetector detector("> First block quote.\n\n> Second block quote.");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::BlockQuote);
        ASSERT_EQ(block.GetLines().size(), 1);
        EXPECT_EQ(block.GetLines()[0], "> First block quote.");

        auto next_block = detector.NextBlock();
        ASSERT_EQ(next_block.GetType(), NodeType::BlockQuote);
        ASSERT_EQ(next_block.GetLines().size(), 1);
        EXPECT_EQ(next_block.GetLines()[0], "> Second block quote.");
    }
}

TEST(BlockDetectorTest, NextBlock_List) {
    {
        // Single unordered list item
        BlockDetector detector("- List item 1");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::List);
        ASSERT_EQ(block.GetLines().size(), 1);
        EXPECT_EQ(block.GetLines()[0], "- List item 1");
    }

    {
        // Multiple unordered list items
        BlockDetector detector("- Item 1\n- Item 2\n- Item 3");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::List);
        ASSERT_EQ(block.GetLines().size(), 3);
        EXPECT_EQ(block.GetLines()[0], "- Item 1");
        EXPECT_EQ(block.GetLines()[1], "- Item 2");
        EXPECT_EQ(block.GetLines()[2], "- Item 3");
    }

    {
        // Single ordered list item
        BlockDetector detector("1. Ordered item 1");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::List);
        ASSERT_EQ(block.GetLines().size(), 1);
        EXPECT_EQ(block.GetLines()[0], "1. Ordered item 1");
    }

    {
        // Multiple ordered list items
        BlockDetector detector("1. First item\n2. Second item\n3. Third item");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::List);
        ASSERT_EQ(block.GetLines().size(), 3);
        EXPECT_EQ(block.GetLines()[0], "1. First item");
        EXPECT_EQ(block.GetLines()[1], "2. Second item");
        EXPECT_EQ(block.GetLines()[2], "3. Third item");
    }

    {
        // Unordered list with leading empty lines
        BlockDetector detector("\n\n- Unordered list with leading empty lines\nNext line.");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::List);
        ASSERT_EQ(block.GetLines().size(), 1);
        EXPECT_EQ(block.GetLines()[0], "- Unordered list with leading empty lines");
    }

    {
        // Ordered list with trailing empty lines
        BlockDetector detector("1. Ordered list with trailing empty lines\n\nNext line.");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::List);
        ASSERT_EQ(block.GetLines().size(), 1);
        EXPECT_EQ(block.GetLines()[0], "1. Ordered list with trailing empty lines");
    }

    /**
     * TODO: Add tests for mixed ordered and unordered lists, nested lists, and lists with block quotes
     */
}

TEST(BlockDetectorTest, NextBlock_Table) {
    {
        // Single table row
        BlockDetector detector("| Header 1 | Header 2 |\n| --- | --- |\n| Cell 1 | Cell 2 |");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::Table);
        ASSERT_EQ(block.GetLines().size(), 3);
        EXPECT_EQ(block.GetLines()[0], "| Header 1 | Header 2 |");
        EXPECT_EQ(block.GetLines()[1], "| --- | --- |");
        EXPECT_EQ(block.GetLines()[2], "| Cell 1 | Cell 2 |");
    }

    {
        // Multiple table rows
        BlockDetector detector("| Row 1 Col 1 | Row 1 Col 2 |\n| Row 2 Col 1 | Row 2 Col 2 |\n| Row 3 Col 1 | Row 3 Col 2 |");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::Table);
        ASSERT_EQ(block.GetLines().size(), 3);
        EXPECT_EQ(block.GetLines()[0], "| Row 1 Col 1 | Row 1 Col 2 |");
        EXPECT_EQ(block.GetLines()[1], "| Row 2 Col 1 | Row 2 Col 2 |");
        EXPECT_EQ(block.GetLines()[2], "| Row 3 Col 1 | Row 3 Col 2 |");
    }

    {
        // Table with leading empty lines
        BlockDetector detector("\n\n| Table with leading empty lines |\n| --- |\n| Cell |\nNext line.");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::Table);
        ASSERT_EQ(block.GetLines().size(), 3);
        EXPECT_EQ(block.GetLines()[0], "| Table with leading empty lines |");
        EXPECT_EQ(block.GetLines()[1], "| --- |");
        EXPECT_EQ(block.GetLines()[2], "| Cell |");
    }

    {
        // Table with trailing empty lines
        BlockDetector detector("| Table with trailing empty lines |\n| --- |\n| Cell |\n\nNext line.");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::Table);
        ASSERT_EQ(block.GetLines().size(), 3);
        EXPECT_EQ(block.GetLines()[0], "| Table with trailing empty lines |");
        EXPECT_EQ(block.GetLines()[1], "| --- |");
        EXPECT_EQ(block.GetLines()[2], "| Cell |");
    }

    {
        // Multiple tables with an empty line in between
        BlockDetector detector("| First table header |\n| --- |\n| Cell 1 |\n\n| Second table header |\n| --- |\n| Cell 2 |");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::Table);
        ASSERT_EQ(block.GetLines().size(), 3);
        EXPECT_EQ(block.GetLines()[0], "| First table header |");
        EXPECT_EQ(block.GetLines()[1], "| --- |");
        EXPECT_EQ(block.GetLines()[2], "| Cell 1 |");

        auto next_block = detector.NextBlock();
        ASSERT_EQ(next_block.GetType(), NodeType::Table);
        ASSERT_EQ(next_block.GetLines().size(), 3);
        EXPECT_EQ(next_block.GetLines()[0], "| Second table header |");
        EXPECT_EQ(next_block.GetLines()[1], "| --- |");
        EXPECT_EQ(next_block.GetLines()[2], "| Cell 2 |");
    }

    /**
     * TODO: Add tests for more complex table structures, such as tables with merged cells, nested tables, and tables with mixed content.
     */
}

TEST(BlockDetectorTest, NextBlock_FencedCodeBlock) {
    {
        // Fenced code block with backticks
        BlockDetector detector("```\nCode line 1\nCode line 2\n```");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::CodeBlock);
        ASSERT_EQ(block.GetLines().size(), 3);
        EXPECT_EQ(block.GetLines()[0], "```");
        EXPECT_EQ(block.GetLines()[1], "Code line 1");
        EXPECT_EQ(block.GetLines()[2], "Code line 2");
    }

    {
        // Fenced code block with tildes
        BlockDetector detector("~~~\nCode line A\nCode line B\n~~~");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::CodeBlock);
        ASSERT_EQ(block.GetLines().size(), 3);
        EXPECT_EQ(block.GetLines()[0], "~~~");
        EXPECT_EQ(block.GetLines()[1], "Code line A");
        EXPECT_EQ(block.GetLines()[2], "Code line B");
    }

    {
        // Code block with leading empty lines
        BlockDetector detector("\n\n```\nIndented code with leading empty lines\n```");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::CodeBlock);
        ASSERT_EQ(block.GetLines().size(), 2);
        EXPECT_EQ(block.GetLines()[0], "```");
        EXPECT_EQ(block.GetLines()[1], "Indented code with leading empty lines");
    }

    {
        // Code block with trailing empty lines
        BlockDetector detector("```\nIndented code with trailing empty lines\n```\n\nNext line.");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::CodeBlock);
        ASSERT_EQ(block.GetLines().size(), 2);
        EXPECT_EQ(block.GetLines()[0], "```");
        EXPECT_EQ(block.GetLines()[1], "Indented code with trailing empty lines");
    }

    {
        // Multiple code blocks with an empty line in between
        BlockDetector detector("```\nFirst code block\n```\n\n~~~\nSecond code block\n~~~");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::CodeBlock);
        ASSERT_EQ(block.GetLines().size(), 2);
        EXPECT_EQ(block.GetLines()[0], "```");
        EXPECT_EQ(block.GetLines()[1], "First code block");

        auto next_block = detector.NextBlock();
        ASSERT_EQ(next_block.GetType(), NodeType::CodeBlock);
        ASSERT_EQ(next_block.GetLines().size(), 2);
        EXPECT_EQ(next_block.GetLines()[0], "~~~");
        EXPECT_EQ(next_block.GetLines()[1], "Second code block");
    }
    /**
     * TODO: Add tests for more complex code blocks, such as those with language annotations, mixed content, and nested code blocks.
     */
}

TEST(BlockDetectorTest, NextBlock_IntentedCodeBlock) {
    {
        // Indented code block with spaces
        BlockDetector detector("    Indented code line 1\n    Indented code line 2");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::CodeBlock);
        ASSERT_EQ(block.GetLines().size(), 2);
        EXPECT_EQ(block.GetLines()[0], "    Indented code line 1");
        EXPECT_EQ(block.GetLines()[1], "    Indented code line 2");
    }

    {
        // Indented code block with tabs
        BlockDetector detector("\tIndented code line 1\n\tIndented code line 2");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::CodeBlock);
        ASSERT_EQ(block.GetLines().size(), 2);
        EXPECT_EQ(block.GetLines()[0], "\tIndented code line 1");
        EXPECT_EQ(block.GetLines()[1], "\tIndented code line 2");
    }

    {
        // Indented code block with leading empty lines
        BlockDetector detector("\n\n    Indented code with leading empty lines\nNext line.");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::CodeBlock);
        ASSERT_EQ(block.GetLines().size(), 1);
        EXPECT_EQ(block.GetLines()[0], "    Indented code with leading empty lines");
    }

    {
        // Indented code block with trailing empty lines
        BlockDetector detector("    Indented code with trailing empty lines\n\nNext line.");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::CodeBlock);
        ASSERT_EQ(block.GetLines().size(), 1);
        EXPECT_EQ(block.GetLines()[0], "    Indented code with trailing empty lines");
    }

    {
        // Multiple indented code blocks with an empty line in between
        BlockDetector detector("    First indented code block\n\n\tSecond indented code block");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::CodeBlock);
        ASSERT_EQ(block.GetLines().size(), 2);
        EXPECT_EQ(block.GetLines()[0], "    First indented code block");
        EXPECT_EQ(block.GetLines()[1], "\tSecond indented code block");
    }

    {
        // Indented code block with mixed spaces and tabs
        BlockDetector detector("    Indented code line 1\n\tIndented code line 2");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::CodeBlock);
        ASSERT_EQ(block.GetLines().size(), 2);
        EXPECT_EQ(block.GetLines()[0], "    Indented code line 1");
        EXPECT_EQ(block.GetLines()[1], "\tIndented code line 2");
    }

    /**
     * TODO: Add tests for more complex code blocks, such as those with language annotations, mixed content, and nested code blocks.
     */
}


TEST(BlockDetectorTest, NextBlock_HorizontalRule) {
    {
        // Single horizontal rule ---
        BlockDetector detector("---");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::HorizontalRule);
        ASSERT_EQ(block.GetLines().size(), 1);
        EXPECT_EQ(block.GetLines()[0], "---");
    }

    {
        // Single horizontal rule ***
        BlockDetector detector("***");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::HorizontalRule);
        ASSERT_EQ(block.GetLines().size(), 1);
        EXPECT_EQ(block.GetLines()[0], "***");
    }

    {
        // Single horizontal rule ___
        BlockDetector detector("___");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::HorizontalRule);
        ASSERT_EQ(block.GetLines().size(), 1);
        EXPECT_EQ(block.GetLines()[0], "___");
    }

    {
        // Multiple horizontal rules
        BlockDetector detector("---\n---\n---");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::HorizontalRule);
        ASSERT_EQ(block.GetLines().size(), 1);
        EXPECT_EQ(block.GetLines()[0], "---");

        auto next_block = detector.NextBlock();
        ASSERT_EQ(next_block.GetType(), NodeType::HorizontalRule);
        ASSERT_EQ(next_block.GetLines().size(), 1);
        EXPECT_EQ(next_block.GetLines()[0], "---");

        auto next_next_block = detector.NextBlock();
        ASSERT_EQ(next_next_block.GetType(), NodeType::HorizontalRule);
        ASSERT_EQ(next_next_block.GetLines().size(), 1);
        EXPECT_EQ(next_next_block.GetLines()[0], "---");
    }

    {
        // Multiple horizontal rules
        BlockDetector detector("---\n---\n---");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::HorizontalRule);
        ASSERT_EQ(block.GetLines().size(), 1);
        EXPECT_EQ(block.GetLines()[0], "---");
        
        auto next_block = detector.NextBlock();
        ASSERT_EQ(next_block.GetType(), NodeType::HorizontalRule);
        ASSERT_EQ(next_block.GetLines().size(), 1);
        EXPECT_EQ(next_block.GetLines()[0], "---");
        
        auto next_next_block = detector.NextBlock();
        ASSERT_EQ(next_next_block.GetType(), NodeType::HorizontalRule);
        ASSERT_EQ(next_next_block.GetLines().size(), 1);
        EXPECT_EQ(next_next_block.GetLines()[0], "---");
    }

    {
        // Horizontal rule with leading empty lines
        BlockDetector detector("\n\n---\nNext line.");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::HorizontalRule);
        ASSERT_EQ(block.GetLines().size(), 1);
        EXPECT_EQ(block.GetLines()[0], "---");
    }

    {
        // Horizontal rule with trailing empty lines
        BlockDetector detector("---\n\nNext line.");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::HorizontalRule);
        ASSERT_EQ(block.GetLines().size(), 1);
        EXPECT_EQ(block.GetLines()[0], "---");
    }

    {
        // Multiple horizontal rules with an empty line in between
        BlockDetector detector("---\n\n---\nNext line.");
        BlockData block = detector.NextBlock();
        ASSERT_EQ(block.GetType(), NodeType::HorizontalRule);
        ASSERT_EQ(block.GetLines().size(), 1);
        EXPECT_EQ(block.GetLines()[0], "---");

        auto next_block = detector.NextBlock();
        ASSERT_EQ(next_block.GetType(), NodeType::HorizontalRule);
        ASSERT_EQ(next_block.GetLines().size(), 1);
        EXPECT_EQ(next_block.GetLines()[0], "---");
    }
}