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
 
#ifndef __have__RC_RX_PINS_h__
#define __have__RC_RX_PINS_h__

#include "RX.h"


//free pins
//pin                          0
//pin                          1

//pins for nRF24L01
#define PIN_CE                 14     // A0
#define PIN_CSN                15     // A1

//software SPI https://nrf24.github.io/RF24/md_docs_arduino.html
//----- SCK                    16 - A2
//----- MOSI                   17 - A3
//----- MISO                   18 - A4

#define PIN_SERVO_1            2
#define PIN_SERVO_2            3
#define PIN_SERVO_3            4
#define PIN_SERVO_4            7
#define PIN_SERVO_5            8
#define PIN_SERVO_6            11
     
#define PIN_PWM_1_MOTOR_A      5
#define PIN_PWM_2_MOTOR_A      6
#define PIN_PWM_3_MOTOR_B      9
#define PIN_PWM_4_MOTOR_B      10

#define PIN_BUTTON_BIND        12
#define PIN_LED                13

#define PIN_RX_BATT_A1         6  // A6 - 20
#define PIN_RX_BATT_A2         7  // A7 - 21

// configure A5 for radio IRQ 
#define RADIO_IRQ_PIN          A5 // 19
#define RADIO_IRQ_PIN_bit      5  // PC5
#define RADIO_IRQ_port         PORTC
#define RADIO_IRQ_ipr          PINC
#define RADIO_IRQ_ddr          DDRC
#define RADIO_IRQ_PIN_MASK     _BV(RADIO_IRQ_PIN_bit)
#define RADIO_IRQ_SET_INPUT    RADIO_IRQ_ddr &= ~RADIO_IRQ_PIN_MASK
#define RADIO_IRQ_SET_OUTPUT   RADIO_IRQ_ddr |=  RADIO_IRQ_PIN_MASK
#define RADIO_IRQ_SET_PULLUP   RADIO_IRQ_port |= RADIO_IRQ_PIN_MASK
#define IS_RADIO_IRQ_on        ((RADIO_IRQ_ipr & RADIO_IRQ_PIN_MASK) == 0x00)

#endif
 
