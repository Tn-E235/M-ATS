#include "stdafx.h"
#include "atsplugin.h"
#include "data.h"
#include "ATS_P.h"
#include <climits>

using namespace atsp;

void ATS_P::initialize(ATS_VEHICLESPEC s) {
	limitCtl = false;
	signalCtl = false;
	decelerate = 3.0;
	limitSpeed = MAX_SPEED;
	trainDistance = LENGTH * s.Cars;
	limitPos_E = INT_MAX;
	spec = s;
}

ATS_HANDLES ATS_P::run(
		int* panel, int* sound,
		TRAIN_STATUS status, ATS_VEHICLESTATE vs) {

	ATS_HANDLES output{};
	output.Brake = status.Brake;
	output.Power = status.Power;
	output.ConstantSpeed = status.ConstantSpeed;
	output.Reverser = status.Reverser;
	const double speed = vs.Speed;
	const double location = vs.Location;
	const int currentSignalIndex = status.Signal;
	// ------------------------------------------------------------------------
	bool atspBrake_l = false;
	panel[255] = limitSpeed;
	if (limitCtl) {
		if (limitPos_E <= location) {
			// 速度制限解除
			limitCtl = false;
			limitSpeed = MAX_SPEED;
			limitPos_E = INT_MAX;
		} else if (limitPos_S <= location) {
			// 速度制限区間内
			if (speed > limitSpeed) atspBrake_l = true;
		} else {
			// 速度制限手前まで
			if (speed > getPattern(limitPos_S-location)) 
				atspBrake_l = true;
			panel[254] = getPattern(limitPos_S - location);
		}
	}
	// ------------------------------------------------------------------------
	//bool atspBrake_s = false;

	//// 信号手前まで
	//if (signalPos > location) {
	//	int signalLimit = 0;
	//	switch (nextSignalIndex) {
	//		case SIG_0:
	//			signalLimit = SIG_R;
	//			break;
	//		case SIG_1:
	//			signalLimit = SIG_YY;
	//			break;
	//		case SIG_2:
	//			signalLimit = SIG_Y;
	//			break;
	//		case SIG_3:
	//			signalLimit = SIG_YG;
	//			break;
	//		default:
	//			signalLimit = MAX_SPEED;
	//			break;
	//	}
	//	const double pattern =
	//		getBrakePattern(signalLimit, speed, location);
	//	double remDis = limitPos_S - location;
	//	if (pattern > remDis) atspBrake_s = true;
	//}
	//if (speed > getSignalSpeed(currentSignalIndex)) {
	//	atspBrake_s = true;
	//}
	// ------------------------------------------------------------------------
	if (atspBrake_l) atspBrake = true;
	if (atspBrake) {
		if (speed <= limitSpeed - RELEASE_BRAKE_SPEED) {
			output.Brake = status.Brake;
			output.Power = status.Power;
			atspBrake = false;
		} else {
			output.Power = 0;
			output.Brake = status.svcBrake;
		}
	}
	// ------------------------------------------------------------------------
	v = vs;
	return output;
}

void ATS_P::beacon(ATS_BEACONDATA beacon) {
	const int type     = beacon.Type;
	const int sendData = beacon.Optional;
	const int signal   = beacon.Signal;
	const int distance = beacon.Distance;
	switch (type) {
		int dis;
		int lim;
		case ATSP_ENABLE:
			if (sendData == 0) atspEnable = false; 
			else if (sendData == 1) atspEnable = true;
			break;
		case ATSP_SIGNAL_S:
			dis = sendData % 1000;
			signalPos = v.Location + dis;
			signalCtl = true;
			nextSignalIndex = signal;
			break;
		case ATSP_SIGNAL_U:
			nextSignalIndex = signal;
			break;
		case ATSP_LIMIT_S:
			dis = sendData / 1000;
			lim = sendData % 1000;
			limitCtl = true;
			limitSpeed = lim;
			limitPos_S = v.Location + dis;
			break;
		case ATSP_LIMIT_E:
			limitPos_E = v.Location + trainDistance;
			break;
	}
}

bool ATS_P::isEnable() { return atspEnable; }

double ATS_P::getRemDistance(
			int limit, double speed, double location) {
	const double decelerate = 3.0;
	return (speed * speed - limit * limit) / 7.2 / decelerate;
}

int ATS_P::getSignalSpeed(int sig) {
	switch (sig) {
		case SIG_0:
			return SIG_R;
		case SIG_1:
			return SIG_YY;
		case SIG_2:
			return SIG_Y;
		case SIG_3:
			return SIG_YG;
		default:
			return MAX_SPEED;
	}
	return MAX_SPEED;
}

double ATS_P::getPattern(double remDis) {
	double d = (limitSpeed * limitSpeed) / (7.2 * decelerate);
	double s = sqrt(7.2 * decelerate * (d + remDis));
	return (s > MAX_SPEED) ? MAX_SPEED : s;
}
