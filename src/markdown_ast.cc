#include "internal/markdown_ast.h"

#include <memory>
#include <string>
#include <vector>

namespace md2rtf::internal::markdown_ast
{
    std::shared_ptr<Node> AstNodeFactory::CreateNode(const std::string& line)
    {
        // TODO: Parse the line and create appropriate AST nodes
        return nullptr; // Placeholder for now
    }

    NodeType AstNodeFactory::DetermineNodeType(const std::string& line)
    {
        // TODO: Implement logic to determine the node type based on the line content
        return NodeType::Text; // Placeholder for now
    }


}
