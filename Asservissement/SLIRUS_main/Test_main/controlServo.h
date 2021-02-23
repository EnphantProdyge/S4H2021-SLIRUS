/** @file controlServo.h
 * The file controlServo.h contains the class Servo.
 */

 /** @class Servo
  * @brief Class containing functions related to the servomotors.
  *
  * This class contains the structures of characters and the functions used to control the servo output.
  * @author CHARBONNEAU, EMILE
  * @date 19/04/2020
  * Modify by BEAUMIER,FANNY
  * @date 10/02/2021
  */

#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>                        ///< Library needed to control the PWM of servo drive.
#include <Wire.h>

  // Section to change when adding servos or characters:
#define NB_MOTORS             10                            ///< Number of active motors.
#define NB_FINGERS            5                             ///< Number of active fingers.
#define NB_LETTERS            36                            ///< Number of characters managed.

// The next defined are the different articulated positions for a finger:
#define VERTICAL              0                             ///< Finger fully unfold.
#define HORIZONTAL            1                             ///< Finger with first knuckle fold.
#define SEC_90				  2                             ///< Finger with second knuckle fold.
#define FULLY_INCLINED        3                             ///< Finger with botch knuckles fold.
#define CONFIG_MRLA			  4                             ///< Finger middle ring or little with 80 degree angle on both knuckle.
#define CONFIG_MRLB			  5                             ///< Finger middle ring or little with 100 degree angle on both knuckle.
#define CONFIG_MRLC			  6                             ///< Finger middle ring or little with 110 degree angle on both knuckle.


/** @brief The next defined are the different fingers implemented.
 *
 * The increment per finger is 2 because there are 2 motors per finger that requires to move.
 * This implementation eases the working principal of the function moveFinge(int,int).
 */
#define THUMB                 0
#define INDEX                 2
#define MIDDLE                4
#define RING                  6
#define LITTLE                8

 /**Defining moving angles.
  */
#define KNUCKLE1_UP				0							///< Angle for a straigth knuckle.
#define KNUCKLE1_90             180                         ///< Angle for an inclined knuckle.
#define KNUCKLE2_UP				0							///< Angle for a straigth knuckle.
#define KNUCKLE2_90             180                         ///< Angle for an inclined knuckle.
#define KNUCKLE_80				80							///< Angle for an 80 degrees inclined knuckle.
#define KNUCKLE_100				100							///< Angle for an 100 degrees inclined knuckle.
#define KNUCKLE_110				110                         ///< Angle for an 110 degrees inclined knuckle.

  /**The next section contains defined PWM constants.
   */
#define MIN_PULSE_WIDTH       650
#define MAX_PULSE_WIDTH       2350
#define DEFAULT_PULSE_WIDTH   1500
#define FREQUENCY             50

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();    ///< Setting up the pwm object with the default address for the driver (0x40).
uint8_t servonum = 1;                                       ///< Creation of a servo driver object (PCA9685).

/** @struct character
 * @brief Creating a structure that will hold informations relative to each character.
 */
struct character {
	int id;                                                   ///< The character in question converted in Ascii.
	int pattern[NB_FINGERS];                                  ///< The pattern in which order should the fingers move.
	int angle[NB_FINGERS];                                    ///< The angles the fingers should move to in accordance to the pattern.
} charact[NB_LETTERS];                                      ///< Creating a structure instance charact wich is an array the size of 

