#include "stdafx.h"
#include "atsplugin.h"
#include "data.h"
#include "M_ATS.h"
#include "ATS_P.h"
#include "ini.h"
#include "Ats.h"
#include <cmath>

BOOL APIENTRY DllMain( HANDLE h, DWORD d, LPVOID l) { 
	ATSP_PANEL pp;
	ATSP_SOUND ps;
	ATSP_KEY pk;
	if (loadIni) return TRUE;
	settingP(h, pp, ps, pk);
	settingM(h, pp, ps, pk);
	loadIni = true;
	return TRUE; 
}
ATS_API int WINAPI GetPluginVersion() { return ATS_VERSION; }
ATS_API void WINAPI SetVehicleSpec(ATS_VEHICLESPEC s) {
	spec = s;
	status.svcBrake = spec.BrakeNotches;
	status.emgBrake = spec.BrakeNotches + 1;
}
ATS_API void WINAPI Initialize(int brake) {
	m_ats.initialize(spec);
	ats_p.initialize(spec);
}
ATS_API ATS_HANDLES WINAPI Elapse(
		ATS_VEHICLESTATE vs, int *p, int *s) {
	if (ATSP_P != -1) p[ATSP_P] = 1;
	ats_p.setData(status, vs);
	if (ats_p.isEnable() && ATSP) {
		return ats_p.run(p, s);
	} else {
		return m_ats.run(p, s, status, vs);
	}
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
ATS_API void WINAPI SetSignal(int s) { 
	ats_p.setSignal(s);
}
ATS_API void WINAPI SetBeaconData(ATS_BEACONDATA beacon) {
	m_ats.beacon(beacon);
	ats_p.beacon(beacon);
}
ATS_API void WINAPI Load() {}
ATS_API void WINAPI Dispose() {}
void settingP(HANDLE h, ATSP_PANEL p, ATSP_SOUND s, ATSP_KEY k) {
	char filePath[_MAX_PATH + 1] = _T("");
	::GetModuleFileName((HMODULE)h, filePath, _MAX_PATH);
	memmove(strstr(filePath, ".dll"), ".ini", 4);
	readIni ini;
	if (!ini.load(filePath)) return;
	if (ini.GENERAL.atsp == 1) ATSP = true;
	p.platformDoor       = ini.PANEL_ATSP.platformDoor;
	p.platformDoor_break = ini.PANEL_ATSP.platformDoor_break;
	p.atsp_enable        = ini.PANEL_ATSP.atsp_enable;
	p.atsp_pattern       = ini.PANEL_ATSP.atsp_pattern;
	p.atsp_brake         = ini.PANEL_ATSP.atsp_brake;
	p.atsp_break         = ini.PANEL_ATSP.atsp_break;
	p.atsp_power         = ini.PANEL_ATSP.atsp_power;
	p.atsp_release       = ini.PANEL_ATSP.atsp_release;
	s.bell               = ini.SOUND.patternBell;
	ats_p.setting(p, s, k);
	ATSP_P = p.atsp_power;
}
void settingM(HANDLE h, ATSP_PANEL p, ATSP_SOUND s, ATSP_KEY k) {
	char filePath[_MAX_PATH + 1] = _T("");
	::GetModuleFileName((HMODULE)h, filePath, _MAX_PATH);
	memmove(strstr(filePath, ".dll"), ".ini", 4);
	readIni ini;
	if (!ini.load(filePath)) return;
}