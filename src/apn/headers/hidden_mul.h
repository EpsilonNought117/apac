#ifndef APN_HIDDEN_MUL
#define APN_HIDDEN_MUL

#include "../../header/apac_internal.h"

/*
 * 1) Calls multiplication routines written in assembly
 * 2) Unbalanced & balanced multiplications can be done
 * 3) O(n^2) schoolbook multiplication
 * 4) size1 must be greater than or equal to size2
 * 5) In-place, O(1) space requirement
 */
void apn_basecase_mul(
    ap_dig_t* result,
    const ap_dig_t* op1,
    const ap_dig_t* op2,
    ap_size_t size1,
    ap_size_t size2
);

/*
 * 1) Subtractive Karatsuba variant
 * 2) Balanced multiplication
 * 3) Uses (2 * n + 64) limbs of space for size n operands
 * 4) O(n^1.585) time complexity
 * 5) size1 must be greater than or equal to size2
 */
void apn_karatsuba_mul(
    ap_dig_t* result,
    const ap_dig_t* op1,
    const ap_dig_t* op2,
    ap_size_t size1,
    ap_size_t size2,
    ap_dig_t* temp
);

#ifdef APAC_64BIT_PLATFORM

typedef struct ntt_prime_t
{

    unsigned long long prime;
    unsigned long long magic;
    unsigned long long barrett_m;

    double twiddle_table[45];
    double omega;

    double prime_f64;
    double prime_f64_inv;

    unsigned short coefficient;
    unsigned short max_power_two;
    unsigned short shift;
    unsigned short add;

} ntt_prime_t;

