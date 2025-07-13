#ifndef MARKDOWN_AST_H
#define MARKDOWN_AST_H

#include <set>
#include <memory>
#include <string>
#include <vector>

namespace md2rtf::internal::markdown_ast
{
    enum class NodeType {
        Document, Heading, Paragraph, Text, Emphasis, List, ListItem,
        Link, Image, CodeBlock, InlineCode, Table, TableRow, TableCell,
        Blockquote, HorizontalRule
    };

    const std::set<NodeType> g_kBlockLevelNodeTypes = {
        NodeType::Heading, NodeType::Paragraph, NodeType::List,
        NodeType::Blockquote, NodeType::HorizontalRule, NodeType::Table
    };

    const std::set<NodeType> g_kInlineNodeTypes = {
        NodeType::Text, NodeType::Emphasis, NodeType::Link,
        NodeType::Image, NodeType::InlineCode
    };

    enum class EmphasisType { Bold, Italic, Strikethrough };
    enum class ListType { Ordered, Unordered };

    class Node {
    public:
        NodeType type;
        std::vector<std::shared_ptr<Node>> children;
        virtual ~Node() = default;
    };

    class DocumentNode : public Node {
    public:
        DocumentNode() { type = NodeType::Document; }
    };

    class HeadingNode : public Node {
    public:
        int level;
        HeadingNode(int lvl = 1) : level(lvl) { type = NodeType::Heading; }
    };

    class ParagraphNode : public Node {
    public:
        ParagraphNode() { type = NodeType::Paragraph; }
    };

    class TextNode : public Node {
    public:
        std::string text;
        TextNode(const std::string& t) : text(t) { type = NodeType::Text; }
    };

    class EmphasisNode : public Node {
    public:
        EmphasisType emphasisType;
        EmphasisNode(EmphasisType et) : emphasisType(et) { type = NodeType::Emphasis; }
    };

    class ListNode : public Node {
    public:
        ListType listType;
        int level;
        ListNode(ListType lt, int lvl = 0) : listType(lt), level(lvl) { type = NodeType::List; }
    };

    class ListItemNode : public Node {
    public:
        ListItemNode() { type = NodeType::ListItem; }
    };

    class LinkNode : public Node {
    public:
        std::string url;
        std::string title;
        LinkNode(const std::string& u, const std::string& t) : url(u), title(t) { type = NodeType::Link; }
    };

    class ImageNode : public Node {
    public:
        std::string url;
        std::string altText;
        ImageNode(const std::string& u, const std::string& a) : url(u), altText(a) { type = NodeType::Image; }
    };

    class CodeBlockNode : public Node {
    public:
        std::string code;
        CodeBlockNode(const std::string& c) : code(c) { type = NodeType::CodeBlock; }
    };

    class InlineCodeNode : public Node {
    public:
        std::string code;
        InlineCodeNode(const std::string& c) : code(c) { type = NodeType::InlineCode; }
    };

    class TableNode : public Node {
    public:
        TableNode() { type = NodeType::Table; }
    };

    class TableRowNode : public Node {
    public:
        TableRowNode() { type = NodeType::TableRow; }
    };

    class TableCellNode : public Node {
    public:
        TableCellNode() { type = NodeType::TableCell; }
    };

    class BlockquoteNode : public Node {
    public:
        BlockquoteNode() { type = NodeType::Blockquote; }
    };

    class HorizontalRuleNode : public Node {
    public:
        HorizontalRuleNode() { type = NodeType::HorizontalRule; }
    };

    class AST {
    public:
        std::shared_ptr<DocumentNode> root;
    };

    class AstNodeFactory {
    public:
        static std::shared_ptr<Node> CreateNode(const std::string& line);
        static NodeType DetermineNodeType(const std::string& line);
    };

} // namespace md2rtf::internal::markdown_ast

#endif // MARKDOWN_AST_H