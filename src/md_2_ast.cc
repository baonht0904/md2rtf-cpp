#include "internal/md_2_ast.h"

#include <memory>
#include <string>
#include <vector>

#include "internal/markdown_ast.h"

namespace md2rtf::internal
{
    using namespace md2rtf::internal::markdown_ast;

    AST ConvertMarkdownToAST(const std::string& markdown)
    {
        AST ast;
        ast.root = std::make_shared<DocumentNode>();

        size_t pos = 0;
        while (pos < markdown.length())
        {
           // TODO: Implement parser
        }

        return ast;
    }

    std::string FindNextLine(const std::string& markdown, size_t startPos)
    {
        size_t nextNewline = markdown.find('\n', startPos);
        if (nextNewline == std::string::npos)
            nextNewline = markdown.length();

        return markdown.substr(startPos, nextNewline - startPos);
    }
}