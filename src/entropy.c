#include "entropy.h"
#include <math.h>


/* Return log2(n!) using Stirlings formula
 * see https://doi.org/10.21468/SciPostPhysLectNotes.76
 */
static double log_p(double n)
{
    if(n == 1)
    {
        return 0;
    }
    return n*log(n) - n + log(sqrt(2.0*M_PI*n));
}

/* Entropy calculated as ln( n! / \prod n_i! ) where n_i is the number
 * of elements of class i. On top of that Stirlings formula
 */


static double
entro_from_histogram(const size_t * H,
                     const u32 npoint,
                     const u32 max_label)
{
    double entro = log_p(npoint);

    for(size_t kk = 0; kk < max_label; kk++)
    {
        if(H[kk] > 0)
        {
            entro -= log_p(H[kk]);
        }
    }
    return entro;
}


double
entropy_split(const u32 * restrict class,
              const f64 * restrict feature,
              const u32 npoint,
              const u32 max_label,
              u32  * restrict _nleft,
              u32  * restrict _nright,
              f64* restrict _eleft,
              f64* restrict _eright)
{
    if(npoint < 2)
    {
        return 0;
    }

    /* Histogram of the number of elements of each class */
    size_t HL[max_label + 1];
    size_t HR[max_label + 1];

    memset(HL, 0, (max_label+1)*sizeof(size_t));
    memset(HR, 0, (max_label+1)*sizeof(size_t));

    assert(HL[0] == 0);
    /* Initially the "right" histogram contain all points */
    for(size_t kk = 0; kk < npoint; kk++)
    {
        assert(class[kk] <= max_label);
        HR[class[kk]]++;
    }

    /* Base line, all in one partition */
    double entro_min =
        entro_from_histogram(HR, npoint, max_label);
    u32 best_nleft = 0;
    double best_lentro = 0;
    double best_rentro = entro_min;

    //printf("All in one S: %f\n", entro_min);
    for(size_t kk = 1; kk < npoint; kk++)
    {
        // move one point from right to left
        HL[class[kk-1]]++;
        HR[class[kk-1]]--;


        if(feature[kk-1] == feature[kk])
        {
            /* There is no threshold that splits the data with kk
               points to the left. */
            continue;
        }
        double left_entro = entro_from_histogram(HL, kk, max_label);
        double right_entro = entro_from_histogram(HR, npoint-kk, max_label);
        #if 0
        printf("nleft: %4u left: %4f, right: %4f, sum: %4f\n",
               kk,
               left_entro, right_entro,
               left_entro+right_entro);
        #endif
        if(left_entro + right_entro < entro_min)
        {
            entro_min = left_entro + right_entro;
            best_nleft = kk;
            best_lentro = left_entro;
            best_rentro = right_entro;
        }
    }

    *_nleft = best_nleft;
    *_nright = npoint - best_nleft;
    *_eleft = best_lentro;
    *_eright = best_rentro;

    return entro_min;
}
