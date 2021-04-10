#pragma once
namespace atsp {

	class ATS_P {
		public:
			ATS_HANDLES run(TRAIN_STATUS);
			void initialize();
			void beacon(ATS_BEACONDATA);
			void keuUp(int);
			void keyDown(int);
		private:
	};

}
