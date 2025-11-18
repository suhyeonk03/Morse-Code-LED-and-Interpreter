#include <stdio.h>
#include <string.h>
#include "esp_adc/adc_oneshot.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//#define ADC_PIN ADC1_CHANNEL_0
#define ADC_CHANNEL ADC_CHANNEL_3
#define ADC_UNIT ADC_UNIT_1
#define TIME_UNIT 35
#define WHILE_LOOP_DELAY 21
#define THRESHOLD 900
#define DOT_DASH_THRESHOLD (TIME_UNIT*8)
#define LETTER_SPACE (TIME_UNIT*14)
#define WORD_SPACE (TIME_UNIT*22)
#define NEW_LINE_SPACE (TIME_UNIT*50)

static const char *TAG = "ADC";

char morse_to_char(const char*morse) {
	struct { const char *morse; char letter; } MORSE_TABLE[] = {
               {".-", 'A'}, {"-...", 'B'}, {"-.-.", 'C'}, {"-..", 'D'}, {".", 'E'}, {"..-.", 'F'}, {"--.", 'G'}, {"....", 'H'}, {"..", 'I'},
               {".---", 'J'}, {"-.-", 'K'}, {".-..", 'L'}, {"--", 'M'}, {"-.", 'N'}, {"---", 'O'}, {".--.", 'P'}, {"--.-", 'Q'}, {".-.", 'R'},
               {"...", 'S'}, {"-", 'T'}, {"..-", 'U'}, {"...-", 'V'}, {".--", 'W'}, {"-..-", 'X'}, {"-.--", 'Y'}, {"--..", 'Z'},
               {"-----", '0'}, {".----", '1'}, {"..---", '2'}, {"...--", '3'}, {"....-", '4'}, {".....", '5'}, {"-....", '6'}, {"--...", '7'}, {"---..", '8'}, {"----.", '9'},
               {NULL, 0}};
	for (int i=0; MORSE_TABLE[i].morse !=NULL; i++) {
		if (strcmp(morse, MORSE_TABLE[i].morse) == 0) {
			return MORSE_TABLE[i].letter;
		}
	}
	return '?';
}

void addChar (char *s, char c) {
	int l = strlen(s);
	s[l] = c;
	s[l+1] = '\0';
}

void app_main(void) {
	setvbuf(stdout, NULL, _IONBF, 0);
	adc_oneshot_unit_handle_t adc_handle;
	adc_oneshot_chan_cfg_t config = {
		.bitwidth = ADC_BITWIDTH_12, 
		.atten = ADC_ATTEN_DB_12, 
	};
	adc_oneshot_unit_init_cfg_t init_config = {
		.unit_id = ADC_UNIT, 
	};
	
	adc_oneshot_new_unit(&init_config, &adc_handle);
	adc_oneshot_config_channel(adc_handle, ADC_CHANNEL, &config);

	int prev_state = 0;
	int state;
	int signal_duration = 0;
	int gap_duration = 0;
	char morse_buffer[10] = {0};
        int morse_index = 0;
	char decoded = '\0';
	char string[30] = {'\0'};

	while (1) {
		int adc_raw = 0;
		adc_oneshot_read(adc_handle, ADC_CHANNEL, &adc_raw);
		//printf("Got Data\n");
		//printf("adc: %d, signal: %d, gap: %d, index: %d\n", adc_raw, signal_duration, gap_duration, morse_index);
		
		state = (adc_raw > THRESHOLD)? 1:0;

		if (state == 1) {
			if (prev_state != 1) {
				if (gap_duration > WORD_SPACE) {
   					if (morse_index > 0) {
      					  	decoded = morse_to_char(morse_buffer);
     				  		addChar(string, decoded);
						morse_index = 0;
       						morse_buffer[0] = '\0';
    					}
   					decoded = ' ';
				        addChar(string, decoded);	
				}
				else if (gap_duration > LETTER_SPACE) {
					if (morse_index > 0) {
						decoded = morse_to_char(morse_buffer);
						addChar(string, decoded);
						morse_index = 0;
						morse_buffer[0] = '\0';
					}
				}
				gap_duration = 0;
			}
			signal_duration += WHILE_LOOP_DELAY;
		}
		else {
			if (prev_state == 1) {
				if (morse_index < 9) {
					if (signal_duration < DOT_DASH_THRESHOLD) {
						morse_buffer[morse_index] = '.';
						morse_index += 1;
					}
					else {
						morse_buffer[morse_index] = '-';
						morse_index += 1;
					}
					morse_buffer[morse_index] = '\0';
				}
				signal_duration = 0;
			}
			gap_duration += WHILE_LOOP_DELAY;
		}

		prev_state = state;
		if (state == 0 && morse_index > 0 && gap_duration > NEW_LINE_SPACE) {
    			
			decoded = morse_to_char(morse_buffer);
			addChar(string, decoded);
                        printf("%s\n", string);
    			morse_index = 0;
    			morse_buffer[0] = '\0';
			string[0] = '\0';
		}
		vTaskDelay(pdMS_TO_TICKS(WHILE_LOOP_DELAY));
	}
}

