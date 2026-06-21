#include "../headers/hidden_div.h"

/**
 * Algorithm 2: RECIPROCAL_WORD
 *
 * Compute the reciprocal floor((2^128 - 1) / d) - 2^64
 * for 64-bit machines.
 *
 * Reference:
 *   Niels Moeller, Torbjorn Granlund
 *   "Improved Division by Invariant Integers"
 */
apn_dig_t apn_recip_word_2by1(
    apn_dig_t d
)
{
    /* Preconditions from the paper */
    APAC_ASSERT(d & APN_DIG_HIGH_BIT);

#if defined(APAC_64BIT_PLATFORM)

    /* LUT for ((2^19) - 3*(2^8)) / d9 */
    static const uint16_t D9_LUT[256] =
    {
        2045, 2037, 2029, 2021, 2013, 2005, 1998, 1990,
        1983, 1975, 1968, 1960, 1953, 1946, 1938, 1931,
        1924, 1917, 1910, 1903, 1896, 1889, 1883, 1876,
        1869, 1863, 1856, 1849, 1843, 1836, 1830, 1824,
        1817, 1811, 1805, 1799, 1792, 1786, 1780, 1774,
        1768, 1762, 1756, 1750, 1745, 1739, 1733, 1727,
        1722, 1716, 1710, 1705, 1699, 1694, 1688, 1683,
        1677, 1672, 1667, 1661, 1656, 1651, 1646, 1641,
        1636, 1630, 1625, 1620, 1615, 1610, 1605, 1600,
        1596, 1591, 1586, 1581, 1576, 1572, 1567, 1562,
        1558, 1553, 1548, 1544, 1539, 1535, 1530, 1526,
        1521, 1517, 1513, 1508, 1504, 1500, 1495, 1491,
        1487, 1483, 1478, 1474, 1470, 1466, 1462, 1458,
        1454, 1450, 1446, 1442, 1438, 1434, 1430, 1426,
        1422, 1418, 1414, 1411, 1407, 1403, 1399, 1396,
        1392, 1388, 1384, 1381, 1377, 1374, 1370, 1366,
        1363, 1359, 1356, 1352, 1349, 1345, 1342, 1338,
        1335, 1332, 1328, 1325, 1322, 1318, 1315, 1312,
        1308, 1305, 1302, 1299, 1295, 1292, 1289, 1286,
        1283, 1280, 1276, 1273, 1270, 1267, 1264, 1261,
        1258, 1255, 1252, 1249, 1246, 1243, 1240, 1237,
        1234, 1231, 1228, 1226, 1223, 1220, 1217, 1214,
        1211, 1209, 1206, 1203, 1200, 1197, 1195, 1192,
        1189, 1187, 1184, 1181, 1179, 1176, 1173, 1171,
        1168, 1165, 1163, 1160, 1158, 1155, 1153, 1150,
        1148, 1145, 1143, 1140, 1138, 1135, 1133, 1130,
        1128, 1125, 1123, 1121, 1118, 1116, 1113, 1111,
        1109, 1106, 1104, 1102, 1099, 1097, 1095, 1092,
        1090, 1088, 1086, 1083, 1081, 1079, 1077, 1074,
        1072, 1070, 1068, 1066, 1064, 1061, 1059, 1057,
        1055, 1053, 1051, 1049, 1047, 1044, 1042, 1040,
        1038, 1036, 1034, 1032, 1030, 1028, 1026, 1024
    };

    apn_dig_t d0 = d & 1;
    apn_dig_t d9 = d >> 55;
    apn_dig_t d40 = (d >> 24) + 1;
    apn_dig_t d63 = (d >> 1) + d0;

    apn_dig_t v0 = (apn_dig_t)D9_LUT[d9 - 256];
    apn_dig_t v1 = (v0 << 11) - ((v0 * v0 * d40) >> 40) - 1;
    apn_dig_t v2 = (v1 << 13) + ((v1 * ((1ULL << 60) - v1 * d40)) >> 47);
    apn_dig_t e = (v2 >> 1) * d0 - v2 * d63;

    #if defined(APAC_X64_WIN)

        uint64_t high64 = __umulh(e, v2);
        apn_dig_t v3 = (high64 >> 1) + (v2 << 31);
        uint64_t low64 = _umul128(v3, d, &high64);

    #elif defined(APAC_ARM64_WIN)

        uint64_t high64 = __umulh(e, v2);
        apn_dig_t v3 = (high64 >> 1) + (v2 << 31);

        uint64_t low64 = v3 * d;
        high64 = __umulh(v3, d);

    #elif (defined(APAC_X64_UNIX) || defined(APAC_ARM64_UNIX))

        apn_dig_t v3 = (v2 << 31) + (((__uint128_t)e * v2) >> 65);
        __uint128_t prod = (__uint128_t)v3 * d;
        
        uint64_t low64 = (uint64_t)prod;
        uint64_t high64 = (uint64_t)(prod >> 64);

    #else
        #error "Unsupported Platform!"
    #endif

    uint8_t carry = (low64 + d) < low64;
    apn_dig_t v4 = v3 - (high64 + d + carry);
    
    return v4;

#else
    #error "Unsupported Platform!"
#endif
}