static const ntt_prime_t NTT_PRIMES[4] =
{
        // ========================================================
    //  p = 9 * 2^43 + 1
    // ========================================================

    {
        .prime         = (9ULL << 43) + 1ULL,

        .magic         = 16397105843297172089ULL,

        .barrett_m     = 16012798675094894ULL,

        .twiddle_table =
        {
            (double)             1ULL, (double)79164837199872ULL, (double)32673132265234ULL, (double)20305099120877ULL, (double)39579957916562ULL,
            (double)75942538918562ULL, (double)59652139836565ULL, (double)20624828515856ULL, (double)76119518187106ULL, (double)69740145741383ULL,
            (double)75732696426949ULL, (double)13973145859902ULL, (double)67083337243677ULL, (double)54852747079751ULL, (double)42609062690486ULL,
            (double)73648718738577ULL, (double)66668608089872ULL, (double)54969621416071ULL, (double)49091704410171ULL, (double)27583800632336ULL,
            (double)52594140768013ULL, (double)53454178270561ULL, (double)46088029742578ULL, (double)42734287245737ULL, (double)43413449706744ULL,
            (double)50815157546300ULL, (double)71005215139865ULL, (double)12178223916498ULL, (double)39424926507588ULL, (double)78638509653119ULL,
            (double)17161673261209ULL, (double)56746395738038ULL, (double) 4502640539222ULL, (double) 3942849128304ULL, (double)70069238268302ULL,
            (double)39157489244367ULL, (double)47286949160522ULL, (double)54843957378729ULL, (double)48234651972722ULL, (double)54616456290052ULL,
            (double)69804035624843ULL, (double)53079601705925ULL, (double) 3814697265625ULL, (double)       1953125ULL, (double)             0ULL
        },

        .prime_f64     = (double)((9ULL << 43) + 1ULL),

        .prime_f64_inv = 1.0 / (double)((9ULL << 43) + 1ULL),

        .coefficient   = 9,

        .max_power_two = 43,

        .shift         = 46,

        .add           = 0
    },

    // ========================================================
    //  p = 15 * 2^44 + 1
    // ========================================================

    {
        .prime         = (15ULL << 44) + 1ULL,

        .magic         = 4919131752989195123ULL,

        .barrett_m     = 4803839602528510ULL,

        .twiddle_table =
        {
            (double)              1ULL, (double)263882790666240ULL, (double) 96874177198991ULL, (double)169647361331209ULL, (double) 74839147327884ULL,
            (double)219479531082877ULL, (double)254953316083702ULL, (double) 14335899030929ULL, (double)104374235877625ULL, (double)140320092078334ULL,
            (double)231460602433612ULL, (double)159686507286794ULL, (double)261297844521479ULL, (double)145765277501825ULL, (double)126865043824258ULL,
            (double) 71123127889687ULL, (double)136694793957464ULL, (double) 50043853185307ULL, (double)165494835505761ULL, (double) 22154170913001ULL,
            (double) 25309197395126ULL, (double) 42489908125698ULL, (double) 24293199849874ULL, (double)177653757651132ULL, (double) 12077505994337ULL,
            (double)118584788982848ULL, (double) 29686080599489ULL, (double)118373120628094ULL, (double)222826588246730ULL, (double) 64223740524830ULL,
            (double)205426212491633ULL, (double)245418320602424ULL, (double)200630553550164ULL, (double)193503047357194ULL, (double) 59370066278789ULL,
            (double)150044964276061ULL, (double) 85517440996627ULL, (double)135998638979624ULL, (double)128845611580283ULL, (double)150223744460055ULL,
            (double)109794787131788ULL, (double)  2300851410641ULL, (double) 96623784492484ULL, (double)177706635564837ULL, (double)  4747561509943ULL
        },

        .prime_f64     = (double)((15ULL << 44) + 1ULL),

        .prime_f64_inv = 1.0 / (double)((15ULL << 44) + 1ULL),

        .coefficient   = 15,

        .max_power_two = 44,

        .shift         = 46,

        .add           = 0
    },

    // ========================================================
    //  p = 27 * 2^44 + 1
    // ========================================================

    {
        .prime         = (27ULL << 44) + 1ULL,

        .magic         = 10931403895531563129ULL,

        .barrett_m     = 2668799779182510ULL,

        .twiddle_table =
        {
            (double)              1ULL, (double)474989023199232ULL, (double)464110467020161ULL, (double)135350708912551ULL, (double)321727460506772ULL,
            (double)106401440901376ULL, (double)220911057284769ULL, (double)127385901584353ULL, (double)435246015222818ULL, (double)118071336691046ULL,
            (double) 81300405842855ULL, (double)227897485989402ULL, (double) 54267550345115ULL, (double)276106822236007ULL, (double)348478044091513ULL,
            (double)  4827849866075ULL, (double)457826451206020ULL, (double)462916617869353ULL, (double)315741889858653ULL, (double)270918994518659ULL,
            (double)425553051638339ULL, (double)243878334397803ULL, (double)382203653262898ULL, (double)408231869856488ULL, (double)307827959340566ULL,
            (double)411138074174517ULL, (double) 87148805141295ULL, (double)292671925825444ULL, (double)432848594034633ULL, (double)358670431663572ULL,
            (double) 29494414159224ULL, (double)454883341789260ULL, (double)387239633649015ULL, (double)246837078833254ULL, (double) 88332430385886ULL,
            (double) 54471702648877ULL, (double)128007654864053ULL, (double)208162606575243ULL, (double)   511232517150ULL, (double)  1818465959047ULL,
            (double)434603160347624ULL, (double)170445880904923ULL, (double)379491420499445ULL, (double)413399648687816ULL, (double)377768043858520ULL
        },

        .prime_f64     = (double)((27ULL << 44) + 1ULL),

        .prime_f64_inv = 1.0 / (double)((27ULL << 44) + 1ULL),

        .coefficient   = 27,

        .max_power_two = 44,

        .shift         = 48,

        .add           = 0
    },

    // ========================================================
    //  p = 63 * 2^44 + 1
    // ========================================================

    {
        .prime         = (63ULL << 44) + 1ULL,

        .magic         = 4684887383799246977ULL,

        .barrett_m     = 1143771333935363ULL,

        .twiddle_table =
        {
            (double)               1ULL, (double)1108307720798208ULL, (double) 834463902588453ULL, (double) 982524503535289ULL, (double) 717649395518176ULL,
            (double) 702315644809335ULL, (double) 739884912804600ULL, (double) 359708859995378ULL, (double)  43573700061864ULL, (double) 127530762639715ULL,
            (double) 714622044849844ULL, (double) 722952786250989ULL, (double)  47508801853780ULL, (double)  54144955344414ULL, (double)1067221487679684ULL,
            (double) 808196340939860ULL, (double) 400705862578896ULL, (double)  29168776481124ULL, (double) 733410228814483ULL, (double) 973198108754437ULL,
            (double) 971018456874554ULL, (double)1039029984172770ULL, (double) 337714919750706ULL, (double) 572965129136056ULL, (double)  29598010259900ULL,
            (double) 335102667773610ULL, (double) 944785731158078ULL, (double) 612173141699626ULL, (double) 496594134287593ULL, (double)    209141447223ULL,
            (double) 529799425356461ULL, (double) 869699914922316ULL, (double) 532478848124777ULL, (double)  33221601499042ULL, (double) 274829696946219ULL,
            (double) 358645175366147ULL, (double) 611526591252824ULL, (double) 353035949292892ULL, (double) 546014641375689ULL, (double) 684421603307337ULL,
            (double) 358499153441500ULL, (double) 291976380365697ULL, (double) 869336496498271ULL, (double) 692415616839170ULL, (double) 194751219211145ULL
        },

        .prime_f64     = (double)((63ULL << 44) + 1ULL),

        .prime_f64_inv = 1.0 / (double)((63ULL << 44) + 1ULL),

        .coefficient   = 63,

        .max_power_two = 44,

        .shift         = 48,

        .add           = 0
    }
};

#define CRT3_MAX_CONV_LEN   (1ULL << 18)
#define CRT4_MAX_CONV_LEN   (1ULL << 43)

#else
    #error "Only 64-bit systems supported!"
#endif

#endif