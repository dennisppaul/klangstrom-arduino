# Beat and Timer

this example demonstrates how to use beat and timer.

a `beat` can be initialized with a timer ID and can be set to run at a frequency specified in beast per minute ( BPM ). 
the beat can be started, stopped, and reset. the beat can be used to trigger events. the default callback is 
`void beat(const uint8_t beat_id, const uint16_t beat_counter)` where `beat_id` is the ID of the timer.

KLST_PANDA and KLST_CATERPILLAR have timers `7`, `13`, and `14` configured for immediate use with a default resolution
of 16-bit ( 65536 steps ) and granularity of 20µsec i.e any phase duration from 20µsec to 1.310720sec is possible.

a `timer`can be initialized in a similar way as a beat but can be used to trigger events at a specified phase duration,
configured via `void timer_set_overflow(Timer* timer, const uint32_t duration_us)` or 
`void timer_set_prescale_period(Timer* timer, const uint32_t prescaler, const uint32_t period)`.

## NERD SECTION

on KLST_PANDA and KLST_CATERPILLAR the timers run on the APB clock which is configured to run at 275 MHz ( or 137.5 MHz).
there other prescaler-period combinations are possible to achieve different phase durations. even other timers with 
different configurations can be used but require manual configuration. the system description to see timer usages.
