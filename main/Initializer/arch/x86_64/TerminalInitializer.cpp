//
// Created by Alexander Shirokih on 05.09.18.
//

#include <std/format.h>
#include <std/strings.h>
#include <std/types.h>

#include <arch/x86_64/init.h>

static void inline terminal_putchar(char c);

enum vga_color
{
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

class TerminalOutputStream : public Std::OutputStream
{
  public:
    virtual void write(int character)
    {
        terminal_putchar((char)character);
    }
};

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
UByte terminal_color;
Char *terminal_buffer;

static TerminalOutputStream TERMINAL_OUTPUT;

static inline UByte vga_entry_color(enum vga_color fg, enum vga_color bg)
{
    return fg | bg << 4;
}

static inline Char vga_entry(unsigned char uc, UByte color)
{
    return (Char)uc | (Char)color << 8;
}

//Инициализируем терминал как текстовый  VGA терминал
Std::OutputStream *System::initTerminal(void)
{
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_GREEN);
    Char *buff = (Char *)0xB8000;
    terminal_buffer = buff;
    const size_t size = VGA_HEIGHT * VGA_WIDTH;
    for (size_t i = 0; i < size; i++)
    {
        terminal_buffer[i] = vga_entry(' ', terminal_color);
    }
    return &TERMINAL_OUTPUT;
}

void terminal_putentryat(char c, UByte color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void terminal_scrollup()
{
    for (UByte i = 0; i < VGA_HEIGHT; i++)
    {
        for (UByte m = 0; m < VGA_WIDTH; m++)
        {
            terminal_buffer[i * VGA_WIDTH + m] = terminal_buffer[(i + 1) * VGA_WIDTH + m];
        }
    }
}

static void inline terminal_putchar(char c)
{
    Bool n = c == '\n';

    if (!n)
        terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH || n)
    {
        terminal_column = 0;
        if (++terminal_row > VGA_HEIGHT)
        {
            terminal_row = VGA_HEIGHT;
            terminal_scrollup();
        }
    }
}
