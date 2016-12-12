// polymult.cpp : Defines the entry point for the console application.
//
#include "stdio.h"
#include "stdlib.h"
#include "memory.h"
//#include "dvb.h"
#include "DVBS2.h"


//
// Display routines.
//
void display_poly( int *in, int len )
{
        printf("\n");
	for( int i = 0; i < len; i++ )
	{
		if(in[i] == 1 )
		{
			if( i == 0 )
                                printf("1");
			else
				if( i == 1 )
                                        printf("+x");
				else
                                        printf("+x^%d",i);
		}
	}
        printf("\n");
}
//
// length is in bits
//
void display_poly_pack( unsigned int *in, int len )
{

        printf("\n");
	for( int i = 0; i < len/32; i++ )
	{
                printf("%.8X",in[i]);
	}
	switch((len%32)/8)
	{
	case 0:
		break;
	case 1:
                printf("%.2X",in[(len/32)]>>24);
		break;
	case 2:
                printf("%.2X",in[(len/32)]>>24);
                printf("%.2X",in[(len/32)]>>16);
		break;
	case 3:
                printf("%.2X",in[(len/32)]>>24);
                printf("%.2X",in[(len/32)]>>16);
                printf("%.2X",in[(len/32)]>>8);
		break;
	}
        printf("\n");
}

//
// Polynomial calculation routines
//
// multiply polynomials
//
int DVB2::poly_mult( const int *ina, int lena, const int *inb, int lenb, int *out )
{
    memset( out, 0, sizeof(int)*(lena+lenb));

    for( int i = 0; i < lena; i++ )
    {
        for( int j = 0; j < lenb; j++ )
        {
            if( ina[i]*inb[j] > 0 ) out[i+j]++;// count number of terms for this pwr of x
        }
    }
    int max=0;
    for( int i = 0; i < lena+lenb; i++ )
    {
        out[i] = out[i]&1;// If even ignore the term
        if(out[i]) max = i;
    }
    // return the size of array to house the result.
    return max+1;

}
//
// Pack the polynomial into a 32 bit array
//

void DVB2::poly_pack( const int *pin, u32* pout, int len )
{
    int lw = len/32;
    int ptr = 0;
    u32 temp;
    if( len % 32 ) lw++;

    for( int i = 0; i < lw; i++ )
    {
        temp    = 0x80000000;
        pout[i] = 0;
        for( int j = 0; j < 32; j++ )
        {
            if( pin[ptr++] ) pout[i] |= temp;
            temp >>= 1;
        }
    }
}

void DVB2::poly_reverse( int *pin, int *pout, int len )
{
    int c;
    c = len-1;

    for( int i = 0; i < len; i++ )
    {
        pout[c--] = pin[i];
    }
}
//
// Shift a 128 bit register
//
void  inline DVB2::reg_4_shift( u32 *sr )
{
    sr[3] = (sr[3]>>1) | (sr[2]<<31);
    sr[2] = (sr[2]>>1) | (sr[1]<<31);
    sr[1] = (sr[1]>>1) | (sr[0]<<31);
    sr[0] = (sr[0]>>1);
}
//
// Shift 160 bits
//
void  inline DVB2::reg_5_shift( u32 *sr )
{
    sr[4] = (sr[4]>>1) | (sr[3]<<31);
    sr[3] = (sr[3]>>1) | (sr[2]<<31);
    sr[2] = (sr[2]>>1) | (sr[1]<<31);
    sr[1] = (sr[1]>>1) | (sr[0]<<31);
    sr[0] = (sr[0]>>1);
}
//
// Shift 192 bits
//
void  inline DVB2::reg_6_shift( u32 *sr )
{
    sr[5] = (sr[5]>>1) | (sr[4]<<31);
    sr[4] = (sr[4]>>1) | (sr[3]<<31);
    sr[3] = (sr[3]>>1) | (sr[2]<<31);
    sr[2] = (sr[2]>>1) | (sr[1]<<31);
    sr[1] = (sr[1]>>1) | (sr[0]<<31);
    sr[0] = (sr[0]>>1);
}

