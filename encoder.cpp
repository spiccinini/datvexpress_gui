#include <chrono>
#include <iostream>
#include "DVBS2.h"

using namespace std;

#define PACKET_SIZE		188
#define PACKET_NUMBER	40000
#define PACKET_STREAM_SIZE	(PACKET_NUMBER*PACKET_SIZE)
#define RUNS     10


void dvb_s2_encode_tp(DVBS2* m_dvbs2, u8 *tp)
{
    int len;

    if((len = m_dvbs2->s2_add_ts_frame(tp, false)) > 0 )
    {
        // dvbs2_modulate(m_dvbs2->pl_get_frame(), len); TODO
    }
}

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
	dvbs2_fmt.code_rate     = DVBS2_CR_1_2;
	dvbs2_fmt.constellation = DVBS2_M_QPSK;
	dvbs2_fmt.roll_off      = RO_0_20;
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

	u8 data_stream[PACKET_STREAM_SIZE];
	for (size_t i=0; i<PACKET_STREAM_SIZE; ++i){
	    data_stream[i] = i % 256;
	}


	chrono::steady_clock::time_point begin = chrono::steady_clock::now();


	for(size_t j=0; j< RUNS; ++j){
	    u8 *data_ptr = data_stream;

	    for (int i=0; i<PACKET_NUMBER; i++)
	    {
	        dvb_s2_encode_tp(m_dvbs2, data_ptr);
	        data_ptr += PACKET_SIZE;
	    }
	}

	chrono::steady_clock::time_point end= chrono::steady_clock::now();
	float elapsed_s = chrono::duration_cast<chrono::microseconds>(end - begin).count()/1000000.0;
	cout << "Elapsed = " << elapsed_s << "s" << endl;
	cout << "throughput = " << (PACKET_STREAM_SIZE*8.0*RUNS/elapsed_s/1024/1024) << "Mbps" << endl;

	delete	m_dvbs2;
    return 0;
}

