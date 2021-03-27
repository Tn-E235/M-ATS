#define SIG_R 5
#define SIG_YY 30
#define SIG_Y 60
#define SIG_YG 90
#define SIG_0 0
#define SIG_1 1
#define SIG_2 2
#define SIG_3 3
#define ATS_TIMER 12

int g_emgBrake;		// 非常ノッチ
int g_svcBrake;		// 常用最大ノッチ
int g_brakeNotch;	// ブレーキノッチ
int g_powerNotch;	// 力行ノッチ
int g_reverser;		// レバーサ
bool g_pilotlamp;	// パイロットランプ
int g_time;			// 現在時刻
float g_speed;		// 速度計の速度[km/h]
int g_deltaT;		// フレーム時間[ms/frame]
int g_signal;
double g_position;
double d_position;
float d_speed;
bool isEB;
bool signalCheck;
int signalCheckIndex;
bool limitCheck;
int limitSpeed;
ATS_HANDLES g_output; // 出力