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
