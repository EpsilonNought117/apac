#ifndef APAC_NTT_PRIMES_H
#define APAC_NTT_PRIMES_H

#include "../../header/apac_internal.h"

#ifdef APAC_64BIT_PLATFORM

typedef enum ntt_tf_t
{
    CYCLIC,
    NEGACYCLIC

} ntt_tf_t;

typedef struct ntt_prime_t
{
    apn_dig_t k, g, p;

    apn_dig_t magic, shift;

    double    prime_inv;
    apn_dig_t prime_inv52;
    apn_dig_t r0;

    apn_dig_t twiddle[43];
    apn_dig_t twiddle_inv[43];
    apn_dig_t size_inv[43];

    double cdltf64_cyclic_fwd[60];
    double cdltf64_cyclic_inv[60];
    double cdltf64_negacyclic_fwd[60];
    double cdltf64_negacyclic_inv[60];

    apn_dig_t cdltu64_cyclic_fwd[60];
    apn_dig_t cdltu64_cyclic_inv[60];
    apn_dig_t cdltu64_negacyclic_fwd[60];
    apn_dig_t cdltu64_negacyclic_inv[60];

} ntt_prime_t;

static const ntt_prime_t NTT_PRIMES[4] =
{
    {

    /* --------------------------------------------------------------- */
    /* k = 11     g = 3    p = 96757023244289     (0x580000000001)     */
    /* --------------------------------------------------------------- */
        
        .k = 11ULL, .g = 3ULL, .p = (11ULL << 43ULL) + 1ULL,

        .magic = 0xBA2E8BA2E8B810EDULL,

        .shift = 46ULL,

        .prime_inv   = 0X1.745D1745D1702P-47,
        .prime_inv52 = 0x057FFFFFFFFFFULL,
        .r0          = 0x03DD1745D1FBDULL,

        .twiddle =
        {
            0x000000002B3FBULL /* w^(2^0 ) */, 0x000074E74F819ULL /* w^(2^1 ) */, 0x045FF027D2586ULL /* w^(2^2 ) */, 0x0470ACA066C22ULL /* w^(2^3 ) */,
            0x0281CC2DC11DEULL /* w^(2^4 ) */, 0x0468A7443AAEFULL /* w^(2^5 ) */, 0x009EBD9293615ULL /* w^(2^6 ) */, 0x037C19F45F5EFULL /* w^(2^7 ) */,
            0x043A7AFFD65D8ULL /* w^(2^8 ) */, 0x01FDC45920C5DULL /* w^(2^9 ) */, 0x0204B4164114AULL /* w^(2^10) */, 0x00D6732B3F034ULL /* w^(2^11) */,
            0x0001B80A4D86FULL /* w^(2^12) */, 0x029FEFC9B1919ULL /* w^(2^13) */, 0x032F1F36AACF9ULL /* w^(2^14) */, 0x0342823F6CA93ULL /* w^(2^15) */,
            0x0405473A32C23ULL /* w^(2^16) */, 0x04999E37AA009ULL /* w^(2^17) */, 0x039BFF931C632ULL /* w^(2^18) */, 0x051F0C5840D1DULL /* w^(2^19) */,
            0x00C5FCD1E2300ULL /* w^(2^20) */, 0x037DCC71868BDULL /* w^(2^21) */, 0x04C8B81865C21ULL /* w^(2^22) */, 0x0509F18D1BBD9ULL /* w^(2^23) */,
            0x04CBE5F949A0DULL /* w^(2^24) */, 0x018F6C9868E2AULL /* w^(2^25) */, 0x04B9A2372E503ULL /* w^(2^26) */, 0x016D82838F173ULL /* w^(2^27) */,
            0x008404EF92E49ULL /* w^(2^28) */, 0x006797F915144ULL /* w^(2^29) */, 0x052ECAD80C09EULL /* w^(2^30) */, 0x01A426F65206FULL /* w^(2^31) */,
            0x03778DE145B2DULL /* w^(2^32) */, 0x044F8AFE4CEE4ULL /* w^(2^33) */, 0x02B0E2DFA121BULL /* w^(2^34) */, 0x036881586841DULL /* w^(2^35) */,
            0x0496096398CD6ULL /* w^(2^36) */, 0x0554B9812E10BULL /* w^(2^37) */, 0x02C670334B6F0ULL /* w^(2^38) */, 0x025DCF86F5D18ULL /* w^(2^39) */,
            0x02DC5D54156E1ULL /* w^(2^40) */, 0x053BAABFB63EFULL /* w^(2^41) */, 0x0580000000000ULL /* w^(2^42) */
        },

        .twiddle_inv =
        {
            0x0101A61DAF913ULL /* w_inv^(2^0 ) */, 0x028F732A8175CULL /* w_inv^(2^1 ) */, 0x04FF49F29759DULL /* w_inv^(2^2 ) */, 0x02F72686054FAULL /* w_inv^(2^3 ) */,
            0x01A5DFF1C4B42ULL /* w_inv^(2^4 ) */, 0x02F1B8D69A58CULL /* w_inv^(2^5 ) */, 0x00E45CA65FB4BULL /* w_inv^(2^6 ) */, 0x049500131C3ACULL /* w_inv^(2^7 ) */,
            0x013A044DE6240ULL /* w_inv^(2^8 ) */, 0x0490005431E49ULL /* w_inv^(2^9 ) */, 0x04E39236D49E6ULL /* w_inv^(2^10) */, 0x02BC08EECED42ULL /* w_inv^(2^11) */,
            0x03BBAC8E9B0FAULL /* w_inv^(2^12) */, 0x01097B331B1B5ULL /* w_inv^(2^13) */, 0x0488DB7112649ULL /* w_inv^(2^14) */, 0x04E7AF546771CULL /* w_inv^(2^15) */,
            0x03C136593602DULL /* w_inv^(2^16) */, 0x02465E81D6727ULL /* w_inv^(2^17) */, 0x01C0F3305B1A1ULL /* w_inv^(2^18) */, 0x054D228A08827ULL /* w_inv^(2^19) */,
            0x04D70F157ECBAULL /* w_inv^(2^20) */, 0x01C82BE050BE6ULL /* w_inv^(2^21) */, 0x0121E3001E1D9ULL /* w_inv^(2^22) */, 0x01BD5FB7E1ACEULL /* w_inv^(2^23) */,
            0x014AC4864D937ULL /* w_inv^(2^24) */, 0x04EF617A0214EULL /* w_inv^(2^25) */, 0x04D33CFC0DBD8ULL /* w_inv^(2^26) */, 0x0492706B88156ULL /* w_inv^(2^27) */,
            0x0426ED8829FB8ULL /* w_inv^(2^28) */, 0x018264A22C96DULL /* w_inv^(2^29) */, 0x056E88D400269ULL /* w_inv^(2^30) */, 0x04F244C9F0A80ULL /* w_inv^(2^31) */,
            0x00DDD2D11BF96ULL /* w_inv^(2^32) */, 0x03962958B48E1ULL /* w_inv^(2^33) */, 0x017765BA448CFULL /* w_inv^(2^34) */, 0x027C3218F9F20ULL /* w_inv^(2^35) */,
            0x0286208C62205ULL /* w_inv^(2^36) */, 0x014C4DC28352FULL /* w_inv^(2^37) */, 0x024CA9B192F79ULL /* w_inv^(2^38) */, 0x01D890EB4D876ULL /* w_inv^(2^39) */,
            0x01FD04E899DD1ULL /* w_inv^(2^40) */, 0x0044554049C12ULL /* w_inv^(2^41) */, 0x0580000000000ULL /* w_inv^(2^42) */
        },

        .size_inv =
        {
            0x0000000000001ULL /* N^(2^0 ) */, 0x02C0000000001ULL /* N^(2^1 ) */, 0x0420000000001ULL /* N^(2^2 ) */, 0x04D0000000001ULL /* N^(2^3 ) */,
            0x0528000000001ULL /* N^(2^4 ) */, 0x0554000000001ULL /* N^(2^5 ) */, 0x056A000000001ULL /* N^(2^6 ) */, 0x0575000000001ULL /* N^(2^7 ) */,
            0x057A800000001ULL /* N^(2^8 ) */, 0x057D400000001ULL /* N^(2^9 ) */, 0x057EA00000001ULL /* N^(2^10) */, 0x057F500000001ULL /* N^(2^11) */,
            0x057FA80000001ULL /* N^(2^12) */, 0x057FD40000001ULL /* N^(2^13) */, 0x057FEA0000001ULL /* N^(2^14) */, 0x057FF50000001ULL /* N^(2^15) */,
            0x057FFA8000001ULL /* N^(2^16) */, 0x057FFD4000001ULL /* N^(2^17) */, 0x057FFEA000001ULL /* N^(2^18) */, 0x057FFF5000001ULL /* N^(2^19) */,
            0x057FFFA800001ULL /* N^(2^20) */, 0x057FFFD400001ULL /* N^(2^21) */, 0x057FFFEA00001ULL /* N^(2^22) */, 0x057FFFF500001ULL /* N^(2^23) */,
            0x057FFFFA80001ULL /* N^(2^24) */, 0x057FFFFD40001ULL /* N^(2^25) */, 0x057FFFFEA0001ULL /* N^(2^26) */, 0x057FFFFF50001ULL /* N^(2^27) */,
            0x057FFFFFA8001ULL /* N^(2^28) */, 0x057FFFFFD4001ULL /* N^(2^29) */, 0x057FFFFFEA001ULL /* N^(2^30) */, 0x057FFFFFF5001ULL /* N^(2^31) */,
            0x057FFFFFFA801ULL /* N^(2^32) */, 0x057FFFFFFD401ULL /* N^(2^33) */, 0x057FFFFFFEA01ULL /* N^(2^34) */, 0x057FFFFFFF501ULL /* N^(2^35) */,
            0x057FFFFFFFA81ULL /* N^(2^36) */, 0x057FFFFFFFD41ULL /* N^(2^37) */, 0x057FFFFFFFEA1ULL /* N^(2^38) */, 0x057FFFFFFFF51ULL /* N^(2^39) */,
            0x057FFFFFFFFA9ULL /* N^(2^40) */, 0x057FFFFFFFFD5ULL /* N^(2^41) */, 0x057FFFFFFFFEBULL /* N^(2^42) */
        },

        .cdltf64_cyclic_fwd =
        {
                          0X1P+0 /* psi^(0 ) */, 0X1.552E604B842CP+46 /* psi^(1 ) */,  0X1.633819A5B78P+45 /* psi^(2 ) */, 0X1.EEC181AEFCF8P+45 /* psi^(3 ) */,
             0X1.2EE7C37AE8CP+45 /* psi^(4 ) */, 0X1.7A425C8D0938P+45 /* psi^(5 ) */,  0X1.82DDABC753CP+42 /* psi^(6 ) */,  0X1.67097E036D8P+41 /* psi^(7 ) */,
            0X1.6E2EAA0AB708P+45 /* psi^(8 ) */,  0X1.354CE4868DFP+45 /* psi^(9 ) */,  0X1.3734A8F3E27P+46 /* psi^(10) */, 0X1.A10C08A308B8P+45 /* psi^(11) */,
             0X1.72D9D80FFFEP+44 /* psi^(12) */,  0X1.0CF6E1D481AP+43 /* psi^(13) */,   0X1.5C59F9C07AP+42 /* psi^(14) */, 0X1.CA166D6A0498P+45 /* psi^(15) */,
                          0X1P+0 /* psi^(0 ) */,  0X1.633819A5B78P+45 /* psi^(2 ) */,  0X1.2EE7C37AE8CP+45 /* psi^(4 ) */,  0X1.82DDABC753CP+42 /* psi^(6 ) */,
            0X1.6E2EAA0AB708P+45 /* psi^(8 ) */,  0X1.3734A8F3E27P+46 /* psi^(10) */,  0X1.72D9D80FFFEP+44 /* psi^(12) */,   0X1.5C59F9C07AP+42 /* psi^(14) */,
            0X1.4EEAAFED8FBCP+46 /* psi^(16) */,  0X1.55AED3AFA17P+46 /* psi^(18) */,  0X1.E93E12840BDP+45 /* psi^(20) */,  0X1.484AA90CDCCP+44 /* psi^(22) */,
             0X1.C17D8BB3118P+45 /* psi^(24) */,  0X1.C84B626B8CBP+45 /* psi^(26) */, 0X1.D3B78A593C58P+45 /* psi^(28) */,  0X1.99AB2736844P+45 /* psi^(30) */,
                          0X1P+0 /* psi^(0 ) */, 0X1.EEC181AEFCF8P+45 /* psi^(3 ) */,  0X1.82DDABC753CP+42 /* psi^(6 ) */,  0X1.354CE4868DFP+45 /* psi^(9 ) */,
             0X1.72D9D80FFFEP+44 /* psi^(12) */, 0X1.CA166D6A0498P+45 /* psi^(15) */,  0X1.55AED3AFA17P+46 /* psi^(18) */,  0X1.CFE25E14018P+43 /* psi^(21) */,
             0X1.C17D8BB3118P+45 /* psi^(24) */,  0X1.1121005A3B6P+45 /* psi^(27) */,  0X1.99AB2736844P+45 /* psi^(30) */,   0X1.5A33F68F7BP+41 /* psi^(33) */,
            0X1.91183C851748P+45 /* psi^(36) */, 0X1.54C7B40FE498P+46 /* psi^(39) */,  0X1.465AB860ECAP+43 /* psi^(42) */,  0X1.3E6123C56FDP+46 /* psi^(45) */,
                          0X1P+0 /* psi^(0 ) */,  0X1.2EE7C37AE8CP+45 /* psi^(4 ) */, 0X1.6E2EAA0AB708P+45 /* psi^(8 ) */,  0X1.72D9D80FFFEP+44 /* psi^(12) */,
                          0X1P+0 /* psi^(0 ) */, 0X1.6E2EAA0AB708P+45 /* psi^(8 ) */, 0X1.4EEAAFED8FBCP+46 /* psi^(16) */,  0X1.C17D8BB3118P+45 /* psi^(24) */,
                          0X1P+0 /* psi^(0 ) */,  0X1.72D9D80FFFEP+44 /* psi^(12) */,  0X1.C17D8BB3118P+45 /* psi^(24) */, 0X1.91183C851748P+45 /* psi^(36) */
        },

        .cdltf64_cyclic_inv =
        {
                          0X1P+0 /* psi_inv^(0 ) */,  0X1.4C4DC28352FP+44 /* psi_inv^(1 ) */, 0X1.2654D8C97BC8P+45 /* psi_inv^(2 ) */,  0X1.7FF98F2D144P+45 /* psi_inv^(3 ) */,
             0X1.D890EB4D876P+44 /* psi_inv^(4 ) */, 0X1.AEDEFFA5C4A8P+45 /* psi_inv^(5 ) */,  0X1.EF693B28E6BP+44 /* psi_inv^(6 ) */, 0X1.43021CDC4D1CP+46 /* psi_inv^(7 ) */,
             0X1.FD04E899DD1P+44 /* psi_inv^(8 ) */, 0X1.048C1F56986CP+46 /* psi_inv^(9 ) */, 0X1.0DED55BCC8D4P+46 /* psi_inv^(10) */, 0X1.2603B43D7FD4P+46 /* psi_inv^(11) */,
             0X1.AD83DAF7E87P+44 /* psi_inv^(12) */,   0X1.D34242902BP+41 /* psi_inv^(13) */,  0X1.4A258A0BD28P+41 /* psi_inv^(14) */, 0X1.47CA64C8AA68P+46 /* psi_inv^(15) */,
                          0X1P+0 /* psi_inv^(0 ) */, 0X1.2654D8C97BC8P+45 /* psi_inv^(2 ) */,  0X1.D890EB4D876P+44 /* psi_inv^(4 ) */,  0X1.EF693B28E6BP+44 /* psi_inv^(6 ) */,
             0X1.FD04E899DD1P+44 /* psi_inv^(8 ) */, 0X1.0DED55BCC8D4P+46 /* psi_inv^(10) */,  0X1.AD83DAF7E87P+44 /* psi_inv^(12) */,  0X1.4A258A0BD28P+41 /* psi_inv^(14) */,
             0X1.11550127048P+42 /* psi_inv^(16) */, 0X1.4A3A6063F864P+46 /* psi_inv^(18) */, 0X1.034989FC000CP+46 /* psi_inv^(20) */,  0X1.465AB860ECAP+43 /* psi_inv^(22) */,
              0X1.51D155F549P+45 /* psi_inv^(24) */, 0X1.47D225438AC8P+46 /* psi_inv^(26) */, 0X1.91183C851748P+45 /* psi_inv^(28) */, 0X1.5CC7E65A4888P+45 /* psi_inv^(30) */,
                          0X1P+0 /* psi_inv^(0 ) */,  0X1.7FF98F2D144P+45 /* psi_inv^(3 ) */,  0X1.EF693B28E6BP+44 /* psi_inv^(6 ) */, 0X1.048C1F56986CP+46 /* psi_inv^(9 ) */,
             0X1.AD83DAF7E87P+44 /* psi_inv^(12) */, 0X1.47CA64C8AA68P+46 /* psi_inv^(15) */, 0X1.4A3A6063F864P+46 /* psi_inv^(18) */,  0X1.1EF3F75CF75P+45 /* psi_inv^(21) */,
              0X1.51D155F549P+45 /* psi_inv^(24) */,  0X1.45BDA372F6DP+45 /* psi_inv^(27) */, 0X1.5CC7E65A4888P+45 /* psi_inv^(30) */, 0X1.0CEC8F5F2B48P+46 /* psi_inv^(33) */,
            0X1.D3B78A593C58P+45 /* psi_inv^(36) */,  0X1.CFDE323B2E8P+42 /* psi_inv^(39) */,  0X1.484AA90CDCCP+44 /* psi_inv^(42) */, 0X1.5165EDEB7EACP+46 /* psi_inv^(45) */,
                          0X1P+0 /* psi_inv^(0 ) */,  0X1.D890EB4D876P+44 /* psi_inv^(4 ) */,  0X1.FD04E899DD1P+44 /* psi_inv^(8 ) */,  0X1.AD83DAF7E87P+44 /* psi_inv^(12) */,
                          0X1P+0 /* psi_inv^(0 ) */,  0X1.FD04E899DD1P+44 /* psi_inv^(8 ) */,  0X1.11550127048P+42 /* psi_inv^(16) */,   0X1.51D155F549P+45 /* psi_inv^(24) */,
                          0X1P+0 /* psi_inv^(0 ) */,  0X1.AD83DAF7E87P+44 /* psi_inv^(12) */,   0X1.51D155F549P+45 /* psi_inv^(24) */, 0X1.D3B78A593C58P+45 /* psi_inv^(36) */
        },

        .cdltf64_negacyclic_fwd =
        {
                          0X1P+0 /* psi^(0 ) */, 0X1.258258E63358P+46 /* psi^(1 ) */, 0X1.552E604B842CP+46 /* psi^(2 ) */, 0X1.21B30F930B7CP+46 /* psi^(3 ) */,
             0X1.633819A5B78P+45 /* psi^(4 ) */,  0X1.2C4AD41C64CP+44 /* psi^(5 ) */, 0X1.EEC181AEFCF8P+45 /* psi^(6 ) */,   0X1.2A5197ED75P+40 /* psi^(7 ) */,
             0X1.2EE7C37AE8CP+45 /* psi^(8 ) */, 0X1.CAD068E10718P+45 /* psi^(9 ) */, 0X1.7A425C8D0938P+45 /* psi^(10) */, 0X1.173E58074DE4P+46 /* psi^(11) */,
             0X1.82DDABC753CP+42 /* psi^(12) */,  0X1.51E3BC56AD4P+45 /* psi^(13) */,  0X1.67097E036D8P+41 /* psi^(14) */,  0X1.0E4328D507CP+46 /* psi^(15) */,
                          0X1P+0 /* psi^(0 ) */, 0X1.552E604B842CP+46 /* psi^(2 ) */,  0X1.633819A5B78P+45 /* psi^(4 ) */, 0X1.EEC181AEFCF8P+45 /* psi^(6 ) */,
             0X1.2EE7C37AE8CP+45 /* psi^(8 ) */, 0X1.7A425C8D0938P+45 /* psi^(10) */,  0X1.82DDABC753CP+42 /* psi^(12) */,  0X1.67097E036D8P+41 /* psi^(14) */,
            0X1.6E2EAA0AB708P+45 /* psi^(16) */,  0X1.354CE4868DFP+45 /* psi^(18) */,  0X1.3734A8F3E27P+46 /* psi^(20) */, 0X1.A10C08A308B8P+45 /* psi^(22) */,
             0X1.72D9D80FFFEP+44 /* psi^(24) */,  0X1.0CF6E1D481AP+43 /* psi^(26) */,   0X1.5C59F9C07AP+42 /* psi^(28) */, 0X1.CA166D6A0498P+45 /* psi^(30) */,
                          0X1P+0 /* psi^(0 ) */, 0X1.21B30F930B7CP+46 /* psi^(3 ) */, 0X1.EEC181AEFCF8P+45 /* psi^(6 ) */, 0X1.CAD068E10718P+45 /* psi^(9 ) */,
             0X1.82DDABC753CP+42 /* psi^(12) */,  0X1.0E4328D507CP+46 /* psi^(15) */,  0X1.354CE4868DFP+45 /* psi^(18) */,  0X1.6F3A6E421FAP+43 /* psi^(21) */,
             0X1.72D9D80FFFEP+44 /* psi^(24) */, 0X1.203C048DECF8P+46 /* psi^(27) */, 0X1.CA166D6A0498P+45 /* psi^(30) */, 0X1.F37CCF9CBBF8P+45 /* psi^(33) */,
             0X1.55AED3AFA17P+46 /* psi^(36) */,  0X1.4890F27E41DP+44 /* psi^(39) */,  0X1.CFE25E14018P+43 /* psi^(42) */,  0X1.E65477EF086P+44 /* psi^(45) */,
                          0X1P+0 /* psi^(0 ) */,  0X1.633819A5B78P+45 /* psi^(4 ) */,  0X1.2EE7C37AE8CP+45 /* psi^(8 ) */,  0X1.82DDABC753CP+42 /* psi^(12) */,
                          0X1P+0 /* psi^(0 ) */,  0X1.2EE7C37AE8CP+45 /* psi^(8 ) */, 0X1.6E2EAA0AB708P+45 /* psi^(16) */,  0X1.72D9D80FFFEP+44 /* psi^(24) */,
                          0X1P+0 /* psi^(0 ) */,  0X1.82DDABC753CP+42 /* psi^(12) */,  0X1.72D9D80FFFEP+44 /* psi^(24) */,  0X1.55AED3AFA17P+46 /* psi^(36) */
        },

        .cdltf64_negacyclic_inv =
        {
                          0X1P+0 /* psi_inv^(0 ) */, 0X1.431046311028P+45 /* psi_inv^(1 ) */,  0X1.4C4DC28352FP+44 /* psi_inv^(2 ) */, 0X1.213967ACAFF8P+46 /* psi_inv^(3 ) */,
            0X1.2654D8C97BC8P+45 /* psi_inv^(4 ) */, 0X1.17A59CB03528P+45 /* psi_inv^(5 ) */,  0X1.7FF98F2D144P+45 /* psi_inv^(6 ) */, 0X1.1AAEFA9B1CB8P+45 /* psi_inv^(7 ) */,
             0X1.D890EB4D876P+44 /* psi_inv^(8 ) */, 0X1.AF15518D10E8P+45 /* psi_inv^(9 ) */, 0X1.AEDEFFA5C4A8P+45 /* psi_inv^(10) */, 0X1.39A050F0E828P+46 /* psi_inv^(11) */,
             0X1.EF693B28E6BP+44 /* psi_inv^(12) */,  0X1.46F271B9F7DP+46 /* psi_inv^(13) */, 0X1.43021CDC4D1CP+46 /* psi_inv^(14) */, 0X1.5C2A1511AE64P+46 /* psi_inv^(15) */,
                          0X1P+0 /* psi_inv^(0 ) */,  0X1.4C4DC28352FP+44 /* psi_inv^(2 ) */, 0X1.2654D8C97BC8P+45 /* psi_inv^(4 ) */,  0X1.7FF98F2D144P+45 /* psi_inv^(6 ) */,
             0X1.D890EB4D876P+44 /* psi_inv^(8 ) */, 0X1.AEDEFFA5C4A8P+45 /* psi_inv^(10) */,  0X1.EF693B28E6BP+44 /* psi_inv^(12) */, 0X1.43021CDC4D1CP+46 /* psi_inv^(14) */,
             0X1.FD04E899DD1P+44 /* psi_inv^(16) */, 0X1.048C1F56986CP+46 /* psi_inv^(18) */, 0X1.0DED55BCC8D4P+46 /* psi_inv^(20) */, 0X1.2603B43D7FD4P+46 /* psi_inv^(22) */,
             0X1.AD83DAF7E87P+44 /* psi_inv^(24) */,   0X1.D34242902BP+41 /* psi_inv^(26) */,  0X1.4A258A0BD28P+41 /* psi_inv^(28) */, 0X1.47CA64C8AA68P+46 /* psi_inv^(30) */,
                          0X1P+0 /* psi_inv^(0 ) */, 0X1.213967ACAFF8P+46 /* psi_inv^(3 ) */,  0X1.7FF98F2D144P+45 /* psi_inv^(6 ) */, 0X1.AF15518D10E8P+45 /* psi_inv^(9 ) */,
             0X1.EF693B28E6BP+44 /* psi_inv^(12) */, 0X1.5C2A1511AE64P+46 /* psi_inv^(15) */, 0X1.048C1F56986CP+46 /* psi_inv^(18) */, 0X1.8D329818C0F8P+45 /* psi_inv^(21) */,
             0X1.AD83DAF7E87P+44 /* psi_inv^(24) */,  0X1.5C4C79D7E04P+46 /* psi_inv^(27) */, 0X1.47CA64C8AA68P+46 /* psi_inv^(30) */,    0X1.F8B3465FCP+42 /* psi_inv^(33) */,
            0X1.4A3A6063F864P+46 /* psi_inv^(36) */, 0X1.105E6D4F0494P+46 /* psi_inv^(39) */,  0X1.1EF3F75CF75P+45 /* psi_inv^(42) */,  0X1.01C364762E5P+46 /* psi_inv^(45) */,
                          0X1P+0 /* psi_inv^(0 ) */, 0X1.2654D8C97BC8P+45 /* psi_inv^(4 ) */,  0X1.D890EB4D876P+44 /* psi_inv^(8 ) */,  0X1.EF693B28E6BP+44 /* psi_inv^(12) */,
                          0X1P+0 /* psi_inv^(0 ) */,  0X1.D890EB4D876P+44 /* psi_inv^(8 ) */,  0X1.FD04E899DD1P+44 /* psi_inv^(16) */,  0X1.AD83DAF7E87P+44 /* psi_inv^(24) */,
                          0X1P+0 /* psi_inv^(0 ) */,  0X1.EF693B28E6BP+44 /* psi_inv^(12) */,  0X1.AD83DAF7E87P+44 /* psi_inv^(24) */, 0X1.4A3A6063F864P+46 /* psi_inv^(36) */
        },

        .cdltu64_cyclic_fwd =
        {
            0x0000000000001ULL /* psi^(0 ) */, 0x0554B9812E10BULL /* psi^(1 ) */, 0x02C670334B6F0ULL /* psi^(2 ) */, 0x03DD83035DF9FULL /* psi^(3 ) */,
            0x025DCF86F5D18ULL /* psi^(4 ) */, 0x02F484B91A127ULL /* psi^(5 ) */, 0x0060B76AF1D4FULL /* psi^(6 ) */, 0x002CE12FC06DBULL /* psi^(7 ) */,
            0x02DC5D54156E1ULL /* psi^(8 ) */, 0x026A99C90D1BEULL /* psi^(9 ) */, 0x04DCD2A3CF89CULL /* psi^(10) */, 0x0342181146117ULL /* psi^(11) */,
            0x0172D9D80FFFEULL /* psi^(12) */, 0x00867B70EA40DULL /* psi^(13) */, 0x0057167E701E8ULL /* psi^(14) */, 0x03942CDAD4093ULL /* psi^(15) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x02C670334B6F0ULL /* psi^(2 ) */, 0x025DCF86F5D18ULL /* psi^(4 ) */, 0x0060B76AF1D4FULL /* psi^(6 ) */,
            0x02DC5D54156E1ULL /* psi^(8 ) */, 0x04DCD2A3CF89CULL /* psi^(10) */, 0x0172D9D80FFFEULL /* psi^(12) */, 0x0057167E701E8ULL /* psi^(14) */,
            0x053BAABFB63EFULL /* psi^(16) */, 0x0556BB4EBE85CULL /* psi^(18) */, 0x03D27C250817AULL /* psi^(20) */, 0x01484AA90CDCCULL /* psi^(22) */,
            0x0382FB1766230ULL /* psi^(24) */, 0x039096C4D7196ULL /* psi^(26) */, 0x03A76F14B278BULL /* psi^(28) */, 0x0333564E6D088ULL /* psi^(30) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x03DD83035DF9FULL /* psi^(3 ) */, 0x0060B76AF1D4FULL /* psi^(6 ) */, 0x026A99C90D1BEULL /* psi^(9 ) */,
            0x0172D9D80FFFEULL /* psi^(12) */, 0x03942CDAD4093ULL /* psi^(15) */, 0x0556BB4EBE85CULL /* psi^(18) */, 0x00E7F12F0A00CULL /* psi^(21) */,
            0x0382FB1766230ULL /* psi^(24) */, 0x02224200B476CULL /* psi^(27) */, 0x0333564E6D088ULL /* psi^(30) */, 0x002B467ED1EF6ULL /* psi^(33) */,
            0x032230790A2E9ULL /* psi^(36) */, 0x05531ED03F926ULL /* psi^(39) */, 0x00A32D5C30765ULL /* psi^(42) */, 0x04F9848F15BF4ULL /* psi^(45) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x025DCF86F5D18ULL /* psi^(4 ) */, 0x02DC5D54156E1ULL /* psi^(8 ) */, 0x0172D9D80FFFEULL /* psi^(12) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x02DC5D54156E1ULL /* psi^(8 ) */, 0x053BAABFB63EFULL /* psi^(16) */, 0x0382FB1766230ULL /* psi^(24) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x0172D9D80FFFEULL /* psi^(12) */, 0x0382FB1766230ULL /* psi^(24) */, 0x032230790A2E9ULL /* psi^(36) */
        },

        .cdltu64_cyclic_inv =
        {
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x014C4DC28352FULL /* psi_inv^(1 ) */, 0x024CA9B192F79ULL /* psi_inv^(2 ) */, 0x02FFF31E5A288ULL /* psi_inv^(3 ) */,
            0x01D890EB4D876ULL /* psi_inv^(4 ) */, 0x035DBDFF4B895ULL /* psi_inv^(5 ) */, 0x01EF693B28E6BULL /* psi_inv^(6 ) */, 0x050C087371347ULL /* psi_inv^(7 ) */,
            0x01FD04E899DD1ULL /* psi_inv^(8 ) */, 0x0412307D5A61BULL /* psi_inv^(9 ) */, 0x0437B556F3235ULL /* psi_inv^(10) */, 0x04980ED0F5FF5ULL /* psi_inv^(11) */,
            0x01AD83DAF7E87ULL /* psi_inv^(12) */, 0x003A684852056ULL /* psi_inv^(13) */, 0x002944B1417A5ULL /* psi_inv^(14) */, 0x051F299322A9AULL /* psi_inv^(15) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x024CA9B192F79ULL /* psi_inv^(2 ) */, 0x01D890EB4D876ULL /* psi_inv^(4 ) */, 0x01EF693B28E6BULL /* psi_inv^(6 ) */,
            0x01FD04E899DD1ULL /* psi_inv^(8 ) */, 0x0437B556F3235ULL /* psi_inv^(10) */, 0x01AD83DAF7E87ULL /* psi_inv^(12) */, 0x002944B1417A5ULL /* psi_inv^(14) */,
            0x0044554049C12ULL /* psi_inv^(16) */, 0x0528E9818FE19ULL /* psi_inv^(18) */, 0x040D2627F0003ULL /* psi_inv^(20) */, 0x00A32D5C30765ULL /* psi_inv^(22) */,
            0x02A3A2ABEA920ULL /* psi_inv^(24) */, 0x051F48950E2B2ULL /* psi_inv^(26) */, 0x032230790A2E9ULL /* psi_inv^(28) */, 0x02B98FCCB4911ULL /* psi_inv^(30) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x02FFF31E5A288ULL /* psi_inv^(3 ) */, 0x01EF693B28E6BULL /* psi_inv^(6 ) */, 0x0412307D5A61BULL /* psi_inv^(9 ) */,
            0x01AD83DAF7E87ULL /* psi_inv^(12) */, 0x051F299322A9AULL /* psi_inv^(15) */, 0x0528E9818FE19ULL /* psi_inv^(18) */, 0x023DE7EEB9EEAULL /* psi_inv^(21) */,
            0x02A3A2ABEA920ULL /* psi_inv^(24) */, 0x028B7B46E5EDAULL /* psi_inv^(27) */, 0x02B98FCCB4911ULL /* psi_inv^(30) */, 0x0433B23D7CAD2ULL /* psi_inv^(33) */,
            0x03A76F14B278BULL /* psi_inv^(36) */, 0x0073F78C8ECBAULL /* psi_inv^(39) */, 0x01484AA90CDCCULL /* psi_inv^(42) */, 0x054597B7ADFABULL /* psi_inv^(45) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x01D890EB4D876ULL /* psi_inv^(4 ) */, 0x01FD04E899DD1ULL /* psi_inv^(8 ) */, 0x01AD83DAF7E87ULL /* psi_inv^(12) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x01FD04E899DD1ULL /* psi_inv^(8 ) */, 0x0044554049C12ULL /* psi_inv^(16) */, 0x02A3A2ABEA920ULL /* psi_inv^(24) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x01AD83DAF7E87ULL /* psi_inv^(12) */, 0x02A3A2ABEA920ULL /* psi_inv^(24) */, 0x03A76F14B278BULL /* psi_inv^(36) */
        },

        .cdltu64_negacyclic_fwd =
        {
            0x0000000000001ULL /* psi^(0 ) */, 0x0496096398CD6ULL /* psi^(1 ) */, 0x0554B9812E10BULL /* psi^(2 ) */, 0x0486CC3E4C2DFULL /* psi^(3 ) */,
            0x02C670334B6F0ULL /* psi^(4 ) */, 0x012C4AD41C64CULL /* psi^(5 ) */, 0x03DD83035DF9FULL /* psi^(6 ) */, 0x0012A5197ED75ULL /* psi^(7 ) */,
            0x025DCF86F5D18ULL /* psi^(8 ) */, 0x0395A0D1C20E3ULL /* psi^(9 ) */, 0x02F484B91A127ULL /* psi^(10) */, 0x045CF9601D379ULL /* psi^(11) */,
            0x0060B76AF1D4FULL /* psi^(12) */, 0x02A3C778AD5A8ULL /* psi^(13) */, 0x002CE12FC06DBULL /* psi^(14) */, 0x04390CA3541F0ULL /* psi^(15) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x0554B9812E10BULL /* psi^(2 ) */, 0x02C670334B6F0ULL /* psi^(4 ) */, 0x03DD83035DF9FULL /* psi^(6 ) */,
            0x025DCF86F5D18ULL /* psi^(8 ) */, 0x02F484B91A127ULL /* psi^(10) */, 0x0060B76AF1D4FULL /* psi^(12) */, 0x002CE12FC06DBULL /* psi^(14) */,
            0x02DC5D54156E1ULL /* psi^(16) */, 0x026A99C90D1BEULL /* psi^(18) */, 0x04DCD2A3CF89CULL /* psi^(20) */, 0x0342181146117ULL /* psi^(22) */,
            0x0172D9D80FFFEULL /* psi^(24) */, 0x00867B70EA40DULL /* psi^(26) */, 0x0057167E701E8ULL /* psi^(28) */, 0x03942CDAD4093ULL /* psi^(30) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x0486CC3E4C2DFULL /* psi^(3 ) */, 0x03DD83035DF9FULL /* psi^(6 ) */, 0x0395A0D1C20E3ULL /* psi^(9 ) */,
            0x0060B76AF1D4FULL /* psi^(12) */, 0x04390CA3541F0ULL /* psi^(15) */, 0x026A99C90D1BEULL /* psi^(18) */, 0x00B79D37210FDULL /* psi^(21) */,
            0x0172D9D80FFFEULL /* psi^(24) */, 0x0480F01237B3EULL /* psi^(27) */, 0x03942CDAD4093ULL /* psi^(30) */, 0x03E6F99F3977FULL /* psi^(33) */,
            0x0556BB4EBE85CULL /* psi^(36) */, 0x014890F27E41DULL /* psi^(39) */, 0x00E7F12F0A00CULL /* psi^(42) */, 0x01E65477EF086ULL /* psi^(45) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x02C670334B6F0ULL /* psi^(4 ) */, 0x025DCF86F5D18ULL /* psi^(8 ) */, 0x0060B76AF1D4FULL /* psi^(12) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x025DCF86F5D18ULL /* psi^(8 ) */, 0x02DC5D54156E1ULL /* psi^(16) */, 0x0172D9D80FFFEULL /* psi^(24) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x0060B76AF1D4FULL /* psi^(12) */, 0x0172D9D80FFFEULL /* psi^(24) */, 0x0556BB4EBE85CULL /* psi^(36) */
        },

        .cdltu64_negacyclic_inv =
        {
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x0286208C62205ULL /* psi_inv^(1 ) */, 0x014C4DC28352FULL /* psi_inv^(2 ) */, 0x0484E59EB2BFEULL /* psi_inv^(3 ) */,
            0x024CA9B192F79ULL /* psi_inv^(4 ) */, 0x022F4B39606A5ULL /* psi_inv^(5 ) */, 0x02FFF31E5A288ULL /* psi_inv^(6 ) */, 0x02355DF536397ULL /* psi_inv^(7 ) */,
            0x01D890EB4D876ULL /* psi_inv^(8 ) */, 0x035E2AA31A21DULL /* psi_inv^(9 ) */, 0x035DBDFF4B895ULL /* psi_inv^(10) */, 0x04E68143C3A0AULL /* psi_inv^(11) */,
            0x01EF693B28E6BULL /* psi_inv^(12) */, 0x051BC9C6E7DF4ULL /* psi_inv^(13) */, 0x050C087371347ULL /* psi_inv^(14) */, 0x0570A85446B99ULL /* psi_inv^(15) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x014C4DC28352FULL /* psi_inv^(2 ) */, 0x024CA9B192F79ULL /* psi_inv^(4 ) */, 0x02FFF31E5A288ULL /* psi_inv^(6 ) */,
            0x01D890EB4D876ULL /* psi_inv^(8 ) */, 0x035DBDFF4B895ULL /* psi_inv^(10) */, 0x01EF693B28E6BULL /* psi_inv^(12) */, 0x050C087371347ULL /* psi_inv^(14) */,
            0x01FD04E899DD1ULL /* psi_inv^(16) */, 0x0412307D5A61BULL /* psi_inv^(18) */, 0x0437B556F3235ULL /* psi_inv^(20) */, 0x04980ED0F5FF5ULL /* psi_inv^(22) */,
            0x01AD83DAF7E87ULL /* psi_inv^(24) */, 0x003A684852056ULL /* psi_inv^(26) */, 0x002944B1417A5ULL /* psi_inv^(28) */, 0x051F299322A9AULL /* psi_inv^(30) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x0484E59EB2BFEULL /* psi_inv^(3 ) */, 0x02FFF31E5A288ULL /* psi_inv^(6 ) */, 0x035E2AA31A21DULL /* psi_inv^(9 ) */,
            0x01EF693B28E6BULL /* psi_inv^(12) */, 0x0570A85446B99ULL /* psi_inv^(15) */, 0x0412307D5A61BULL /* psi_inv^(18) */, 0x031A65303181FULL /* psi_inv^(21) */,
            0x01AD83DAF7E87ULL /* psi_inv^(24) */, 0x057131E75F810ULL /* psi_inv^(27) */, 0x051F299322A9AULL /* psi_inv^(30) */, 0x007E2CD197F00ULL /* psi_inv^(33) */,
            0x0528E9818FE19ULL /* psi_inv^(36) */, 0x044179B53C125ULL /* psi_inv^(39) */, 0x023DE7EEB9EEAULL /* psi_inv^(42) */, 0x04070D91D8B94ULL /* psi_inv^(45) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x024CA9B192F79ULL /* psi_inv^(4 ) */, 0x01D890EB4D876ULL /* psi_inv^(8 ) */, 0x01EF693B28E6BULL /* psi_inv^(12) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x01D890EB4D876ULL /* psi_inv^(8 ) */, 0x01FD04E899DD1ULL /* psi_inv^(16) */, 0x01AD83DAF7E87ULL /* psi_inv^(24) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x01EF693B28E6BULL /* psi_inv^(12) */, 0x01AD83DAF7E87ULL /* psi_inv^(24) */, 0x0528E9818FE19ULL /* psi_inv^(36) */
        }
    },

    {

    /* --------------------------------------------------------------- */
    /* k = 29     g = 3    p = 255086697644033    (0xe80000000001)     */
    /* --------------------------------------------------------------- */
        
        .k = 29ULL, .g = 3ULL, .p = (29ULL << 43ULL) + 1ULL,

        .magic = 0x8D3DCB08D3DC14B3ULL,

        .shift = 47ULL,

        .prime_inv   = 0X1.1A7B9611A7B83P-48,
        .prime_inv52 = 0x0E7FFFFFFFFFFULL,
        .r0          = 0x05C69EE5847D7ULL,

        .twiddle =
        {
            0x03E6B41437D93ULL /* w^(2^0 ) */, 0x0ABA671CCBF3CULL /* w^(2^1 ) */, 0x0E7648BFFC2A7ULL /* w^(2^2 ) */, 0x0C295368A710CULL /* w^(2^3 ) */,
            0x0C753D6052146ULL /* w^(2^4 ) */, 0x057DDC0930919ULL /* w^(2^5 ) */, 0x05BFBCEA57632ULL /* w^(2^6 ) */, 0x03ABDC828E28BULL /* w^(2^7 ) */,
            0x086DF44963397ULL /* w^(2^8 ) */, 0x01A32725BEF23ULL /* w^(2^9 ) */, 0x06CB5C65AF0EAULL /* w^(2^10) */, 0x06CEBD4D6F547ULL /* w^(2^11) */,
            0x09FF375E3841DULL /* w^(2^12) */, 0x06FBD160CDFE6ULL /* w^(2^13) */, 0x056EC299F7F41ULL /* w^(2^14) */, 0x02D690315B3A6ULL /* w^(2^15) */,
            0x065557366C382ULL /* w^(2^16) */, 0x030F8601FAA1FULL /* w^(2^17) */, 0x0A53BBBB46C2FULL /* w^(2^18) */, 0x00B3092324D0CULL /* w^(2^19) */,
            0x03DB9B6304051ULL /* w^(2^20) */, 0x0DB312EE57CE4ULL /* w^(2^21) */, 0x04ECB3C13DFE6ULL /* w^(2^22) */, 0x09E80146D30EDULL /* w^(2^23) */,
            0x0BAEE84C7148FULL /* w^(2^24) */, 0x0074A216F576FULL /* w^(2^25) */, 0x0D2E8EB966DB3ULL /* w^(2^26) */, 0x00E7EB8C76996ULL /* w^(2^27) */,
            0x0D9DD1BA5A92CULL /* w^(2^28) */, 0x00644114E9B5DULL /* w^(2^29) */, 0x03E02124D0CCBULL /* w^(2^30) */, 0x073967B93BE43ULL /* w^(2^31) */,
            0x04D05C4C819B3ULL /* w^(2^32) */, 0x0512E159213ACULL /* w^(2^33) */, 0x037FCB934F884ULL /* w^(2^34) */, 0x0D081D9D5148DULL /* w^(2^35) */,
            0x088E7897DD639ULL /* w^(2^36) */, 0x00F382BB0866EULL /* w^(2^37) */, 0x022027A2D98FDULL /* w^(2^38) */, 0x068E7F865C2B0ULL /* w^(2^39) */,
            0x0B3D86C4AEAD3ULL /* w^(2^40) */, 0x026B44ADE5793ULL /* w^(2^41) */, 0x0E80000000000ULL /* w^(2^42) */
        },

        .twiddle_inv =
        {
            0x063D381748F5EULL /* w_inv^(2^0 ) */, 0x08B8ED0CAEA64ULL /* w_inv^(2^1 ) */, 0x020B6093548CBULL /* w_inv^(2^2 ) */, 0x0863CCA120369ULL /* w_inv^(2^3 ) */,
            0x02E3261983C16ULL /* w_inv^(2^4 ) */, 0x01F752934D3CEULL /* w_inv^(2^5 ) */, 0x0E0C6C2672C4AULL /* w_inv^(2^6 ) */, 0x09965BA5B3525ULL /* w_inv^(2^7 ) */,
            0x0257D9EDD67BCULL /* w_inv^(2^8 ) */, 0x0CB116024A744ULL /* w_inv^(2^9 ) */, 0x037FC0EDC80F7ULL /* w_inv^(2^10) */, 0x0B6CE6AAC0EFDULL /* w_inv^(2^11) */,
            0x03B2E75F6FFC9ULL /* w_inv^(2^12) */, 0x057418A904CE5ULL /* w_inv^(2^13) */, 0x0ADB64248820CULL /* w_inv^(2^14) */, 0x086961DF327B6ULL /* w_inv^(2^15) */,
            0x0292856E8EA55ULL /* w_inv^(2^16) */, 0x03E9D7E002F49ULL /* w_inv^(2^17) */, 0x0992FA4D21685ULL /* w_inv^(2^18) */, 0x0D2F3DC51466DULL /* w_inv^(2^19) */,
            0x011F671961A79ULL /* w_inv^(2^20) */, 0x01C89D157C623ULL /* w_inv^(2^21) */, 0x05590BA9A4DC7ULL /* w_inv^(2^22) */, 0x08C5805AF9A57ULL /* w_inv^(2^23) */,
            0x03B65D2B3C7BCULL /* w_inv^(2^24) */, 0x04D144B815969ULL /* w_inv^(2^25) */, 0x04C8AEE417A04ULL /* w_inv^(2^26) */, 0x052ADC28BD39EULL /* w_inv^(2^27) */,
            0x0AF3007BD50ABULL /* w_inv^(2^28) */, 0x05C3FF5E0C9EFULL /* w_inv^(2^29) */, 0x096D915A78346ULL /* w_inv^(2^30) */, 0x063BCECF658A1ULL /* w_inv^(2^31) */,
            0x0A80E8FBF0621ULL /* w_inv^(2^32) */, 0x0B142324A73F4ULL /* w_inv^(2^33) */, 0x075538AA5C334ULL /* w_inv^(2^34) */, 0x0022BCA4B9BF0ULL /* w_inv^(2^35) */,
            0x07B064A2247F6ULL /* w_inv^(2^36) */, 0x0707CAC72ED63ULL /* w_inv^(2^37) */, 0x0DBF305B1E380ULL /* w_inv^(2^38) */, 0x082F917DA68C0ULL /* w_inv^(2^39) */,
            0x0E4B897D26006ULL /* w_inv^(2^40) */, 0x0C14BB521A86EULL /* w_inv^(2^41) */, 0x0E80000000000ULL /* w_inv^(2^42) */
        },

        .size_inv =
        {
            0x0000000000001ULL /* N^(2^0 ) */, 0x0740000000001ULL /* N^(2^1 ) */, 0x0AE0000000001ULL /* N^(2^2 ) */, 0x0CB0000000001ULL /* N^(2^3 ) */,
            0x0D98000000001ULL /* N^(2^4 ) */, 0x0E0C000000001ULL /* N^(2^5 ) */, 0x0E46000000001ULL /* N^(2^6 ) */, 0x0E63000000001ULL /* N^(2^7 ) */,
            0x0E71800000001ULL /* N^(2^8 ) */, 0x0E78C00000001ULL /* N^(2^9 ) */, 0x0E7C600000001ULL /* N^(2^10) */, 0x0E7E300000001ULL /* N^(2^11) */,
            0x0E7F180000001ULL /* N^(2^12) */, 0x0E7F8C0000001ULL /* N^(2^13) */, 0x0E7FC60000001ULL /* N^(2^14) */, 0x0E7FE30000001ULL /* N^(2^15) */,
            0x0E7FF18000001ULL /* N^(2^16) */, 0x0E7FF8C000001ULL /* N^(2^17) */, 0x0E7FFC6000001ULL /* N^(2^18) */, 0x0E7FFE3000001ULL /* N^(2^19) */,
            0x0E7FFF1800001ULL /* N^(2^20) */, 0x0E7FFF8C00001ULL /* N^(2^21) */, 0x0E7FFFC600001ULL /* N^(2^22) */, 0x0E7FFFE300001ULL /* N^(2^23) */,
            0x0E7FFFF180001ULL /* N^(2^24) */, 0x0E7FFFF8C0001ULL /* N^(2^25) */, 0x0E7FFFFC60001ULL /* N^(2^26) */, 0x0E7FFFFE30001ULL /* N^(2^27) */,
            0x0E7FFFFF18001ULL /* N^(2^28) */, 0x0E7FFFFF8C001ULL /* N^(2^29) */, 0x0E7FFFFFC6001ULL /* N^(2^30) */, 0x0E7FFFFFE3001ULL /* N^(2^31) */,
            0x0E7FFFFFF1801ULL /* N^(2^32) */, 0x0E7FFFFFF8C01ULL /* N^(2^33) */, 0x0E7FFFFFFC601ULL /* N^(2^34) */, 0x0E7FFFFFFE301ULL /* N^(2^35) */,
            0x0E7FFFFFFF181ULL /* N^(2^36) */, 0x0E7FFFFFFF8C1ULL /* N^(2^37) */, 0x0E7FFFFFFFC61ULL /* N^(2^38) */, 0x0E7FFFFFFFE31ULL /* N^(2^39) */,
            0x0E7FFFFFFFF19ULL /* N^(2^40) */, 0x0E7FFFFFFFF8DULL /* N^(2^41) */, 0x0E7FFFFFFFFC7ULL /* N^(2^42) */
        },

        .cdltf64_cyclic_fwd =
        {
                          0X1P+0 /* psi^(0 ) */,  0X1.E7057610CDCP+43 /* psi^(1 ) */, 0X1.1013D16CC7E8P+45 /* psi^(2 ) */,  0X1.24B38DBA877P+46 /* psi^(3 ) */,
             0X1.A39FE1970ACP+46 /* psi^(4 ) */, 0X1.69F34393E336P+47 /* psi^(5 ) */,  0X1.3B38C356674P+45 /* psi^(6 ) */,  0X1.1C5A0E5C2C6P+46 /* psi^(7 ) */,
            0X1.67B0D895D5A6P+47 /* psi^(8 ) */, 0X1.7FF156DBFAA4P+46 /* psi^(9 ) */, 0X1.67183A8F8924P+47 /* psi^(10) */,  0X1.BC3218A98C1P+46 /* psi^(11) */,
            0X1.CDF1497776D2P+47 /* psi^(12) */,  0X1.D5B68C1D1C7P+46 /* psi^(13) */, 0X1.0341853B0378P+46 /* psi^(14) */,  0X1.DA938D41B42P+45 /* psi^(15) */,
                          0X1P+0 /* psi^(0 ) */, 0X1.1013D16CC7E8P+45 /* psi^(2 ) */,  0X1.A39FE1970ACP+46 /* psi^(4 ) */,  0X1.3B38C356674P+45 /* psi^(6 ) */,
            0X1.67B0D895D5A6P+47 /* psi^(8 ) */, 0X1.67183A8F8924P+47 /* psi^(10) */, 0X1.CDF1497776D2P+47 /* psi^(12) */, 0X1.0341853B0378P+46 /* psi^(14) */,
            0X1.35A256F2BC98P+45 /* psi^(16) */, 0X1.B5D9EDF30E5EP+47 /* psi^(18) */, 0X1.1EC726715F66P+47 /* psi^(20) */,   0X1.E540E55A1AP+40 /* psi^(22) */,
             0X1.A3B416CFFD8P+41 /* psi^(24) */, 0X1.4E557801E356P+47 /* psi^(26) */, 0X1.941BA0965D04P+46 /* psi^(28) */,  0X1.819F49C3902P+43 /* psi^(30) */,
                          0X1P+0 /* psi^(0 ) */,  0X1.24B38DBA877P+46 /* psi^(3 ) */,  0X1.3B38C356674P+45 /* psi^(6 ) */, 0X1.7FF156DBFAA4P+46 /* psi^(9 ) */,
            0X1.CDF1497776D2P+47 /* psi^(12) */,  0X1.DA938D41B42P+45 /* psi^(15) */, 0X1.B5D9EDF30E5EP+47 /* psi^(18) */, 0X1.18A5EA3BBAB8P+45 /* psi^(21) */,
             0X1.A3B416CFFD8P+41 /* psi^(24) */, 0X1.16E78B2B1A72P+47 /* psi^(27) */,  0X1.819F49C3902P+43 /* psi^(30) */, 0X1.B18FA89EF326P+47 /* psi^(33) */,
            0X1.FC601E68F544P+46 /* psi^(36) */, 0X1.41D2F8D1E9D2P+47 /* psi^(39) */, 0X1.A39F15C1DB78P+45 /* psi^(42) */, 0X1.CA4973E2E394P+46 /* psi^(45) */,
                          0X1P+0 /* psi^(0 ) */,  0X1.A39FE1970ACP+46 /* psi^(4 ) */, 0X1.67B0D895D5A6P+47 /* psi^(8 ) */, 0X1.CDF1497776D2P+47 /* psi^(12) */,
                          0X1P+0 /* psi^(0 ) */, 0X1.67B0D895D5A6P+47 /* psi^(8 ) */, 0X1.35A256F2BC98P+45 /* psi^(16) */,  0X1.A3B416CFFD8P+41 /* psi^(24) */,
                          0X1P+0 /* psi^(0 ) */, 0X1.CDF1497776D2P+47 /* psi^(12) */,  0X1.A3B416CFFD8P+41 /* psi^(24) */, 0X1.FC601E68F544P+46 /* psi^(36) */
        },

        .cdltf64_cyclic_inv =
        {
                          0X1P+0 /* psi_inv^(0 ) */, 0X1.C1F2B1CBB58CP+46 /* psi_inv^(1 ) */,   0X1.B7E60B63C7P+47 /* psi_inv^(2 ) */, 0X1.315A98F756C6P+47 /* psi_inv^(3 ) */,
             0X1.05F22FB4D18P+47 /* psi_inv^(4 ) */,  0X1.7230E9A9CB2P+46 /* psi_inv^(5 ) */, 0X1.03550FFC3958P+46 /* psi_inv^(6 ) */, 0X1.C3B68BEAB6BEP+47 /* psi_inv^(7 ) */,
            0X1.C9712FA4C00CP+47 /* psi_inv^(8 ) */,  0X1.02595AE959CP+44 /* psi_inv^(9 ) */, 0X1.CC357E354BCEP+47 /* psi_inv^(10) */, 0X1.89D685711154P+47 /* psi_inv^(11) */,
            0X1.6271B31D4138P+46 /* psi_inv^(12) */,  0X1.5D0469C7468P+41 /* psi_inv^(13) */,  0X1.A26120CF1A4P+43 /* psi_inv^(14) */, 0X1.2C9E2B510588P+46 /* psi_inv^(15) */,
                          0X1P+0 /* psi_inv^(0 ) */,   0X1.B7E60B63C7P+47 /* psi_inv^(2 ) */,  0X1.05F22FB4D18P+47 /* psi_inv^(4 ) */, 0X1.03550FFC3958P+46 /* psi_inv^(6 ) */,
            0X1.C9712FA4C00CP+47 /* psi_inv^(8 ) */, 0X1.CC357E354BCEP+47 /* psi_inv^(10) */, 0X1.6271B31D4138P+46 /* psi_inv^(12) */,  0X1.A26120CF1A4P+43 /* psi_inv^(14) */,
            0X1.82976A4350DCP+47 /* psi_inv^(16) */, 0X1.4E5F3D627E46P+47 /* psi_inv^(18) */,   0X1.075B444498P+40 /* psi_inv^(20) */, 0X1.A39F15C1DB78P+45 /* psi_inv^(22) */,
             0X1.A13C9DA8A97P+45 /* psi_inv^(24) */, 0X1.8131CF2A6632P+47 /* psi_inv^(26) */, 0X1.FC601E68F544P+46 /* psi_inv^(28) */, 0X1.8BFB0BA4CE08P+47 /* psi_inv^(30) */,
                          0X1P+0 /* psi_inv^(0 ) */, 0X1.315A98F756C6P+47 /* psi_inv^(3 ) */, 0X1.03550FFC3958P+46 /* psi_inv^(6 ) */,  0X1.02595AE959CP+44 /* psi_inv^(9 ) */,
            0X1.6271B31D4138P+46 /* psi_inv^(12) */, 0X1.2C9E2B510588P+46 /* psi_inv^(15) */, 0X1.4E5F3D627E46P+47 /* psi_inv^(18) */, 0X1.E3CDE75673F4P+46 /* psi_inv^(21) */,
             0X1.A13C9DA8A97P+45 /* psi_inv^(24) */,  0X1.9832F1B0733P+45 /* psi_inv^(27) */, 0X1.8BFB0BA4CE08P+47 /* psi_inv^(30) */, 0X1.DE0D4E344A78P+46 /* psi_inv^(33) */,
            0X1.941BA0965D04P+46 /* psi_inv^(36) */,  0X1.892E82A9288P+42 /* psi_inv^(39) */,   0X1.E540E55A1AP+40 /* psi_inv^(42) */, 0X1.CA8BEE58E2E8P+47 /* psi_inv^(45) */,
                          0X1P+0 /* psi_inv^(0 ) */,  0X1.05F22FB4D18P+47 /* psi_inv^(4 ) */, 0X1.C9712FA4C00CP+47 /* psi_inv^(8 ) */, 0X1.6271B31D4138P+46 /* psi_inv^(12) */,
                          0X1P+0 /* psi_inv^(0 ) */, 0X1.C9712FA4C00CP+47 /* psi_inv^(8 ) */, 0X1.82976A4350DCP+47 /* psi_inv^(16) */,  0X1.A13C9DA8A97P+45 /* psi_inv^(24) */,
                          0X1P+0 /* psi_inv^(0 ) */, 0X1.6271B31D4138P+46 /* psi_inv^(12) */,  0X1.A13C9DA8A97P+45 /* psi_inv^(24) */, 0X1.941BA0965D04P+46 /* psi_inv^(36) */
        },

        .cdltf64_negacyclic_fwd =
        {
                          0X1P+0 /* psi^(0 ) */, 0X1.11CF12FBAC72P+47 /* psi^(1 ) */,  0X1.E7057610CDCP+43 /* psi^(2 ) */,  0X1.A5C296C5A36P+46 /* psi^(3 ) */,
            0X1.1013D16CC7E8P+45 /* psi^(4 ) */,  0X1.18175ADF5BAP+47 /* psi^(5 ) */,  0X1.24B38DBA877P+46 /* psi^(6 ) */, 0X1.E8B415526F68P+45 /* psi^(7 ) */,
             0X1.A39FE1970ACP+46 /* psi^(8 ) */,  0X1.4C4F151C18BP+46 /* psi^(9 ) */, 0X1.69F34393E336P+47 /* psi^(10) */, 0X1.9598C8F6B58AP+47 /* psi^(11) */,
             0X1.3B38C356674P+45 /* psi^(12) */, 0X1.B90DE5A7143CP+46 /* psi^(13) */,  0X1.1C5A0E5C2C6P+46 /* psi^(14) */, 0X1.E34641FC7FC8P+45 /* psi^(15) */,
                          0X1P+0 /* psi^(0 ) */,  0X1.E7057610CDCP+43 /* psi^(2 ) */, 0X1.1013D16CC7E8P+45 /* psi^(4 ) */,  0X1.24B38DBA877P+46 /* psi^(6 ) */,
             0X1.A39FE1970ACP+46 /* psi^(8 ) */, 0X1.69F34393E336P+47 /* psi^(10) */,  0X1.3B38C356674P+45 /* psi^(12) */,  0X1.1C5A0E5C2C6P+46 /* psi^(14) */,
            0X1.67B0D895D5A6P+47 /* psi^(16) */, 0X1.7FF156DBFAA4P+46 /* psi^(18) */, 0X1.67183A8F8924P+47 /* psi^(20) */,  0X1.BC3218A98C1P+46 /* psi^(22) */,
            0X1.CDF1497776D2P+47 /* psi^(24) */,  0X1.D5B68C1D1C7P+46 /* psi^(26) */, 0X1.0341853B0378P+46 /* psi^(28) */,  0X1.DA938D41B42P+45 /* psi^(30) */,
                          0X1P+0 /* psi^(0 ) */,  0X1.A5C296C5A36P+46 /* psi^(3 ) */,  0X1.24B38DBA877P+46 /* psi^(6 ) */,  0X1.4C4F151C18BP+46 /* psi^(9 ) */,
             0X1.3B38C356674P+45 /* psi^(12) */, 0X1.E34641FC7FC8P+45 /* psi^(15) */, 0X1.7FF156DBFAA4P+46 /* psi^(18) */, 0X1.9967EA304E3AP+47 /* psi^(21) */,
            0X1.CDF1497776D2P+47 /* psi^(24) */,  0X1.08B1CAED64EP+43 /* psi^(27) */,  0X1.DA938D41B42P+45 /* psi^(30) */, 0X1.9BDC1B139A2CP+46 /* psi^(33) */,
            0X1.B5D9EDF30E5EP+47 /* psi^(36) */,  0X1.B5FC13AD10EP+47 /* psi^(39) */, 0X1.18A5EA3BBAB8P+45 /* psi^(42) */, 0X1.03AF769C4E96P+47 /* psi^(45) */,
                          0X1P+0 /* psi^(0 ) */, 0X1.1013D16CC7E8P+45 /* psi^(4 ) */,  0X1.A39FE1970ACP+46 /* psi^(8 ) */,  0X1.3B38C356674P+45 /* psi^(12) */,
                          0X1P+0 /* psi^(0 ) */,  0X1.A39FE1970ACP+46 /* psi^(8 ) */, 0X1.67B0D895D5A6P+47 /* psi^(16) */, 0X1.CDF1497776D2P+47 /* psi^(24) */,
                          0X1P+0 /* psi^(0 ) */,  0X1.3B38C356674P+45 /* psi^(12) */, 0X1.CDF1497776D2P+47 /* psi^(24) */, 0X1.B5D9EDF30E5EP+47 /* psi^(36) */
        },

        .cdltf64_negacyclic_inv =
        {
                          0X1P+0 /* psi_inv^(0 ) */, 0X1.EC1928891FD8P+46 /* psi_inv^(1 ) */, 0X1.C1F2B1CBB58CP+46 /* psi_inv^(2 ) */,  0X1.84BDEC23714P+46 /* psi_inv^(3 ) */,
              0X1.B7E60B63C7P+47 /* psi_inv^(4 ) */, 0X1.AFB942F711CAP+47 /* psi_inv^(5 ) */, 0X1.315A98F756C6P+47 /* psi_inv^(6 ) */, 0X1.CC4459B49748P+46 /* psi_inv^(7 ) */,
             0X1.05F22FB4D18P+47 /* psi_inv^(8 ) */, 0X1.9AA0505BBA68P+47 /* psi_inv^(9 ) */,  0X1.7230E9A9CB2P+46 /* psi_inv^(10) */, 0X1.6784F48C9004P+46 /* psi_inv^(11) */,
            0X1.03550FFC3958P+46 /* psi_inv^(12) */, 0X1.89CAAFDE5EF4P+46 /* psi_inv^(13) */, 0X1.C3B68BEAB6BEP+47 /* psi_inv^(14) */, 0X1.264640DA432CP+46 /* psi_inv^(15) */,
                          0X1P+0 /* psi_inv^(0 ) */, 0X1.C1F2B1CBB58CP+46 /* psi_inv^(2 ) */,   0X1.B7E60B63C7P+47 /* psi_inv^(4 ) */, 0X1.315A98F756C6P+47 /* psi_inv^(6 ) */,
             0X1.05F22FB4D18P+47 /* psi_inv^(8 ) */,  0X1.7230E9A9CB2P+46 /* psi_inv^(10) */, 0X1.03550FFC3958P+46 /* psi_inv^(12) */, 0X1.C3B68BEAB6BEP+47 /* psi_inv^(14) */,
            0X1.C9712FA4C00CP+47 /* psi_inv^(16) */,  0X1.02595AE959CP+44 /* psi_inv^(18) */, 0X1.CC357E354BCEP+47 /* psi_inv^(20) */, 0X1.89D685711154P+47 /* psi_inv^(22) */,
            0X1.6271B31D4138P+46 /* psi_inv^(24) */,  0X1.5D0469C7468P+41 /* psi_inv^(26) */,  0X1.A26120CF1A4P+43 /* psi_inv^(28) */, 0X1.2C9E2B510588P+46 /* psi_inv^(30) */,
                          0X1P+0 /* psi_inv^(0 ) */,  0X1.84BDEC23714P+46 /* psi_inv^(3 ) */, 0X1.315A98F756C6P+47 /* psi_inv^(6 ) */, 0X1.9AA0505BBA68P+47 /* psi_inv^(9 ) */,
            0X1.03550FFC3958P+46 /* psi_inv^(12) */, 0X1.264640DA432CP+46 /* psi_inv^(15) */,  0X1.02595AE959CP+44 /* psi_inv^(18) */, 0X1.F25F77BB8AD4P+46 /* psi_inv^(21) */,
            0X1.6271B31D4138P+46 /* psi_inv^(24) */,  0X1.FB058C06D31P+45 /* psi_inv^(27) */, 0X1.2C9E2B510588P+46 /* psi_inv^(30) */,  0X1.1A70D733343P+47 /* psi_inv^(33) */,
            0X1.4E5F3D627E46P+47 /* psi_inv^(36) */, 0X1.2D0BA293FA66P+47 /* psi_inv^(39) */, 0X1.E3CDE75673F4P+46 /* psi_inv^(42) */,  0X1.D58D80DD75EP+43 /* psi_inv^(45) */,
                          0X1P+0 /* psi_inv^(0 ) */,   0X1.B7E60B63C7P+47 /* psi_inv^(4 ) */,  0X1.05F22FB4D18P+47 /* psi_inv^(8 ) */, 0X1.03550FFC3958P+46 /* psi_inv^(12) */,
                          0X1P+0 /* psi_inv^(0 ) */,  0X1.05F22FB4D18P+47 /* psi_inv^(8 ) */, 0X1.C9712FA4C00CP+47 /* psi_inv^(16) */, 0X1.6271B31D4138P+46 /* psi_inv^(24) */,
                          0X1P+0 /* psi_inv^(0 ) */, 0X1.03550FFC3958P+46 /* psi_inv^(12) */, 0X1.6271B31D4138P+46 /* psi_inv^(24) */, 0X1.4E5F3D627E46P+47 /* psi_inv^(36) */
        },

        .cdltu64_cyclic_fwd =
        {
            0x0000000000001ULL /* psi^(0 ) */, 0x00F382BB0866EULL /* psi^(1 ) */, 0x022027A2D98FDULL /* psi^(2 ) */, 0x0492CE36EA1DCULL /* psi^(3 ) */,
            0x068E7F865C2B0ULL /* psi^(4 ) */, 0x0B4F9A1C9F19BULL /* psi^(5 ) */, 0x02767186ACCE8ULL /* psi^(6 ) */, 0x0471683970B18ULL /* psi^(7 ) */,
            0x0B3D86C4AEAD3ULL /* psi^(8 ) */, 0x05FFC55B6FEA9ULL /* psi^(9 ) */, 0x0B38C1D47C492ULL /* psi^(10) */, 0x06F0C862A6304ULL /* psi^(11) */,
            0x0E6F8A4BBBB69ULL /* psi^(12) */, 0x0756DA307471CULL /* psi^(13) */, 0x040D0614EC0DEULL /* psi^(14) */, 0x03B5271A83684ULL /* psi^(15) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x022027A2D98FDULL /* psi^(2 ) */, 0x068E7F865C2B0ULL /* psi^(4 ) */, 0x02767186ACCE8ULL /* psi^(6 ) */,
            0x0B3D86C4AEAD3ULL /* psi^(8 ) */, 0x0B38C1D47C492ULL /* psi^(10) */, 0x0E6F8A4BBBB69ULL /* psi^(12) */, 0x040D0614EC0DEULL /* psi^(14) */,
            0x026B44ADE5793ULL /* psi^(16) */, 0x0DAECF6F9872FULL /* psi^(18) */, 0x08F639338AFB3ULL /* psi^(20) */, 0x001E540E55A1AULL /* psi^(22) */,
            0x00347682D9FFBULL /* psi^(24) */, 0x0A72ABC00F1ABULL /* psi^(26) */, 0x06506E8259741ULL /* psi^(28) */, 0x00C0CFA4E1C81ULL /* psi^(30) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x0492CE36EA1DCULL /* psi^(3 ) */, 0x02767186ACCE8ULL /* psi^(6 ) */, 0x05FFC55B6FEA9ULL /* psi^(9 ) */,
            0x0E6F8A4BBBB69ULL /* psi^(12) */, 0x03B5271A83684ULL /* psi^(15) */, 0x0DAECF6F9872FULL /* psi^(18) */, 0x02314BD477757ULL /* psi^(21) */,
            0x00347682D9FFBULL /* psi^(24) */, 0x08B73C5958D39ULL /* psi^(27) */, 0x00C0CFA4E1C81ULL /* psi^(30) */, 0x0D8C7D44F7993ULL /* psi^(33) */,
            0x07F18079A3D51ULL /* psi^(36) */, 0x0A0E97C68F4E9ULL /* psi^(39) */, 0x03473E2B83B6FULL /* psi^(42) */, 0x072925CF8B8E5ULL /* psi^(45) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x068E7F865C2B0ULL /* psi^(4 ) */, 0x0B3D86C4AEAD3ULL /* psi^(8 ) */, 0x0E6F8A4BBBB69ULL /* psi^(12) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x0B3D86C4AEAD3ULL /* psi^(8 ) */, 0x026B44ADE5793ULL /* psi^(16) */, 0x00347682D9FFBULL /* psi^(24) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x0E6F8A4BBBB69ULL /* psi^(12) */, 0x00347682D9FFBULL /* psi^(24) */, 0x07F18079A3D51ULL /* psi^(36) */
        },

        .cdltu64_cyclic_inv =
        {
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x0707CAC72ED63ULL /* psi_inv^(1 ) */, 0x0DBF305B1E380ULL /* psi_inv^(2 ) */, 0x098AD4C7BAB63ULL /* psi_inv^(3 ) */,
            0x082F917DA68C0ULL /* psi_inv^(4 ) */, 0x05C8C3A6A72C8ULL /* psi_inv^(5 ) */, 0x040D543FF0E56ULL /* psi_inv^(6 ) */, 0x0E1DB45F55B5FULL /* psi_inv^(7 ) */,
            0x0E4B897D26006ULL /* psi_inv^(8 ) */, 0x0102595AE959CULL /* psi_inv^(9 ) */, 0x0E61ABF1AA5E7ULL /* psi_inv^(10) */, 0x0C4EB42B888AAULL /* psi_inv^(11) */,
            0x0589C6CC7504EULL /* psi_inv^(12) */, 0x002BA08D38E8DULL /* psi_inv^(13) */, 0x00D13090678D2ULL /* psi_inv^(14) */, 0x04B278AD44162ULL /* psi_inv^(15) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x0DBF305B1E380ULL /* psi_inv^(2 ) */, 0x082F917DA68C0ULL /* psi_inv^(4 ) */, 0x040D543FF0E56ULL /* psi_inv^(6 ) */,
            0x0E4B897D26006ULL /* psi_inv^(8 ) */, 0x0E61ABF1AA5E7ULL /* psi_inv^(10) */, 0x0589C6CC7504EULL /* psi_inv^(12) */, 0x00D13090678D2ULL /* psi_inv^(14) */,
            0x0C14BB521A86EULL /* psi_inv^(16) */, 0x0A72F9EB13F23ULL /* psi_inv^(18) */, 0x001075B444498ULL /* psi_inv^(20) */, 0x03473E2B83B6FULL /* psi_inv^(22) */,
            0x0342793B5152EULL /* psi_inv^(24) */, 0x0C098E7953319ULL /* psi_inv^(26) */, 0x07F18079A3D51ULL /* psi_inv^(28) */, 0x0C5FD85D26704ULL /* psi_inv^(30) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x098AD4C7BAB63ULL /* psi_inv^(3 ) */, 0x040D543FF0E56ULL /* psi_inv^(6 ) */, 0x0102595AE959CULL /* psi_inv^(9 ) */,
            0x0589C6CC7504EULL /* psi_inv^(12) */, 0x04B278AD44162ULL /* psi_inv^(15) */, 0x0A72F9EB13F23ULL /* psi_inv^(18) */, 0x078F379D59CFDULL /* psi_inv^(21) */,
            0x0342793B5152EULL /* psi_inv^(24) */, 0x033065E360E66ULL /* psi_inv^(27) */, 0x0C5FD85D26704ULL /* psi_inv^(30) */, 0x07783538D129EULL /* psi_inv^(33) */,
            0x06506E8259741ULL /* psi_inv^(36) */, 0x00624BA0AA4A2ULL /* psi_inv^(39) */, 0x001E540E55A1AULL /* psi_inv^(42) */, 0x0E545F72C7174ULL /* psi_inv^(45) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x082F917DA68C0ULL /* psi_inv^(4 ) */, 0x0E4B897D26006ULL /* psi_inv^(8 ) */, 0x0589C6CC7504EULL /* psi_inv^(12) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x0E4B897D26006ULL /* psi_inv^(8 ) */, 0x0C14BB521A86EULL /* psi_inv^(16) */, 0x0342793B5152EULL /* psi_inv^(24) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x0589C6CC7504EULL /* psi_inv^(12) */, 0x0342793B5152EULL /* psi_inv^(24) */, 0x06506E8259741ULL /* psi_inv^(36) */
        },

        .cdltu64_negacyclic_fwd =
        {
            0x0000000000001ULL /* psi^(0 ) */, 0x088E7897DD639ULL /* psi^(1 ) */, 0x00F382BB0866EULL /* psi^(2 ) */, 0x06970A5B168D8ULL /* psi^(3 ) */,
            0x022027A2D98FDULL /* psi^(4 ) */, 0x08C0BAD6FADD0ULL /* psi^(5 ) */, 0x0492CE36EA1DCULL /* psi^(6 ) */, 0x03D1682AA4DEDULL /* psi^(7 ) */,
            0x068E7F865C2B0ULL /* psi^(8 ) */, 0x05313C547062CULL /* psi^(9 ) */, 0x0B4F9A1C9F19BULL /* psi^(10) */, 0x0CACC647B5AC5ULL /* psi^(11) */,
            0x02767186ACCE8ULL /* psi^(12) */, 0x06E437969C50FULL /* psi^(13) */, 0x0471683970B18ULL /* psi^(14) */, 0x03C68C83F8FF9ULL /* psi^(15) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x00F382BB0866EULL /* psi^(2 ) */, 0x022027A2D98FDULL /* psi^(4 ) */, 0x0492CE36EA1DCULL /* psi^(6 ) */,
            0x068E7F865C2B0ULL /* psi^(8 ) */, 0x0B4F9A1C9F19BULL /* psi^(10) */, 0x02767186ACCE8ULL /* psi^(12) */, 0x0471683970B18ULL /* psi^(14) */,
            0x0B3D86C4AEAD3ULL /* psi^(16) */, 0x05FFC55B6FEA9ULL /* psi^(18) */, 0x0B38C1D47C492ULL /* psi^(20) */, 0x06F0C862A6304ULL /* psi^(22) */,
            0x0E6F8A4BBBB69ULL /* psi^(24) */, 0x0756DA307471CULL /* psi^(26) */, 0x040D0614EC0DEULL /* psi^(28) */, 0x03B5271A83684ULL /* psi^(30) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x06970A5B168D8ULL /* psi^(3 ) */, 0x0492CE36EA1DCULL /* psi^(6 ) */, 0x05313C547062CULL /* psi^(9 ) */,
            0x02767186ACCE8ULL /* psi^(12) */, 0x03C68C83F8FF9ULL /* psi^(15) */, 0x05FFC55B6FEA9ULL /* psi^(18) */, 0x0CCB3F518271DULL /* psi^(21) */,
            0x0E6F8A4BBBB69ULL /* psi^(24) */, 0x008458E576B27ULL /* psi^(27) */, 0x03B5271A83684ULL /* psi^(30) */, 0x066F706C4E68BULL /* psi^(33) */,
            0x0DAECF6F9872FULL /* psi^(36) */, 0x0DAFE09D68870ULL /* psi^(39) */, 0x02314BD477757ULL /* psi^(42) */, 0x081D7BB4E274BULL /* psi^(45) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x022027A2D98FDULL /* psi^(4 ) */, 0x068E7F865C2B0ULL /* psi^(8 ) */, 0x02767186ACCE8ULL /* psi^(12) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x068E7F865C2B0ULL /* psi^(8 ) */, 0x0B3D86C4AEAD3ULL /* psi^(16) */, 0x0E6F8A4BBBB69ULL /* psi^(24) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x02767186ACCE8ULL /* psi^(12) */, 0x0E6F8A4BBBB69ULL /* psi^(24) */, 0x0DAECF6F9872FULL /* psi^(36) */
        },

        .cdltu64_negacyclic_inv =
        {
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x07B064A2247F6ULL /* psi_inv^(1 ) */, 0x0707CAC72ED63ULL /* psi_inv^(2 ) */, 0x0612F7B08DC50ULL /* psi_inv^(3 ) */,
            0x0DBF305B1E380ULL /* psi_inv^(4 ) */, 0x0D7DCA17B88E5ULL /* psi_inv^(5 ) */, 0x098AD4C7BAB63ULL /* psi_inv^(6 ) */, 0x07311166D25D2ULL /* psi_inv^(7 ) */,
            0x082F917DA68C0ULL /* psi_inv^(8 ) */, 0x0CD50282DDD34ULL /* psi_inv^(9 ) */, 0x05C8C3A6A72C8ULL /* psi_inv^(10) */, 0x059E13D232401ULL /* psi_inv^(11) */,
            0x040D543FF0E56ULL /* psi_inv^(12) */, 0x06272ABF797BDULL /* psi_inv^(13) */, 0x0E1DB45F55B5FULL /* psi_inv^(14) */, 0x04991903690CBULL /* psi_inv^(15) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x0707CAC72ED63ULL /* psi_inv^(2 ) */, 0x0DBF305B1E380ULL /* psi_inv^(4 ) */, 0x098AD4C7BAB63ULL /* psi_inv^(6 ) */,
            0x082F917DA68C0ULL /* psi_inv^(8 ) */, 0x05C8C3A6A72C8ULL /* psi_inv^(10) */, 0x040D543FF0E56ULL /* psi_inv^(12) */, 0x0E1DB45F55B5FULL /* psi_inv^(14) */,
            0x0E4B897D26006ULL /* psi_inv^(16) */, 0x0102595AE959CULL /* psi_inv^(18) */, 0x0E61ABF1AA5E7ULL /* psi_inv^(20) */, 0x0C4EB42B888AAULL /* psi_inv^(22) */,
            0x0589C6CC7504EULL /* psi_inv^(24) */, 0x002BA08D38E8DULL /* psi_inv^(26) */, 0x00D13090678D2ULL /* psi_inv^(28) */, 0x04B278AD44162ULL /* psi_inv^(30) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x0612F7B08DC50ULL /* psi_inv^(3 ) */, 0x098AD4C7BAB63ULL /* psi_inv^(6 ) */, 0x0CD50282DDD34ULL /* psi_inv^(9 ) */,
            0x040D543FF0E56ULL /* psi_inv^(12) */, 0x04991903690CBULL /* psi_inv^(15) */, 0x0102595AE959CULL /* psi_inv^(18) */, 0x07C97DDEEE2B5ULL /* psi_inv^(21) */,
            0x0589C6CC7504EULL /* psi_inv^(24) */, 0x03F60B180DA62ULL /* psi_inv^(27) */, 0x04B278AD44162ULL /* psi_inv^(30) */, 0x08D386B999A18ULL /* psi_inv^(33) */,
            0x0A72F9EB13F23ULL /* psi_inv^(36) */, 0x09685D149FD33ULL /* psi_inv^(39) */, 0x078F379D59CFDULL /* psi_inv^(42) */, 0x00EAC6C06EBAFULL /* psi_inv^(45) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x0DBF305B1E380ULL /* psi_inv^(4 ) */, 0x082F917DA68C0ULL /* psi_inv^(8 ) */, 0x040D543FF0E56ULL /* psi_inv^(12) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x082F917DA68C0ULL /* psi_inv^(8 ) */, 0x0E4B897D26006ULL /* psi_inv^(16) */, 0x0589C6CC7504EULL /* psi_inv^(24) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x040D543FF0E56ULL /* psi_inv^(12) */, 0x0589C6CC7504EULL /* psi_inv^(24) */, 0x0A72F9EB13F23ULL /* psi_inv^(36) */
        }
    },

    {

    /* --------------------------------------------------------------- */
    /* k = 51     g = 11   p = 448600744132609    (0x1980000000001)    */
    /* --------------------------------------------------------------- */

        .k = 51ULL, .g = 11ULL, .p = (51ULL << 43ULL) + 1ULL,

        .magic = 0x141414141414077BULL,

        .shift = 45ULL,

        .prime_inv   = 0X1.4141414141407P-49,
        .prime_inv52 = 0x197FFFFFFFFFFULL,
        .r0          = 0x1575F5F5F5FC5ULL,

        .twiddle =
        {
            0x148B1E78B1E47ULL /* w^(2^0 ) */, 0x0699DFE8C8724ULL /* w^(2^1 ) */, 0x0682051CF2786ULL /* w^(2^2 ) */, 0x00D90BDB6F83BULL /* w^(2^3 ) */,
            0x02D398BEF1C91ULL /* w^(2^4 ) */, 0x0EDEA7F88988EULL /* w^(2^5 ) */, 0x0A51B1532262AULL /* w^(2^6 ) */, 0x042218E36893AULL /* w^(2^7 ) */,
            0x0B2167AE7B2D3ULL /* w^(2^8 ) */, 0x11AAD8ADF0B2AULL /* w^(2^9 ) */, 0x0654822D08DF2ULL /* w^(2^10) */, 0x03296DBB09A9EULL /* w^(2^11) */,
            0x0CD41789DF997ULL /* w^(2^12) */, 0x060E530A4013EULL /* w^(2^13) */, 0x12B71E574A460ULL /* w^(2^14) */, 0x0FC33525224E1ULL /* w^(2^15) */,
            0x0DFBCB7A8FED2ULL /* w^(2^16) */, 0x1699E88322311ULL /* w^(2^17) */, 0x0B98CF5B1C6FBULL /* w^(2^18) */, 0x16E76A7C3DFDAULL /* w^(2^19) */,
            0x027BD5ED28405ULL /* w^(2^20) */, 0x06793A20717ABULL /* w^(2^21) */, 0x1803D59369AD2ULL /* w^(2^22) */, 0x03982C4D547FDULL /* w^(2^23) */,
            0x106C4FCEDD149ULL /* w^(2^24) */, 0x07380C7D6F278ULL /* w^(2^25) */, 0x0C8BE0AC75E08ULL /* w^(2^26) */, 0x0EFB73F934796ULL /* w^(2^27) */,
            0x0C1937B38E3BAULL /* w^(2^28) */, 0x0173D7F512D2CULL /* w^(2^29) */, 0x1923461A44200ULL /* w^(2^30) */, 0x196EACAD2007AULL /* w^(2^31) */,
            0x11E36FE2C4CE7ULL /* w^(2^32) */, 0x01CF0CF118835ULL /* w^(2^33) */, 0x17CCB8E187A40ULL /* w^(2^34) */, 0x1480EA913ADBCULL /* w^(2^35) */,
            0x195AE353D36C4ULL /* w^(2^36) */, 0x08E6082748B5CULL /* w^(2^37) */, 0x0A5E8D6DB88C2ULL /* w^(2^38) */, 0x0F5106A51D5D4ULL /* w^(2^39) */,
            0x0768A1C735867ULL /* w^(2^40) */, 0x03FEA60682D83ULL /* w^(2^41) */, 0x1980000000000ULL /* w^(2^42) */
        },

        .twiddle_inv =
        {
            0x16CAC0C431BE6ULL /* w_inv^(2^0 ) */, 0x09353295AABAEULL /* w_inv^(2^1 ) */, 0x18CBC5114493EULL /* w_inv^(2^2 ) */, 0x0B13C830A1969ULL /* w_inv^(2^3 ) */,
            0x15CA18195808EULL /* w_inv^(2^4 ) */, 0x063936C652867ULL /* w_inv^(2^5 ) */, 0x0120C698ACD52ULL /* w_inv^(2^6 ) */, 0x02E92E10965DCULL /* w_inv^(2^7 ) */,
            0x0AF0406C5159AULL /* w_inv^(2^8 ) */, 0x0A27A54DF9F78ULL /* w_inv^(2^9 ) */, 0x051A9D99D7B02ULL /* w_inv^(2^10) */, 0x017B513628FF0ULL /* w_inv^(2^11) */,
            0x1774F7E0D4BDBULL /* w_inv^(2^12) */, 0x09948E3C0D713ULL /* w_inv^(2^13) */, 0x0BDC84B5AB416ULL /* w_inv^(2^14) */, 0x123C0E18A3C0DULL /* w_inv^(2^15) */,
            0x0F912FA2207FDULL /* w_inv^(2^16) */, 0x16DA682339AE1ULL /* w_inv^(2^17) */, 0x069CD3FC53A84ULL /* w_inv^(2^18) */, 0x146B8630B39DDULL /* w_inv^(2^19) */,
            0x0A34CFD721613ULL /* w_inv^(2^20) */, 0x07664E7518C76ULL /* w_inv^(2^21) */, 0x0574C0C831A56ULL /* w_inv^(2^22) */, 0x0D1FE5CA024FCULL /* w_inv^(2^23) */,
            0x00EBE797511FCULL /* w_inv^(2^24) */, 0x142114354561CULL /* w_inv^(2^25) */, 0x09257665F808AULL /* w_inv^(2^26) */, 0x1575D40767ECFULL /* w_inv^(2^27) */,
            0x131AECAD83A5CULL /* w_inv^(2^28) */, 0x002C5781FA2CFULL /* w_inv^(2^29) */, 0x1931C13FB5352ULL /* w_inv^(2^30) */, 0x014C3808254C9ULL /* w_inv^(2^31) */,
            0x0A84AB4D846E1ULL /* w_inv^(2^32) */, 0x18F0D56958553ULL /* w_inv^(2^33) */, 0x0730952449158ULL /* w_inv^(2^34) */, 0x1673A9DD10BD8ULL /* w_inv^(2^35) */,
            0x034D195026C62ULL /* w_inv^(2^36) */, 0x1635D7ADFC602ULL /* w_inv^(2^37) */, 0x17043344BCC46ULL /* w_inv^(2^38) */, 0x009AC91D27C77ULL /* w_inv^(2^39) */,
            0x0CBBC84E97DA7ULL /* w_inv^(2^40) */, 0x158159F97D27EULL /* w_inv^(2^41) */, 0x1980000000000ULL /* w_inv^(2^42) */
        },

        .size_inv =
        {
            0x0000000000001ULL /* N^(2^0 ) */, 0x0CC0000000001ULL /* N^(2^1 ) */, 0x1320000000001ULL /* N^(2^2 ) */, 0x1650000000001ULL /* N^(2^3 ) */,
            0x17E8000000001ULL /* N^(2^4 ) */, 0x18B4000000001ULL /* N^(2^5 ) */, 0x191A000000001ULL /* N^(2^6 ) */, 0x194D000000001ULL /* N^(2^7 ) */,
            0x1966800000001ULL /* N^(2^8 ) */, 0x1973400000001ULL /* N^(2^9 ) */, 0x1979A00000001ULL /* N^(2^10) */, 0x197CD00000001ULL /* N^(2^11) */,
            0x197E680000001ULL /* N^(2^12) */, 0x197F340000001ULL /* N^(2^13) */, 0x197F9A0000001ULL /* N^(2^14) */, 0x197FCD0000001ULL /* N^(2^15) */,
            0x197FE68000001ULL /* N^(2^16) */, 0x197FF34000001ULL /* N^(2^17) */, 0x197FF9A000001ULL /* N^(2^18) */, 0x197FFCD000001ULL /* N^(2^19) */,
            0x197FFE6800001ULL /* N^(2^20) */, 0x197FFF3400001ULL /* N^(2^21) */, 0x197FFF9A00001ULL /* N^(2^22) */, 0x197FFFCD00001ULL /* N^(2^23) */,
            0x197FFFE680001ULL /* N^(2^24) */, 0x197FFFF340001ULL /* N^(2^25) */, 0x197FFFF9A0001ULL /* N^(2^26) */, 0x197FFFFCD0001ULL /* N^(2^27) */,
            0x197FFFFE68001ULL /* N^(2^28) */, 0x197FFFFF34001ULL /* N^(2^29) */, 0x197FFFFF9A001ULL /* N^(2^30) */, 0x197FFFFFCD001ULL /* N^(2^31) */,
            0x197FFFFFE6801ULL /* N^(2^32) */, 0x197FFFFFF3401ULL /* N^(2^33) */, 0x197FFFFFF9A01ULL /* N^(2^34) */, 0x197FFFFFFCD01ULL /* N^(2^35) */,
            0x197FFFFFFE681ULL /* N^(2^36) */, 0x197FFFFFFF341ULL /* N^(2^37) */, 0x197FFFFFFF9A1ULL /* N^(2^38) */, 0x197FFFFFFFCD1ULL /* N^(2^39) */,
            0x197FFFFFFFE69ULL /* N^(2^40) */, 0x197FFFFFFFF35ULL /* N^(2^41) */, 0x197FFFFFFFF9BULL /* N^(2^42) */
        },

        .cdltf64_cyclic_fwd =
        {
                          0X1P+0 /* psi^(0 ) */, 0X1.1CC104E916B8P+47 /* psi^(1 ) */, 0X1.4BD1ADB71184P+47 /* psi^(2 ) */, 0X1.261D70B2CDD9P+48 /* psi^(3 ) */,
            0X1.EA20D4A3ABA8P+47 /* psi^(4 ) */, 0X1.928CFA3B08D8P+47 /* psi^(5 ) */, 0X1.0425FD18AE8AP+48 /* psi^(6 ) */,  0X1.75B88D56E68P+47 /* psi^(7 ) */,
            0X1.DA2871CD619CP+46 /* psi^(8 ) */, 0X1.AAFF13EBA076P+47 /* psi^(9 ) */, 0X1.3CFB2ECAB2AEP+47 /* psi^(10) */, 0X1.72F268832DD6P+48 /* psi^(11) */,
            0X1.32C3ADD744ECP+47 /* psi^(12) */,  0X1.CDA61FC92DFP+44 /* psi^(13) */,  0X1.6459224A58CP+44 /* psi^(14) */, 0X1.10C0E30AAE98P+48 /* psi^(15) */,
                          0X1P+0 /* psi^(0 ) */, 0X1.4BD1ADB71184P+47 /* psi^(2 ) */, 0X1.EA20D4A3ABA8P+47 /* psi^(4 ) */, 0X1.0425FD18AE8AP+48 /* psi^(6 ) */,
            0X1.DA2871CD619CP+46 /* psi^(8 ) */, 0X1.3CFB2ECAB2AEP+47 /* psi^(10) */, 0X1.32C3ADD744ECP+47 /* psi^(12) */,  0X1.6459224A58CP+44 /* psi^(14) */,
            0X1.FF5303416C18P+45 /* psi^(16) */, 0X1.5B72766D258EP+47 /* psi^(18) */, 0X1.B9D247D18172P+47 /* psi^(20) */, 0X1.1F971B5C988EP+47 /* psi^(22) */,
            0X1.9886F62D04B4P+47 /* psi^(24) */, 0X1.09007C720B18P+46 /* psi^(26) */, 0X1.8E536E2D838AP+48 /* psi^(28) */, 0X1.3DE65DA19DD8P+45 /* psi^(30) */,
                          0X1P+0 /* psi^(0 ) */, 0X1.261D70B2CDD9P+48 /* psi^(3 ) */, 0X1.0425FD18AE8AP+48 /* psi^(6 ) */, 0X1.AAFF13EBA076P+47 /* psi^(9 ) */,
            0X1.32C3ADD744ECP+47 /* psi^(12) */, 0X1.10C0E30AAE98P+48 /* psi^(15) */, 0X1.5B72766D258EP+47 /* psi^(18) */, 0X1.5F9A5BCD57A8P+46 /* psi^(21) */,
            0X1.9886F62D04B4P+47 /* psi^(24) */, 0X1.526501990111P+48 /* psi^(27) */, 0X1.3DE65DA19DD8P+45 /* psi^(30) */, 0X1.099F7D8B74A5P+48 /* psi^(33) */,
            0X1.45DF2B5C545AP+47 /* psi^(36) */, 0X1.BA4772A91982P+47 /* psi^(39) */, 0X1.F304D1354D54P+47 /* psi^(42) */, 0X1.7B259E036D22P+48 /* psi^(45) */,
                          0X1P+0 /* psi^(0 ) */, 0X1.EA20D4A3ABA8P+47 /* psi^(4 ) */, 0X1.DA2871CD619CP+46 /* psi^(8 ) */, 0X1.32C3ADD744ECP+47 /* psi^(12) */,
                          0X1P+0 /* psi^(0 ) */, 0X1.DA2871CD619CP+46 /* psi^(8 ) */, 0X1.FF5303416C18P+45 /* psi^(16) */, 0X1.9886F62D04B4P+47 /* psi^(24) */,
                          0X1P+0 /* psi^(0 ) */, 0X1.32C3ADD744ECP+47 /* psi^(12) */, 0X1.9886F62D04B4P+47 /* psi^(24) */, 0X1.45DF2B5C545AP+47 /* psi^(36) */
        },

        .cdltf64_cyclic_inv =
        {
                          0X1P+0 /* psi_inv^(0 ) */, 0X1.635D7ADFC602P+48 /* psi_inv^(1 ) */, 0X1.7043344BCC46P+48 /* psi_inv^(2 ) */, 0X1.DC1927556F6CP+46 /* psi_inv^(3 ) */,
             0X1.35923A4F8EEP+43 /* psi_inv^(4 ) */,  0X1.166BF99BFBCP+46 /* psi_inv^(5 ) */, 0X1.55BFE0E37D3BP+48 /* psi_inv^(6 ) */, 0X1.890D93F35316P+48 /* psi_inv^(7 ) */,
            0X1.977909D2FB4EP+47 /* psi_inv^(8 ) */, 0X1.84C2B4867079P+48 /* psi_inv^(9 ) */, 0X1.08347251B3BAP+48 /* psi_inv^(10) */, 0X1.4019690CAA17P+48 /* psi_inv^(11) */,
             0X1.762DB82E7E9P+47 /* psi_inv^(12) */, 0X1.0608182085A8P+47 /* psi_inv^(13) */, 0X1.D48D8992DA74P+47 /* psi_inv^(14) */, 0X1.4A498291A0F4P+48 /* psi_inv^(15) */,
                          0X1P+0 /* psi_inv^(0 ) */, 0X1.7043344BCC46P+48 /* psi_inv^(2 ) */,  0X1.35923A4F8EEP+43 /* psi_inv^(4 ) */, 0X1.55BFE0E37D3BP+48 /* psi_inv^(6 ) */,
            0X1.977909D2FB4EP+47 /* psi_inv^(8 ) */, 0X1.08347251B3BAP+48 /* psi_inv^(10) */,  0X1.762DB82E7E9P+47 /* psi_inv^(12) */, 0X1.D48D8992DA74P+47 /* psi_inv^(14) */,
            0X1.58159F97D27EP+48 /* psi_inv^(16) */, 0X1.81BA6DDB5A75P+48 /* psi_inv^(18) */, 0X1.FD3C5228BB16P+47 /* psi_inv^(20) */, 0X1.F304D1354D54P+47 /* psi_inv^(22) */,
            0X1.2175E38CA79AP+48 /* psi_inv^(24) */, 0X1.27B405CEA2EEP+47 /* psi_inv^(26) */, 0X1.45DF2B5C545AP+47 /* psi_inv^(28) */, 0X1.E42E5248EE7EP+47 /* psi_inv^(30) */,
                          0X1P+0 /* psi_inv^(0 ) */, 0X1.DC1927556F6CP+46 /* psi_inv^(3 ) */, 0X1.55BFE0E37D3BP+48 /* psi_inv^(6 ) */, 0X1.84C2B4867079P+48 /* psi_inv^(9 ) */,
             0X1.762DB82E7E9P+47 /* psi_inv^(12) */, 0X1.4A498291A0F4P+48 /* psi_inv^(15) */, 0X1.81BA6DDB5A75P+48 /* psi_inv^(18) */, 0X1.286CBBE69158P+45 /* psi_inv^(21) */,
            0X1.2175E38CA79AP+48 /* psi_inv^(24) */, 0X1.9D7305C4F72AP+47 /* psi_inv^(27) */, 0X1.E42E5248EE7EP+47 /* psi_inv^(30) */, 0X1.A5142901CFF8P+45 /* psi_inv^(33) */,
            0X1.8E536E2D838AP+48 /* psi_inv^(36) */,  0X1.DE4D81959D6P+43 /* psi_inv^(39) */, 0X1.1F971B5C988EP+47 /* psi_inv^(42) */, 0X1.14FBF3EFBD2DP+48 /* psi_inv^(45) */,
                          0X1P+0 /* psi_inv^(0 ) */,  0X1.35923A4F8EEP+43 /* psi_inv^(4 ) */, 0X1.977909D2FB4EP+47 /* psi_inv^(8 ) */,  0X1.762DB82E7E9P+47 /* psi_inv^(12) */,
                          0X1P+0 /* psi_inv^(0 ) */, 0X1.977909D2FB4EP+47 /* psi_inv^(8 ) */, 0X1.58159F97D27EP+48 /* psi_inv^(16) */, 0X1.2175E38CA79AP+48 /* psi_inv^(24) */,
                          0X1P+0 /* psi_inv^(0 ) */,  0X1.762DB82E7E9P+47 /* psi_inv^(12) */, 0X1.2175E38CA79AP+48 /* psi_inv^(24) */, 0X1.8E536E2D838AP+48 /* psi_inv^(36) */
        },

        .cdltf64_negacyclic_fwd =
        {
                          0X1P+0 /* psi^(0 ) */, 0X1.95AE353D36C4P+48 /* psi^(1 ) */, 0X1.1CC104E916B8P+47 /* psi^(2 ) */, 0X1.25FCD682C8C6P+47 /* psi^(3 ) */,
            0X1.4BD1ADB71184P+47 /* psi^(4 ) */,  0X1.10D970D781AP+45 /* psi^(5 ) */, 0X1.261D70B2CDD9P+48 /* psi^(6 ) */, 0X1.6CACAAB608E8P+45 /* psi^(7 ) */,
            0X1.EA20D4A3ABA8P+47 /* psi^(8 ) */, 0X1.0C2FBFD144BFP+48 /* psi^(9 ) */, 0X1.928CFA3B08D8P+47 /* psi^(10) */, 0X1.8A155F6481B6P+48 /* psi^(11) */,
            0X1.0425FD18AE8AP+48 /* psi^(12) */,  0X1.B277118FC6FP+46 /* psi^(13) */,  0X1.75B88D56E68P+47 /* psi^(14) */, 0X1.20F8A8C3B39FP+48 /* psi^(15) */,
                          0X1P+0 /* psi^(0 ) */, 0X1.1CC104E916B8P+47 /* psi^(2 ) */, 0X1.4BD1ADB71184P+47 /* psi^(4 ) */, 0X1.261D70B2CDD9P+48 /* psi^(6 ) */,
            0X1.EA20D4A3ABA8P+47 /* psi^(8 ) */, 0X1.928CFA3B08D8P+47 /* psi^(10) */, 0X1.0425FD18AE8AP+48 /* psi^(12) */,  0X1.75B88D56E68P+47 /* psi^(14) */,
            0X1.DA2871CD619CP+46 /* psi^(16) */, 0X1.AAFF13EBA076P+47 /* psi^(18) */, 0X1.3CFB2ECAB2AEP+47 /* psi^(20) */, 0X1.72F268832DD6P+48 /* psi^(22) */,
            0X1.32C3ADD744ECP+47 /* psi^(24) */,  0X1.CDA61FC92DFP+44 /* psi^(26) */,  0X1.6459224A58CP+44 /* psi^(28) */, 0X1.10C0E30AAE98P+48 /* psi^(30) */,
                          0X1P+0 /* psi^(0 ) */, 0X1.25FCD682C8C6P+47 /* psi^(3 ) */, 0X1.261D70B2CDD9P+48 /* psi^(6 ) */, 0X1.0C2FBFD144BFP+48 /* psi^(9 ) */,
            0X1.0425FD18AE8AP+48 /* psi^(12) */, 0X1.20F8A8C3B39FP+48 /* psi^(15) */, 0X1.AAFF13EBA076P+47 /* psi^(18) */, 0X1.2D1EA5E0DBCCP+46 /* psi^(21) */,
            0X1.32C3ADD744ECP+47 /* psi^(24) */, 0X1.95571272620FP+48 /* psi^(27) */, 0X1.10C0E30AAE98P+48 /* psi^(30) */, 0X1.2883658635BAP+48 /* psi^(33) */,
            0X1.5B72766D258EP+47 /* psi^(36) */, 0X1.0B4B8EF060D4P+46 /* psi^(39) */, 0X1.5F9A5BCD57A8P+46 /* psi^(42) */,  0X1.E2AEB268F02P+45 /* psi^(45) */,
                          0X1P+0 /* psi^(0 ) */, 0X1.4BD1ADB71184P+47 /* psi^(4 ) */, 0X1.EA20D4A3ABA8P+47 /* psi^(8 ) */, 0X1.0425FD18AE8AP+48 /* psi^(12) */,
                          0X1P+0 /* psi^(0 ) */, 0X1.EA20D4A3ABA8P+47 /* psi^(8 ) */, 0X1.DA2871CD619CP+46 /* psi^(16) */, 0X1.32C3ADD744ECP+47 /* psi^(24) */,
                          0X1P+0 /* psi^(0 ) */, 0X1.0425FD18AE8AP+48 /* psi^(12) */, 0X1.32C3ADD744ECP+47 /* psi^(24) */, 0X1.5B72766D258EP+47 /* psi^(36) */
        },

        .cdltf64_negacyclic_inv =
        {
                          0X1P+0 /* psi_inv^(0 ) */,  0X1.A68CA813631P+45 /* psi_inv^(1 ) */, 0X1.635D7ADFC602P+48 /* psi_inv^(2 ) */, 0X1.273EF30EC645P+48 /* psi_inv^(3 ) */,
            0X1.7043344BCC46P+48 /* psi_inv^(4 ) */, 0X1.4F7F6567C458P+47 /* psi_inv^(5 ) */, 0X1.DC1927556F6CP+46 /* psi_inv^(6 ) */, 0X1.3C11D945CF8FP+48 /* psi_inv^(7 ) */,
             0X1.35923A4F8EEP+43 /* psi_inv^(8 ) */,  0X1.E44FAC5C47CP+42 /* psi_inv^(9 ) */,  0X1.166BF99BFBCP+46 /* psi_inv^(10) */, 0X1.118B6E714F9AP+47 /* psi_inv^(11) */,
            0X1.55BFE0E37D3BP+48 /* psi_inv^(12) */, 0X1.24EB6A0100FDP+48 /* psi_inv^(13) */, 0X1.890D93F35316P+48 /* psi_inv^(14) */, 0X1.22F38552A422P+48 /* psi_inv^(15) */,
                          0X1P+0 /* psi_inv^(0 ) */, 0X1.635D7ADFC602P+48 /* psi_inv^(2 ) */, 0X1.7043344BCC46P+48 /* psi_inv^(4 ) */, 0X1.DC1927556F6CP+46 /* psi_inv^(6 ) */,
             0X1.35923A4F8EEP+43 /* psi_inv^(8 ) */,  0X1.166BF99BFBCP+46 /* psi_inv^(10) */, 0X1.55BFE0E37D3BP+48 /* psi_inv^(12) */, 0X1.890D93F35316P+48 /* psi_inv^(14) */,
            0X1.977909D2FB4EP+47 /* psi_inv^(16) */, 0X1.84C2B4867079P+48 /* psi_inv^(18) */, 0X1.08347251B3BAP+48 /* psi_inv^(20) */, 0X1.4019690CAA17P+48 /* psi_inv^(22) */,
             0X1.762DB82E7E9P+47 /* psi_inv^(24) */, 0X1.0608182085A8P+47 /* psi_inv^(26) */, 0X1.D48D8992DA74P+47 /* psi_inv^(28) */, 0X1.4A498291A0F4P+48 /* psi_inv^(30) */,
                          0X1P+0 /* psi_inv^(0 ) */, 0X1.273EF30EC645P+48 /* psi_inv^(3 ) */, 0X1.DC1927556F6CP+46 /* psi_inv^(6 ) */,  0X1.E44FAC5C47CP+42 /* psi_inv^(9 ) */,
            0X1.55BFE0E37D3BP+48 /* psi_inv^(12) */, 0X1.22F38552A422P+48 /* psi_inv^(15) */, 0X1.84C2B4867079P+48 /* psi_inv^(18) */, 0X1.2732D81CFC6AP+48 /* psi_inv^(21) */,
             0X1.762DB82E7E9P+47 /* psi_inv^(24) */,  0X1.738EC1DB102P+47 /* psi_inv^(27) */, 0X1.4A498291A0F4P+48 /* psi_inv^(30) */, 0X1.C2831A58BB24P+46 /* psi_inv^(33) */,
            0X1.81BA6DDB5A75P+48 /* psi_inv^(36) */,  0X1.A906F656CC8P+43 /* psi_inv^(39) */, 0X1.286CBBE69158P+45 /* psi_inv^(42) */, 0X1.79810533F686P+47 /* psi_inv^(45) */,
                          0X1P+0 /* psi_inv^(0 ) */, 0X1.7043344BCC46P+48 /* psi_inv^(4 ) */,  0X1.35923A4F8EEP+43 /* psi_inv^(8 ) */, 0X1.55BFE0E37D3BP+48 /* psi_inv^(12) */,
                          0X1P+0 /* psi_inv^(0 ) */,  0X1.35923A4F8EEP+43 /* psi_inv^(8 ) */, 0X1.977909D2FB4EP+47 /* psi_inv^(16) */,  0X1.762DB82E7E9P+47 /* psi_inv^(24) */,
                          0X1P+0 /* psi_inv^(0 ) */, 0X1.55BFE0E37D3BP+48 /* psi_inv^(12) */,  0X1.762DB82E7E9P+47 /* psi_inv^(24) */, 0X1.81BA6DDB5A75P+48 /* psi_inv^(36) */
        },

        .cdltu64_cyclic_fwd =
        {
            0x0000000000001ULL /* psi^(0 ) */, 0x08E6082748B5CULL /* psi^(1 ) */, 0x0A5E8D6DB88C2ULL /* psi^(2 ) */, 0x1261D70B2CDD9ULL /* psi^(3 ) */,
            0x0F5106A51D5D4ULL /* psi^(4 ) */, 0x0C9467D1D846CULL /* psi^(5 ) */, 0x10425FD18AE8AULL /* psi^(6 ) */, 0x0BADC46AB7340ULL /* psi^(7 ) */,
            0x0768A1C735867ULL /* psi^(8 ) */, 0x0D57F89F5D03BULL /* psi^(9 ) */, 0x09E7D97655957ULL /* psi^(10) */, 0x172F268832DD6ULL /* psi^(11) */,
            0x09961D6EBA276ULL /* psi^(12) */, 0x01CDA61FC92DFULL /* psi^(13) */, 0x016459224A58CULL /* psi^(14) */, 0x110C0E30AAE98ULL /* psi^(15) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x0A5E8D6DB88C2ULL /* psi^(2 ) */, 0x0F5106A51D5D4ULL /* psi^(4 ) */, 0x10425FD18AE8AULL /* psi^(6 ) */,
            0x0768A1C735867ULL /* psi^(8 ) */, 0x09E7D97655957ULL /* psi^(10) */, 0x09961D6EBA276ULL /* psi^(12) */, 0x016459224A58CULL /* psi^(14) */,
            0x03FEA60682D83ULL /* psi^(16) */, 0x0ADB93B3692C7ULL /* psi^(18) */, 0x0DCE923E8C0B9ULL /* psi^(20) */, 0x08FCB8DAE4C47ULL /* psi^(22) */,
            0x0CC437B16825AULL /* psi^(24) */, 0x042401F1C82C6ULL /* psi^(26) */, 0x18E536E2D838AULL /* psi^(28) */, 0x027BCCBB433BBULL /* psi^(30) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x1261D70B2CDD9ULL /* psi^(3 ) */, 0x10425FD18AE8AULL /* psi^(6 ) */, 0x0D57F89F5D03BULL /* psi^(9 ) */,
            0x09961D6EBA276ULL /* psi^(12) */, 0x110C0E30AAE98ULL /* psi^(15) */, 0x0ADB93B3692C7ULL /* psi^(18) */, 0x057E696F355EAULL /* psi^(21) */,
            0x0CC437B16825AULL /* psi^(24) */, 0x1526501990111ULL /* psi^(27) */, 0x027BCCBB433BBULL /* psi^(30) */, 0x1099F7D8B74A5ULL /* psi^(33) */,
            0x0A2EF95AE2A2DULL /* psi^(36) */, 0x0DD23B9548CC1ULL /* psi^(39) */, 0x0F982689AA6AAULL /* psi^(42) */, 0x17B259E036D22ULL /* psi^(45) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x0F5106A51D5D4ULL /* psi^(4 ) */, 0x0768A1C735867ULL /* psi^(8 ) */, 0x09961D6EBA276ULL /* psi^(12) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x0768A1C735867ULL /* psi^(8 ) */, 0x03FEA60682D83ULL /* psi^(16) */, 0x0CC437B16825AULL /* psi^(24) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x09961D6EBA276ULL /* psi^(12) */, 0x0CC437B16825AULL /* psi^(24) */, 0x0A2EF95AE2A2DULL /* psi^(36) */
        },

        .cdltu64_cyclic_inv =
        {
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x1635D7ADFC602ULL /* psi_inv^(1 ) */, 0x17043344BCC46ULL /* psi_inv^(2 ) */, 0x0770649D55BDBULL /* psi_inv^(3 ) */,
            0x009AC91D27C77ULL /* psi_inv^(4 ) */, 0x0459AFE66FEF0ULL /* psi_inv^(5 ) */, 0x155BFE0E37D3BULL /* psi_inv^(6 ) */, 0x1890D93F35316ULL /* psi_inv^(7 ) */,
            0x0CBBC84E97DA7ULL /* psi_inv^(8 ) */, 0x184C2B4867079ULL /* psi_inv^(9 ) */, 0x108347251B3BAULL /* psi_inv^(10) */, 0x14019690CAA17ULL /* psi_inv^(11) */,
            0x0BB16DC173F48ULL /* psi_inv^(12) */, 0x083040C1042D4ULL /* psi_inv^(13) */, 0x0EA46C4C96D3AULL /* psi_inv^(14) */, 0x14A498291A0F4ULL /* psi_inv^(15) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x17043344BCC46ULL /* psi_inv^(2 ) */, 0x009AC91D27C77ULL /* psi_inv^(4 ) */, 0x155BFE0E37D3BULL /* psi_inv^(6 ) */,
            0x0CBBC84E97DA7ULL /* psi_inv^(8 ) */, 0x108347251B3BAULL /* psi_inv^(10) */, 0x0BB16DC173F48ULL /* psi_inv^(12) */, 0x0EA46C4C96D3AULL /* psi_inv^(14) */,
            0x158159F97D27EULL /* psi_inv^(16) */, 0x181BA6DDB5A75ULL /* psi_inv^(18) */, 0x0FE9E29145D8BULL /* psi_inv^(20) */, 0x0F982689AA6AAULL /* psi_inv^(22) */,
            0x12175E38CA79AULL /* psi_inv^(24) */, 0x093DA02E75177ULL /* psi_inv^(26) */, 0x0A2EF95AE2A2DULL /* psi_inv^(28) */, 0x0F2172924773FULL /* psi_inv^(30) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x0770649D55BDBULL /* psi_inv^(3 ) */, 0x155BFE0E37D3BULL /* psi_inv^(6 ) */, 0x184C2B4867079ULL /* psi_inv^(9 ) */,
            0x0BB16DC173F48ULL /* psi_inv^(12) */, 0x14A498291A0F4ULL /* psi_inv^(15) */, 0x181BA6DDB5A75ULL /* psi_inv^(18) */, 0x0250D977CD22BULL /* psi_inv^(21) */,
            0x12175E38CA79AULL /* psi_inv^(24) */, 0x0CEB982E27B95ULL /* psi_inv^(27) */, 0x0F2172924773FULL /* psi_inv^(30) */, 0x034A2852039FFULL /* psi_inv^(33) */,
            0x18E536E2D838AULL /* psi_inv^(36) */, 0x00EF26C0CACEBULL /* psi_inv^(39) */, 0x08FCB8DAE4C47ULL /* psi_inv^(42) */, 0x114FBF3EFBD2DULL /* psi_inv^(45) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x009AC91D27C77ULL /* psi_inv^(4 ) */, 0x0CBBC84E97DA7ULL /* psi_inv^(8 ) */, 0x0BB16DC173F48ULL /* psi_inv^(12) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x0CBBC84E97DA7ULL /* psi_inv^(8 ) */, 0x158159F97D27EULL /* psi_inv^(16) */, 0x12175E38CA79AULL /* psi_inv^(24) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x0BB16DC173F48ULL /* psi_inv^(12) */, 0x12175E38CA79AULL /* psi_inv^(24) */, 0x18E536E2D838AULL /* psi_inv^(36) */
        },

        .cdltu64_negacyclic_fwd =
        {
            0x0000000000001ULL /* psi^(0 ) */, 0x195AE353D36C4ULL /* psi^(1 ) */, 0x08E6082748B5CULL /* psi^(2 ) */, 0x092FE6B416463ULL /* psi^(3 ) */,
            0x0A5E8D6DB88C2ULL /* psi^(4 ) */, 0x0221B2E1AF034ULL /* psi^(5 ) */, 0x1261D70B2CDD9ULL /* psi^(6 ) */, 0x02D959556C11DULL /* psi^(7 ) */,
            0x0F5106A51D5D4ULL /* psi^(8 ) */, 0x10C2FBFD144BFULL /* psi^(9 ) */, 0x0C9467D1D846CULL /* psi^(10) */, 0x18A155F6481B6ULL /* psi^(11) */,
            0x10425FD18AE8AULL /* psi^(12) */, 0x06C9DC463F1BCULL /* psi^(13) */, 0x0BADC46AB7340ULL /* psi^(14) */, 0x120F8A8C3B39FULL /* psi^(15) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x08E6082748B5CULL /* psi^(2 ) */, 0x0A5E8D6DB88C2ULL /* psi^(4 ) */, 0x1261D70B2CDD9ULL /* psi^(6 ) */,
            0x0F5106A51D5D4ULL /* psi^(8 ) */, 0x0C9467D1D846CULL /* psi^(10) */, 0x10425FD18AE8AULL /* psi^(12) */, 0x0BADC46AB7340ULL /* psi^(14) */,
            0x0768A1C735867ULL /* psi^(16) */, 0x0D57F89F5D03BULL /* psi^(18) */, 0x09E7D97655957ULL /* psi^(20) */, 0x172F268832DD6ULL /* psi^(22) */,
            0x09961D6EBA276ULL /* psi^(24) */, 0x01CDA61FC92DFULL /* psi^(26) */, 0x016459224A58CULL /* psi^(28) */, 0x110C0E30AAE98ULL /* psi^(30) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x092FE6B416463ULL /* psi^(3 ) */, 0x1261D70B2CDD9ULL /* psi^(6 ) */, 0x10C2FBFD144BFULL /* psi^(9 ) */,
            0x10425FD18AE8AULL /* psi^(12) */, 0x120F8A8C3B39FULL /* psi^(15) */, 0x0D57F89F5D03BULL /* psi^(18) */, 0x04B47A97836F3ULL /* psi^(21) */,
            0x09961D6EBA276ULL /* psi^(24) */, 0x195571272620FULL /* psi^(27) */, 0x110C0E30AAE98ULL /* psi^(30) */, 0x12883658635BAULL /* psi^(33) */,
            0x0ADB93B3692C7ULL /* psi^(36) */, 0x042D2E3BC1835ULL /* psi^(39) */, 0x057E696F355EAULL /* psi^(42) */, 0x03C55D64D1E04ULL /* psi^(45) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x0A5E8D6DB88C2ULL /* psi^(4 ) */, 0x0F5106A51D5D4ULL /* psi^(8 ) */, 0x10425FD18AE8AULL /* psi^(12) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x0F5106A51D5D4ULL /* psi^(8 ) */, 0x0768A1C735867ULL /* psi^(16) */, 0x09961D6EBA276ULL /* psi^(24) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x10425FD18AE8AULL /* psi^(12) */, 0x09961D6EBA276ULL /* psi^(24) */, 0x0ADB93B3692C7ULL /* psi^(36) */
        },

        .cdltu64_negacyclic_inv =
        {
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x034D195026C62ULL /* psi_inv^(1 ) */, 0x1635D7ADFC602ULL /* psi_inv^(2 ) */, 0x1273EF30EC645ULL /* psi_inv^(3 ) */,
            0x17043344BCC46ULL /* psi_inv^(4 ) */, 0x0A7BFB2B3E22CULL /* psi_inv^(5 ) */, 0x0770649D55BDBULL /* psi_inv^(6 ) */, 0x13C11D945CF8FULL /* psi_inv^(7 ) */,
            0x009AC91D27C77ULL /* psi_inv^(8 ) */, 0x007913EB1711FULL /* psi_inv^(9 ) */, 0x0459AFE66FEF0ULL /* psi_inv^(10) */, 0x088C5B738A7CDULL /* psi_inv^(11) */,
            0x155BFE0E37D3BULL /* psi_inv^(12) */, 0x124EB6A0100FDULL /* psi_inv^(13) */, 0x1890D93F35316ULL /* psi_inv^(14) */, 0x122F38552A422ULL /* psi_inv^(15) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x1635D7ADFC602ULL /* psi_inv^(2 ) */, 0x17043344BCC46ULL /* psi_inv^(4 ) */, 0x0770649D55BDBULL /* psi_inv^(6 ) */,
            0x009AC91D27C77ULL /* psi_inv^(8 ) */, 0x0459AFE66FEF0ULL /* psi_inv^(10) */, 0x155BFE0E37D3BULL /* psi_inv^(12) */, 0x1890D93F35316ULL /* psi_inv^(14) */,
            0x0CBBC84E97DA7ULL /* psi_inv^(16) */, 0x184C2B4867079ULL /* psi_inv^(18) */, 0x108347251B3BAULL /* psi_inv^(20) */, 0x14019690CAA17ULL /* psi_inv^(22) */,
            0x0BB16DC173F48ULL /* psi_inv^(24) */, 0x083040C1042D4ULL /* psi_inv^(26) */, 0x0EA46C4C96D3AULL /* psi_inv^(28) */, 0x14A498291A0F4ULL /* psi_inv^(30) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x1273EF30EC645ULL /* psi_inv^(3 ) */, 0x0770649D55BDBULL /* psi_inv^(6 ) */, 0x007913EB1711FULL /* psi_inv^(9 ) */,
            0x155BFE0E37D3BULL /* psi_inv^(12) */, 0x122F38552A422ULL /* psi_inv^(15) */, 0x184C2B4867079ULL /* psi_inv^(18) */, 0x12732D81CFC6AULL /* psi_inv^(21) */,
            0x0BB16DC173F48ULL /* psi_inv^(24) */, 0x0B9C760ED8810ULL /* psi_inv^(27) */, 0x14A498291A0F4ULL /* psi_inv^(30) */, 0x070A0C6962EC9ULL /* psi_inv^(33) */,
            0x181BA6DDB5A75ULL /* psi_inv^(36) */, 0x00D4837B2B664ULL /* psi_inv^(39) */, 0x0250D977CD22BULL /* psi_inv^(42) */, 0x0BCC08299FB43ULL /* psi_inv^(45) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x17043344BCC46ULL /* psi_inv^(4 ) */, 0x009AC91D27C77ULL /* psi_inv^(8 ) */, 0x155BFE0E37D3BULL /* psi_inv^(12) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x009AC91D27C77ULL /* psi_inv^(8 ) */, 0x0CBBC84E97DA7ULL /* psi_inv^(16) */, 0x0BB16DC173F48ULL /* psi_inv^(24) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x155BFE0E37D3BULL /* psi_inv^(12) */, 0x0BB16DC173F48ULL /* psi_inv^(24) */, 0x181BA6DDB5A75ULL /* psi_inv^(36) */
        }
    },

    {

    /* --------------------------------------------------------------- */
    /* k = 75     g = 11   p = 659706976665601    (0x2580000000001)    */
    /* --------------------------------------------------------------- */

        .k = 75ULL, .g = 11ULL, .p = (75ULL << 43ULL) + 1ULL,

        .magic = 0xDA740DA740DA16D9ULL,

        .shift = 49ULL,

        .prime_inv   = 0X1.B4E81B4E81B43P-50,
        .prime_inv52 = 0x257FFFFFFFFFFULL,
        .r0          = 0x0FDF92C5F92F5ULL,

        .twiddle =
        {
            0x17FDCF46C6A89ULL /* w^(2^0 ) */, 0x034E18223BFACULL /* w^(2^1 ) */, 0x0535788518E65ULL /* w^(2^2 ) */, 0x24C592834E457ULL /* w^(2^3 ) */,
            0x17DDA81ADC559ULL /* w^(2^4 ) */, 0x0A166D7D5C24EULL /* w^(2^5 ) */, 0x1BD9909F2DDF9ULL /* w^(2^6 ) */, 0x08CB8199339E5ULL /* w^(2^7 ) */,
            0x226297720F449ULL /* w^(2^8 ) */, 0x1A24526C138B4ULL /* w^(2^9 ) */, 0x23C1EB20D0BB6ULL /* w^(2^10) */, 0x24A123F0F2AD9ULL /* w^(2^11) */,
            0x0FE09957BE60CULL /* w^(2^12) */, 0x10709E5317950ULL /* w^(2^13) */, 0x068E801E051C7ULL /* w^(2^14) */, 0x1BB8B29283655ULL /* w^(2^15) */,
            0x0EE23B93A9365ULL /* w^(2^16) */, 0x08188A3FB233FULL /* w^(2^17) */, 0x09A3952EC1401ULL /* w^(2^18) */, 0x0B353412D3FE0ULL /* w^(2^19) */,
            0x05016B8A58451ULL /* w^(2^20) */, 0x16C738A2B9FC9ULL /* w^(2^21) */, 0x158E9DE663591ULL /* w^(2^22) */, 0x22961A9613DBFULL /* w^(2^23) */,
            0x0D2D17027BD3FULL /* w^(2^24) */, 0x1468D70AD9F88ULL /* w^(2^25) */, 0x24B6DBFE51830ULL /* w^(2^26) */, 0x218DCFA7A475DULL /* w^(2^27) */,
            0x07D42FA22871EULL /* w^(2^28) */, 0x191E028EAEFFCULL /* w^(2^29) */, 0x2366DF82DC566ULL /* w^(2^30) */, 0x000C3D61C85BCULL /* w^(2^31) */,
            0x1A72B97116985ULL /* w^(2^32) */, 0x141E391770830ULL /* w^(2^33) */, 0x18F0EFE208EDBULL /* w^(2^34) */, 0x0E828E6D2E05CULL /* w^(2^35) */,
            0x1E3C59F3C279BULL /* w^(2^36) */, 0x20FD40FF6E876ULL /* w^(2^37) */, 0x1D47AE4270ADEULL /* w^(2^38) */, 0x0471DB3A7526DULL /* w^(2^39) */,
            0x0B4E5361F273BULL /* w^(2^40) */, 0x2515B8BDD610EULL /* w^(2^41) */, 0x2580000000000ULL /* w^(2^42) */
        },

        .twiddle_inv =
        {
            0x057A2FC022C9CULL /* w_inv^(2^0 ) */, 0x0F87AF955D3E8ULL /* w_inv^(2^1 ) */, 0x2053840C9CCD4ULL /* w_inv^(2^2 ) */, 0x1D46103A6EFFFULL /* w_inv^(2^3 ) */,
            0x2281874F3C664ULL /* w_inv^(2^4 ) */, 0x112A8D0333FBBULL /* w_inv^(2^5 ) */, 0x2458788DA9C69ULL /* w_inv^(2^6 ) */, 0x09DFBB0B53DA9ULL /* w_inv^(2^7 ) */,
            0x09EE5965B87A5ULL /* w_inv^(2^8 ) */, 0x20ACC825345E6ULL /* w_inv^(2^9 ) */, 0x01D1C08F40135ULL /* w_inv^(2^10) */, 0x0B4D412540534ULL /* w_inv^(2^11) */,
            0x03BEF3113997BULL /* w_inv^(2^12) */, 0x1D1EE9663649FULL /* w_inv^(2^13) */, 0x1D45320269558ULL /* w_inv^(2^14) */, 0x1C4AAB741B79AULL /* w_inv^(2^15) */,
            0x20861D0293AA6ULL /* w_inv^(2^16) */, 0x1FC69510CBF7BULL /* w_inv^(2^17) */, 0x0EDE6A1B74E77ULL /* w_inv^(2^18) */, 0x0D172F7EF6B9CULL /* w_inv^(2^19) */,
            0x0F763BA35D51EULL /* w_inv^(2^20) */, 0x13034D9AF125DULL /* w_inv^(2^21) */, 0x05867601AA4CAULL /* w_inv^(2^22) */, 0x003E2E32F93BDULL /* w_inv^(2^23) */,
            0x0408650FD7E9FULL /* w_inv^(2^24) */, 0x239AA7EF14C2CULL /* w_inv^(2^25) */, 0x207FE3A88590DULL /* w_inv^(2^26) */, 0x04A1D0820A21FULL /* w_inv^(2^27) */,
            0x124294687888EULL /* w_inv^(2^28) */, 0x025A778813800ULL /* w_inv^(2^29) */, 0x0D35C8A9720A4ULL /* w_inv^(2^30) */, 0x1B3D3908C7DC7ULL /* w_inv^(2^31) */,
            0x06E10A00ECC93ULL /* w_inv^(2^32) */, 0x24AC1FFE00E06ULL /* w_inv^(2^33) */, 0x06F45AE03231EULL /* w_inv^(2^34) */, 0x0C14FD99A8E60ULL /* w_inv^(2^35) */,
            0x053A6092F09F6ULL /* w_inv^(2^36) */, 0x1EAB6887EB956ULL /* w_inv^(2^37) */, 0x1E099516D8692ULL /* w_inv^(2^38) */, 0x08F92CCCE1DD9ULL /* w_inv^(2^39) */,
            0x1CCAEDF2C6C0FULL /* w_inv^(2^40) */, 0x006A474229EF3ULL /* w_inv^(2^41) */, 0x2580000000000ULL /* w_inv^(2^42) */
        },

        .size_inv =
        {
            0x0000000000001ULL /* N^(2^0 ) */, 0x12C0000000001ULL /* N^(2^1 ) */, 0x1C20000000001ULL /* N^(2^2 ) */, 0x20D0000000001ULL /* N^(2^3 ) */,
            0x2328000000001ULL /* N^(2^4 ) */, 0x2454000000001ULL /* N^(2^5 ) */, 0x24EA000000001ULL /* N^(2^6 ) */, 0x2535000000001ULL /* N^(2^7 ) */,
            0x255A800000001ULL /* N^(2^8 ) */, 0x256D400000001ULL /* N^(2^9 ) */, 0x2576A00000001ULL /* N^(2^10) */, 0x257B500000001ULL /* N^(2^11) */,
            0x257DA80000001ULL /* N^(2^12) */, 0x257ED40000001ULL /* N^(2^13) */, 0x257F6A0000001ULL /* N^(2^14) */, 0x257FB50000001ULL /* N^(2^15) */,
            0x257FDA8000001ULL /* N^(2^16) */, 0x257FED4000001ULL /* N^(2^17) */, 0x257FF6A000001ULL /* N^(2^18) */, 0x257FFB5000001ULL /* N^(2^19) */,
            0x257FFDA800001ULL /* N^(2^20) */, 0x257FFED400001ULL /* N^(2^21) */, 0x257FFF6A00001ULL /* N^(2^22) */, 0x257FFFB500001ULL /* N^(2^23) */,
            0x257FFFDA80001ULL /* N^(2^24) */, 0x257FFFED40001ULL /* N^(2^25) */, 0x257FFFF6A0001ULL /* N^(2^26) */, 0x257FFFFB50001ULL /* N^(2^27) */,
            0x257FFFFDA8001ULL /* N^(2^28) */, 0x257FFFFED4001ULL /* N^(2^29) */, 0x257FFFFF6A001ULL /* N^(2^30) */, 0x257FFFFFB5001ULL /* N^(2^31) */,
            0x257FFFFFDA801ULL /* N^(2^32) */, 0x257FFFFFED401ULL /* N^(2^33) */, 0x257FFFFFF6A01ULL /* N^(2^34) */, 0x257FFFFFFB501ULL /* N^(2^35) */,
            0x257FFFFFFDA81ULL /* N^(2^36) */, 0x257FFFFFFED41ULL /* N^(2^37) */, 0x257FFFFFFF6A1ULL /* N^(2^38) */, 0x257FFFFFFFB51ULL /* N^(2^39) */,
            0x257FFFFFFFDA9ULL /* N^(2^40) */, 0x257FFFFFFFED5ULL /* N^(2^41) */, 0x257FFFFFFFF6BULL /* N^(2^42) */
        },

        .cdltf64_cyclic_fwd =
        {
                           0X1P+0 /* psi^(0 ) */,  0X1.07EA07FB743BP+49 /* psi^(1 ) */,  0X1.D47AE4270ADEP+48 /* psi^(2 ) */,  0X1.EC471E772396P+48 /* psi^(3 ) */,
             0X1.1C76CE9D49B4P+46 /* psi^(4 ) */,  0X1.04E91D21A7D7P+49 /* psi^(5 ) */,   0X1.7D17F386D92P+48 /* psi^(6 ) */,  0X1.B749201BEA86P+48 /* psi^(7 ) */,
             0X1.69CA6C3E4E76P+47 /* psi^(8 ) */,  0X1.6AA879A7FCE1P+48 /* psi^(9 ) */,  0X1.32E2E06AFDEAP+47 /* psi^(10) */,  0X1.ECBA204663BBP+48 /* psi^(11) */,
             0X1.11D9FAFF2174P+47 /* psi^(12) */,  0X1.D937A3ED39FCP+47 /* psi^(13) */,  0X1.F07A240409F8P+48 /* psi^(14) */,  0X1.5C0ABACA248AP+47 /* psi^(15) */,
                           0X1P+0 /* psi^(0 ) */,  0X1.D47AE4270ADEP+48 /* psi^(2 ) */,  0X1.1C76CE9D49B4P+46 /* psi^(4 ) */,   0X1.7D17F386D92P+48 /* psi^(6 ) */,
             0X1.69CA6C3E4E76P+47 /* psi^(8 ) */,  0X1.32E2E06AFDEAP+47 /* psi^(10) */,  0X1.11D9FAFF2174P+47 /* psi^(12) */,  0X1.F07A240409F8P+48 /* psi^(14) */,
             0X1.28ADC5EEB087P+49 /* psi^(16) */,   0X1.1B281CF382AP+45 /* psi^(18) */, 0X1.28A834B4DAFD8P+49 /* psi^(20) */,   0X1.E6C6A8BCC7DP+47 /* psi^(22) */,
             0X1.16A241A727E4P+47 /* psi^(24) */,  0X1.702E776F22EFP+48 /* psi^(26) */,  0X1.C86D3331E228P+48 /* psi^(28) */,  0X1.DD9ABA49E5BCP+46 /* psi^(30) */,
                           0X1P+0 /* psi^(0 ) */,  0X1.EC471E772396P+48 /* psi^(3 ) */,   0X1.7D17F386D92P+48 /* psi^(6 ) */,  0X1.6AA879A7FCE1P+48 /* psi^(9 ) */,
             0X1.11D9FAFF2174P+47 /* psi^(12) */,  0X1.5C0ABACA248AP+47 /* psi^(15) */,   0X1.1B281CF382AP+45 /* psi^(18) */,  0X1.5B02748A0178P+46 /* psi^(21) */,
             0X1.16A241A727E4P+47 /* psi^(24) */,  0X1.0A432FC34024P+49 /* psi^(27) */,  0X1.DD9ABA49E5BCP+46 /* psi^(30) */,  0X1.20AFC0245E2CP+46 /* psi^(33) */,
             0X1.0871262C56CAP+49 /* psi^(36) */,  0X1.416DBFC82AF6P+47 /* psi^(39) */,  0X1.BE8E8FCA810CP+48 /* psi^(42) */,  0X1.6B642E096303P+48 /* psi^(45) */,
                           0X1P+0 /* psi^(0 ) */,  0X1.1C76CE9D49B4P+46 /* psi^(4 ) */,  0X1.69CA6C3E4E76P+47 /* psi^(8 ) */,  0X1.11D9FAFF2174P+47 /* psi^(12) */,
                           0X1P+0 /* psi^(0 ) */,  0X1.69CA6C3E4E76P+47 /* psi^(8 ) */,  0X1.28ADC5EEB087P+49 /* psi^(16) */,  0X1.16A241A727E4P+47 /* psi^(24) */,
                           0X1P+0 /* psi^(0 ) */,  0X1.11D9FAFF2174P+47 /* psi^(12) */,  0X1.16A241A727E4P+47 /* psi^(24) */,  0X1.0871262C56CAP+49 /* psi^(36) */
        },

        .cdltf64_cyclic_inv =
        {
                           0X1P+0 /* psi_inv^(0 ) */,  0X1.EAB6887EB956P+48 /* psi_inv^(1 ) */,  0X1.E099516D8692P+48 /* psi_inv^(2 ) */,  0X1.A17BE1C359FFP+48 /* psi_inv^(3 ) */,
             0X1.1F25999C3BB2P+47 /* psi_inv^(4 ) */,  0X1.0DE681E5FEE4P+46 /* psi_inv^(5 ) */,  0X1.CFA31121BA24P+47 /* psi_inv^(6 ) */,  0X1.903721A3AFE8P+45 /* psi_inv^(7 ) */,
             0X1.CCAEDF2C6C0FP+48 /* psi_inv^(8 ) */,   0X1.32F997D9DD4P+44 /* psi_inv^(9 ) */,  0X1.649CABA19C19P+48 /* psi_inv^(10) */, 0X1.009FB16EBFD18P+49 /* psi_inv^(11) */,
              0X1.ABE5A592818P+42 /* psi_inv^(12) */,  0X1.56E6FC2F7F33P+48 /* psi_inv^(13) */, 0X1.1A4D7E30C7D68P+49 /* psi_inv^(14) */,  0X1.8205F8D679FCP+48 /* psi_inv^(15) */,
                           0X1P+0 /* psi_inv^(0 ) */,  0X1.E099516D8692P+48 /* psi_inv^(2 ) */,  0X1.1F25999C3BB2P+47 /* psi_inv^(4 ) */,  0X1.CFA31121BA24P+47 /* psi_inv^(6 ) */,
             0X1.CCAEDF2C6C0FP+48 /* psi_inv^(8 ) */,  0X1.649CABA19C19P+48 /* psi_inv^(10) */,   0X1.ABE5A592818P+42 /* psi_inv^(12) */, 0X1.1A4D7E30C7D68P+49 /* psi_inv^(14) */,
              0X1.A91D08A7BCCP+42 /* psi_inv^(16) */,  0X1.9E176FEFD824P+46 /* psi_inv^(18) */,  0X1.CF1302806F47P+48 /* psi_inv^(20) */,  0X1.BE8E8FCA810CP+48 /* psi_inv^(22) */,
             0X1.A31AC9E0D8C6P+48 /* psi_inv^(24) */,  0X1.B5D018F24DC2P+47 /* psi_inv^(26) */,  0X1.0871262C56CAP+49 /* psi_inv^(28) */,  0X1.070A37B1EA46P+47 /* psi_inv^(30) */,
                           0X1P+0 /* psi_inv^(0 ) */,  0X1.A17BE1C359FFP+48 /* psi_inv^(3 ) */,  0X1.CFA31121BA24P+47 /* psi_inv^(6 ) */,   0X1.32F997D9DD4P+44 /* psi_inv^(9 ) */,
              0X1.ABE5A592818P+42 /* psi_inv^(12) */,  0X1.8205F8D679FCP+48 /* psi_inv^(15) */,  0X1.9E176FEFD824P+46 /* psi_inv^(18) */,  0X1.AD177EE67118P+46 /* psi_inv^(21) */,
             0X1.A31AC9E0D8C6P+48 /* psi_inv^(24) */,  0X1.38B716F2C14CP+46 /* psi_inv^(27) */,  0X1.070A37B1EA46P+47 /* psi_inv^(30) */,  0X1.B525DE051AACP+46 /* psi_inv^(33) */,
             0X1.C86D3331E228P+48 /* psi_inv^(36) */,  0X1.12FC8DE5C502P+49 /* psi_inv^(39) */,   0X1.E6C6A8BCC7DP+47 /* psi_inv^(42) */,  0X1.011903D080CEP+48 /* psi_inv^(45) */,
                           0X1P+0 /* psi_inv^(0 ) */,  0X1.1F25999C3BB2P+47 /* psi_inv^(4 ) */,  0X1.CCAEDF2C6C0FP+48 /* psi_inv^(8 ) */,   0X1.ABE5A592818P+42 /* psi_inv^(12) */,
                           0X1P+0 /* psi_inv^(0 ) */,  0X1.CCAEDF2C6C0FP+48 /* psi_inv^(8 ) */,   0X1.A91D08A7BCCP+42 /* psi_inv^(16) */,  0X1.A31AC9E0D8C6P+48 /* psi_inv^(24) */,
                           0X1P+0 /* psi_inv^(0 ) */,   0X1.ABE5A592818P+42 /* psi_inv^(12) */,  0X1.A31AC9E0D8C6P+48 /* psi_inv^(24) */,  0X1.C86D3331E228P+48 /* psi_inv^(36) */
        },

        .cdltf64_negacyclic_fwd =
        {
                           0X1P+0 /* psi^(0 ) */,  0X1.E3C59F3C279BP+48 /* psi^(1 ) */,  0X1.07EA07FB743BP+49 /* psi^(2 ) */, 0X1.0FA6AA7D4BE68P+49 /* psi^(3 ) */,
             0X1.D47AE4270ADEP+48 /* psi^(4 ) */, 0X1.1695EC4FBFE58P+49 /* psi^(5 ) */,  0X1.EC471E772396P+48 /* psi^(6 ) */,  0X1.373D591C5713P+48 /* psi^(7 ) */,
             0X1.1C76CE9D49B4P+46 /* psi^(8 ) */,  0X1.C04D44FA897AP+48 /* psi^(9 ) */,  0X1.04E91D21A7D7P+49 /* psi^(10) */,  0X1.AD967D984335P+48 /* psi^(11) */,
              0X1.7D17F386D92P+48 /* psi^(12) */,  0X1.24D4F248A692P+47 /* psi^(13) */,  0X1.B749201BEA86P+48 /* psi^(14) */,  0X1.A34C369F96CAP+48 /* psi^(15) */,
                           0X1P+0 /* psi^(0 ) */,  0X1.07EA07FB743BP+49 /* psi^(2 ) */,  0X1.D47AE4270ADEP+48 /* psi^(4 ) */,  0X1.EC471E772396P+48 /* psi^(6 ) */,
             0X1.1C76CE9D49B4P+46 /* psi^(8 ) */,  0X1.04E91D21A7D7P+49 /* psi^(10) */,   0X1.7D17F386D92P+48 /* psi^(12) */,  0X1.B749201BEA86P+48 /* psi^(14) */,
             0X1.69CA6C3E4E76P+47 /* psi^(16) */,  0X1.6AA879A7FCE1P+48 /* psi^(18) */,  0X1.32E2E06AFDEAP+47 /* psi^(20) */,  0X1.ECBA204663BBP+48 /* psi^(22) */,
             0X1.11D9FAFF2174P+47 /* psi^(24) */,  0X1.D937A3ED39FCP+47 /* psi^(26) */,  0X1.F07A240409F8P+48 /* psi^(28) */,  0X1.5C0ABACA248AP+47 /* psi^(30) */,
                           0X1P+0 /* psi^(0 ) */, 0X1.0FA6AA7D4BE68P+49 /* psi^(3 ) */,  0X1.EC471E772396P+48 /* psi^(6 ) */,  0X1.C04D44FA897AP+48 /* psi^(9 ) */,
              0X1.7D17F386D92P+48 /* psi^(12) */,  0X1.A34C369F96CAP+48 /* psi^(15) */,  0X1.6AA879A7FCE1P+48 /* psi^(18) */, 0X1.0DFDF51BBF588P+49 /* psi^(21) */,
             0X1.11D9FAFF2174P+47 /* psi^(24) */,   0X1.6A6010B8F5CP+43 /* psi^(27) */,  0X1.5C0ABACA248AP+47 /* psi^(30) */,  0X1.FE8490DFF36AP+47 /* psi^(33) */,
              0X1.1B281CF382AP+45 /* psi^(36) */,  0X1.2FD2D49058FBP+48 /* psi^(39) */,  0X1.5B02748A0178P+46 /* psi^(42) */,  0X1.F275681A2F4DP+48 /* psi^(45) */,
                           0X1P+0 /* psi^(0 ) */,  0X1.D47AE4270ADEP+48 /* psi^(4 ) */,  0X1.1C76CE9D49B4P+46 /* psi^(8 ) */,   0X1.7D17F386D92P+48 /* psi^(12) */,
                           0X1P+0 /* psi^(0 ) */,  0X1.1C76CE9D49B4P+46 /* psi^(8 ) */,  0X1.69CA6C3E4E76P+47 /* psi^(16) */,  0X1.11D9FAFF2174P+47 /* psi^(24) */,
                           0X1P+0 /* psi^(0 ) */,   0X1.7D17F386D92P+48 /* psi^(12) */,  0X1.11D9FAFF2174P+47 /* psi^(24) */,   0X1.1B281CF382AP+45 /* psi^(36) */
        },

        .cdltf64_negacyclic_inv =
        {
                           0X1P+0 /* psi_inv^(0 ) */,  0X1.4E9824BC27D8P+46 /* psi_inv^(1 ) */,  0X1.EAB6887EB956P+48 /* psi_inv^(2 ) */,  0X1.BAAC2EB7727AP+47 /* psi_inv^(3 ) */,
             0X1.E099516D8692P+48 /* psi_inv^(4 ) */,  0X1.31B3554DFD3DP+48 /* psi_inv^(5 ) */,  0X1.A17BE1C359FFP+48 /* psi_inv^(6 ) */,  0X1.5F28461617AFP+48 /* psi_inv^(7 ) */,
             0X1.1F25999C3BB2P+47 /* psi_inv^(8 ) */,  0X1.82DA03F1AD5DP+48 /* psi_inv^(9 ) */,  0X1.0DE681E5FEE4P+46 /* psi_inv^(10) */,  0X1.066F1EB76468P+49 /* psi_inv^(11) */,
             0X1.CFA31121BA24P+47 /* psi_inv^(12) */,  0X1.A5BDDB49BFB2P+47 /* psi_inv^(13) */,  0X1.903721A3AFE8P+45 /* psi_inv^(14) */,   0X1.0409D24E32AP+45 /* psi_inv^(15) */,
                           0X1P+0 /* psi_inv^(0 ) */,  0X1.EAB6887EB956P+48 /* psi_inv^(2 ) */,  0X1.E099516D8692P+48 /* psi_inv^(4 ) */,  0X1.A17BE1C359FFP+48 /* psi_inv^(6 ) */,
             0X1.1F25999C3BB2P+47 /* psi_inv^(8 ) */,  0X1.0DE681E5FEE4P+46 /* psi_inv^(10) */,  0X1.CFA31121BA24P+47 /* psi_inv^(12) */,  0X1.903721A3AFE8P+45 /* psi_inv^(14) */,
             0X1.CCAEDF2C6C0FP+48 /* psi_inv^(16) */,   0X1.32F997D9DD4P+44 /* psi_inv^(18) */,  0X1.649CABA19C19P+48 /* psi_inv^(20) */, 0X1.009FB16EBFD18P+49 /* psi_inv^(22) */,
              0X1.ABE5A592818P+42 /* psi_inv^(24) */,  0X1.56E6FC2F7F33P+48 /* psi_inv^(26) */, 0X1.1A4D7E30C7D68P+49 /* psi_inv^(28) */,  0X1.8205F8D679FCP+48 /* psi_inv^(30) */,
                           0X1P+0 /* psi_inv^(0 ) */,  0X1.BAAC2EB7727AP+47 /* psi_inv^(3 ) */,  0X1.A17BE1C359FFP+48 /* psi_inv^(6 ) */,  0X1.82DA03F1AD5DP+48 /* psi_inv^(9 ) */,
             0X1.CFA31121BA24P+47 /* psi_inv^(12) */,   0X1.0409D24E32AP+45 /* psi_inv^(15) */,   0X1.32F997D9DD4P+44 /* psi_inv^(18) */,  0X1.D9E524110B58P+45 /* psi_inv^(21) */,
              0X1.ABE5A592818P+42 /* psi_inv^(24) */,  0X1.9523C9F5A7FCP+46 /* psi_inv^(27) */,  0X1.8205F8D679FCP+48 /* psi_inv^(30) */,   0X1.DB3792D6398P+44 /* psi_inv^(33) */,
             0X1.9E176FEFD824P+46 /* psi_inv^(36) */,  0X1.5A5F64C05E76P+48 /* psi_inv^(39) */,  0X1.AD177EE67118P+46 /* psi_inv^(42) */,  0X1.9049C40837E1P+48 /* psi_inv^(45) */,
                           0X1P+0 /* psi_inv^(0 ) */,  0X1.E099516D8692P+48 /* psi_inv^(4 ) */,  0X1.1F25999C3BB2P+47 /* psi_inv^(8 ) */,  0X1.CFA31121BA24P+47 /* psi_inv^(12) */,
                           0X1P+0 /* psi_inv^(0 ) */,  0X1.1F25999C3BB2P+47 /* psi_inv^(8 ) */,  0X1.CCAEDF2C6C0FP+48 /* psi_inv^(16) */,   0X1.ABE5A592818P+42 /* psi_inv^(24) */,
                           0X1P+0 /* psi_inv^(0 ) */,  0X1.CFA31121BA24P+47 /* psi_inv^(12) */,   0X1.ABE5A592818P+42 /* psi_inv^(24) */,  0X1.9E176FEFD824P+46 /* psi_inv^(36) */
        },

        .cdltu64_cyclic_fwd =
        {
            0x0000000000001ULL /* psi^(0 ) */, 0x20FD40FF6E876ULL /* psi^(1 ) */, 0x1D47AE4270ADEULL /* psi^(2 ) */, 0x1EC471E772396ULL /* psi^(3 ) */,
            0x0471DB3A7526DULL /* psi^(4 ) */, 0x209D23A434FAEULL /* psi^(5 ) */, 0x17D17F386D920ULL /* psi^(6 ) */, 0x1B749201BEA86ULL /* psi^(7 ) */,
            0x0B4E5361F273BULL /* psi^(8 ) */, 0x16AA879A7FCE1ULL /* psi^(9 ) */, 0x0997170357EF5ULL /* psi^(10) */, 0x1ECBA204663BBULL /* psi^(11) */,
            0x088ECFD7F90BAULL /* psi^(12) */, 0x0EC9BD1F69CFEULL /* psi^(13) */, 0x1F07A240409F8ULL /* psi^(14) */, 0x0AE055D651245ULL /* psi^(15) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x1D47AE4270ADEULL /* psi^(2 ) */, 0x0471DB3A7526DULL /* psi^(4 ) */, 0x17D17F386D920ULL /* psi^(6 ) */,
            0x0B4E5361F273BULL /* psi^(8 ) */, 0x0997170357EF5ULL /* psi^(10) */, 0x088ECFD7F90BAULL /* psi^(12) */, 0x1F07A240409F8ULL /* psi^(14) */,
            0x2515B8BDD610EULL /* psi^(16) */, 0x02365039E7054ULL /* psi^(18) */, 0x251506969B5FBULL /* psi^(20) */, 0x0F363545E63E8ULL /* psi^(22) */,
            0x08B5120D393F2ULL /* psi^(24) */, 0x1702E776F22EFULL /* psi^(26) */, 0x1C86D3331E228ULL /* psi^(28) */, 0x07766AE92796FULL /* psi^(30) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x1EC471E772396ULL /* psi^(3 ) */, 0x17D17F386D920ULL /* psi^(6 ) */, 0x16AA879A7FCE1ULL /* psi^(9 ) */,
            0x088ECFD7F90BAULL /* psi^(12) */, 0x0AE055D651245ULL /* psi^(15) */, 0x02365039E7054ULL /* psi^(18) */, 0x056C09D22805EULL /* psi^(21) */,
            0x08B5120D393F2ULL /* psi^(24) */, 0x214865F868048ULL /* psi^(27) */, 0x07766AE92796FULL /* psi^(30) */, 0x0482BF009178BULL /* psi^(33) */,
            0x210E24C58AD94ULL /* psi^(36) */, 0x0A0B6DFE4157BULL /* psi^(39) */, 0x1BE8E8FCA810CULL /* psi^(42) */, 0x16B642E096303ULL /* psi^(45) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x0471DB3A7526DULL /* psi^(4 ) */, 0x0B4E5361F273BULL /* psi^(8 ) */, 0x088ECFD7F90BAULL /* psi^(12) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x0B4E5361F273BULL /* psi^(8 ) */, 0x2515B8BDD610EULL /* psi^(16) */, 0x08B5120D393F2ULL /* psi^(24) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x088ECFD7F90BAULL /* psi^(12) */, 0x08B5120D393F2ULL /* psi^(24) */, 0x210E24C58AD94ULL /* psi^(36) */
        },

        .cdltu64_cyclic_inv =
        {
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x1EAB6887EB956ULL /* psi_inv^(1 ) */, 0x1E099516D8692ULL /* psi_inv^(2 ) */, 0x1A17BE1C359FFULL /* psi_inv^(3 ) */,
            0x08F92CCCE1DD9ULL /* psi_inv^(4 ) */, 0x04379A0797FB9ULL /* psi_inv^(5 ) */, 0x0E7D18890DD12ULL /* psi_inv^(6 ) */, 0x03206E43475FDULL /* psi_inv^(7 ) */,
            0x1CCAEDF2C6C0FULL /* psi_inv^(8 ) */, 0x0132F997D9DD4ULL /* psi_inv^(9 ) */, 0x1649CABA19C19ULL /* psi_inv^(10) */, 0x2013F62DD7FA3ULL /* psi_inv^(11) */,
            0x006AF96964A06ULL /* psi_inv^(12) */, 0x156E6FC2F7F33ULL /* psi_inv^(13) */, 0x2349AFC618FADULL /* psi_inv^(14) */, 0x18205F8D679FCULL /* psi_inv^(15) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x1E099516D8692ULL /* psi_inv^(2 ) */, 0x08F92CCCE1DD9ULL /* psi_inv^(4 ) */, 0x0E7D18890DD12ULL /* psi_inv^(6 ) */,
            0x1CCAEDF2C6C0FULL /* psi_inv^(8 ) */, 0x1649CABA19C19ULL /* psi_inv^(10) */, 0x006AF96964A06ULL /* psi_inv^(12) */, 0x2349AFC618FADULL /* psi_inv^(14) */,
            0x006A474229EF3ULL /* psi_inv^(16) */, 0x06785DBFBF609ULL /* psi_inv^(18) */, 0x1CF1302806F47ULL /* psi_inv^(20) */, 0x1BE8E8FCA810CULL /* psi_inv^(22) */,
            0x1A31AC9E0D8C6ULL /* psi_inv^(24) */, 0x0DAE80C7926E1ULL /* psi_inv^(26) */, 0x210E24C58AD94ULL /* psi_inv^(28) */, 0x083851BD8F523ULL /* psi_inv^(30) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x1A17BE1C359FFULL /* psi_inv^(3 ) */, 0x0E7D18890DD12ULL /* psi_inv^(6 ) */, 0x0132F997D9DD4ULL /* psi_inv^(9 ) */,
            0x006AF96964A06ULL /* psi_inv^(12) */, 0x18205F8D679FCULL /* psi_inv^(15) */, 0x06785DBFBF609ULL /* psi_inv^(18) */, 0x06B45DFB99C46ULL /* psi_inv^(21) */,
            0x1A31AC9E0D8C6ULL /* psi_inv^(24) */, 0x04E2DC5BCB053ULL /* psi_inv^(27) */, 0x083851BD8F523ULL /* psi_inv^(30) */, 0x06D49778146ABULL /* psi_inv^(33) */,
            0x1C86D3331E228ULL /* psi_inv^(36) */, 0x225F91BCB8A04ULL /* psi_inv^(39) */, 0x0F363545E63E8ULL /* psi_inv^(42) */, 0x1011903D080CEULL /* psi_inv^(45) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x08F92CCCE1DD9ULL /* psi_inv^(4 ) */, 0x1CCAEDF2C6C0FULL /* psi_inv^(8 ) */, 0x006AF96964A06ULL /* psi_inv^(12) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x1CCAEDF2C6C0FULL /* psi_inv^(8 ) */, 0x006A474229EF3ULL /* psi_inv^(16) */, 0x1A31AC9E0D8C6ULL /* psi_inv^(24) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x006AF96964A06ULL /* psi_inv^(12) */, 0x1A31AC9E0D8C6ULL /* psi_inv^(24) */, 0x1C86D3331E228ULL /* psi_inv^(36) */
        },

        .cdltu64_negacyclic_fwd =
        {
            0x0000000000001ULL /* psi^(0 ) */, 0x1E3C59F3C279BULL /* psi^(1 ) */, 0x20FD40FF6E876ULL /* psi^(2 ) */, 0x21F4D54FA97CDULL /* psi^(3 ) */,
            0x1D47AE4270ADEULL /* psi^(4 ) */, 0x22D2BD89F7FCBULL /* psi^(5 ) */, 0x1EC471E772396ULL /* psi^(6 ) */, 0x1373D591C5713ULL /* psi^(7 ) */,
            0x0471DB3A7526DULL /* psi^(8 ) */, 0x1C04D44FA897AULL /* psi^(9 ) */, 0x209D23A434FAEULL /* psi^(10) */, 0x1AD967D984335ULL /* psi^(11) */,
            0x17D17F386D920ULL /* psi^(12) */, 0x0926A79245349ULL /* psi^(13) */, 0x1B749201BEA86ULL /* psi^(14) */, 0x1A34C369F96CAULL /* psi^(15) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x20FD40FF6E876ULL /* psi^(2 ) */, 0x1D47AE4270ADEULL /* psi^(4 ) */, 0x1EC471E772396ULL /* psi^(6 ) */,
            0x0471DB3A7526DULL /* psi^(8 ) */, 0x209D23A434FAEULL /* psi^(10) */, 0x17D17F386D920ULL /* psi^(12) */, 0x1B749201BEA86ULL /* psi^(14) */,
            0x0B4E5361F273BULL /* psi^(16) */, 0x16AA879A7FCE1ULL /* psi^(18) */, 0x0997170357EF5ULL /* psi^(20) */, 0x1ECBA204663BBULL /* psi^(22) */,
            0x088ECFD7F90BAULL /* psi^(24) */, 0x0EC9BD1F69CFEULL /* psi^(26) */, 0x1F07A240409F8ULL /* psi^(28) */, 0x0AE055D651245ULL /* psi^(30) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x21F4D54FA97CDULL /* psi^(3 ) */, 0x1EC471E772396ULL /* psi^(6 ) */, 0x1C04D44FA897AULL /* psi^(9 ) */,
            0x17D17F386D920ULL /* psi^(12) */, 0x1A34C369F96CAULL /* psi^(15) */, 0x16AA879A7FCE1ULL /* psi^(18) */, 0x21BFBEA377EB1ULL /* psi^(21) */,
            0x088ECFD7F90BAULL /* psi^(24) */, 0x00B530085C7AEULL /* psi^(27) */, 0x0AE055D651245ULL /* psi^(30) */, 0x0FF42486FF9B5ULL /* psi^(33) */,
            0x02365039E7054ULL /* psi^(36) */, 0x12FD2D49058FBULL /* psi^(39) */, 0x056C09D22805EULL /* psi^(42) */, 0x1F275681A2F4DULL /* psi^(45) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x1D47AE4270ADEULL /* psi^(4 ) */, 0x0471DB3A7526DULL /* psi^(8 ) */, 0x17D17F386D920ULL /* psi^(12) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x0471DB3A7526DULL /* psi^(8 ) */, 0x0B4E5361F273BULL /* psi^(16) */, 0x088ECFD7F90BAULL /* psi^(24) */,
            0x0000000000001ULL /* psi^(0 ) */, 0x17D17F386D920ULL /* psi^(12) */, 0x088ECFD7F90BAULL /* psi^(24) */, 0x02365039E7054ULL /* psi^(36) */
        },

        .cdltu64_negacyclic_inv =
        {
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x053A6092F09F6ULL /* psi_inv^(1 ) */, 0x1EAB6887EB956ULL /* psi_inv^(2 ) */, 0x0DD56175BB93DULL /* psi_inv^(3 ) */,
            0x1E099516D8692ULL /* psi_inv^(4 ) */, 0x131B3554DFD3DULL /* psi_inv^(5 ) */, 0x1A17BE1C359FFULL /* psi_inv^(6 ) */, 0x15F28461617AFULL /* psi_inv^(7 ) */,
            0x08F92CCCE1DD9ULL /* psi_inv^(8 ) */, 0x182DA03F1AD5DULL /* psi_inv^(9 ) */, 0x04379A0797FB9ULL /* psi_inv^(10) */, 0x20CDE3D6EC8D0ULL /* psi_inv^(11) */,
            0x0E7D18890DD12ULL /* psi_inv^(12) */, 0x0D2DEEDA4DFD9ULL /* psi_inv^(13) */, 0x03206E43475FDULL /* psi_inv^(14) */, 0x020813A49C654ULL /* psi_inv^(15) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x1EAB6887EB956ULL /* psi_inv^(2 ) */, 0x1E099516D8692ULL /* psi_inv^(4 ) */, 0x1A17BE1C359FFULL /* psi_inv^(6 ) */,
            0x08F92CCCE1DD9ULL /* psi_inv^(8 ) */, 0x04379A0797FB9ULL /* psi_inv^(10) */, 0x0E7D18890DD12ULL /* psi_inv^(12) */, 0x03206E43475FDULL /* psi_inv^(14) */,
            0x1CCAEDF2C6C0FULL /* psi_inv^(16) */, 0x0132F997D9DD4ULL /* psi_inv^(18) */, 0x1649CABA19C19ULL /* psi_inv^(20) */, 0x2013F62DD7FA3ULL /* psi_inv^(22) */,
            0x006AF96964A06ULL /* psi_inv^(24) */, 0x156E6FC2F7F33ULL /* psi_inv^(26) */, 0x2349AFC618FADULL /* psi_inv^(28) */, 0x18205F8D679FCULL /* psi_inv^(30) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x0DD56175BB93DULL /* psi_inv^(3 ) */, 0x1A17BE1C359FFULL /* psi_inv^(6 ) */, 0x182DA03F1AD5DULL /* psi_inv^(9 ) */,
            0x0E7D18890DD12ULL /* psi_inv^(12) */, 0x020813A49C654ULL /* psi_inv^(15) */, 0x0132F997D9DD4ULL /* psi_inv^(18) */, 0x03B3CA482216BULL /* psi_inv^(21) */,
            0x006AF96964A06ULL /* psi_inv^(24) */, 0x06548F27D69FFULL /* psi_inv^(27) */, 0x18205F8D679FCULL /* psi_inv^(30) */, 0x01DB3792D6398ULL /* psi_inv^(33) */,
            0x06785DBFBF609ULL /* psi_inv^(36) */, 0x15A5F64C05E76ULL /* psi_inv^(39) */, 0x06B45DFB99C46ULL /* psi_inv^(42) */, 0x19049C40837E1ULL /* psi_inv^(45) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x1E099516D8692ULL /* psi_inv^(4 ) */, 0x08F92CCCE1DD9ULL /* psi_inv^(8 ) */, 0x0E7D18890DD12ULL /* psi_inv^(12) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x08F92CCCE1DD9ULL /* psi_inv^(8 ) */, 0x1CCAEDF2C6C0FULL /* psi_inv^(16) */, 0x006AF96964A06ULL /* psi_inv^(24) */,
            0x0000000000001ULL /* psi_inv^(0 ) */, 0x0E7D18890DD12ULL /* psi_inv^(12) */, 0x006AF96964A06ULL /* psi_inv^(24) */, 0x06785DBFBF609ULL /* psi_inv^(36) */
        }
    }
};

#define NTT_PRIME_POW2      (43ULL)      /* Power of 2 in each prime of the table */
#define MIN_CONV_LEN		(1ULL << 6)  /* smallest codelet size is 64 */
#define CRT3_MAX_CONV_LEN   (1ULL << 17) /* using the larger 3 primes */
#define CRT4_MAX_CONV_LEN   (1ULL << 42) /* when using all 4 primes */

#else
    #error "Only 64-bit systems supported!"
#endif

#endif