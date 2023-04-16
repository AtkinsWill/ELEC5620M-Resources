#include "HPS_PrivateTimer.h"

//Driver Base Addresses
volatile unsigned int   *timer_base_ptr     = 0x0;  //0xFFFEC600

//Driver Initialised
bool timer_initialised = false;

//Control Bit Map
#define TIMER_ENABLE    		(1 << 0)  //Output enab1e
#define TIMER_AUTO_RELOAD 		(1 << 1)  //Whether 1ms pulse or 2ms pulse
#define TIMER_INTERRUPT_ENABLE  (1 << 2)  //Whether the servo is ready for changing

#define TIMER_PRESCALER  		8  //Whether the servo is ready for changing

//Register Offsets
#define TIMER_LOAD      (0x00/sizeof(unsigned int))
#define TIMER_VALUE     (0x04/sizeof(unsigned int))
#define TIMER_CONTROL   (0x08/sizeof(unsigned int))
#define TIMER_INTERRUPT (0x0C/sizeof(unsigned int))

//Function to initialise the Timer
signed int Timer_initialise(unsigned int base_address){
    //Initialise base address pointers
    timer_base_ptr = (unsigned int *)  base_address;
    //Ensure timer initialises to disabled
    timer_base_ptr[TIMER_CONTROL] = 0;
    //Timer now initialised
    timer_initialised = true;
    return TIMER_SUCCESS;
}

//Check if driver initialised
bool Timer_isInitialised() {
    return timer_initialised;
}


//Set prescaler value
signed int Timer_set_prescaler(signed char new_prescaler_value){
    volatile unsigned int* control_ptr;
	if (!Timer_isInitialised()) return TIMER_ERRORNOINIT;

	//If ready, update the prescaler value
	control_ptr = (unsigned int*)&timer_base_ptr[TIMER_CONTROL];
	//control_ptr = timer_base_ptr + TIMER_CONTROL;
	//control_ptr[TIMER_PRESCALER] = new_prescaler_value;
	*control_ptr = new_prescaler_value << TIMER_PRESCALER;
	return TIMER_SUCCESS;
}

//Set load value
signed int Timer_set_load(unsigned int new_load_value){
    volatile unsigned int* load_ptr;
	if (!Timer_isInitialised()) return TIMER_ERRORNOINIT;

	//If ready, determine the load register pointer
	load_ptr = (unsigned int*)&timer_base_ptr[TIMER_LOAD];

	//Update the load value
	*load_ptr = new_load_value;

	return TIMER_SUCCESS;
}

//Set Enable Flag
signed int Timer_set_control_flags(
		bool set_timer_enable,
		bool set_auto_reload,
		bool set_interrupt_enable){
    volatile unsigned int* control_ptr;
	if (!Timer_isInitialised()) return TIMER_ERRORNOINIT;

	//Determine the control register pointer
	control_ptr = timer_base_ptr + TIMER_CONTROL;

	//If ready, update the control register values
	if (set_timer_enable) {
		control_ptr[0] |=  TIMER_ENABLE;
	}
	else {
		control_ptr[0] &= ~TIMER_ENABLE;
	}
	if (set_auto_reload) {
		control_ptr[0] |=  TIMER_AUTO_RELOAD;
	}
	else {
		control_ptr[0] &= ~TIMER_AUTO_RELOAD;
	}
	if (set_interrupt_enable) {
		control_ptr[0] |=  TIMER_INTERRUPT_ENABLE;
	}
	else {
		control_ptr[0] &= ~TIMER_INTERRUPT_ENABLE;
	}

	return TIMER_SUCCESS;
}


signed int Timer_read_current_time(){
	volatile unsigned int* value_ptr;
	if (!Timer_isInitialised()) return TIMER_ERRORNOINIT;

	//If ready, read the current timer value
	value_ptr = (unsigned int*)&timer_base_ptr[TIMER_VALUE];

	return *value_ptr;
}

bool Timer_read_interrupt_flag(){
	volatile unsigned int* interrupt_ptr;
	if (!Timer_isInitialised()) return TIMER_ERRORNOINIT;

	//If ready,  value
	interrupt_ptr = (unsigned int*)&timer_base_ptr[TIMER_INTERRUPT];

	return (*interrupt_ptr & 0x1);
}

signed int Timer_clear_interrupt(){
	volatile unsigned int* interrupt_ptr;
	if (!Timer_isInitialised()) return TIMER_ERRORNOINIT;

	//If ready,  value
	interrupt_ptr = (unsigned int*)&timer_base_ptr[TIMER_INTERRUPT];
	*interrupt_ptr = 0x1;

	return TIMER_SUCCESS;
}



signed int startTimer(bool timer_flags[]){
	timer_flags[2] = true;
	if (!Timer_isInitialised()) return TIMER_ERRORNOINIT;
	Timer_set_control_flags(timer_flags[0], timer_flags[1], timer_flags[2]);
	return Timer_read_current_time();
}

signed int stopTimer(bool timer_flags[]){
	timer_flags[2] = false;
	if (!Timer_isInitialised()) return TIMER_ERRORNOINIT;
	Timer_set_control_flags(timer_flags[0], timer_flags[1], timer_flags[2]);
	return Timer_read_current_time();
}

double calculateElapsedTime(signed int start_time, signed int current_time){
	double time_difference;
	if (start_time < current_time){
		time_difference = (start_time - current_time) + 100000000;
    }
	else{
		time_difference = (start_time - current_time);
	}
	time_difference *= 8.889;
	time_difference /= 1000000000;

	return time_difference;
}

