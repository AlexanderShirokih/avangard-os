//
// Created by Alexander Shirokih on 13.06.18.
//
#ifndef AVANGARDOS_STREAMS_H
#define AVANGARDOS_STREAMS_H

#include <std/types.h>
#include <stddef.h>

namespace Std
{

/*
     * Виртуальный класс входного потока.
     * Определяет метод записи единичного символа
     */

class InputStream
{
  public:
    virtual int read() = 0;
};

/*
     * Виртуальный класс выходного потока.
     * Определяет метод чтения единичного символа
     */
class OutputStream
{
  public:
    virtual void write(int character) = 0;

    void write(String data);

    void write(String data, size_t len);

    friend OutputStream &operator<<(OutputStream &cos, const String &str);

    friend OutputStream &operator<<(OutputStream &cos, const char &ch);

    friend OutputStream &operator<<(OutputStream &cos, const int &d);
};

class EmptyInputStream : public InputStream
{
  public:
    EmptyInputStream() {}
    virtual int read() { return 0; }
};

class EmptyOutputStream : public OutputStream
{
  public:
    EmptyOutputStream() {}
    virtual void write(int ch) { (void)ch; }
};
} // namespace Std
#endif //AVANGARDOS_STREAMS_H
