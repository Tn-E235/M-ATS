#include "stdafx.h"
#include "atsplugin.h"
#include "data.h"
#include "M_ATS.h"
#include "ATS_P.h"
#include "Ats.h"
#include <cmath>

BOOL APIENTRY DllMain( HANDLE h, DWORD d, LPVOID l) { return TRUE; }
ATS_API int WINAPI GetPluginVersion() { return ATS_VERSION; }
ATS_API void WINAPI SetVehicleSpec(ATS_VEHICLESPEC s) {
	spec = s;
	status.svcBrake = spec.BrakeNotches;
	status.emgBrake = spec.BrakeNotches + 1;
}
ATS_API void WINAPI Initialize(int brake) {
	m_ats.initialize(spec);
}
ATS_API ATS_HANDLES WINAPI Elapse(
		ATS_VEHICLESTATE vs, int *p, int *s) {
	return m_ats.run(p, s, status, vs);
}

ATS_API void WINAPI SetPower(int n) { status.Power = n; }
ATS_API void WINAPI SetBrake(int n) { status.Brake = n; }
ATS_API void WINAPI SetReverser(int p) { status.Reverser = p; }
ATS_API void WINAPI KeyDown(int key) {
	m_ats.keyDown(key);
	// ats_p.keyDown(key);
}
ATS_API void WINAPI KeyUp(int key) {}
ATS_API void WINAPI HornBlow(int h) {}
ATS_API void WINAPI DoorOpen() { status.Pilotlamp = false; }
ATS_API void WINAPI DoorClose() { status.Pilotlamp = true; }
ATS_API void WINAPI SetSignal(int s) { status.Signal = s; }
ATS_API void WINAPI SetBeaconData(ATS_BEACONDATA beacon) {
	m_ats.beacon(beacon);
	// ats_p.beacon(beacon);
}
ATS_API void WINAPI Load() {}
ATS_API void WINAPI Dispose() {}