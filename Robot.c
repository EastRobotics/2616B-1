#define VOLTAGE_THRESHOLD 1250

#define LIFT_LOWER_LIMIT 840
#define LIFT_UPPER_LIMIT 2270

void lift(int power) {
	motor[LLift] = power;
	motor[RLift] = power;
}

void drive(int left, int right) {
	motor[LFDrive] = left;
	motor[LCDrive] = left;
	motor[LDDrive] = left;
	motor[RFDrive] = right;
	motor[RCDrive] = right;
	motor[RDDrive] = right;
}

void driveAdjusted(int left, int right) {
	drive(left, right);
}

void intake(int power) {
	motor[LIntake] = power;
	motor[RIntake] = power;
}

void launcher(bool launch) {
	SensorValue[solenoid] = launch ? 1 : 0;
}

bool launched() {
	return (SensorValue[solenoid] != 0);
}

void displayVoltage(int line, int position, int millivolts, bool leftAligned) {
	displayLCDPos(line, position);

	if (millivolts <= VOLTAGE_THRESHOLD) {
		displayNextLCDString(leftAligned ? "None " : " None");
		return;
	}

	float volts = ((float)millivolts / 1000);

	string displayString;
	sprintf(displayString, "%1.2f", volts);
	displayNextLCDString(displayString);
	displayNextLCDChar('V');
}

void displayLCDVoltageString(int line) {
	clearLCDLine(line);
	int expanderBatteryLevel = (float)SensorValue[expander] * 1000 / 280;
	displayVoltage(1, 0, nImmediateBatteryLevel, true);
	displayVoltage(1, 11, expanderBatteryLevel, false);
}

void driveTicks(int power, int ticks) {
	nMotorEncoder[LDDrive] = 0;
	nMotorEncoder[RDDrive] = 0;
	while (abs(nMotorEncoder[LDDrive]) < ticks || abs(nMotorEncoder[RDDrive]) < ticks) {
		driveAdjusted(abs(nMotorEncoder[LDDrive]) < ticks ? power : 0, abs(nMotorEncoder[RDDrive]) < ticks ? power : 0);
	}
	drive(0, 0);
}

void turnTicks(bool right, int power, int ticks) {
	nMotorEncoder[LDDrive] = 0;
	nMotorEncoder[RDDrive] = 0;
	int leftScale = right ? 1 : -1;
	int rightScale = right ? -1 : 1;
	while (abs(nMotorEncoder[LDDrive]) < ticks || abs(nMotorEncoder[RDDrive]) < ticks) {
		driveAdjusted(abs(nMotorEncoder[LDDrive]) < ticks ? leftScale * power : 0, abs(nMotorEncoder[RDDrive]) < ticks ? rightScale * power : 0);
	}
	drive(0, 0);
}

int liftTarget(int target) {
	clearTimer(T4);
	int maxTime = 2000;
	int difference = target - SensorValue[liftHeight];
	if (difference > 0) {
		while (SensorValue[liftHeight] < target && time1[T4] < maxTime) {
			lift(127);
		}
	} else if (difference < 0) {
		while (SensorValue[liftHeight] > target && time1[T4] < maxTime) {
			lift(-127);
		}
	}
	lift(0);
	if (time1[T4] >= maxTime) return -1;
	else return 0;
}
