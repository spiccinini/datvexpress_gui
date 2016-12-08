#ifndef DVB_TYPES_H
#define DVB_TYPES_H

#include "inttypes.h"

#pragma once

#include <vector>
#include <map>
#include <iostream>
using namespace std;


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

#define FRAME_SIZE_NORMAL 64800
#define FRAME_SIZE_SHORT  16200

// BCH Code
#define BCH_CODE_N8  0
#define BCH_CODE_N10 1
#define BCH_CODE_N12 2
#define BCH_CODE_S12 3

#define LDPC_ENCODE_TABLE_LENGTH (FRAME_SIZE_NORMAL * 10)

// Code rates
#define CR_1_4 0
#define CR_1_3 1
#define CR_2_5 2
#define CR_1_2 3
#define CR_3_5 4
#define CR_2_3 5
#define CR_3_4 6
#define CR_4_5 7
#define CR_5_6 8
#define CR_8_9 9
#define CR_9_10 10
#define CODE_RATE_COUNT	11

#define FRAME_NORMAL 0x00
#define FRAME_SHORT  0x10


// Constellation
#define M_QPSK   0
#define M_8PSK   1
#define M_16APSK 2
#define M_32APSK 3
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

#define		CODE_RATE_DEFAULT	CR_1_2
#define		FRAME_TYPE_DEFAULT	FRAME_SHORT

typedef struct{
    short re;
    short im;
}scmplx;


//#define scmplx fftw_complex

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

// The number of useable and stuff bits in a frame
typedef struct{
	int data_bits;
	int stuff_bits;
}FrameBits;

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

	int configure()
	{
		int error = 0;
		if( this->broadcasting )
		{
			// Set standard parametrs for broadcasting
			//this->frame_type        = FRAME_NORMAL;
			this->bb_header.ts_gs   = TS_GS_TRANSPORT;
			this->bb_header.sis_mis = SIS_MIS_SINGLE;
			this->bb_header.ccm_acm = CCM;
			this->bb_header.issyi   = 0;
			this->bb_header.npd     = 0;
			this->bb_header.upl     = 188*8;
			this->bb_header.sync    = 0x47;
		}
		this->bb_header.ro = this->roll_off;
		// Fill in the mode specific values and bit lengths
		if( this->frame_type == FRAME_NORMAL )
		{
			this->nldpc = 64800;
			this->bch_code = BCH_CODE_N12;
			// Apply code rate
			switch(this->code_rate )
			{
			case CR_1_4:
				this->q_val = 135;
				this->kbch  = 16008;
				this->bch_code = BCH_CODE_N12;
				break;
			case CR_1_3:
				this->q_val = 120;
				this->kbch  = 21408;
				this->bch_code = BCH_CODE_N12;
				break;
			case CR_2_5:
				this->q_val = 108;
				this->kbch  = 25728;
				this->bch_code = BCH_CODE_N12;
				break;
			case CR_1_2:
				this->q_val = 90;
				this->kbch  = 32208;
				this->bch_code = BCH_CODE_N12;
				break;
			case CR_3_5:
				this->q_val = 72;
				this->kbch  = 38688;
				this->bch_code = BCH_CODE_N12;
				break;
			case CR_2_3:
				this->q_val = 60;
				this->kbch  = 43040;
				this->bch_code = BCH_CODE_N10;
				break;
			case CR_3_4:
				this->q_val = 45;
				this->kbch  = 48408;
				this->bch_code = BCH_CODE_N12;
				break;
			case CR_4_5:
				this->q_val = 36;
				this->kbch  = 51648;
				this->bch_code = BCH_CODE_N12;
				break;
			case CR_5_6:
				this->q_val = 30;
				this->kbch  = 53840;
				this->bch_code = BCH_CODE_N10;
				break;
			case CR_8_9:
				this->q_val = 20;
				this->kbch  = 57472;
				this->bch_code = BCH_CODE_N8;
				break;
			case CR_9_10:
				this->q_val = 18;
				this->kbch  = 58192;
				this->bch_code = BCH_CODE_N8;
				break;
			default:
				printf("Configuration error DVB2\n");
				error = -1;
				break;
			}
		}

		if( this->frame_type == FRAME_SHORT )
		{
			this->nldpc = 16200;
			this->bch_code = BCH_CODE_S12;
			// Apply code rate
			switch(this->code_rate )
			{
			case CR_1_4:
				this->q_val = 36;
				this->kbch  = 3072;
				break;
			case CR_1_3:
				this->q_val = 30;
				this->kbch  = 5232;
				break;
			case CR_2_5:
				this->q_val = 27;
				this->kbch  = 6312;
				break;
			case CR_1_2:
				this->q_val = 25;
				this->kbch  = 7032;
				break;
			case CR_3_5:
				this->q_val = 18;
				this->kbch  = 9552;
				break;
			case CR_2_3:
				this->q_val = 15;
				this->kbch  = 10632;
				break;
			case CR_3_4:
				this->q_val = 12;
				this->kbch  = 11712;
				break;
			case CR_4_5:
				this->q_val = 10;
				this->kbch  = 12432;
				break;
			case CR_5_6:
				this->q_val = 8;
				this->kbch  = 13152;
				break;
			case CR_8_9:
				this->q_val = 5;
				this->kbch  = 14232;
				break;
			case CR_9_10:
				error = 1;
				this->kbch  = 0;
				break;
			default:
				printf("Configuration error DVB2\n");
				error = -1;
				break;
			}
		}

		int bch_bits = 0;
		if( this->frame_type == FRAME_NORMAL )
		{
			switch(this->code_rate )
			{
			case CR_2_3:
			case CR_5_6:
				bch_bits = 160;
				break;
			case CR_8_9:
			case CR_9_10:
				bch_bits = 128;
				break;
			default:
				bch_bits = 192;
			}
		}
		else
		{
			bch_bits = 168;
		}

		if( error == 0 )
		{
			// Length of the user packets
			this->bb_header.upl  = 188*8;
			// Payload length
			this->bb_header.dfl = this->kbch - 80;
			// Transport packet sync
			this->bb_header.sync = 0x47;
			// Start of LDPC bits
			this->kldpc = this->kbch + bch_bits;
			// Number of padding bits required (not used)
			this->padding_bits = 0;
			// Number of useable data bits (not used)
			this->useable_data_bits = this->kbch - 80;
		}

		return error;
	}
}DVB2FrameFormat;


#endif // DVB_TYPES_H
