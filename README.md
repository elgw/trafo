**trafo** is a tiny, and quite performant, random forest library.

Features and Limitations
- Will only work with integer labels and floating point features,
i.e. it has no facilites for data pre-processing. For best performance
the first label should be 0, and there should be no gaps between the
used labels.
- Does not impute missing features.
- Very little functionality besides the basics. See the `trafo_cli.c`
  for one way to add k-fold cross validation on top of the library. It
  should be very simple implement the feature permutation method to
  estimate feature imporatance on top of this library as well.


## Usage

Train on labelled data and save the classifier to disk:
``` C
#include "elgw/trafo.h"

# Basic configuration
trafo_conf C = {0};
C.F_row_major = F;
C.label = L;
C.n_tree = conf->n_tree;
C.n_sample = n_sample;
C.n_feature = n_feature;

# Fitting / Training
trafo * T = trafo_fit(C);

trafo_save(T, "classifier.trafo");
trafo_free(T);
```

Load a saved classifier and apply it to some data

``` C
#include "elgw/trafo.h"

trafo * T = trafo_load("classifier.trafo");
uint32_t * class = trafo_predict(T, features, NULL, n_features);
trafo_free(T);
```

see `trafo.h` for the full API. For more examples, look in `trafo_cli.c`.

## Performance hints


## Installation


## See also
