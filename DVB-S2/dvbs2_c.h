#ifndef DVBS2_H_INCLUDED
#define DVBS2_H_INCLUDED

#ifndef DVBS2_API
#   if (__GNUC__ >= 4) && !defined(DVBS2_PY)
#       define DVBS2_API __attribute__((visibility("default")))
#   else
#       define DVBS2_API
#   endif
#endif

#ifdef __cplusplus
extern "C" {
#endif


struct dvbs2_s;
typedef struct dvbs2_s dvbs2_t;
struct dvbs2_config;
typedef struct dvbs2_config dvbs2_config_t;

DVBS2_API dvbs2_t *dvbs2_new();

DVBS2_API void dvbs2_free(dvbs2_t *encoder);

DVBS2_API int dvbs2_config(dvbs2_t *encoder, uint32_t code_rate, uint32_t constellation);

DVBS2_API int dvbs2_bch_encode(dvbs2_t *encoder, uint8_t * frame);

DVBS2_API void dvbs2_ldpc_encode(dvbs2_t *encoder, uint8_t * frame);

DVBS2_API void dvbs2_bb_scramble(dvbs2_t *encoder, uint8_t * frame);

DVBS2_API void dvbs2_add_bbheader(dvbs2_t *encoder, uint8_t * frame,
                                  uint32_t user_packet_remaining_bytes);

#ifdef __cplusplus
}
#endif
#endif
