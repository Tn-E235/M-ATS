#pragma once

struct TRAIN_STATUS {
	int emgBrake;		// 非常ノッチ
	int svcBrake;		// 常用最大ノッチ
	int Signal;			// 現在の信号インデックス
	bool Pilotlamp;		// パイロットランプ

	int Brake;			// Brake Notch
	int Power;			// Power Notch
	int Reverser;		// Reverser Position
	int ConstantSpeed;	// Constant Speed Control
};

class ATSP_PANEL {
	public:
		int platformDoor = -1;
		int platformDoor_break = -1;
		int atsp_enable = -1;
		int atsp_pattern = -1;
		int atsp_brake = -1;
		int atsp_break = -1;
		int atsp_power = -1;
		int atsp_release = -1;
};

class ATSP_SOUND {
	public:
		int bell = -1;
};

class ATSP_KEY {
	public:
};

class M_ATS_PANEL {
	public:
};

class M_ATS_SOUND {
	public:
};

class M_ATS_KEY {
	public:
};