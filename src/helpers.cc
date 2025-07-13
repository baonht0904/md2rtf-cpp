#include "internal/helpers.h"

#include <string>
#include <vector>

namespace md2rtf::internal::helpers
{
    bool StartsWith(const std::string& str, const std::string& prefix)
    {
        return str.compare(0, prefix.length(), prefix) == 0;
    }

    bool StartsWithAnyOf(const std::string& str, const std::vector<std::string>& prefixes)
    {
        if(str.empty() && prefixes.empty())
        {
            return true;
        }

        for (const auto& prefix : prefixes)
        {
            if (StartsWith(str, prefix))
            {
                return true;
            }
        }
        return false;
    }

    bool EndsWith(const std::string& str, const std::string& suffix)
    {
        if (suffix.length() > str.length())
            return false;
        return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
    }

    bool EndsWithAnyOf(const std::string& str, const std::vector<std::string>& suffixes)
    {
        if(str.empty() && suffixes.empty())
        {
            return true;
        }

        for (const auto& suffix : suffixes)
        {
            if (EndsWith(str, suffix))
            {
                return true;
            }
        }
        return false;
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