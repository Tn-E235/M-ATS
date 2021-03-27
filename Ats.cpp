#include "stdafx.h"
#include "atsplugin.h"
#include "Ats.h"
#include <cmath>

BOOL APIENTRY DllMain( HANDLE h, DWORD d, LPVOID l) { return TRUE; }
ATS_API int WINAPI GetPluginVersion() { return ATS_VERSION; }
ATS_API void WINAPI SetVehicleSpec(ATS_VEHICLESPEC spec) {
	g_svcBrake = spec.BrakeNotches;
	g_emgBrake = g_svcBrake + 1;
}
ATS_API void WINAPI Initialize(int brake) { 
	limitCheck = false;
	signalCheck = false;
}
ATS_API ATS_HANDLES WINAPI Elapse(
		ATS_VEHICLESTATE vs, int *panel, int *sound) {
	g_speed = vs.Speed;
	if (g_pilotlamp) g_output.Reverser = g_reverser; 
	else g_output.Reverser = 0; 
	g_output.Brake = g_brakeNotch;
	g_output.Power = g_powerNotch;
	g_output.ConstantSpeed = ATS_CONSTANTSPEED_CONTINUE;

	if (signalCheck) {
		switch (signalCheckIndex) {
			case SIG_0:
				if (g_speed > SIG_R) isEB = true;
				break;
			case SIG_1:
				if (g_speed > SIG_YY) isEB = true;
				break;
			case SIG_2:
				if (g_speed > SIG_Y) isEB = true;
				break;
			case SIG_3:
				if (g_speed > SIG_YG) isEB = true;
				break;
		}
		signalCheck = false;
		signalCheckIndex = 0;
	}
	if (limitCheck) {
		if (g_speed > limitSpeed / 10) isEB = true; 
		limitCheck = false;
	}
	if (isEB) g_output.Brake = g_emgBrake;
	
    return g_output;
}

ATS_API void WINAPI SetPower(int n) { g_powerNotch = n; }
ATS_API void WINAPI SetBrake(int n) { g_brakeNotch = n; }
ATS_API void WINAPI SetReverser(int p) { g_reverser = p; }
ATS_API void WINAPI KeyDown(int key) {
	// ATS復帰
	if (key == ATS_KEY_B1) {
		if (g_reverser == 0 && g_powerNotch == 0 
				&& g_brakeNotch == g_emgBrake) {
			isEB = false;
		}
	}
}
ATS_API void WINAPI KeyUp(int key) {}
ATS_API void WINAPI HornBlow(int h) {}
ATS_API void WINAPI DoorOpen() { g_pilotlamp = false; }
ATS_API void WINAPI DoorClose() { g_pilotlamp = true; }
ATS_API void WINAPI SetSignal(int s) { g_signal = s; }
ATS_API void WINAPI SetBeaconData(ATS_BEACONDATA beacon) {
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
			} else if(option == 2) {
				// 点照査モード(信号)
				if (signal > SIG_3) break;
				signalCheckIndex = signal;
				signalCheck = true;
			}
			break;
	}
}
ATS_API void WINAPI Load() {}
ATS_API void WINAPI Dispose() {}