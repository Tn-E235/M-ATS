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
