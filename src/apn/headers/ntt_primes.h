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

    ap_dig_t magic;
    ap_dig_t shift;
    ap_dig_t barrett;

    double prime_inv;

    ap_dig_t twiddle[44];
    ap_dig_t twiddle_inv[44];
    ap_dig_t size_inv[44];

    double cdlt64_cyclic_fwd[60];
    double cdlt64_cyclic_inv[60];
    double cdlt64_negacyclic_fwd[60];
    double cdlt64_negacyclic_inv[60];

} ntt_prime_t;

static const ntt_prime_t NTT_PRIMES[4] =
{
    {
        .prime = (0X49ULL << 0X2CULL) + 1ULL,

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
            0X0003CB5D22355606ULL /* w^(2^40) */, 0X0003527D5D2B6128ULL /* w^(2^41) */, 0X000461B629D2CD9CULL /* w^(2^42) */, 0X0004900000000000ULL /* w^(2^43) */
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
            0X00038930EE96E16FULL /* w_inv^(2^40) */, 0X0003AE332E74FE2CULL /* w_inv^(2^41) */, 0X00002E49D62D3265ULL /* w_inv^(2^42) */, 0X0004900000000000ULL /* w_inv^(2^43) */
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
            0X00048FFFFFFFFB71ULL /* N^(2^40) */, 0X00048FFFFFFFFDB9ULL /* N^(2^41) */, 0X00048FFFFFFFFEDDULL /* N^(2^42) */, 0X00048FFFFFFFFF6FULL /* N^(2^43) */
        },

        .cdlt64_cyclic_fwd =
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

        .cdlt64_cyclic_inv =
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

        .cdlt64_negacyclic_fwd =
        {
                          0X1P+0 /* psi^(0 ) */,     0X1.C167D70BDE53P+48 /* psi^(1 ) */,     0X1.7FDBE1B9EB64P+48 /* psi^(2 ) */,    0X1.97A0BAE7C94F8P+49 /* psi^(3 ) */,
           0X1.BEDE3B0473898P+49 /* psi^(4 ) */,     0X1.52ACFC024553P+48 /* psi^(5 ) */,     0X1.1206D954946AP+48 /* psi^(6 ) */,     0X1.0FCB672AACC7P+50 /* psi^(7 ) */,
            0X1.E5AE911AAB03P+49 /* psi^(8 ) */,     0X1.41887045D5A8P+49 /* psi^(9 ) */,     0X1.69D4906E6D79P+48 /* psi^(10) */,    0X1.68A8568663098P+49 /* psi^(11) */,
            0X1.B0ECD5BBA084P+46 /* psi^(12) */,     0X1.867136349A16P+49 /* psi^(13) */,     0X1.28018FE8EBBEP+48 /* psi^(14) */,     0X1.A5D9C89F11E8P+48 /* psi^(15) */,
                          0X1P+0 /* psi^(0 ) */,     0X1.7FDBE1B9EB64P+48 /* psi^(2 ) */,    0X1.BEDE3B0473898P+49 /* psi^(4 ) */,     0X1.1206D954946AP+48 /* psi^(6 ) */,
            0X1.E5AE911AAB03P+49 /* psi^(8 ) */,     0X1.69D4906E6D79P+48 /* psi^(10) */,     0X1.B0ECD5BBA084P+46 /* psi^(12) */,     0X1.28018FE8EBBEP+48 /* psi^(14) */,
            0X1.A93EAE95B094P+49 /* psi^(16) */,     0X1.EFF8F86066EAP+49 /* psi^(18) */,     0X1.FE8DAA671ED8P+49 /* psi^(20) */,    0X1.0A30DE23BE238P+50 /* psi^(22) */,
            0X1.FCAC424109ADP+49 /* psi^(24) */,     0X1.916F3751EB48P+49 /* psi^(26) */,    0X1.23E10C81A5C88P+49 /* psi^(28) */,    0X1.98E29FD0C38F8P+49 /* psi^(30) */,
                          0X1P+0 /* psi^(0 ) */,    0X1.97A0BAE7C94F8P+49 /* psi^(3 ) */,     0X1.1206D954946AP+48 /* psi^(6 ) */,     0X1.41887045D5A8P+49 /* psi^(9 ) */,
            0X1.B0ECD5BBA084P+46 /* psi^(12) */,     0X1.A5D9C89F11E8P+48 /* psi^(15) */,     0X1.EFF8F86066EAP+49 /* psi^(18) */,     0X1.2B7FF74C6F1EP+49 /* psi^(21) */,
            0X1.FCAC424109ADP+49 /* psi^(24) */,     0X1.DB52A1F4107AP+48 /* psi^(27) */,    0X1.98E29FD0C38F8P+49 /* psi^(30) */,     0X1.06798F2DF2B4P+47 /* psi^(33) */,
            0X1.34EEC13D4DDEP+49 /* psi^(36) */,     0X1.5F6369BF8A78P+49 /* psi^(39) */,    0X1.606B7CAEACB68P+49 /* psi^(42) */,     0X1.43E50F67EBAEP+49 /* psi^(45) */,
                          0X1P+0 /* psi^(0 ) */,    0X1.BEDE3B0473898P+49 /* psi^(4 ) */,     0X1.E5AE911AAB03P+49 /* psi^(8 ) */,     0X1.B0ECD5BBA084P+46 /* psi^(12) */,
                          0X1P+0 /* psi^(0 ) */,     0X1.E5AE911AAB03P+49 /* psi^(8 ) */,     0X1.A93EAE95B094P+49 /* psi^(16) */,     0X1.FCAC424109ADP+49 /* psi^(24) */,
                          0X1P+0 /* psi^(0 ) */,     0X1.B0ECD5BBA084P+46 /* psi^(12) */,     0X1.FCAC424109ADP+49 /* psi^(24) */,     0X1.34EEC13D4DDEP+49 /* psi^(36) */
        },

        .cdlt64_negacyclic_inv =
        {
                          0X1P+0 /* psi_inv^(0 ) */,     0X1.E167CC1DE34EP+47 /* psi_inv^(1 ) */,     0X1.E41AD00C16F4P+46 /* psi_inv^(2 ) */,     0X1.1F179C66161FP+50 /* psi_inv^(3 ) */,
             0X1.936453DD7D9P+45 /* psi_inv^(4 ) */,     0X1.F4DC19FDBF16P+48 /* psi_inv^(5 ) */,    0X1.BD62A795EC838P+49 /* psi_inv^(6 ) */,     0X1.8BF9F38F354CP+49 /* psi_inv^(7 ) */,
           0X1.C498774B70B78P+49 /* psi_inv^(8 ) */,     0X1.A063C87A82F1P+48 /* psi_inv^(9 ) */,     0X1.C779AFCB9CA8P+46 /* psi_inv^(10) */,    0X1.0A65C75FDA154P+50 /* psi_inv^(11) */,
           0X1.A07194F197198P+49 /* psi_inv^(12) */,     0X1.3B490CD8C13AP+49 /* psi_inv^(13) */,    0X1.55C8AD40658D8P+49 /* psi_inv^(14) */,    0X1.B94669F2EF008P+49 /* psi_inv^(15) */,
                          0X1P+0 /* psi_inv^(0 ) */,     0X1.E41AD00C16F4P+46 /* psi_inv^(2 ) */,      0X1.936453DD7D9P+45 /* psi_inv^(4 ) */,    0X1.BD62A795EC838P+49 /* psi_inv^(6 ) */,
           0X1.C498774B70B78P+49 /* psi_inv^(8 ) */,     0X1.C779AFCB9CA8P+46 /* psi_inv^(10) */,    0X1.A07194F197198P+49 /* psi_inv^(12) */,    0X1.55C8AD40658D8P+49 /* psi_inv^(14) */,
            0X1.D719973A7F16P+49 /* psi_inv^(16) */,      0X1.092E6F0BB74P+48 /* psi_inv^(18) */,     0X1.B4BA8A7A51E6P+49 /* psi_inv^(20) */,     0X1.CF2906A2A694P+48 /* psi_inv^(22) */,
            0X1.3AED2B1042FAP+48 /* psi_inv^(24) */,     0X1.79CB64002F14P+46 /* psi_inv^(26) */,    0X1.13113EC2B2228P+49 /* psi_inv^(28) */,    0X1.0925FBC92C578P+50 /* psi_inv^(30) */,
                          0X1P+0 /* psi_inv^(0 ) */,     0X1.1F179C66161FP+50 /* psi_inv^(3 ) */,    0X1.BD62A795EC838P+49 /* psi_inv^(6 ) */,     0X1.A063C87A82F1P+48 /* psi_inv^(9 ) */,
           0X1.A07194F197198P+49 /* psi_inv^(12) */,    0X1.B94669F2EF008P+49 /* psi_inv^(15) */,      0X1.092E6F0BB74P+48 /* psi_inv^(18) */,    0X1.1E121BAF0A44CP+50 /* psi_inv^(21) */,
            0X1.3AED2B1042FAP+48 /* psi_inv^(24) */,     0X1.158837944DB3P+49 /* psi_inv^(27) */,    0X1.0925FBC92C578P+50 /* psi_inv^(30) */,      0X1.33F78DD128AP+43 /* psi_inv^(33) */,
            0X1.241EF37E5A38P+49 /* psi_inv^(36) */,    0X1.0A71CF5F78EDCP+50 /* psi_inv^(39) */,     0X1.9CF21DC41DCCP+46 /* psi_inv^(42) */,     0X1.0A8013CF713DP+50 /* psi_inv^(45) */,
                          0X1P+0 /* psi_inv^(0 ) */,      0X1.936453DD7D9P+45 /* psi_inv^(4 ) */,    0X1.C498774B70B78P+49 /* psi_inv^(8 ) */,    0X1.A07194F197198P+49 /* psi_inv^(12) */,
                          0X1P+0 /* psi_inv^(0 ) */,    0X1.C498774B70B78P+49 /* psi_inv^(8 ) */,     0X1.D719973A7F16P+49 /* psi_inv^(16) */,     0X1.3AED2B1042FAP+48 /* psi_inv^(24) */,
                          0X1P+0 /* psi_inv^(0 ) */,    0X1.A07194F197198P+49 /* psi_inv^(12) */,     0X1.3AED2B1042FAP+48 /* psi_inv^(24) */,     0X1.241EF37E5A38P+49 /* psi_inv^(36) */
        },

    },

    {
        .prime = (0X5DULL << 0X2CULL) + 1ULL,

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
            0X0004E5502DBC06A2ULL /* w^(2^40) */, 0X00029E0A92506E3EULL /* w^(2^41) */, 0X0000AE242D7BF62AULL /* w^(2^42) */, 0X0005D00000000000ULL /* w^(2^43) */
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
            0X00020EA8D11C9986ULL /* w_inv^(2^40) */, 0X000013572426D6BFULL /* w_inv^(2^41) */, 0X000521DBD28409D7ULL /* w_inv^(2^42) */, 0X0005D00000000000ULL /* w_inv^(2^43) */
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
            0X0005CFFFFFFFFA31ULL /* N^(2^40) */, 0X0005CFFFFFFFFD19ULL /* N^(2^41) */, 0X0005CFFFFFFFFE8DULL /* N^(2^42) */, 0X0005CFFFFFFFFF47ULL /* N^(2^43) */
        },

        .cdlt64_cyclic_fwd =
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

        .cdlt64_cyclic_inv =
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

        .cdlt64_negacyclic_fwd =
        {
                          0X1P+0 /* psi^(0 ) */,    0X1.4BB121F110134P+50 /* psi^(1 ) */,    0X1.054A913187EA4P+50 /* psi^(2 ) */,     0X1.28C407B3FC0FP+48 /* psi^(3 ) */,
            0X1.65F0832511D4P+46 /* psi^(4 ) */,      0X1.F08BF0497DAP+43 /* psi^(5 ) */,    0X1.1086DDD5C1124P+50 /* psi^(6 ) */,     0X1.65F0A9E58C6AP+49 /* psi^(7 ) */,
           0X1.39540B6F01A88P+50 /* psi^(8 ) */,    0X1.09434E110D978P+49 /* psi^(9 ) */,     0X1.F1D3591AC5C8P+47 /* psi^(10) */,     0X1.12B88AE960D9P+49 /* psi^(11) */,
           0X1.30164F3433EA8P+50 /* psi^(12) */,    0X1.094203CC8E608P+50 /* psi^(13) */,    0X1.43BC3E7BD72B8P+49 /* psi^(14) */,     0X1.9A2C3F748275P+49 /* psi^(15) */,
                          0X1P+0 /* psi^(0 ) */,    0X1.054A913187EA4P+50 /* psi^(2 ) */,     0X1.65F0832511D4P+46 /* psi^(4 ) */,    0X1.1086DDD5C1124P+50 /* psi^(6 ) */,
           0X1.39540B6F01A88P+50 /* psi^(8 ) */,     0X1.F1D3591AC5C8P+47 /* psi^(10) */,    0X1.30164F3433EA8P+50 /* psi^(12) */,    0X1.43BC3E7BD72B8P+49 /* psi^(14) */,
            0X1.4F054928371FP+49 /* psi^(16) */,    0X1.A4DCD8A146C78P+49 /* psi^(18) */,    0X1.2D1745C92EF84P+50 /* psi^(20) */,     0X1.C4C6122B6F23P+49 /* psi^(22) */,
             0X1.B26FEF6D108P+43 /* psi^(24) */,     0X1.4423D3468B0EP+48 /* psi^(26) */,      0X1.2A5A98A7291P+48 /* psi^(28) */,       0X1.8D033B959EP+40 /* psi^(30) */,
                          0X1P+0 /* psi^(0 ) */,     0X1.28C407B3FC0FP+48 /* psi^(3 ) */,    0X1.1086DDD5C1124P+50 /* psi^(6 ) */,    0X1.09434E110D978P+49 /* psi^(9 ) */,
           0X1.30164F3433EA8P+50 /* psi^(12) */,     0X1.9A2C3F748275P+49 /* psi^(15) */,    0X1.A4DCD8A146C78P+49 /* psi^(18) */,    0X1.2C58D0D192A34P+50 /* psi^(21) */,
             0X1.B26FEF6D108P+43 /* psi^(24) */,     0X1.02A27CBEEED2P+48 /* psi^(27) */,       0X1.8D033B959EP+40 /* psi^(30) */,    0X1.5DB6E64BB9914P+50 /* psi^(33) */,
           0X1.11CB2593EAAE8P+49 /* psi^(36) */,    0X1.045DDEC44B438P+50 /* psi^(39) */,     0X1.23B2B1AE885BP+49 /* psi^(42) */,     0X1.F945F354E659P+48 /* psi^(45) */,
                          0X1P+0 /* psi^(0 ) */,     0X1.65F0832511D4P+46 /* psi^(4 ) */,    0X1.39540B6F01A88P+50 /* psi^(8 ) */,    0X1.30164F3433EA8P+50 /* psi^(12) */,
                          0X1P+0 /* psi^(0 ) */,    0X1.39540B6F01A88P+50 /* psi^(8 ) */,     0X1.4F054928371FP+49 /* psi^(16) */,      0X1.B26FEF6D108P+43 /* psi^(24) */,
                          0X1P+0 /* psi^(0 ) */,    0X1.30164F3433EA8P+50 /* psi^(12) */,      0X1.B26FEF6D108P+43 /* psi^(24) */,    0X1.11CB2593EAAE8P+49 /* psi^(36) */
        },

        .cdlt64_negacyclic_inv =
        {
                          0X1P+0 /* psi_inv^(0 ) */,    0X1.1920C15460E98P+49 /* psi_inv^(1 ) */,      0X1.46581FA4639P+45 /* psi_inv^(2 ) */,     0X1.F2A5AF6B8908P+48 /* psi_inv^(3 ) */,
            0X1.F04A33D3F97DP+49 /* psi_inv^(4 ) */,    0X1.A53E8584173C8P+49 /* psi_inv^(5 ) */,    0X1.67EE9C7222424P+50 /* psi_inv^(6 ) */,     0X1.B4E96259B2A8P+48 /* psi_inv^(7 ) */,
            0X1.0754688E4CC3P+49 /* psi_inv^(8 ) */,    0X1.5EE2B2594F848P+50 /* psi_inv^(9 ) */,    0X1.A6B4BD703B9D8P+49 /* psi_inv^(10) */,       0X1.5FEC4A8953P+47 /* psi_inv^(11) */,
           0X1.620F495B56CA8P+49 /* psi_inv^(12) */,     0X1.6BC6519A903CP+50 /* psi_inv^(13) */,     0X1.00DE5E08DBFAP+49 /* psi_inv^(14) */,     0X1.3E380F9E0494P+48 /* psi_inv^(15) */,
                          0X1P+0 /* psi_inv^(0 ) */,      0X1.46581FA4639P+45 /* psi_inv^(2 ) */,     0X1.F04A33D3F97DP+49 /* psi_inv^(4 ) */,    0X1.67EE9C7222424P+50 /* psi_inv^(6 ) */,
            0X1.0754688E4CC3P+49 /* psi_inv^(8 ) */,    0X1.A6B4BD703B9D8P+49 /* psi_inv^(10) */,    0X1.620F495B56CA8P+49 /* psi_inv^(12) */,     0X1.00DE5E08DBFAP+49 /* psi_inv^(14) */,
             0X1.3572426D6BFP+44 /* psi_inv^(16) */,     0X1.1E130BCE0524P+47 /* psi_inv^(18) */,    0X1.28A210FD82684P+50 /* psi_inv^(20) */,    0X1.C44D4E5177A58P+49 /* psi_inv^(22) */,
            0X1.1B249497C71DP+49 /* psi_inv^(24) */,    0X1.4DE41387919DCP+50 /* psi_inv^(26) */,     0X1.D634DA6C1552P+49 /* psi_inv^(28) */,      0X1.14B156450CDP+50 /* psi_inv^(30) */,
                          0X1P+0 /* psi_inv^(0 ) */,     0X1.F2A5AF6B8908P+48 /* psi_inv^(3 ) */,    0X1.67EE9C7222424P+50 /* psi_inv^(6 ) */,    0X1.5EE2B2594F848P+50 /* psi_inv^(9 ) */,
           0X1.620F495B56CA8P+49 /* psi_inv^(12) */,     0X1.3E380F9E0494P+48 /* psi_inv^(15) */,     0X1.1E130BCE0524P+47 /* psi_inv^(18) */,     0X1.07B7DA3BA425P+49 /* psi_inv^(21) */,
            0X1.1B249497C71DP+49 /* psi_inv^(24) */,     0X1.2815BE90B528P+47 /* psi_inv^(27) */,      0X1.14B156450CDP+50 /* psi_inv^(30) */,     0X1.E956209FB984P+49 /* psi_inv^(33) */,
           0X1.296959D635BC4P+50 /* psi_inv^(36) */,    0X1.BD9A8ADB35168P+49 /* psi_inv^(39) */,    0X1.2339EDD490DD8P+49 /* psi_inv^(42) */,    0X1.2C90DACD7B9C4P+50 /* psi_inv^(45) */,
                          0X1P+0 /* psi_inv^(0 ) */,     0X1.F04A33D3F97DP+49 /* psi_inv^(4 ) */,     0X1.0754688E4CC3P+49 /* psi_inv^(8 ) */,    0X1.620F495B56CA8P+49 /* psi_inv^(12) */,
                          0X1P+0 /* psi_inv^(0 ) */,     0X1.0754688E4CC3P+49 /* psi_inv^(8 ) */,      0X1.3572426D6BFP+44 /* psi_inv^(16) */,     0X1.1B249497C71DP+49 /* psi_inv^(24) */,
                          0X1P+0 /* psi_inv^(0 ) */,    0X1.620F495B56CA8P+49 /* psi_inv^(12) */,     0X1.1B249497C71DP+49 /* psi_inv^(24) */,    0X1.296959D635BC4P+50 /* psi_inv^(36) */
        },

    },

    {
        .prime = (0X6FULL << 0X2CULL) + 1ULL,

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
            0X00020493F45CBC24ULL /* w^(2^40) */, 0X00054353204F8194ULL /* w^(2^41) */, 0X0002E8EFB06857E9ULL /* w^(2^42) */, 0X0006F00000000000ULL /* w^(2^43) */
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
            0X000052C1DE23CC65ULL /* w_inv^(2^40) */, 0X00014BEF7D068764ULL /* w_inv^(2^41) */, 0X000407104F97A818ULL /* w_inv^(2^42) */, 0X0006F00000000000ULL /* w_inv^(2^43) */
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
            0X0006EFFFFFFFF911ULL /* N^(2^40) */, 0X0006EFFFFFFFFC89ULL /* N^(2^41) */, 0X0006EFFFFFFFFE45ULL /* N^(2^42) */, 0X0006EFFFFFFFFF23ULL /* N^(2^43) */
        },

        .cdlt64_cyclic_fwd =
        {
                          0X1P+0 /* psi^(0 ) */,      0X1.146BB7932A1P+45 /* psi^(1 ) */,     0X1.DCE45983C1ADP+48 /* psi^(2 ) */,     0X1.68412CCB06E4P+50 /* psi^(3 ) */,
            0X1.0249FA2E5E12P+49 /* psi^(4 ) */,    0X1.ECDB97FA4F7A8P+49 /* psi^(5 ) */,    0X1.605B2A1A01FA8P+50 /* psi^(6 ) */,    0X1.11BA05848A76CP+50 /* psi^(7 ) */,
            0X1.50D4C813E065P+50 /* psi^(8 ) */,    0X1.3A2EFDFC85FA8P+49 /* psi^(9 ) */,     0X1.6A97AB5257A2P+47 /* psi^(10) */,    0X1.26C65066EA8A8P+49 /* psi^(11) */,
           0X1.19E59F6A688ECP+50 /* psi^(12) */,    0X1.168477514090CP+50 /* psi^(13) */,    0X1.AF9A26E76953CP+50 /* psi^(14) */,     0X1.F5050D5F82CEP+47 /* psi^(15) */,
                          0X1P+0 /* psi^(0 ) */,     0X1.DCE45983C1ADP+48 /* psi^(2 ) */,     0X1.0249FA2E5E12P+49 /* psi^(4 ) */,    0X1.605B2A1A01FA8P+50 /* psi^(6 ) */,
            0X1.50D4C813E065P+50 /* psi^(8 ) */,     0X1.6A97AB5257A2P+47 /* psi^(10) */,    0X1.19E59F6A688ECP+50 /* psi^(12) */,    0X1.AF9A26E76953CP+50 /* psi^(14) */,
           0X1.7477D8342BF48P+49 /* psi^(16) */,    0X1.265A83F98F808P+50 /* psi^(18) */,     0X1.B354725D415EP+49 /* psi^(20) */,     0X1.6B4750603ABFP+50 /* psi^(22) */,
           0X1.690420BE5E274P+50 /* psi^(24) */,     0X1.EB47972D8A95P+49 /* psi^(26) */,     0X1.A74F88770CE7P+50 /* psi^(28) */,     0X1.AE3402DF47E5P+49 /* psi^(30) */,
                          0X1P+0 /* psi^(0 ) */,     0X1.68412CCB06E4P+50 /* psi^(3 ) */,    0X1.605B2A1A01FA8P+50 /* psi^(6 ) */,    0X1.3A2EFDFC85FA8P+49 /* psi^(9 ) */,
           0X1.19E59F6A688ECP+50 /* psi^(12) */,     0X1.F5050D5F82CEP+47 /* psi^(15) */,    0X1.265A83F98F808P+50 /* psi^(18) */,     0X1.0BD9B47B679CP+46 /* psi^(21) */,
           0X1.690420BE5E274P+50 /* psi^(24) */,    0X1.7544E3F84698CP+50 /* psi^(27) */,     0X1.AE3402DF47E5P+49 /* psi^(30) */,    0X1.B35CA24366AFCP+50 /* psi^(33) */,
           0X1.3ADB02E8D0F74P+50 /* psi^(36) */,     0X1.548BF4F6EB13P+49 /* psi^(39) */,     0X1.8EAD0A95B50CP+50 /* psi^(42) */,     0X1.4AF7115D7EDFP+49 /* psi^(45) */,
                          0X1P+0 /* psi^(0 ) */,     0X1.0249FA2E5E12P+49 /* psi^(4 ) */,     0X1.50D4C813E065P+50 /* psi^(8 ) */,    0X1.19E59F6A688ECP+50 /* psi^(12) */,
                          0X1P+0 /* psi^(0 ) */,     0X1.50D4C813E065P+50 /* psi^(8 ) */,    0X1.7477D8342BF48P+49 /* psi^(16) */,    0X1.690420BE5E274P+50 /* psi^(24) */,
                          0X1P+0 /* psi^(0 ) */,    0X1.19E59F6A688ECP+50 /* psi^(12) */,    0X1.690420BE5E274P+50 /* psi^(24) */,    0X1.3ADB02E8D0F74P+50 /* psi^(36) */
        },

        .cdlt64_cyclic_inv =
        {
                          0X1P+0 /* psi_inv^(0 ) */,    0X1.323EA688C86F8P+50 /* psi_inv^(1 ) */,    0X1.C9CBFD20B81B8P+49 /* psi_inv^(2 ) */,     0X1.32C7F3EA5868P+46 /* psi_inv^(3 ) */,
            0X1.4B07788F3194P+46 /* psi_inv^(4 ) */,     0X1.1AEC701EE59EP+48 /* psi_inv^(5 ) */,    0X1.8CB868D2756B8P+49 /* psi_inv^(6 ) */,      0X1.B22554B1993P+45 /* psi_inv^(7 ) */,
            0X1.4BEF7D068764P+48 /* psi_inv^(8 ) */,     0X1.B97423347B2EP+49 /* psi_inv^(9 ) */,     0X1.42E2BE7F1505P+48 /* psi_inv^(10) */,    0X1.AB4264B849868P+50 /* psi_inv^(11) */,
           0X1.C4AB8DA2BEA28P+49 /* psi_inv^(12) */,    0X1.C6FA59AC2F4A8P+49 /* psi_inv^(13) */,    0X1.2B4AF80CE0FF8P+49 /* psi_inv^(14) */,    0X1.63CE062105E28P+49 /* psi_inv^(15) */,
                          0X1P+0 /* psi_inv^(0 ) */,    0X1.C9CBFD20B81B8P+49 /* psi_inv^(2 ) */,     0X1.4B07788F3194P+46 /* psi_inv^(4 ) */,    0X1.8CB868D2756B8P+49 /* psi_inv^(6 ) */,
            0X1.4BEF7D068764P+48 /* psi_inv^(8 ) */,     0X1.42E2BE7F1505P+48 /* psi_inv^(10) */,    0X1.C4AB8DA2BEA28P+49 /* psi_inv^(12) */,    0X1.2B4AF80CE0FF8P+49 /* psi_inv^(14) */,
            0X1.01C413E5EA06P+50 /* psi_inv^(16) */,      0X1.8CBB2312D59P+45 /* psi_inv^(18) */,     0X1.4434C12B2EE3P+49 /* psi_inv^(20) */,     0X1.8EAD0A95B50CP+50 /* psi_inv^(22) */,
            0X1.ACACDFB07E6DP+48 /* psi_inv^(24) */,     0X1.6E935797F817P+48 /* psi_inv^(26) */,    0X1.3ADB02E8D0F74P+50 /* psi_inv^(28) */,     0X1.44C6E99F0F95P+50 /* psi_inv^(30) */,
                          0X1P+0 /* psi_inv^(0 ) */,     0X1.32C7F3EA5868P+46 /* psi_inv^(3 ) */,    0X1.8CB868D2756B8P+49 /* psi_inv^(6 ) */,     0X1.B97423347B2EP+49 /* psi_inv^(9 ) */,
           0X1.C4AB8DA2BEA28P+49 /* psi_inv^(12) */,    0X1.63CE062105E28P+49 /* psi_inv^(15) */,      0X1.8CBB2312D59P+45 /* psi_inv^(18) */,     0X1.289CD7CC8ABBP+50 /* psi_inv^(21) */,
            0X1.ACACDFB07E6DP+48 /* psi_inv^(24) */,     0X1.8B246805B086P+49 /* psi_inv^(27) */,     0X1.44C6E99F0F95P+50 /* psi_inv^(30) */,    0X1.1382B2EE6F218P+49 /* psi_inv^(33) */,
            0X1.A74F88770CE7P+50 /* psi_inv^(36) */,    0X1.AE6ED55A7336CP+50 /* psi_inv^(39) */,     0X1.6B4750603ABFP+50 /* psi_inv^(42) */,     0X1.B105A653D0B6P+49 /* psi_inv^(45) */,
                          0X1P+0 /* psi_inv^(0 ) */,     0X1.4B07788F3194P+46 /* psi_inv^(4 ) */,     0X1.4BEF7D068764P+48 /* psi_inv^(8 ) */,    0X1.C4AB8DA2BEA28P+49 /* psi_inv^(12) */,
                          0X1P+0 /* psi_inv^(0 ) */,     0X1.4BEF7D068764P+48 /* psi_inv^(8 ) */,     0X1.01C413E5EA06P+50 /* psi_inv^(16) */,     0X1.ACACDFB07E6DP+48 /* psi_inv^(24) */,
                          0X1P+0 /* psi_inv^(0 ) */,    0X1.C4AB8DA2BEA28P+49 /* psi_inv^(12) */,     0X1.ACACDFB07E6DP+48 /* psi_inv^(24) */,     0X1.A74F88770CE7P+50 /* psi_inv^(36) */
        },

        .cdlt64_negacyclic_fwd =
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

        .cdlt64_negacyclic_inv =
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
            0X00057AC6B54D9900ULL /* w^(2^40) */, 0X00016574F08A018EULL /* w^(2^41) */, 0X0000000002C00000ULL /* w^(2^42) */, 0X0007900000000000ULL /* w^(2^43) */
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
            0X000715518BE1E363ULL /* w_inv^(2^40) */, 0X0004B47BBA01FBFBULL /* w_inv^(2^41) */, 0X00078FFFFD400001ULL /* w_inv^(2^42) */, 0X0007900000000000ULL /* w_inv^(2^43) */
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
            0X00078FFFFFFFF871ULL /* N^(2^40) */, 0X00078FFFFFFFFC39ULL /* N^(2^41) */, 0X00078FFFFFFFFE1DULL /* N^(2^42) */, 0X00078FFFFFFFFF0FULL /* N^(2^43) */
        },

        .cdlt64_cyclic_fwd =
        {
                          0X1P+0 /* psi^(0 ) */,    0X1.EE0A2A1AED238P+49 /* psi^(1 ) */,    0X1.B35843C449F14P+50 /* psi^(2 ) */,     0X1.96BF62FEC60FP+50 /* psi^(3 ) */,
             0X1.5EB1AD53664P+50 /* psi^(4 ) */,     0X1.D6A910687F28P+45 /* psi^(5 ) */,     0X1.F1E4394C3561P+48 /* psi^(6 ) */,    0X1.3DB665AF644E4P+50 /* psi^(7 ) */,
            0X1.6574F08A018EP+48 /* psi^(8 ) */,    0X1.5700CADCF7A88P+49 /* psi^(9 ) */,     0X1.CF15E0FF9BE8P+49 /* psi^(10) */,    0X1.9F42FD81E822CP+50 /* psi^(11) */,
            0X1.4B4C536B2722P+50 /* psi^(12) */,    0X1.C0CCD6BDB1554P+50 /* psi^(13) */,    0X1.777B7946C0568P+49 /* psi^(14) */,    0X1.683332BFA0358P+50 /* psi^(15) */,
                          0X1P+0 /* psi^(0 ) */,    0X1.B35843C449F14P+50 /* psi^(2 ) */,      0X1.5EB1AD53664P+50 /* psi^(4 ) */,     0X1.F1E4394C3561P+48 /* psi^(6 ) */,
            0X1.6574F08A018EP+48 /* psi^(8 ) */,     0X1.CF15E0FF9BE8P+49 /* psi^(10) */,     0X1.4B4C536B2722P+50 /* psi^(12) */,    0X1.777B7946C0568P+49 /* psi^(14) */,
                       0X1.6P+25 /* psi^(16) */,     0X1.BE5AB68D8ADEP+47 /* psi^(18) */,     0X1.1AB25F00F325P+49 /* psi^(20) */,     0X1.A3259415E5A8P+47 /* psi^(22) */,
            0X1.6DC222FF0203P+49 /* psi^(24) */,    0X1.E3765EABCD7A4P+50 /* psi^(26) */,     0X1.EAB9D0787278P+46 /* psi^(28) */,     0X1.AA43B55DD80AP+47 /* psi^(30) */,
                          0X1P+0 /* psi^(0 ) */,     0X1.96BF62FEC60FP+50 /* psi^(3 ) */,     0X1.F1E4394C3561P+48 /* psi^(6 ) */,    0X1.5700CADCF7A88P+49 /* psi^(9 ) */,
            0X1.4B4C536B2722P+50 /* psi^(12) */,    0X1.683332BFA0358P+50 /* psi^(15) */,     0X1.BE5AB68D8ADEP+47 /* psi^(18) */,    0X1.10FAED6AA6CE8P+50 /* psi^(21) */,
            0X1.6DC222FF0203P+49 /* psi^(24) */,    0X1.C27CBDF1FDD38P+49 /* psi^(27) */,     0X1.AA43B55DD80AP+47 /* psi^(30) */,     0X1.D9F5D5E512DDP+49 /* psi^(33) */,
           0X1.0A9CA55933808P+49 /* psi^(36) */,     0X1.4C9334A13764P+49 /* psi^(39) */,    0X1.F8EA1F0064188P+49 /* psi^(42) */,     0X1.19994A127558P+47 /* psi^(45) */,
                          0X1P+0 /* psi^(0 ) */,      0X1.5EB1AD53664P+50 /* psi^(4 ) */,     0X1.6574F08A018EP+48 /* psi^(8 ) */,     0X1.4B4C536B2722P+50 /* psi^(12) */,
                          0X1P+0 /* psi^(0 ) */,     0X1.6574F08A018EP+48 /* psi^(8 ) */,                0X1.6P+25 /* psi^(16) */,     0X1.6DC222FF0203P+49 /* psi^(24) */,
                          0X1P+0 /* psi^(0 ) */,     0X1.4B4C536B2722P+50 /* psi^(12) */,     0X1.6DC222FF0203P+49 /* psi^(24) */,    0X1.0A9CA55933808P+49 /* psi^(36) */
        },

        .cdlt64_cyclic_inv =
        {
                          0X1P+0 /* psi_inv^(0 ) */,    0X1.B9076D62FB588P+50 /* psi_inv^(1 ) */,     0X1.AEB7895444FFP+50 /* psi_inv^(2 ) */,     0X1.665856333337P+50 /* psi_inv^(3 ) */,
           0X1.C55462F878D8CP+50 /* psi_inv^(4 ) */,    0X1.02C1A10701168P+50 /* psi_inv^(5 ) */,       0X1.1342A8650CP+41 /* psi_inv^(6 ) */,    0X1.6856F11CBA788P+49 /* psi_inv^(7 ) */,
           0X1.2D1EEE807EFECP+50 /* psi_inv^(8 ) */,     0X1.6D614A1C4128P+47 /* psi_inv^(9 ) */,    0X1.AF9B4D7D434B4P+50 /* psi_inv^(10) */,    0X1.A60A252AB2638P+49 /* psi_inv^(11) */,
           0X1.56A6D07F866DCP+50 /* psi_inv^(12) */,    0X1.375F5753E8814P+50 /* psi_inv^(13) */,    0X1.AC34A92E4EA48P+50 /* psi_inv^(14) */,     0X1.97CF7C4E9A6CP+47 /* psi_inv^(15) */,
                          0X1P+0 /* psi_inv^(0 ) */,     0X1.AEB7895444FFP+50 /* psi_inv^(2 ) */,    0X1.C55462F878D8CP+50 /* psi_inv^(4 ) */,       0X1.1342A8650CP+41 /* psi_inv^(6 ) */,
           0X1.2D1EEE807EFECP+50 /* psi_inv^(8 ) */,    0X1.AF9B4D7D434B4P+50 /* psi_inv^(10) */,    0X1.56A6D07F866DCP+50 /* psi_inv^(12) */,    0X1.AC34A92E4EA48P+50 /* psi_inv^(14) */,
           0X1.E3FFFF5000004P+50 /* psi_inv^(16) */,     0X1.2842435C9FD5P+50 /* psi_inv^(18) */,    0X1.31675929B1BC8P+49 /* psi_inv^(20) */,    0X1.F8EA1F0064188P+49 /* psi_inv^(22) */,
           0X1.8AA2C3DD7F9CCP+50 /* psi_inv^(24) */,     0X1.6786F1ACF2A8P+50 /* psi_inv^(26) */,    0X1.0A9CA55933808P+49 /* psi_inv^(28) */,     0X1.853DE1DDB078P+47 /* psi_inv^(30) */,
                          0X1P+0 /* psi_inv^(0 ) */,     0X1.665856333337P+50 /* psi_inv^(3 ) */,       0X1.1342A8650CP+41 /* psi_inv^(6 ) */,     0X1.6D614A1C4128P+47 /* psi_inv^(9 ) */,
           0X1.56A6D07F866DCP+50 /* psi_inv^(12) */,     0X1.97CF7C4E9A6CP+47 /* psi_inv^(15) */,     0X1.2842435C9FD5P+50 /* psi_inv^(18) */,     0X1.12F409F85F76P+48 /* psi_inv^(21) */,
           0X1.8AA2C3DD7F9CCP+50 /* psi_inv^(24) */,     0X1.D54AB77CBC07P+50 /* psi_inv^(27) */,     0X1.853DE1DDB078P+47 /* psi_inv^(30) */,     0X1.57C494E8253EP+47 /* psi_inv^(33) */,
            0X1.EAB9D0787278P+46 /* psi_inv^(36) */,     0X1.2FD48771A2C4P+50 /* psi_inv^(39) */,     0X1.A3259415E5A8P+47 /* psi_inv^(42) */,     0X1.594151582EFEP+49 /* psi_inv^(45) */,
                          0X1P+0 /* psi_inv^(0 ) */,    0X1.C55462F878D8CP+50 /* psi_inv^(4 ) */,    0X1.2D1EEE807EFECP+50 /* psi_inv^(8 ) */,    0X1.56A6D07F866DCP+50 /* psi_inv^(12) */,
                          0X1P+0 /* psi_inv^(0 ) */,    0X1.2D1EEE807EFECP+50 /* psi_inv^(8 ) */,    0X1.E3FFFF5000004P+50 /* psi_inv^(16) */,    0X1.8AA2C3DD7F9CCP+50 /* psi_inv^(24) */,
                          0X1P+0 /* psi_inv^(0 ) */,    0X1.56A6D07F866DCP+50 /* psi_inv^(12) */,    0X1.8AA2C3DD7F9CCP+50 /* psi_inv^(24) */,     0X1.EAB9D0787278P+46 /* psi_inv^(36) */
        },

        .cdlt64_negacyclic_fwd =
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

        .cdlt64_negacyclic_inv =
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
        }
    }
};

#define MIN_CONV_LEN		(1ULL << 7)
#define CRT3_MAX_CONV_LEN   (1ULL << 23)
#define CRT4_MAX_CONV_LEN   (1ULL << 43)

#else
    #error "Only 64-bit systems supported!"
#endif

#endif