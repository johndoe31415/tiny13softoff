/* Automatically generated HAL from hal.xml */
/* NEVER EDIT MANUALLY */

/* Generated on: 2020-10-11 12:29:14 */

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

/* Unused1 -> PB2 (Output, Initially Inactive) */
#define Unused1_BIT                              2
#define Unused1_PIN                              PINB
#define Unused1_PORT                             PORTB
#define Unused1_DDR                              DDRB
#define Unused1_ModeOutput()                     Unused1_DDR |= _BV(Unused1_BIT)
#define Unused1_IsOutput()                       ((Unused1_DDR & _BV(Unused1_BIT)) != 0)
#define Unused1_SetHIGH()                        Unused1_PORT |= _BV(Unused1_BIT)
#define Unused1_SetLOW()                         Unused1_PORT &= ~_BV(Unused1_BIT)
#define Unused1_Get()                            (Unused1_PIN & _BV(Unused1_BIT))
#define Unused1_SetInactive()                    Unused1_SetLOW()
#define Unused1_SetActive()                      Unused1_SetHIGH()
#define Unused1_Toggle()                         Unused1_PORT ^= _BV(Unused1_BIT)
#define Unused1_SetConditional(condition)        if (condition) Unused1_SetActive(); else Unused1_SetInactive()
#define Unused1_SetConditionalToggle(conditionon, conditionoff, conditiontoggle) if (conditionon) { Unused1_SetActive(); } else if (conditionoff) { Unused1_SetInactive(); } else if (conditiontoggle) { Unused1_Toggle(); }
#define Unused1_Pulse()                          { Unused1_SetActive(); Unused1_SetInactive(); }
#define Unused1_PulseNop()                       { Unused1_SetActive(); nop(); Unused1_SetInactive(); }
#define Unused1_IsInactive()                     (Unused1_Get() == 0)
#define Unused1_IsActive()                       (Unused1_Get() != 0)
#define Unused1_Init()                           { Unused1_SetInactive(); Unused1_ModeOutput(); }

/* Unused2 -> PB1 (Output, Initially Inactive) */
#define Unused2_BIT                              1
#define Unused2_PIN                              PINB
#define Unused2_PORT                             PORTB
#define Unused2_DDR                              DDRB
#define Unused2_ModeOutput()                     Unused2_DDR |= _BV(Unused2_BIT)
#define Unused2_IsOutput()                       ((Unused2_DDR & _BV(Unused2_BIT)) != 0)
#define Unused2_SetHIGH()                        Unused2_PORT |= _BV(Unused2_BIT)
#define Unused2_SetLOW()                         Unused2_PORT &= ~_BV(Unused2_BIT)
#define Unused2_Get()                            (Unused2_PIN & _BV(Unused2_BIT))
#define Unused2_SetInactive()                    Unused2_SetLOW()
#define Unused2_SetActive()                      Unused2_SetHIGH()
#define Unused2_Toggle()                         Unused2_PORT ^= _BV(Unused2_BIT)
#define Unused2_SetConditional(condition)        if (condition) Unused2_SetActive(); else Unused2_SetInactive()
#define Unused2_SetConditionalToggle(conditionon, conditionoff, conditiontoggle) if (conditionon) { Unused2_SetActive(); } else if (conditionoff) { Unused2_SetInactive(); } else if (conditiontoggle) { Unused2_Toggle(); }
#define Unused2_Pulse()                          { Unused2_SetActive(); Unused2_SetInactive(); }
#define Unused2_PulseNop()                       { Unused2_SetActive(); nop(); Unused2_SetInactive(); }
#define Unused2_IsInactive()                     (Unused2_Get() == 0)
#define Unused2_IsActive()                       (Unused2_Get() != 0)
#define Unused2_Init()                           { Unused2_SetInactive(); Unused2_ModeOutput(); }

#define initHAL() {\
		EXT_IN_Init();\
		AUX_IN_Init();\
		TURN_OFF_Init();\
		Unused1_Init();\
		Unused2_Init();\
}

#endif
