/*
	tiny13softoff - ATtiny13 based soft-off circuit.
	Copyright (C) 2020-2020 Johannes Bauer

	This file is part of tiny13softoff.

	tiny13softoff is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; this program is ONLY licensed under
	version 3 of the License, later versions are explicitly excluded.

	tiny13softoff is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with tiny13softoff; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

	Johannes Bauer <JohannesBauer@gmx.de>
*/

#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "hal.h"

enum ext_function_t {
	EXT_DISABLED = 0,
	EXT_LEVEL_RESET_TIMER = 1,
	EXT_EDGE_RESET_TIMER = 2,
};

enum aux_function_t {
	AUX_DISABLED = 0,
	AUX_LEVEL_INSTANT_SHUTOFF = 1,
	AUX_EDGE_INSTANT_SHUTOFF = 2,
};

struct config_block_t {
	uint32_t magic;
	uint32_t shutoff_after_secs;
	enum ext_function_t ext_function;
	bool ext_in_pullup, ext_in_activelow;
	enum aux_function_t aux_function;
	bool aux_in_pullup, aux_in_activelow;
	uint8_t aux_deadtime_ticks;
} __attribute__ ((packed));

struct runtime_data_t {
	uint32_t no_activity_secs;
	uint8_t ticks;
	uint8_t aux_deadtime;
};

struct config_block_t config = {
	.magic = 0xfc9b2f17,
	.shutoff_after_secs = 10,
	.ext_function = EXT_LEVEL_RESET_TIMER,
	.ext_in_pullup = true,
	.ext_in_activelow = true,
	.aux_in_pullup = true,
	.aux_function = AUX_DISABLED,
};
static struct runtime_data_t runtime;

static bool ext_active(void) {
	return EXT_IN_IsActive() ^ config.ext_in_activelow;
}

static bool aux_active(void) {
	return (runtime.aux_deadtime == 0) && (AUX_IN_IsActive() ^ config.aux_in_activelow);
}


static void do_shutoff(void) {
	TURN_OFF_SetActive();

	/* Hang device in low power, disable interrupts and enter deep sleep */
	TCCR0B = 0;		 /* Disable timer/counter entirely */
	cli();
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	while (true) {
		sleep_mode();
	}
}

static void second_tick(void) {
	runtime.no_activity_secs++;
	if (runtime.no_activity_secs > config.shutoff_after_secs) {
		do_shutoff();
	}

	if ((config.ext_function == EXT_LEVEL_RESET_TIMER) && ext_active()) {
		runtime.no_activity_secs = 0;
	}

	if ((config.aux_function == AUX_LEVEL_INSTANT_SHUTOFF) && aux_active()) {
		do_shutoff();
	}
}

ISR(PCINT0_vect) {
	if ((config.ext_function == EXT_EDGE_RESET_TIMER) && (ext_active())) {
		runtime.no_activity_secs = 0;
	}

	if ((config.aux_function == AUX_EDGE_INSTANT_SHUTOFF) && (aux_active())) {
		do_shutoff();
	}
}

ISR(TIM0_OVF_vect) {
	runtime.ticks++;
	if (runtime.ticks >= 2) {
		runtime.ticks = 0;
		second_tick();
	}
	if (runtime.aux_deadtime) {
		runtime.aux_deadtime--;
	}
}

int main(void) {
	initHAL();


	/* PB1 and PB2 are entirely unused: AIN1 and ADC1
	 * PB4 is an output: ADC2
	 * RESET pin is ADC0
	 */
	DIDR0 |= _BV(ADC0D) | _BV(ADC1D) | _BV(ADC2D) | _BV(AIN1D);

	/* External pin is PB3 aka ADC3 */
	if (config.ext_function == EXT_DISABLED) {
		DIDR0 |= _BV(ADC3D);
		EXT_IN_SetPullupActive();
	} else {
		if (config.ext_in_pullup) {
			EXT_IN_SetPullupActive();
		}
	}

	/* Aux pin is PB0 aka AIN0 */
	if (config.aux_function == AUX_DISABLED) {
		DIDR0 |= _BV(AIN0D);
		AUX_IN_SetPullupActive();
	} else {
		if (config.aux_in_pullup) {
			AUX_IN_SetPullupActive();
		}
	}

	if ((config.ext_function == EXT_EDGE_RESET_TIMER) || (config.aux_function == AUX_EDGE_INSTANT_SHUTOFF)) {
		/* We need at least one external interrupt */
		GIMSK |= _BV(PCIE);			/* Pin change interrupt enable */

		if (config.ext_function == EXT_EDGE_RESET_TIMER) {
			PCMSK |= _BV(PCINT3);
		}

		if (config.aux_function == AUX_EDGE_INSTANT_SHUTOFF) {
			PCMSK |= _BV(PCINT0);
		}
	}

	runtime.aux_deadtime = config.aux_deadtime_ticks;

	TCCR0B = _BV(CS00);		/* No prescaling, overflow every 512ms (~ 2 Hz) at 500 Hz core clock */
	TIMSK0 = _BV(TOIE0);

	/* Prepare for IDLE mode later on */
	set_sleep_mode(SLEEP_MODE_IDLE);

	/* Drop clock rate from 16 kHz core to 500 Hz */
	CLKPR = _BV(CLKPCE);
	CLKPR = _BV(CLKPS3);
	sei();

	while (true) {
		sleep_mode();
	}

	return 0;
}
