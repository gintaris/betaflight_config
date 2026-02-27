/*
 * This file is part of Betaflight.
 *
 * Betaflight is free software. You can redistribute this software
 * and/or modify this software under the terms of the GNU General
 * Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later
 * version.
 *
 * Betaflight is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this software.
 *
 * If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#define FC_TARGET_MCU        RP2350B
#define BOARD_NAME           PIMORONI_PICO_DEV
#define MANUFACTURER_ID      MRLT

// ============================================================
//  DEV BOARD CONFIGURATION — Pimoroni Pico Plus 2
// ============================================================
//
// Pimoroni Pico Plus 2 has RP2350B but only GPIO 0–28 on headers.
// GPIO 29 = VSYS sense (internal), GPIO 30–47 not on headers.
//
// Compared to DIVISION_RP_FPV_V2 PCB:
//   - IMU: ICM-42688-P on SPI1 (PA10, PA11, PA28), CS on PA19
//   - PIOUART0 (PA10-11) sacrificed for IMU SPI
//   - MAG on I2C1 (PA2-3) instead of I2C0 (PA32-33)
//   - PA23-25 NOT on headers → no PINIO, LED1 used for CS
//   - No SD card, no barometer, no INA226, no ADC
//   - LED0 moved from PA45 → PA18 (available on header)
//
// Purpose: Test motors, servos, OSD, IMU, and MAG on dev board
//          before ordering custom PCB.

// ============================================================
//  IMU — ICM-42688-P on SPI1
// ============================================================
// Remapped to GPIO 0–28 available on Pico Plus 2 headers.
// SPI1 valid pins:
//   SCK: 10, 14, 26, 30, 42  → PA10 (was PIOUART0_TX)
//   TX:  11, 15, 27, 31, 43  → PA11 (was PIOUART0_RX)
//   RX:   8, 12, 24, 28, 40  → PA28 (free)
// Note: PA23-25 NOT on Pimoroni headers — CS moved to PA19

#define USE_GYRO
#define USE_ACC
#define USE_GYRO_SPI_ICM42688P
#define USE_ACC_SPI_ICM42688P
#define SPI1_SCK_PIN         PA10   // SPI1 CLK (was PIOUART0_TX on PCB)
#define SPI1_SDI_PIN         PA28   // SPI1 RX — IMU SDO → MCU
#define SPI1_SDO_PIN         PA11   // SPI1 TX — MCU → IMU SDI (was PIOUART0_RX)
#define GYRO_1_EXTI_PIN      PA27   // INT1 data-ready interrupt
#define GYRO_1_CS_PIN        PA19   // Chip select (was LED1, PA23 not on headers)
#define GYRO_1_SPI_INSTANCE  SPI1
#define GYRO_1_ALIGN         CW0_DEG

// Alternative IMU drivers (in case of part substitution)
#define USE_ACCGYRO_LSM6DSV16X
#define USE_ACCGYRO_LSM6DSO
#define USE_ACCGYRO_ICM45686

// ============================================================
//  MAGNETOMETER — LIS2MDL on I2C1
// ============================================================
// LIS2MDL I2C address: 0x1E
// Using I2C1 on PA2/PA3 (available on Pico headers)
// DRDY → PA26 (optional, connected on dev board)

#define USE_MAG
#define USE_MAG_LIS2MDL
#define MAG_I2C_INSTANCE     I2CDEV_1
#define I2C1_SDA_PIN         PA2
#define I2C1_SCL_PIN         PA3

// Additional magnetometer drivers
#define USE_MAG_HMC5883
#define USE_MAG_QMC5883
#define USE_MAG_LIS3MDL
#define USE_MAG_AK8975
#define USE_MAG_IST8310

// ============================================================
//  OSD — Framebuffer OSD via PIO2
// ============================================================
// LM393 sync detection, resistor video mixer
// Pins MUST be consecutive: W, EN, SYNC
// Video mixer: W → 470Ω, EN → 1kΩ, CAM → 75Ω → VTX

#define USE_FB_OSD
#define OSD_W_PIN            PA20   // White/Black pixel output
#define OSD_EN_PIN           PA21   // Overlay enable
#define OSD_SYNC_PIN         PA22   // Sync input from LM393

// ============================================================
//  MOTORS — DSHOT600 via PIO0
// ============================================================

#define MOTOR1_PIN           PA6    // PIO0 SM0
#define MOTOR2_PIN           PA7    // PIO0 SM1
#define MOTOR3_PIN           PA8    // PIO0 SM2
#define MOTOR4_PIN           PA9    // PIO0 SM3

// ============================================================
//  SERVOS — Hardware PWM 50Hz
// ============================================================
// PWM slice 6: GPIO 12 (A) + 13 (B)
// PWM slice 7: GPIO 14 (A) + 15 (B)

#define USE_SERVOS
#define SERVO1_PIN           PA12   // PWM slice 6A
#define SERVO2_PIN           PA13   // PWM slice 6B
#define SERVO3_PIN           PA14   // PWM slice 7A
#define SERVO4_PIN           PA15   // PWM slice 7B

// ============================================================
//  UARTs
// ============================================================

// UART0: RC Radio (ELRS / CRSF)
#define UART0_TX_PIN         PA0
#define UART0_RX_PIN         PA1

// UART1: GPS / Debug
#define UART1_TX_PIN         PA4
#define UART1_RX_PIN         PA5

// PIOUART0: NOT AVAILABLE — PA10/PA11 repurposed for IMU SPI1

// PIOUART1: Software UART (telemetry/spare)
#define PIOUART1_TX_PIN      PA16
#define PIOUART1_RX_PIN      PA17

// ============================================================
//  LEDs and PINIO
// ============================================================

// Status LED
// Note: PA23-25 NOT on Pimoroni Pico Plus 2 headers
// PA25 = onboard LED (active low, directly on board)
// PA19 repurposed for GYRO_CS — no spare LED available
#define LED0_PIN             PA18   // External LED for Betaflight status

// GPIO outputs — PA24/PA25 not on headers, no PINIO available

// ============================================================
//  DISABLED FEATURES (not available on dev board)
// ============================================================

// No SD card — SPI1 repurposed for IMU, GPIO 30-31 not on headers
// USE_USB_MSC auto-disabled by common_post.h when USE_SDCARD is absent

// No barometer (BMP580 would need I2C0 on PA32-33, not on headers)
// No INA226 current monitor
// No ADC (PA40-44 not on headers)
// No PIOUART0 (PA10-11 repurposed for SPI1)

// ============================================================
//  PIN SUMMARY (Pico Plus 2 Header Map)
// ============================================================
//
//  GPIO  | Function       | Pico Header Pin
//  ------|----------------|----------------
//  PA0   | UART0_TX       | Pin 1
//  PA1   | UART0_RX       | Pin 2
//  PA2   | I2C1_SDA (MAG) | Pin 4
//  PA3   | I2C1_SCL (MAG) | Pin 5
//  PA4   | UART1_TX       | Pin 6
//  PA5   | UART1_RX       | Pin 7
//  PA6   | MOTOR1         | Pin 9
//  PA7   | MOTOR2         | Pin 10
//  PA8   | MOTOR3         | Pin 11
//  PA9   | MOTOR4         | Pin 12
//  PA10  | SPI1_SCK (IMU) | Pin 14
//  PA11  | SPI1_SDO (IMU) | Pin 15
//  PA12  | SERVO1         | Pin 16
//  PA13  | SERVO2         | Pin 17
//  PA14  | SERVO3         | Pin 19
//  PA15  | SERVO4         | Pin 20
//  PA16  | PIOUART1_TX    | Pin 21
//  PA17  | PIOUART1_RX    | Pin 22
//  PA18  | LED0           | Pin 24
//  PA19  | GYRO_CS (IMU)  | Pin 25
//  PA20  | OSD_W          | Pin 26
//  PA21  | OSD_EN         | Pin 27
//  PA22  | OSD_SYNC       | Pin 29
//  PA23  | — (not on hdr) | N/A
//  PA24  | — (not on hdr) | N/A
//  PA25  | onboard LED    | N/A (not on header)
//  PA26  | MAG_DRDY       | Pin 31
//  PA27  | GYRO_EXTI      | Pin 32
//  PA28  | SPI1_SDI (IMU) | Pin 34
