#include <stdint.h>



#ifndef EVENT_UNIT_H
#define EVENT_UNIT_H

  // ╭──────────────────────────╮
  // │ INTERRUPT AND EVENT UNIT │
  // ╰──────────────────────────╯
 
  // ┌                                                  ┐
  // │ I created these because I thought it was being   │
  // │ used on the current pulpino configuration, it is │
  // │ not being used...                                │
  // │                                                  │
  // │ I'm leaving here if we need to use in the future │
  // └                                                  ┘

	// Sleep struct
	typedef struct{
		volatile uint32_t CONTROL;
		volatile uint32_t STATUS;
	} sleep_controller_t;


	// Struct for async controller (both interrupt and event controllers)
	typedef struct{
		volatile uint32_t ENABLE;
		volatile uint32_t PENDING_RW;
		volatile uint32_t PENDING_SET;
		volatile uint32_t PENDING_CLEAR;
	} async_controller_t;


	// Struct for the complete Event Unit
	typedef struct {
		async_controller_t INTER;
		async_controller_t EVENT;
		sleep_controller_t SLEEP;
	} event_unit_t;



#endif
