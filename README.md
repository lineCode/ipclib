A tiny C++ wrapper around POSIX ipc objects, it just semplifies using POSIX semaphore, shared memory and message box. Undocumented at the moment as this is pretty straightforward (totally untested at the moment by the way)

Compile it using Code::Blocks or generate a makefile from the .cbp using an automated tool (ie: cbp2mak)

If your project uses this library it should be compiled with -lrt and -pthread flags.
