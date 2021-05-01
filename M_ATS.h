#pragma once
#define SIG_R 5
#define SIG_YY 30
#define SIG_Y 60
#define SIG_YG 90
#define SIG_0 0
#define SIG_1 1
#define SIG_2 2
#define SIG_3 3
#define ATS_TIMER 12

namespace mats {

	class M_ATS {
		public:
			ATS_HANDLES run(
				int*, int*, TRAIN_STATUS, 
				ATS_VEHICLESTATE);
			void initialize(ATS_VEHICLESPEC);
			void beacon(ATS_BEACONDATA);
			void keuUp(int);
			void keyDown(int);
			void setting(M_ATS_PANEL, M_ATS_SOUND, M_ATS_KEY);
			
		private:
			bool isEB;
			bool signalCheck;
			int signalCheckIndex;
			bool limitCheck;
			int limitSpeed;
			bool loadIni = false;
			TRAIN_STATUS status;
			ATS_VEHICLESPEC spec;
			M_ATS_PANEL m_panel;
			M_ATS_SOUND m_sound;
			M_ATS_KEY m_key;
	};

}