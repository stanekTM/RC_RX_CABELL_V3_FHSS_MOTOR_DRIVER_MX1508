/*
 Copyright 2017 by Dennis Cabell
 KE8FZX

 To use this software, you must adhere to the license terms described below, and assume all responsibility for the use
 of the software.  The user is responsible for all consequences or damage that may result from using this software.
 The user is responsible for ensuring that the hardware used to run this software complies with local regulations and that
 any radio signal generated from use of this software is legal for that user to generate.  The author(s) of this software
 assume no liability whatsoever.  The author(s) of this software is not responsible for legal or civil consequences of
 using this software, including, but not limited to, any damages cause by lost control of a vehicle using this software.
 If this software is copied or modified, this disclaimer must accompany all copies.
 
 This project is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 RC_RX_CABELL_V3_FHSS is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with RC_RX_CABELL_V3_FHSS.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#ifndef __have__RC_RX_TX_RX_h__
#define __have__RC_RX_TX_RX_h__

#include "My_RF24.h"


//settings PWM motorA (pin D5 or D6 are paired on timer0, functions delay(), millis(), micros() and delayMicroseconds())
//1024 = 61Hz, 256 = 244Hz, 64 = 976Hz(default), 8 = 7812Hz
#define PWM_MOTOR_A  256

//settings PWM motorB (pin D9 or D10 are paired on timer1, Servo library)
//1024 = 30Hz, 256 = 122Hz, 64 = 488Hz(default), 8 = 3906Hz
#define PWM_MOTOR_B  256

//settings PWM (pin D3 or D11 are paired on timer2/8-bit, ServoTimer2 library)
//1024 = 30Hz, 256 = 122Hz, 128 = 244Hz, 64 = 488Hz(default), 32 = 976Hz, 8 = 3906Hz
//#define PWM_MOTOR_B  256

//setting the reaction of the motor to be rotated after the lever has been moved (0-255)
#define ACCELERATE_MOTOR_A  0
#define ACCELERATE_MOTOR_B  0

//setting the maximum motor power. Suitable for TX transmitters without endpoint setting (0-255)
#define MAX_MOTOR_A  255
#define MAX_MOTOR_B  255

//brake setting, adjustment (0-255), no brake 0, max brake 255
#define BRAKE_MOTOR_A  255
#define BRAKE_MOTOR_B  255

//setting the dead zone of poor quality joysticks TX for the motor controller
#define DEAD_ZONE  15

#define CH_MOTOR_A  0
#define CH_MOTOR_B  1
#define CH_SERVO_1  2
#define CH_SERVO_2  3
#define CH_SERVO_3  4
#define CH_SERVO_4  5
#define CH_SERVO_5  6
#define CH_SERVO_6  7

#define MIN_CONTROL_VAL  1000
#define MID_CONTROL_VAL  ((MIN_CONTROL_VAL + MAX_CONTROL_VAL) / 2)
#define MAX_CONTROL_VAL  2000

#define CABELL_BIND_RADIO_ADDR  0xA4B7C123F7LL

#define CABELL_NUM_CHANNELS   16 // The maximum number of RC channels that can be sent in one packet
#define CABELL_MIN_CHANNELS   4  // The minimum number of channels that must be included in a packet

#define CABELL_PAYLOAD_BYTES  24 // 12 bits per value * 16 channels

#define CABELL_RADIO_CHANNELS         9 // This is 1/5 of the total number of radio channels used for FHSS
#define CABELL_RADIO_MIN_CHANNEL_NUM  3 // Channel 0 is right on the boarder of allowed frequency range, so move up to avoid bleeding over

#define CABELL_RESERVED_MASK_CHANNEL  0x3F

#define CABELL_OPTION_MASK_MAX_POWER_OVERRIDE  0x40
#define CABELL_OPTION_MASK_CHANNEL_REDUCTION   0x0F

#define RX_CONNECTION_TIMEOUT  1000000 // If no packet received in this time frame apply failsafe settings. In microseconds

// FHSS parameters
#define DEFAULT_PACKET_INTERVAL     ((uint32_t)3000) 
#define MAX_PACKET_INTERVAL         ((uint32_t)4000) // Max packet interval - used with telemetry and 16 channels
#define INITIAL_PACKET_TIMEOUT_ADD  200ul
#define RESYNC_TIME_OUT             ((uint32_t)2000000) // Go to re-sync if no packet received in 3 seconds
#define RESYNC_WAIT_MICROS          (((((uint32_t)CABELL_RADIO_CHANNELS)*5ul)+8ul) * MAX_PACKET_INTERVAL) // when syncing listen on each channel for slightly longer than the time it takes to cycle through all channels

#define INITIAL_TELEMETRY_PACKETS_TO_SKIP  1000 // dont send initial telemetry packets to avoid anoying warnings at startup

#define DO_NOT_SOFT_REBIND             0xAA
#define BOUND_WITH_FAILSAFE_NO_PULSES  0x99

typedef struct
{
  enum RxMode_t : uint8_t
  {
    normal              = 0,
    bind                = 1,
    setFailSafe         = 2,
    normalWithTelemetry = 3,
    telemetryResponse   = 4,
    bindFalesafeNoPulse = 5,
    unBind              = 127
  }
  RxMode;
  
  uint8_t reserved = 0; /* Contains the channel number that the packet was sent on in bits 0-5 */
  uint8_t option;       /*   mask 0x0F    : Channel reduction.  The number of channels to not send (subtracted from the 16 max channels) at least 4 are always sent
                         *   mask 0x30>>4 : Receiver output mode
                         *                  0 (00) = Single PPM on individual pins for each channel 
                         *                  1 (01) = SUM PPM on channel 1 pin
                         *                  2 (10) = SBUS output
                         *                  3 (11) = Unused
                         *   mask 0x40>>6   Contains max power override flag for Multi-protocol TX module. Also sent to RX
                         *   mask 0x80>>7   Unused 
                         */
  uint8_t  modelNum;
  uint8_t  checkSum_LSB;  // Checksum least significant byte
  uint8_t  checkSum_MSB;  // Checksum most significant byte
  uint8_t  payloadValue[CABELL_PAYLOAD_BYTES] = {0}; // 12 bits per channel value, unsigned
  
}
CABELL_RxTxPacket_t;   


void attachServoPins();
void outputServo();
void outputPWM();
void outputChannels();
void ADC_Processing();
void setupReciever();
void setNextRadioChannel(bool missedPacket);
bool getPacket();
void checkFailsafeDisarmTimeout(unsigned long lastPacketTime, bool inititalGoodPacketRecieved);
void outputFailSafeValues(bool callOutputChannels);
void unbindReciever();
void bindReciever(uint8_t modelNum, uint16_t tempHoldValues[], CABELL_RxTxPacket_t :: RxMode_t RxMode);
void setFailSafeDefaultValues();
void loadFailSafeDefaultValues();
void setFailSafeValues(uint16_t newFailsafeValues[]);
bool validateChecksum(CABELL_RxTxPacket_t const& packet, uint8_t maxPayloadValueIndex);
bool readAndProcessPacket();
bool processRxMode(uint8_t RxMode, uint8_t modelNum, uint16_t tempHoldValues[]);
bool decodeChannelValues(CABELL_RxTxPacket_t const& RxPacket, uint8_t channelsRecieved, uint16_t tempHoldValues[]);
unsigned long sendTelemetryPacket(); // returns micros of when the transmit is expected to be complete
bool failSafeButtonHeld();
void setTelemetryPowerMode(uint8_t option);
void initializeRadio(My_RF24* radio);

#endif
 