/**
 * Algorithm: RECIPROCAL_WORD_3BY2
 *
 * Compute a single-word reciprocal for a 2-word divisor.
 *
 * Reference:
 *   Niels Moeller, Torbjorn Granlund
 *   "Improved Division by Invariant Integers"
 */
apn_dig_t apn_recip_word_3by2(
    apn_dig_t d1, 
    apn_dig_t d0
)
{
    /* Step 1: v = RECIPROCAL_WORD(d1) */
    apn_dig_t v = apn_recip_word_2by1(d1);

    apn_dig_t p;

    /* Step 2-3: p = (d1 * v + d0) mod 2^64 */
    p = v * d1;
    p += d0;

    /* Step 4-9 */
    {
        apn_dig_t c = (p < d0);
        apn_dig_t d = c & (p >= d1);

        v -= c + d;
        p -= d1 * (c + d);
    }

    uint64_t t0, t1;

#if defined(APAC_X64_WIN)

    t0 = _umul128(v, d0, &t1);

#elif defined(APAC_ARM64_WIN)

    t0 = v * d0;
    t1 = __umulh(v, d0);

#elif (defined(APAC_X64_UNIX) || defined(APAC_ARM64_UNIX))

    __uint128_t prod = (__uint128_t)v * d0;
    t0 = (uint64_t)prod;
    t1 = (uint64_t)(prod >> 64);

#else
    #error "Unsupported compiler"
#endif

    /* Step 11 */
    p += t1;

    /* Step 12-15 */
    {
        apn_dig_t c = (p < t1);
        apn_dig_t d = c & ((p > d1) | ((p == d1) & (t0 >= d0)));

        v -= c + d;
    }

    /* Step 16 */
    return v;
}

/**
 * Algorithm 4: UDIV2BY1
 *
 * Divide a 2-word numerator by a 1-word denominator using a precomputed
 * single-word reciprocal.
 *
 * Reference:
 *   Niels Moeller, Torbjorn Granlund
 *   "Improved Division by Invariant Integers"
 */
apn_dig_t apn_udiv_2by1(
    apn_dig_t u1,
    apn_dig_t u0,
    apn_dig_t d,
    apn_dig_t v,
    apn_dig_t* r
)
{
    /* Preconditions from the paper */
    APAC_ASSERT(u1 < d);
    APAC_ASSERT(d & APN_DIG_HIGH_BIT);

#if defined(APAC_X64_WIN)

    uint64_t q0, q1;

    /* Step 1: <q1,q0> = v * u1 */
    q0 = _umul128(v, u1, &q1);

    /* Step 2: <q1,q0> += <u1,u0> */
    uint8_t c = _addcarry_u64(0, q0, u0, &q0);
    c = _addcarry_u64(c, q1, u1, &q1);

#elif defined(APAC_ARM64_WIN)

    uint64_t q0, q1;

    /* Step 1: <q1,q0> = v * u1 */
    q0 = v * u1;
    q1 = __umulh(v, u1);

    /* Step 2: <q1,q0> += <u1,u0> */

    uint64_t old_q0 = q0;
    q0 += u0;

    uint64_t carry = (q0 < old_q0);

    q1 += u1;
    q1 += carry;

#elif defined(APAC_X64_UNIX) || defined(APAC_ARM64_UNIX)

    __uint128_t q;

    /* Step 1 */
    q = (__uint128_t)v * u1;

    /* Step 2 */
    q += ((__uint128_t)u1 << APN_DIG_BITS) | u0;

    uint64_t q1 = (apn_dig_t)(q >> APN_DIG_BITS);
    uint64_t q0 = (apn_dig_t)q;

#else

    #error "Unsupported Platform!"

#endif

    /* Step 3 */
    q1++;

    /* Step 4 */
    *r = u0 - q1 * d;

    /* Step 5-7: first correction */
    {
        apn_dig_t m = (*r > q0);
        q1 -= m;
        *r += d * m;
    }

    /* Step 8-10: second correction */
    {
        apn_dig_t m = (*r >= d);
        q1 += m;
        *r -= d * m;
    }

    return q1;
}

/**
 * Algorithm 5: DIV3BY2
 *
 * Divide a 3-word numerator by a 2-word denominator using a precomputed
 * single-word reciprocal.
 *
 * Reference:
 *   Niels Moeller, Torbjorn Granlund
 *   "Improved Division by Invariant Integers"
 */
