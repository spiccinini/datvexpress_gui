#ifndef DVB2_H
#define DVB2_H
#include <queue>
#include <deque>
#include <list>
#include <stdio.h>
#include "dvb_types.h"
#include "DVB2.h"

typedef struct{
    int table_length;
    int d[LDPC_ENCODE_TABLE_LENGTH];
    int p[LDPC_ENCODE_TABLE_LENGTH];
}Ldpc_encode_table;

using namespace std;

typedef unsigned int u32;
typedef unsigned char u8;

#define CRC8_LENGTH_BITS 8

#define PADDING_LENGTH 200

class DVB2{

public:

protected:
    Bit m_frame[FRAME_SIZE_NORMAL];
    DVB2FrameFormat m_format[2];
    Bit m_padding[PADDING_LENGTH];
        int m_frame_offset_bits;
        int m_params_changed;

protected:
    int m_bbheader[BB_HEADER_LENGTH_BITS+CRC8_LENGTH_BITS];
    int m_bb_randomise[FRAME_SIZE_NORMAL];
    static u32 m_poly_n_8[4];
    static u32 m_poly_n_10[5];
    static u32 m_poly_n_12[6];
    static u32 m_poly_s_12[6];
    u8  m_crc_tab[256];
    u8  m_dnp; // Deleted null packet
    // Transport packet queue
    queue <u8> m_tp_q;

    // LDPC tables
    const static int ldpc_tab_1_4N[45][13];
    const static int ldpc_tab_1_3N[60][13];
    const static int ldpc_tab_2_5N[72][13];
    const static int ldpc_tab_1_2N[90][9];
    const static int ldpc_tab_3_5N[108][13];
    const static int ldpc_tab_2_3N[120][14];
    const static int ldpc_tab_3_4N[135][13];
    const static int ldpc_tab_4_5N[144][12];
    const static int ldpc_tab_5_6N[150][14];
    const static int ldpc_tab_8_9N[160][5];
    const static int ldpc_tab_9_10N[162][5];
    const static int ldpc_tab_1_4S[9][13];
    const static int ldpc_tab_1_3S[15][13];
    const static int ldpc_tab_2_5S[18][13];
    const static int ldpc_tab_1_2S[20][9];
    const static int ldpc_tab_3_5S[27][13];
    const static int ldpc_tab_2_3S[30][14];
    const static int ldpc_tab_3_4S[33][13];
    const static int ldpc_tab_4_5S[35][4];
    const static int ldpc_tab_5_6S[37][14];
    const static int ldpc_tab_8_9S[40][5];


    Ldpc_encode_table m_ldpc_encode;

    void init_scrambler(void);
    void init_bb_scrambler(void);
    void ldpc_lookup_generate(void);
    int add_transport_packet( u8 *pkt, Bit *b );
    void build_crc8_table( void );
    u8	 calc_crc8( u8 *b, int len );
    int add_crc8_bits( Bit *in, int length );
    void unpack_transport_packet_add_crc( u8 *ts );
    void add_bbheader( void );
    void poly_reverse( int *pin, int *pout, int len );
    void poly_pack( const int *pin, u32* pout, int len );
    int poly_mult( const int *ina, int lena, const int *inb, int lenb, int *out );
    void bch_poly_build_tables( void );
    static void reg_4_shift( u32 *sr );
    static void reg_5_shift( u32 *sr );
    static void reg_6_shift( u32 *sr );
    static uint32_t bch_n_8_encode(  Bit *in, uint32_t len );
    static uint32_t bch_n_10_encode( Bit *in, uint32_t len );
    static uint32_t bch_n_12_encode( Bit *in, uint32_t len );
    static uint32_t bch_s_12_encode( Bit *in, uint32_t len );

    int add_ts_frame_base( u8 *ts );
    void ldpc_encode_test();
    void base_end_of_frame_actions(void);
protected:
    int  set_configure( DVB2FrameFormat *f );

    int  next_ts_frame_base( u8 *ts );

public:
    DVB2();
    ~DVB2();

    void get_configure( DVB2FrameFormat *f );
    static int bch_encode(DVB2FrameFormat *fmt, Bit* frame);
    static void ldpc_encode(DVB2 *, DVB2FrameFormat *fmt, Bit* frame);
    static void bb_scramble(DVB2 *, DVB2FrameFormat *fmt, Bit* frame);
};

#endif
