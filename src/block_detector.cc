#include "internal/block_detector.h"

#include <algorithm>
#include <functional>
#include <string>
#include <string_view>
#include <vector>

#include "internal/markdown_ast.h"
#include "internal/helpers.h"

namespace md2rtf::internal::markdown_ast
{
    BlockData::BlockData(std::string_view line)
    {
        if (!helpers::IsEmptyLineOrWhitespace(line))
        {
            lines.emplace_back(line);
            type = helpers::DetermineBlockType(line);
        }
    }

    void BlockData::AddLine(std::string_view line)
    {
        if (!helpers::IsEmptyLineOrWhitespace(line))
        {
            lines.emplace_back(line);
        }
    }

    const std::vector<std::string>& BlockData::GetLines() const
    {
        return lines;
    }

    NodeType BlockData::GetType() const
    {
        return type;
    }

    bool BlockData::IsEmpty() const
    {
        return lines.empty();
    }

    BlockDetector::BlockDetector(std::string_view markdown) 
    : current_position_(0), markdown_content_(markdown) {}

    BlockData BlockDetector::NextBlock()
    {
        // Prepare to collect the next block of lines
        SkipEmptyLines();
        if(!HasMore())
        {
            return BlockData(""); // No more blocks to process
        }

        // Get the next line and determine its type
        auto next_line = NextLine(current_position_);
        MoveCurrentPositionPassLine(next_line);
        BlockData block(next_line);

        // Collect other lines of the block if it is a multi-line type
        if(helpers::IsMultipleLineType(block.GetType()))
        {
            GatherContiguousBlockLines(block);
        }

        return block;
    }

    bool BlockDetector::HasMore() const
    {
        return current_position_ < markdown_content_.length();
    }

    void BlockDetector::SkipEmptyLines()
    {
        while (HasMore())
        {
            auto line = NextLine(current_position_);
            if (!helpers::IsEmptyLineOrWhitespace(line))
            {
                break;
            }
            MoveCurrentPositionPassLine(line);
        }
    }

    std::string_view BlockDetector::NextLine(size_t pos) const
    {
        size_t line_end = markdown_content_.find_first_of("\r\n", pos);
        if (line_end == std::string_view::npos)
        {
            line_end = markdown_content_.length();
        }
        return markdown_content_.substr(pos, line_end - pos);
    }

    void BlockDetector::MoveCurrentPositionPassLine(std::string_view line)
    {
        current_position_ += line.length() + 1; // Move past the line break
    }

    void BlockDetector::GatherContiguousBlockLines(BlockData &block)
    { 
        if(block.GetType() == NodeType::CodeBlock)
        {
            CollectCodeBlockLines(block);
        }
        else
        {
            CollectDefaultBlockLines(block);
        }
    }

    void BlockDetector::CollectDefaultBlockLines(BlockData &block)
    {
        // Ensure the block has at least one line to start with
        // This is important for default block types to ensure we have a starting point
        if (block.IsEmpty())
        {
            return; // No lines to collect
        }

        // Collect lines for default block types (e.g., Paragraph, Heading)
        while (HasMore())
        {
            auto next_line = NextLine(current_position_);
            if (helpers::IsEmptyLineOrWhitespace(next_line))
            {
                break;
            }

            if (NodeType next_type = helpers::DetermineBlockType(next_line);
                next_type != block.GetType())
            {
                return;
            }

            block.AddLine(next_line);
            MoveCurrentPositionPassLine(next_line);
        }
    }

    void BlockDetector::CollectCodeBlockLines(BlockData &block)
    {
        // Ensure the block has at least one line to start with
        // This is important for code blocks to ensure we have a starting point
        if (block.GetLines().empty())
        {
            return; // No lines to collect
        }

        // Determine if this is a fenced or indented code block
        if (helpers::IsIndentedCodeLine(block.GetLines().front()))
        {
            CollectIndentedCodeBlockLines(block);
        }
        else
        {
            CollectFencedCodeBlockLines(block);
        }
    }

    void BlockDetector::CollectFencedCodeBlockLines(BlockData &block)
    {
        // Determine the fence marker used to start the block
        const std::string& first_line = block.GetLines().front();
        std::string fence_marker;
        if (first_line.starts_with("```"))
            fence_marker = "```";
        else if (first_line.starts_with("~~~"))
            fence_marker = "~~~";

        // Collect lines for fenced code blocks
        while (HasMore())
        {
            auto next_line = NextLine(current_position_);
            if (!fence_marker.empty() && next_line.starts_with(fence_marker))
            {
                // Stop at the closing marker
                MoveCurrentPositionPassLine(next_line);
                break;
            }

            block.AddLine(next_line);
            MoveCurrentPositionPassLine(next_line);
        }
    }

    void BlockDetector::CollectIndentedCodeBlockLines(BlockData &block)
    {
        while (HasMore())
        {
            auto next_line = NextLine(current_position_);
            // Accept blank lines or lines indented by 4+ spaces
            if (helpers::IsEmptyLineOrWhitespace(next_line) ||
                helpers::IsIndentedCodeLine(next_line))
            {
                block.AddLine(next_line);
                MoveCurrentPositionPassLine(next_line);
            }
            else
            {
                break; // End of indented code block
            }
        }
    }

} // namespace md2rtf::internal::markdown_ast