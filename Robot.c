#define VOLTAGE_THRESHOLD 1250

void lift(int power) {
	motor[LLift] = power;
	motor[RLift] = power;
}

void drive(int left, int right) {
	motor[LFDrive] = left;
	motor[LCDrive] = left;
	motor[LRDrive] = left;
	motor[RFDrive] = right;
	motor[RCDrive] = right;
	motor[RRDrive] = right;
}

void intake(int power) {
	motor[LIntake] = power;
	motor[RIntake] = power;
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
