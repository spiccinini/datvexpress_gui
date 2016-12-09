#include "DVB2.h"

void DVB2::init_bb_scrambler(void)
{
    int sr = 0x4A80;
    for( int i = 0; i < FRAME_SIZE_NORMAL; i++ )
    {
        int b = ((sr)^(sr>>1))&1;
        m_bb_randomise[i] = b;
        sr >>= 1;
        if( b ) sr |= 0x4000;
    }
}

//
// Randomise the data bits
//
void DVB2::bb_scramble(DVB2 *encoder, DVB2FrameFormat *fmt, Bit* frame)
{
    uint32_t kbch = fmt->kbch;
    for(uint32_t i = 0; i < kbch; i++)
    {
        frame[i] ^= encoder->m_bb_randomise[i];
    }
}
