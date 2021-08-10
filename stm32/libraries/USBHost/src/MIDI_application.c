#ifdef USE_USBHOST

/* Includes ------------------------------------------------------------------*/

#include "USBHost.h"
#include "usb_host.h"
#include "MIDI_application.h"

/* Private define ------------------------------------------------------------*/

#define RX_BUFF_SIZE 64 /* USB MIDI buffer : max received data 64 bytes */
uint8_t MIDI_RX_Buffer[RX_BUFF_SIZE]; // MIDI reception buffer
extern ApplicationTypeDef Appli_state;

/* Private function prototypes -----------------------------------------------*/
void ProcessReceivedMidiDatas(void);

/*-----------------------------------------------------------------------------*/
/**
 * @brief  Main routine for MIDI application, looped in main.c
 * @param  None
 * @retval none
 */
void MIDI_Application(void) {
	if (Appli_state == APPLICATION_READY) {
		USBH_MIDI_Receive(&hUsbHostFS, MIDI_RX_Buffer, RX_BUFF_SIZE); // just once at the beginning, start the first reception
		Appli_state = APPLICATION_RUNNING;
	} else if (Appli_state == APPLICATION_RUNNING) {
	} else if (Appli_state == APPLICATION_DISCONNECT) {
		Appli_state = APPLICATION_IDLE;
		USBH_MIDI_Stop(&hUsbHostFS);
	}
}

/*-----------------------------------------------------------------------------*/
/**
 * @brief  MIDI data receive callback.
 * @param  phost: Host handle
 * @retval None
 */
void USBH_MIDI_ReceiveCallback(USBH_HandleTypeDef *phost) {
	ProcessReceivedMidiDatas();
	USBH_MIDI_Receive(&hUsbHostFS, MIDI_RX_Buffer, RX_BUFF_SIZE); // start a new reception
}

void USBH_MIDI_TransmitCallback(USBH_HandleTypeDef *phost) {
}

void ProcessReceivedMidiDatas(void) {
	uint16_t numberOfPackets;
	uint8_t *ptr = MIDI_RX_Buffer;
	midi_package_t pack;

	numberOfPackets = USBH_MIDI_GetLastReceivedDataSize(&hUsbHostFS) / 4; // each USB midi package is 4 bytes long

	while (numberOfPackets--) {
		pack.cin_cable = *ptr;
		ptr++;
		pack.evnt0 = *ptr;
		ptr++;
		pack.evnt1 = *ptr;
		ptr++;
		pack.evnt2 = *ptr;
		ptr++;

		// const uint8_t mCable = pack.cin_cable;
		const uint8_t channel = pack.evnt0 & 0x0F;
		const uint8_t command = pack.evnt0 & 0xF0;
		const uint8_t a = pack.evnt1;
		const uint8_t b = pack.evnt2;
		switch(command) {
			case MIDI_NOTE_OFF     		: receive_midi_note_off(channel, a); break;
			case MIDI_NOTE_ON           : receive_midi_note_on(channel, a, b); break;
			case MIDI_AFTERTOUCH        : receive_midi_aftertouch(channel, a, b); break;
			case MIDI_CONTROL_CHANGE    : receive_midi_control_change(channel, a, b); break;
			case MIDI_PROGRAM_CHANGE    : receive_midi_program_change(channel, a); break;
			case MIDI_CHANNEL_PRESSURE  : receive_midi_channel_pressure(channel, a); break;
			case MIDI_PITCHBEND         : receive_midi_pitch_bend(channel, (int16_t)(((a << 7) + b) - 0x2000)); break;
			case MIDI_SYS_EX			: receive_midi_sys_ex(channel, a, b); break;
			default                     : receive_midi_message(pack.cin_cable, pack.evnt0, pack.evnt1, pack.evnt2);
		}
	}
}

/* --- transmit MIDI messages --- */

const uint8_t mCable = 0;

void transmit_midi_message(const uint8_t cable, const uint8_t data1, const uint8_t data2, const uint8_t data3) {
	const uint8_t length = 4;
	uint8_t data[length];
	data[0] = cable;
	data[1] = data1;
	data[2] = data2;
	data[3] = data3;
	USBH_MIDI_Transmit(&hUsbHostFS, data, length);
}

void transmit_midi_note_off(const uint8_t channel, const uint8_t note) {
	const uint8_t mCMD = MIDI_NOTE_OFF | channel;
	transmit_midi_message(mCable, mCMD, note, 0);
}

void transmit_midi_note_on(const uint8_t channel, const uint8_t note, const uint8_t velocity) {
	const uint8_t mCMD = MIDI_NOTE_ON | channel;
	transmit_midi_message(mCable, mCMD, note, velocity);
}

void transmit_midi_aftertouch(const uint8_t channel, const uint8_t note, const uint8_t pressure) {
	const uint8_t mCMD = MIDI_AFTERTOUCH | channel;
	transmit_midi_message(mCable, mCMD, note, pressure);
}

void transmit_midi_control_change(const uint8_t channel, const uint8_t command, const uint8_t value) {
	const uint8_t mCMD = MIDI_CONTROL_CHANGE | channel;
	transmit_midi_message(mCable, mCMD, command, value);
}

void transmit_midi_program_change(const uint8_t channel, const uint8_t program) {
	const uint8_t mCMD = MIDI_PROGRAM_CHANGE | channel;
	transmit_midi_message(mCable, mCMD, program, 0);
}

void transmit_midi_channel_pressure(const uint8_t channel, const uint8_t pressure) {
	const uint8_t mCMD = MIDI_CHANNEL_PRESSURE | channel;
	transmit_midi_message(mCable, mCMD, pressure, 0);
}

void transmit_midi_pitch_bend(const uint8_t channel, const int16_t value) {
	// @todo(test this!)
	const uint16_t mValue = (uint16_t)value;
	const uint16_t LSB_MASK = 0b00000001111111;
	const uint16_t MSB_MASK = 0b11111110000000;
	const uint8_t msb = (mValue & MSB_MASK) / 128;
	const uint8_t lsb = mValue & LSB_MASK;
	const uint8_t mCMD = MIDI_PITCHBEND | channel;
	transmit_midi_message(mCable, mCMD, lsb, msb);
}

void transmit_midi_sys_ex(const uint8_t type, const uint8_t data1, const uint8_t data2) {
	transmit_midi_message(mCable, type, data1, data2);
}

/*-----------------------------------------------------------------------------*/

#endif // USE_USBHOST
