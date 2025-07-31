#include "internal/helpers.h"

#include <algorithm>
#include <regex>
#include <string_view>

#include "internal/markdown_ast.h"

namespace md2rtf::internal::helpers
{
    namespace determine_block_type
    {
        bool is_heading(std::string_view sv)
        {
            return sv.starts_with("#");
        }

        bool is_blockquote(std::string_view sv)
        {
            return sv.starts_with(">");
        }

        bool is_unordered_list(std::string_view sv)
        {
            return sv.starts_with("- ") || sv.starts_with("* ") || sv.starts_with("+ ");
        }

        bool is_ordered_list(std::string_view line)
        {
            static const std::regex ordered_list_re(R"(^\d+\.\s)");
            return std::regex_search(line.data(), ordered_list_re);
        }

        bool is_fenced_code_block(std::string_view sv)
        {
            return sv.starts_with("```") || sv.starts_with("~~~");
        }

        bool is_indented_code_block(std::string_view sv)
        {
            return sv.starts_with("    ") || sv.starts_with("\t");
        }

        bool is_horizontal_rule(std::string_view sv)
        {
            return sv == "---" || sv == "***" || sv == "___";
        }

        bool is_table(std::string_view sv)
        {
            return sv.contains('|');
        }
    } // namespace determine_block_type

    markdown_ast::NodeType DetermineBlockType(std::string_view line)
    {
        using enum md2rtf::internal::markdown_ast::NodeType;
        using namespace determine_block_type;

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

    bool IsMultipleLineType(markdown_ast::NodeType type)
    {
        using enum md2rtf::internal::markdown_ast::NodeType;
        auto multiple_line_types = {
            BlockQuote, List, CodeBlock, Paragraph, Table
        };
        return std::ranges::find(multiple_line_types, type) != multiple_line_types.end();
    }

    bool IsEmptyLineOrWhitespace(std::string_view line)
    {
        return line.empty() || std::ranges::all_of(line, [](char c) { return std::isspace(c); });
    }

    bool IsIndentedCodeLine(std::string_view line)
    {
        // Check if the line starts with 4 spaces or a tab
        return line.starts_with("    ") || line.starts_with("\t");
    }

} // namespace md2rtf::internal::helpers