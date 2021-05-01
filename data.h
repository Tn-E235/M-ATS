#pragma once

struct TRAIN_STATUS {
	int emgBrake;		// ���m�b�`
	int svcBrake;		// ��p�ő�m�b�`
	int Signal;			// ���݂̐M���C���f�b�N�X
	bool Pilotlamp;		// �p�C���b�g�����v

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