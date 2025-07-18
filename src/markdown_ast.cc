#include "internal/markdown_ast.h"

#include <memory>
#include <regex>
#include <string>
#include <vector>
#include <utility>

#include "internal/helpers.h"

namespace
{
    using std::string_view;

    bool is_heading(string_view sv)
    {
        return sv.starts_with("#");
    }

    bool is_blockquote(string_view sv)
    {
        return sv.starts_with(">");
    }

    bool is_unordered_list(string_view sv)
    {
        return sv.starts_with("- ") || sv.starts_with("* ") || sv.starts_with("+ ");
    }

    bool is_ordered_list(string_view line)
    {
        static const std::regex ordered_list_re(R"(^\d+\.\s)");
        return std::regex_search(line.data(), ordered_list_re);
    }

    bool is_fenced_code_block(string_view sv)
    {
        return sv.starts_with("```") || sv.starts_with("~~~");
    }

    bool is_indented_code_block(string_view sv)
    {
        return sv.starts_with("    ");
    }

    bool is_horizontal_rule(string_view sv)
    {
        return sv == "---" || sv == "***" || sv == "___";
    }

    bool is_table(string_view sv)
    {
        return sv.contains('|');
    }

} // anonymous namespace

namespace md2rtf::internal::markdown_ast
{
    std::shared_ptr<BlockNode> AstNodeFactory::CreateBlockNode(const std::vector<std::string> &lines)
    {
        if (lines.empty())
        {
            return nullptr;
        }
        
        using enum md2rtf::internal::markdown_ast::NodeType;

        switch (auto node_type = DetermineBlockNodeType(lines.at(0)))
        {
        case Paragraph:
            return CreateParagraphNode(lines);
        case Heading:
            return CreateHeadingNode(lines);
        case BlockQuote:
            return CreateBlockQuoteNode(lines);
        case List:
            return CreateListNode(lines);
        case CodeBlock:
            return CreateCodeBlockNode(lines);
        case HorizontalRule:
            return CreateHorizontalRuleNode(lines);
        case Table:
            return CreateTableNode(lines);
        default:
            return nullptr;
        }

        return nullptr; // Placeholder for now
    }

    NodeType AstNodeFactory::DetermineBlockNodeType(std::string_view line)
    {
        using enum md2rtf::internal::markdown_ast::NodeType;

        if (is_heading(line))
            return Heading;
        if (is_blockquote(line))
            return BlockQuote;
        if (is_unordered_list(line))
            return List;
        if (is_ordered_list(line))
            return List;
        if (is_fenced_code_block(line))
            return CodeBlock;
        if (is_indented_code_block(line))
            return CodeBlock;
        if (is_horizontal_rule(line))
            return HorizontalRule;
        if (is_table(line))
            return Table;

        return Paragraph;
    }

    std::shared_ptr<ParagraphNode> AstNodeFactory::CreateParagraphNode(const std::vector<std::string> &lines)
    {
        // TODO: Implement paragraph node creation logic
        return nullptr;
    }

    std::shared_ptr<HeadingNode> AstNodeFactory::CreateHeadingNode(const std::vector<std::string> &lines)
    {
        // TODO: Implement heading node creation logic
        return nullptr;
    }

    std::shared_ptr<BlockQuoteNode> AstNodeFactory::CreateBlockQuoteNode(const std::vector<std::string> &lines)
    {
        // TODO: Implement blockquote node creation logic
        return nullptr;
    }

    std::shared_ptr<ListNode> AstNodeFactory::CreateListNode(const std::vector<std::string> &lines)
    {
        // TODO: Implement list node creation logic
        return nullptr;
    }

    std::shared_ptr<CodeBlockNode> AstNodeFactory::CreateCodeBlockNode(const std::vector<std::string> &lines)
    {
        // TODO: Implement code block node creation logic
        return nullptr;
    }

    std::shared_ptr<HorizontalRuleNode> AstNodeFactory::CreateHorizontalRuleNode(const std::vector<std::string> &lines)
    {
        // TODO: Implement horizontal rule node creation logic
        return nullptr;
    }

    std::shared_ptr<TableNode> AstNodeFactory::CreateTableNode(const std::vector<std::string> &lines)
    {
        // TODO: Implement table node creation logic
        return nullptr;
    }
}