#ifndef DVB_TYPES_H
#define DVB_TYPES_H

#ifndef DVBS2_NOINCLUDE
#include "inttypes.h"
#endif

#define BB_HEADER_LENGTH_BITS 72

// BB HEADER fields
#define TS_GS_TRANSPORT 3
#define TS_GS_GENERIC_PACKETIZED 0
#define TS_GS_GENERIC_CONTINUOUS 1
#define TS_GS_RESERVED 2

#define SIS_MIS_SINGLE 1
#define SIS_MIS_MULTIPLE 0

#define CCM 1
#define ACM 0

#define ISSYI_ACTIVE 1
#define ISSYI_NOT_ACTIVE 0

#define NPD_ACTIVE 1
#define NPD_NOT_ACTIVE 0

enum {
    FRAME_SIZE_NORMAL = 64800,
    FRAME_SIZE_SHORT = 16200,
};

// BCH Code
#define BCH_CODE_N8  0
#define BCH_CODE_N10 1
#define BCH_CODE_N12 2
#define BCH_CODE_S12 3

#define LDPC_ENCODE_TABLE_LENGTH (FRAME_SIZE_NORMAL * 10)

// Code rates
enum {
    DVBS2_CR_1_4 = 0,
    DVBS2_CR_1_3 = 1,
    DVBS2_CR_2_5 = 2,
    DVBS2_CR_1_2 = 3,
    DVBS2_CR_3_5 = 4,
    DVBS2_CR_2_3 = 5,
    DVBS2_CR_3_4 = 6,
    DVBS2_CR_4_5 = 7,
    DVBS2_CR_5_6 = 8,
    DVBS2_CR_8_9 = 9,
    DVBS2_CR_9_10 = 10,
    CODE_RATE_COUNT	= 11,
};

#define FRAME_NORMAL 0x00
#define FRAME_SHORT  0x10


// Constellation
enum {
    DVBS2_M_QPSK   = 0,
    DVBS2_M_8PSK   = 1,
    DVBS2_M_16APSK = 2,
    DVBS2_M_32APSK = 3,
};

#define M_CONST_NUMBER	 4
#define M_SYMBOL_SIZE_MAX (1<<5)//32

enum Rolloff_Factor
{
	RO_0_35 = 0,
	RO_0_25,
	RO_0_20,
	RO_RESERVED,
	RO_0_15,
	RO_0_10,
	RO_0_05,
};

#define CP 0x7FFF

#define		CODE_RATE_DEFAULT	DVBS2_CR_1_2
#define		FRAME_TYPE_DEFAULT	FRAME_SHORT

typedef struct{
    short re;
    short im;
}scmplx;


typedef struct{
    double re;
    double im;
}dcmplx;

typedef struct{
    float re;
    float im;
}fcmplx;

typedef struct{
	int ts_gs;
	int sis_mis;
	int ccm_acm;
	int issyi;
	int npd;
	int ro;
	int isi;
	int upl;
	int dfl;
	int sync;
	int syncd;
}BBHeader;

typedef int Bit;

typedef struct{
	int frame_type;
	int code_rate;
	int roll_off;
	int constellation;
	int pilots;
	int dummy_frame;
	int null_deletion;
	int interface;
	int broadcasting;
	// Calculated information, not used by caller
	BBHeader bb_header;
	int kldpc;
	int kbch;
	int nldpc;
	int q_val;
	int bch_code;
	int useable_data_bits;
	int padding_bits;
	int total_bits;
	int nr_tps;
	int nr_tps_bits;
}DVB2FrameFormat;


#endif // DVB_TYPES_H
