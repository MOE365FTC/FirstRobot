#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     gyro,           sensorAnalogInactive)
#pragma config(Sensor, S3,     IR,             sensorI2CCustom)
#pragma config(Motor,  mtr_S1_C1_1,     rightDrive,    tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     leftDrive,     tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     lift,          tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     motorG,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     arm,           tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     bucket,        tmotorTetrix, openLoop, encoder)
#pragma config(Servo,  srvo_S1_C4_1,    dumper,               tServoStandard)
#pragma config(Servo,  srvo_S1_C4_2,    flagMount,            tServoStandard)
#pragma config(Servo,  srvo_S1_C4_3,    flagRaiser,           tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C4_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.
#include "hitechnic-irseeker-v2.h" //includes irseeker drivers
#include "gyro.h"//include gyro drivers
#include "pidturn.h"//includes pidturns
#include "main.h"//includes main library
#include "delay.h"//includes delay library

GYRO g_Gyro;
PIDTURN g_PidTurn;

const int MIN_TURN_POWER = 45;
const float TURN_KP = 0.85;//Default was 0.9
const float TURN_TOLERANCE = 0.3;

const int servoRestPosition = 233;

void initializeRobot(){
	servo[dumper] = servoRestPosition;
	servo[flagMount] = 17;
	return;
}

task main()
{
	int timeToWait = requestTimeToWait();
	initializeRobot();

	waitForStart(); // Wait for the beginning of autonomous phase.
	GyroInit(g_Gyro, gyro, 0);
	PidTurnInit(g_PidTurn, leftDrive, rightDrive, MIN_TURN_POWER, g_Gyro, TURN_KP, TURN_TOLERANCE);
	//Align against right wall, with left edge of left wheels on left edge of third tile (6ft from right wall).
	countdown(timeToWait);

	//moveForwardInches(60, 1, false, LEFTENCODER); //away from wall
	//turn(g_PidTurn, 136); //turn to parallel with buckets
	//clearEncoders(); //clears encoder for the next step
	//moveBackwardInches(60,4, false, LEFTENCODER);

	clearEncoders();
	wait1Msec(50);
	const int totalTics = 7600;//total tics from before IR to end-- DONT CHANGE!

	while(HTIRS2readACDir(IR) != 6){ //finds the beacon
		nxtDisplayCenteredTextLine(1,"Direction: %d", HTIRS2readACDir(IR));

		nxtDisplayCenteredTextLine(6,"%d",nMotorEncoder[leftDrive]);
		//if(abs(nMotorEncoder[leftDrive]) >= totalTics-500) break;
		startBackward(35);
	}
	stopDrive();
	wait1Msec(300);
	while(HTIRS2readACDir (IR)!= 5){
		startBackward(15);
	}
	moveBackwardInchesNoReset(30, 8);//reverse back a small amount to correct for IR inaccuracy

	stopDrive();//stops robot
	servo[dumper] = 30;//dumps the block
	motor[lift]= 50;//starts the lift up
	wait1Msec(700);
	motor[lift]= 0;//stops lift
	servo[dumper] = servoRestPosition;//resets servo
	int ticsToMove= totalTics - abs(nMotorEncoder[leftDrive]);//tics left after IR
	nxtDisplayCenteredTextLine(0,"TTM: %d", ticsToMove);
	//while(true){}
	moveBackwardTics(90, ticsToMove); //move to end after IR
	turn(g_PidTurn, -85,60); //turn to go towards ramp
	moveForwardInches(90, 44, false, LEFTENCODER); //forwards to ramp
	turn(g_PidTurn, 95, 60); //turn to face ramp
	moveForwardInches(90, 40, false, LEFTENCODER);//onto ramp
    motor[lift]= -50;//starts the lift down
	wait1Msec(500);
	motor[lift]= 0;//stops lift

	while (true)
	{}
}
