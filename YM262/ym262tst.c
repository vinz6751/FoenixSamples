/* Ce program lit des données MIDI depuis le port série du A2560U et les
 * rejoue sur le synthétiseur YM262. Il est monophonique et utilise le canal 0,
 * seuls les messages note on/note off sont reconnus.
 * 
 *  This program reads MIDI data from the serial port and plays it on the
 * YM262's channel 0
 * Author: Vinz/MPS Octobre 2002
 * Domaine public / Public domain 
 */
#include <stdio.h>
#include "ym262.h"
#include "midimsg.h"

uint16_t blocks[128]; /* OPL3 pitch control */
uint16_t fnums[128];
uint32_t scale[128]; /* Tune table */
uint8_t  sysex[128]; /* midimsg sysex buffer */


void note_off(MIDIMSG_NOTE_OFF *e)
{
	printf("Note off\r\n");
	ym262_channel_off(0);
}

void note_on(MIDIMSG_NOTE_ON *e)
{
	if (e->velocity == 0)
	{
		note_off((MIDIMSG_NOTE_OFF*)e);
		return;
	}
		
	printf("Note on %x %d %d\r\n", e->channel, e->note, e->velocity);
	ym262_channel_off(0);
	ym262_set_block_fnum(0, blocks[e->note], fnums[e->note]);
	ym262_channel_on(0);
}


int main(void)
{
	uint16_t i;

	ym262_reset();
	
	/* Compute frequencies for the scale with 440.0Hz */
	ym262_create_note_to_freq_lut(4400, scale);

	/* Store block/fnum for each note number */
	for (i=0; i<128; i++)
		ym262_get_block_fnum_by_freq(scale[i], &blocks[i], &fnums[i]);

	ym262_reset();
	ym262_channel_off(0);
	/*ym262_write_reg(YM262_REG_OPL3_EN, YM262_OPL3_EN_MASK);*/

	/* Global settings*/
	ym262_set_vibrato_depth(1);
	ym262_set_tremolo_depth(1);

	/* Program a basic sound */
	/* Carrier */
	ym262_set_key_scale_level(0, 1, 0);
	ym262_set_oscfreqmult(0, 1, 3);
	ym262_set_env_type(0, 1, YM262_EGT_SUSTAINED);
	ym262_set_attack_rate(0, 1, 15);
	ym262_set_decay_rate(0, 1, 15);
	ym262_set_slope_rate(0, 1, 0);
	ym262_set_release_rate(0, 1, 5);
	ym262_set_osc_volume(0, 1,20);
	ym262_set_vibrato(0, 1, false);
	ym262_set_tremolo(0, 1, false);
	ym262_set_osc_connection(0, YM262_SYN_FM);
	ym262_set_feedback(0,3);

	/* Modulator */
	ym262_set_key_scale_level(0, 0, 0);
	ym262_set_oscfreqmult(0, 0, 2);
	ym262_set_env_type(0, 0, YM262_EGT_SUSTAINED);
	ym262_set_attack_rate(0, 0, 15);
	ym262_set_decay_rate(0, 0, 2);
	ym262_set_slope_rate(0, 0, 4);
	ym262_set_release_rate(0, 0, 5);
	ym262_set_osc_volume(0, 0,63);
	ym262_set_vibrato(0, 0, false);
	ym262_set_tremolo(0, 0, false);

	/* Channel general settings */
	ym262_set_output(0,YM262_OUTSEL_LR);
	ym262_set_osc_connection(0, YM262_SYN_ADD);

	/* Setup MIDI */
	midimsg_init(sysex, sizeof(sysex));
	midimsg_callbacks.note_on = note_on;
	midimsg_callbacks.note_off = note_off;
	
	/* Flush clavier et MIDI */
	while (Bconstat(3))
		Bconin(3);
	while (Cconis())
		Cnecin();

	Cconws("Hit a key to exit\r\n");
	while (!Cconis())
	{
		if (Bconstat(1))
		{
			unsigned char c = Bconin(1);
			midimsg_process(c);
		}
   	}
	
	midimsg_exit();
	
	ym262_channel_off(0);
    printf("Done !");
	return 0;
}