//
// Take an bit array, bch encode it and place the result in a bit array
// The input length is in bits.
//
uint32_t DVB2::bch_n_8_encode( Bit *in, uint32_t len )
{
    Bit b;
    uint32_t i;
    u32 shift[4];

    //Zero the shift register
    memset( shift,0,sizeof(u32)*4);

    for( i = 0; i < len; i++ )
    {
        b =  in[i]^(shift[3]&1);
        reg_4_shift( shift );
        if( b )
        {
            shift[0] ^= m_poly_n_8[0];
            shift[1] ^= m_poly_n_8[1];
            shift[2] ^= m_poly_n_8[2];
            shift[3] ^= m_poly_n_8[3];
        }
    }
    // Now add the parity bits to the output
    for( int n = 0; n < 128; n++ )
    {
        in[i++] = shift[3]&1;
        reg_4_shift( shift );
    }
    return i;
}

uint32_t DVB2::bch_n_10_encode( Bit *in, uint32_t len )
{
    Bit b;
    uint32_t i;
    u32 shift[5];

    //Zero the shift register
    memset( shift,0,sizeof(u32)*5);

    for( i = 0; i < len; i++ )
    {
        b = in[i]^(shift[4]&1);
        reg_5_shift( shift );
        if(b)
        {
            shift[0] ^= m_poly_n_10[0];
            shift[1] ^= m_poly_n_10[1];
            shift[2] ^= m_poly_n_10[2];
            shift[3] ^= m_poly_n_10[3];
            shift[4] ^= m_poly_n_10[4];
        }
    }
    // Now add the parity bits to the output
    for( int n = 0; n < 160; n++ )
    {
        in[i++] = shift[4]&1;
        reg_5_shift( shift );
    }
    return i;
}

inline uint8_t ReverseBitsInByte(uint8_t v)
{
    return (v * 0x0202020202ULL & 0x010884422010ULL) % 1023;
}

inline uint8_t ReverseBits4ops64bit(uint8_t v)
{
    return ((v * 0x80200802ULL) & 0x0884422110ULL) * 0x0101010101ULL >> 32;
}

#define R2(n)     n,     n + 2*64,     n + 1*64,     n + 3*64
#define R4(n) R2(n), R2(n + 2*16), R2(n + 1*16), R2(n + 3*16)
#define R6(n) R4(n), R4(n + 2*4 ), R4(n + 1*4 ), R4(n + 3*4 )

static const unsigned char BitReverseTable256[256] =
{
    R6(0), R6(2), R6(1), R6(3)
};

inline uint8_t ReverseBitsLookupTable(uint8_t v)
{
    return BitReverseTable256[v];
}

inline uint8_t ReverseBits5logNOps(uint8_t v)
{
    v = ((v >> 1) & 0x55) | ((v & 0x55) << 1);
    v = ((v >> 2) & 0x33) | ((v & 0x33) << 2);
    v = ((v >> 4) & 0x0F) | ((v & 0x0F) << 4);
    return v;
}

inline uint32_t reverse(uint32_t x)
{
    x = ((x >> 1) & 0x55555555u) | ((x & 0x55555555u) << 1);
    x = ((x >> 2) & 0x33333333u) | ((x & 0x33333333u) << 2);
    x = ((x >> 4) & 0x0f0f0f0fu) | ((x & 0x0f0f0f0fu) << 4);
    x = ((x >> 8) & 0x00ff00ffu) | ((x & 0x00ff00ffu) << 8);
    x = ((x >> 16) & 0xffffu) | ((x & 0xffffu) << 16);
    return x;
}

