#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     rightDrive,    tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     leftDrive,     tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     lift,          tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     flag,          tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     arm,           tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     bucket,        tmotorTetrix, openLoop, encoder)
#pragma config(Servo,  srvo_S1_C4_1,    dumper,               tServoStandard)
#pragma config(Servo,  srvo_S1_C4_2,    flagMount,            tServoStandard)
#pragma config(Servo,  srvo_S1_C4_3,    flagRaiser,           tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C4_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.
#include "drive.h" //include for assisted TeleOp

void initializeRobot()
{
	servo[dumper] = 233;
	servo[flagMount] = 17;
	return;
	nMotorEncoder[rightDrive] = 0;
}

//ranges: motor power = -100 to +100, joystick = -128 to +128.
//For more prescise movement we normalize the values
const float multiplier = 0.5078;// used to normalize joystick values
const int driveLowerThreshold = 15;//prevents motors from straining at very low power if the joystick is not at center
int drivePowerBoost1 = 80;
int drivePowerBoost2 = 100;
//int driveUpperThreshold = 85;
bool flagOut = false;
bool dumperOut = false;

const int bucketStartSpeed = 50;
const int bucketScoopAngle = 100;
const int bucketDumpAngle = 120;
const int bucketGearRatio = 2;
const int bucketDumpTics = 4*bucketGearRatio*bucketDumpAngle;
const int bucketScoopTics = 4*bucketGearRatio*bucketScoopAngle;
const int bucketDumpSlope = bucketStartSpeed/bucketDumpTics;
const int bucketScoopSlope = bucketStartSpeed/bucketScoopTics;
task main()
{
	initializeRobot();
	waitForStart();   // wait for start of tele-op phase
	nMotorEncoder[bucket] = 0;
	nMotorEncoder[arm] = 0;

	nMotorEncoder[rightDrive] = 0;
	nMotorEncoder[leftDrive] = 0;
	while (true)//infinite loop
	{
		bool isFlagExtended = false;
		bool waitingForRelease = false;
		getJoystickSettings(joystick);	//retrieves current joystick positions
		//Drive Code
		float leftPower = joystick.joy1_y1;
		float rightPower = joystick.joy1_y2;

		//left motor normalization and stopping at low joystick value
		if(abs(leftPower) <= driveLowerThreshold){	//if absolute value is < 15
			leftPower = 0;	//stop motors
		}
		else if(joy1Btn(8) == 1){	//power boost btn
			if(leftPower < 0){
				leftPower = drivePowerBoost2*-1;
			}
			else{
				leftPower = drivePowerBoost2;	//set to high power
			}
		}
		else if(joy1Btn(6) == 1){	//power boost btn
			if(leftPower < 0){
				leftPower = drivePowerBoost1*-1;
			}
			else{
				leftPower = drivePowerBoost1;	//set to high power
			}
		}
		else{
			leftPower = joystick.joy1_y1*multiplier;	//normalize
			//if(leftPower > driveUpperThreshold){	//sets it to lower power than power boost so
			//leftPower = driveUpperThreshold;
			//}
		}

		//right motor normalization and stopping at low joystick value
		if(abs(rightPower) <= driveLowerThreshold){	//if absolute value is < 15
			rightPower = 0;	//stop motors
		}
		else if(joy1Btn(8) == 1){	//power boost btn
			if(rightPower < 0){	//if its a negative #
				rightPower = drivePowerBoost2*-1;	//set to 100 reverse power
			}
			else{	//other wise- meaning it is a positve #
				rightPower = drivePowerBoost2;	//set to 100 forward power
			}
		}	//end of else-if
		else if(joy1Btn(6) == 1){	//power boost btn
			if(rightPower < 0){	//if its a negative #
				rightPower = drivePowerBoost1*-1;	//set to 100 reverse power
			}
			else{	//other wise- meaning it is a positve #
				rightPower = drivePowerBoost1;	//set to 100 forward power
			}
		}	//end of else-if
		else{
			rightPower = joystick.joy1_y2*multiplier;	//normalize
			//if(rightPower > driveUpperThreshold){	//sets it to lower power than power boost so
			//	rightPower = driveUpperThreshold;
			//}
		}
		//setting both motor powers
		motor[leftDrive] = leftPower;
		motor[rightDrive] = rightPower;



		//Lift code
		if(abs(joystick.joy2_y2) > 30){	//if the absolute value is > 30
			if(joystick.joy2_y2 > 0){	//if it's a positive #
				motor[lift] = 70;	//set to low power
			}
			else{
				motor[lift] = -40;
			}
		}//end of absolute value if
		else{
			motor[lift] = 0;
		}


		//Arm code
		if(abs(joystick.joy2_y1) > 30){			//if absolute value of joystick is less than 30
			if(joy2Btn(5) == 1){
				if( (abs(nMotorEncoder[arm]) >= 9000) && joystick.joy2_y1 > 0)
						motor[arm] = 0;
				else
					motor[arm] = joystick.joy2_y1*85/abs(joystick.joy2_y1);
			}
			else{
				if((abs(nMotorEncoder[arm]) >= 9000) && joystick.joy2_y1 > 0)
						motor[arm] = 0;
				else
					motor[arm] = joystick.joy2_y1*75/abs(joystick.joy2_y1);
			}
		}
		else{
			motor[arm] = 0;
		}
		//Bucket code
		//this section makes it go towards the front of the robot
		if(joy2Btn(8)){//kills on button release
				if(nMotorEncoder[bucket] > 150)//makes sure not already up (for front side)
					motor[bucket] = -(0.033 * abs(nMotorEncoder[bucket]) + 5);//speed is linear fuction of angle (5 added for safety as 0 is approached) NEEDS NEW SLOPE FOR NEW GEAR RATIO
				else if (nMotorEncoder[bucket] < 150)//if back, go the other way (from over the back)
					motor[bucket] = (0.033 * abs(nMotorEncoder[bucket]) + 5);//opposite direction function for over the back
		}

		else if(joy2Btn(7)){
			if(abs(nMotorEncoder[arm]) < 6740){
				//motor[bucket] = 10;
				motor[bucket] = ((-1/10) * nMotorEncoder[bucket] + 10);
			}
			else
				motor[bucket] = -((-1/10) * nMotorEncoder[bucket] + 10);
			//motor[bucket] = -10;
		}
		else if(joystick.joy2_TopHat == 0){ //if tophat is pressed up (0)
			motor[bucket] = 40; //else power =20  //35 without PID
		}
		//this section makes the bucket go towards the back of the robot
		else if(joystick.joy2_TopHat == 4){ //if prev is false and tophat is pressed down (4)
			motor[bucket] = -40; //else power = -20 //35 without PID
		}
		else{
			motor[bucket] = 0;
		}
		//Flag code
		if(joy2Btn(3)) {
			servo[flagMount] = 134;
			flagOut = true;
		}
		else if(joy2Btn(4) && flagOut == true){
			servo[flagMount] = 134;
			motor[flag] = 100;
		}
		else if(joy2Btn(2) && flagOut == true){
			servo[flagMount] = 134;
			motor[flag] = 15;
		}
		else if(joy2Btn(1)){
			servo[flagMount] = 17;
			flagOut = false;
		}
		else{
			motor[flag] = 0;
		}
		//nudge code
		if(joy1Btn(5)) {
			moveBackwardInches(25, 1, false, LEFTENCODER);
		}
		//dumper code for TeleOp
		if(joy2Btn(5) && dumperOut == false) {
			servo[dumper] = 30;
			ClearTimer(T4);//delay for boolean setting
			dumperOut = true;//limits to one use per match
		}
		else if(time1[T4]>3000){//wait for correct timer amount
			servo[dumper] = 233;//reset servo
		}
	}//end bracket of loop
}//end task main bracket
