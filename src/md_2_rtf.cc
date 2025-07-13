#include "md_2_rtf.h"
#include <string>

namespace md2rtf
{
    std::string ConvertMdToRtf(const std::string& markdown) {
        // Simple implementation: just wrap the text in RTF tags
        return "{\\rtf1\\ansi " + markdown + "}";
    }
} // namespace md2rtf