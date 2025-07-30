#ifndef MD_2_AST_H
#define MD_2_AST_H

#include "internal/markdown_ast.h"

namespace md2rtf::internal
{
    // Convert Markdown to AST
    markdown_ast::AST ConvertMarkdownToAST(const std::string& markdown_string);
}

#endif // MD_2_AST_H