#ifndef MD2RTF_EXCEPTION_H
#define MD2RTF_EXCEPTION_H

#include <stdexcept>
#include <string>

namespace md2rtf {

    class Md2RtfException : public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };

} // namespace md2rtf

#endif // MD2RTF_EXCEPTION_H