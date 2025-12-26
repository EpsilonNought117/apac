#include "hidden_div.h"

/**
* Research Paper: Improved Division by Invariant Integers
* Authors: Neils Moller and Torbjorn Granlund
*/
apn_seg_t recip_word64_2by1(apn_seg_t dvsr)
{
    // pre-requisite condition
    APAC_ASSERT(dvsr & (APN_SEG_HIGH_BIT));

    // LUT for ((2 ^ 19) - 3 * (2 ^ 8)) / d9
    // where d9 is nine most significant bits of 
    // divisor

    // (256 <= d9 <= 511) as high bit of divisor is assumed to be set by caller
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

    apn_seg_t recip = 0;                // value to return
    apn_seg_t d0 = dvsr & 1;            // d0 = dvsr (mod 2)
    apn_seg_t d9 = dvsr >> 55;          // d9 = floor(dvsr * (2 ^ -55))

    apn_seg_t d40 = (dvsr >> 24) + 1;   // d40 = floor(dvsr * (2 ^ -24)) + 1
    apn_seg_t d63 = (dvsr >> 1) + d0;   // d63 = floor(dvsr / 2)
   
    apn_seg_t v0 = (apn_seg_t)UDIV21_RECIP_LUT[d9 - 256];
    apn_seg_t v1 = (v0 << 11) - ((v0 * v0 * d40) >> 40) - 1;
    apn_seg_t v2 = (v1 << 13) + ((v1 * ((1ULL << 60) - v1 * d40)) >> 47);
    apn_seg_t e = (v2 >> 1) * d0 - v2 * d63;

    uint64_t high64 = 0, low64 = 0;
    uint8_t carry = 0;
    
    // common part ends here

#if defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64))

    apn_seg_t v3 = (__umulh(e, v2) >> 1) + (v2 << 31);

    low64 = _umul128(v3, dvsr, &high64);
    carry = _addcarry_u64(carry, dvsr, low64, &low64);

    apn_seg_t v4 = v3 - (high64 + dvsr + carry);

#elif defined(__GNUC__) || defined(__clang__)

    apn_seg_t v3 = (apn_seg_t)((((__uint128_t)v2 << 31) + (((__uint128_t)e * v2) >> 65)));
    __uint128_t prod = (__uint128_t)v3 * dvsr;

    low64 = (uint64_t)prod;
    high64 = (uint64_t)(prod >> 64);
    carry = ((low64 + dvsr) < low64);

    apn_seg_t v4 = v3 - (high64 + dvsr + carry);

#else

    #error "Unknown Compiler!"

#endif

    return v4;
}

/**
* Research Paper: Improved Division by Invariant Integers
* Authors: Neils Moller and Torbjorn Granlund
*/
apn_seg_t recip_word64_3by2(apn_seg_t dvsr1, apn_seg_t dvsr0)
{
    /* Initial reciprocal approximation */
    apn_seg_t v = recip_word64_2by1(dvsr1);

    uint64_t p;

    /* p = v*dvsr1 + dvsr0 */
    p = v * dvsr1;
    p += dvsr0;

    {
        uint64_t c = (p < dvsr0);
        uint64_t d = c & (p >= dvsr1);

        v -= c + d;
        p -= dvsr1 * (c + d);
    }

    uint64_t t0, t1;

#if defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64))

    t0 = _umul128(v, dvsr0, &t1);

#elif defined(__GNUC__) || defined(__clang__)

    __uint128_t prod = (__uint128_t)v * dvsr0;
    t0 = (uint64_t)prod;
    t1 = (uint64_t)(prod >> 64);

#else

    #error "Unknown Compiler!"

#endif

    p += t1;

    {
        uint64_t c = (p < t1);
        uint64_t d = c & ((p > dvsr1) | ((p == dvsr1) & (t0 >= dvsr0)));

        v -= c + d;
    }

    return v;
}

/**
* Research Paper: Improved Division by Invariant Integers
* Authors: Neils Moller and Torbjorn Granlund
*/
apn_seg_t udiv64_2by1(
    apn_seg_t divd1,
    apn_seg_t divd0,
    apn_seg_t dvsr,
    apn_seg_t recip,
    apn_seg_t* rmdr
)
{
    APAC_ASSERT(divd1 < dvsr);
    APAC_ASSERT(dvsr & APN_SEG_HIGH_BIT);

    uint64_t q0, q1;
    uint64_t r;

#if defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64))

    /* <q1,q0> = recip * divd1 */
    q0 = _umul128(divd1, recip, &q1);

    /* <q1,q0> += <divd1,divd0> */
    uint8_t c = 0;
    c = _addcarry_u64(c, q0, divd0, &q0);
    _addcarry_u64(c, q1, divd1, &q1);

#elif defined(__GNUC__) || defined(__clang__)

    __uint128_t q;

    q = (__uint128_t)recip * divd1;
    q += ((__uint128_t)divd1 << APN_SEG_BITS) | divd0;

    q1 = (uint64_t)(q >> APN_SEG_BITS);
    q0 = (uint64_t)q;

#else

    #error "Unknown Compiler!"

#endif

    /* ---- Common path from here ---- */

    q1++;
    r = divd0 - q1 * dvsr;

    /* First correction */
    {
        uint64_t m = (r > q0);
        q1 -= m;
        r += dvsr * m;
    }

    /* Second correction */
    {
        uint64_t m = (r >= dvsr);
        q1 += m;
        r -= dvsr * m;
    }

    *rmdr = (apn_seg_t)r;
    return (apn_seg_t)q1;
}

