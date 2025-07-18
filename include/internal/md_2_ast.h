#ifndef MD_2_AST_H
#define MD_2_AST_H

#include "internal/markdown_ast.h"

namespace md2rtf::internal
{
    // Convert Markdown to AST
    markdown_ast::AST ConvertMarkdownToAST(const std::string& markdown_string);

    // Additional functions for AST manipulation
    class BlockFinder {
    public:
        explicit BlockFinder(std::string_view markdown);

        // Returns the next block as a vector of strings, or empty if done
        std::vector<std::string> NextBlock();
        bool HasMore() const;

    private:
        std::string_view markdown_;
        size_t pos_;

        size_t SkipLeadingEmptyLines(size_t pos) const;
        size_t FindLineEnd(size_t pos) const;
        bool IsLineEmptyOrWhitespace(std::string_view line) const;
        size_t SkipNewlineChars(size_t pos) const;
        void CollectMultiLineBlock(std::vector<std::string>& block, markdown_ast::NodeType first_type);
    };
}

#endif // MD_2_AST_H