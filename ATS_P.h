#pragma once
#define MAX_SPEED 130
#define RELEASE_BRAKE_SPEED 10
#define STOP_SPEED 5
#define ATSP_TYPE       3
#define ATSP_ENABLE   120
#define ATSP_SIGNAL_L 121
#define ATSP_SIGNAL_U 122
#define ATSP_LIMIT_S  123
#define ATSP_LIMIT_E  124
#define SIG_R    0
#define SIG_YY  30
#define SIG_Y   60
#define SIG_YG  90
#define SIG_G  120
#define SIG_0    0
#define SIG_1    1
#define SIG_2    2
#define SIG_3    3
#define SIG_4    4
#define LENGTH  19

namespace atsp {

	class ATS_P {
		public:
			ATS_HANDLES run(int*, int*);
			void initialize(ATS_VEHICLESPEC);
			void beacon(ATS_BEACONDATA);
			void keuUp(int);
			void keyDown(int);
			bool isEnable();
			bool isPatternAP(double, double, int);
			double getRemDistance(int, double, double);
			void setData(TRAIN_STATUS, ATS_VEHICLESTATE);
			void setSignal(int);

		private:
			int getSignalSpeed(int);
			double getPattern(double, int);
			bool atspEnable;
			bool atspBrake;
			double decelerate;
			int limitPos_S;
			int limitPos_E;
			
			int limitSpeed;
			int signalPos;
			int nextSignalIndex;
			int currentSignalIndex;
			int SignalSpeed;
			bool limitCtl;
			bool signalCtl;
			double location;
			int trainDistance;
			int receiveSignalIndex;
			ATS_VEHICLESTATE vs;
			ATS_VEHICLESPEC spec;
			TRAIN_STATUS status;
	};

}
