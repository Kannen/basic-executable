# basic-executable

libraries to ease generation of code without standard libraries: libc, libgcc and startupfiles

Only for x86/x86\_64 plateforms.

## library basic-execution

This library provide a very basic entry point:
  - at entry, it find begining and end of argv, envp and auxilary vector as documented in the elf specification
  - it then call a function with C linkage: 
``
int execution_start(char** argv_beg, char** end
               ,char** envp_beg, char** envp_end
	       ,auxv_t** auxv_beg, auxv_t** auxv_end)
``
for which library user must provides a definition. When this function is called no initialization is performed yet.
Notably, the user is responsible to call initialization and finalization routines listed in `__preinit_array_start`, `__init_array_end`, etc., (See `execution_start.hpp`). The user is also responsible to set the FS segment correctly to an owned region of memory. This is used as the location of the thread local buffer.

When `execution_start`, `exit` system call is executed with the returned value as exit code.

## library gcc-expected

Provides some of the functions that gcc expects to be always defined: `strlen`, `strcmp`, `memcpy`, etc.
