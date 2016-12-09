#include <stdint.h>
#include "dvbs2_c.h"
#include "DVBS2.h"


#define AS_TYPE(Type, Obj) reinterpret_cast<Type *>(Obj)
#define AS_CTYPE(Type, Obj) reinterpret_cast<const Type *>(Obj)

dvbs2_t *dvbs2_new()
{
    return AS_TYPE(dvbs2_t, new DVBS2());
}

void dvbs2_free(dvbs2_t *encoder)
{
    if (!encoder)
        return;
    delete AS_TYPE(DVBS2, encoder);
}

int dvbs2_config(dvbs2_t *encoder, uint32_t code_rate, uint32_t constellation)
{
    if (!encoder)
        return -2;

    DVB2FrameFormat dvbs2_fmt;
    dvbs2_fmt.frame_type = FRAME_NORMAL;
    dvbs2_fmt.code_rate = code_rate;
    dvbs2_fmt.constellation = constellation;
    dvbs2_fmt.roll_off = RO_0_20;
    dvbs2_fmt.pilots = 1; // FIXME ¿?
    dvbs2_fmt.dummy_frame = 0;
    dvbs2_fmt.null_deletion = 0;

    return AS_TYPE(DVBS2, encoder)->s2_set_configure(&dvbs2_fmt);
}

void dvbs2_bb_scramble(dvbs2_t *encoder, uint8_t * frame) {
    DVB2FrameFormat fmt;
    AS_TYPE(DVBS2, encoder)->get_configure(&fmt);
    AS_TYPE(DVBS2, encoder)->bb_scramble(AS_TYPE(DVBS2, encoder), &fmt, frame);
}

int dvbs2_bch_encode(dvbs2_t *encoder, uint8_t * frame) {
    DVB2FrameFormat fmt;
    AS_TYPE(DVBS2, encoder)->get_configure(&fmt);
    return AS_TYPE(DVBS2, encoder)->bch_encode(&fmt, frame);
}

void dvbs2_ldpc_encode(dvbs2_t *encoder, uint8_t * frame) {
    DVB2FrameFormat fmt;
    AS_TYPE(DVBS2, encoder)->get_configure(&fmt);
    AS_TYPE(DVBS2, encoder)->ldpc_encode(AS_TYPE(DVBS2, encoder), &fmt, frame);
}

