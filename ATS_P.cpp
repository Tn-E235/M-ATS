#include "stdafx.h"
#include "atsplugin.h"
#include "data.h"
#include "ATS_P.h"
#include <climits>
#include <vector>
using namespace atsp;

void ATS_P::initialize(ATS_VEHICLESPEC s) {
	decelerate = 3.0;
	trainDistance = LENGTH * s.Cars;
	spec = s;
	atsp_beacon_l.clear();
	sortATSP_BEACON_L();
}

ATS_HANDLES ATS_P::run(int* panel, int* sound) {

	ATS_HANDLES output{};
	output.Brake = status.Brake;
	output.Power = status.Power;
	output.ConstantSpeed = status.ConstantSpeed;
	output.Reverser = status.Reverser;
	const double speed = vs.Speed;
	const double location = vs.Location;
	sound[210] = ATS_SOUND_CONTINUE;
	// ------------------------------------------------------------------------
	// ���x��������
	double lp = info.speedLimit;
	bool atspBrake_l = false;
	bool atspPattern_l = false;
	if (!atsp_beacon_l.empty()) {
		double pt_l = MAX_SPEED;
		std::vector<ATSP_BEACON_L> data = atsp_beacon_l;
		for (int i = 0; i < data.size(); ++i) {
			// �������x��Ԃ𔲂����f�[�^�͏���
			ATSP_BEACON_L b = data[i];
			const double startLocation = b.getStartLocation();
			const double endLocation = b.getEndLocation();
			if (endLocation > 0 && endLocation <= location) {
				atsp_beacon_l.erase(atsp_beacon_l.begin());
				continue;
			}
			double p = 0.0;
			const int lim = b.getSpeedLimit();
			if (startLocation <= location
				&& (endLocation > location || endLocation < 0)) {
				// �������x��ԓ��̂Ƃ�
				p = lim;
				info.speedLimit = lim;
			} else {
				// �������x��Ԃ܂ł̃p�^�[������
				const double remDis = startLocation - location;
				p = getPattern(remDis, lim);
				if (isPatternAP(speed, remDis, lim)) atspPattern_l = true;
			}
			if (p < pt_l) pt_l = p;
		}
		if (speed > pt_l) atspBrake_l = true;
		lp = pt_l;
	} else {
		info.speedLimit = MAX_SPEED;
	}
	// ------------------------------------------------------------------------
	// �M������
	bool atspBrake_s = false;
	bool atspPattern_s = false;
	const int currentSignalIndex = info.signalIndex;
	double sp = getSignalSpeed(currentSignalIndex);
	// �M����O�܂�
	if (info.signalPos >= location) {
		// MessageBox(NULL, TEXT("Kitty on your lap"),TEXT("���b�Z�[�W�{�b�N�X"), MB_OK);
		const int nextSignalSpeed = getSignalSpeed(info.nextSignalIndex);
		const double remDis = info.signalPos - location;
		sp = getPattern(remDis, nextSignalSpeed);
		atspPattern_s = isPatternAP(speed, remDis, nextSignalSpeed);
	}
	if (speed > sp) atspBrake_s = true;
	// ------------------------------------------------------------------------
	// ATS-P�u���[�L����
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
	// �z�[���h�A
	panel[243] = 0;
	// �z�[���h�A�̏�H
	panel[244] = 0;
	// ATS-P
	const int atspEnableNum = (atspEnable) ? 1 : 0;
	if (panel[245] != atspEnableNum) {
		panel[245] = atspEnableNum;
		sound[210] = ATS_SOUND_PLAY;
	}
	// �̏�
	panel[248] = 0;
	// P�d��
	panel[249] = 1;
	// �J��
	panel[250] = 0;
	// �������x�E�M�������p�^�[��
	panel[254] = (int)min(lp, sp);		// G
	// �������x�E�M������
	panel[255] = (int)min(info.speedLimit, sp);	// R
	// �p�^�[���ڋ�
	const int patternAP = (atspPattern_l || atspPattern_s) ? 1 : 0;
	if (panel[246] != patternAP) {
		panel[246] = patternAP;
		sound[210] = ATS_SOUND_PLAY;
	}
	// �u���[�L����
	if (panel[247] != ((atspBrake) ? 1 : 0)) {
		panel[247] = atspBrake;
		sound[210] = ATS_SOUND_PLAY;
	}

	return output;
}

