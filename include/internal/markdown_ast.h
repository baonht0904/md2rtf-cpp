#ifndef MARKDOWN_AST_H
#define MARKDOWN_AST_H

#include <set>
#include <memory>
#include <string>
#include <vector>

namespace md2rtf::internal::markdown_ast
{

    enum class NodeType
    {
        Document,
        Heading,
        Paragraph,
        BlockQuote,
        List,
        ListItem,
        Table,
        TableRow,
        TableCell,
        CodeBlock,
        HorizontalRule,
        Text,
        Emphasis,
        Strong,
        Strikethrough,
        InlineCode,
        Link,
        Image
    };

    // Base node
    class Node
    {
    public:
        NodeType type;
        virtual ~Node() = default;
    };

    // Block node base
    class BlockNode : public Node
    {
    public:
        virtual ~BlockNode() = default;
    };

    // Inline node base
    class InlineNode : public Node
    {
    public:
        virtual ~InlineNode() = default;
    };

    // Block nodes
    class DocumentNode : public BlockNode
    {
    public:
        std::vector<std::shared_ptr<BlockNode>> children;
        DocumentNode() { type = NodeType::Document; }
    };

    class HeadingNode : public BlockNode
    {
    public:
        int level;
        std::vector<std::shared_ptr<InlineNode>> children; // inline nodes
        HeadingNode(int lvl) : level(lvl) { type = NodeType::Heading; }
    };

    class ParagraphNode : public BlockNode
    {
    public:
        std::vector<std::shared_ptr<InlineNode>> children; // inline nodes
        ParagraphNode() { type = NodeType::Paragraph; }
    };

    class BlockquoteNode : public BlockNode
    {
    public:
        std::vector<std::shared_ptr<BlockNode>> children; // block nodes
        BlockquoteNode() { type = NodeType::BlockQuote; }
    };
    
    class ListItemNode : public BlockNode
    {
    public:
        std::vector<std::shared_ptr<BlockNode>> children; // block nodes
        ListItemNode() { type = NodeType::ListItem; }
    };

    class ListNode : public BlockNode
    {
    public:
        bool ordered;
        std::vector<std::shared_ptr<ListItemNode>> children; // list items
        ListNode(bool ord) : ordered(ord) { type = NodeType::List; }
    };

    class TableCellNode : public BlockNode
    {
    public:
        std::vector<std::shared_ptr<InlineNode>> children; // inline nodes
        std::string align;
        TableCellNode(const std::string &alignment = "") : align(alignment) { type = NodeType::TableCell; }
    };

    class TableRowNode : public BlockNode
    {
    public:
        bool isHeader;
        std::vector<std::shared_ptr<TableCellNode>> children; // table cells
        TableRowNode(bool header) : isHeader(header) { type = NodeType::TableRow; }
    };

    class TableNode : public BlockNode
    {
    public:
        std::vector<std::shared_ptr<TableRowNode>> children; // table rows
        TableNode() { type = NodeType::Table; }
    };

    class CodeBlockNode : public BlockNode
    {
    public:
        std::string value;
        std::string language;
        CodeBlockNode(const std::string &val, const std::string &lang = "") : value(val), language(lang) { type = NodeType::CodeBlock; }
    };

    class HorizontalRuleNode : public BlockNode
    {
    public:
        HorizontalRuleNode() { type = NodeType::HorizontalRule; }
    };

    // Inline nodes
    class TextNode : public InlineNode
    {
    public:
        std::string value;
        TextNode(const std::string &val) : value(val) { type = NodeType::Text; }
    };

    class EmphasisNode : public InlineNode
    {
    public:
        std::vector<std::shared_ptr<InlineNode>> children;
        EmphasisNode() { type = NodeType::Emphasis; }
    };

    class StrongNode : public InlineNode
    {
    public:
        std::vector<std::shared_ptr<InlineNode>> children;
        StrongNode() { type = NodeType::Strong; }
    };

    class StrikethroughNode : public InlineNode
    {
    public:
        std::vector<std::shared_ptr<InlineNode>> children;
        StrikethroughNode() { type = NodeType::Strikethrough; }
    };

    class InlineCodeNode : public InlineNode
    {
    public:
        std::string value;
        InlineCodeNode(const std::string &val) : value(val) { type = NodeType::InlineCode; }
    };

    class LinkNode : public InlineNode
    {
    public:
        std::string url;
        std::string title;
        std::vector<std::shared_ptr<InlineNode>> children;
        LinkNode(const std::string &u, const std::string &t = "") : url(u), title(t) { type = NodeType::Link; }
    };

    class ImageNode : public InlineNode
    {
    public:
        std::string url;
        std::string alt;
        std::string title;
        ImageNode(const std::string &u, const std::string &a = "", const std::string &t = "") : url(u), alt(a), title(t) { type = NodeType::Image; }
    };

    class AST
    {
    public:
        std::shared_ptr<DocumentNode> root;
    };

    class AstNodeFactory
    {
    public:
        static std::shared_ptr<Node> CreateNode(const std::string &line);
        static NodeType DetermineBlockNodeType(const std::string &line);
    };

} // namespace md2rtf::internal::markdown_ast

#endif // MARKDOWN_AST_H