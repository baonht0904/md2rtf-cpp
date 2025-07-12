#include "md_2_rtf/md_2_rtf.h"
#include <string>

std::string convert_md_to_rtf(const std::string& markdown) {
    // Simple implementation: just wrap the text in RTF tags
    return "{\\rtf1\\ansi " + markdown + "}";
}