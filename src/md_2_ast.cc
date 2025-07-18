#include "internal/md_2_ast.h"

#include <memory>
#include <string>
#include <vector>

#include "internal/markdown_ast.h"

namespace md2rtf::internal
{

    markdown_ast::AST ConvertMarkdownToAST(const std::string &markdown_string)
    {
        using namespace md2rtf::internal::markdown_ast;
        AST ast;
        ast.root = std::make_shared<DocumentNode>();

        BlockFinder finder(markdown_string);

        while (finder.HasMore())
        {
            auto block = finder.NextBlock();
            if (block.empty())
                continue;
            auto node = AstNodeFactory::CreateBlockNode(block);
            ast.root->children.push_back(node);
        }

        return ast;
    }

    // BlockFinder implementation
    BlockFinder::BlockFinder(std::string_view markdown)
        : markdown_(markdown), pos_(0) {}

    namespace
    {
        bool IsSingleLineBlock(markdown_ast::NodeType type)
        {
            using enum md2rtf::internal::markdown_ast::NodeType;
            return type == Heading ||
                   type == HorizontalRule ||
                   type == CodeBlock;
        }
    }

    std::vector<std::string> BlockFinder::NextBlock()
    {
        std::vector<std::string> block;
        size_t len = markdown_.length();

        pos_ = SkipLeadingEmptyLines(pos_);
        if (pos_ >= len)
            return block;

        size_t line_end = FindLineEnd(pos_);
        std::string_view first_line = markdown_.substr(pos_, line_end - pos_);
        markdown_ast::NodeType first_type = markdown_ast::AstNodeFactory::DetermineBlockNodeType(first_line);
        block.emplace_back(first_line);
        pos_ = SkipNewlineChars(line_end);

        if (IsSingleLineBlock(first_type))
            return block;

        CollectMultiLineBlock(block, first_type);

        return block;
    }

    void BlockFinder::CollectMultiLineBlock(std::vector<std::string> &block, markdown_ast::NodeType first_type)
    {
        size_t len = markdown_.length();
        while (pos_ < len)
        {
            size_t next_line_end = FindLineEnd(pos_);
            std::string_view next_line = markdown_.substr(pos_, next_line_end - pos_);

            if (IsLineEmptyOrWhitespace(next_line))
                return;

            if (markdown_ast::NodeType next_type = markdown_ast::AstNodeFactory::DetermineBlockNodeType(next_line);
                next_type != first_type)
                return;

            block.emplace_back(next_line);
            pos_ = SkipNewlineChars(next_line_end);
        }
    }

    bool BlockFinder::HasMore() const
    {
        return pos_ < markdown_.length();
    }

    size_t BlockFinder::SkipLeadingEmptyLines(size_t pos) const
    {
        size_t len = markdown_.length();
        while (pos < len && (markdown_[pos] == '\n' || markdown_[pos] == '\r'))
            ++pos;
        return pos;
    }

    size_t BlockFinder::FindLineEnd(size_t pos) const
    {
        size_t rel_line_end = markdown_.substr(pos).find_first_of("\r\n");
        if (rel_line_end == std::string_view::npos)
            return markdown_.length();
        return pos + rel_line_end;
    }

    bool BlockFinder::IsLineEmptyOrWhitespace(std::string_view line) const
    {
        return line.find_first_not_of(" \t\r") == std::string_view::npos;
    }

    size_t BlockFinder::SkipNewlineChars(size_t pos) const
    {
        size_t len = markdown_.length();
        while (pos < len && (markdown_[pos] == '\n' || markdown_[pos] == '\r'))
            ++pos;
        return pos;
    }

}