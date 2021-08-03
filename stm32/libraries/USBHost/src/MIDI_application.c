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

#define MIDI_NOTE_ON        0x90
#define MIDI_NOTE_OFF       0x80
#define MIDI_AFTERTOUCH     0xA0
#define MIDI_PITCHBEND      0xE0
#define MIDI_CONTROL_CHANGE 0xB0

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
			case MIDI_NOTE_ON           : receive_midi_note_on(channel, a, b); break;
			case MIDI_NOTE_OFF          : receive_midi_note_off(channel, a, b); break;
			case MIDI_AFTERTOUCH        : break;
			case MIDI_PITCHBEND         : receive_midi_pitch_bend(channel, (int16_t)(((a << 7) + b) - 0x2000)); break;
			case MIDI_CONTROL_CHANGE    : receive_midi_control_change(channel, a, b); break;
		}
	}
}

/*-----------------------------------------------------------------------------*/

#endif // USE_USBHOST
