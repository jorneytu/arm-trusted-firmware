/*
 * Copyright (c) 2016, ARM Limited and Contributors. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of ARM nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include <arch.h>
#include <asm_macros.S>
#include <hardware.h>

/*
 * Pull in generic functions to provide backwards compatibility for
 * platform makefiles
 */
	.globl	console_core_init
	.globl	console_core_putc
	.globl	console_core_getc
	

	/* -----------------------------------------------
	 * int console_core_init(uintptr_t base_addr,
	 * unsigned int uart_clk, unsigned int baud_rate)
	 * Function to initialize the console without a
	 * C Runtime to print debug information. This
	 * function will be accessed by console_init and
	 * crash reporting.
	 * In: x0 - console base address
	 *     x1 - Uart clock in Hz
	 *     x2 - Baud rate
	 * Out: return 1 on success else 0 on error
	 * Clobber list : x1, x2, x3
	 * -----------------------------------------------
	 */
func console_core_init
	ldr		x3, =IOMUX_BASE
	ldr		w4, [x3, 0xc]
	orr		w4, w4, #(1 << 0)
	str		w4, [x3, 0xc]

	ldr		w4, [x3, 0x10]
	bic		w4, w4, #(1 << 0)
	str		w4, [x3, 0x10]

	ldr		w4, [x3, 0x14]
	bic		w4, w4, #(1 << 0)
	str		w4, [x3, 0x14]

	mov		w4, #1
	str		w4, [x3, 0xf0]
	mov		w4, #0
	str		w4, [x3, 0xf0]

	mov		w4, #1
	ldr		x3, =RCT_BASE
	str		w4, [x3, 0x38]

	mov		w4, #0
	strb	w4, [x0, #UART_SRR]
	mov		w4, #0x80
	strb	w4, [x0, #UART_LCR]
	mov		w4, #13
	strb	w4, [x0, #UART_DLL]
	mov		w4, #0
	strb	w4, [x0, #UART_DLH]
	mov		w4, #0x3
	strb	w4, [x0, #UART_LCR]
	ret
endfunc console_core_init

	/* --------------------------------------------------------
	 * int console_core_putc(int c, uintptr_t base_addr)
	 * Function to output a character over the console. It
	 * returns the character printed on success or -1 on error.
	 * In : x0 - character to be printed
	 *      x1 - console base address
	 * Out : return -1 on error else return character.
	 * Clobber list : x2
	 * --------------------------------------------------------
	 */
func console_core_putc
	ldr		w4, [x1, #UART_LSR]
	tst		w4, #0x40
	b.eq 	console_core_putc
0:	mov		w2, #'\n'
	cmp		w0, w2
	b.ne	2f
	mov		w3, #'\r'
	str		w3, [x1, #UART_THR]
1:  ldr		w4, [x1, #UART_LSR]
	tst		w4, #0x40
	b.eq 	1b
2:	str		w0, [x1, #UART_THR]
	ret
endfunc console_core_putc

	/* ---------------------------------------------
	 * int console_core_getc(uintptr_t base_addr)
	 * Function to get a character from the console.
	 * It returns the character grabbed on success
	 * or -1 on error.
	 * In : x0 - console base address
	 * Clobber list : x0, x1
	 * ---------------------------------------------
	 */
func console_core_getc
	ret
endfunc console_core_getc
