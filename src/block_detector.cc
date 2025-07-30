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

        // For code blocks, we need to handle both fenced and indented code blocks
        // Determine the type of code block based on the first line
        // and set a stop condition for collecting lines.
        auto code_block_marker = block.GetLines().front();
        std::function<bool(std::string_view)> stop_condition;

        if (code_block_marker.starts_with("```") || code_block_marker.starts_with("~~~"))
        {
            // For fenced code blocks, stop at the closing marker
            stop_condition = [code_block_marker](std::string_view line) {
                return line == code_block_marker;
            };
        }
        else
        {
            // For indented code blocks, stop at an empty line
            stop_condition = [](std::string_view line) {
                return helpers::IsEmptyLineOrWhitespace(line);
            };
        }

        // Collect lines for code blocks
        while (HasMore())
        {
            auto next_line = NextLine(current_position_);
            
            block.AddLine(next_line);
            MoveCurrentPositionPassLine(next_line);

            if (stop_condition(next_line))
            {
                break;
            }
        }
    }

} // namespace md2rtf::internal::markdown_ast