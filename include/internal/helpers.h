#ifndef HELPERS_H
#define HELPERS_H

#include <string>

namespace md2rtf::internal::helpers
{
    bool IsStartWith(const std::string& str, const std::string& prefix);
    bool IsEndWith(const std::string& str, const std::string& suffix);
    std::string Trim(const std::string& str);
}

#endif // HELPERS_H