/**
* Research Paper: Improved Division by Invariant Integers
* Authors: Neils Moller and Torbjorn Granlund
*/
apn_seg_t udiv64_3by2_quot(
    apn_seg_t divd2,
    apn_seg_t divd1,
    apn_seg_t divd0,
    apn_seg_t dvsr1,
    apn_seg_t dvsr0,
    apn_seg_t recip
)
{
    APAC_ASSERT((dvsr1 > divd2) || (dvsr1 == divd2 && dvsr0 > divd1));
    APAC_ASSERT(dvsr1 & (APN_SEG_HIGH_BIT));

    // <u2, u1, u0> = <divd2, divd1, divd0>
    // <d1, d0> = <dvsr1, dvsr0>
    // v = recip

#if defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64))

    apn_seg_t q[2] = { 0 };
    apn_seg_t r[2] = { 0 };
    uint8_t carry = 0;

    q[0] = _umul128(recip, divd2, &q[1]);
    carry = _addcarry_u64(carry, q[0], divd1, &q[0]);
    carry = _addcarry_u64(carry, q[1], divd2, &q[1]);

    uint64_t high64 = 0, low64 = 0;

    carry = 0;
    low64 = _umul128(divd1, dvsr1, &high64);

    /* carry now acts as borrow */
    // r1 = (u1 - q1 * d1) (mod 2 ^ (APN_SEG_BITS)) 
    carry = _subborrow_u64(carry, r[0], low64, &r[0]);
    carry = _subborrow_u64(carry, r[1], high64, &r[1]);

    uint64_t t[2] = { 0 };

    // <t1, t0> = d0 * q1
    t[0] = _umul128(divd0, q[1], &t[1]);
    carry = 0;

    carry = _subborrow_u64(carry, divd0, t[0], &low64);
    carry = _subborrow_u64(carry, r[1], t[1], &high64);

    carry = 0;

    carry = _subborrow_u64(carry, low64, dvsr0, &r[0]);
    carry = _subborrow_u64(carry, high64, dvsr1, &r[1]);

    q[1]++;

    if (r[1] >= q[0])
    {
        q[1]--;

        carry = 0;
        carry = _addcarry_u64(carry, r[0], dvsr0, &r[0]);
        carry = _addcarry_u64(carry, r[1], dvsr1, &r[1]);
    }

    if ((r[1] > dvsr1) || (r[0] > dvsr0) || (r[1] == dvsr1 && r[0] == dvsr0))
    {
        q[1]++;

        carry = 0;
        carry = _subborrow_u64(carry, r[0], dvsr0, &r[0]);
        carry = _subborrow_u64(carry, r[1], dvsr1, &r[1]);
    }

    apn_seg_t q1 = (apn_seg_t)q[1];

#elif defined(__GNUC__) || defined(__clang__)

    __uint128_t q = 0, r = 0;

    q = (__uint128_t)recip * divd2;
    q += (((__uint128_t)divd2 << (APN_SEG_BITS)) | (__uint128_t)divd1);		// <q1, q0> += <u2, u1>
    r = (__uint128_t)divd1 - (q >> (APN_SEG_BITS)) * dvsr1;					// r = (u1 - q1 * d1) (mod 2 ^ (APN_SEG_BITS)) 
    __uint128_t t = (q >> (APN_SEG_BITS)) * dvsr0;							// <t1, t0> = d0 * q1

    uint64_t r1 = (r >> (APN_SEG_BITS));
    __uint128_t d = (((__uint128_t)dvsr1 << (APN_SEG_BITS)) | (__uint128_t)dvsr0);
    r = (((__uint128_t)r1 << (APN_SEG_BITS)) | (__uint128_t)divd0) - t - d;

    uint64_t q1 = (uint64_t)(q >> (APN_SEG_BITS));
    uint64_t q0 = (uint64_t)q;

    if (r1 >= q0)
    {
        q1--;
        r += d;
    }

    if (r >= d)
    {
        q1++;
        r -= d;
    }

#else

    #error "Unknown Compiler!"

#endif

    return (apn_seg_t)q1;
}