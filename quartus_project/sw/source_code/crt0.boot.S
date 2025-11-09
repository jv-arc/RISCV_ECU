//.............................................................................
// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright 2025 João Carvalho.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
//.............................................................................



#include "pulpino.h"
#define EXCEPTION_STACK_SIZE 72



/*
*======================================================
*                _
*       ___  ___| |_ _   _ _ __
*      / __|/ _ \ __| | | | '_ \
*      \__ \  __/ |_| |_| | |_) |
*      |___/\___|\__|\__,_| .__/
*                         |_|
*
*------------------------------------------------------
* Just cleans everything that is needed to be cleaned
* and starts the C code
*======================================================
*/

.section .text


reset_handler:
  // set all registers to zero
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

  // Sets x2 (sp) initializing the stack
  la   x2, _stack_start


_start:
  .global _start

	la x26, _bss_start
  la x27, _bss_end

	// ignore if no BSS section
  bge x26, x27, zero_loop_end



zero_loop:
	// If BSS exists write 0 all over it
  sw x0, 0(x26)
  addi x26, x26, 4
  ble x26, x27, zero_loop
zero_loop_end:


main_entry:
	// Cleans cli arguments (argc and argv)
  addi x10, x0, 0
  addi x11, x0, 0

	// Jumps to the C program
  jal x1, main


/*
*================================================================
*                  _                  _        _     _
*  __   _____  ___| |_ ___  _ __     | |_ __ _| |__ | | ___
*  \ \ / / _ \/ __| __/ _ \| '__|____| __/ _` | '_ \| |/ _ \
*   \ V /  __/ (__| || (_) | | |_____| || (_| | |_) | |  __/
*    \_/ \___|\___|\__\___/|_|        \__\__,_|_.__/|_|\___|
*
*----------------------------------------------------------------
*
* This section contains the handlers for interruptions and needs
* to directly match interrupt setup on Qsys project.
*
* In Qsys there must be a definition for the memory address for
* the whole vector table and interruption numbers for each
* interruption generating device.
*
* The interrupt table address needs to match the liknerscript
* memory address for this section and each handler must be in a
* 4X offset of it's number. INT 1 -> 0x04, INT 20 -> 0x80 etc.
*
*
*================================================================
*/

  .section .vectors, "ax"

	//disable compressed instructions to avoid address mismatch
  .option norvc


  // Interruption number: 0
	// Origin: JTAG DEBUG UART
  .org 0x00
  jal x0, jtag_interrupt_handler



  // Interruption number: 1
	// Origin: PIO_IN (Board's keys and switches)
  .org 0x04
  jal x0, board_input_handler


  // Interruption number: 2
	// Origin: TIMER32 (32bit IP timer)
  .org 0x08
  jal x0, timer_finished_handler
 
 
  // Interruption number: 3
	// Origin: GPIO_0 (the first 32 bits of the first bank)
	.org 0x0C
  jal x0, gpio_zero_handler


  // Interruption number: 4
	// Origin: GPIO_1 (the first 32 bits of the second bank)
  .org 0x10
  jal x0, gpio_one_handler


  // Interruption number: 5
	// Origin: GPIO_EXTRA (the 8 remaining gpio pins, 4 from each bank)
  .org 0x14
  jal x0, gpio_extra_handler


	// A series of "nop" so anything in between
	// ends up in the default_exec_handler as a fallback
  .rept 19
  nop
  .endr
  jal x0, default_exc_handler



  // Interruption number: 20
  // It's triggered by the reset signal, it runs the execution
	// of the setup program in the beginning of this file
  .org 0x80
  jal x0, reset_handler



	// These 2 also use default_exec_handler for now
  .org 0x84 //illegal instruction INT 21
  jal x0, default_exc_handler

  .org 0x88 // ecal handler INT22
  jal x0, default_exc_handler


