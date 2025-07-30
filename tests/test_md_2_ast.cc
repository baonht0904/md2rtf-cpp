// #include <gtest/gtest.h>

// #include "internal/markdown_ast.h"
// #include "internal/md_2_ast.h"

// using namespace md2rtf::internal::markdown_ast;

// // Tests for BlockFinder class (previously FindNextBlock)
// TEST(Md2AstTest, BlockFinder_Paragraph) {
//     using md2rtf::internal::BlockFinder;

//     // Single paragraph
//     {
//         std::string input = "This is a paragraph.\n";
//         BlockFinder finder(input);
//         auto block = finder.NextBlock();
//         ASSERT_EQ(block.size(), 1);
//         EXPECT_EQ(block[0], "This is a paragraph.");
//     }

//     // Multiple lines in a paragraph (no empty line between)
//     {
//         std::string input = "Line 1\nLine 2\nLine 3\n";
//         BlockFinder finder(input);
//         auto block = finder.NextBlock();
//         ASSERT_EQ(block.size(), 3);
//         EXPECT_EQ(block[0], "Line 1");
//         EXPECT_EQ(block[1], "Line 2");
//         EXPECT_EQ(block[2], "Line 3");
//     }

//     // Paragraph with leading empty lines
//     {
//         std::string input = "\n\nParagraph content\n";
//         BlockFinder finder(input);
//         auto block = finder.NextBlock();
//         ASSERT_EQ(block.size(), 1);
//         EXPECT_EQ(block[0], "Paragraph content");
//     }

//     // Only empty lines
//     {
//         std::string input = "\n\n\r\n";
//         BlockFinder finder(input);
//         auto block = finder.NextBlock();
//         EXPECT_TRUE(block.empty());
//     }
// }

// TEST(Md2AstTest, BlockFinder_Heading) {
//     using md2rtf::internal::BlockFinder;

//     // Single heading
//     {
//         std::string input = "# Heading 1\n";
//         BlockFinder finder(input);
//         auto block = finder.NextBlock();
//         ASSERT_EQ(block.size(), 1);
//         EXPECT_EQ(block[0], "# Heading 1");
//     }

//     // Multiple heading lines (should be treated as separate blocks in real parsing, but BlockFinder returns first block)
//     {
//         std::string input = "# Heading 1\n## Heading 2\n";
//         BlockFinder finder(input);
//         auto block = finder.NextBlock();
//         ASSERT_EQ(block.size(), 1);
//         EXPECT_EQ(block[0], "# Heading 1");
//     }

//     // Heading with leading empty lines
//     {
//         std::string input = "\n\n## Heading 2\n";
//         BlockFinder finder(input);
//         auto block = finder.NextBlock();
//         ASSERT_EQ(block.size(), 1);
//         EXPECT_EQ(block[0], "## Heading 2");
//     }

//     // Heading followed by paragraph (should only return heading as block)
//     {
//         std::string input = "# Heading 1\nParagraph text\n";
//         BlockFinder finder(input);
//         auto block = finder.NextBlock();
//         ASSERT_EQ(block.size(), 1);
//         EXPECT_EQ(block[0], "# Heading 1");
//     }
// }