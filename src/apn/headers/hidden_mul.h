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
    ap_dig_t prime;
    ap_dig_t barrett_m; // floor((2^100) / prime)

    ap_dig_t magic;
    ap_dig_t shift;
    
    ap_dig_t coefficient;
    ap_dig_t max_power_two;
    
    double twiddle_table[44];
    double prime_f64;
    double prime_f64_inv;

} ntt_prime_t;

static const ntt_prime_t NTT_PRIMES[8] = 
{
    // ========================================================
    //  p = 115 * 2^42 + 1
    // ========================================================

    {
        .prime         = (115ULL << 42) + 1ULL,

        .magic         = 5133007046597430301ULL,

        .barrett_m     = 2506351096971401ULL,

        .twiddle_table =
        {
            (double)  172516186673807ULL, (double)  226211343681490ULL, (double)  115924480682815ULL, (double)  195679620262303ULL,
            (double)  464522514521873ULL, (double)  102864900221067ULL, (double)  251973326463538ULL, (double)   36810722646142ULL,
            (double)  425244151253244ULL, (double)  369411237486795ULL, (double)  204272269604548ULL, (double)  357110303465470ULL,
            (double)  112969927974249ULL, (double)  475187202602034ULL, (double)    2475516680456ULL, (double)  225010625857364ULL,
            (double)  193339503160158ULL, (double)  147588361990727ULL, (double)  103478837003149ULL, (double)  117571113519187ULL,
            (double)   41314268431577ULL, (double)  225915254374440ULL, (double)  365333686467790ULL, (double)  252860494318280ULL,
            (double)  384222690600920ULL, (double)  151187250847423ULL, (double)   85823262971288ULL, (double)    7591186588076ULL,
            (double)   57665233327122ULL, (double)  329491885654487ULL, (double)  500427090054903ULL, (double)   64372209723759ULL,
            (double)  295065444842456ULL, (double)  166630294507922ULL, (double)  328748620899517ULL, (double)  148702827038269ULL,
            (double)  299060829846566ULL, (double)  141012338864163ULL, (double)  321564659195546ULL, (double)  415704513977290ULL,
            (double)  318590188045477ULL, (double)  505775348776960ULL, (double)                1ULL, (double)                0ULL
        },

        .prime_f64     = (double)505775348776961ULL,

        .prime_f64_inv = 1.0 / (double)505775348776961ULL,

        .coefficient   = 115,

        .max_power_two = 42,

        .shift         = 47
    },

    // ========================================================
    //  p = 159 * 2^42 + 1
    // ========================================================

    {
        .prime         = (159ULL << 42) + 1ULL,

        .magic         = 7425104532813896951ULL,

        .barrett_m     = 1812769661331517ULL,

        .twiddle_table =
        {
            (double)  643876812462936ULL, (double)  651799495178364ULL, (double)   58873305744392ULL, (double)  559861804683365ULL,
            (double)  530518765208025ULL, (double)   95985907372983ULL, (double)  299960076468837ULL, (double)  110366640477232ULL,
            (double)  513374379332229ULL, (double)  372494585029016ULL, (double)  199272209064928ULL, (double)  360606354885115ULL,
            (double)  349127071085236ULL, (double)  323003168758522ULL, (double)   79876745311839ULL, (double)  144449987797064ULL,
            (double)  103261426565294ULL, (double)  605575372258894ULL, (double)   69223014675082ULL, (double)  415870892267877ULL,
            (double)  189717489056055ULL, (double)  292695431604081ULL, (double)   65944000111363ULL, (double)  273710408819860ULL,
            (double)  364368634952905ULL, (double)  622779517142604ULL, (double)  329206268876788ULL, (double)  597464592558120ULL,
            (double)  103377132373619ULL, (double)  623608487135405ULL, (double)  488591728611846ULL, (double)  202884888100841ULL,
            (double)  696795994541917ULL, (double)  179721527579332ULL, (double)  239012899833485ULL, (double)  244070046291945ULL,
            (double)  668076890266666ULL, (double)   98233049163354ULL, (double)  610841607039564ULL, (double)  184770160673964ULL,
            (double)  514847792185270ULL, (double)  699289395265536ULL, (double)                1ULL, (double)                0ULL
        },

        .prime_f64     = (double)699289395265537ULL,

        .prime_f64_inv = 1.0 / (double)699289395265537ULL,

        .coefficient   = 159,

        .max_power_two = 42,

        .shift         = 48
    },

    // ========================================================
    //  p = 207 * 2^42 + 1
    // ========================================================

    {
        .prime         = (207ULL << 42) + 1ULL,

        .magic         = 11406682325772077359ULL,

        .barrett_m     = 1392417276095224ULL,

        .twiddle_table =
        {
            (double)  212465101045679ULL, (double)  803402251629806ULL, (double)  628933820577364ULL, (double)  205469306257703ULL,
            (double)  451980748078623ULL, (double)  193218791447654ULL, (double)  202389316246287ULL, (double)   37248301570755ULL,
            (double)  847857065749852ULL, (double)  884096819761335ULL, (double)  300950555162459ULL, (double)  437748249827255ULL,
            (double)  322980301055091ULL, (double)  845010139816708ULL, (double)  398651143098115ULL, (double)  193369998286205ULL,
            (double)  332590843575892ULL, (double)  536364198913702ULL, (double)  334697738217984ULL, (double)  732061010752108ULL,
            (double)  303478118445587ULL, (double)  106094026763177ULL, (double)  620799350043677ULL, (double)  750624629738156ULL,
            (double)  861566755153795ULL, (double)  887605180829768ULL, (double)  471774853951387ULL, (double)  772826871432937ULL,
            (double)  372732884266381ULL, (double)  705773813529841ULL, (double)  434344676731654ULL, (double)  562458611013956ULL,
            (double)  597843530306532ULL, (double)  102955201479009ULL, (double)  673755987546267ULL, (double)  424716232952107ULL,
            (double)  688916947494790ULL, (double)  303336742766754ULL, (double)  356689100781988ULL, (double)  389524527106070ULL,
            (double)  318852943881667ULL, (double)  910395627798528ULL, (double)                1ULL, (double)                0ULL
        },

        .prime_f64     = (double)910395627798529ULL,

        .prime_f64_inv = 1.0 / (double)910395627798529ULL,

        .coefficient   = 207,

        .max_power_two = 42,

        .shift         = 49
    },

    // ========================================================
    //  p = 247 * 2^42 + 1
    // ========================================================

    {
        .prime         = (247ULL << 42) + 1ULL,

        .magic         = 9559446321598463293ULL,

        .barrett_m     = 1166924599804499ULL,

        .twiddle_table =
        {
            (double)  813873581740013ULL, (double) 1017584146621833ULL, (double) 1001883406154027ULL, (double)  399388810901566ULL,
            (double)  580057676602761ULL, (double)  861704420831397ULL, (double)   83055472667820ULL, (double)  956434454527863ULL,
            (double)  280506422740464ULL, (double)  656050848733060ULL, (double)  179078271057436ULL, (double)  417782972570273ULL,
            (double) 1049302414900875ULL, (double)  952170334743460ULL, (double)  920833329738732ULL, (double) 1061007779199887ULL,
            (double)  975096655085155ULL, (double)  109989026498247ULL, (double)    4244788471044ULL, (double)  678204445944222ULL,
            (double)  872136963179980ULL, (double)  480314125092252ULL, (double)  702537324697685ULL, (double)  771483826635079ULL,
            (double)  245092761087288ULL, (double) 1068241675986200ULL, (double)  615456732651794ULL, (double)  367101539206423ULL,
            (double)  419324129799797ULL, (double)  144911083810310ULL, (double)  414921319947421ULL, (double)  656776753396433ULL,
            (double)  774156178124198ULL, (double)   12909816601084ULL, (double)  125720652037893ULL, (double)  365166793134028ULL,
            (double)   29490787084355ULL, (double)  751324195164222ULL, (double)  725138888531475ULL, (double)  210201668638023ULL,
            (double)  301092149163518ULL, (double) 1086317488242688ULL, (double)                1ULL, (double)                0ULL
        },

        .prime_f64     = (double)1086317488242689ULL,

        .prime_f64_inv = 1.0 / (double)1086317488242689ULL,

        .coefficient   = 247,

        .max_power_two = 42,

        .shift         = 49
    },

    // ========================================================
    //  p = 11 * 2^43 + 1
    // ========================================================

    {
        .prime         = (11ULL << 43) + 1ULL,

        .magic         = 13415813871788626157ULL,

        .barrett_m     = 13101380734168580ULL,

        .twiddle_table =
        {
            (double)           177147ULL, (double)      31381059609ULL, (double)   76961560733062ULL, (double)   78111664663586ULL,
            (double)   44103993397726ULL, (double)   77560470022895ULL, (double)   10908565321237ULL, (double)   61304740378095ULL,
            (double)   74387491218904ULL, (double)   35030920465501ULL, (double)   35507591713098ULL, (double)   14736883445812ULL,
            (double)     118122403951ULL, (double)   46175136454937ULL, (double)   56014752361721ULL, (double)   57347006712467ULL,
            (double)   70731461504035ULL, (double)   80925295288329ULL, (double)   63496682325554ULL, (double)   90094547766557ULL,
            (double)   13605602730752ULL, (double)   61421372598461ULL, (double)   84162057231393ULL, (double)   88644246420441ULL,
            (double)   84380531071501ULL, (double)   27448222060074ULL, (double)   83125391779075ULL, (double)   25117643567475ULL,
            (double)    9072295882313ULL, (double)    7118901039428ULL, (double)   91176476655774ULL, (double)   28872639062127ULL,
            (double)   60992261479213ULL, (double)   75834893586148ULL, (double)   47339900899867ULL, (double)   59958104589341ULL,
            (double)   80679186042070ULL, (double)   93783162282251ULL, (double)   48820947039984ULL, (double)   41630991080728ULL,
            (double)   50327709636321ULL, (double)   92061214401519ULL, (double)   96757023244288ULL, (double)                1ULL
        },

        .prime_f64     = (double)96757023244289ULL,

        .prime_f64_inv = 1.0 / (double)96757023244289ULL,

        .coefficient   = 11,

        .max_power_two = 43,

        .shift         = 46
    },

    // ========================================================
    //  p = 29 * 2^43 + 1
    // ========================================================

    {
        .prime         = (29ULL << 43) + 1ULL,

        .magic         = 10177513971701781683ULL,

        .barrett_m     = 4969489243995010ULL,

        .twiddle_table =
        {
            (double)   68630377364883ULL, (double)  188731362164540ULL, (double)  254419031540391ULL, (double)  213946120958220ULL,
            (double)  219162886873414ULL, (double)   96609930250521ULL, (double)  101137061869106ULL, (double)   64586781352587ULL,
            (double)  148293486523287ULL, (double)   28803969314595ULL, (double)  119527972729066ULL, (double)  119760143971655ULL,
            (double)  175868003714077ULL, (double)  122857909444582ULL, (double)   95572310589249ULL, (double)   49929046569894ULL,
            (double)  111417682740098ULL, (double)   53843322710559ULL, (double)  181675970817071ULL, (double)   12303239105804ULL,
            (double)   67867834859601ULL, (double)  241004286672100ULL, (double)   86634793263078ULL, (double)  174272935702765ULL,
            (double)  205533592622223ULL, (double)    8014969919343ULL, (double)  231897826749875ULL, (double)   15937428744598ULL,
            (double)  239543674841388ULL, (double)    6889417907037ULL, (double)   68178617896139ULL, (double)  127090155568707ULL,
            (double)   84687171623347ULL, (double)   89258372240300ULL, (double)   61558578542724ULL, (double)  229256123978893ULL,
            (double)  150528025548345ULL, (double)   16733925574254ULL, (double)   37394035087613ULL, (double)  115345514152624ULL,
            (double)  197742111156947ULL, (double)   42555792054163ULL, (double)  255086697644032ULL, (double)                1ULL
        },

        .prime_f64     = (double)255086697644033ULL,

        .prime_f64_inv = 1.0 / (double)255086697644033ULL,

        .coefficient   = 29,

        .max_power_two = 43,

        .shift         = 47
    },

    // ========================================================
    //  p = 51 * 2^43 + 1
    // ========================================================

    {
        .prime         = (51ULL << 43) + 1ULL,

        .magic         = 1446803456761530235ULL,

        .barrett_m     = 2825788001487363ULL,

        .twiddle_table =
        {
            (double)  361403907776071ULL, (double)  116127301404452ULL, (double)  114488020772742ULL, (double)   14915309336635ULL,
            (double)   49725184089233ULL, (double)  261591417788558ULL, (double)  181535738308138ULL, (double)   72711887292730ULL,
            (double)  195809621029587ULL, (double)  310807638248234ULL, (double)  111360496143858ULL, (double)   55623512267422ULL,
            (double)  225681080187287ULL, (double)  106537479831870ULL, (double)  329243157636192ULL, (double)  277297354646753ULL,
            (double)  246001628151506ULL, (double)  397604587447057ULL, (double)  204015068628731ULL, (double)  402930876538842ULL,
            (double)   43694293156869ULL, (double)  113883776227243ULL, (double)  422475954821842ULL, (double)   63233810843645ULL,
            (double)  288918103511369ULL, (double)  126996945760888ULL, (double)  220718550179336ULL, (double)  263570324670358ULL,
            (double)  212839171679162ULL, (double)   25552896470316ULL, (double)  442228650820096ULL, (double)  447410145984634ULL,
            (double)  314696518094055ULL, (double)   31820591761461ULL, (double)  418688680884800ULL, (double)  360702780091836ULL,
            (double)  446050426762948ULL, (double)  156545156680540ULL, (double)  182419455707330ULL, (double)  269450852029908ULL,
            (double)  130335554885735ULL, (double)   70275872337283ULL, (double)  448600744132608ULL, (double)                1ULL
        },

        .prime_f64     = (double)448600744132609ULL,

        .prime_f64_inv = 1.0 / (double)448600744132609ULL,

        .coefficient   = 51,

        .max_power_two = 43,

        .shift         = 45
    },

    // ========================================================
    //  p = 75 * 2^43 + 1
    // ========================================================

    {
        .prime         = (75ULL << 43) + 1ULL,

        .magic         = 15741221609565460185ULL,

        .barrett_m     = 1921535841011408ULL,

        .twiddle_table =
        {
            (double)  422061946989193ULL, (double)   58143155666860ULL, (double)   91635414306405ULL, (double)  646895763776599ULL,
            (double)  419852408702297ULL, (double)  177463079846478ULL, (double)  489939971268089ULL, (double)  154722330950117ULL,
            (double)  604909487584329ULL, (double)  459892863350964ULL, (double)  629052487306166ULL, (double)  644392181246681ULL,
            (double)  279317116085772ULL, (double)  289214058035536ULL, (double)  115345673179591ULL, (double)  487681342060117ULL,
            (double)  261837198889829ULL, (double)  142423866680127ULL, (double)  169570995016705ULL, (double)  197170157141984ULL,
            (double)   88058517357649ULL, (double)  400718471864265ULL, (double)  379236458575249ULL, (double)  608449383710143ULL,
            (double)  231796971650367ULL, (double)  359048271470472ULL, (double)  645884696401968ULL, (double)  590287327610717ULL,
            (double)  137726617880350ULL, (double)  441866921897980ULL, (double)  622795896505702ULL, (double)     841110816188ULL,
            (double)  465280636643717ULL, (double)  353920630589488ULL, (double)  438769532571355ULL, (double)  255262368915548ULL,
            (double)  531912896292763ULL, (double)  580353428744310ULL, (double)  515099255573214ULL, (double)   78192893710957ULL,
            (double)  198896548521787ULL, (double)  652403583836430ULL, (double)  659706976665600ULL, (double)                1ULL
        },

        .prime_f64     = (double)659706976665601ULL,

        .prime_f64_inv = 1.0 / (double)659706976665601ULL,

        .coefficient   = 75,

        .max_power_two = 43,

        .shift         = 49
    }
};

#define CRT3_MAX_CONV_LEN   (1ULL << 17)
#define CRT4_MAX_CONV_LEN   (1ULL << 42)

#else
    #error "Only 64-bit systems supported!"
#endif

#endif