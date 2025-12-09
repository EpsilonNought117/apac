#include "../../../include/apac.h"
#include "x64_hidden_funcs.h"

// LUT for ((2 ^ 19) - 3 * (2 ^ 8)) / d9
// where d9 is nine most significant bits of 
// divisor

// (256 <= d9 < 512) as high bit of divisor is assumed to be set by caller
static const uint32_t UDIV21_RECIP_LUT[256] = 
{
      2045,   2037,   2029,   2021,   2013,   2005,   1998,   1990,
      1983,   1975,   1968,   1960,   1953,   1946,   1938,   1931,
      1924,   1917,   1910,   1903,   1896,   1889,   1883,   1876,
      1869,   1863,   1856,   1849,   1843,   1836,   1830,   1824,
      1817,   1811,   1805,   1799,   1792,   1786,   1780,   1774,
      1768,   1762,   1756,   1750,   1745,   1739,   1733,   1727,
      1722,   1716,   1710,   1705,   1699,   1694,   1688,   1683,
      1677,   1672,   1667,   1661,   1656,   1651,   1646,   1641,
      1636,   1630,   1625,   1620,   1615,   1610,   1605,   1600,
      1596,   1591,   1586,   1581,   1576,   1572,   1567,   1562,
      1558,   1553,   1548,   1544,   1539,   1535,   1530,   1526,
      1521,   1517,   1513,   1508,   1504,   1500,   1495,   1491,
      1487,   1483,   1478,   1474,   1470,   1466,   1462,   1458,
      1454,   1450,   1446,   1442,   1438,   1434,   1430,   1426,
      1422,   1418,   1414,   1411,   1407,   1403,   1399,   1396,
      1392,   1388,   1384,   1381,   1377,   1374,   1370,   1366,
      1363,   1359,   1356,   1352,   1349,   1345,   1342,   1338,
      1335,   1332,   1328,   1325,   1322,   1318,   1315,   1312,
      1308,   1305,   1302,   1299,   1295,   1292,   1289,   1286,
      1283,   1280,   1276,   1273,   1270,   1267,   1264,   1261,
      1258,   1255,   1252,   1249,   1246,   1243,   1240,   1237,
      1234,   1231,   1228,   1226,   1223,   1220,   1217,   1214,
      1211,   1209,   1206,   1203,   1200,   1197,   1195,   1192,
      1189,   1187,   1184,   1181,   1179,   1176,   1173,   1171,
      1168,   1165,   1163,   1160,   1158,   1155,   1153,   1150,
      1148,   1145,   1143,   1140,   1138,   1135,   1133,   1130,
      1128,   1125,   1123,   1121,   1118,   1116,   1113,   1111,
      1109,   1106,   1104,   1102,   1099,   1097,   1095,   1092,
      1090,   1088,   1086,   1083,   1081,   1079,   1077,   1074,
      1072,   1070,   1068,   1066,   1064,   1061,   1059,   1057,
      1055,   1053,   1051,   1049,   1047,   1044,   1042,   1040,
      1038,   1036,   1034,   1032,   1030,   1028,   1026,   1024
};

