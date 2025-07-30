#include "internal/md_2_ast.h"

#include <memory>
#include <string>
#include <vector>

#include "internal/markdown_ast.h"

namespace md2rtf::internal
{

    markdown_ast::AST ConvertMarkdownToAST(const std::string &markdown_string)
    {
        using markdown_ast::AST;
        using markdown_ast::DocumentNode;

        AST ast;
        ast.root = std::make_shared<DocumentNode>();

        // TODO: Implement the logic to parse the markdown_string

        return ast;
    }

}