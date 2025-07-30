#include "internal/block_detector.h"

#include <algorithm>
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
            CollectOtherLinesOfBlock(block);
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
        size_t line_end = markdown_content_.find('\n', pos);
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

    void BlockDetector::CollectOtherLinesOfBlock(BlockData &block)
    { 
        // Ensure the block has at least one line to start with
        if (block.IsEmpty())
        {
            return; // No lines to collect
        }
        
        // Collect lines until a different type is found or an empty line is encountered
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

} // namespace md2rtf::internal::markdown_ast