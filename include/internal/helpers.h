#ifndef HELPERS_H
#define HELPERS_H

#include <string_view>

#include "internal/markdown_ast.h"

namespace md2rtf::internal::helpers
{
    markdown_ast::NodeType DetermineBlockType(std::string_view line);
    bool IsMultipleLineType(markdown_ast::NodeType type);
    bool IsEmptyLineOrWhitespace(std::string_view line);
    bool IsIndentedCodeLine(std::string_view line);
    
}

#endif // HELPERS_H