apn_dig_t apn_udiv_3by2_quot(
    apn_dig_t u2,
    apn_dig_t u1,
    apn_dig_t u0,
    apn_dig_t d1,
    apn_dig_t d0,
    apn_dig_t v   /* reciprocal */
)
{
    /* Preconditions from the paper */
    APAC_ASSERT(d1 & APN_DIG_HIGH_BIT);
    APAC_ASSERT((d1 > u2) || (d1 == u2 && d0 > u1));

#if defined(APAC_X64_WIN)

    uint64_t q0, q1;
    uint64_t r0, r1;
    uint64_t t0, t1;
    uint8_t c;

    /* Step 1: q = v * u2 */
    q0 = _umul128(v, u2, &q1);

    /* Step 2: q += (u2,u1) */
    c = _addcarry_u64(0, q0, u1, &q0);
    c = _addcarry_u64(c, q1, u2, &q1);

    /* Step 3: r1 = u1 - q1 * d1 (mod 2^64) */
    r1 = u1 - q1 * d1;

    /* Step 4: t = d0 * q1 */
    t0 = _umul128(d0, q1, &t1);

    /* Step 5: r = (r1,u0) - (t1,t0) - (d1,d0) */
    c = _subborrow_u64(0, u0, t0, &r0);
    c = _subborrow_u64(c, r1, t1, &r1);
    c = _subborrow_u64(0, r0, d0, &r0);
    c = _subborrow_u64(c, r1, d1, &r1);

    /* Step 6 */
    q1++;

    /* Steps 7-9 */
    if (r1 >= q0)
    {
        q1--;
        c = _addcarry_u64(0, r0, d0, &r0);
        c = _addcarry_u64(c, r1, d1, &r1);
    }

    /* Steps 10-12 */
    if ((r1 > d1) || (r1 == d1 && r0 >= d0))
    {
        q1++;
        c = _subborrow_u64(0, r0, d0, &r0);
        c = _subborrow_u64(c, r1, d1, &r1);
    }

#elif defined(APAC_ARM64_WIN)

    uint64_t q0, q1;
    uint64_t r0, r1;
    uint64_t t0, t1;
    uint64_t carry;
    uint64_t borrow;

    /* Step 1: q = v * u2 */
    q0 = v * u2;
    q1 = __umulh(v, u2);

    /* Step 2: q += (u2,u1) */
    uint64_t old = q0;
    q0 += u1;
    carry = (q0 < old);

    q1 += u2;
    q1 += carry;

    /* Step 3: r1 = u1 - q1 * d1 (mod 2^64) */
    r1 = u1 - q1 * d1;

    /* Step 4: t = d0 * q1 */
    t0 = d0 * q1;
    t1 = __umulh(d0, q1);

    /* Step 5: r = (r1,u0) - (t1,t0) - (d1,d0) */

    borrow = (u0 < t0);
    r0 = u0 - t0;

    uint64_t tmp = r1 - t1;
    borrow = (r1 < t1) + borrow;
    r1 = tmp - (borrow != 0);

    borrow = (r0 < d0);
    r0 -= d0;

    tmp = r1 - d1;
    borrow = (r1 < d1) + borrow;
    r1 = tmp - (borrow != 0);

    /* Step 6 */
    q1++;

    /* Steps 7-9 */
    if (r1 >= q0)
    {
        q1--;

        old = r0;
        r0 += d0;
        carry = (r0 < old);

        r1 += d1;
        r1 += carry;
    }

    /* Steps 10-12 */
    if ((r1 > d1) || (r1 == d1 && r0 >= d0))
    {
        q1++;

        borrow = (r0 < d0);
        r0 -= d0;

        r1 = r1 - d1 - borrow;
    }

#elif (defined(APAC_X64_UNIX) || defined(APAC_ARM64_UNIX))

    __uint128_t q;
    __uint128_t r;
    __uint128_t d;
    uint64_t q1, q0;

    /* Step 1: q = v * u2 */
    q = (__uint128_t)v * u2;

    /* Step 2: q += (u2,u1) */
    q += ((__uint128_t)u2 << APN_DIG_BITS) | u1;

    q1 = (uint64_t)(q >> APN_DIG_BITS);
    q0 = (uint64_t)q;

    /* Step 3 */
    uint64_t r1 = u1 - q1 * d1;

    /* Step 4 */
    __uint128_t t = (__uint128_t)d0 * q1;

    /* Step 5 */
    d = ((__uint128_t)d1 << APN_DIG_BITS) | d0;
    r = ((__uint128_t)r1 << APN_DIG_BITS) | u0;
    r = r - t - d;

    /* Step 6 */
    q1++;

    /* Steps 7-9 */
    if ((uint64_t)(r >> APN_DIG_BITS) >= q0)
    {
        q1--;
        r += d;
    }

    /* Steps 10-12 */
    if (r >= d)
    {
        q1++;
        r -= d;
    }

#else
    #error "Unsupported compiler"
#endif

    return (apn_dig_t)q1;
}