/*
  xsns_96_flowratemeter.ino - flowratemeter support for Tasmota
                              - up to two flowratemeter YF-DN50 and similary
                              - flow rate frequencies f = 1 Hz up to 5 kHz
                              - uses the FreqRes resolution

  Copyright (C) 2022  Norbert Richter

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

  Aide & tuto Sensors API:
  https://tasmota.github.io/docs/Sensor-API/#managing-a-forked-branch
*/

#ifdef USE_FLOWRATEMETER

#define XSNS_96                       96

//#define FLOWRATEMETER_WEIGHT_AVG_SAMPLE   20  // number of samples for smooth weigted average
#define FLOWRATEMETER_MIN_FREQ             1  // Hz

#define D_JSON_FLOWRATEMETER_COUNT          "frequence"
#define D_JSON_FLOWRATEMETER_RATE           "debit"
#define D_JSON_FLOWRATEMETER_VALUE          "Type"
#define D_JSON_FLOWRATEMETER_UNIT           "Unite"
#define D_JSON_FLOWRATEMETER_REGLAGE        "Reglage"
#define D_JSON_FLOWRATEMETER_TEST           "Test"
#define D_JSON_FLOWRATEMETER_AVG_SAMPLE     "nbSample"
#define D_JSON_FLOWRATEMETER_AMOUNT_TODAY   "qteJour"
#define D_JSON_FLOWRATEMETER_AMOUNT_UNIT    "qteUnite"
#define D_JSON_FLOWRATEMETER_DURATION_TODAY "dureeJour"
#define D_JSON_FLOWRATEMETER_VALUE_AVG      "moyenne"
#define D_JSON_FLOWRATEMETER_SHOW_FREQ      "affichageFreq"
#define D_JSON_FLOWRATEMETER_VALUE_RAW      "donneeBrute"
#define D_JSON_FLOWRATEMETERS               "Debitmetres"

#define FLOWRATEMETER_INVALID (uint32_t)-1

int32_t flowratemeter_period[MAX_FLOWRATEMETER] = {0};
float   flowratemeter_period_avg[MAX_FLOWRATEMETER] = {0};
uint32_t flowratemeter_count[MAX_FLOWRATEMETER] = {0};
volatile uint32_t flowratemeter_last_irq[MAX_FLOWRATEMETER] = {FLOWRATEMETER_INVALID};

uint32_t flowratemeter_period_sum[MAX_FLOWRATEMETER];
int32_t flowratemeter_period_sum_dT[MAX_FLOWRATEMETER];
int32_t flowratemeter_period_duration[MAX_FLOWRATEMETER];

bool bool_flowratemeter_test = false;

#define FlowRateMeterIsValid(time, meter) flowratemeter_last_irq[meter] != FLOWRATEMETER_INVALID && flowratemeter_last_irq[meter] < time

// Basic data calculation function by interrupt on GPIO pin
void IRAM_ATTR FlowRateMeterIR(uint16_t irq) {
	uint32_t time = micros();
#if defined(ESP8266)
	uint32_t gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);
	GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status);
#endif	

	// If the index of the sensor is indeed < MAX_FLOWRATEMETER (6)
	if (irq < MAX_FLOWRATEMETER) {
		// Is invalid if (1st measure / noted invalid / last_irq < time)
		if (FlowRateMeterIsValid(time, irq)) {
			// frequency(Hz) = 1/Period(s) -> T(microS)=1000000(microS)/f
			// freqCalculated > freqMin -> 1/Tcalc > 1/Tmin -> Tcalc < Tmin -> Tcalc < 1/freqMin
			if ((time - flowratemeter_last_irq[irq]) < (1000000 / FLOWRATEMETER_MIN_FREQ)) {
				flowratemeter_period_sum_dT[irq] = millis();
				flowratemeter_period_sum[irq] ++;

                flowratemeter_period[irq] = time - flowratemeter_last_irq[irq];
				flowratemeter_period_duration[irq] += flowratemeter_period[irq] / 100;
			} else {
				flowratemeter_period[irq] = 0;
			}
		}
		
		// Saves the timer (microS) of the measurement
		flowratemeter_last_irq[irq] = time;
	}
}

