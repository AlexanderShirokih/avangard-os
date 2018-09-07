/*
 * Initializer.cpp
 *
 *  Created on: 5 сен. 2018 г.
 *      Author: Alexander Shirokih
 */

#include <sys/system.h>
#include <sys/arch_init.h>
#include <arch/x86_64/init.h>
#include <arch/x86_64/interrupts.h>

void System::ArchInit::init(const Address mbootAddr)
{
    //Setup standard output stream to write
    //data in terminal
    OutputStream *os = System::initTerminal();
    System::setStdOut(os);

    //Parse memory areas info from multiboot struct
    parseMultibootTags(mbootAddr);

    //Init interrupts
    setupInterrupts();
}