/**
* Research Paper: Improved Division by Invariant Integers
* Authors: Neils Moller and Torbjorn Granlund
*/
apn_seg_t recip_word_2by1_x64(apn_seg_t dvsr)
{
    // pre-requisite condition
    APAC_ASSERT((dvsr & (1ULL << 63)) != 0);

    // value to return
    apn_seg_t recip = 0;

    apn_seg_t d0 = dvsr & 1ULL;     // d0 = dvsr (mod 2)
    apn_seg_t d9 = dvsr >> 55;      // d9 = floor(dvsr * (2 ^ -55))
    apn_seg_t d40 = (dvsr >> 24) + 1ULL;    // d40 = ceil(dvsr * (2 ^ -24))
    apn_seg_t d63 = (dvsr >> 1) + 1ULL;     // d63 = ceil(dvsr / 2)

    // done via table look-up, at most 11-bit value
    apn_seg_t v0 = (apn_seg_t)UDIV21_RECIP_LUT[d9 - 256];   // v0 = ((2 ^ 19) - 3 * (2 ^ 8)) / d9
    
    // v1 = (v0 * (2 ^ 11)) - (v0 * v0 * d40) - 1
    // v0 is 11-bits
    // d40 is at most 41 bits (2 ^ 40)
    // ((v0 * v0 * d40) >> 40) is hence at most (63 - 40) = 23-bits wide
    // however the actual value is less than max(v0) * (2 ^ 11)
    // hence v1 is at-most 21-bits (lesser in practice)
    apn_seg_t v1 = (v0 << 11) - ((v0 * v0 * d40) >> 40) - 1;

    // v2 = (v1 * (2 ^ 13)) + (v1 * ((2 ^ 60) - v1 * d40)) / (2 ^ 47)
    // in theory, v1 * d40 is (41 + 21 = 63 bits wide)
    // but it is always less than (2 ^ 60)
    // ((2 ^ 60) - v1 * d40) is always less than (2 ^ 43 - 1) (as per the paper)
    // therefore (v1 * ((2 ^ 60) - v1 * d40)) is at most 64-bits, then shifted by 47-bits
    // hence v2 is at most max_bits(v1) + 13 = 43 (v1 <= 2 ^ 21)
    apn_seg_t v2 = (v1 << 13) + ((v1 * ((1ULL << 60) - v1 * d40)) >> 47);
  
    /*
    *   Common comments for both sections
    * 
    *   e = (2 ^ 96) - (v2 * d63) - floor(v2 / 2) * d0
    *   e is at most 96-bits, but only the lower 64-bits are needed
    *   v3 is truncated to lower 64-bits and so is v4
    */

#if defined(_MSC_VER)

    uint64_t high64 = 0, low64 = 0;
    uint64_t hword = 0, lword = 0;
    uint8_t borrow = 0, carry = 0;
    uint64_t temp = 0;
    
    // (2 ^ 96) - (v2 * d63)

    low64 = _umul128(v2, d63, &high64);
    hword = (1ULL << 32);

    temp = (v2 >> 1) * d0;
    carry = _addcarry_u64(carry, lword, temp, &lword);
    carry = _addcarry_u64(carry, hword, 0, &hword); /* No carry-out should arise here */

    // we only need the lower 64-bits, so no point to do the high64 SBB instr
    borrow = _subborrow_u64(borrow, lword, low64, &lword);

    apn_seg_t e = (apn_seg_t)lword; // select the lower 64-bits
    
    low64 = _umul128(e, v2, &high64);
    high64 >>= 1;
    temp = v2 << 31;
    apn_seg_t v3 = high64 + temp; // this might overflow, but we only need lower 64-bits
    
    low64 = _umul128(v3 + 1ULL, dvsr, &high64);
    high64 += dvsr;
    apn_seg_t v4 = v3 - high64;

#elif defined(__GNUC__)  || defined(__clang__)

    // exact same as the MSVC code

    uint64_t e = (uint64_t)((__uint128_t)1 << 96) - ((__uint128_t)v2 * d63) + ((v2 >> 1) * d0); // discard high 64-bits
    uint64_t v3 = (uint64_t)(((__uint128_t)v2 << 31) + (((__uint128_t)e * v2) >> 65)); // v3 (mod (2 ^ 64))
    apn_seg_t v4 = (apn_seg_t)(((__uint128_t)v3 - ((((__uint128_t)v3 + 0xFFFFFFFFFFFFFFFF /* (2 ^ 64) - 1 */ + 2) * dvsr) >> 64)));

#else
    #error "Unknown Compiler!"
#endif

    return v4;
}

/**
* Research Paper: Improved Division by Invariant Integers
* Authors: Neils Moller and Torbjorn Granlund
*/
apn_seg_t recip_word_3by2_x64(apn_seg_t dvsr1, apn_seg_t dvsr0)
{
    // get recip word of dvsr1
    apn_seg_t v = recip_word_2by1_x64(dvsr1);

#if defined(_MSC_VER)

    uint64_t high64 = 0, low64 = 0; // p = high64 * (2 ^ 64) + low64
    uint8_t carry = 0;

    low64 = _umul128(v, dvsr1, &high64);    // p = (d1 * v) (mod 2 ^ 64)
    carry = _addcarry_u64(carry, low64, dvsr0, &low64); // p = (p + d0) (mod 2 ^ 64)

    if (carry)  // if overflow
    {
        v--; // v = v - 1
        
        if (low64 >= dvsr1)
        {
            v--;            // same here
            low64 -= dvsr1; // p = p - d1
        }

        low64 -= dvsr1; // this results in borrow (carry) out
    }

    uint64_t hword = 0, lword = 0;
    
    lword = _umul128(v, dvsr0, &hword); // <t1, t0> = v * d0
    carry = 0;      
    carry = _addcarry_u64(carry, low64, hword, &low64); // p = (p + t1) (mod 2 ^ 64)

    if (carry)
    {
        v--;

        // if (<p, t0> >= <d1, d0>)
        if ((low64 > dvsr1) || (lword > dvsr0) || ((low64 == dvsr1 && lword == dvsr0)))
        {
            v--;
        }
    }

#elif defined(__GNUC__) || defined(__clang__)

    // exact same as MSVC code here

    uint64_t prod = (uint64_t)(((__uint128_t)v * dvsr1));
    prod = prod + dvsr0;

    if (prod < dvsr0)
    {
        v--;
        
        if (prod >= dvsr1)
        {
            v--;
            prod -= dvsr1;
        }

        prod -= dvsr1;
    }

    __uint128_t val = (__uint128_t)v * dvsr0;

    prod += (uint64_t)(val >> 64);

    if (prod < (uint64_t)(val >> 64))
    {
        v--;

        if ((((__uint128_t)prod << 64) | (val >> 64)) >= (((__uint128_t)dvsr1 << 64) | dvsr0))
        {
            v--;
        }
    }

#else
    #error "Unknown Compiler!"
#endif

    return v;
}