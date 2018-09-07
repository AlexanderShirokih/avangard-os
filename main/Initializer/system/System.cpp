//
// Created by Alexander Shirokih on 06.09.18.
//


#include <sys/system.h>

//Initializer's standard IO streams

typedef struct IOStream
{
  private:
    Std::EmptyInputStream EMPTY_IN;
    Std::EmptyOutputStream EMPTY_OUT;

  public:
    Std::OutputStream *out = &EMPTY_OUT;
    Std::InputStream *in = &EMPTY_IN;
} IOStream;

IOStream streams;

void System::setStdIn(Std::InputStream *is)
{
    streams.in = is;
}

void System::setStdOut(Std::OutputStream *os)
{
    streams.out = os;
}

Std::OutputStream *System::getStdOut()
{
    return streams.out;
}

Std::InputStream *System::getStdIn()
{
    return streams.in;
}