// Fonctions de callBack
// GPIO_STATUS is always 0 (?), so can only determine the IR source using this way
void IRAM_ATTR FlowRateMeter1IR(void) {FlowRateMeterIR(0);}
void IRAM_ATTR FlowRateMeter2IR(void) {FlowRateMeterIR(1);}
void IRAM_ATTR FlowRateMeter3IR(void) {FlowRateMeterIR(2);}
void IRAM_ATTR FlowRateMeter4IR(void) {FlowRateMeterIR(3);}
void IRAM_ATTR FlowRateMeter5IR(void) {FlowRateMeterIR(4);}
void IRAM_ATTR FlowRateMeter6IR(void) {FlowRateMeterIR(5);}

// Reset counters at midnight
void FlowRateMeterMidnightReset(void) {
    uint32_t t = millis();

    for (uint32_t i = 0; i < MAX_FLOWRATEMETER; i++) {
        flowratemeter_period_sum[i] = 0;
        flowratemeter_period_duration[i] = 0;
        flowratemeter_period_sum_dT[i] = t;
    }
}

// Reads counters every 250ms
void FlowRateMeterRead(void) {
    for (uint32_t i = 0; i < MAX_FLOWRATEMETER; i++) {
        uint32_t time = micros();
        if (PinUsed(GPIO_FLOWRATEMETER_IN, i) && FlowRateMeterIsValid(time, i)) {
            // flowratemeter_period > 1000000 microS
            if ((time - flowratemeter_last_irq[i]) >= (1000000 / FLOWRATEMETER_MIN_FREQ)) {
                // invalid in case of pulse outage
                flowratemeter_period[i] = 0;
                flowratemeter_period_avg[i] = 0;
                flowratemeter_count[i] = 0;
                flowratemeter_last_irq[i] = FLOWRATEMETER_INVALID;
            }

            // Calculation of an average according to a defined sampling 'FLOWRATEMETER_WEIGHT_AVG_SAMPLE'
            // exponentially weighted average
            if (flowratemeter_count[i] <= Settings->sbflag1.flowratemeter_weight_avg_sample) {
                flowratemeter_count[i] ++;
            }

            flowratemeter_period_avg[i] -= flowratemeter_period_avg[i] / flowratemeter_count[i];
            flowratemeter_period_avg[i] += float(flowratemeter_period[i]) / flowratemeter_count[i];
        }
    }
}

void FlowRateMeterInit(void) {
    // To avoid unwanted triggering of the interrupt if the pin is not connected
    void (*irq_service[MAX_FLOWRATEMETER]) (void) = {
        (MAX_FLOWRATEMETER >= 1) ? FlowRateMeter1IR : nullptr, 
        (MAX_FLOWRATEMETER >= 2) ? FlowRateMeter2IR : nullptr, 
        (MAX_FLOWRATEMETER >= 3) ? FlowRateMeter3IR : nullptr, 
        (MAX_FLOWRATEMETER >= 4) ? FlowRateMeter4IR : nullptr, 
        (MAX_FLOWRATEMETER >= 5) ? FlowRateMeter5IR : nullptr, 
        (MAX_FLOWRATEMETER >= 6) ? FlowRateMeter6IR : nullptr
    };

    FlowRateMeterMidnightReset();
    for (uint32_t i = 0; i < MAX_FLOWRATEMETER; i++) {
        if (PinUsed(GPIO_FLOWRATEMETER_IN, i)) {
            pinMode(Pin(GPIO_FLOWRATEMETER_IN, i), INPUT);
            attachInterrupt(Pin(GPIO_FLOWRATEMETER_IN, i), irq_service[i], RISING);
        }
    }
}

