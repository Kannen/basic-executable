//Documentation: psABI-x86_64, 3.4.1 Process Initialization/Initial Stack and Register State
//c++ _start.S -static -fno-exceptions -fno-rtti -nostdlib -fno-threadsafe-statics
#pragma once

[[noreturn]] void start (void* entry_stack_pointer,void(*finalizing_function)());
