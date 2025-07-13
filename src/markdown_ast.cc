#include "internal/markdown_ast.h"

#include <memory>
#include <regex>
#include <string>
#include <vector>
#include <utility>

#include "internal/helpers.h"

namespace md2rtf::internal::markdown_ast
{
    // Sorted by longest/most specific patterns first, then by generality
    const std::vector<std::pair<NodeType, std::string>> g_kNodeRegexes = {
        {NodeType::CodeBlock, R"(^```.*\n(?:.*\n)*?```$)"},
        {NodeType::HorizontalRule, R"(^[-*_]{3,})"},
        {NodeType::Emphasis, R"(\*|_|\*\*|__|~~)"},
        {NodeType::Table, R"(^\|.*\n\|[-:]+.*\n\|.*)"},
        {NodeType::TableRow, R"(^\|.*$)"},
        {NodeType::TableCell, R"(^\|[^|]+\|$)"},
        {NodeType::Heading, R"(^#+\s+.*)"},
        {NodeType::Blockquote, R"(^>\s+.*)"},
        {NodeType::List, R"(^[-*+]\s+.*|^\d+\.\s+.*)"},
        {NodeType::ListItem, R"(^[-*+]\s+.*|^\d+\.\s+.*)"},
        {NodeType::Image, R"(!\[.*\]\([^\)]+\))"},
        {NodeType::Link, R"(\[[^\]]+\]\([^\)]+\))"},
        {NodeType::InlineCode, R"(`[^`]+`)"},
        {NodeType::Paragraph, R"(^.*)"},
        {NodeType::Text, R"(^.*)"},
    };

    std::shared_ptr<Node> AstNodeFactory::CreateNode(const std::string& line)
    {
        // TODO: Parse the line and create appropriate AST nodes
        return nullptr; // Placeholder for now
    }

    NodeType AstNodeFactory::DetermineNodeType(const std::string& line)
    {
        auto trimmed_line = internal::helpers::Trim(line);
        for(const auto& [type, regex_str] : g_kNodeRegexes)
        {
            std::regex regex(regex_str);
            if (std::regex_match(trimmed_line, regex))
            {
                return type;
            }
        }

        return NodeType::Paragraph; // Default to Paragraph if no match found
    }
}