uint32_t DVB2::bch_n_12_encode_packed( Bit *in, uint32_t len )
{
    Bit b, c;
    uint32_t i;
    u32 shift[6] = {0};

    // MSB of the codeword first
    for( i = 0; i < len; i++ )
    {
        for(int j=7; j >= 0; --j) {
            c = (in[i] >> j);
            b =  (c ^ shift[5]) & 1;
            reg_6_shift( shift );
            if(b)
            {
                shift[0] ^= m_poly_n_12[0];
                shift[1] ^= m_poly_n_12[1];
                shift[2] ^= m_poly_n_12[2];
                shift[3] ^= m_poly_n_12[3];
                shift[4] ^= m_poly_n_12[4];
                shift[5] ^= m_poly_n_12[5];
             }
        }
    }
    //printf("shift [%X, %X, %X, %X, %X, %X]\n", shift[0], shift[1], shift[2], shift[3], shift[4], shift[5]);
    // Now add the parity bits to the output

    for( int n = 0; n < (192/8/4); n++ )
    {
        uint32_t u = reverse(shift[5]);

        in[i++] = (u >> 24) & 0xff;
        in[i++] = (u >> 16) & 0xff;
        in[i++] = (u >> 8) & 0xff;
        in[i++] =  u & 0xff;

        for(uint8_t j =0; j <(8*4); ++j) {
            reg_6_shift( shift );
        }
    }

    return i;
}

uint32_t DVB2::bch_n_12_encode( Bit *in, uint32_t len )
{
    Bit b;
    uint32_t i;
    u32 shift[6] = {0};

    // MSB of the codeword first
    for( i = 0; i < len; i++ )
    {
        //if (i < 10) {printf("in %d", in[i]);}
        b =  in[i] ^ (shift[5]&1);
        reg_6_shift( shift );
        if(b)
        {
            shift[0] ^= m_poly_n_12[0];
            shift[1] ^= m_poly_n_12[1];
            shift[2] ^= m_poly_n_12[2];
            shift[3] ^= m_poly_n_12[3];
            shift[4] ^= m_poly_n_12[4];
            shift[5] ^= m_poly_n_12[5];
         }
    }
    // Now add the parity bits to the output
    for( int n = 0; n < 192; n++ )
    {
        in[i++] = shift[5]&1;
        reg_6_shift( shift );
    }
    return i;
}


uint32_t DVB2::bch_s_12_encode( Bit *in, uint32_t len )
{
    Bit b;
    uint32_t i;
    u32 shift[6];

    //Zero the shift register
    memset( shift,0,sizeof(u32)*6);

    for( i = 0; i < len; i++ )
    {
        b = (in[i] ^ ((shift[5]&0x01000000)?1:0));
        reg_6_shift( shift );
        if(b)
        {
            shift[0] ^= m_poly_s_12[0];
            shift[1] ^= m_poly_s_12[1];
            shift[2] ^= m_poly_s_12[2];
            shift[3] ^= m_poly_s_12[3];
            shift[4] ^= m_poly_s_12[4];
            shift[5] ^= m_poly_s_12[5];
        }
    }
    // Now add the parity bits to the output
    for( int n = 0; n < 168; n++ )
    {
        in[i++] = (shift[5]&0x01000000) ? 1:0;
        reg_6_shift( shift );
    }
    return i;
}


int DVB2::bch_encode(DVB2FrameFormat *fmt, Bit *frame, uint8_t packed)
{
    int res;
    int len = fmt->kbch;
    if (packed) {
        len /= 8;
    }

    switch(fmt->bch_code)
    {
    case BCH_CODE_N8:
        res = bch_n_8_encode( frame, len );
        break;
    case BCH_CODE_N10:
        res = bch_n_10_encode( frame, len );
        break;
    case BCH_CODE_N12:
        if(packed){
            res = bch_n_12_encode_packed( frame, len );
        } else {
            res = bch_n_12_encode( frame, len );
        }
        break;
    case BCH_CODE_S12:
        res = bch_s_12_encode( frame, len );
        break;
    default:
        printf("BCH error situation\n");
        res = 0;
        break;
    }
    return res;
}

