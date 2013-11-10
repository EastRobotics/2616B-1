#define ZONE_HANGING 0
#define ZONE_MIDDLE 1

#define COLOR_BLUE 0
#define COLOR_RED 1

int fieldZone = -1;
int fieldColor = -1;
bool enabled = false;

bool doneSelection = false;

task selection() {
  doneSelection = false;

  displayLCDCenteredString(0, "Select Zone");
  displayLCDString(1, 0, "Hanging");
  displayLCDString(1, 10, "Middle");
  int buttons = 0;
  while (true) {
    if ((buttons & 0x1) && !(nLCDButtons & 0x1)) {
      fieldZone = ZONE_HANGING;
      break;
    }
    if ((buttons & 0x4) && !(nLCDButtons & 0x4)) {
      fieldZone = ZONE_MIDDLE;
      break;
    }

    buttons = nLCDButtons;
    wait1Msec(20);
  }

  clearLCDLine(0);
  clearLCDLine(1);

  displayLCDCenteredString(0, "Select Color");
  displayLCDString(1, 0, "Blue");
  displayLCDString(1, 13, "Red");
  buttons = 0;
  while (true) {
    if ((buttons & 0x1) && !(nLCDButtons & 0x1)) {
      fieldColor = COLOR_BLUE;
      break;
    }
    if ((buttons & 0x4) && !(nLCDButtons & 0x4)) {
      fieldColor = COLOR_RED;
      break;
    }

    buttons = nLCDButtons;
    wait1Msec(20);
  }

  clearLCDLine(0);
  clearLCDLine(1);

  displayLCDCenteredString(0, "Enabled");
  displayLCDString(1, 0, "Yes");
  displayLCDString(1, 11, "No");
  buttons = 0;
  while (true) {
    if ((buttons & 0x1) && !(nLCDButtons & 0x1)) {
      enabled = true;
      break;
    }
    if ((buttons & 0x4) && !(nLCDButtons & 0x4)) {
      enabled = false;
      break;
    }

    buttons = nLCDButtons;
    wait1Msec(20);
  }

  clearLCDLine(0);
  clearLCDLine(1);

  doneSelection = true;
}

void SelectAutonomousRoutine() {
  if (!bIfiRobotDisabled) return;

  bLCDBacklight = true;
  StartTask(selection);

  while (!doneSelection) {
    if (!bIfiRobotDisabled) doneSelection = true;
    wait1Msec(25);
  }

  StopTask(selection);
  bLCDBacklight = false;
}
