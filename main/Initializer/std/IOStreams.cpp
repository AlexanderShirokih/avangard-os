//
// Created by Alexander Shirokih on 13.06.18.
//

#include <std/streams.h>
#include <std/strings.h>
#include <std/types.h>
#include <stddef.h>

namespace Std
{

void OutputStream::write(String data)
{
    UInt ind = 0;
    while (true)
    {
        char c = data[ind++];
        if (c)
            write(c);
        else
            break;
    }
}

void OutputStream::write(String data, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        write(data[i]);
    }
}

OutputStream &operator<<(OutputStream &sos, const String &str)
{
    int t = 0;
    while (str[t])
    {
        sos.write(str[t++]);
    }
    return sos;
}

OutputStream &operator<<(OutputStream &sos, const char &ch)
{
    sos.write(ch);
    return sos;
}

OutputStream &operator<<(OutputStream &sos, const int &d)
{
    char buffer[20];
    int_to_dstr(d, buffer);
    size_t len = strlen(buffer);
    sos.write(buffer, len);
    return sos;
}
} // namespace Std