void ATS_P::beacon(ATS_BEACONDATA beacon) {
	const int type = beacon.Type;
	const int sendData = beacon.Optional;
	const int signal = beacon.Signal;
	const double distance = beacon.Distance;
	const double location = vs.Location;

	if (type == ATSP_ENABLE) {
		if (sendData == 0) {
			atspEnable = false;
		} else if (sendData == 1) {
			atspEnable = true;
			info.signalPos = distance + location;
			info.nextSignalIndex = signal;
			info.receiveSignalIndex = signal + 1;
			info.updateSignal();
		}
		return;
	}
	if (type == ATSP_SIGNAL_L) {
		info.signalPos = distance + location;
		info.nextSignalIndex = signal;
		info.updateSignal();
		return;
	}
	if (type == ATSP_SIGNAL_U) {
		info.signalPos = distance + location;
		info.nextSignalIndex = signal;
		info.updateSignal();
		return;
	}
	if (type == ATSP_LIMIT_S) {
		const int dis = sendData / 1000;
		const int lim = sendData % 1000;
		const double startLocation = location + dis;

		ATSP_BEACON_L atspb(location,startLocation,lim);
		if (atsp_beacon_l.empty()) {
			atsp_beacon_l.emplace_back(atspb);
			return;
		}

		const int size = atsp_beacon_l.size();
		ATSP_BEACON_L b = atsp_beacon_l[size-1];
		const int speedLimit = b.getSpeedLimit();
		// ���̐������x�����̐������x��荂���Ƃ� ��)80->90
		if (!b.isUpdated()) {
			if (speedLimit < lim) {
				b.setEndLocation(startLocation+trainDistance);
				atspb.setStartLocation(startLocation + trainDistance);
				b.update();
			} else {
				b.setEndLocation(startLocation);
				b.update();
			}
		}
		atsp_beacon_l.emplace_back(atspb);
		// �擪�v�f���폜
		atsp_beacon_l.erase(atsp_beacon_l.begin());
		// �擪�v�f�ɒǉ� -> ���ʓI�ɏ��X�V
		atsp_beacon_l.insert(atsp_beacon_l.begin(), b);
		sortATSP_BEACON_L();
		return;
	}
	if (type == ATSP_LIMIT_E) {
		if (atsp_beacon_l.empty()) return;

		int size = atsp_beacon_l.size();
		for (int i = 1; i < size; ++i)
			atsp_beacon_l.erase(atsp_beacon_l.begin());

		size = atsp_beacon_l.size();
		ATSP_BEACON_L b = atsp_beacon_l[size - 1];
		const double endLocation = location + trainDistance;
		b.setEndLocation(endLocation);
		b.update();
		// �擪�v�f���폜
		atsp_beacon_l.erase(atsp_beacon_l.begin());
		// �擪�v�f�ɒǉ� -> ���ʓI�ɏ��X�V
		atsp_beacon_l.insert(atsp_beacon_l.begin(), b);
		sortATSP_BEACON_L();
		return;
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

void ATS_P::setSignal(int s) { info.receiveSignalIndex = s; }

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

void ATS_P::sortATSP_BEACON_L() {
	const int size = atsp_beacon_l.size();
	if (size == 1) return;
	double dis = 0;
	for (int i = 0; i < size; ++i) {
		for (int j = size-1; j > i; --j) {
			double dis1 = atsp_beacon_l[j].getStartLocation();
			double dis2 = atsp_beacon_l[j-1].getStartLocation();
			if (dis1 < dis2) {
				ATSP_BEACON_L tmp = atsp_beacon_l[j];
				atsp_beacon_l[j] = atsp_beacon_l[j];
				atsp_beacon_l[j-1] = tmp;
			}
		}
	}
}

bool ATS_P::isPatternAP(double speed, double remDis, int lim) {
	double postion = speed / 3600.0 * 2.5 * 1000;
	return (getPattern(remDis - postion, lim) <= speed) ? true : false;
}

// ----------------------------------------------------------------------------
ATSP_BEACON_L::ATSP_BEACON_L() {
	putLocation   = -1;
	startLocation = -1;
	speedLimit    = -1;
	endLocation   = -1;
	updated       = false;
}

ATSP_BEACON_L::ATSP_BEACON_L(double i, double j, int k) {
	putLocation   = i;
	startLocation = j;
	speedLimit    = k;
	endLocation   = -1;
	updated       = false;
}

double ATSP_BEACON_L::getPutLocation() { return putLocation; }
double ATSP_BEACON_L::getStartLocation() { return startLocation; }
double ATSP_BEACON_L::getEndLocation() { return endLocation; }
int ATSP_BEACON_L::getSpeedLimit() { return speedLimit; }
void ATSP_BEACON_L::setStartLocation(double d) { startLocation = d; }
void ATSP_BEACON_L::setEndLocation(double d) { endLocation = d; }
void ATSP_BEACON_L::update() { updated = true; }
bool ATSP_BEACON_L::isUpdated() { return updated; }

// ----------------------------------------------------------------------------
ATSP_INFO::ATSP_INFO() {
	signalIndex = 0;
	nextSignalIndex = 0;
	receiveSignalIndex = 0;
	speedLimit = 0;
	signalPos = 0;
	pattern = 0;
}
void ATSP_INFO::updateSignal() { signalIndex = receiveSignalIndex; }
