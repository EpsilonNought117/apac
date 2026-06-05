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
    ap_dig_t prime;
    ap_dig_t coeff;
    ap_dig_t power_of_two;

    ntt_tf_t transform;    

    ap_dig_t    magic;
    ap_dig_t    shift;
    ap_dig_t  barrett;

    double prime_inv;

    ap_dig_t        twiddle[46];
    ap_dig_t    twiddle_inv[46];
    ap_dig_t       size_inv[46];

    double      codelet64_twd[60];
    double  codelet64_twd_inv[60];

} ntt_prime_t;

static const ntt_prime_t NTT_PRIMES[8] =
{
    {
        .prime = (0XA5ULL << 0X2BULL) + 1ULL,

        .coeff = 0XA5ULL,

        .power_of_two = 0X2BULL,

        .transform = CYCLIC,

        .magic = 0XC6980C6980C67189ULL,

        .shift = 0X32ULL,

        .barrett = 0XC6980C6980C67ULL,

        .prime_inv =    0X1.8D3018D3018CEP-51,

        .twiddle =
        {
            0X000072DC94ABF929ULL /* w^(2^0 ) */, 0X000354178906F8B3ULL /* w^(2^1 ) */, 0X0002287E6C11DBE7ULL /* w^(2^2 ) */, 0X0000AAEDF56874D0ULL /* w^(2^3 ) */,
            0X00000D32EF583510ULL /* w^(2^4 ) */, 0X000433969C9A85EDULL /* w^(2^5 ) */, 0X00041BCBA72FEAACULL /* w^(2^6 ) */, 0X0004FBD657823B38ULL /* w^(2^7 ) */,
            0X00000EF953CF0CD5ULL /* w^(2^8 ) */, 0X0004D3F662FBDDAAULL /* w^(2^9 ) */, 0X00049EF4245364CBULL /* w^(2^10) */, 0X00017258D003C59BULL /* w^(2^11) */,
            0X000197D1FCAA32ACULL /* w^(2^12) */, 0X000383C279409E0FULL /* w^(2^13) */, 0X0001E12691C8F9BCULL /* w^(2^14) */, 0X0004DD1C013F4A61ULL /* w^(2^15) */,
            0X0003EEFAB63946DCULL /* w^(2^16) */, 0X0003A31176AEED8EULL /* w^(2^17) */, 0X00023AA6F47D6822ULL /* w^(2^18) */, 0X0001F9547D79D632ULL /* w^(2^19) */,
            0X00016084F26670CBULL /* w^(2^20) */, 0X000381710C866E7CULL /* w^(2^21) */, 0X0003804469436D20ULL /* w^(2^22) */, 0X000521B804209101ULL /* w^(2^23) */,
            0X000328A4EB54F23EULL /* w^(2^24) */, 0X0001F0FBAA3278FEULL /* w^(2^25) */, 0X0002519B51827FE7ULL /* w^(2^26) */, 0X0002AC3E1D12D683ULL /* w^(2^27) */,
            0X0001DAC0746CAE76ULL /* w^(2^28) */, 0X0000B27EBFBA2F11ULL /* w^(2^29) */, 0X0002EA3CC989C09FULL /* w^(2^30) */, 0X00039B4250DDACDFULL /* w^(2^31) */,
            0X00040337F47D515BULL /* w^(2^32) */, 0X0003E8276FCCCBABULL /* w^(2^33) */, 0X00038CBE5DDF24EEULL /* w^(2^34) */, 0X0004AB003A1BCBCBULL /* w^(2^35) */,
            0X0001CB85E443F6B4ULL /* w^(2^36) */, 0X0000D08B2C5C4F60ULL /* w^(2^37) */, 0X0004E5DFA28E94B3ULL /* w^(2^38) */, 0X0004FA6689B300F3ULL /* w^(2^39) */,
            0X00037B7213CA6EC8ULL /* w^(2^40) */, 0X00016C9DDBC9BEC8ULL /* w^(2^41) */, 0X0005280000000000ULL /* w^(2^42) */, 0X0000000000000000ULL /* w^(2^43) */,
            0X0000000000000000ULL /* w^(2^44) */, 0X0000000000000000ULL /* w^(2^45) */
        },

        .twiddle_inv =
        {
            0X0002077AFA98C970ULL /* w_inv^(2^0 ) */, 0X000446EE1DC0094EULL /* w_inv^(2^1 ) */, 0X000052F51E487D43ULL /* w_inv^(2^2 ) */, 0X00011AE9B8CD3AC5ULL /* w_inv^(2^3 ) */,
            0X00000AF980952DBDULL /* w_inv^(2^4 ) */, 0X0003023DEA679521ULL /* w_inv^(2^5 ) */, 0X00026AD0C1B7C67BULL /* w_inv^(2^6 ) */, 0X000356F91DE9C05CULL /* w_inv^(2^7 ) */,
            0X0004D436EF0A205EULL /* w_inv^(2^8 ) */, 0X00047ABEBD33FF9CULL /* w_inv^(2^9 ) */, 0X0004F1512D08B269ULL /* w_inv^(2^10) */, 0X0002CA9046ECE0FDULL /* w_inv^(2^11) */,
            0X000314084893AFBBULL /* w_inv^(2^12) */, 0X00037D4914B82E04ULL /* w_inv^(2^13) */, 0X00040D7721EAB843ULL /* w_inv^(2^14) */, 0X00017F990228C3C4ULL /* w_inv^(2^15) */,
            0X00039A5B300FCE29ULL /* w_inv^(2^16) */, 0X0000C6EEFEFCEBD4ULL /* w_inv^(2^17) */, 0X0000D60EC1409521ULL /* w_inv^(2^18) */, 0X0000A20C3A734640ULL /* w_inv^(2^19) */,
            0X000081D87171D72AULL /* w_inv^(2^20) */, 0X000358ACF1F723B9ULL /* w_inv^(2^21) */, 0X0001CC758121E88EULL /* w_inv^(2^22) */, 0X00010209605FC98EULL /* w_inv^(2^23) */,
            0X0004AC8CA9F06A54ULL /* w_inv^(2^24) */, 0X0003A565793211B9ULL /* w_inv^(2^25) */, 0X000120803974D1DDULL /* w_inv^(2^26) */, 0X000120B52A1AAC96ULL /* w_inv^(2^27) */,
            0X0004585F784F7739ULL /* w_inv^(2^28) */, 0X0004AB56C387A78BULL /* w_inv^(2^29) */, 0X00017A1078A4A369ULL /* w_inv^(2^30) */, 0X00050FEBF189C9CCULL /* w_inv^(2^31) */,
            0X0003BC5B5FCC3C07ULL /* w_inv^(2^32) */, 0X000247925897DD43ULL /* w_inv^(2^33) */, 0X000422A1A075E6C8ULL /* w_inv^(2^34) */, 0X00036D52D0C14518ULL /* w_inv^(2^35) */,
            0X00044675C6D4AA9BULL /* w_inv^(2^36) */, 0X000092CBDC9BAF31ULL /* w_inv^(2^37) */, 0X0001FF2C99E2E9BDULL /* w_inv^(2^38) */, 0X00034D89416267D7ULL /* w_inv^(2^39) */,
            0X0003B30F7C904D8CULL /* w_inv^(2^40) */, 0X0003BB6224364139ULL /* w_inv^(2^41) */, 0X0005280000000000ULL /* w_inv^(2^42) */, 0X0000000000000000ULL /* w_inv^(2^43) */,
            0X0000000000000000ULL /* w_inv^(2^44) */, 0X0000000000000000ULL /* w_inv^(2^45) */
        },

        .size_inv =
        {
            0X0000000000000001ULL /* N^(2^0 ) */, 0X0002940000000001ULL /* N^(2^1 ) */, 0X0003DE0000000001ULL /* N^(2^2 ) */, 0X0004830000000001ULL /* N^(2^3 ) */,
            0X0004D58000000001ULL /* N^(2^4 ) */, 0X0004FEC000000001ULL /* N^(2^5 ) */, 0X0005136000000001ULL /* N^(2^6 ) */, 0X00051DB000000001ULL /* N^(2^7 ) */,
            0X000522D800000001ULL /* N^(2^8 ) */, 0X0005256C00000001ULL /* N^(2^9 ) */, 0X000526B600000001ULL /* N^(2^10) */, 0X0005275B00000001ULL /* N^(2^11) */,
            0X000527AD80000001ULL /* N^(2^12) */, 0X000527D6C0000001ULL /* N^(2^13) */, 0X000527EB60000001ULL /* N^(2^14) */, 0X000527F5B0000001ULL /* N^(2^15) */,
            0X000527FAD8000001ULL /* N^(2^16) */, 0X000527FD6C000001ULL /* N^(2^17) */, 0X000527FEB6000001ULL /* N^(2^18) */, 0X000527FF5B000001ULL /* N^(2^19) */,
            0X000527FFAD800001ULL /* N^(2^20) */, 0X000527FFD6C00001ULL /* N^(2^21) */, 0X000527FFEB600001ULL /* N^(2^22) */, 0X000527FFF5B00001ULL /* N^(2^23) */,
            0X000527FFFAD80001ULL /* N^(2^24) */, 0X000527FFFD6C0001ULL /* N^(2^25) */, 0X000527FFFEB60001ULL /* N^(2^26) */, 0X000527FFFF5B0001ULL /* N^(2^27) */,
            0X000527FFFFAD8001ULL /* N^(2^28) */, 0X000527FFFFD6C001ULL /* N^(2^29) */, 0X000527FFFFEB6001ULL /* N^(2^30) */, 0X000527FFFFF5B001ULL /* N^(2^31) */,
            0X000527FFFFFAD801ULL /* N^(2^32) */, 0X000527FFFFFD6C01ULL /* N^(2^33) */, 0X000527FFFFFEB601ULL /* N^(2^34) */, 0X000527FFFFFF5B01ULL /* N^(2^35) */,
            0X000527FFFFFFAD81ULL /* N^(2^36) */, 0X000527FFFFFFD6C1ULL /* N^(2^37) */, 0X000527FFFFFFEB61ULL /* N^(2^38) */, 0X000527FFFFFFF5B1ULL /* N^(2^39) */,
            0X000527FFFFFFFAD9ULL /* N^(2^40) */, 0X000527FFFFFFFD6DULL /* N^(2^41) */, 0X000527FFFFFFFEB7ULL /* N^(2^42) */, 0X0000000000000000ULL /* N^(2^43) */,
            0X0000000000000000ULL /* N^(2^44) */, 0X0000000000000000ULL /* N^(2^45) */
        },

        .codelet64_twd =
        {
                          0X1P+0 /* psi^(0 ) */,      0X1.A11658B89ECP+47 /* psi^(1 ) */,    0X1.3977E8A3A52CCP+50 /* psi^(2 ) */,    0X1.5975BB11B6A08P+49 /* psi^(3 ) */,
           0X1.3E99A26CC03CCP+50 /* psi^(4 ) */,     0X1.BE0A128E9865P+48 /* psi^(5 ) */,    0X1.2B5406AD555DCP+50 /* psi^(6 ) */,    0X1.FE4B835CCF908P+49 /* psi^(7 ) */,
            0X1.BDB909E53764P+49 /* psi^(8 ) */,    0X1.94375B4BECF88P+49 /* psi^(9 ) */,     0X1.82072A329412P+48 /* psi^(10) */,     0X1.1A3DCE66D78CP+47 /* psi^(11) */,
            0X1.18135D271348P+49 /* psi^(12) */,     0X1.646B5E0AC1A9P+48 /* psi^(13) */,    0X1.FAF1741ACB4A8P+49 /* psi^(14) */,    0X1.1BA6F85E5FDBCP+50 /* psi^(15) */,
                          0X1P+0 /* psi^(0 ) */,    0X1.3977E8A3A52CCP+50 /* psi^(2 ) */,    0X1.3E99A26CC03CCP+50 /* psi^(4 ) */,    0X1.2B5406AD555DCP+50 /* psi^(6 ) */,
            0X1.BDB909E53764P+49 /* psi^(8 ) */,     0X1.82072A329412P+48 /* psi^(10) */,     0X1.18135D271348P+49 /* psi^(12) */,    0X1.FAF1741ACB4A8P+49 /* psi^(14) */,
            0X1.6C9DDBC9BEC8P+48 /* psi^(16) */,     0X1.CBF8EFF8A449P+49 /* psi^(18) */,    0X1.1D55D921486A8P+49 /* psi^(20) */,     0X1.331290F21376P+49 /* psi^(22) */,
            0X1.74F0836FB275P+48 /* psi^(24) */,    0X1.48DA616537128P+49 /* psi^(26) */,     0X1.DA76BE9D982AP+48 /* psi^(28) */,     0X1.9469B30E8B22P+49 /* psi^(30) */,
                          0X1P+0 /* psi^(0 ) */,    0X1.5975BB11B6A08P+49 /* psi^(3 ) */,    0X1.2B5406AD555DCP+50 /* psi^(6 ) */,    0X1.94375B4BECF88P+49 /* psi^(9 ) */,
            0X1.18135D271348P+49 /* psi^(12) */,    0X1.1BA6F85E5FDBCP+50 /* psi^(15) */,     0X1.CBF8EFF8A449P+49 /* psi^(18) */,    0X1.AF5A2F5191BE8P+49 /* psi^(21) */,
            0X1.74F0836FB275P+48 /* psi^(24) */,     0X1.137069735949P+50 /* psi^(27) */,     0X1.9469B30E8B22P+49 /* psi^(30) */,    0X1.15DD34E8EC284P+50 /* psi^(33) */,
             0X1.6CCBB267F87P+45 /* psi^(36) */,      0X1.2B68F94660EP+48 /* psi^(39) */,    0X1.D2FC6AE6B5F78P+49 /* psi^(42) */,     0X1.E1CA50FA9F2CP+49 /* psi^(45) */,
                          0X1P+0 /* psi^(0 ) */,    0X1.3E99A26CC03CCP+50 /* psi^(4 ) */,     0X1.BDB909E53764P+49 /* psi^(8 ) */,     0X1.18135D271348P+49 /* psi^(12) */,
                          0X1P+0 /* psi^(0 ) */,     0X1.BDB909E53764P+49 /* psi^(8 ) */,     0X1.6C9DDBC9BEC8P+48 /* psi^(16) */,     0X1.74F0836FB275P+48 /* psi^(24) */,
                          0X1P+0 /* psi^(0 ) */,     0X1.18135D271348P+49 /* psi^(12) */,     0X1.74F0836FB275P+48 /* psi^(24) */,      0X1.6CCBB267F87P+45 /* psi^(36) */
        },

        .codelet64_twd_inv =
        {
                          0X1P+0 /* psi_inv^(0 ) */,     0X1.2597B9375E62P+47 /* psi_inv^(1 ) */,     0X1.FF2C99E2E9BDP+48 /* psi_inv^(2 ) */,     0X1.5201CBD8EB74P+49 /* psi_inv^(3 ) */,
           0X1.A6C4A0B133EB8P+49 /* psi_inv^(4 ) */,     0X1.B47CB46535BAP+47 /* psi_inv^(5 ) */,     0X1.4B259E9AC8EEP+49 /* psi_inv^(6 ) */,     0X1.59E9BCB3A663P+49 /* psi_inv^(7 ) */,
            0X1.D987BE4826C6P+49 /* psi_inv^(8 ) */,     0X1.3DA4A6A1762AP+47 /* psi_inv^(9 ) */,    0X1.60ED6F0DEC8A8P+49 /* psi_inv^(10) */,     0X1.C94BA15CDC84P+48 /* psi_inv^(11) */,
            0X1.76AA26DEB796P+49 /* psi_inv^(12) */,    0X1.44049993EAD1CP+50 /* psi_inv^(13) */,     0X1.900E200EB76FP+48 /* psi_inv^(14) */,     0X1.6FB23E089D69P+48 /* psi_inv^(15) */,
                          0X1P+0 /* psi_inv^(0 ) */,     0X1.FF2C99E2E9BDP+48 /* psi_inv^(2 ) */,    0X1.A6C4A0B133EB8P+49 /* psi_inv^(4 ) */,     0X1.4B259E9AC8EEP+49 /* psi_inv^(6 ) */,
            0X1.D987BE4826C6P+49 /* psi_inv^(8 ) */,    0X1.60ED6F0DEC8A8P+49 /* psi_inv^(10) */,     0X1.76AA26DEB796P+49 /* psi_inv^(12) */,     0X1.900E200EB76FP+48 /* psi_inv^(14) */,
           0X1.DDB1121B209C8P+49 /* psi_inv^(16) */,     0X1.321D17CA696CP+48 /* psi_inv^(18) */,    0X1.7BECA2D8ECB88P+49 /* psi_inv^(20) */,    0X1.D2FC6AE6B5F78P+49 /* psi_inv^(22) */,
            0X1.AC8DEC359139P+48 /* psi_inv^(24) */,     0X1.EABF952AAA28P+46 /* psi_inv^(26) */,      0X1.6CCBB267F87P+45 /* psi_inv^(28) */,     0X1.088175C5AD38P+46 /* psi_inv^(30) */,
                          0X1P+0 /* psi_inv^(0 ) */,     0X1.5201CBD8EB74P+49 /* psi_inv^(3 ) */,     0X1.4B259E9AC8EEP+49 /* psi_inv^(6 ) */,     0X1.3DA4A6A1762AP+47 /* psi_inv^(9 ) */,
            0X1.76AA26DEB796P+49 /* psi_inv^(12) */,     0X1.6FB23E089D69P+48 /* psi_inv^(15) */,     0X1.321D17CA696CP+48 /* psi_inv^(18) */,    0X1.26B84633250ECP+50 /* psi_inv^(21) */,
            0X1.AC8DEC359139P+48 /* psi_inv^(24) */,     0X1.B4FAF6B8B3CEP+49 /* psi_inv^(27) */,     0X1.088175C5AD38P+46 /* psi_inv^(30) */,     0X1.254D08D91434P+50 /* psi_inv^(33) */,
            0X1.DA76BE9D982AP+48 /* psi_inv^(36) */,    0X1.3A16434C599D8P+49 /* psi_inv^(39) */,     0X1.331290F21376P+49 /* psi_inv^(42) */,      0X1.7ED99B054BAP+44 /* psi_inv^(45) */,
                          0X1P+0 /* psi_inv^(0 ) */,    0X1.A6C4A0B133EB8P+49 /* psi_inv^(4 ) */,     0X1.D987BE4826C6P+49 /* psi_inv^(8 ) */,     0X1.76AA26DEB796P+49 /* psi_inv^(12) */,
                          0X1P+0 /* psi_inv^(0 ) */,     0X1.D987BE4826C6P+49 /* psi_inv^(8 ) */,    0X1.DDB1121B209C8P+49 /* psi_inv^(16) */,     0X1.AC8DEC359139P+48 /* psi_inv^(24) */,
                          0X1P+0 /* psi_inv^(0 ) */,     0X1.76AA26DEB796P+49 /* psi_inv^(12) */,     0X1.AC8DEC359139P+48 /* psi_inv^(24) */,     0X1.DA76BE9D982AP+48 /* psi_inv^(36) */
        },

    },

    {
        .prime = (0X43ULL << 0X2CULL) + 1ULL,

        .coeff = 0X43ULL,

        .power_of_two = 0X2CULL,

        .transform = CYCLIC,

        .magic = 0XF4898D5F85BAFEEBULL,

        .shift = 0X32ULL,

        .barrett = 0XF4898D5F85BAFULL,

        .prime_inv =     0X1.E9131ABF0B76P-51,

        .twiddle =
        {
            0X00008AFF04DF6B7AULL /* w^(2^0 ) */, 0X0000515E846F21D4ULL /* w^(2^1 ) */, 0X0002C4A3DB19EE20ULL /* w^(2^2 ) */, 0X0003A859C0FE850AULL /* w^(2^3 ) */,
            0X000373B18CECD730ULL /* w^(2^4 ) */, 0X00034E62F6066F53ULL /* w^(2^5 ) */, 0X00013167700B3707ULL /* w^(2^6 ) */, 0X00010F44139E3ABFULL /* w^(2^7 ) */,
            0X00021158555CDA0BULL /* w^(2^8 ) */, 0X000252A5D6B8662AULL /* w^(2^9 ) */, 0X0003CFEA02E8C96DULL /* w^(2^10) */, 0X00031FA69B9CF147ULL /* w^(2^11) */,
            0X000241F87E216BACULL /* w^(2^12) */, 0X000157FD1715F179ULL /* w^(2^13) */, 0X00021C0DC5CFE4DCULL /* w^(2^14) */, 0X00027D54CDED5513ULL /* w^(2^15) */,
            0X00025DD630AB4AB1ULL /* w^(2^16) */, 0X000172285B91470FULL /* w^(2^17) */, 0X0003FBD5D5C76618ULL /* w^(2^18) */, 0X000004EE52631CBDULL /* w^(2^19) */,
            0X00006C2E82F8FD66ULL /* w^(2^20) */, 0X00039944B8754BBDULL /* w^(2^21) */, 0X0001F14AE41B5D14ULL /* w^(2^22) */, 0X00020197B464CFA9ULL /* w^(2^23) */,
            0X00038106F10B605DULL /* w^(2^24) */, 0X0002EBAF7984619AULL /* w^(2^25) */, 0X00038BC7D4B18B54ULL /* w^(2^26) */, 0X00040DC54D1F16E9ULL /* w^(2^27) */,
            0X00036786528AB7CAULL /* w^(2^28) */, 0X000257D5BAC457B4ULL /* w^(2^29) */, 0X0001CFED231DB6B5ULL /* w^(2^30) */, 0X00028BC4C0F186CAULL /* w^(2^31) */,
            0X00011318BDFACD2DULL /* w^(2^32) */, 0X0003C8816C783477ULL /* w^(2^33) */, 0X0003C9A2E98AEFD5ULL /* w^(2^34) */, 0X0000FA8110B44F3AULL /* w^(2^35) */,
            0X00001ED1203E6625ULL /* w^(2^36) */, 0X00011C46188DE9F9ULL /* w^(2^37) */, 0X00014B53F73902BDULL /* w^(2^38) */, 0X00025E213B5D849AULL /* w^(2^39) */,
            0X00019E44DEEF16C1ULL /* w^(2^40) */, 0X00032D32A0B2E7D6ULL /* w^(2^41) */, 0X0004218A929A59B2ULL /* w^(2^42) */, 0X0004300000000000ULL /* w^(2^43) */,
            0X0000000000000000ULL /* w^(2^44) */, 0X0000000000000000ULL /* w^(2^45) */
        },

        .twiddle_inv =
        {
            0X0001001595525797ULL /* w_inv^(2^0 ) */, 0X0001E60B4DF73300ULL /* w_inv^(2^1 ) */, 0X0002A9023036C2B8ULL /* w_inv^(2^2 ) */, 0X0001AEFC0CB5A0EFULL /* w_inv^(2^3 ) */,
            0X00010535B654718EULL /* w_inv^(2^4 ) */, 0X00040E76C1B1B9F7ULL /* w_inv^(2^5 ) */, 0X0001FB8305482CB1ULL /* w_inv^(2^6 ) */, 0X0001F8F940D52262ULL /* w_inv^(2^7 ) */,
            0X0000276818D6DBBFULL /* w_inv^(2^8 ) */, 0X0001F9451988028DULL /* w_inv^(2^9 ) */, 0X0001D49DA2D9FFC5ULL /* w_inv^(2^10) */, 0X00017420F7FEBFB7ULL /* w_inv^(2^11) */,
            0X0002B8065131F8DBULL /* w_inv^(2^12) */, 0X00039F9628F68925ULL /* w_inv^(2^13) */, 0X00041302BF821E3CULL /* w_inv^(2^14) */, 0X000045621193D58DULL /* w_inv^(2^15) */,
            0X0003B5320EAFC005ULL /* w_inv^(2^16) */, 0X00018D271A95AD34ULL /* w_inv^(2^17) */, 0X00033966B36013A0ULL /* w_inv^(2^18) */, 0X000417790C4A8821ULL /* w_inv^(2^19) */,
            0X00014026519BCA94ULL /* w_inv^(2^20) */, 0X00021953FA725C52ULL /* w_inv^(2^21) */, 0X0002533E1C0E3B59ULL /* w_inv^(2^22) */, 0X0001C85DC50C87D9ULL /* w_inv^(2^23) */,
            0X00037B12D4528CB2ULL /* w_inv^(2^24) */, 0X0001D3FE6C6843DFULL /* w_inv^(2^25) */, 0X0001BFF0826F82D7ULL /* w_inv^(2^26) */, 0X0003EB68B75B094CULL /* w_inv^(2^27) */,
            0X0001023551D696A7ULL /* w_inv^(2^28) */, 0X00012E2A57845CF6ULL /* w_inv^(2^29) */, 0X00036F2B47260102ULL /* w_inv^(2^30) */, 0X00025BD92546E054ULL /* w_inv^(2^31) */,
            0X0000A2BB44972412ULL /* w_inv^(2^32) */, 0X0001C5FD2EC855D8ULL /* w_inv^(2^33) */, 0X0001984C85EFB75EULL /* w_inv^(2^34) */, 0X0002094CB2D10B15ULL /* w_inv^(2^35) */,
            0X00001F13FEB8FBA2ULL /* w_inv^(2^36) */, 0X00032E2581B504B4ULL /* w_inv^(2^37) */, 0X00024074362CF229ULL /* w_inv^(2^38) */, 0X00025D3A694D073FULL /* w_inv^(2^39) */,
            0X00013FF8241FC4BDULL /* w_inv^(2^40) */, 0X000337F55ACE9816ULL /* w_inv^(2^41) */, 0X00000E756D65A64FULL /* w_inv^(2^42) */, 0X0004300000000000ULL /* w_inv^(2^43) */,
            0X0000000000000000ULL /* w_inv^(2^44) */, 0X0000000000000000ULL /* w_inv^(2^45) */
        },

        .size_inv =
        {
            0X0000000000000001ULL /* N^(2^0 ) */, 0X0002180000000001ULL /* N^(2^1 ) */, 0X0003240000000001ULL /* N^(2^2 ) */, 0X0003AA0000000001ULL /* N^(2^3 ) */,
            0X0003ED0000000001ULL /* N^(2^4 ) */, 0X00040E8000000001ULL /* N^(2^5 ) */, 0X00041F4000000001ULL /* N^(2^6 ) */, 0X000427A000000001ULL /* N^(2^7 ) */,
            0X00042BD000000001ULL /* N^(2^8 ) */, 0X00042DE800000001ULL /* N^(2^9 ) */, 0X00042EF400000001ULL /* N^(2^10) */, 0X00042F7A00000001ULL /* N^(2^11) */,
            0X00042FBD00000001ULL /* N^(2^12) */, 0X00042FDE80000001ULL /* N^(2^13) */, 0X00042FEF40000001ULL /* N^(2^14) */, 0X00042FF7A0000001ULL /* N^(2^15) */,
            0X00042FFBD0000001ULL /* N^(2^16) */, 0X00042FFDE8000001ULL /* N^(2^17) */, 0X00042FFEF4000001ULL /* N^(2^18) */, 0X00042FFF7A000001ULL /* N^(2^19) */,
            0X00042FFFBD000001ULL /* N^(2^20) */, 0X00042FFFDE800001ULL /* N^(2^21) */, 0X00042FFFEF400001ULL /* N^(2^22) */, 0X00042FFFF7A00001ULL /* N^(2^23) */,
            0X00042FFFFBD00001ULL /* N^(2^24) */, 0X00042FFFFDE80001ULL /* N^(2^25) */, 0X00042FFFFEF40001ULL /* N^(2^26) */, 0X00042FFFFF7A0001ULL /* N^(2^27) */,
            0X00042FFFFFBD0001ULL /* N^(2^28) */, 0X00042FFFFFDE8001ULL /* N^(2^29) */, 0X00042FFFFFEF4001ULL /* N^(2^30) */, 0X00042FFFFFF7A001ULL /* N^(2^31) */,
            0X00042FFFFFFBD001ULL /* N^(2^32) */, 0X00042FFFFFFDE801ULL /* N^(2^33) */, 0X00042FFFFFFEF401ULL /* N^(2^34) */, 0X00042FFFFFFF7A01ULL /* N^(2^35) */,
            0X00042FFFFFFFBD01ULL /* N^(2^36) */, 0X00042FFFFFFFDE81ULL /* N^(2^37) */, 0X00042FFFFFFFEF41ULL /* N^(2^38) */, 0X00042FFFFFFFF7A1ULL /* N^(2^39) */,
            0X00042FFFFFFFFBD1ULL /* N^(2^40) */, 0X00042FFFFFFFFDE9ULL /* N^(2^41) */, 0X00042FFFFFFFFEF5ULL /* N^(2^42) */, 0X00042FFFFFFFFF7BULL /* N^(2^43) */,
            0X0000000000000000ULL /* N^(2^44) */, 0X0000000000000000ULL /* N^(2^45) */
        },

        .codelet64_twd =
        {
                          0X1P+0 /* psi^(0 ) */,     0X1.4B53F73902BDP+48 /* psi^(1 ) */,     0X1.2F109DAEC24DP+49 /* psi^(2 ) */,     0X1.E459CC59FD89P+48 /* psi^(3 ) */,
            0X1.9E44DEEF16C1P+48 /* psi^(4 ) */,     0X1.C04F7C831D1BP+48 /* psi^(5 ) */,    0X1.310FE1BBD7F18P+49 /* psi^(6 ) */,     0X1.6C5A84632CACP+48 /* psi^(7 ) */,
            0X1.9699505973EBP+49 /* psi^(8 ) */,    0X1.ED1933D653D68P+49 /* psi^(9 ) */,    0X1.A143978C81B28P+49 /* psi^(10) */,     0X1.84EA5C7B45CAP+49 /* psi^(11) */,
           0X1.5BD17AA1DE3F8P+49 /* psi^(12) */,    0X1.65AC97D21F758P+49 /* psi^(13) */,     0X1.49969EBDC11FP+48 /* psi^(14) */,     0X1.059A4466CB29P+49 /* psi^(15) */,
                          0X1P+0 /* psi^(0 ) */,     0X1.2F109DAEC24DP+49 /* psi^(2 ) */,     0X1.9E44DEEF16C1P+48 /* psi^(4 ) */,    0X1.310FE1BBD7F18P+49 /* psi^(6 ) */,
            0X1.9699505973EBP+49 /* psi^(8 ) */,    0X1.A143978C81B28P+49 /* psi^(10) */,    0X1.5BD17AA1DE3F8P+49 /* psi^(12) */,     0X1.49969EBDC11FP+48 /* psi^(14) */,
           0X1.0862A4A6966C8P+50 /* psi^(16) */,     0X1.8BB00372A7CCP+48 /* psi^(18) */,      0X1.08D6A37BF5DP+44 /* psi^(20) */,     0X1.4B695690130DP+49 /* psi^(22) */,
            0X1.F0154A62CFD6P+47 /* psi^(24) */,    0X1.0321986F08834P+50 /* psi^(26) */,     0X1.7803EDF01DA2P+49 /* psi^(28) */,     0X1.D2C596B2F8C2P+48 /* psi^(30) */,
                          0X1P+0 /* psi^(0 ) */,     0X1.E459CC59FD89P+48 /* psi^(3 ) */,    0X1.310FE1BBD7F18P+49 /* psi^(6 ) */,    0X1.ED1933D653D68P+49 /* psi^(9 ) */,
           0X1.5BD17AA1DE3F8P+49 /* psi^(12) */,     0X1.059A4466CB29P+49 /* psi^(15) */,     0X1.8BB00372A7CCP+48 /* psi^(18) */,    0X1.8428C1F907CC8P+49 /* psi^(21) */,
            0X1.F0154A62CFD6P+47 /* psi^(24) */,     0X1.AE48D7520DEDP+49 /* psi^(27) */,     0X1.D2C596B2F8C2P+48 /* psi^(30) */,     0X1.725604637EA2P+49 /* psi^(33) */,
             0X1.48DD908874AP+49 /* psi^(36) */,    0X1.61D2BDCE69AA8P+49 /* psi^(39) */,     0X1.DAF1A1CDF938P+47 /* psi^(42) */,     0X1.64A6D05BC116P+48 /* psi^(45) */,
                          0X1P+0 /* psi^(0 ) */,     0X1.9E44DEEF16C1P+48 /* psi^(4 ) */,     0X1.9699505973EBP+49 /* psi^(8 ) */,    0X1.5BD17AA1DE3F8P+49 /* psi^(12) */,
                          0X1P+0 /* psi^(0 ) */,     0X1.9699505973EBP+49 /* psi^(8 ) */,    0X1.0862A4A6966C8P+50 /* psi^(16) */,     0X1.F0154A62CFD6P+47 /* psi^(24) */,
                          0X1P+0 /* psi^(0 ) */,    0X1.5BD17AA1DE3F8P+49 /* psi^(12) */,     0X1.F0154A62CFD6P+47 /* psi^(24) */,      0X1.48DD908874AP+49 /* psi^(36) */
        },

        .codelet64_twd_inv =
        {
                          0X1P+0 /* psi_inv^(0 ) */,    0X1.203A1B1679148P+49 /* psi_inv^(1 ) */,    0X1.2E9D34A6839F8P+49 /* psi_inv^(2 ) */,     0X1.51C21D594D12P+48 /* psi_inv^(3 ) */,
            0X1.3FF8241FC4BDP+48 /* psi_inv^(4 ) */,     0X1.A6DCA2B7C84EP+47 /* psi_inv^(5 ) */,      0X1.1BCCF21EEFAP+45 /* psi_inv^(6 ) */,    0X1.BD074C995B798P+49 /* psi_inv^(7 ) */,
            0X1.9BFAAD674C0BP+49 /* psi_inv^(8 ) */,     0X1.D24F7C783748P+49 /* psi_inv^(9 ) */,     0X1.992D52DFD9E7P+48 /* psi_inv^(10) */,     0X1.27AE7C0DF068P+48 /* psi_inv^(11) */,
            0X1.07DCA5721029P+50 /* psi_inv^(12) */,     0X1.09C16575C342P+48 /* psi_inv^(13) */,    0X1.5227FE46AC1A8P+49 /* psi_inv^(14) */,     0X1.1164E5C2C127P+49 /* psi_inv^(15) */,
                          0X1P+0 /* psi_inv^(0 ) */,    0X1.2E9D34A6839F8P+49 /* psi_inv^(2 ) */,     0X1.3FF8241FC4BDP+48 /* psi_inv^(4 ) */,      0X1.1BCCF21EEFAP+45 /* psi_inv^(6 ) */,
            0X1.9BFAAD674C0BP+49 /* psi_inv^(8 ) */,     0X1.992D52DFD9E7P+48 /* psi_inv^(10) */,     0X1.07DCA5721029P+50 /* psi_inv^(12) */,    0X1.5227FE46AC1A8P+49 /* psi_inv^(14) */,
             0X1.CEADACB4C9EP+43 /* psi_inv^(16) */,     0X1.7334B0A11F71P+49 /* psi_inv^(18) */,     0X1.785D0ABC4382P+48 /* psi_inv^(20) */,     0X1.DAF1A1CDF938P+47 /* psi_inv^(22) */,
            0X1.02CD5F4D182BP+48 /* psi_inv^(24) */,     0X1.CDE03C88501EP+48 /* psi_inv^(26) */,      0X1.48DD908874AP+49 /* psi_inv^(28) */,     0X1.D1DEC4A27B67P+48 /* psi_inv^(30) */,
                          0X1P+0 /* psi_inv^(0 ) */,     0X1.51C21D594D12P+48 /* psi_inv^(3 ) */,      0X1.1BCCF21EEFAP+45 /* psi_inv^(6 ) */,     0X1.D24F7C783748P+49 /* psi_inv^(9 ) */,
            0X1.07DCA5721029P+50 /* psi_inv^(12) */,     0X1.1164E5C2C127P+49 /* psi_inv^(15) */,     0X1.7334B0A11F71P+49 /* psi_inv^(18) */,     0X1.262B4709746DP+48 /* psi_inv^(21) */,
            0X1.02CD5F4D182BP+48 /* psi_inv^(24) */,     0X1.37D841BE7173P+49 /* psi_inv^(27) */,     0X1.D1DEC4A27B67P+48 /* psi_inv^(30) */,     0X1.EF8BC9D30DD8P+48 /* psi_inv^(33) */,
            0X1.7803EDF01DA2P+49 /* psi_inv^(36) */,     0X1.6BE2CD9A921CP+47 /* psi_inv^(39) */,     0X1.4B695690130DP+49 /* psi_inv^(42) */,    0X1.931F4D451E5F8P+49 /* psi_inv^(45) */,
                          0X1P+0 /* psi_inv^(0 ) */,     0X1.3FF8241FC4BDP+48 /* psi_inv^(4 ) */,     0X1.9BFAAD674C0BP+49 /* psi_inv^(8 ) */,     0X1.07DCA5721029P+50 /* psi_inv^(12) */,
                          0X1P+0 /* psi_inv^(0 ) */,     0X1.9BFAAD674C0BP+49 /* psi_inv^(8 ) */,      0X1.CEADACB4C9EP+43 /* psi_inv^(16) */,     0X1.02CD5F4D182BP+48 /* psi_inv^(24) */,
                          0X1P+0 /* psi_inv^(0 ) */,     0X1.07DCA5721029P+50 /* psi_inv^(12) */,     0X1.02CD5F4D182BP+48 /* psi_inv^(24) */,     0X1.7803EDF01DA2P+49 /* psi_inv^(36) */
        },

    },

    {
        .prime = (0X49ULL << 0X2CULL) + 1ULL,

        .coeff = 0X49ULL,

        .power_of_two = 0X2CULL,

        .transform = CYCLIC,

        .magic = 0XE070381C0E06D251ULL,

        .shift = 0X32ULL,

        .barrett = 0XE070381C0E06DULL,

        .prime_inv =    0X1.C0E070381C0DAP-51,

        .twiddle =
        {
            0X000029A37DA45AEEULL /* w^(2^0 ) */, 0X00040FE90BC6C116ULL /* w^(2^1 ) */, 0X00040F150036F17BULL /* w^(2^2 ) */, 0X000307940EC051BAULL /* w^(2^3 ) */,
            0X0003317AA5608C3BULL /* w^(2^4 ) */, 0X0002BB42F0A2EE9BULL /* w^(2^5 ) */, 0X00000E5EB6449356ULL /* w^(2^6 ) */, 0X0003FED7A01404C9ULL /* w^(2^7 ) */,
            0X00034D707F8D56EFULL /* w^(2^8 ) */, 0X00008D74900F3192ULL /* w^(2^9 ) */, 0X000357D96DCCF925ULL /* w^(2^10) */, 0X0002D3E26379382AULL /* w^(2^11) */,
            0X0002705B711AE609ULL /* w^(2^12) */, 0X00016E8D0F8FE6F2ULL /* w^(2^13) */, 0X0002197F7DFEC7C4ULL /* w^(2^14) */, 0X000211C4F984074CULL /* w^(2^15) */,
            0X00026E5FCD520770ULL /* w^(2^16) */, 0X0002F3AF94B72D20ULL /* w^(2^17) */, 0X0003281276BF6B3DULL /* w^(2^18) */, 0X00047B82672AC321ULL /* w^(2^19) */,
            0X0001AC4C54C28C8DULL /* w^(2^20) */, 0X00035EBFF9F43ACEULL /* w^(2^21) */, 0X0003BD827063290FULL /* w^(2^22) */, 0X0003A29E128BECE8ULL /* w^(2^23) */,
            0X000122B0D0A14620ULL /* w^(2^24) */, 0X00023B347EF47022ULL /* w^(2^25) */, 0X00046A3F093FAFD0ULL /* w^(2^26) */, 0X0000CCDE26FF658CULL /* w^(2^27) */,
            0X00016BD1F0612628ULL /* w^(2^28) */, 0X000488656BA52F57ULL /* w^(2^29) */, 0X0000F9F7457548DCULL /* w^(2^30) */, 0X0003BA0B61046E67ULL /* w^(2^31) */,
            0X0001E94F9F630830ULL /* w^(2^32) */, 0X0000AB1E2F525B7EULL /* w^(2^33) */, 0X0000748D2D854EE3ULL /* w^(2^34) */, 0X0000C5937FAFBF62ULL /* w^(2^35) */,
            0X000315AA89F6C030ULL /* w^(2^36) */, 0X0001C167D70BDE53ULL /* w^(2^37) */, 0X00017FDBE1B9EB64ULL /* w^(2^38) */, 0X00037DBC7608E713ULL /* w^(2^39) */,
            0X0003CB5D22355606ULL /* w^(2^40) */, 0X0003527D5D2B6128ULL /* w^(2^41) */, 0X000461B629D2CD9CULL /* w^(2^42) */, 0X0004900000000000ULL /* w^(2^43) */,
            0X0000000000000000ULL /* w^(2^44) */, 0X0000000000000000ULL /* w^(2^45) */
        },

        .twiddle_inv =
        {
            0X00010CE8C2FCC06CULL /* w_inv^(2^0 ) */, 0X0000F305D7095E54ULL /* w_inv^(2^1 ) */, 0X0002C1802F6EA419ULL /* w_inv^(2^2 ) */, 0X000350EED3974017ULL /* w_inv^(2^3 ) */,
            0X00037EBFC463D534ULL /* w_inv^(2^4 ) */, 0X00041B918514BE3FULL /* w_inv^(2^5 ) */, 0X000399FB74247588ULL /* w_inv^(2^6 ) */, 0X0001409BE9C2E6C0ULL /* w_inv^(2^7 ) */,
            0X000360EAB1B4C2C7ULL /* w_inv^(2^8 ) */, 0X0003084AB318CABDULL /* w_inv^(2^9 ) */, 0X00025F3BE32F654CULL /* w_inv^(2^10) */, 0X0000C3C0EDF6A485ULL /* w_inv^(2^11) */,
            0X00015EF010D5E5B1ULL /* w_inv^(2^12) */, 0X00029DDCC49E4A78ULL /* w_inv^(2^13) */, 0X0003C1FB21C8504DULL /* w_inv^(2^14) */, 0X00047A2265CDFBF5ULL /* w_inv^(2^15) */,
            0X00021AF33A6C41E7ULL /* w_inv^(2^16) */, 0X00027B7E4B7461FCULL /* w_inv^(2^17) */, 0X0000089F2442C016ULL /* w_inv^(2^18) */, 0X000422B180B0A091ULL /* w_inv^(2^19) */,
            0X0004375270BC559EULL /* w_inv^(2^20) */, 0X00027D4FA9A7C761ULL /* w_inv^(2^21) */, 0X00015F53381E08A1ULL /* w_inv^(2^22) */, 0X0003A40576421E96ULL /* w_inv^(2^23) */,
            0X00027F6A01D59414ULL /* w_inv^(2^24) */, 0X000087DCDC2EBFC9ULL /* w_inv^(2^25) */, 0X000441D536D003EEULL /* w_inv^(2^26) */, 0X000157D529F3D2BCULL /* w_inv^(2^27) */,
            0X0004615A87EE852DULL /* w_inv^(2^28) */, 0X000257C3632D0B2FULL /* w_inv^(2^29) */, 0X000106336FC11807ULL /* w_inv^(2^30) */, 0X00020068F6768E3EULL /* w_inv^(2^31) */,
            0X000301DA50FB7DE8ULL /* w_inv^(2^32) */, 0X0000697421D449A0ULL /* w_inv^(2^33) */, 0X0002FA7FE925E2DAULL /* w_inv^(2^34) */, 0X00012710CC1EF548ULL /* w_inv^(2^35) */,
            0X0003591EB2821FE3ULL /* w_inv^(2^36) */, 0X0000F0B3E60EF1A7ULL /* w_inv^(2^37) */, 0X00007906B40305BDULL /* w_inv^(2^38) */, 0X0000326C8A7BAFB2ULL /* w_inv^(2^39) */,
            0X00038930EE96E16FULL /* w_inv^(2^40) */, 0X0003AE332E74FE2CULL /* w_inv^(2^41) */, 0X00002E49D62D3265ULL /* w_inv^(2^42) */, 0X0004900000000000ULL /* w_inv^(2^43) */,
            0X0000000000000000ULL /* w_inv^(2^44) */, 0X0000000000000000ULL /* w_inv^(2^45) */
        },

        .size_inv =
        {
            0X0000000000000001ULL /* N^(2^0 ) */, 0X0002480000000001ULL /* N^(2^1 ) */, 0X00036C0000000001ULL /* N^(2^2 ) */, 0X0003FE0000000001ULL /* N^(2^3 ) */,
            0X0004470000000001ULL /* N^(2^4 ) */, 0X00046B8000000001ULL /* N^(2^5 ) */, 0X00047DC000000001ULL /* N^(2^6 ) */, 0X000486E000000001ULL /* N^(2^7 ) */,
            0X00048B7000000001ULL /* N^(2^8 ) */, 0X00048DB800000001ULL /* N^(2^9 ) */, 0X00048EDC00000001ULL /* N^(2^10) */, 0X00048F6E00000001ULL /* N^(2^11) */,
            0X00048FB700000001ULL /* N^(2^12) */, 0X00048FDB80000001ULL /* N^(2^13) */, 0X00048FEDC0000001ULL /* N^(2^14) */, 0X00048FF6E0000001ULL /* N^(2^15) */,
            0X00048FFB70000001ULL /* N^(2^16) */, 0X00048FFDB8000001ULL /* N^(2^17) */, 0X00048FFEDC000001ULL /* N^(2^18) */, 0X00048FFF6E000001ULL /* N^(2^19) */,
            0X00048FFFB7000001ULL /* N^(2^20) */, 0X00048FFFDB800001ULL /* N^(2^21) */, 0X00048FFFEDC00001ULL /* N^(2^22) */, 0X00048FFFF6E00001ULL /* N^(2^23) */,
            0X00048FFFFB700001ULL /* N^(2^24) */, 0X00048FFFFDB80001ULL /* N^(2^25) */, 0X00048FFFFEDC0001ULL /* N^(2^26) */, 0X00048FFFFF6E0001ULL /* N^(2^27) */,
            0X00048FFFFFB70001ULL /* N^(2^28) */, 0X00048FFFFFDB8001ULL /* N^(2^29) */, 0X00048FFFFFEDC001ULL /* N^(2^30) */, 0X00048FFFFFF6E001ULL /* N^(2^31) */,
            0X00048FFFFFFB7001ULL /* N^(2^32) */, 0X00048FFFFFFDB801ULL /* N^(2^33) */, 0X00048FFFFFFEDC01ULL /* N^(2^34) */, 0X00048FFFFFFF6E01ULL /* N^(2^35) */,
            0X00048FFFFFFFB701ULL /* N^(2^36) */, 0X00048FFFFFFFDB81ULL /* N^(2^37) */, 0X00048FFFFFFFEDC1ULL /* N^(2^38) */, 0X00048FFFFFFFF6E1ULL /* N^(2^39) */,
            0X00048FFFFFFFFB71ULL /* N^(2^40) */, 0X00048FFFFFFFFDB9ULL /* N^(2^41) */, 0X00048FFFFFFFFEDDULL /* N^(2^42) */, 0X00048FFFFFFFFF6FULL /* N^(2^43) */,
            0X0000000000000000ULL /* N^(2^44) */, 0X0000000000000000ULL /* N^(2^45) */
        },

        .codelet64_twd =
        {
                          0X1P+0 /* psi^(0 ) */,     0X1.7FDBE1B9EB64P+48 /* psi^(1 ) */,    0X1.BEDE3B0473898P+49 /* psi^(2 ) */,     0X1.1206D954946AP+48 /* psi^(3 ) */,
            0X1.E5AE911AAB03P+49 /* psi^(4 ) */,     0X1.69D4906E6D79P+48 /* psi^(5 ) */,     0X1.B0ECD5BBA084P+46 /* psi^(6 ) */,     0X1.28018FE8EBBEP+48 /* psi^(7 ) */,
            0X1.A93EAE95B094P+49 /* psi^(8 ) */,     0X1.EFF8F86066EAP+49 /* psi^(9 ) */,     0X1.FE8DAA671ED8P+49 /* psi^(10) */,    0X1.0A30DE23BE238P+50 /* psi^(11) */,
            0X1.FCAC424109ADP+49 /* psi^(12) */,     0X1.916F3751EB48P+49 /* psi^(13) */,    0X1.23E10C81A5C88P+49 /* psi^(14) */,    0X1.98E29FD0C38F8P+49 /* psi^(15) */,
                          0X1P+0 /* psi^(0 ) */,    0X1.BEDE3B0473898P+49 /* psi^(2 ) */,     0X1.E5AE911AAB03P+49 /* psi^(4 ) */,     0X1.B0ECD5BBA084P+46 /* psi^(6 ) */,
            0X1.A93EAE95B094P+49 /* psi^(8 ) */,     0X1.FE8DAA671ED8P+49 /* psi^(10) */,     0X1.FCAC424109ADP+49 /* psi^(12) */,    0X1.23E10C81A5C88P+49 /* psi^(14) */,
            0X1.186D8A74B367P+50 /* psi^(16) */,     0X1.34EEC13D4DDEP+49 /* psi^(18) */,    0X1.AA896A77DE838P+49 /* psi^(20) */,     0X1.268AEB0B5C35P+48 /* psi^(22) */,
            0X1.C399A31603AAP+47 /* psi^(24) */,     0X1.4F1CD61CD1CEP+48 /* psi^(26) */,     0X1.06CF11691E92P+48 /* psi^(28) */,    0X1.1764DD611413CP+50 /* psi^(30) */,
                          0X1P+0 /* psi^(0 ) */,     0X1.1206D954946AP+48 /* psi^(3 ) */,     0X1.B0ECD5BBA084P+46 /* psi^(6 ) */,     0X1.EFF8F86066EAP+49 /* psi^(9 ) */,
            0X1.FCAC424109ADP+49 /* psi^(12) */,    0X1.98E29FD0C38F8P+49 /* psi^(15) */,     0X1.34EEC13D4DDEP+49 /* psi^(18) */,    0X1.606B7CAEACB68P+49 /* psi^(21) */,
            0X1.C399A31603AAP+47 /* psi^(24) */,    0X1.078865034635CP+50 /* psi^(27) */,    0X1.1764DD611413CP+50 /* psi^(30) */,    0X1.88120F230A4E8P+49 /* psi^(33) */,
            0X1.8945BB9553F6P+47 /* psi^(36) */,    0X1.B3FF380B8A218P+49 /* psi^(39) */,     0X1.25C9566384A2P+47 /* psi^(42) */,     0X1.6D21915C2971P+48 /* psi^(45) */,
                          0X1P+0 /* psi^(0 ) */,     0X1.E5AE911AAB03P+49 /* psi^(4 ) */,     0X1.A93EAE95B094P+49 /* psi^(8 ) */,     0X1.FCAC424109ADP+49 /* psi^(12) */,
                          0X1P+0 /* psi^(0 ) */,     0X1.A93EAE95B094P+49 /* psi^(8 ) */,     0X1.186D8A74B367P+50 /* psi^(16) */,     0X1.C399A31603AAP+47 /* psi^(24) */,
                          0X1P+0 /* psi^(0 ) */,     0X1.FCAC424109ADP+49 /* psi^(12) */,     0X1.C399A31603AAP+47 /* psi^(24) */,     0X1.8945BB9553F6P+47 /* psi^(36) */
        },

        .codelet64_twd_inv =
        {
                          0X1P+0 /* psi_inv^(0 ) */,     0X1.E41AD00C16F4P+46 /* psi_inv^(1 ) */,      0X1.936453DD7D9P+45 /* psi_inv^(2 ) */,    0X1.BD62A795EC838P+49 /* psi_inv^(3 ) */,
           0X1.C498774B70B78P+49 /* psi_inv^(4 ) */,     0X1.C779AFCB9CA8P+46 /* psi_inv^(5 ) */,    0X1.A07194F197198P+49 /* psi_inv^(6 ) */,    0X1.55C8AD40658D8P+49 /* psi_inv^(7 ) */,
            0X1.D719973A7F16P+49 /* psi_inv^(8 ) */,      0X1.092E6F0BB74P+48 /* psi_inv^(9 ) */,     0X1.B4BA8A7A51E6P+49 /* psi_inv^(10) */,     0X1.CF2906A2A694P+48 /* psi_inv^(11) */,
            0X1.3AED2B1042FAP+48 /* psi_inv^(12) */,     0X1.79CB64002F14P+46 /* psi_inv^(13) */,    0X1.13113EC2B2228P+49 /* psi_inv^(14) */,    0X1.0925FBC92C578P+50 /* psi_inv^(15) */,
                          0X1P+0 /* psi_inv^(0 ) */,      0X1.936453DD7D9P+45 /* psi_inv^(2 ) */,    0X1.C498774B70B78P+49 /* psi_inv^(4 ) */,    0X1.A07194F197198P+49 /* psi_inv^(6 ) */,
            0X1.D719973A7F16P+49 /* psi_inv^(8 ) */,     0X1.B4BA8A7A51E6P+49 /* psi_inv^(10) */,     0X1.3AED2B1042FAP+48 /* psi_inv^(12) */,    0X1.13113EC2B2228P+49 /* psi_inv^(14) */,
            0X1.724EB1699328P+45 /* psi_inv^(16) */,     0X1.241EF37E5A38P+49 /* psi_inv^(18) */,     0X1.2D4EF6FBD94EP+47 /* psi_inv^(20) */,     0X1.25C9566384A2P+47 /* psi_inv^(22) */,
            0X1.3D82A2D49ED9P+48 /* psi_inv^(24) */,     0X1.08F132A445F8P+50 /* psi_inv^(26) */,     0X1.8945BB9553F6P+47 /* psi_inv^(28) */,     0X1.124389F718EEP+48 /* psi_inv^(30) */,
                          0X1P+0 /* psi_inv^(0 ) */,    0X1.BD62A795EC838P+49 /* psi_inv^(3 ) */,    0X1.A07194F197198P+49 /* psi_inv^(6 ) */,      0X1.092E6F0BB74P+48 /* psi_inv^(9 ) */,
            0X1.3AED2B1042FAP+48 /* psi_inv^(12) */,    0X1.0925FBC92C578P+50 /* psi_inv^(15) */,     0X1.241EF37E5A38P+49 /* psi_inv^(18) */,     0X1.9CF21DC41DCCP+46 /* psi_inv^(21) */,
            0X1.3D82A2D49ED9P+48 /* psi_inv^(24) */,     0X1.9315B7C8C944P+49 /* psi_inv^(27) */,     0X1.124389F718EEP+48 /* psi_inv^(30) */,     0X1.05BE52FF3E91P+50 /* psi_inv^(33) */,
            0X1.06CF11691E92P+48 /* psi_inv^(36) */,     0X1.E46EA57F34E6P+48 /* psi_inv^(39) */,     0X1.268AEB0B5C35P+48 /* psi_inv^(42) */,     0X1.0C6349BFFD0FP+50 /* psi_inv^(45) */,
                          0X1P+0 /* psi_inv^(0 ) */,    0X1.C498774B70B78P+49 /* psi_inv^(4 ) */,     0X1.D719973A7F16P+49 /* psi_inv^(8 ) */,     0X1.3AED2B1042FAP+48 /* psi_inv^(12) */,
                          0X1P+0 /* psi_inv^(0 ) */,     0X1.D719973A7F16P+49 /* psi_inv^(8 ) */,     0X1.724EB1699328P+45 /* psi_inv^(16) */,     0X1.3D82A2D49ED9P+48 /* psi_inv^(24) */,
                          0X1P+0 /* psi_inv^(0 ) */,     0X1.3AED2B1042FAP+48 /* psi_inv^(12) */,     0X1.3D82A2D49ED9P+48 /* psi_inv^(24) */,     0X1.06CF11691E92P+48 /* psi_inv^(36) */
        },

    },

    {
        .prime = (0X5DULL << 0X2CULL) + 1ULL,

        .coeff = 0X5DULL,

        .power_of_two = 0X2CULL,

        .transform = CYCLIC,

        .magic = 0X2C0B02C0B02C036FULL,

        .shift = 0X30ULL,

        .barrett = 0XB02C0B02C0B00ULL,

        .prime_inv =    0X1.6058160581602P-51,

        .twiddle =
        {
            0X000292CDFD24BFC1ULL /* w^(2^0 ) */, 0X000393B53FA4B062ULL /* w^(2^1 ) */, 0X00020BD0FF41D9E5ULL /* w^(2^2 ) */, 0X00010AC0CD37F765ULL /* w^(2^3 ) */,
            0X00025A21E3B1A99FULL /* w^(2^4 ) */, 0X00051171785D850CULL /* w^(2^5 ) */, 0X000370330AB716A0ULL /* w^(2^6 ) */, 0X00047BCB5E37A1C7ULL /* w^(2^7 ) */,
            0X0000FD2016075BDDULL /* w^(2^8 ) */, 0X00051230EC125B88ULL /* w^(2^9 ) */, 0X0000907BA3C35717ULL /* w^(2^10) */, 0X00009807A05FE1EDULL /* w^(2^11) */,
            0X0001D9BF44A6BBF3ULL /* w^(2^12) */, 0X00031436AC61A85DULL /* w^(2^13) */, 0X0004FD32FFCD3F40ULL /* w^(2^14) */, 0X0005854CCA8C2955ULL /* w^(2^15) */,
            0X00054C15C65B5DD8ULL /* w^(2^16) */, 0X000027589C82AEB6ULL /* w^(2^17) */, 0X000097B960462F1CULL /* w^(2^18) */, 0X0001F592549DDC0FULL /* w^(2^19) */,
            0X0004C7CBEFEF02CDULL /* w^(2^20) */, 0X0002CEFDF01FB96CULL /* w^(2^21) */, 0X000233A765B639DCULL /* w^(2^22) */, 0X0002FAE2DDB3FCDEULL /* w^(2^23) */,
            0X0004D9FAA8E48919ULL /* w^(2^24) */, 0X0004AA26DD481052ULL /* w^(2^25) */, 0X0001D279AD7B196DULL /* w^(2^26) */, 0X0005765C56FB3F36ULL /* w^(2^27) */,
            0X0001D9BE36F8F604ULL /* w^(2^28) */, 0X000379033B24865AULL /* w^(2^29) */, 0X0001721A7B2AC0A7ULL /* w^(2^30) */, 0X00034E2265C61828ULL /* w^(2^31) */,
            0X000031D3E2DBBE8EULL /* w^(2^32) */, 0X0001517B26E803CEULL /* w^(2^33) */, 0X00002AD17A37E4A0ULL /* w^(2^34) */, 0X00029E211C47B0F8ULL /* w^(2^35) */,
            0X000224EDF66CE541ULL /* w^(2^36) */, 0X00052EC487C4404DULL /* w^(2^37) */, 0X0004152A44C61FA9ULL /* w^(2^38) */, 0X0000597C20C94475ULL /* w^(2^39) */,
            0X0004E5502DBC06A2ULL /* w^(2^40) */, 0X00029E0A92506E3EULL /* w^(2^41) */, 0X0000AE242D7BF62AULL /* w^(2^42) */, 0X0005D00000000000ULL /* w^(2^43) */,
            0X0000000000000000ULL /* w^(2^44) */, 0X0000000000000000ULL /* w^(2^45) */
        },

        .twiddle_inv =
        {
            0X0002D64E431E4F4CULL /* w_inv^(2^0 ) */, 0X0000D0E455ECEAFBULL /* w_inv^(2^1 ) */, 0X0001911186A79F0EULL /* w_inv^(2^2 ) */, 0X0000E8056C7FA277ULL /* w_inv^(2^3 ) */,
            0X0003652F452C01DEULL /* w_inv^(2^4 ) */, 0X0005705EEC7B87F5ULL /* w_inv^(2^5 ) */, 0X00000F5079C5C4D4ULL /* w_inv^(2^6 ) */, 0X000019309439F447ULL /* w_inv^(2^7 ) */,
            0X000037C39E5B66A8ULL /* w_inv^(2^8 ) */, 0X0001853286BFD3E0ULL /* w_inv^(2^9 ) */, 0X00022F4032887437ULL /* w_inv^(2^10) */, 0X0002B170C0649A93ULL /* w_inv^(2^11) */,
            0X0004A694BEECC4F2ULL /* w_inv^(2^12) */, 0X00016686D97F6888ULL /* w_inv^(2^13) */, 0X0001F69CC9D19384ULL /* w_inv^(2^14) */, 0X0003B45218E4FC3AULL /* w_inv^(2^15) */,
            0X000324E4C1C3CED6ULL /* w_inv^(2^16) */, 0X00020D70BAA28B4DULL /* w_inv^(2^17) */, 0X000145758DEAF0C0ULL /* w_inv^(2^18) */, 0X0003CD21133F0116ULL /* w_inv^(2^19) */,
            0X000070ED6F248234ULL /* w_inv^(2^20) */, 0X000249D3F94885F5ULL /* w_inv^(2^21) */, 0X00023E5C0416E18CULL /* w_inv^(2^22) */, 0X00010804DC6DAA14ULL /* w_inv^(2^23) */,
            0X00040D1436C2D0E0ULL /* w_inv^(2^24) */, 0X00048491D88C9BAFULL /* w_inv^(2^25) */, 0X0004053A06CE589BULL /* w_inv^(2^26) */, 0X00028ECF87278BCAULL /* w_inv^(2^27) */,
            0X0003B40EDA10D38FULL /* w_inv^(2^28) */, 0X00056904020E4B87ULL /* w_inv^(2^29) */, 0X00047B09CAAC4090ULL /* w_inv^(2^30) */, 0X0001CFB5A3EE4415ULL /* w_inv^(2^31) */,
            0X0003C3A59759E74AULL /* w_inv^(2^32) */, 0X0002FFBE2BB316D2ULL /* w_inv^(2^33) */, 0X0005C68C4FF15207ULL /* w_inv^(2^34) */, 0X0001C29BDED72AFBULL /* w_inv^(2^35) */,
            0X000002E18007AD58ULL /* w_inv^(2^36) */, 0X0002324182A8C1D3ULL /* w_inv^(2^37) */, 0X000028CB03F48C72ULL /* w_inv^(2^38) */, 0X0003E09467A7F2FAULL /* w_inv^(2^39) */,
            0X00020EA8D11C9986ULL /* w_inv^(2^40) */, 0X000013572426D6BFULL /* w_inv^(2^41) */, 0X000521DBD28409D7ULL /* w_inv^(2^42) */, 0X0005D00000000000ULL /* w_inv^(2^43) */,
            0X0000000000000000ULL /* w_inv^(2^44) */, 0X0000000000000000ULL /* w_inv^(2^45) */
        },

        .size_inv =
        {
            0X0000000000000001ULL /* N^(2^0 ) */, 0X0002E80000000001ULL /* N^(2^1 ) */, 0X00045C0000000001ULL /* N^(2^2 ) */, 0X0005160000000001ULL /* N^(2^3 ) */,
            0X0005730000000001ULL /* N^(2^4 ) */, 0X0005A18000000001ULL /* N^(2^5 ) */, 0X0005B8C000000001ULL /* N^(2^6 ) */, 0X0005C46000000001ULL /* N^(2^7 ) */,
            0X0005CA3000000001ULL /* N^(2^8 ) */, 0X0005CD1800000001ULL /* N^(2^9 ) */, 0X0005CE8C00000001ULL /* N^(2^10) */, 0X0005CF4600000001ULL /* N^(2^11) */,
            0X0005CFA300000001ULL /* N^(2^12) */, 0X0005CFD180000001ULL /* N^(2^13) */, 0X0005CFE8C0000001ULL /* N^(2^14) */, 0X0005CFF460000001ULL /* N^(2^15) */,
            0X0005CFFA30000001ULL /* N^(2^16) */, 0X0005CFFD18000001ULL /* N^(2^17) */, 0X0005CFFE8C000001ULL /* N^(2^18) */, 0X0005CFFF46000001ULL /* N^(2^19) */,
            0X0005CFFFA3000001ULL /* N^(2^20) */, 0X0005CFFFD1800001ULL /* N^(2^21) */, 0X0005CFFFE8C00001ULL /* N^(2^22) */, 0X0005CFFFF4600001ULL /* N^(2^23) */,
            0X0005CFFFFA300001ULL /* N^(2^24) */, 0X0005CFFFFD180001ULL /* N^(2^25) */, 0X0005CFFFFE8C0001ULL /* N^(2^26) */, 0X0005CFFFFF460001ULL /* N^(2^27) */,
            0X0005CFFFFFA30001ULL /* N^(2^28) */, 0X0005CFFFFFD18001ULL /* N^(2^29) */, 0X0005CFFFFFE8C001ULL /* N^(2^30) */, 0X0005CFFFFFF46001ULL /* N^(2^31) */,
            0X0005CFFFFFFA3001ULL /* N^(2^32) */, 0X0005CFFFFFFD1801ULL /* N^(2^33) */, 0X0005CFFFFFFE8C01ULL /* N^(2^34) */, 0X0005CFFFFFFF4601ULL /* N^(2^35) */,
            0X0005CFFFFFFFA301ULL /* N^(2^36) */, 0X0005CFFFFFFFD181ULL /* N^(2^37) */, 0X0005CFFFFFFFE8C1ULL /* N^(2^38) */, 0X0005CFFFFFFFF461ULL /* N^(2^39) */,
            0X0005CFFFFFFFFA31ULL /* N^(2^40) */, 0X0005CFFFFFFFFD19ULL /* N^(2^41) */, 0X0005CFFFFFFFFE8DULL /* N^(2^42) */, 0X0005CFFFFFFFFF47ULL /* N^(2^43) */,
            0X0000000000000000ULL /* N^(2^44) */, 0X0000000000000000ULL /* N^(2^45) */
        },

        .codelet64_twd =
        {
                          0X1P+0 /* psi^(0 ) */,    0X1.054A913187EA4P+50 /* psi^(1 ) */,     0X1.65F0832511D4P+46 /* psi^(2 ) */,    0X1.1086DDD5C1124P+50 /* psi^(3 ) */,
           0X1.39540B6F01A88P+50 /* psi^(4 ) */,     0X1.F1D3591AC5C8P+47 /* psi^(5 ) */,    0X1.30164F3433EA8P+50 /* psi^(6 ) */,    0X1.43BC3E7BD72B8P+49 /* psi^(7 ) */,
            0X1.4F054928371FP+49 /* psi^(8 ) */,    0X1.A4DCD8A146C78P+49 /* psi^(9 ) */,    0X1.2D1745C92EF84P+50 /* psi^(10) */,     0X1.C4C6122B6F23P+49 /* psi^(11) */,
             0X1.B26FEF6D108P+43 /* psi^(12) */,     0X1.4423D3468B0EP+48 /* psi^(13) */,      0X1.2A5A98A7291P+48 /* psi^(14) */,       0X1.8D033B959EP+40 /* psi^(15) */,
                          0X1P+0 /* psi^(0 ) */,     0X1.65F0832511D4P+46 /* psi^(2 ) */,    0X1.39540B6F01A88P+50 /* psi^(4 ) */,    0X1.30164F3433EA8P+50 /* psi^(6 ) */,
            0X1.4F054928371FP+49 /* psi^(8 ) */,    0X1.2D1745C92EF84P+50 /* psi^(10) */,      0X1.B26FEF6D108P+43 /* psi^(12) */,      0X1.2A5A98A7291P+48 /* psi^(14) */,
            0X1.5C485AF7EC54P+47 /* psi^(16) */,    0X1.11CB2593EAAE8P+49 /* psi^(18) */,    0X1.CCDB6B6838E38P+49 /* psi^(20) */,      0X1.2D77BC09F66P+48 /* psi^(22) */,
           0X1.6F2A36F64A508P+50 /* psi^(24) */,     0X1.85F0B6A4A936P+49 /* psi^(26) */,    0X1.E0AB9771B33D8P+49 /* psi^(28) */,     0X1.EF6B98580D07P+48 /* psi^(30) */,
                          0X1P+0 /* psi^(0 ) */,    0X1.1086DDD5C1124P+50 /* psi^(3 ) */,    0X1.30164F3433EA8P+50 /* psi^(6 ) */,    0X1.A4DCD8A146C78P+49 /* psi^(9 ) */,
             0X1.B26FEF6D108P+43 /* psi^(12) */,       0X1.8D033B959EP+40 /* psi^(15) */,    0X1.11CB2593EAAE8P+49 /* psi^(18) */,     0X1.23B2B1AE885BP+49 /* psi^(21) */,
           0X1.6F2A36F64A508P+50 /* psi^(24) */,     0X1.414B428FC463P+49 /* psi^(27) */,     0X1.EF6B98580D07P+48 /* psi^(30) */,     0X1.BAD5BB39E058P+48 /* psi^(33) */,
            0X1.D55FA487F2BEP+47 /* psi^(36) */,     0X1.A443C18428D5P+49 /* psi^(39) */,      0X1.1BA2E8DB442P+48 /* psi^(42) */,    0X1.22F70B2E5D3CCP+50 /* psi^(45) */,
                          0X1P+0 /* psi^(0 ) */,    0X1.39540B6F01A88P+50 /* psi^(4 ) */,     0X1.4F054928371FP+49 /* psi^(8 ) */,      0X1.B26FEF6D108P+43 /* psi^(12) */,
                          0X1P+0 /* psi^(0 ) */,     0X1.4F054928371FP+49 /* psi^(8 ) */,     0X1.5C485AF7EC54P+47 /* psi^(16) */,    0X1.6F2A36F64A508P+50 /* psi^(24) */,
                          0X1P+0 /* psi^(0 ) */,      0X1.B26FEF6D108P+43 /* psi^(12) */,    0X1.6F2A36F64A508P+50 /* psi^(24) */,     0X1.D55FA487F2BEP+47 /* psi^(36) */
        },

        .codelet64_twd_inv =
        {
                          0X1P+0 /* psi_inv^(0 ) */,      0X1.46581FA4639P+45 /* psi_inv^(1 ) */,     0X1.F04A33D3F97DP+49 /* psi_inv^(2 ) */,    0X1.67EE9C7222424P+50 /* psi_inv^(3 ) */,
            0X1.0754688E4CC3P+49 /* psi_inv^(4 ) */,    0X1.A6B4BD703B9D8P+49 /* psi_inv^(5 ) */,    0X1.620F495B56CA8P+49 /* psi_inv^(6 ) */,     0X1.00DE5E08DBFAP+49 /* psi_inv^(7 ) */,
             0X1.3572426D6BFP+44 /* psi_inv^(8 ) */,     0X1.1E130BCE0524P+47 /* psi_inv^(9 ) */,    0X1.28A210FD82684P+50 /* psi_inv^(10) */,    0X1.C44D4E5177A58P+49 /* psi_inv^(11) */,
            0X1.1B249497C71DP+49 /* psi_inv^(12) */,    0X1.4DE41387919DCP+50 /* psi_inv^(13) */,     0X1.D634DA6C1552P+49 /* psi_inv^(14) */,      0X1.14B156450CDP+50 /* psi_inv^(15) */,
                          0X1P+0 /* psi_inv^(0 ) */,     0X1.F04A33D3F97DP+49 /* psi_inv^(2 ) */,     0X1.0754688E4CC3P+49 /* psi_inv^(4 ) */,    0X1.620F495B56CA8P+49 /* psi_inv^(6 ) */,
             0X1.3572426D6BFP+44 /* psi_inv^(8 ) */,    0X1.28A210FD82684P+50 /* psi_inv^(10) */,     0X1.1B249497C71DP+49 /* psi_inv^(12) */,     0X1.D634DA6C1552P+49 /* psi_inv^(14) */,
           0X1.4876F4A10275CP+50 /* psi_inv^(16) */,    0X1.296959D635BC4P+50 /* psi_inv^(18) */,    0X1.709B202125DF4P+50 /* psi_inv^(20) */,      0X1.1BA2E8DB442P+48 /* psi_inv^(22) */,
           0X1.98FAB6D7C8E18P+49 /* psi_inv^(24) */,     0X1.0FA6C32F3057P+48 /* psi_inv^(26) */,     0X1.D55FA487F2BEP+47 /* psi_inv^(28) */,     0X1.5DA0F7CDAEE3P+50 /* psi_inv^(30) */,
                          0X1P+0 /* psi_inv^(0 ) */,    0X1.67EE9C7222424P+50 /* psi_inv^(3 ) */,    0X1.620F495B56CA8P+49 /* psi_inv^(6 ) */,     0X1.1E130BCE0524P+47 /* psi_inv^(9 ) */,
            0X1.1B249497C71DP+49 /* psi_inv^(12) */,      0X1.14B156450CDP+50 /* psi_inv^(15) */,    0X1.296959D635BC4P+50 /* psi_inv^(18) */,    0X1.2339EDD490DD8P+49 /* psi_inv^(21) */,
           0X1.98FAB6D7C8E18P+49 /* psi_inv^(24) */,    0X1.35C594DCA7474P+50 /* psi_inv^(27) */,     0X1.5DA0F7CDAEE3P+50 /* psi_inv^(30) */,    0X1.69CD3F02DCE3CP+50 /* psi_inv^(33) */,
           0X1.E0AB9771B33D8P+49 /* psi_inv^(36) */,    0X1.E721A1F724068P+49 /* psi_inv^(39) */,      0X1.2D77BC09F66P+48 /* psi_inv^(42) */,     0X1.30DF63C37314P+47 /* psi_inv^(45) */,
                          0X1P+0 /* psi_inv^(0 ) */,     0X1.0754688E4CC3P+49 /* psi_inv^(4 ) */,      0X1.3572426D6BFP+44 /* psi_inv^(8 ) */,     0X1.1B249497C71DP+49 /* psi_inv^(12) */,
                          0X1P+0 /* psi_inv^(0 ) */,      0X1.3572426D6BFP+44 /* psi_inv^(8 ) */,    0X1.4876F4A10275CP+50 /* psi_inv^(16) */,    0X1.98FAB6D7C8E18P+49 /* psi_inv^(24) */,
                          0X1P+0 /* psi_inv^(0 ) */,     0X1.1B249497C71DP+49 /* psi_inv^(12) */,    0X1.98FAB6D7C8E18P+49 /* psi_inv^(24) */,    0X1.E0AB9771B33D8P+49 /* psi_inv^(36) */
        },

    },

    {
        .prime = (0X6FULL << 0X2CULL) + 1ULL,

        .coeff = 0X6FULL,

        .power_of_two = 0X2CULL,

        .transform = NEGACYCLIC,

        .magic = 0X49CD42E2049CC98BULL,

        .shift = 0X31ULL,

        .barrett = 0X939A85C409399ULL,

        .prime_inv =    0X1.27350B8812732P-51,

        .twiddle =
        {
            0X00012B5C3AC229F8ULL /* w^(2^0 ) */, 0X000666124E816627ULL /* w^(2^1 ) */, 0X00047649066B2FA4ULL /* w^(2^2 ) */, 0X00052FF304BCFC68ULL /* w^(2^3 ) */,
            0X0000FFE80D06D33FULL /* w^(2^4 ) */, 0X0004A0F340FE1B66ULL /* w^(2^5 ) */, 0X0004F6F79407D61CULL /* w^(2^6 ) */, 0X0005BC497A2A491CULL /* w^(2^7 ) */,
            0X0006E8BF2FE12508ULL /* w^(2^8 ) */, 0X00007205185B6A6EULL /* w^(2^9 ) */, 0X00009D9382E05750ULL /* w^(2^10) */, 0X0000252B06AD21FDULL /* w^(2^11) */,
            0X0002EEE5AF2324BCULL /* w^(2^12) */, 0X00035BE8E8C868ECULL /* w^(2^13) */, 0X0006EE7535F92664ULL /* w^(2^14) */, 0X000496DE589573BAULL /* w^(2^15) */,
            0X00044C334FCA2F5BULL /* w^(2^16) */, 0X0003959CF2FC2C9AULL /* w^(2^17) */, 0X0004824FA44B4334ULL /* w^(2^18) */, 0X0002B0147ECBEBD3ULL /* w^(2^19) */,
            0X0004BCF5E8835E02ULL /* w^(2^20) */, 0X0005E13587DF7C86ULL /* w^(2^21) */, 0X00011836AFC06DACULL /* w^(2^22) */, 0X00067EEFB2555A98ULL /* w^(2^23) */,
            0X0003812E9D77EA20ULL /* w^(2^24) */, 0X0005BB9BFB41C963ULL /* w^(2^25) */, 0X000184ED4090469EULL /* w^(2^26) */, 0X00033FFEB24327DDULL /* w^(2^27) */,
            0X00046FD6675A1D1FULL /* w^(2^28) */, 0X00063E8F33B893C3ULL /* w^(2^29) */, 0X000098F5A0ECACAFULL /* w^(2^30) */, 0X00024C731B5A0AFCULL /* w^(2^31) */,
            0X0000B96BA8F3B952ULL /* w^(2^32) */, 0X0004D6752E1A1D89ULL /* w^(2^33) */, 0X000190E431772BD9ULL /* w^(2^34) */, 0X0002464CE59D5D88ULL /* w^(2^35) */,
            0X000023E5ACD7194FULL /* w^(2^36) */, 0X0005EF59EE72E2DCULL /* w^(2^37) */, 0X0000228D76F26542ULL /* w^(2^38) */, 0X0001DCE45983C1ADULL /* w^(2^39) */,
            0X00020493F45CBC24ULL /* w^(2^40) */, 0X00054353204F8194ULL /* w^(2^41) */, 0X0002E8EFB06857E9ULL /* w^(2^42) */, 0X0006F00000000000ULL /* w^(2^43) */,
            0X0000000000000000ULL /* w^(2^44) */, 0X0000000000000000ULL /* w^(2^45) */
        },

        .twiddle_inv =
        {
            0X000477D6975132F1ULL /* w_inv^(2^0 ) */, 0X0006CC144AF925B8ULL /* w_inv^(2^1 ) */, 0X00064826325B26C3ULL /* w_inv^(2^2 ) */, 0X0002AD5341E68382ULL /* w_inv^(2^3 ) */,
            0X000450304E8E2C8CULL /* w_inv^(2^4 ) */, 0X0000181982AE6F36ULL /* w_inv^(2^5 ) */, 0X000498067DED9152ULL /* w_inv^(2^6 ) */, 0X0006717BFB0A61FDULL /* w_inv^(2^7 ) */,
            0X0006C3B4C129066BULL /* w_inv^(2^8 ) */, 0X0002623414511068ULL /* w_inv^(2^9 ) */, 0X0003DCD3D1A69846ULL /* w_inv^(2^10) */, 0X0000BDE2EAF0F0E2ULL /* w_inv^(2^11) */,
            0X0002C9DC5062451CULL /* w_inv^(2^12) */, 0X0005D21B113B09E6ULL /* w_inv^(2^13) */, 0X0004C13647256B3FULL /* w_inv^(2^14) */, 0X0001A14C6BD4C0D3ULL /* w_inv^(2^15) */,
            0X00027B3B5597DF82ULL /* w_inv^(2^16) */, 0X0004333CCCD2C505ULL /* w_inv^(2^17) */, 0X0003E4E59B985481ULL /* w_inv^(2^18) */, 0X0001B82FB8D3072FULL /* w_inv^(2^19) */,
            0X00048E70729998F5ULL /* w_inv^(2^20) */, 0X0000BCAAAD468D5AULL /* w_inv^(2^21) */, 0X000075FA6B0B70D1ULL /* w_inv^(2^22) */, 0X0002DB2F47666082ULL /* w_inv^(2^23) */,
            0X0002A12D38E9E844ULL /* w_inv^(2^24) */, 0X00018C150745C91DULL /* w_inv^(2^25) */, 0X00002272210DCCBBULL /* w_inv^(2^26) */, 0X000581679D463DBAULL /* w_inv^(2^27) */,
            0X0004DC2CE526FA56ULL /* w_inv^(2^28) */, 0X0000D7C1FBD980CAULL /* w_inv^(2^29) */, 0X00066DB59669B707ULL /* w_inv^(2^30) */, 0X00024F0FBCA34B6AULL /* w_inv^(2^31) */,
            0X00059CA386457FC6ULL /* w_inv^(2^32) */, 0X000530A5D91C9496ULL /* w_inv^(2^33) */, 0X00035E78BEBB9A44ULL /* w_inv^(2^34) */, 0X0006DF60FD65B3D1ULL /* w_inv^(2^35) */,
            0X000299A8AE1ABAFDULL /* w_inv^(2^36) */, 0X0001A58BE9198112ULL /* w_inv^(2^37) */, 0X0004C8FA9A2321BEULL /* w_inv^(2^38) */, 0X00039397FA417037ULL /* w_inv^(2^39) */,
            0X000052C1DE23CC65ULL /* w_inv^(2^40) */, 0X00014BEF7D068764ULL /* w_inv^(2^41) */, 0X000407104F97A818ULL /* w_inv^(2^42) */, 0X0006F00000000000ULL /* w_inv^(2^43) */,
            0X0000000000000000ULL /* w_inv^(2^44) */, 0X0000000000000000ULL /* w_inv^(2^45) */
        },

        .size_inv =
        {
            0X0000000000000001ULL /* N^(2^0 ) */, 0X0003780000000001ULL /* N^(2^1 ) */, 0X0005340000000001ULL /* N^(2^2 ) */, 0X0006120000000001ULL /* N^(2^3 ) */,
            0X0006810000000001ULL /* N^(2^4 ) */, 0X0006B88000000001ULL /* N^(2^5 ) */, 0X0006D44000000001ULL /* N^(2^6 ) */, 0X0006E22000000001ULL /* N^(2^7 ) */,
            0X0006E91000000001ULL /* N^(2^8 ) */, 0X0006EC8800000001ULL /* N^(2^9 ) */, 0X0006EE4400000001ULL /* N^(2^10) */, 0X0006EF2200000001ULL /* N^(2^11) */,
            0X0006EF9100000001ULL /* N^(2^12) */, 0X0006EFC880000001ULL /* N^(2^13) */, 0X0006EFE440000001ULL /* N^(2^14) */, 0X0006EFF220000001ULL /* N^(2^15) */,
            0X0006EFF910000001ULL /* N^(2^16) */, 0X0006EFFC88000001ULL /* N^(2^17) */, 0X0006EFFE44000001ULL /* N^(2^18) */, 0X0006EFFF22000001ULL /* N^(2^19) */,
            0X0006EFFF91000001ULL /* N^(2^20) */, 0X0006EFFFC8800001ULL /* N^(2^21) */, 0X0006EFFFE4400001ULL /* N^(2^22) */, 0X0006EFFFF2200001ULL /* N^(2^23) */,
            0X0006EFFFF9100001ULL /* N^(2^24) */, 0X0006EFFFFC880001ULL /* N^(2^25) */, 0X0006EFFFFE440001ULL /* N^(2^26) */, 0X0006EFFFFF220001ULL /* N^(2^27) */,
            0X0006EFFFFF910001ULL /* N^(2^28) */, 0X0006EFFFFFC88001ULL /* N^(2^29) */, 0X0006EFFFFFE44001ULL /* N^(2^30) */, 0X0006EFFFFFF22001ULL /* N^(2^31) */,
            0X0006EFFFFFF91001ULL /* N^(2^32) */, 0X0006EFFFFFFC8801ULL /* N^(2^33) */, 0X0006EFFFFFFE4401ULL /* N^(2^34) */, 0X0006EFFFFFFF2201ULL /* N^(2^35) */,
            0X0006EFFFFFFF9101ULL /* N^(2^36) */, 0X0006EFFFFFFFC881ULL /* N^(2^37) */, 0X0006EFFFFFFFE441ULL /* N^(2^38) */, 0X0006EFFFFFFFF221ULL /* N^(2^39) */,
            0X0006EFFFFFFFF911ULL /* N^(2^40) */, 0X0006EFFFFFFFFC89ULL /* N^(2^41) */, 0X0006EFFFFFFFFE45ULL /* N^(2^42) */, 0X0006EFFFFFFFFF23ULL /* N^(2^43) */,
            0X0000000000000000ULL /* N^(2^44) */, 0X0000000000000000ULL /* N^(2^45) */
        },

        .codelet64_twd =
        {
                          0X1P+0 /* psi^(0 ) */,     0X1.7BD67B9CB8B7P+50 /* psi^(1 ) */,      0X1.146BB7932A1P+45 /* psi^(2 ) */,     0X1.CD85B96F8F14P+49 /* psi^(3 ) */,
            0X1.DCE45983C1ADP+48 /* psi^(4 ) */,     0X1.66294FF88116P+48 /* psi^(5 ) */,     0X1.68412CCB06E4P+50 /* psi^(6 ) */,    0X1.6970FF0BBDAF8P+50 /* psi^(7 ) */,
            0X1.0249FA2E5E12P+49 /* psi^(8 ) */,     0X1.3FD5D4218E1BP+49 /* psi^(9 ) */,    0X1.ECDB97FA4F7A8P+49 /* psi^(10) */,      0X1.2D860211DEFP+46 /* psi^(11) */,
           0X1.605B2A1A01FA8P+50 /* psi^(12) */,     0X1.315733671D24P+46 /* psi^(13) */,    0X1.11BA05848A76CP+50 /* psi^(14) */,     0X1.D36393C76245P+48 /* psi^(15) */,
                          0X1P+0 /* psi^(0 ) */,      0X1.146BB7932A1P+45 /* psi^(2 ) */,     0X1.DCE45983C1ADP+48 /* psi^(4 ) */,     0X1.68412CCB06E4P+50 /* psi^(6 ) */,
            0X1.0249FA2E5E12P+49 /* psi^(8 ) */,    0X1.ECDB97FA4F7A8P+49 /* psi^(10) */,    0X1.605B2A1A01FA8P+50 /* psi^(12) */,    0X1.11BA05848A76CP+50 /* psi^(14) */,
            0X1.50D4C813E065P+50 /* psi^(16) */,    0X1.3A2EFDFC85FA8P+49 /* psi^(18) */,     0X1.6A97AB5257A2P+47 /* psi^(20) */,    0X1.26C65066EA8A8P+49 /* psi^(22) */,
           0X1.19E59F6A688ECP+50 /* psi^(24) */,    0X1.168477514090CP+50 /* psi^(26) */,    0X1.AF9A26E76953CP+50 /* psi^(28) */,     0X1.F5050D5F82CEP+47 /* psi^(30) */,
                          0X1P+0 /* psi^(0 ) */,     0X1.CD85B96F8F14P+49 /* psi^(3 ) */,     0X1.68412CCB06E4P+50 /* psi^(6 ) */,     0X1.3FD5D4218E1BP+49 /* psi^(9 ) */,
           0X1.605B2A1A01FA8P+50 /* psi^(12) */,     0X1.D36393C76245P+48 /* psi^(15) */,    0X1.3A2EFDFC85FA8P+49 /* psi^(18) */,     0X1.36D9C7A25342P+47 /* psi^(21) */,
           0X1.19E59F6A688ECP+50 /* psi^(24) */,    0X1.82AF76FF70D6CP+50 /* psi^(27) */,     0X1.F5050D5F82CEP+47 /* psi^(30) */,    0X1.A068512D198CCP+50 /* psi^(33) */,
           0X1.265A83F98F808P+50 /* psi^(36) */,     0X1.C3C1A4499598P+48 /* psi^(39) */,     0X1.0BD9B47B679CP+46 /* psi^(42) */,    0X1.7654B7CCC6C38P+50 /* psi^(45) */,
                          0X1P+0 /* psi^(0 ) */,     0X1.DCE45983C1ADP+48 /* psi^(4 ) */,     0X1.0249FA2E5E12P+49 /* psi^(8 ) */,    0X1.605B2A1A01FA8P+50 /* psi^(12) */,
                          0X1P+0 /* psi^(0 ) */,     0X1.0249FA2E5E12P+49 /* psi^(8 ) */,     0X1.50D4C813E065P+50 /* psi^(16) */,    0X1.19E59F6A688ECP+50 /* psi^(24) */,
                          0X1P+0 /* psi^(0 ) */,    0X1.605B2A1A01FA8P+50 /* psi^(12) */,    0X1.19E59F6A688ECP+50 /* psi^(24) */,    0X1.265A83F98F808P+50 /* psi^(36) */
        },

        .codelet64_twd_inv =
        {
                          0X1P+0 /* psi_inv^(0 ) */,     0X1.A58BE9198112P+48 /* psi_inv^(1 ) */,    0X1.323EA688C86F8P+50 /* psi_inv^(2 ) */,     0X1.7575B37EC42CP+50 /* psi_inv^(3 ) */,
           0X1.C9CBFD20B81B8P+49 /* psi_inv^(4 ) */,      0X1.5CDAF23A5D9P+48 /* psi_inv^(5 ) */,     0X1.32C7F3EA5868P+46 /* psi_inv^(6 ) */,    0X1.36FF872B57C9CP+50 /* psi_inv^(7 ) */,
            0X1.4B07788F3194P+46 /* psi_inv^(8 ) */,     0X1.7A52BB2BB31AP+50 /* psi_inv^(9 ) */,     0X1.1AEC701EE59EP+48 /* psi_inv^(10) */,     0X1.0D61A1CB0D38P+49 /* psi_inv^(11) */,
           0X1.8CB868D2756B8P+49 /* psi_inv^(12) */,    0X1.2FEA20C46B808P+49 /* psi_inv^(13) */,      0X1.B22554B1993P+45 /* psi_inv^(14) */,    0X1.37615E434FB28P+49 /* psi_inv^(15) */,
                          0X1P+0 /* psi_inv^(0 ) */,    0X1.323EA688C86F8P+50 /* psi_inv^(2 ) */,    0X1.C9CBFD20B81B8P+49 /* psi_inv^(4 ) */,     0X1.32C7F3EA5868P+46 /* psi_inv^(6 ) */,
            0X1.4B07788F3194P+46 /* psi_inv^(8 ) */,     0X1.1AEC701EE59EP+48 /* psi_inv^(10) */,    0X1.8CB868D2756B8P+49 /* psi_inv^(12) */,      0X1.B22554B1993P+45 /* psi_inv^(14) */,
            0X1.4BEF7D068764P+48 /* psi_inv^(16) */,     0X1.B97423347B2EP+49 /* psi_inv^(18) */,     0X1.42E2BE7F1505P+48 /* psi_inv^(20) */,    0X1.AB4264B849868P+50 /* psi_inv^(22) */,
           0X1.C4AB8DA2BEA28P+49 /* psi_inv^(24) */,    0X1.C6FA59AC2F4A8P+49 /* psi_inv^(26) */,    0X1.2B4AF80CE0FF8P+49 /* psi_inv^(28) */,    0X1.63CE062105E28P+49 /* psi_inv^(30) */,
                          0X1P+0 /* psi_inv^(0 ) */,     0X1.7575B37EC42CP+50 /* psi_inv^(3 ) */,     0X1.32C7F3EA5868P+46 /* psi_inv^(6 ) */,     0X1.7A52BB2BB31AP+50 /* psi_inv^(9 ) */,
           0X1.8CB868D2756B8P+49 /* psi_inv^(12) */,    0X1.37615E434FB28P+49 /* psi_inv^(15) */,     0X1.B97423347B2EP+49 /* psi_inv^(18) */,     0X1.0846B0BB4C71P+49 /* psi_inv^(21) */,
           0X1.C4AB8DA2BEA28P+49 /* psi_inv^(24) */,     0X1.639AF5D2531DP+49 /* psi_inv^(27) */,    0X1.63CE062105E28P+49 /* psi_inv^(30) */,      0X1.9AB68C63B33P+47 /* psi_inv^(33) */,
             0X1.8CBB2312D59P+45 /* psi_inv^(36) */,     0X1.4F401A37FFD7P+49 /* psi_inv^(39) */,     0X1.289CD7CC8ABBP+50 /* psi_inv^(42) */,    0X1.2BC9DA9ABA144P+50 /* psi_inv^(45) */,
                          0X1P+0 /* psi_inv^(0 ) */,    0X1.C9CBFD20B81B8P+49 /* psi_inv^(4 ) */,     0X1.4B07788F3194P+46 /* psi_inv^(8 ) */,    0X1.8CB868D2756B8P+49 /* psi_inv^(12) */,
                          0X1P+0 /* psi_inv^(0 ) */,     0X1.4B07788F3194P+46 /* psi_inv^(8 ) */,     0X1.4BEF7D068764P+48 /* psi_inv^(16) */,    0X1.C4AB8DA2BEA28P+49 /* psi_inv^(24) */,
                          0X1P+0 /* psi_inv^(0 ) */,    0X1.8CB868D2756B8P+49 /* psi_inv^(12) */,    0X1.C4AB8DA2BEA28P+49 /* psi_inv^(24) */,      0X1.8CBB2312D59P+45 /* psi_inv^(36) */
        },

    },

    {
        .prime = (0X79ULL << 0X2CULL) + 1ULL,

        .coeff = 0X79ULL,

        .power_of_two = 0X2CULL,

        .transform = NEGACYCLIC,

        .magic = 0X43B3D5AF9A723685ULL,

        .shift = 0X31ULL,

        .barrett = 0X8767AB5F34E46ULL,

        .prime_inv =    0X1.0ECF56BE69C8EP-51,

        .twiddle =
        {
            0X0007071D8B9E602EULL /* w^(2^0 ) */, 0X000444F00DC80A26ULL /* w^(2^1 ) */, 0X00069B979B640652ULL /* w^(2^2 ) */, 0X0005916E2808DBD1ULL /* w^(2^3 ) */,
            0X000635193481E3EFULL /* w^(2^4 ) */, 0X0000B8D35F96A15EULL /* w^(2^5 ) */, 0X000783B42B8CF501ULL /* w^(2^6 ) */, 0X0001DFD6E44618C3ULL /* w^(2^7 ) */,
            0X0003986328EB6C49ULL /* w^(2^8 ) */, 0X0007553DDF5419ECULL /* w^(2^9 ) */, 0X000616FDE61D1B38ULL /* w^(2^10) */, 0X000725814505AA0DULL /* w^(2^11) */,
            0X0001B006E3F4F1DDULL /* w^(2^12) */, 0X0005F53F3ECA9E3EULL /* w^(2^13) */, 0X000611E3D52636B3ULL /* w^(2^14) */, 0X000234F9A237CB5CULL /* w^(2^15) */,
            0X00015DA198565DC6ULL /* w^(2^16) */, 0X00061C660839BA92ULL /* w^(2^17) */, 0X000694E6272537D8ULL /* w^(2^18) */, 0X0002FA2F484C8A04ULL /* w^(2^19) */,
            0X000125EBE4097773ULL /* w^(2^20) */, 0X0005BBA10364E051ULL /* w^(2^21) */, 0X0005A85218180B61ULL /* w^(2^22) */, 0X0001030B644B7853ULL /* w^(2^23) */,
            0X000562B94DD27FA4ULL /* w^(2^24) */, 0X0007030A5B4A6198ULL /* w^(2^25) */, 0X0002268688234E18ULL /* w^(2^26) */, 0X0003EC6B78C6CC72ULL /* w^(2^27) */,
            0X0004C83CE6771310ULL /* w^(2^28) */, 0X0000C6FC8BF8EF13ULL /* w^(2^29) */, 0X000332A2ECDE6850ULL /* w^(2^30) */, 0X00002CA3A4EEDAAFULL /* w^(2^31) */,
            0X000268DE1224071FULL /* w^(2^32) */, 0X00026B48F214E952ULL /* w^(2^33) */, 0X0001DEDBBE7D7BC1ULL /* w^(2^34) */, 0X0003B2F93427774CULL /* w^(2^35) */,
            0X0004A130C66E008AULL /* w^(2^36) */, 0X00052A4B92C8C492ULL /* w^(2^37) */, 0X0003DC145435DA47ULL /* w^(2^38) */, 0X0006CD610F1127C5ULL /* w^(2^39) */,
            0X00057AC6B54D9900ULL /* w^(2^40) */, 0X00016574F08A018EULL /* w^(2^41) */, 0X0000000002C00000ULL /* w^(2^42) */, 0X0007900000000000ULL /* w^(2^43) */,
            0X0000000000000000ULL /* w^(2^44) */, 0X0000000000000000ULL /* w^(2^45) */
        },

        .twiddle_inv =
        {
            0X0004C1A7819346E1ULL /* w_inv^(2^0 ) */, 0X0000DFDE1AC18277ULL /* w_inv^(2^1 ) */, 0X00024F47257B00D8ULL /* w_inv^(2^2 ) */, 0X00017C92E219BCEAULL /* w_inv^(2^3 ) */,
            0X0005C43BADCDD583ULL /* w_inv^(2^4 ) */, 0X000564CD883B4A70ULL /* w_inv^(2^5 ) */, 0X00010B5FCFE125A2ULL /* w_inv^(2^6 ) */, 0X0005962E1117EB64ULL /* w_inv^(2^7 ) */,
            0X000156EB5DB29D48ULL /* w_inv^(2^8 ) */, 0X00075857D3C3B84BULL /* w_inv^(2^9 ) */, 0X0004581EA4A45818ULL /* w_inv^(2^10) */, 0X0002112695A9017FULL /* w_inv^(2^11) */,
            0X0006FA63F423E8AAULL /* w_inv^(2^12) */, 0X000583B1DCB5E27CULL /* w_inv^(2^13) */, 0X0007109590EE303EULL /* w_inv^(2^14) */, 0X0000B2819B3EB8B5ULL /* w_inv^(2^15) */,
            0X00004CFDAE719B51ULL /* w_inv^(2^16) */, 0X0002F496C6F37035ULL /* w_inv^(2^17) */, 0X0003777F627A7E33ULL /* w_inv^(2^18) */, 0X00037CFD3EBF3DE8ULL /* w_inv^(2^19) */,
            0X000481848C8A86A5ULL /* w_inv^(2^20) */, 0X000353165F5F5332ULL /* w_inv^(2^21) */, 0X000365B02D267AA8ULL /* w_inv^(2^22) */, 0X00023D397C13FB00ULL /* w_inv^(2^23) */,
            0X0005E8FDC7A78DEFULL /* w_inv^(2^24) */, 0X000508517E615D01ULL /* w_inv^(2^25) */, 0X00063DE5A7236784ULL /* w_inv^(2^26) */, 0X000189448929F0F6ULL /* w_inv^(2^27) */,
            0X0005399385DE6A11ULL /* w_inv^(2^28) */, 0X000380D73B637A3FULL /* w_inv^(2^29) */, 0X0003E75E0342AEBCULL /* w_inv^(2^30) */, 0X0002C7A6DA965F41ULL /* w_inv^(2^31) */,
            0X0003F3421AAA11D8ULL /* w_inv^(2^32) */, 0X0006EFC2088675ACULL /* w_inv^(2^33) */, 0X000111E2B551D804ULL /* w_inv^(2^34) */, 0X0000BDF0F3415E42ULL /* w_inv^(2^35) */,
            0X000306BFB29283E0ULL /* w_inv^(2^36) */, 0X000615742BD124B7ULL /* w_inv^(2^37) */, 0X0006E41DB58BED62ULL /* w_inv^(2^38) */, 0X0006BADE255113FCULL /* w_inv^(2^39) */,
            0X000715518BE1E363ULL /* w_inv^(2^40) */, 0X0004B47BBA01FBFBULL /* w_inv^(2^41) */, 0X00078FFFFD400001ULL /* w_inv^(2^42) */, 0X0007900000000000ULL /* w_inv^(2^43) */,
            0X0000000000000000ULL /* w_inv^(2^44) */, 0X0000000000000000ULL /* w_inv^(2^45) */
        },

        .size_inv =
        {
            0X0000000000000001ULL /* N^(2^0 ) */, 0X0003C80000000001ULL /* N^(2^1 ) */, 0X0005AC0000000001ULL /* N^(2^2 ) */, 0X00069E0000000001ULL /* N^(2^3 ) */,
            0X0007170000000001ULL /* N^(2^4 ) */, 0X0007538000000001ULL /* N^(2^5 ) */, 0X000771C000000001ULL /* N^(2^6 ) */, 0X000780E000000001ULL /* N^(2^7 ) */,
            0X0007887000000001ULL /* N^(2^8 ) */, 0X00078C3800000001ULL /* N^(2^9 ) */, 0X00078E1C00000001ULL /* N^(2^10) */, 0X00078F0E00000001ULL /* N^(2^11) */,
            0X00078F8700000001ULL /* N^(2^12) */, 0X00078FC380000001ULL /* N^(2^13) */, 0X00078FE1C0000001ULL /* N^(2^14) */, 0X00078FF0E0000001ULL /* N^(2^15) */,
            0X00078FF870000001ULL /* N^(2^16) */, 0X00078FFC38000001ULL /* N^(2^17) */, 0X00078FFE1C000001ULL /* N^(2^18) */, 0X00078FFF0E000001ULL /* N^(2^19) */,
            0X00078FFF87000001ULL /* N^(2^20) */, 0X00078FFFC3800001ULL /* N^(2^21) */, 0X00078FFFE1C00001ULL /* N^(2^22) */, 0X00078FFFF0E00001ULL /* N^(2^23) */,
            0X00078FFFF8700001ULL /* N^(2^24) */, 0X00078FFFFC380001ULL /* N^(2^25) */, 0X00078FFFFE1C0001ULL /* N^(2^26) */, 0X00078FFFFF0E0001ULL /* N^(2^27) */,
            0X00078FFFFF870001ULL /* N^(2^28) */, 0X00078FFFFFC38001ULL /* N^(2^29) */, 0X00078FFFFFE1C001ULL /* N^(2^30) */, 0X00078FFFFFF0E001ULL /* N^(2^31) */,
            0X00078FFFFFF87001ULL /* N^(2^32) */, 0X00078FFFFFFC3801ULL /* N^(2^33) */, 0X00078FFFFFFE1C01ULL /* N^(2^34) */, 0X00078FFFFFFF0E01ULL /* N^(2^35) */,
            0X00078FFFFFFF8701ULL /* N^(2^36) */, 0X00078FFFFFFFC381ULL /* N^(2^37) */, 0X00078FFFFFFFE1C1ULL /* N^(2^38) */, 0X00078FFFFFFFF0E1ULL /* N^(2^39) */,
            0X00078FFFFFFFF871ULL /* N^(2^40) */, 0X00078FFFFFFFFC39ULL /* N^(2^41) */, 0X00078FFFFFFFFE1DULL /* N^(2^42) */, 0X00078FFFFFFFFF0FULL /* N^(2^43) */,
            0X0000000000000000ULL /* N^(2^44) */, 0X0000000000000000ULL /* N^(2^45) */
        },

        .codelet64_twd =
        {
                          0X1P+0 /* psi^(0 ) */,    0X1.4A92E4B231248P+50 /* psi^(1 ) */,    0X1.EE0A2A1AED238P+49 /* psi^(2 ) */,     0X1.D485062E6B88P+45 /* psi^(3 ) */,
           0X1.B35843C449F14P+50 /* psi^(4 ) */,     0X1.A251D4755B17P+48 /* psi^(5 ) */,     0X1.96BF62FEC60FP+50 /* psi^(6 ) */,    0X1.D3866A1855168P+50 /* psi^(7 ) */,
             0X1.5EB1AD53664P+50 /* psi^(8 ) */,    0X1.197531FE0C448P+49 /* psi^(9 ) */,     0X1.D6A910687F28P+45 /* psi^(10) */,     0X1.AACCA12F1AAFP+48 /* psi^(11) */,
            0X1.F1E4394C3561P+48 /* psi^(12) */,     0X1.C0D74F59DB24P+50 /* psi^(13) */,    0X1.3DB665AF644E4P+50 /* psi^(14) */,    0X1.C910A47EABB38P+49 /* psi^(15) */,
                          0X1P+0 /* psi^(0 ) */,    0X1.EE0A2A1AED238P+49 /* psi^(2 ) */,    0X1.B35843C449F14P+50 /* psi^(4 ) */,     0X1.96BF62FEC60FP+50 /* psi^(6 ) */,
             0X1.5EB1AD53664P+50 /* psi^(8 ) */,     0X1.D6A910687F28P+45 /* psi^(10) */,     0X1.F1E4394C3561P+48 /* psi^(12) */,    0X1.3DB665AF644E4P+50 /* psi^(14) */,
            0X1.6574F08A018EP+48 /* psi^(16) */,    0X1.5700CADCF7A88P+49 /* psi^(18) */,     0X1.CF15E0FF9BE8P+49 /* psi^(20) */,    0X1.9F42FD81E822CP+50 /* psi^(22) */,
            0X1.4B4C536B2722P+50 /* psi^(24) */,    0X1.C0CCD6BDB1554P+50 /* psi^(26) */,    0X1.777B7946C0568P+49 /* psi^(28) */,    0X1.683332BFA0358P+50 /* psi^(30) */,
                          0X1P+0 /* psi^(0 ) */,     0X1.D485062E6B88P+45 /* psi^(3 ) */,     0X1.96BF62FEC60FP+50 /* psi^(6 ) */,    0X1.197531FE0C448P+49 /* psi^(9 ) */,
            0X1.F1E4394C3561P+48 /* psi^(12) */,    0X1.C910A47EABB38P+49 /* psi^(15) */,    0X1.5700CADCF7A88P+49 /* psi^(18) */,     0X1.15041015466AP+50 /* psi^(21) */,
            0X1.4B4C536B2722P+50 /* psi^(24) */,     0X1.B504C0EA3861P+49 /* psi^(27) */,    0X1.683332BFA0358P+50 /* psi^(30) */,    0X1.660723E7CA95CP+50 /* psi^(33) */,
            0X1.BE5AB68D8ADEP+47 /* psi^(36) */,     0X1.E5D3FA6BFB56P+49 /* psi^(39) */,    0X1.10FAED6AA6CE8P+50 /* psi^(42) */,     0X1.3E3544B991F5P+49 /* psi^(45) */,
                          0X1P+0 /* psi^(0 ) */,    0X1.B35843C449F14P+50 /* psi^(4 ) */,      0X1.5EB1AD53664P+50 /* psi^(8 ) */,     0X1.F1E4394C3561P+48 /* psi^(12) */,
                          0X1P+0 /* psi^(0 ) */,      0X1.5EB1AD53664P+50 /* psi^(8 ) */,     0X1.6574F08A018EP+48 /* psi^(16) */,     0X1.4B4C536B2722P+50 /* psi^(24) */,
                          0X1P+0 /* psi^(0 ) */,     0X1.F1E4394C3561P+48 /* psi^(12) */,     0X1.4B4C536B2722P+50 /* psi^(24) */,     0X1.BE5AB68D8ADEP+47 /* psi^(36) */
        },

        .codelet64_twd_inv =
        {
                          0X1P+0 /* psi_inv^(0 ) */,    0X1.855D0AF4492DCP+50 /* psi_inv^(1 ) */,    0X1.B9076D62FB588P+50 /* psi_inv^(2 ) */,    0X1.D186C42E91688P+50 /* psi_inv^(3 ) */,
            0X1.AEB7895444FFP+50 /* psi_inv^(4 ) */,    0X1.25F27AEF753A8P+50 /* psi_inv^(5 ) */,     0X1.665856333337P+50 /* psi_inv^(6 ) */,    0X1.BE50BEC0C4868P+50 /* psi_inv^(7 ) */,
           0X1.C55462F878D8CP+50 /* psi_inv^(8 ) */,    0X1.8BAB19EA4424CP+50 /* psi_inv^(9 ) */,    0X1.02C1A10701168P+50 /* psi_inv^(10) */,     0X1.D57E5CE4BBA9P+50 /* psi_inv^(11) */,
              0X1.1342A8650CP+41 /* psi_inv^(12) */,      0X1.CCA99C4CB4BP+47 /* psi_inv^(13) */,    0X1.6856F11CBA788P+49 /* psi_inv^(14) */,     0X1.F619F6EF474FP+48 /* psi_inv^(15) */,
                          0X1P+0 /* psi_inv^(0 ) */,    0X1.B9076D62FB588P+50 /* psi_inv^(2 ) */,     0X1.AEB7895444FFP+50 /* psi_inv^(4 ) */,     0X1.665856333337P+50 /* psi_inv^(6 ) */,
           0X1.C55462F878D8CP+50 /* psi_inv^(8 ) */,    0X1.02C1A10701168P+50 /* psi_inv^(10) */,       0X1.1342A8650CP+41 /* psi_inv^(12) */,    0X1.6856F11CBA788P+49 /* psi_inv^(14) */,
           0X1.2D1EEE807EFECP+50 /* psi_inv^(16) */,     0X1.6D614A1C4128P+47 /* psi_inv^(18) */,    0X1.AF9B4D7D434B4P+50 /* psi_inv^(20) */,    0X1.A60A252AB2638P+49 /* psi_inv^(22) */,
           0X1.56A6D07F866DCP+50 /* psi_inv^(24) */,    0X1.375F5753E8814P+50 /* psi_inv^(26) */,    0X1.AC34A92E4EA48P+50 /* psi_inv^(28) */,     0X1.97CF7C4E9A6CP+47 /* psi_inv^(30) */,
                          0X1P+0 /* psi_inv^(0 ) */,    0X1.D186C42E91688P+50 /* psi_inv^(3 ) */,     0X1.665856333337P+50 /* psi_inv^(6 ) */,    0X1.8BAB19EA4424CP+50 /* psi_inv^(9 ) */,
              0X1.1342A8650CP+41 /* psi_inv^(12) */,     0X1.F619F6EF474FP+48 /* psi_inv^(15) */,     0X1.6D614A1C4128P+47 /* psi_inv^(18) */,     0X1.3F9DA7D6CCC9P+50 /* psi_inv^(21) */,
           0X1.56A6D07F866DCP+50 /* psi_inv^(24) */,     0X1.6EA2C0AC0EE1P+49 /* psi_inv^(27) */,     0X1.97CF7C4E9A6CP+47 /* psi_inv^(30) */,    0X1.AC36C2BD9632CP+50 /* psi_inv^(33) */,
            0X1.2842435C9FD5P+50 /* psi_inv^(36) */,    0X1.B5E38EC24BE88P+50 /* psi_inv^(39) */,     0X1.12F409F85F76P+48 /* psi_inv^(42) */,      0X1.45A1E394F07P+50 /* psi_inv^(45) */,
                          0X1P+0 /* psi_inv^(0 ) */,     0X1.AEB7895444FFP+50 /* psi_inv^(4 ) */,    0X1.C55462F878D8CP+50 /* psi_inv^(8 ) */,       0X1.1342A8650CP+41 /* psi_inv^(12) */,
                          0X1P+0 /* psi_inv^(0 ) */,    0X1.C55462F878D8CP+50 /* psi_inv^(8 ) */,    0X1.2D1EEE807EFECP+50 /* psi_inv^(16) */,    0X1.56A6D07F866DCP+50 /* psi_inv^(24) */,
                          0X1P+0 /* psi_inv^(0 ) */,       0X1.1342A8650CP+41 /* psi_inv^(12) */,    0X1.56A6D07F866DCP+50 /* psi_inv^(24) */,     0X1.2842435C9FD5P+50 /* psi_inv^(36) */
        },

    },

    {
        .prime = (0X23ULL << 0X2DULL) + 1ULL,

        .coeff = 0X23ULL,

        .power_of_two = 0X2DULL,

        .transform = NEGACYCLIC,

        .magic = 0XEA0EA0EA0EA0B48FULL,

        .shift = 0X32ULL,

        .barrett = 0XEA0EA0EA0EA0BULL,

        .prime_inv =    0X1.D41D41D41D417P-51,

        .twiddle =
        {
            0X0002BF6CD9309773ULL /* w^(2^0 ) */, 0X000073375CBFFC82ULL /* w^(2^1 ) */, 0X00004D79F66833A0ULL /* w^(2^2 ) */, 0X0000B58576D93ECEULL /* w^(2^3 ) */,
            0X00016EE203029F34ULL /* w^(2^4 ) */, 0X00005F63AF8EB4E0ULL /* w^(2^5 ) */, 0X000107F79AC8C17CULL /* w^(2^6 ) */, 0X0000ED498577821DULL /* w^(2^7 ) */,
            0X0001194097D66639ULL /* w^(2^8 ) */, 0X0002B805A66DB67CULL /* w^(2^9 ) */, 0X0003C7FB09B67E2FULL /* w^(2^10) */, 0X00001488A0BA3F7DULL /* w^(2^11) */,
            0X0003F34C31D5E688ULL /* w^(2^12) */, 0X00031494417F42E0ULL /* w^(2^13) */, 0X0003C6FE6B795A0AULL /* w^(2^14) */, 0X00004BBEB2BBFFD6ULL /* w^(2^15) */,
            0X00011A5DF8C33CE4ULL /* w^(2^16) */, 0X0001A1574D96394CULL /* w^(2^17) */, 0X000004C3D1FE1471ULL /* w^(2^18) */, 0X000405ADE3545C2DULL /* w^(2^19) */,
            0X0002DB7200013D6DULL /* w^(2^20) */, 0X00039565063581CFULL /* w^(2^21) */, 0X0001E12A0BDA46AEULL /* w^(2^22) */, 0X00019A90C53DDFB1ULL /* w^(2^23) */,
            0X00006DD9DB8C085CULL /* w^(2^24) */, 0X0001FC1852C268A7ULL /* w^(2^25) */, 0X00001F7DA42C8755ULL /* w^(2^26) */, 0X0000534309EBC845ULL /* w^(2^27) */,
            0X0001192B90C8440CULL /* w^(2^28) */, 0X000076A9CC525A7FULL /* w^(2^29) */, 0X00009DD3FF4BFBA3ULL /* w^(2^30) */, 0X00005C507CC0F937ULL /* w^(2^31) */,
            0X0002C5A82D582C71ULL /* w^(2^32) */, 0X00019AA6C5E205C9ULL /* w^(2^33) */, 0X0003829082BCF6C4ULL /* w^(2^34) */, 0X000451B1B554CEA0ULL /* w^(2^35) */,
            0X0002BDF44DC55928ULL /* w^(2^36) */, 0X00043C1225E9D5C7ULL /* w^(2^37) */, 0X00024C1300CA9B00ULL /* w^(2^38) */, 0X0003CD91D24B52B9ULL /* w^(2^39) */,
            0X00001B2A738EA4EBULL /* w^(2^40) */, 0X00020619D2734286ULL /* w^(2^41) */, 0X000443899737254BULL /* w^(2^42) */, 0X00015E592BDEE28FULL /* w^(2^43) */,
            0X0004600000000000ULL /* w^(2^44) */, 0X0000000000000000ULL /* w^(2^45) */
        },

        .twiddle_inv =
        {
            0X0000BD6F983C6F19ULL /* w_inv^(2^0 ) */, 0X0001F3D5723BCED9ULL /* w_inv^(2^1 ) */, 0X00015E3B64B01433ULL /* w_inv^(2^2 ) */, 0X0000FEE07DA3A890ULL /* w_inv^(2^3 ) */,
            0X0002FD3C331FDC4EULL /* w_inv^(2^4 ) */, 0X00039B7635B00080ULL /* w_inv^(2^5 ) */, 0X000455CC2CDD5A44ULL /* w_inv^(2^6 ) */, 0X0003AF1C34C3CDCBULL /* w_inv^(2^7 ) */,
            0X0003759D36ABB14EULL /* w_inv^(2^8 ) */, 0X0001EB57DB64CB0CULL /* w_inv^(2^9 ) */, 0X00002DB080E217C9ULL /* w_inv^(2^10) */, 0X00001BF9DAEDA146ULL /* w_inv^(2^11) */,
            0X0001D0B854A09C0EULL /* w_inv^(2^12) */, 0X00003E79437C802FULL /* w_inv^(2^13) */, 0X0000F496ECB6C428ULL /* w_inv^(2^14) */, 0X0003059B2DC1B25FULL /* w_inv^(2^15) */,
            0X0001E9F57DA9DC88ULL /* w_inv^(2^16) */, 0X0001680AEB7C28DEULL /* w_inv^(2^17) */, 0X00004CD950F43284ULL /* w_inv^(2^18) */, 0X00044704F2B9CF22ULL /* w_inv^(2^19) */,
            0X000198F0C661F08BULL /* w_inv^(2^20) */, 0X0000A4DEA592850DULL /* w_inv^(2^21) */, 0X000245A5C157ECCBULL /* w_inv^(2^22) */, 0X00034EDA05142214ULL /* w_inv^(2^23) */,
            0X000324A85D178DFFULL /* w_inv^(2^24) */, 0X0001B3EC0BE7927BULL /* w_inv^(2^25) */, 0X0002432D44CBAF7CULL /* w_inv^(2^26) */, 0X0003456053797B68ULL /* w_inv^(2^27) */,
            0X0001EB9943C7D31AULL /* w_inv^(2^28) */, 0X0002ADC6CBA3D892ULL /* w_inv^(2^29) */, 0X0001744650C138A8ULL /* w_inv^(2^30) */, 0X000335621EE0D4B3ULL /* w_inv^(2^31) */,
            0X000373F877A64682ULL /* w_inv^(2^32) */, 0X0002AFBDC0582224ULL /* w_inv^(2^33) */, 0X00040C65F379C9AAULL /* w_inv^(2^34) */, 0X00031FE38B741C3DULL /* w_inv^(2^35) */,
            0X00004A3CC74F98CAULL /* w_inv^(2^36) */, 0X00039A0E00485F35ULL /* w_inv^(2^37) */, 0X00012B29B9B8FCEEULL /* w_inv^(2^38) */, 0X0003AB8FBF885DFFULL /* w_inv^(2^39) */,
            0X00021D972D8E4173ULL /* w_inv^(2^40) */, 0X00016FD475C7D2AEULL /* w_inv^(2^41) */, 0X0002FDEED03C480DULL /* w_inv^(2^42) */, 0X000301A6D4211D72ULL /* w_inv^(2^43) */,
            0X0004600000000000ULL /* w_inv^(2^44) */, 0X0000000000000000ULL /* w_inv^(2^45) */
        },

        .size_inv =
        {
            0X0000000000000001ULL /* N^(2^0 ) */, 0X0002300000000001ULL /* N^(2^1 ) */, 0X0003480000000001ULL /* N^(2^2 ) */, 0X0003D40000000001ULL /* N^(2^3 ) */,
            0X00041A0000000001ULL /* N^(2^4 ) */, 0X00043D0000000001ULL /* N^(2^5 ) */, 0X00044E8000000001ULL /* N^(2^6 ) */, 0X0004574000000001ULL /* N^(2^7 ) */,
            0X00045BA000000001ULL /* N^(2^8 ) */, 0X00045DD000000001ULL /* N^(2^9 ) */, 0X00045EE800000001ULL /* N^(2^10) */, 0X00045F7400000001ULL /* N^(2^11) */,
            0X00045FBA00000001ULL /* N^(2^12) */, 0X00045FDD00000001ULL /* N^(2^13) */, 0X00045FEE80000001ULL /* N^(2^14) */, 0X00045FF740000001ULL /* N^(2^15) */,
            0X00045FFBA0000001ULL /* N^(2^16) */, 0X00045FFDD0000001ULL /* N^(2^17) */, 0X00045FFEE8000001ULL /* N^(2^18) */, 0X00045FFF74000001ULL /* N^(2^19) */,
            0X00045FFFBA000001ULL /* N^(2^20) */, 0X00045FFFDD000001ULL /* N^(2^21) */, 0X00045FFFEE800001ULL /* N^(2^22) */, 0X00045FFFF7400001ULL /* N^(2^23) */,
            0X00045FFFFBA00001ULL /* N^(2^24) */, 0X00045FFFFDD00001ULL /* N^(2^25) */, 0X00045FFFFEE80001ULL /* N^(2^26) */, 0X00045FFFFF740001ULL /* N^(2^27) */,
            0X00045FFFFFBA0001ULL /* N^(2^28) */, 0X00045FFFFFDD0001ULL /* N^(2^29) */, 0X00045FFFFFEE8001ULL /* N^(2^30) */, 0X00045FFFFFF74001ULL /* N^(2^31) */,
            0X00045FFFFFFBA001ULL /* N^(2^32) */, 0X00045FFFFFFDD001ULL /* N^(2^33) */, 0X00045FFFFFFEE801ULL /* N^(2^34) */, 0X00045FFFFFFF7401ULL /* N^(2^35) */,
            0X00045FFFFFFFBA01ULL /* N^(2^36) */, 0X00045FFFFFFFDD01ULL /* N^(2^37) */, 0X00045FFFFFFFEE81ULL /* N^(2^38) */, 0X00045FFFFFFFF741ULL /* N^(2^39) */,
            0X00045FFFFFFFFBA1ULL /* N^(2^40) */, 0X00045FFFFFFFFDD1ULL /* N^(2^41) */, 0X00045FFFFFFFFEE9ULL /* N^(2^42) */, 0X00045FFFFFFFFF75ULL /* N^(2^43) */,
            0X00045FFFFFFFFFBBULL /* N^(2^44) */, 0X0000000000000000ULL /* N^(2^45) */
        },

        .codelet64_twd =
        {
                          0X1P+0 /* psi^(0 ) */,      0X1.260980654D8P+49 /* psi^(1 ) */,    0X1.E6C8E925A95C8P+49 /* psi^(2 ) */,     0X1.4526CD592267P+49 /* psi^(3 ) */,
             0X1.B2A738EA4EBP+44 /* psi^(4 ) */,    0X1.5AD9798258CE8P+49 /* psi^(5 ) */,     0X1.FCA10927E096P+49 /* psi^(6 ) */,     0X1.C84FEEADBC46P+47 /* psi^(7 ) */,
            0X1.030CE939A143P+49 /* psi^(8 ) */,    0X1.5CA35D75FA708P+49 /* psi^(9 ) */,     0X1.7BF15335292FP+48 /* psi^(10) */,     0X1.028F8841A458P+45 /* psi^(11) */,
            0X1.8D841EBBB82AP+47 /* psi^(12) */,     0X1.889008A32A72P+48 /* psi^(13) */,    0X1.AB289A4C6A358P+49 /* psi^(14) */,     0X1.659CD54B7BCCP+47 /* psi^(15) */,
                          0X1P+0 /* psi^(0 ) */,    0X1.E6C8E925A95C8P+49 /* psi^(2 ) */,      0X1.B2A738EA4EBP+44 /* psi^(4 ) */,     0X1.FCA10927E096P+49 /* psi^(6 ) */,
            0X1.030CE939A143P+49 /* psi^(8 ) */,     0X1.7BF15335292FP+48 /* psi^(10) */,     0X1.8D841EBBB82AP+47 /* psi^(12) */,    0X1.AB289A4C6A358P+49 /* psi^(14) */,
           0X1.10E265CDC952CP+50 /* psi^(16) */,     0X1.111115E52B9DP+50 /* psi^(18) */,     0X1.0BAC6624F789P+49 /* psi^(20) */,     0X1.291C23148E9FP+49 /* psi^(22) */,
            0X1.2AC271933F4CP+48 /* psi^(24) */,     0X1.98A70DD1F47DP+49 /* psi^(26) */,     0X1.F595BE050DB3P+48 /* psi^(28) */,     0X1.A3167F21EE8DP+49 /* psi^(30) */,
                          0X1P+0 /* psi^(0 ) */,     0X1.4526CD592267P+49 /* psi^(3 ) */,     0X1.FCA10927E096P+49 /* psi^(6 ) */,    0X1.5CA35D75FA708P+49 /* psi^(9 ) */,
            0X1.8D841EBBB82AP+47 /* psi^(12) */,     0X1.659CD54B7BCCP+47 /* psi^(15) */,     0X1.111115E52B9DP+50 /* psi^(18) */,     0X1.B9A2DA180B5EP+49 /* psi^(21) */,
            0X1.2AC271933F4CP+48 /* psi^(24) */,     0X1.2F7C66CC3266P+48 /* psi^(27) */,     0X1.A3167F21EE8DP+49 /* psi^(30) */,     0X1.B40B7ACF10DBP+49 /* psi^(33) */,
           0X1.54AA44A098158P+49 /* psi^(36) */,     0X1.DE0F5AB38934P+49 /* psi^(39) */,    0X1.5CE22D672F238P+49 /* psi^(42) */,     0X1.E22A3E35CE78P+46 /* psi^(45) */,
                          0X1P+0 /* psi^(0 ) */,      0X1.B2A738EA4EBP+44 /* psi^(4 ) */,     0X1.030CE939A143P+49 /* psi^(8 ) */,     0X1.8D841EBBB82AP+47 /* psi^(12) */,
                          0X1P+0 /* psi^(0 ) */,     0X1.030CE939A143P+49 /* psi^(8 ) */,    0X1.10E265CDC952CP+50 /* psi^(16) */,     0X1.2AC271933F4CP+48 /* psi^(24) */,
                          0X1P+0 /* psi^(0 ) */,     0X1.8D841EBBB82AP+47 /* psi^(12) */,     0X1.2AC271933F4CP+48 /* psi^(24) */,    0X1.54AA44A098158P+49 /* psi^(36) */
        },

        .codelet64_twd_inv =
        {
                          0X1P+0 /* psi_inv^(0 ) */,     0X1.2B29B9B8FCEEP+48 /* psi_inv^(1 ) */,    0X1.D5C7DFC42EFF8P+49 /* psi_inv^(2 ) */,     0X1.F7FB44E123ECP+46 /* psi_inv^(3 ) */,
           0X1.0ECB96C720B98P+49 /* psi_inv^(4 ) */,    0X1.5D58416322208P+49 /* psi_inv^(5 ) */,     0X1.9AB8A7F8951DP+49 /* psi_inv^(6 ) */,     0X1.9CC3A4BAB4F4P+48 /* psi_inv^(7 ) */,
            0X1.6FD475C7D2AEP+48 /* psi_inv^(8 ) */,     0X1.FFCC7792AF78P+46 /* psi_inv^(9 ) */,    0X1.0418A8B217854P+50 /* psi_inv^(10) */,    0X1.A1345A9CCB168P+49 /* psi_inv^(11) */,
             0X1.5FDD2AA82B8P+45 /* psi_inv^(12) */,       0X1.F6D786B81EP+41 /* psi_inv^(13) */,     0X1.59CEAFAA0936P+49 /* psi_inv^(14) */,     0X1.6538FD3D7F07P+49 /* psi_inv^(15) */,
                          0X1P+0 /* psi_inv^(0 ) */,    0X1.D5C7DFC42EFF8P+49 /* psi_inv^(2 ) */,    0X1.0ECB96C720B98P+49 /* psi_inv^(4 ) */,     0X1.9AB8A7F8951DP+49 /* psi_inv^(6 ) */,
            0X1.6FD475C7D2AEP+48 /* psi_inv^(8 ) */,    0X1.0418A8B217854P+50 /* psi_inv^(10) */,      0X1.5FDD2AA82B8P+45 /* psi_inv^(12) */,     0X1.59CEAFAA0936P+49 /* psi_inv^(14) */,
           0X1.7EF7681E24068P+49 /* psi_inv^(16) */,     0X1.6023A9E07FA8P+46 /* psi_inv^(18) */,     0X1.3AC5D27B3299P+49 /* psi_inv^(20) */,     0X1.A63BA531A1BAP+48 /* psi_inv^(22) */,
           0X1.0E926832E0598P+49 /* psi_inv^(24) */,    0X1.FB9D8C21B7948P+49 /* psi_inv^(26) */,     0X1.B6AB76BECFD6P+48 /* psi_inv^(28) */,     0X1.57D09D032806P+47 /* psi_inv^(30) */,
                          0X1P+0 /* psi_inv^(0 ) */,     0X1.F7FB44E123ECP+46 /* psi_inv^(3 ) */,     0X1.9AB8A7F8951DP+49 /* psi_inv^(6 ) */,     0X1.FFCC7792AF78P+46 /* psi_inv^(9 ) */,
             0X1.5FDD2AA82B8P+45 /* psi_inv^(12) */,     0X1.6538FD3D7F07P+49 /* psi_inv^(15) */,     0X1.6023A9E07FA8P+46 /* psi_inv^(18) */,     0X1.C8A4B1481A55P+49 /* psi_inv^(21) */,
           0X1.0E926832E0598P+49 /* psi_inv^(24) */,    0X1.B9016EAEDB028P+49 /* psi_inv^(27) */,     0X1.57D09D032806P+47 /* psi_inv^(30) */,     0X1.BBD5BEC96221P+49 /* psi_inv^(33) */,
            0X1.353520FD7927P+49 /* psi_inv^(36) */,    0X1.F0A3206854968P+49 /* psi_inv^(39) */,    0X1.06E3DCEB71618P+49 /* psi_inv^(42) */,     0X1.6F474493B4BEP+49 /* psi_inv^(45) */,
                          0X1P+0 /* psi_inv^(0 ) */,    0X1.0ECB96C720B98P+49 /* psi_inv^(4 ) */,     0X1.6FD475C7D2AEP+48 /* psi_inv^(8 ) */,      0X1.5FDD2AA82B8P+45 /* psi_inv^(12) */,
                          0X1P+0 /* psi_inv^(0 ) */,     0X1.6FD475C7D2AEP+48 /* psi_inv^(8 ) */,    0X1.7EF7681E24068P+49 /* psi_inv^(16) */,    0X1.0E926832E0598P+49 /* psi_inv^(24) */,
                          0X1P+0 /* psi_inv^(0 ) */,      0X1.5FDD2AA82B8P+45 /* psi_inv^(12) */,    0X1.0E926832E0598P+49 /* psi_inv^(24) */,     0X1.353520FD7927P+49 /* psi_inv^(36) */
        },

    },

    {
        .prime = (0X13ULL << 0X2EULL) + 1ULL,

        .coeff = 0X13ULL,

        .power_of_two = 0X2EULL,

        .transform = NEGACYCLIC,

        .magic = 0X35E50D79435E457FULL,

        .shift = 0X30ULL,

        .barrett = 0XD79435E50D791ULL,

        .prime_inv =    0X1.AF286BCA1AF23P-51,

        .twiddle =
        {
            0X000000004546B3DBULL /* w^(2^0 ) */, 0X0001B07AE81FF967ULL /* w^(2^1 ) */, 0X00037464660A4532ULL /* w^(2^2 ) */, 0X00008B66436EC4CEULL /* w^(2^3 ) */,
            0X0001231E8E6ACD66ULL /* w^(2^4 ) */, 0X00006AA1B0561904ULL /* w^(2^5 ) */, 0X00013D0891F13D70ULL /* w^(2^6 ) */, 0X00040F458F77A448ULL /* w^(2^7 ) */,
            0X000368081D516C2CULL /* w^(2^8 ) */, 0X00015DCB1E9B5B7DULL /* w^(2^9 ) */, 0X00030D582B68490AULL /* w^(2^10) */, 0X0002F7D6A920C5EEULL /* w^(2^11) */,
            0X00031CA1FBB76AEAULL /* w^(2^12) */, 0X00028F7FE411F109ULL /* w^(2^13) */, 0X00030A2D0C4099D0ULL /* w^(2^14) */, 0X00033FDAE81EECC1ULL /* w^(2^15) */,
            0X00018E7D8B1A52BBULL /* w^(2^16) */, 0X0004949389701F70ULL /* w^(2^17) */, 0X000262AD5A302880ULL /* w^(2^18) */, 0X00006FDA59F23EC4ULL /* w^(2^19) */,
            0X0001E86518A33B69ULL /* w^(2^20) */, 0X0000EFE9A4440923ULL /* w^(2^21) */, 0X0003CA2A3509110AULL /* w^(2^22) */, 0X00045AA0CA2E4C2BULL /* w^(2^23) */,
            0X0002CC9E5125EBC4ULL /* w^(2^24) */, 0X0001C46DEE8F3E34ULL /* w^(2^25) */, 0X0002D7C21B2EBC43ULL /* w^(2^26) */, 0X0000503CD93F99C0ULL /* w^(2^27) */,
            0X0000EF2925D76B50ULL /* w^(2^28) */, 0X00020AE0E5EF0D6EULL /* w^(2^29) */, 0X0001C4EA92C63C30ULL /* w^(2^30) */, 0X0001D535F9D306ABULL /* w^(2^31) */,
            0X00010CAD6AA28B5CULL /* w^(2^32) */, 0X000041977B4D8916ULL /* w^(2^33) */, 0X000179A126B203B0ULL /* w^(2^34) */, 0X000222FB79C4C14BULL /* w^(2^35) */,
            0X000294F8267C0165ULL /* w^(2^36) */, 0X000120E4802C0D61ULL /* w^(2^37) */, 0X0004251B4D56DA64ULL /* w^(2^38) */, 0X000177FE12F8B5D1ULL /* w^(2^39) */,
            0X00024FABFCA5C666ULL /* w^(2^40) */, 0X0000AA520BC11DF0ULL /* w^(2^41) */, 0X000300259B2F6894ULL /* w^(2^42) */, 0X000386F5CDFBCF69ULL /* w^(2^43) */,
            0X00038E56A0C585D9ULL /* w^(2^44) */, 0X0004C00000000000ULL /* w^(2^45) */
        },

        .twiddle_inv =
        {
            0X00013246713C60A9ULL /* w_inv^(2^0 ) */, 0X00032EDA561E3F1CULL /* w_inv^(2^1 ) */, 0X0004BF9F4DEFD16CULL /* w_inv^(2^2 ) */, 0X0003718D07697541ULL /* w_inv^(2^3 ) */,
            0X00043C5E37820E00ULL /* w_inv^(2^4 ) */, 0X0002A51A0106E724ULL /* w_inv^(2^5 ) */, 0X0001F046A878FEC4ULL /* w_inv^(2^6 ) */, 0X0003D03695378A39ULL /* w_inv^(2^7 ) */,
            0X000113491723F833ULL /* w_inv^(2^8 ) */, 0X00019C6AD79DCD90ULL /* w_inv^(2^9 ) */, 0X00049FEDA3C529DCULL /* w_inv^(2^10) */, 0X0003B3CBC8079AD1ULL /* w_inv^(2^11) */,
            0X0000CCD6F6B6BA79ULL /* w_inv^(2^12) */, 0X0001E898AE40C2FEULL /* w_inv^(2^13) */, 0X000102C12847FF31ULL /* w_inv^(2^14) */, 0X000168D14CA1BB53ULL /* w_inv^(2^15) */,
            0X0000CFDC282D2FE3ULL /* w_inv^(2^16) */, 0X00036B90DAE89441ULL /* w_inv^(2^17) */, 0X0004587B00D8814AULL /* w_inv^(2^18) */, 0X0002DA705CD02558ULL /* w_inv^(2^19) */,
            0X00049A4F881A0B92ULL /* w_inv^(2^20) */, 0X0002C5435D47C565ULL /* w_inv^(2^21) */, 0X000260B6523F5649ULL /* w_inv^(2^22) */, 0X0004A207EB6DACF6ULL /* w_inv^(2^23) */,
            0X0003932F76352F1DULL /* w_inv^(2^24) */, 0X00003C57F2828BA0ULL /* w_inv^(2^25) */, 0X0002F64E0169AEF8ULL /* w_inv^(2^26) */, 0X0002E53CD25D12AAULL /* w_inv^(2^27) */,
            0X0001B44F55741E41ULL /* w_inv^(2^28) */, 0X0001885379651D16ULL /* w_inv^(2^29) */, 0X0000BB1E9A749A0FULL /* w_inv^(2^30) */, 0X0003E72EB6DF1F13ULL /* w_inv^(2^31) */,
            0X00020273EE49CAB6ULL /* w_inv^(2^32) */, 0X0002E80D617ED30EULL /* w_inv^(2^33) */, 0X0001E2EAC6DB678EULL /* w_inv^(2^34) */, 0X0003E809615714FAULL /* w_inv^(2^35) */,
            0X00037EAAF9275168ULL /* w_inv^(2^36) */, 0X0002AE4683B4FC17ULL /* w_inv^(2^37) */, 0X00045736975D3C39ULL /* w_inv^(2^38) */, 0X0001CD973C6FD45FULL /* w_inv^(2^39) */,
            0X00021B4060A9C5FBULL /* w_inv^(2^40) */, 0X0000856FD7EF073EULL /* w_inv^(2^41) */, 0X000282782872B1FAULL /* w_inv^(2^42) */, 0X0000B9EED398743BULL /* w_inv^(2^43) */,
            0X000131A95F3A7A28ULL /* w_inv^(2^44) */, 0X0004C00000000000ULL /* w_inv^(2^45) */
        },

        .size_inv =
        {
            0X0000000000000001ULL /* N^(2^0 ) */, 0X0002600000000001ULL /* N^(2^1 ) */, 0X0003900000000001ULL /* N^(2^2 ) */, 0X0004280000000001ULL /* N^(2^3 ) */,
            0X0004740000000001ULL /* N^(2^4 ) */, 0X00049A0000000001ULL /* N^(2^5 ) */, 0X0004AD0000000001ULL /* N^(2^6 ) */, 0X0004B68000000001ULL /* N^(2^7 ) */,
            0X0004BB4000000001ULL /* N^(2^8 ) */, 0X0004BDA000000001ULL /* N^(2^9 ) */, 0X0004BED000000001ULL /* N^(2^10) */, 0X0004BF6800000001ULL /* N^(2^11) */,
            0X0004BFB400000001ULL /* N^(2^12) */, 0X0004BFDA00000001ULL /* N^(2^13) */, 0X0004BFED00000001ULL /* N^(2^14) */, 0X0004BFF680000001ULL /* N^(2^15) */,
            0X0004BFFB40000001ULL /* N^(2^16) */, 0X0004BFFDA0000001ULL /* N^(2^17) */, 0X0004BFFED0000001ULL /* N^(2^18) */, 0X0004BFFF68000001ULL /* N^(2^19) */,
            0X0004BFFFB4000001ULL /* N^(2^20) */, 0X0004BFFFDA000001ULL /* N^(2^21) */, 0X0004BFFFED000001ULL /* N^(2^22) */, 0X0004BFFFF6800001ULL /* N^(2^23) */,
            0X0004BFFFFB400001ULL /* N^(2^24) */, 0X0004BFFFFDA00001ULL /* N^(2^25) */, 0X0004BFFFFED00001ULL /* N^(2^26) */, 0X0004BFFFFF680001ULL /* N^(2^27) */,
            0X0004BFFFFFB40001ULL /* N^(2^28) */, 0X0004BFFFFFDA0001ULL /* N^(2^29) */, 0X0004BFFFFFED0001ULL /* N^(2^30) */, 0X0004BFFFFFF68001ULL /* N^(2^31) */,
            0X0004BFFFFFFB4001ULL /* N^(2^32) */, 0X0004BFFFFFFDA001ULL /* N^(2^33) */, 0X0004BFFFFFFED001ULL /* N^(2^34) */, 0X0004BFFFFFFF6801ULL /* N^(2^35) */,
            0X0004BFFFFFFFB401ULL /* N^(2^36) */, 0X0004BFFFFFFFDA01ULL /* N^(2^37) */, 0X0004BFFFFFFFED01ULL /* N^(2^38) */, 0X0004BFFFFFFFF681ULL /* N^(2^39) */,
            0X0004BFFFFFFFFB41ULL /* N^(2^40) */, 0X0004BFFFFFFFFDA1ULL /* N^(2^41) */, 0X0004BFFFFFFFFED1ULL /* N^(2^42) */, 0X0004BFFFFFFFFF69ULL /* N^(2^43) */,
            0X0004BFFFFFFFFFB5ULL /* N^(2^44) */, 0X0004BFFFFFFFFFDBULL /* N^(2^45) */
        },

        .codelet64_twd =
        {
                          0X1P+0 /* psi^(0 ) */,     0X1.77FE12F8B5D1P+48 /* psi^(1 ) */,     0X1.27D5FE52E333P+49 /* psi^(2 ) */,     0X1.0CEBA42DA89EP+49 /* psi^(3 ) */,
             0X1.54A417823BEP+47 /* psi^(4 ) */,     0X1.57C448E5BEECP+46 /* psi^(5 ) */,     0X1.F65218D337A6P+47 /* psi^(6 ) */,     0X1.2A0FAAA8AEFCP+50 /* psi^(7 ) */,
            0X1.8012CD97B44AP+49 /* psi^(8 ) */,     0X1.010420A2944CP+49 /* psi^(9 ) */,      0X1.F34E267B02DP+49 /* psi^(10) */,    0X1.28AE49851433CP+50 /* psi^(11) */,
            0X1.E4F154072A46P+49 /* psi^(12) */,     0X1.B6D222C66744P+46 /* psi^(13) */,     0X1.0059031D04A4P+48 /* psi^(14) */,    0X1.86E1C5FD39258P+49 /* psi^(15) */,
                          0X1P+0 /* psi^(0 ) */,     0X1.27D5FE52E333P+49 /* psi^(2 ) */,      0X1.54A417823BEP+47 /* psi^(4 ) */,     0X1.F65218D337A6P+47 /* psi^(6 ) */,
            0X1.8012CD97B44AP+49 /* psi^(8 ) */,      0X1.F34E267B02DP+49 /* psi^(10) */,     0X1.E4F154072A46P+49 /* psi^(12) */,     0X1.0059031D04A4P+48 /* psi^(14) */,
           0X1.C37AE6FDE7B48P+49 /* psi^(16) */,      0X1.06039F81368P+46 /* psi^(18) */,     0X1.09AC07B8F546P+50 /* psi^(20) */,     0X1.8550B77AE0C5P+49 /* psi^(22) */,
           0X1.1A57ED497BCA8P+50 /* psi^(24) */,    0X1.765331D94BD18P+49 /* psi^(26) */,     0X1.B56356AD4854P+48 /* psi^(28) */,    0X1.19655C3BE5E38P+49 /* psi^(30) */,
                          0X1P+0 /* psi^(0 ) */,     0X1.0CEBA42DA89EP+49 /* psi^(3 ) */,     0X1.F65218D337A6P+47 /* psi^(6 ) */,     0X1.010420A2944CP+49 /* psi^(9 ) */,
            0X1.E4F154072A46P+49 /* psi^(12) */,    0X1.86E1C5FD39258P+49 /* psi^(15) */,      0X1.06039F81368P+46 /* psi^(18) */,     0X1.9A8BF7496CA7P+48 /* psi^(21) */,
           0X1.1A57ED497BCA8P+50 /* psi^(24) */,     0X1.069F53BD1EE2P+49 /* psi^(27) */,    0X1.19655C3BE5E38P+49 /* psi^(30) */,     0X1.69FACDC2BA92P+48 /* psi^(33) */,
            0X1.DA2EC1530744P+47 /* psi^(36) */,     0X1.7687DF31DD66P+48 /* psi^(39) */,     0X1.1BBBDB804828P+45 /* psi^(42) */,     0X1.40F78D8B6F0AP+49 /* psi^(45) */,
                          0X1P+0 /* psi^(0 ) */,      0X1.54A417823BEP+47 /* psi^(4 ) */,     0X1.8012CD97B44AP+49 /* psi^(8 ) */,     0X1.E4F154072A46P+49 /* psi^(12) */,
                          0X1P+0 /* psi^(0 ) */,     0X1.8012CD97B44AP+49 /* psi^(8 ) */,    0X1.C37AE6FDE7B48P+49 /* psi^(16) */,    0X1.1A57ED497BCA8P+50 /* psi^(24) */,
                          0X1P+0 /* psi^(0 ) */,     0X1.E4F154072A46P+49 /* psi^(12) */,    0X1.1A57ED497BCA8P+50 /* psi^(24) */,     0X1.DA2EC1530744P+47 /* psi^(36) */
        },

        .codelet64_twd_inv =
        {
                          0X1P+0 /* psi_inv^(0 ) */,     0X1.CD973C6FD45FP+48 /* psi_inv^(1 ) */,    0X1.0DA03054E2FD8P+49 /* psi_inv^(2 ) */,     0X1.B49C964498E4P+47 /* psi_inv^(3 ) */,
            0X1.0ADFAFDE0E7CP+47 /* psi_inv^(4 ) */,    0X1.20A53016D2108P+50 /* psi_inv^(5 ) */,     0X1.B3AA0C136794P+48 /* psi_inv^(6 ) */,    0X1.BEDB4D30A8CF8P+49 /* psi_inv^(7 ) */,
            0X1.413C143958FDP+49 /* psi_inv^(8 ) */,    0X1.1407207977F5CP+50 /* psi_inv^(9 ) */,     0X1.6C7DE41DDD67P+49 /* psi_inv^(10) */,     0X1.74DC4E7E2E8BP+49 /* psi_inv^(11) */,
            0X1.9FD3E884A2F5P+49 /* psi_inv^(12) */,     0X1.A58F4EF3C556P+49 /* psi_inv^(13) */,    0X1.1F9B7C965367CP+50 /* psi_inv^(14) */,     0X1.217E8548E186P+48 /* psi_inv^(15) */,
                          0X1P+0 /* psi_inv^(0 ) */,    0X1.0DA03054E2FD8P+49 /* psi_inv^(2 ) */,     0X1.0ADFAFDE0E7CP+47 /* psi_inv^(4 ) */,     0X1.B3AA0C136794P+48 /* psi_inv^(6 ) */,
            0X1.413C143958FDP+49 /* psi_inv^(8 ) */,     0X1.6C7DE41DDD67P+49 /* psi_inv^(10) */,     0X1.9FD3E884A2F5P+49 /* psi_inv^(12) */,    0X1.1F9B7C965367CP+50 /* psi_inv^(14) */,
            0X1.73DDA730E876P+47 /* psi_inv^(16) */,     0X1.134226565D84P+49 /* psi_inv^(18) */,    0X1.EE6020881AA78P+49 /* psi_inv^(20) */,     0X1.27222123FDBFP+50 /* psi_inv^(22) */,
            0X1.9DFEB1C88B14P+47 /* psi_inv^(24) */,     0X1.689EE046B394P+49 /* psi_inv^(26) */,    0X1.E9744FAB3E2F8P+49 /* psi_inv^(28) */,    0X1.AB08A780B93F8P+49 /* psi_inv^(30) */,
                          0X1P+0 /* psi_inv^(0 ) */,     0X1.B49C964498E4P+47 /* psi_inv^(3 ) */,     0X1.B3AA0C136794P+48 /* psi_inv^(6 ) */,    0X1.1407207977F5CP+50 /* psi_inv^(9 ) */,
            0X1.9FD3E884A2F5P+49 /* psi_inv^(12) */,     0X1.217E8548E186P+48 /* psi_inv^(15) */,     0X1.134226565D84P+49 /* psi_inv^(18) */,    0X1.30822B504EDA8P+49 /* psi_inv^(21) */,
            0X1.9DFEB1C88B14P+47 /* psi_inv^(24) */,      0X1.1CD64C642D4P+49 /* psi_inv^(27) */,    0X1.AB08A780B93F8P+49 /* psi_inv^(30) */,     0X1.2287A7710A9EP+47 /* psi_inv^(33) */,
           0X1.854E54A95BD68P+49 /* psi_inv^(36) */,    0X1.78E0DB5C64848P+49 /* psi_inv^(39) */,     0X1.B55E910A3E77P+48 /* psi_inv^(42) */,     0X1.AA46B2DA49E4P+46 /* psi_inv^(45) */,
                          0X1P+0 /* psi_inv^(0 ) */,     0X1.0ADFAFDE0E7CP+47 /* psi_inv^(4 ) */,     0X1.413C143958FDP+49 /* psi_inv^(8 ) */,     0X1.9FD3E884A2F5P+49 /* psi_inv^(12) */,
                          0X1P+0 /* psi_inv^(0 ) */,     0X1.413C143958FDP+49 /* psi_inv^(8 ) */,     0X1.73DDA730E876P+47 /* psi_inv^(16) */,     0X1.9DFEB1C88B14P+47 /* psi_inv^(24) */,
                          0X1P+0 /* psi_inv^(0 ) */,     0X1.9FD3E884A2F5P+49 /* psi_inv^(12) */,     0X1.9DFEB1C88B14P+47 /* psi_inv^(24) */,    0X1.854E54A95BD68P+49 /* psi_inv^(36) */
        },
    }
};

#define MIN_COV_LEN			(1ULL << 7)
#define CRT3_MAX_CONV_LEN   (1ULL << 23)
#define CRT4_MAX_CONV_LEN   (1ULL << 43)

#else
    #error "Only 64-bit systems supported!"
#endif

#endif