class Servo
{
public:
	Servo() {
		/** @brief Default constructor for the object servo.
		 *
		 * This constructor initializes each character coded below in accordance to the structure character.
		 * This is the structure to follow :
		 * charact[int] = {{('Character id')},{pattern},{Angles}};
		 */
		charact[0] = { ('0'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{FULLY_INCLINED,FULLY_INCLINED,CONFIG_MRLC,CONFIG_MRLC,CONFIG_MRLC} };
		charact[1] = { ('1'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{FULLY_INCLINED,VERTICAL,FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED} };
		charact[2] = { ('2'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{FULLY_INCLINED,VERTICAL,VERTICAL,FULLY_INCLINED,FULLY_INCLINED} };
		charact[3] = { ('3'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{VERTICAL,VERTICAL,VERTICAL,FULLY_INCLINED,FULLY_INCLINED} };
		charact[4] = { ('4'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{FULLY_INCLINED,VERTICAL,VERTICAL,VERTICAL,VERTICAL} };
		charact[5] = { ('5'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{VERTICAL,VERTICAL,VERTICAL,VERTICAL,VERTICAL} };
		charact[6] = { ('a'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{VERTICAL,FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED} };
		charact[7] = { ('b'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{HORIZONTAL,VERTICAL,VERTICAL,VERTICAL,VERTICAL} };
		charact[8] = { ('c'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{CONFIG_MRLA,CONFIG_MRLA,CONFIG_MRLA,CONFIG_MRLA,CONFIG_MRLA} };
		charact[9] = { ('d'),{INDEX,MIDDLE,RING,LITTLE,THUMB},{VERTICAL,CONFIG_MRLC,CONFIG_MRLC,CONFIG_MRLC,CONFIG_MRLC} };
		charact[10] = { ('e'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{FULLY_INCLINED,FULLY_INCLINED,CONFIG_MRLB,CONFIG_MRLB,CONFIG_MRLB} };
		charact[11] = { ('f'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{SEC_90,SEC_90,VERTICAL,VERTICAL,VERTICAL} };
		charact[12] = { ('g'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{VERTICAL,VERTICAL,FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED} };
		charact[13] = { ('h'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{VERTICAL,VERTICAL,VERTICAL,FULLY_INCLINED,FULLY_INCLINED} };
		charact[14] = { ('i'),{INDEX,MIDDLE,RING,LITTLE,THUMB},{FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED,VERTICAL,FULLY_INCLINED} };
		charact[15] = { ('j'),{INDEX,MIDDLE,RING,LITTLE,THUMB},{FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED,VERTICAL,FULLY_INCLINED} };
		charact[16] = { ('k'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{VERTICAL,VERTICAL,VERTICAL,FULLY_INCLINED,FULLY_INCLINED} };
		charact[17] = { ('l'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{VERTICAL,VERTICAL,FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED} };
		charact[18] = { ('m'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{FULLY_INCLINED,FULLY_INCLINED,CONFIG_MRLC,CONFIG_MRLC,FULLY_INCLINED} };
		charact[19] = { ('n'),{LITTLE,RING,THUMB,INDEX,MIDDLE,},{FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED,CONFIG_MRLC} };
		charact[20] = { ('o'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{SEC_90,SEC_90,CONFIG_MRLC,CONFIG_MRLC,CONFIG_MRLC} };
		charact[21] = { ('p'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{VERTICAL,VERTICAL,HORIZONTAL,FULLY_INCLINED,FULLY_INCLINED} };
		charact[22] = { ('q'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{VERTICAL,VERTICAL,FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED} };
		charact[23] = { ('r'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{FULLY_INCLINED,VERTICAL,VERTICAL,FULLY_INCLINED,FULLY_INCLINED} };
		charact[24] = { ('s'),{INDEX,MIDDLE,RING,LITTLE,THUMB},{FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED} };
		charact[25] = { ('t'),{INDEX,MIDDLE,RING,LITTLE,THUMB},{FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED,VERTICAL} };
		charact[26] = { ('u'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{FULLY_INCLINED,VERTICAL,VERTICAL,FULLY_INCLINED,FULLY_INCLINED} };
		charact[27] = { ('v'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{FULLY_INCLINED,VERTICAL,VERTICAL,FULLY_INCLINED,FULLY_INCLINED} };
		charact[28] = { ('w'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{VERTICAL,VERTICAL,VERTICAL,VERTICAL,FULLY_INCLINED} };
		charact[29] = { ('x'),{LITTLE,RING,MIDDLE,INDEX,THUMB},{FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED,SEC_90,FULLY_INCLINED} };
		charact[30] = { ('y'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{VERTICAL,FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED,VERTICAL} };
		charact[31] = { ('z'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{VERTICAL,FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED,FULLY_INCLINED} };
		charact[32] = { ('6'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{FULLY_INCLINED,VERTICAL,VERTICAL,VERTICAL,FULLY_INCLINED} };
		charact[33] = { ('7'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{FULLY_INCLINED,VERTICAL,VERTICAL,FULLY_INCLINED,VERTICAL} };
		charact[34] = { ('8'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{FULLY_INCLINED,VERTICAL,CONFIG_MRLC,VERTICAL,VERTICAL} };
		charact[35] = { ('9'),{THUMB,INDEX,MIDDLE,RING,LITTLE},{FULLY_INCLINED,FULLY_INCLINED,VERTICAL,VERTICAL,VERTICAL} };
	}


	bool servoOut(int character, int increment) {
		character = adjustCommand(character);
		int finger = charact[character].pattern[increment];
		int moveOption = charact[character].angle[increment];
		moveFinger(finger, moveOption);
		if (increment < NB_FINGERS - 1) { return false; }
		else { return true; }
	}
	/** @fn bool servoOut(int character,int increment)
	 * @brief Function of the servo class. This function will send the command to move a finger.
	 * @param character the character to be displayed.
	 * @param increment an integer instructing wich pattern and angle index to reach in the character's structure.
	 * @return true only once every finger moved.
	 */

	bool reverseMove(int character, int decrement) {
		character = adjustCommand(character);
		int finger = charact[character].pattern[decrement];
		int moveOption = VERTICAL;
		moveFinger(finger, moveOption);
		if (decrement > 0) { return false; }
		else { return true; }
	}
	/** @fn bool reverseMove(int character,int decrement)
	 * @brief This function will move a finger to the vertical.
	 * @param character the character previously displayed.
	 * @param decrement an integer instructing wich finger to unfold.
	 * @return true only once every finger has unfold.
	 */

	int moveFinger(int finger, int moveOption) {
		int nbMotor = 2;
		int angle[nbMotor];
		for (int i = 0; i < nbMotor; i++) {
			angle[i] = 25;
		}
		bool readyToMove = false;
		if (moveOption == 0) {
			angle[0] = KNUCKLE1_UP;
			angle[1] = KNUCKLE2_UP;
			readyToMove = true;
		}
		if (moveOption == 1) {
			angle[0] = KNUCKLE1_90;
			angle[1] = KNUCKLE2_UP;
			readyToMove = true;
		}
		if (moveOption == 2) {
			angle[0] = KNUCKLE1_UP;
			angle[1] = KNUCKLE2_90;
			readyToMove = true;
		}
		if (moveOption == 3) {
			angle[0] = KNUCKLE1_90;
			angle[1] = KNUCKLE2_90;
			readyToMove = true;
		}
		if (moveOption == 4) {
			angle[0] = KNUCKLE_80;
			angle[1] = KNUCKLE_80;
			readyToMove = true;
		}
		if (moveOption == 5) {
			angle[0] = KNUCKLE_100;
			angle[1] = KNUCKLE2_90;
			readyToMove = true;
		}
		if (moveOption == 6) {
			angle[0] = KNUCKLE_110;
			angle[1] = KNUCKLE_110;
			readyToMove = true;
		}
		else {}
		if (readyToMove) {
			for (int i = 0; i < nbMotor; i++) {
				pwm.setPWM(finger + i, 0, pulseWidth(angle[i]));
			}
		}
		return 0;
	}
	/** @fn int moveFinger(int finger, int moveOption)
	 * @brief This function sends the PWM to the servos of a finger.
	 * @param finger a finger to be moved.
	 * @param moveOption a selected moveOption.
	 * The different move options are : VERTICAL, HORIZONTAL, FULLY_INCLINED AND A90_DEGREE.
	 * @return 0 once every motor of the finger moved.
	 */

	int pulseWidth(int angle) {
		int pulse_wide, analog_value;
		pulse_wide = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
		analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
		return analog_value;
	}
	/** @fn int pulseWidth(int angle)
	 * @brief This function returns the analog value to set the PWM.
	 * @param angle the desired angle shall the motor reach.
	 * This function calculates the analog value corresponding a received angle.
	 * This analog value will bet sent to the servo motor.
	 * @return the analog value of the angle.
	 */

	int adjustCommand(int command) {
		int adjustedCommand = ' ';
		bool found = false;
		if (command <= 53 && command >= 48) {
			adjustedCommand = command - 48;
			found = true;
		}
		if (command >= 97 && command <= 122) {
			adjustedCommand = command - 91;
			found = true;
		}
		if (!found) {
			adjustedCommand = ' ';
		}
		return adjustedCommand;
	}
	/** @fn int adjustCommand(int command)
	 * @brief This function adjust the command.
	 * @param command the Ascii value of the desired character to display.
	 * The Ascii value is hashed to fit neatly in the charact[NB_LETTERS] array.
	 * @The charact[adjustedCommand] contains informations relative to command.
	*/
};
