/*
 * Constants.h
 *
 *  Created on: Nov 20, 2013
 *      Author: aaron
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

//#include "ev3_constants.h"

// Debug Constants

#define PRINT_EV3_DEBUG 1

// Timing Constants

#define EV3_SEND_STATUS_FPS 20
#define SOAR_SEND_COMMAND_FPS 20
#define SOAR_DC_WAIT 0

// Defining devices that messages can be sent to
#define NO_DEV 0
#define MANAGER_DEV 1
#define BRICK_DEV 2
#define OUTPUT_MAN_DEV 3
#define INPUT_MAN_DEV 4

#define INPUT_1_DEV 11
#define INPUT_2_DEV 12
#define INPUT_3_DEV 13
#define INPUT_4_DEV 14

#define OUTPUT_1_DEV 21
#define OUTPUT_2_DEV 22
#define OUTPUT_3_DEV 23
#define OUTPUT_4_DEV 24


#define COMMAND_SUCCESS 0
#define COMMAND_FAILURE -1

// Constants for Manager
#define STATUS_MESSAGE 1
#define COMMAND_MESSAGE 3
#define ACK_MESSAGE 2

// Constants for Brick
#define BRICK_COMMAND_SET_LED 1

#define NUM_INPUTS 4
#define NUM_OUTPUTS 4

#define BUTTON_ID_UP     0x01 //!< The up button.
#define BUTTON_ID_ENTER  0x02 //!< The enter button.
#define BUTTON_ID_DOWN   0x04 //!< The down button.
#define BUTTON_ID_RIGHT  0x08 //!< The right button.
#define BUTTON_ID_LEFT   0x10 //!< The left button.
#define BUTTON_ID_ESCAPE 0x20 //!< The exit (escape) button.

#define LED_BLACK        0  //!< LED black pattern
#define LED_GREEN        1  //!< LED green pattern
#define LED_RED          2  //!< LED red pattern
#define LED_ORANGE       3  //!< LED orange pattern
#define LED_GREEN_FLASH  4  //!< LED green flash pattern
#define LED_RED_FLASH    5  //!< LED red flash pattern
#define LED_ORANGE_FLASH 6  //!< LED orange flash pattern
#define LED_GREEN_PULSE  7  //!< LED green pulse pattern
#define LED_RED_PULSE    8  //!< LED red pulse pattern
#define LED_ORANGE_PULSE 9  //!< LED orange pulse pattern
#define NUM_LED_PATTERNS 10 //!< The number of LED patterns

// Constants for Motors
#define MOTOR_DIR_FORWARD 1
#define MOTOR_DIR_BACKWARD 2

#define MAX_MOTOR_SPEED 100

#define MOTOR_COMMAND_START 0xA6 /*!< opOutputStart */
#define MOTOR_COMMAND_STOP  0xA3 /*!< opOutputStop */
#define MOTOR_COMMAND_SET_POWER 0xA4 /*!< opOutputPower */
#define MOTOR_COMMAND_SET_SPEED 0xA5 /*!< opOutputSpeed */
#define MOTOR_COMMAND_SET_DIRECTION 0xA7 /*!< opOutputPolarity */


#define LARGE_MOTOR_TYPE 7
#define MEDIUM_MOTOR_TYPE 8

// Constants for Sensor
#define CHANGE_MODE_COMMAND 1
#define CREATE_ANALOG_SENSOR_COMMAND 2
#define DELETE_ANALOG_SENSOR_COMMAND 3

#define SENSOR_CAT_NONE 1
#define SENSOR_CAT_SERIAL 2
#define SENSOR_CAT_ANALOG 3

// Constants for Color Sensor
#define EV3_COLOR_SENSOR_TYPE 29

#define COLOR_SENSOR_MODE_REFLECT 0
#define COLOR_SENSOR_MODE_AMBIENT 1
#define COLOR_SENSOR_MODE_COLOR 2

// Constants for Touch Sensor
#define EV3_TOUCH_SENSOR_TYPE 125

// Constants for IR Remote
#define EV3_IR_REMOTE_SENSOR_TYPE 33

#define NUM_IR_CHANNELS 4
#define NUM_IR_REMOTE_BUTTONS 4

#define EV3_IR_REMOTE_PROXIMITY_MODE 0
#define EV3_IR_REMOTE_BEACON_MODE 1
#define EV3_IR_REMOTE_REMOTE_MODE 2

#endif /* CONSTANTS_H_ */
