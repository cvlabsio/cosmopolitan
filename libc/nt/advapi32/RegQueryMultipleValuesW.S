#include "libc/nt/codegen.h"
.imp	advapi32,__imp_RegQueryMultipleValuesW,RegQueryMultipleValuesW

	.text.windows
        .ftrace1
RegQueryMultipleValues:
        .ftrace2
#ifdef __x86_64__
	push	%rbp
	mov	%rsp,%rbp
	mov	__imp_RegQueryMultipleValuesW(%rip),%rax
	jmp	__sysv2nt6
#elif defined(__aarch64__)
	mov	x0,#0
	ret
#endif
	.endfn	RegQueryMultipleValues,globl
	.previous
