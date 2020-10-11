/* Automatically generated HAL from hal.xml */
/* NEVER EDIT MANUALLY */

/* Generated on: 2020-10-11 00:39:22 */

#ifndef __HAL_H__
#define __HAL_H__

#include <avr/io.h>

#define nop()                                    __asm__ __volatile__("nop")

/* EXT_IN -> PB3 (Input, Initially Pullup Off) */
#define EXT_IN_BIT                               3
#define EXT_IN_PIN                               PINB
#define EXT_IN_PORT                              PORTB
#define EXT_IN_DDR                               DDRB
#define EXT_IN_SetPullupActive()                 EXT_IN_PORT |= _BV(EXT_IN_BIT)
#define EXT_IN_SetPullupInactive()               EXT_IN_PORT &= ~_BV(EXT_IN_BIT)
#define EXT_IN_ModeInput()                       EXT_IN_DDR &= ~_BV(EXT_IN_BIT)
#define EXT_IN_IsInput()                         ((EXT_IN_DDR & _BV(EXT_IN_BIT)) == 0)
#define EXT_IN_Get()                             (EXT_IN_PIN & _BV(EXT_IN_BIT))
#define EXT_IN_GetBit()                          (EXT_IN_Get() >> EXT_IN_BIT)
#define EXT_IN_IsInactive()                      (EXT_IN_Get() == 0)
#define EXT_IN_IsActive()                        (EXT_IN_Get() != 0)
#define EXT_IN_Init()                            { EXT_IN_SetPullupInactive(); EXT_IN_ModeInput(); }

/* AUX_IN -> PB0 (Input, Initially Pullup Off) */
#define AUX_IN_BIT                               0
#define AUX_IN_PIN                               PINB
#define AUX_IN_PORT                              PORTB
#define AUX_IN_DDR                               DDRB
#define AUX_IN_SetPullupActive()                 AUX_IN_PORT |= _BV(AUX_IN_BIT)
#define AUX_IN_SetPullupInactive()               AUX_IN_PORT &= ~_BV(AUX_IN_BIT)
#define AUX_IN_ModeInput()                       AUX_IN_DDR &= ~_BV(AUX_IN_BIT)
#define AUX_IN_IsInput()                         ((AUX_IN_DDR & _BV(AUX_IN_BIT)) == 0)
#define AUX_IN_Get()                             (AUX_IN_PIN & _BV(AUX_IN_BIT))
#define AUX_IN_GetBit()                          (AUX_IN_Get() >> AUX_IN_BIT)
#define AUX_IN_IsInactive()                      (AUX_IN_Get() == 0)
#define AUX_IN_IsActive()                        (AUX_IN_Get() != 0)
#define AUX_IN_Init()                            { AUX_IN_SetPullupInactive(); AUX_IN_ModeInput(); }

/* TURN_OFF -> PB4 (Output, Initially Inactive) */
#define TURN_OFF_BIT                             4
#define TURN_OFF_PIN                             PINB
#define TURN_OFF_PORT                            PORTB
#define TURN_OFF_DDR                             DDRB
#define TURN_OFF_ModeOutput()                    TURN_OFF_DDR |= _BV(TURN_OFF_BIT)
#define TURN_OFF_IsOutput()                      ((TURN_OFF_DDR & _BV(TURN_OFF_BIT)) != 0)
#define TURN_OFF_SetHIGH()                       TURN_OFF_PORT |= _BV(TURN_OFF_BIT)
#define TURN_OFF_SetLOW()                        TURN_OFF_PORT &= ~_BV(TURN_OFF_BIT)
#define TURN_OFF_Get()                           (TURN_OFF_PIN & _BV(TURN_OFF_BIT))
#define TURN_OFF_SetInactive()                   TURN_OFF_SetLOW()
#define TURN_OFF_SetActive()                     TURN_OFF_SetHIGH()
#define TURN_OFF_Toggle()                        TURN_OFF_PORT ^= _BV(TURN_OFF_BIT)
#define TURN_OFF_SetConditional(condition)       if (condition) TURN_OFF_SetActive(); else TURN_OFF_SetInactive()
#define TURN_OFF_SetConditionalToggle(conditionon, conditionoff, conditiontoggle) if (conditionon) { TURN_OFF_SetActive(); } else if (conditionoff) { TURN_OFF_SetInactive(); } else if (conditiontoggle) { TURN_OFF_Toggle(); }
#define TURN_OFF_Pulse()                         { TURN_OFF_SetActive(); TURN_OFF_SetInactive(); }
#define TURN_OFF_PulseNop()                      { TURN_OFF_SetActive(); nop(); TURN_OFF_SetInactive(); }
#define TURN_OFF_IsInactive()                    (TURN_OFF_Get() == 0)
#define TURN_OFF_IsActive()                      (TURN_OFF_Get() != 0)
#define TURN_OFF_Init()                          { TURN_OFF_SetInactive(); TURN_OFF_ModeOutput(); }

#define initHAL() {\
		EXT_IN_Init();\
		AUX_IN_Init();\
		TURN_OFF_Init();\
}

#endif
