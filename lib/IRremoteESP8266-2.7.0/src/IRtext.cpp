// Copyright 2019 - David Conran (@crankyoldgit)

#ifndef UNIT_TEST
#include <Arduino.h>
#endif  // UNIT_TEST
#include "IRremoteESP8266.h"
#include "i18n.h"

// Common

String kUnknownStr = D_STR_UNKNOWN;
String kProtocolStr = D_STR_PROTOCOL;
String kPowerStr = D_STR_POWER;
String kOnStr = D_STR_ON;
String kOffStr = D_STR_OFF;
String kModeStr = D_STR_MODE;
String kToggleStr = D_STR_TOGGLE;
String kTurboStr = D_STR_TURBO;
String kSuperStr = D_STR_SUPER;
String kSleepStr = D_STR_SLEEP;
String kLightStr = D_STR_LIGHT;
String kPowerfulStr = D_STR_POWERFUL;
String kQuietStr = D_STR_QUIET;
String kEconoStr = D_STR_ECONO;
String kSwingStr = D_STR_SWING;
String kSwingHStr = D_STR_SWINGH;
String kSwingVStr = D_STR_SWINGV;
String kBeepStr = D_STR_BEEP;
String kZoneFollowStr = D_STR_ZONEFOLLOW;
String kFixedStr = D_STR_FIXED;
String kMouldStr = D_STR_MOULD;
String kCleanStr = D_STR_CLEAN;
String kPurifyStr = D_STR_PURIFY;
String kTimerStr = D_STR_TIMER;
String kOnTimerStr = D_STR_ONTIMER;
String kOffTimerStr = D_STR_OFFTIMER;
String kClockStr = D_STR_CLOCK;
String kCommandStr = D_STR_COMMAND;
String kXFanStr = D_STR_XFAN;
String kHealthStr = D_STR_HEALTH;
String kModelStr = D_STR_MODEL;
String kTempStr = D_STR_TEMP;
String kIFeelStr = D_STR_IFEEL;
String kHumidStr = D_STR_HUMID;
String kSaveStr = D_STR_SAVE;
String kEyeStr = D_STR_EYE;
String kFollowStr = D_STR_FOLLOW;
String kIonStr = D_STR_ION;
String kFreshStr = D_STR_FRESH;
String kHoldStr = D_STR_HOLD;
String kButtonStr = D_STR_BUTTON;
String k8CHeatStr = D_STR_8C_HEAT;
String kNightStr = D_STR_NIGHT;
String kSilentStr = D_STR_SILENT;
String kFilterStr = D_STR_FILTER;
String k3DStr = D_STR_3D;
String kCelsiusStr = D_STR_CELSIUS;
String kTempUpStr = D_STR_TEMPUP;
String kTempDownStr = D_STR_TEMPDOWN;
String kStartStr = D_STR_START;
String kStopStr = D_STR_STOP;
String kMoveStr = D_STR_MOVE;
String kSetStr = D_STR_SET;
String kCancelStr = D_STR_CANCEL;
String kUpStr = D_STR_UP;
String kDownStr = D_STR_DOWN;
String kChangeStr = D_STR_CHANGE;
String kComfortStr = D_STR_COMFORT;
String kSensorStr = D_STR_SENSOR;
String kWeeklyTimerStr = D_STR_WEEKLYTIMER;
String kWifiStr = D_STR_WIFI;
String kLastStr = D_STR_LAST;
String kFastStr = D_STR_FAST;
String kSlowStr = D_STR_SLOW;
String kAirFlowStr = D_STR_AIRFLOW;
String kStepStr = D_STR_STEP;
String kNAStr = D_STR_NA;
String kOutsideStr = D_STR_OUTSIDE;
String kLoudStr = D_STR_LOUD;
String kLowerStr = D_STR_LOWER;
String kUpperStr = D_STR_UPPER;
String kBreezeStr = D_STR_BREEZE;
String kCirculateStr = D_STR_CIRCULATE;
String kCeilingStr = D_STR_CEILING;
String kWallStr = D_STR_WALL;
String kRoomStr = D_STR_ROOM;
String k6thSenseStr = D_STR_6THSENSE;

String kAutoStr = D_STR_AUTO;
String kAutomaticStr = D_STR_AUTOMATIC;
String kManualStr = D_STR_MANUAL;
String kCoolStr = D_STR_COOL;
String kHeatStr = D_STR_HEAT;
String kFanStr = D_STR_FAN;
String kDryStr = D_STR_DRY;
String kFanOnlyStr = D_STR_FANONLY;

String kMaxStr = D_STR_MAX;
String kMaximumStr = D_STR_MAXIMUM;
String kMinStr = D_STR_MIN;
String kMinimumStr = D_STR_MINIMUM;
String kMedStr = D_STR_MED;
String kMediumStr = D_STR_MEDIUM;

String kHighestStr = D_STR_HIGHEST;
String kHighStr = D_STR_HIGH;
String kHiStr = D_STR_HI;
String kMidStr = D_STR_MID;
String kMiddleStr = D_STR_MIDDLE;
String kLowStr = D_STR_LOW;
String kLoStr = D_STR_LO;
String kLowestStr = D_STR_LOWEST;
String kMaxRightStr = D_STR_MAXRIGHT;
String kRightMaxStr = D_STR_RIGHTMAX_NOSPACE;
String kRightStr = D_STR_RIGHT;
String kLeftStr = D_STR_LEFT;
String kMaxLeftStr = D_STR_MAXLEFT;
String kLeftMaxStr = D_STR_LEFTMAX_NOSPACE;
String kWideStr = D_STR_WIDE;
String kCentreStr = D_STR_CENTRE;
String kTopStr = D_STR_TOP;
String kBottomStr = D_STR_BOTTOM;

String kSpaceLBraceStr = D_STR_SPACELBRACE;
String kCommaSpaceStr = D_STR_COMMASPACE;
String kColonSpaceStr = D_STR_COLONSPACE;

// IRutils
//  - Time
char kTimeSep = D_CHR_TIME_SEP;
String kDayStr = D_STR_DAY;
String kDaysStr = D_STR_DAYS;
String kHourStr = D_STR_HOUR;
String kHoursStr = D_STR_HOURS;
String kMinuteStr = D_STR_MINUTE;
String kMinutesStr = D_STR_MINUTES;
String kSecondStr = D_STR_SECOND;
String kSecondsStr = D_STR_SECONDS;
String kNowStr = D_STR_NOW;
String kThreeLetterDayOfWeekStr = D_STR_THREELETTERDAYS;

String kYesStr = D_STR_YES;
String kNoStr = D_STR_NO;
String kTrueStr = D_STR_TRUE;
String kFalseStr = D_STR_FALSE;

String kRepeatStr = D_STR_REPEAT;
String kCodeStr = D_STR_CODE;
String kBitsStr = D_STR_BITS;
