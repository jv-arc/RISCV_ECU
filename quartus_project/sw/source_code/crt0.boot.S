// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include "pulpino.h"
#include "debbuging.h"

#define EXCEPTION_STACK_SIZE 72

/* ========================================================= [ entry ] === */
  .section .text




/*----- RESET HANDLER ------
Sets execution up before running
programs, cleans registers, triggers
stack creation and passes control to
main execution.
*/

reset_handler:
  /* set all registers to zero */
  mv  x1, x0
  mv  x2, x1
  mv  x3, x1
  mv  x4, x1
  mv  x5, x1
  mv  x6, x1
  mv  x7, x1
  mv  x8, x1
  mv  x9, x1
  mv x10, x1
  mv x11, x1
  mv x12, x1
  mv x13, x1
  mv x14, x1
  mv x15, x1
  mv x16, x1
  mv x17, x1
  mv x18, x1
  mv x19, x1
  mv x20, x1
  mv x21, x1
  mv x22, x1
  mv x23, x1
  mv x24, x1
  mv x25, x1
  mv x26, x1
  mv x27, x1
  mv x28, x1
  mv x29, x1
  mv x30, x1
  mv x31, x1

  /* stack initilization */
  la   x2, _stack_start


_start:
  .global _start
  
  /* clear BSS */
  la x26, _bss_start
  la x27, _bss_end

  bge x26, x27, zero_loop_end

zero_loop:
  sw x0, 0(x26)
  addi x26, x26, 4
  ble x26, x27, zero_loop
zero_loop_end:


main_entry:
  /* jump to main program entry point (argc = argv = 0) */
  addi x10, x0, 0
  addi x11, x0, 0
  jal x1, main

/* =================================================== [ exceptions ] === */
/* This section has to be down here, since we have to disable rvc for it  */

  .section .vectors, "ax"
  .option norvc;

  // external interrupts are handled by the same callback
  // until compiler supports IRQ routines


  // Interuption number: 0
	// Origin: JTAG DEBUG UART
	// Address-range: 0x0010_0010 - 0x0010_0017
  .org 0x00
  jal x0, jtag_interrupt_handler



  // Interuption number: 1
	// Origin: PIO_I (Board's keys and switches)
	// Address-range: 0x0020_0020 - 0x0020_002F
  .org 0x04
  jal x0, board_input_handler


  // Interuption number: 2
	// Origin: TIMER (32bit IP timer)
	// Address-range: 0x0020_00A0 - 0x0020_00AF
  .org 0x08
  jal x0, timer_finished_handler
 
 
  // Interuption number: 3
	// Origin: GPIO_0 (the first 32 bits of the first bank)
	// Address range: 0x0020_0040 - 0x0020_005F
	.org 0x0C
  jal x0, gpio_zero_handler


  // Interuption number: 4
	// Origin: GPIO_1 (the first 32 bits of the second bank)
	// Address-range: 0x0020_0060 - 0x0020_007F
  .org 0x10
  jal x0, gpio_one_handler


  // Interuption number: 5
	// Origin: GPIO_EXTRA (the 8 remaining gpio pins, 4 from each bank)
	// Address-range: 0x0020_0080 - 0x0020_008F
  .org 0x14
  jal x0, gpio_extra_handler


	// default_exec_handler as fallback
  .rept 19
  nop
  .endr
  jal x0, default_exc_handler


  // reset vector
  .org 0x80
  jal x0, reset_handler

  // illegal instruction exception
  .org 0x84
  jal x0, default_exc_handler

  // ecall handler
  .org 0x88
  jal x0, default_exc_handler
