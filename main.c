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

enum pin_function_t {
	FNC_DISABLED = 0,
	FNC_LEVEL_RESET_TIMER = 1,
	FNC_EDGE_RESET_TIMER = 2,
	FNC_LEVEL_INSTANT_SHUTOFF = 3,
	FNC_EDGE_INSTANT_SHUTOFF = 4,
	FNC_UNDERVOLTAGE_SHUTOFF = 5,		/* Only available on EXT */
};

struct pin_config_t {
	enum pin_function_t function;
	bool pullup_active;
	bool active_low;
	uint8_t deadtime_ticks;
} __attribute__ ((packed));

struct config_block_t {
	uint32_t magic;
	uint32_t shutoff_after_secs;
	struct pin_config_t ext_pin;
	struct pin_config_t aux_pin;
	uint16_t undervoltage_shutoff_threshold_adu;
	uint8_t undervoltage_ticks_threshold;
} __attribute__ ((packed));

struct runtime_data_t {
	uint32_t no_activity_secs;
	uint8_t undervoltage_counts;
	uint8_t ticks;
	bool second_passed;
};

struct config_block_t config = {
	.magic = 0xfc9b2f17,
	.shutoff_after_secs = 60,
	.ext_pin = {
		.function = FNC_UNDERVOLTAGE_SHUTOFF,
	},
	.aux_pin = {
		.function = FNC_LEVEL_INSTANT_SHUTOFF,
		.pullup_active = true,
		.active_low = true,
		.deadtime_ticks = 2,
	},
	.undervoltage_shutoff_threshold_adu = 879,		/* 3.4V with 39k / 15k predivider (4.0V -> 1.1V) */
	.undervoltage_ticks_threshold = 6,
};
static struct runtime_data_t runtime;

static bool ext_active(void) {
	return (config.ext_pin.deadtime_ticks == 0) && (EXT_IN_IsActive() ^ config.ext_pin.active_low);
}

static bool aux_active(void) {
	return (config.aux_pin.deadtime_ticks == 0) && (AUX_IN_IsActive() ^ config.aux_pin.active_low);
}

static void do_shutoff(void) {
	TURN_OFF_SetActive();

	while (true) {
		TURN_OFF_Toggle();		/* TODO REMOVEME DEBUG ONLY */
	}

	/* Hang device in low power, disable interrupts and enter deep sleep */
	TCCR0B = 0;		 /* Disable timer/counter entirely */
	cli();
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	while (true) {
		sleep_mode();
	}
}

static void do_pin_action(enum pin_function_t function) {
	if ((function == FNC_LEVEL_RESET_TIMER) || (function == FNC_EDGE_RESET_TIMER)) {
		runtime.no_activity_secs = 0;
	} else if ((function == FNC_LEVEL_INSTANT_SHUTOFF) || (function == FNC_EDGE_INSTANT_SHUTOFF)) {
		do_shutoff();
	}
}

static void poll_pins(void) {
	if (ext_active()) {
		do_pin_action(config.ext_pin.function);
	}
	if (aux_active()) {
		do_pin_action(config.aux_pin.function);
	}
}

static void set_clockrate_slow(void) {
	/* Drop clock rate from 128 kHz core to 500 Hz */
	CLKPR = _BV(CLKPCE);
	CLKPR = _BV(CLKPS3);
}

static void set_clockrate_fast(void) {
	/* Raise core clock rate to 128 kHz */
	CLKPR = _BV(CLKPCE);
	CLKPR = 0;
}

static void trigger_adc(void) {
	/* ADC needs minimum clock frequency of 50 kHz; we need to raise core clock
	 * to 128 kHz for that */
	set_clockrate_fast();

	/* Enable ADC and enable ADC complete interrupt */
	ADCSRA = _BV(ADEN) | _BV(ADIE);

	/* Start conversion and enter sleep mode */
	set_sleep_mode(SLEEP_MODE_ADC);
	sleep_mode();

	/* Afterwards, drop clockrate again */
	set_clockrate_slow();
}

ISR(ADC_vect) {
	/* Conversion just completed */
	uint16_t adu = ADC;
	if (adu < config.undervoltage_shutoff_threshold_adu) {
		runtime.undervoltage_counts++;
		if (runtime.undervoltage_counts >= config.undervoltage_ticks_threshold) {
			/* Undervoltage shutoff */
			do_shutoff();
		}
	}
}

static void second_tick(void) {
	runtime.no_activity_secs++;
	runtime.second_passed = true;
	if (runtime.no_activity_secs > config.shutoff_after_secs) {
		do_shutoff();
	}
	poll_pins();
}

ISR(PCINT0_vect) {
	poll_pins();
}

ISR(TIM0_OVF_vect) {
	/* One tick every 500ms */
	runtime.ticks++;
	if (runtime.ticks >= 2) {
		runtime.ticks = 0;
		second_tick();
	}
	if (config.ext_pin.deadtime_ticks) {
		config.ext_pin.deadtime_ticks--;
	}
	if (config.aux_pin.deadtime_ticks) {
		config.aux_pin.deadtime_ticks--;
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
	if (config.ext_pin.function == FNC_DISABLED) {
		DIDR0 |= _BV(ADC3D);
		EXT_IN_SetPullupActive();
	} else if (config.ext_pin.function == FNC_UNDERVOLTAGE_SHUTOFF) {
		DIDR0 |= _BV(ADC3D);
		ADMUX = _BV(REFS0) | _BV(MUX1) | _BV(MUX0);
	} else {
		if (config.ext_pin.pullup_active) {
			EXT_IN_SetPullupActive();
		}
	}

	/* Aux pin is PB0 aka AIN0 */
	if (config.aux_pin.function == FNC_DISABLED) {
		DIDR0 |= _BV(AIN0D);
		AUX_IN_SetPullupActive();
	} else {
		if (config.aux_pin.pullup_active) {
			AUX_IN_SetPullupActive();
		}
	}

	if ((config.ext_pin.function == FNC_EDGE_RESET_TIMER) || (config.ext_pin.function == FNC_EDGE_INSTANT_SHUTOFF)
			|| (config.aux_pin.function == FNC_EDGE_RESET_TIMER) || (config.aux_pin.function == FNC_EDGE_INSTANT_SHUTOFF)) {
		/* We need at least one external interrupt */
		GIMSK |= _BV(PCIE);			/* Pin change interrupt enable */

		if ((config.ext_pin.function == FNC_EDGE_RESET_TIMER) || (config.ext_pin.function == FNC_EDGE_INSTANT_SHUTOFF)) {
			PCMSK |= _BV(PCINT3);
		}

		if ((config.aux_pin.function == FNC_EDGE_RESET_TIMER) || (config.aux_pin.function == FNC_EDGE_INSTANT_SHUTOFF)) {
			PCMSK |= _BV(PCINT0);
		}
	}

	TCCR0B = _BV(CS00);		/* No prescaling, overflow every 512ms (~ 2 Hz) at 500 Hz core clock */
	TIMSK0 = _BV(TOIE0);

	set_clockrate_slow();
	sei();

	while (true) {
		if (runtime.second_passed && (config.ext_pin.function == FNC_UNDERVOLTAGE_SHUTOFF)) {
			runtime.second_passed = false;
			trigger_adc();
		} else {
			/* Sleep in IDLE mode */
			set_sleep_mode(SLEEP_MODE_IDLE);
			sleep_mode();
		}
	}

	return 0;
}
