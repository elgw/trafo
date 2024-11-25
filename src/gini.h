#pragma once

#include "trafo_util.h"

/* Suggests how to split the vector _class_ containing class labels
 * into two partitions of size _nleft and _nright.
 *
 * Since a split only is possible between points with different
 * values, the feature vector is used for exactly this purpose. For example if
 * feature = [0.1, 0.2, 0.2, 0.3, 0.3] the only possible outcomes are
 * nleft \in {0, 1, 3, 5}.
 *
 * Returns:
 * the return value is the gini of the split.
 * _gleft and _gright is gini purity of the left and right partition
 * _nleft and _nright is the number of elements in the left and right partition
 *
 */

double
gini_split(const u32 * restrict class,
           const f64 * restrict feature,
           const u32 npoint,
           const u32 max_label,
           u32  * restrict _nleft,
           u32  * restrict _nright,
           f64* restrict _gleft,
           f64* restrict _gright);
