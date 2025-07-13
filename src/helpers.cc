#include "internal/helpers.h"

#include <string>

namespace md2rtf::internal::helpers
{
    bool IsStartWith(const std::string& str, const std::string& prefix)
    {
        return str.compare(0, prefix.length(), prefix) == 0;
    }

    bool IsEndWith(const std::string& str, const std::string& suffix)
    {
        if (suffix.length() > str.length())
            return false;
        return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
    }

    std::string Trim(const std::string& str)
    {
        size_t first = str.find_first_not_of(" \t\n\r");
        if (first == std::string::npos)
            return ""; // String is all whitespace

        size_t last = str.find_last_not_of(" \t\n\r");
        return str.substr(first, last - first + 1);
    }
} // namespace md2rtf::internal::helpers