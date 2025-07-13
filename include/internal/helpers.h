#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include <vector>

namespace md2rtf::internal::helpers
{
    bool StartsWith(const std::string& str, const std::string& prefix);
    bool StartsWithAnyOf(const std::string& str, const std::vector<std::string>& prefixes);
    bool EndsWith(const std::string& str, const std::string& suffix);
    bool EndsWithAnyOf(const std::string& str, const std::vector<std::string>& suffixes);
    std::string Trim(const std::string& str);
}

#endif // HELPERS_H