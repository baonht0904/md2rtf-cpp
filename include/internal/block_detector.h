#ifndef BLOCK_DETECTOR_H
#define BLOCK_DETECTOR_H

#include <regex>
#include <string_view>
#include <string>
#include <vector>

#include "internal/markdown_ast.h"

namespace md2rtf::internal::markdown_ast
{
    class BlockData
    {
    public:
        explicit BlockData(std::string_view line);
        ~BlockData() = default;
        void AddLine(std::string_view line);
        const std::vector<std::string>& GetLines() const;
        NodeType GetType() const;
        bool IsEmpty() const;

    private:
        std::vector<std::string> lines;
        NodeType type = NodeType::Paragraph; // Default type is Paragraph
    };

    class BlockDetector
    {
    public:
        explicit BlockDetector(std::string_view markdown);
        ~BlockDetector() = default;

        BlockData NextBlock();
        bool HasMore() const;

        
    protected:
        void SkipEmptyLines();
        std::string_view NextLine(size_t pos) const;
        void MoveCurrentPositionPassLine(std::string_view line);

        // Multiple lines collection
        void GatherContiguousBlockLines(BlockData &block);
        void CollectDefaultBlockLines(BlockData &block);
        void CollectCodeBlockLines(BlockData &block);
        void CollectFencedCodeBlockLines(BlockData &block);
        void CollectIndentedCodeBlockLines(BlockData &block);

    private:
        size_t current_position_;
        std::string_view markdown_content_;
    };

} // namespace md2rtf::internal::markdown_ast

#endif // BLOCK_DETECTOR_H