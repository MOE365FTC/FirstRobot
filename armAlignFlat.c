#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     rightDrive,    tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     leftDrive,     tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     lift,          tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     motorG,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     arm,           tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     bucket,        tmotorTetrix, openLoop, encoder)
#pragma config(Servo,  srvo_S1_C4_1,    dumper,               tServoStandard)
#pragma config(Servo,  srvo_S1_C4_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//


//Constants for Screen coordinates
const int LEFT_X = 3;
const int RIGHT_X = 85;
const int BOTTOM_Y = 14;
const int TOP_Y = 60;

//Constants for button definitions. These values are hard-coded into RobotC.
const int NO_BUTTON = -1;
const int RIGHT_BUTTON = 1;
const int LEFT_BUTTON = 2;
const int ORANGE_BUTTON = 3;
const int DARK_BUTTON = 0;

task main(){
	PlaySound(soundBeepBeep);
	PlaySound(soundFastUpwardTones);
	servo[dumper] = 110;
	eraseDisplay();
	nxtDisplayStringAt(LEFT_X, BOTTOM_Y+5, "Left");
	nxtDisplayStringAt(RIGHT_X-10, BOTTOM_Y+5, "Right");
	while(true){
		if(nNxtButtonPressed == LEFT_BUTTON)	motor[arm] = -9;
		else if(nNxtButtonPressed == RIGHT_BUTTON) motor[arm] = 9;
		else motor[arm] = 0;
	}
}