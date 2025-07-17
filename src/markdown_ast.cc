#include "internal/markdown_ast.h"

#include <memory>
#include <regex>
#include <string>
#include <vector>
#include <utility>

#include "internal/helpers.h"

namespace md2rtf::internal::markdown_ast
{

    std::shared_ptr<Node> AstNodeFactory::CreateNode(const std::string &line)
    {
        // TODO: Parse the line and create appropriate AST nodes
        return nullptr; // Placeholder for now
    }

    NodeType AstNodeFactory::DetermineBlockNodeType(const std::string &line)
    {
        std::string_view sv(line);

        // Heading
        if (sv.starts_with("#"))
            return NodeType::Heading;

        // Blockquote
        if (sv.starts_with(">"))
            return NodeType::BlockQuote;

        // Unordered List
        if (sv.starts_with("- ") || sv.starts_with("* ") || sv.starts_with("+ "))
            return NodeType::List;

        // Ordered List
        if (std::regex_search(line, std::regex(R"(^\d+\.\s)")))
            return NodeType::List;

        // Code Block (fenced)
        if (sv.starts_with("```") || sv.starts_with("~~~"))
            return NodeType::CodeBlock;

        // Code Block (indented)
        if (sv.size() >= 4 && sv.starts_with("    "))
            return NodeType::CodeBlock;

        // Horizontal Rule
        if (sv == "---" || sv == "***" || sv == "___")
            return NodeType::HorizontalRule;

        // Table
        if (sv.contains('|'))
            return NodeType::Table;

        // Default: Paragraph
        return NodeType::Paragraph;
    }
}
