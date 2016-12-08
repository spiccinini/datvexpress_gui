#include "DVBS2.h"

#define PACKET_SIZE		188
#define PACKET_NUMBER	10000
#define PACKET_STREAM	(PACKET_NUMBER*PACKET_SIZE)
#define CP 0x7FFF
#define PRINT_SIZE		16
#define DATA_FROM_ENC	1	// ENCODE OR FILE
#define VALUE_DIFF		60

void init(u8* buffer, int n);	// initialize info


int main()
{
	scmplx pl[FRAME_SIZE_NORMAL];
	short  pBuffer[FRAME_SIZE_NORMAL*2];

	DVBS2*	m_dvbs2 = new DVBS2;
	DVB2FrameFormat	dvbs2_fmt;
	//
	// DVB-S2
	//
	dvbs2_fmt.frame_type    = FRAME_NORMAL;
	dvbs2_fmt.code_rate     = CR_3_4;
	dvbs2_fmt.constellation = M_32APSK;
	dvbs2_fmt.roll_off      = RO_0_35;
	dvbs2_fmt.pilots        = 0;
	dvbs2_fmt.dummy_frame   = 0;
	dvbs2_fmt.null_deletion = 0;
	int nStatus = m_dvbs2->s2_set_configure( &dvbs2_fmt );
	if( -1 == nStatus )
	{
		printf(" mode(%d, %d) is invalid ! \n",
			dvbs2_fmt.constellation, dvbs2_fmt.code_rate );

		delete	m_dvbs2;

		return -1;
	}

	u8 b[PACKET_STREAM];
	init( b, PACKET_STREAM );

	for (int i=0;i<PACKET_NUMBER;i++)
	{
		if( -1 == m_dvbs2->s2_add_ts_frame( b + i*PACKET_SIZE ) )
			break;
	}

		int nFrameCount = m_dvbs2->get_frame_count();
		printf("\nframe count : %d ... ... \n\n", nFrameCount );

		fwrite( &nFrameCount, sizeof(int), 1, stdout );

		for ( int i = 0; i<nFrameCount; i++ )
		{
            /*
			memcpy( pl, sizeof(scmplx)*FRAME_SIZE_NORMAL,
				m_dvbs2->pl_get_frame(i), sizeof(scmplx)*FRAME_SIZE_NORMAL);

			memcpy(  pBuffer,	sizeof(short)*FRAME_SIZE_NORMAL*2,
				pl, sizeof(scmplx)*FRAME_SIZE_NORMAL );

			fwrite( pBuffer, sizeof(short), FRAME_SIZE_NORMAL*2, stdout );
            */
		}

	delete	m_dvbs2;
    return 0;
}

void init(u8* buffer, int n)	// initialize info
{
	for (int i=0;i<n;i++)
		buffer[i] = i%256;
}