// Get the values ​​to display
void FlowRateMeterGetValue(uint32_t meter, uint32_t *count, float *rate_float, float *amount_today, float *rateMin, float *rateMax) {
    float _flowratemeter_count = 0.0;
    float _lmin = 0.0;

    // Generation of random pulse counters in test case
    if(Settings->sbflag1.flowratemeter_test) {
        _flowratemeter_count = random(1, 4000);

        *count = (uint32_t)_flowratemeter_count;
        *rate_float = _flowratemeter_count * 2.02 * 60 / 1000;
        *amount_today = 0.0;

        goto calculExtremes;
    }

    if (nullptr != count) {
        *count = 0;
        _flowratemeter_count = 0.0;
        if (meter < MAX_FLOWRATEMETER && flowratemeter_period[meter]) {
            // nb impulsions / s
            _flowratemeter_count = 1000000.0 
                / (Settings->sbflag1.flowratemeter_raw_value ? flowratemeter_period[meter] : flowratemeter_period_avg[meter])
                * ((Settings->flowratemeter_calibration[meter] ? (float)Settings->flowratemeter_calibration[meter] : 1000.0) / 1000);
            *count = (uint32_t)_flowratemeter_count;
        }
    }

    // Measuring sensor 1000ml = 660 pulses (red label) ==> measurementsFlow.flow = measurementsFlow.frequency * 1.52
    // Measuring sensor 1000ml = 495 pulses (black label) ==> measurementsFlow.flow = measurementsFlow.frequency * 2.02
    if (nullptr != rate_float) {
        *rate_float = 0;
        if (meter < MAX_FLOWRATEMETER && flowratemeter_period[meter]) {
            *rate_float = _flowratemeter_count * 2.02 * 60 / 1000; // flow in L/min: 1 pulse = 2.02ml
        }
    }

    if (nullptr != amount_today) {
        *amount_today = 0;
        if (meter < MAX_FLOWRATEMETER && flowratemeter_period_sum[meter]) {
            _flowratemeter_count = 1000000.0 
                / (uint32_t)((float)flowratemeter_period_sum_dT[meter] / (float)flowratemeter_period_sum[meter] * 1000.0)       //en uS
                * ((Settings->flowratemeter_calibration[meter] ? (float)Settings->flowratemeter_calibration[meter] : 1000.0) / 1000);
            _lmin = _flowratemeter_count * 2.02 * 60 / 1000;       // flow in L/min: 1 pulse = 2.02ml
            *amount_today = _lmin * ((flowratemeter_period_sum_dT[meter] / 1000) / 60);
        }
    }

    // Determines the max and min values ​​of flow
calculExtremes:
    if (*rate_float < *rateMin) {*rateMin = *rate_float;}
    if (*rate_float > *rateMax) {*rateMax = *rate_float;}
}

