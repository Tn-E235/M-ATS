#define SIG_R 5
#define SIG_YY 30
#define SIG_Y 60
#define SIG_YG 90
#define SIG_0 0
#define SIG_1 1
#define SIG_2 2
#define SIG_3 3
#define ATS_TIMER 12

int g_emgBrake;		// ���m�b�`
int g_svcBrake;		// ��p�ő�m�b�`
int g_brakeNotch;	// �u���[�L�m�b�`
int g_powerNotch;	// �͍s�m�b�`
int g_reverser;		// ���o�[�T
bool g_pilotlamp;	// �p�C���b�g�����v
int g_time;			// ���ݎ���
float g_speed;		// ���x�v�̑��x[km/h]
int g_deltaT;		// �t���[������[ms/frame]
int g_signal;
double g_position;
double d_position;
float d_speed;
bool isEB;
bool signalCheck;
int signalCheckIndex;
bool limitCheck;
int limitSpeed;
ATS_HANDLES g_output; // �o��