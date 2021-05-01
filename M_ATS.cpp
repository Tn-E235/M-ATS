#include "stdafx.h"
#include "atsplugin.h"
#include "data.h"
#include "M_ATS.h"

using namespace mats;

void M_ATS::initialize(ATS_VEHICLESPEC s) {
	limitCheck = false;
	signalCheck = false;
	spec = s;
}

ATS_HANDLES M_ATS::run(
		int* panel, int* sound,
		TRAIN_STATUS data, ATS_VEHICLESTATE vs) {

	ATS_HANDLES output{};
	status = data;

	output.Brake = status.Brake;
	output.Power = status.Power;
	output.ConstantSpeed = status.ConstantSpeed;
	output.Reverser = status.Reverser;
	const float speed = vs.Speed;
	const int time = vs.Time;

	if (status.Pilotlamp) output.Reverser = status.Reverser;
	else output.Reverser = 0;
	output.Brake = status.Brake;
	output.Power = status.Power;
	output.ConstantSpeed = ATS_CONSTANTSPEED_CONTINUE;

	if (signalCheck) {
		switch (signalCheckIndex) {
		case SIG_0:
			if (speed > SIG_R) isEB = true;
			break;
		case SIG_1:
			if (speed > SIG_YY) isEB = true;
			break;
		case SIG_2:
			if (speed > SIG_Y) isEB = true;
			break;
		case SIG_3:
			if (speed > SIG_YG) isEB = true;
			break;
		}
		signalCheck = false;
		signalCheckIndex = 0;
	}
	if (limitCheck) {
		if (speed > limitSpeed / 10.0) isEB = true;
		limitCheck = false;
	}
	if (isEB) output.Brake = status.emgBrake;

	return output;
}

void M_ATS::keyDown(int key) {
	// ATS復帰
	if (key == ATS_KEY_B1) {
		if (status.Reverser == 0 && status.Power == 0
			&& status.Brake == status.emgBrake) {
			isEB = false;
		}
	}
}

void M_ATS::beacon(ATS_BEACONDATA beacon) {
	const int type = beacon.Type;
	const int sendData = beacon.Optional;
	const int signal = beacon.Signal;
	const int distance = beacon.Distance;
	switch (type) {
		case ATS_TIMER:
			const int option = sendData / 10000;
			if (option == 0) {
				if (sendData == 0) {
					// 二点間照査モード(速度制限)
				} else {
					// 点照査モード(速度制限)
					limitSpeed = sendData;
					limitCheck = true;
				}
			} else if (option == 1) {
				// 二点間照査モード(信号)
			} else if (option == 2) {
				// 点照査モード(信号)
				if (signal > SIG_3) break;
				signalCheckIndex = signal;
				signalCheck = true;
			}
			break;
	}
}

void M_ATS::setting(M_ATS_PANEL p, M_ATS_SOUND s, M_ATS_KEY k) {
	m_panel = p;
	m_sound = s;
	m_key = k;
}