void FlowRateMeterShow(bool json) {
    uint16_t flowmeter_count = 0;
    const char* open_square_bracket;
    const char* close_square_bracket;
    float flowratemeter_rate_float[MAX_FLOWRATEMETER];
    float floatrate_amount_today[MAX_FLOWRATEMETER];
    uint32_t flowratemeter_count[MAX_FLOWRATEMETER];
    static float min[MAX_FLOWRATEMETER] = {9999.9};
    static float max[MAX_FLOWRATEMETER] = {0};
    float rate;
    float amount_today;

    for (uint32_t i = 0; i < MAX_FLOWRATEMETER; i++) {
        // Tests if there is at least 1 flow sensor
        FlowRateMeterGetValue(i, &flowratemeter_count[i], &flowratemeter_rate_float[i], &floatrate_amount_today[i], &min[i], &max[i]);
        if (PinUsed(GPIO_FLOWRATEMETER_IN, i)) {
            flowmeter_count++;
        }
    }

    if (flowmeter_count > 1) {
        open_square_bracket = PSTR("[");
        close_square_bracket = PSTR("]");
    } else {
        open_square_bracket = PSTR("");
        close_square_bracket = PSTR("");
    }

    if (json) {
        ResponseAppend_P(PSTR(",\"" D_JSON_FLOWRATEMETERS "\":{"));
        for (uint32_t i = 0; i < MAX_FLOWRATEMETER; i++) {
            if (PinUsed(GPIO_FLOWRATEMETER_IN, i)) {
                // To be modified according to test mode -> random generation
                // If TEST=ON ==> period generation of random digits
                if (Settings->sbflag1.flowratemeter_unit == 0) {
                    rate = flowratemeter_rate_float[i];
                    amount_today = floatrate_amount_today[i];
                } else if (Settings->sbflag1.flowratemeter_unit == 1) {
                    rate = flowratemeter_rate_float[i] * 60;
                    amount_today = floatrate_amount_today[i];
                } else if (Settings->sbflag1.flowratemeter_unit == 2) {
                    rate = flowratemeter_rate_float[i] * 60 / 1000;
                    amount_today = floatrate_amount_today[i] / 1000;
                }

                if (i == 0) {
                    ResponseAppend_P(PSTR("\"" D_JSON_FLOWRATEMETER "%d\":{\"name\":\"%s\","), i + 1, FLOWRATEMETER1);
                } else if (i == 1) {
                    ResponseAppend_P(PSTR("\"" D_JSON_FLOWRATEMETER "%d\":{\"name\":\"%s\","), i + 1, FLOWRATEMETER2);
                } else if (i == 2) {
                    ResponseAppend_P(PSTR("\"" D_JSON_FLOWRATEMETER "%d\":{\"name\":\"%s\","), i + 1, FLOWRATEMETER3);
                } else if (i == 3) {
                    ResponseAppend_P(PSTR("\"" D_JSON_FLOWRATEMETER "%d\":{\"name\":\"%s\","), i + 1, FLOWRATEMETER4);
                } else if (i == 4) {
                    ResponseAppend_P(PSTR("\"" D_JSON_FLOWRATEMETER "%d\":{\"name\":\"%s\","), i + 1, FLOWRATEMETER5);
                } else if (i == 5) {
                    ResponseAppend_P(PSTR("\"" D_JSON_FLOWRATEMETER "%d\":{\"name\":\"%s\","), i + 1, FLOWRATEMETER6);
                }
                ResponseAppend_P(PSTR("\"" D_JSON_FLOWRATEMETER_COUNT "\":%d,"), flowratemeter_count[i]);
                ResponseAppend_P(PSTR("\"" D_JSON_FLOWRATEMETER_RATE "\":%*_f,"),
                        Settings->flag2.frequency_resolution, &rate);
                ResponseAppend_P(PSTR("\"" D_JSON_FLOWRATEMETER_AMOUNT_TODAY "\":%*_f,"),
                        Settings->flag2.frequency_resolution, &amount_today);
                ResponseAppend_P(PSTR("\"" D_JSON_FLOWRATEMETER_DURATION_TODAY "\":%ld"),
                        GetDuration(flowratemeter_period_duration[i] / 10000, false).c_str());
                ResponseAppend_P(PSTR(",\"extremes\":{\"min\":%*_f,\"max\":%*_f}"),
                        Settings->flag2.frequency_resolution, &min[i], Settings->flag2.frequency_resolution, &max[i]);
                ResponseAppend_P(PSTR("},"));
            }
        }

        ResponseAppend_P(PSTR("\"" D_JSON_FLOWRATEMETER_VALUE "\":\"%s\""),
                Settings->sbflag1.flowratemeter_raw_value ? PSTR(D_JSON_FLOWRATEMETER_VALUE_RAW) : PSTR(D_JSON_FLOWRATEMETER_VALUE_AVG));
        if (Settings->sbflag1.flowratemeter_raw_value == 0) {
            ResponseAppend_P(PSTR(",\"" D_JSON_FLOWRATEMETER_AVG_SAMPLE "\":%d"),
                    Settings->sbflag1.flowratemeter_weight_avg_sample
            );
        }

        ResponseAppend_P(PSTR(",\"unites\":{"));
        if (Settings->sbflag1.flowratemeter_unit == 0) {
            ResponseAppend_P(PSTR("\"" D_JSON_FLOWRATEMETER_AMOUNT_UNIT "\":\"%s\",\"" D_JSON_FLOWRATEMETER_UNIT "\":\"%s\""), 
                    PSTR(D_UNIT_LITERS), PSTR(D_UNIT_LITER_PER_MINUTE));
        } else if (Settings->sbflag1.flowratemeter_unit == 1) {
            ResponseAppend_P(PSTR("\"" D_JSON_FLOWRATEMETER_AMOUNT_UNIT "\":\"%s\",\"" D_JSON_FLOWRATEMETER_UNIT "\":\"%s\""), 
                    PSTR(D_UNIT_LITERS), PSTR(D_UNIT_LITER_PER_HOUR));          
        } else if (Settings->sbflag1.flowratemeter_unit == 2) {
            ResponseAppend_P(PSTR("\"" D_JSON_FLOWRATEMETER_AMOUNT_UNIT "\":\"%s\",\"" D_JSON_FLOWRATEMETER_UNIT "\":\"%s\""), 
                    PSTR(D_UNIT_CUBIC_METER), PSTR(D_UNIT_CUBICMETER_PER_HOUR));  
        }

        ResponseAppend_P(PSTR("},\"" D_JSON_FLOWRATEMETER_REGLAGE "\":\"%s\""),
                Settings->sbflag1.flowratemeter_reglage ? PSTR("ON") : PSTR("OFF"));
        ResponseAppend_P(PSTR(",\"" D_JSON_FLOWRATEMETER_TEST "\":\"%s\"}"),
                Settings->sbflag1.flowratemeter_test ? PSTR("ON") : PSTR("OFF"));
#ifdef USE_WEBSERVER
    } else {
        // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
        // head
        WSContentSend_PD(PSTR("{s}&nbsp;</th>&nbsp;<th></th>"));
        if (Settings->sbflag1.flowratemeter_show_freq) {
            WSContentSend_PD(PSTR("<th style=\"text-align:%s\">" D_FLOWRATEMETER_COUNT "</th><th>&nbsp;</th>"),
                    Settings->flag5.gui_table_align ? PSTR("right") : PSTR("center"));
        }
        WSContentSend_PD(PSTR("<th style=\"text-align:%s\">" D_FLOWRATEMETER_NAME "</th><th>&nbsp;</th>"),
                Settings->flag5.gui_table_align ? PSTR("right") : PSTR("center"));
        WSContentSend_PD(PSTR("<th style=\"text-align:%s\">" D_FLOWRATEMETER_AMOUNT_TODAY "</th><th>&nbsp;</th>"),
                Settings->flag5.gui_table_align ? PSTR("right") : PSTR("center"));
        WSContentSend_PD(PSTR("<th style=\"text-align:%s\">" D_FLOWRATEMETER_DURATION_TODAY "</th><th>&nbsp;</th>"),
                Settings->flag5.gui_table_align ? PSTR("right") : PSTR("center"));
        WSContentSend_PD(PSTR("<th></th></tr>"));

        for (uint32_t i = 0; i < MAX_FLOWRATEMETER; i++) {
            if (PinUsed(GPIO_FLOWRATEMETER_IN, i)) {
                if (i == 0) {
                    WSContentSend_PD(PSTR("{s}" FLOWRATEMETER1 "{m}&nbsp;</td>"));
                } else if (i == 1) {
                    WSContentSend_PD(PSTR("{s}" FLOWRATEMETER2 "{m}&nbsp;</td>"));
                } else if (i == 2) {
                    WSContentSend_PD(PSTR("{s}" FLOWRATEMETER3 "{m}&nbsp;</td>"));
                } else if (i == 3) {
                    WSContentSend_PD(PSTR("{s}" FLOWRATEMETER4 "{m}&nbsp;</td>"));
                } else if (i == 4) {
                    WSContentSend_PD(PSTR("{s}" FLOWRATEMETER5 "{m}&nbsp;</td>"));
                } else if (i == 5) {
                    WSContentSend_PD(PSTR("{s}" FLOWRATEMETER6 "{m}&nbsp;</td>"));
                }

                // To be modified according to test mode -> random generation
                // If TEST=ON ==> period generation of random digits
                if (Settings->sbflag1.flowratemeter_unit == 0) {
                    rate = flowratemeter_rate_float[i];
                    amount_today = floatrate_amount_today[i];
                } else if (Settings->sbflag1.flowratemeter_unit == 1) {
                    rate = flowratemeter_rate_float[i] * 60;
                    amount_today = floatrate_amount_today[i];
                } else if (Settings->sbflag1.flowratemeter_unit == 2) {
                    rate = flowratemeter_rate_float[i] * 60 / 1000;
                    amount_today = floatrate_amount_today[i] / 1000;
                }

                // Number of pulses
                if (Settings->sbflag1.flowratemeter_show_freq) {
                    WSContentSend_PD(PSTR("<td style=\"text-align:%s\">%d</td><td>&nbsp;</td>"),
                            Settings->flag5.gui_table_align ? PSTR("right") : PSTR("center"),
                            flowratemeter_count[i]
                    );
                }

                // Flowrate
                if (Settings->sbflag1.flowratemeter_unit == 0) {
                    WSContentSend_PD(PSTR("<td style=\"text-align:%s\">%*_f %s</td><td>&nbsp;</td>"),
                            Settings->flag5.gui_table_align ? PSTR("right") : PSTR("center"),
                            Settings->flag2.frequency_resolution, &rate,
                            PSTR(D_UNIT_LITER_PER_MINUTE));
                } else if (Settings->sbflag1.flowratemeter_unit == 1) {
                    WSContentSend_PD(PSTR("<td style=\"text-align:%s\">%*_f %s</td><td>&nbsp;</td>"),
                            Settings->flag5.gui_table_align ? PSTR("right") : PSTR("center"),
                            Settings->flag2.frequency_resolution, &rate,
                            PSTR(D_UNIT_LITER_PER_HOUR));      
                } else if (Settings->sbflag1.flowratemeter_unit == 2) {
                    WSContentSend_PD(PSTR("<td style=\"text-align:%s\">%*_f %s</td><td>&nbsp;</td>"),
                            Settings->flag5.gui_table_align ? PSTR("right") : PSTR("center"),
                            Settings->flag2.frequency_resolution, &rate,
                            PSTR(D_UNIT_CUBICMETER_PER_HOUR));   
                }

                // Amount today
                WSContentSend_PD(PSTR("<td style=\"text-align:%s\">%*_f %s</td><td>&nbsp;</td>"),
                        Settings->flag5.gui_table_align ? PSTR("right") : PSTR("center"),
                        Settings->flag2.frequency_resolution, &amount_today,
                        Settings->sbflag1.flowratemeter_unit < 2 ? PSTR(D_UNIT_LITERS) : PSTR(D_UNIT_CUBIC_METER)
                );

                // Duration today
                WSContentSend_PD(PSTR("<td style=\"text-align:%s\">%s</td><td>&nbsp;</td>"),
                        (Settings->flag5.gui_table_align)?PSTR("right"):PSTR("center"),
                        GetDuration(flowratemeter_period_duration[i] / 10000, false).c_str()
                );

                WSContentSend_PD(PSTR("</tr>"));
            }
        }
#endif  // USE_WEBSERVER
    }
}

