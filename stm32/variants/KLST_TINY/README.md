# KLST_TINY / tech specs

## timer map

counter resolution is 16BIT except for `TIM2` + `TIM5`:

- `TIM1` :: (available)
- `TIM2` :: reserved for `ENCODER_02` (32BIT)
- `TIM3` :: reserved for `ENCODER_00` 
- `TIM4` :: (available)
- `TIM5` :: reserved for `beat()` (32BIT)
- `TIM6` :: reserved for `TIMER_TONE` @todo(maybe remove this)
- `TIM7` :: reserved for `TIMER_SERVO` @todo(maybe remove this)
- `TIM8` :: reserved for `ENCODER_01` 
- `TIM9` :: (available)
- `TIM10` :: (available)
- `TIM11` :: (available)
- `TIM12` :: (available)
- `TIM13` :: (available)
- `TIM14` :: (available)

( see [STM32F446xC/E datasheet](https://www.st.com/resource/en/datasheet/stm32f446re.pdf), p30 for more details on timer capabilities )