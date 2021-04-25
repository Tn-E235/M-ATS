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
	receiveSignalIndex = SIG_0;
}

ATS_HANDLES ATS_P::run(int* panel, int* sound) {

	ATS_HANDLES output{};
	output.Brake = status.Brake;
	output.Power = status.Power;
	output.ConstantSpeed = status.ConstantSpeed;
	output.Reverser = status.Reverser;
	const double speed = vs.Speed;
	const double location = vs.Location;
	const int currentLimitSpeed = limitSpeed;
	const int currentSignalSpeed = getSignalSpeed(currentSignalIndex);
	const int nextSignalSpeed = getSignalSpeed(nextSignalIndex);
	// ------------------------------------------------------------------------
	sound[200] = ATS_SOUND_CONTINUE;
	// ------------------------------------------------------------------------
	bool atspBrake_l = false;
	bool atspPattern_l = false;
	double lp = limitSpeed;
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
			const double remDis = limitPos_S - location;
			lp = getPattern(limitPos_S - location, limitSpeed);
			if (speed > lp) atspBrake_l = true;
			atspPattern_l = isPatternAP(speed, remDis, limitSpeed);
		}
	}
	// ------------------------------------------------------------------------
	bool atspBrake_s = false;
	bool atspPattern_s = false;
	double sp = getSignalSpeed(currentSignalIndex);
	// 信号手前まで
	if (signalPos >= location) {
		// MessageBox(NULL, TEXT("Kitty on your lap"),TEXT("メッセージボックス"), MB_OK);
		const int nextSignalSpeed = getSignalSpeed(nextSignalIndex);
		const double remDis = signalPos - location;
		sp = getPattern(remDis, nextSignalSpeed);
		atspPattern_s = isPatternAP(speed, remDis, nextSignalSpeed);
	}
	if (speed > sp) atspBrake_s = true;
	// ------------------------------------------------------------------------
	bool atspBrake = false;
	if (atspBrake_l || atspBrake_s) {
		atspBrake = true;
		if (speed >= min(lp, sp) - RELEASE_BRAKE_SPEED) {
			output.Power = 0;
			output.Brake = status.svcBrake;
		}
	} else {
		output.Brake = status.Brake;
		output.Power = status.Power;
	}
	// ------------------------------------------------------------------------
	// ホームドア
	panel[243] = 0;
	// ホームドア故障？
	panel[244] = 0;
	// ATS-P
	const int atspEnableNum = (atspEnable) ? 1 : 0;
	if (panel[245] != atspEnableNum) {
		panel[245] = atspEnableNum;
		sound[200] = ATS_SOUND_PLAY;
	}
	// 故障
	panel[248] = 0;
	// P電源
	panel[249] = 1;
	// 開放
	panel[250] = 0;
	// 制限速度・信号現示パターン
	panel[254] = min(lp, sp);		// G
	// 制限速度・信号現示
	panel[255] = min(limitSpeed,currentSignalSpeed);	// R
	// パターン接近
	const int patternAP = (atspPattern_l || atspPattern_s) ? 1 : 0;
	if (panel[246] != patternAP) {
		panel[246] = patternAP;
		sound[200] = ATS_SOUND_PLAY;
	}
	// ブレーキ動作
	if (panel[247] != (atspBrake) ? 1 : 0) {
		panel[247] = atspBrake;
		sound[200] = ATS_SOUND_PLAY;
	}

	return output;
}

void ATS_P::beacon(ATS_BEACONDATA beacon) {
	const int type = beacon.Type;
	const int sendData = beacon.Optional;
	const int signal = beacon.Signal;
	const int distance = beacon.Distance;
	const int location = vs.Location;
	int dis;
	int lim;
	switch (type) {
		case ATSP_ENABLE:
			if (sendData == 0) {
				atspEnable = false;
			} else if (sendData == 1) {
				atspEnable = true;
				signalPos = distance + location;
				nextSignalIndex = signal;
				currentSignalIndex = signal;
				receiveSignalIndex = signal + 1;
			}
			break;
		case ATSP_SIGNAL_L:
			signalPos = distance + location;
			nextSignalIndex = signal;
			currentSignalIndex = receiveSignalIndex;
			break;
		case ATSP_SIGNAL_U:
			signalPos = distance + location;
			nextSignalIndex = signal;
			currentSignalIndex = receiveSignalIndex;
			break;
		case ATSP_LIMIT_S:
			dis = sendData / 1000;
			lim = sendData % 1000;
			limitCtl = true;
			limitSpeed = lim;
			limitPos_S = location + dis;
			break;
		case ATSP_LIMIT_E:
			limitPos_E = location + trainDistance;
			break;
	}
}

bool ATS_P::isEnable() { return atspEnable; }

double ATS_P::getRemDistance(
	int limit, double speed, double location) {
	const double decelerate = 3.0;
	return (speed * speed - limit * limit) / 7.2 / decelerate;
}

void ATS_P::setData(TRAIN_STATUS s, ATS_VEHICLESTATE v) {
	status = s;
	vs = v;
}

void atsp::ATS_P::setSignal(int s) { receiveSignalIndex = s; }

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

double ATS_P::getPattern(double remDis, int lim) {
	double d = (lim * lim) / (7.2 * decelerate);
	double s = sqrt(7.2 * decelerate * (d + remDis));
	return (s > MAX_SPEED) ? MAX_SPEED : s;
}

bool ATS_P::isPatternAP(double speed, double remDis, int lim) {
	double postion = speed / 3600.0 * 2.5 * 1000;
	return (getPattern(remDis - postion, lim) <= speed) ? true : false;
}