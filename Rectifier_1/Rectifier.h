#include "type_def.h"

typedef unsigned char		byte;
typedef signed short		s16;
typedef unsigned short		u16;

#define START_RECTIFIER		(u16)0x0001
#define ON_LINE				(u16)0x0002
#define FREQ_DETECT			(u16)0x0004
#define START_SECTOR		(u16)0x0008
#define	ENABLE_PI_CONTROL	(u16)0x0010

#define ALL_SECTOR					(u16)0xE000
#define SECTOR_126					(u16)0x2000
#define SECTOR_456					(u16)0x4000
#define SECTOR_156					(u16)0x6000
#define SECTOR_423					(u16)0x8000
#define SECTOR_123					(u16)0xA000
#define SECTOR_453					(u16)0xC000
#define SIGN						(int)0x80000000
#define Gop_1						(u16)0x1000
#define Gop_2						(u16)0x0800
#define Gop_3						(u16)0x0400
#define Gop_4						(u16)0x0200
#define Gop_5						(u16)0x0100
#define Gop_6						(u16)0x0080
#define Gop_1_OF					(u16)0x0C
#define Gop_2_OF					(u16)0x0B
#define Gop_3_OF					(u16)0x0A
#define Gop_4_OF					(u16)0x09
#define Gop_5_OF					(u16)0x08
#define Gop_6_OF					(u16)0x07
#define MaxTimeEnabledThyristor		(byte)0x04
#define	Max_Uop						(double)1.0
#define TH_1						(u16)0x01
#define TH_2						(u16)0x02
#define TH_3						(u16)0x04
#define TH_4						(u16)0x08
#define TH_5						(u16)0x10
#define TH_6						(u16)0x20
#define TH_1_6						(u16)0x3F
#define inverse_sequence			(u16)0x40

#define ud_max						0.32

typedef struct {
	double pi_ref;
	double pi_fdb;
	double Kp;
	double Ki;
	double ip_1;
	double pi_out_max;
	double pi_out_min;
	double pi_out;
} R_Regul;

typedef struct {
	double	k;
	u16		Tgen;
	double	Uop[3];
	u16		Control;
	double	Um;
	byte	Count[2];
	R_Regul Regul;
} R_CONTROL;

typedef struct {
	int A_B;
	int B_C;
	int C_A;
} ABC_SYSTEM;

typedef struct {
	u16			Control;
	ABC_SYSTEM	Uabc;
	double		Uabc_sum;
	double		Ud;
	R_CONTROL	rectifier;
} VARIABLE;

void RectifierSwitch(VARIABLE* src);
void RectifierControl_S1(VARIABLE* src);
void RectifierControl_S2(VARIABLE* src);
void RectifierControl_S3(VARIABLE* src);
void RectifierControl_S4(VARIABLE* src);
void RectifierControl_S5(VARIABLE* src);
void RectifierControl_S6(VARIABLE* src);
void Enable_Thyristor(VARIABLE* src, byte index, byte index_2, int Up_p_00, int Up_p_10, u16 Thyristor_0, u16 Thyristor_1, u16 Thyristor_2);
