#include <stdbool.h>

#define TIMER_SUCCESS       0
#define TIMER_ERRORNOINIT  -1

signed int Timer_initialise(unsigned int base_address);
bool Timer_isInitialised( void );

signed int Timer_set_prescaler(signed char new_prescaler_value);
signed int Timer_set_load(unsigned int new_load_value);
signed int Timer_se_control_flags(
		bool set_timer_enable,
		bool set_auto_reload,
		bool set_interrupt_enable);

signed int Timer_read_current_time();

bool Timer_read_interrupt_flag();
signed int Timer_clear_interrupt();

