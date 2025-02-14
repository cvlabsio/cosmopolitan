#include "libc/nt/codegen.h"
.imp	advapi32,__imp_OpenProcessToken,OpenProcessToken

	.text.windows
        .ftrace1
OpenProcessToken:
        .ftrace2
#ifdef __x86_64__
	push	%rbp
	mov	%rsp,%rbp
	mov	__imp_OpenProcessToken(%rip),%rax
	jmp	__sysv2nt
#elif defined(__aarch64__)
	mov	x0,#0
	ret
#endif
	.endfn	OpenProcessToken,globl
	.previous