//
//
//
void DVB2::bch_poly_build_tables( void )
{
    // Normal polynomials
    const int polyn01[]={1,0,1,1,0,1,0,0,0,0,0,0,0,0,0,0,1};
    const int polyn02[]={1,1,0,0,1,1,1,0,1,0,0,0,0,0,0,0,1};
    const int polyn03[]={1,0,1,1,1,1,0,1,1,1,1,1,0,0,0,0,1};
    const int polyn04[]={1,0,1,0,1,0,1,0,0,1,0,1,1,0,1,0,1};
    const int polyn05[]={1,1,1,1,0,1,0,0,1,1,1,1,1,0,0,0,1};
    const int polyn06[]={1,0,1,0,1,1,0,1,1,1,1,0,1,1,1,1,1};
    const int polyn07[]={1,0,1,0,0,1,1,0,1,1,1,1,0,1,0,1,1};
    const int polyn08[]={1,1,1,0,0,1,1,0,1,1,0,0,1,1,1,0,1};
    const int polyn09[]={1,0,0,0,0,1,0,1,0,1,1,1,0,0,0,0,1};
    const int polyn10[]={1,1,1,0,0,1,0,1,1,0,1,0,1,1,1,0,1};
    const int polyn11[]={1,0,1,1,0,1,0,0,0,1,0,1,1,1,0,0,1};
    const int polyn12[]={1,1,0,0,0,1,1,1,0,1,0,1,1,0,0,0,1};

    // Short polynomials
    const int polys01[]={1,1,0,1,0,1,0,0,0,0,0,0,0,0,1};
    const int polys02[]={1,0,0,0,0,0,1,0,1,0,0,1,0,0,1};
    const int polys03[]={1,1,1,0,0,0,1,0,0,1,1,0,0,0,1};
    const int polys04[]={1,0,0,0,1,0,0,1,1,0,1,0,1,0,1};
    const int polys05[]={1,0,1,0,1,0,1,0,1,1,0,1,0,1,1};
    const int polys06[]={1,0,0,1,0,0,0,1,1,1,0,0,0,1,1};
    const int polys07[]={1,0,1,0,0,1,1,1,0,0,1,1,0,1,1};
    const int polys08[]={1,0,0,0,0,1,0,0,1,1,1,1,0,0,1};
    const int polys09[]={1,1,1,1,0,0,0,0,0,1,1,0,0,0,1};
    const int polys10[]={1,0,0,1,0,0,1,0,0,1,0,1,1,0,1};
    const int polys11[]={1,0,0,0,1,0,0,0,0,0,0,1,1,0,1};
    const int polys12[]={1,1,1,1,0,1,1,1,1,0,1,0,0,1,1};

    int len;
    int polyout[3][2000];

    len = poly_mult( polyn01, 17, polyn02,    17,  polyout[0] );
    len = poly_mult( polyn03, 17, polyout[0], len, polyout[1] );
    len = poly_mult( polyn04, 17, polyout[1], len, polyout[0] );
    len = poly_mult( polyn05, 17, polyout[0], len, polyout[1] );
    len = poly_mult( polyn06, 17, polyout[1], len, polyout[0] );
    len = poly_mult( polyn07, 17, polyout[0], len, polyout[1] );
    len = poly_mult( polyn08, 17, polyout[1], len, polyout[0] );
    poly_pack( polyout[0], m_poly_n_8, 128 );

    len = poly_mult( polyn09, 17, polyout[0], len, polyout[1] );
    len = poly_mult( polyn10, 17, polyout[1], len, polyout[0] );
    poly_pack( polyout[0], m_poly_n_10, 160 );

    len = poly_mult( polyn11, 17, polyout[0], len, polyout[1] );
    len = poly_mult( polyn12, 17, polyout[1], len, polyout[0] );
    poly_pack( polyout[0], m_poly_n_12, 192 );

    len = poly_mult( polys01, 15, polys02,    15,  polyout[0] );
    len = poly_mult( polys03, 15, polyout[0], len, polyout[1] );
    len = poly_mult( polys04, 15, polyout[1], len, polyout[0] );
    len = poly_mult( polys05, 15, polyout[0], len, polyout[1] );
    len = poly_mult( polys06, 15, polyout[1], len, polyout[0] );
    len = poly_mult( polys07, 15, polyout[0], len, polyout[1] );
    len = poly_mult( polys08, 15, polyout[1], len, polyout[0] );
    len = poly_mult( polys09, 15, polyout[0], len, polyout[1] );
    len = poly_mult( polys10, 15, polyout[1], len, polyout[0] );
    len = poly_mult( polys11, 15, polyout[0], len, polyout[1] );
    len = poly_mult( polys12, 15, polyout[1], len, polyout[0] );
    poly_pack( polyout[0], m_poly_s_12, 168 );
}