/*********************************************************************************************
 * Supported commands for Sensor96:
 * Sensor96 - Displays current settings

 * Sensor96 1 <correction-factor> - Correction factor of flowmeter1 (x 1000) - To set 0.2 enter 'Sensor96 1 200'
 * Sensor96 2 <correction-factor> - Flowmeter correction factor2 (x 1000)
 * Sensor96 3 <correction-factor> - Flowmeter correction factor2 (x 1000)
 * Sensor96 4 <correction-factor> - Flowmeter correction factor2 (x 1000)
 * Sensor96 5 <correction-factor> - Flowmeter correction factor2 (x 1000)
 * Sensor96 6 <correction-factor> - Flowmeter correction factor2 (x 1000)
 * 
 * Sensor96 10 0|1|2    - Show flow value in l/min (0) or l/h (1) or m³/h (2)
 * Sensor96 11 0|1      - Value mode: Switch between displaying avg(0) / raw(1) readings (not permanently)
 * Sensor96 12 0|1      - Value setting: Switch between telePeriod 300 setting(0) / telePeriod 3 setting(1)
 * Sensor96 13 0|1      - Value test: Switch between displaying random value test(0) / raw(1) readings (not permanently)
 * Sensor96 14 0->31    - Value weight avg sample (0 to 31)
 * Sensor96 15 0|1      - Reset total values
 * Sensor96 16 0|1      - Displays the frequency on the web page: NO(0) / YES(1)
 *
 * Calibration of flowmeters:
 * - current flow displayed (D)
 * - current <correction-factor> (c)
 * - actual measured flow (M)
 * - new <correction-factor> to apply = M / (c * D)
 *
* Example:
 * - current flow displayed = 254.39 l/min (D)
 * - current <correction-factor> = 1.0 (c)
 * - actual measured flow = 83.42 l/min (M)
 *
 * new <correction-factor> to apply = M / (c * D) = 83.42 / (1 * 254.39) = 0.328
 * Cmd: Sensor96x328
*********************************************************************************************/
bool FlowRateMeterCommand(void) {
    bool show_parms = true;
    char argument[XdrvMailbox.data_len];
    char *flowUnitStr;

    long value = 0;
    for (uint32_t ca = 0; ca < XdrvMailbox.data_len; ca++) {
        if ((' ' == XdrvMailbox.data[ca]) || ('=' == XdrvMailbox.data[ca])) { XdrvMailbox.data[ca] = ','; }
    }
    bool any_value = (strchr(XdrvMailbox.data, ',') != nullptr);

    if (any_value) {
        value = strtol(ArgV(argument, 2), nullptr, 10);
    }

    switch (XdrvMailbox.payload) {
        case 1:  // Sensor calibration value
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            if (any_value) {
                Settings->flowratemeter_calibration[XdrvMailbox.payload - 1] = value;
                ResponseCmndNumber(value);
                show_parms = true;
            }
            break;

        case 10:  // Unit
            if (any_value) {
                Settings->sbflag1.flowratemeter_unit = value & 0x03;
                ResponseCmndNumber(value & 0x03);
                show_parms = true;
            }
            break;
        case 11:  // avg/raw values
            if (any_value) {
                Settings->sbflag1.flowratemeter_raw_value = value & 1;
                ResponseCmndNumber(value & 1);
                show_parms = true;
            }
            break;
        case 12:  // setting: Teleperiod (300/3)
            if (any_value) {
                Settings->sbflag1.flowratemeter_reglage = value & 1;
                ResponseCmndNumber(value & 1);

                Settings->sbflag1.flowratemeter_reglage ? value = 3 : value = 300;
                Settings->tele_period = value;
                
                show_parms = true;
            }
            break;
        case 13:  // test : random value /real
            if (any_value) {
                Settings->sbflag1.flowratemeter_test = value & 1;
                ResponseCmndNumber(value & 1);

                Settings->sbflag1.flowratemeter_reglage = value & 1;
                ResponseCmndNumber(value & 1);

                if (Settings->sbflag1.flowratemeter_reglage) {FlowRateMeterShow(true);}

                Settings->sbflag1.flowratemeter_reglage ? value = 3 : value = 300;
                Settings->tele_period = value;

                show_parms = true;
            }
            break;
        case 14:  // weight avg sample (0 à 31)
            if (any_value) {
                Settings->sbflag1.flowratemeter_weight_avg_sample = value & 0x1F;
                ResponseCmndNumber(value & 0x1F);
                
                show_parms = true;
            }
            break;
        case 15:  // Reset total values
            if (any_value) {
                if (value & 1) {FlowRateMeterMidnightReset();}
                ResponseCmndNumber(value & 1);
                FlowRateMeterShow(true);

                show_parms = true;
            }
            break;
        case 16:  // Sets the frequency display on the web page
            if (any_value) {
                Settings->sbflag1.flowratemeter_show_freq = value & 1;
                ResponseCmndNumber(value & 1);

                show_parms = true;
            }
            break;
    }

    if (show_parms) {
        Response_P(PSTR("{\"Sensor%d\":{\"" D_JSON_POWERFACTOR "\":["), XSNS_96);
        for (uint32_t i = 0; i < MAX_FLOWRATEMETER; i++) {
            float flowratemeter_factor = Settings->flowratemeter_calibration[i] ? (float)Settings->flowratemeter_calibration[i] / 1000 : 1;
            ResponseAppend_P(PSTR("%s%3_f"), i ? PSTR(",") : PSTR(""), &flowratemeter_factor);
        }
        ResponseAppend_P(PSTR("],\"" D_JSON_FLOWRATEMETER_VALUE "\":\"%s\""),
                Settings->sbflag1.flowratemeter_raw_value ? PSTR(D_JSON_FLOWRATEMETER_VALUE_RAW) : PSTR(D_JSON_FLOWRATEMETER_VALUE_AVG));

        ResponseAppend_P(PSTR(",\"unites\":{"));
        if (Settings->sbflag1.flowratemeter_unit == 0) {
            ResponseAppend_P(PSTR("\"" D_JSON_FLOWRATEMETER_AMOUNT_UNIT "\":\"%s\",\"" D_JSON_FLOWRATEMETER_UNIT "\":\"%s\""), 
                    PSTR(D_UNIT_LITERS), PSTR(D_UNIT_LITER_PER_MINUTE));
        } else if (Settings->sbflag1.flowratemeter_unit == 1) {
            ResponseAppend_P(PSTR("\"" D_JSON_FLOWRATEMETER_AMOUNT_UNIT "\":\"%s\",\"" D_JSON_FLOWRATEMETER_UNIT "\":\"%s\""), 
                    PSTR(D_UNIT_LITERS), PSTR(D_UNIT_LITER_PER_HOUR));          
        } else if (Settings->sbflag1.flowratemeter_unit == 2) {
            ResponseAppend_P(PSTR("\"" D_JSON_FLOWRATEMETER_AMOUNT_UNIT "\":\"%s\",\"" D_JSON_FLOWRATEMETER_UNIT "\":\"%s\""), 
                    PSTR(D_UNIT_CUBIC_METER), PSTR(D_UNIT_CUBICMETER_PER_HOUR));  
        }

        ResponseAppend_P(PSTR("},\"" D_JSON_FLOWRATEMETER_REGLAGE "\":\"%s\""),
                Settings->sbflag1.flowratemeter_reglage ? PSTR("ON") : PSTR("OFF"));
        ResponseAppend_P(PSTR(",\"" D_JSON_FLOWRATEMETER_TEST "\":\"%s\""),
                Settings->sbflag1.flowratemeter_test ? PSTR("ON") : PSTR("OFF"));
        if (Settings->sbflag1.flowratemeter_raw_value == 0) {
            ResponseAppend_P(PSTR(",\"" D_JSON_FLOWRATEMETER_AVG_SAMPLE "\":%d"),
                    Settings->sbflag1.flowratemeter_weight_avg_sample);
        }
        ResponseAppend_P(PSTR(",\"" D_JSON_FLOWRATEMETER_SHOW_FREQ "\":\"%s\""),
                Settings->sbflag1.flowratemeter_show_freq ? PSTR("ON") : PSTR("OFF"));
        ResponseAppend_P(PSTR("}"));
    }

  return true;
}

/*********************************************************************************************
 * Interface
*********************************************************************************************/

bool Xsns96(uint32_t function) {
  bool result = false;

    if (PinUsed(GPIO_FLOWRATEMETER_IN, GPIO_ANY)) {
        switch (function) {
            case FUNC_INIT:
                FlowRateMeterInit();
                break;
            case FUNC_SAVE_AT_MIDNIGHT:
                FlowRateMeterMidnightReset();
                break;
            case FUNC_EVERY_250_MSECOND:
                FlowRateMeterRead();
                break;
            case FUNC_EVERY_SECOND:

                break;
            case FUNC_COMMAND_SENSOR:
                if (XSNS_96 == XdrvMailbox.index) {
                    result = FlowRateMeterCommand();
                }
                break;
            case FUNC_JSON_APPEND:
                FlowRateMeterShow(true);
                break;
#ifdef USE_WEBSERVER
            case FUNC_WEB_SENSOR:
                FlowRateMeterShow(false);
                break;
#endif  // USE_WEBSERVER
        }
    }

    return result;
}

#endif  // USE_FLOWRATEMETER
