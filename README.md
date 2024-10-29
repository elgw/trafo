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

Not really a benchmark as it is only one run per set. Test system: AMD
Ryzen 3 PRO 3300U.

- The memory usage metric not directly comparable since the
values for trafo includes the whole program while for skl it is just
the delta value, memory before and after the call to the `.fit`
method.

See `test/run_on_test_data.sh` for the full code.

### A single tree

In this case for scikit-learn the following settings were used:

``` Python
{'bootstrap': False, 'ccp_alpha': 0.0, 'class_weight': None, 'criterion': 'gini', 'max_depth': None, 'max_features': 10, 'max_leaf_nodes': None, 'max_samples': None, 'min_impurity_decrease': 0.0, 'min_samples_leaf': 1, 'min_samples_split': 2, 'min_weight_fraction_leaf': 0.0, 'monotonic_cst': None, 'n_estimators': 1, 'n_jobs': -1, 'oob_score': False, 'random_state': None, 'verbose': 0, 'warm_start': False}
```

| bin   | dataset       | time (s) | rss (kb) | accuracy |
|-------|---------------|----------|----------|----------|
| trafo | iris          | 0.002    | 2436     | 100%     |
| trafo | digits        | 0.020    | 6152     | 100%     |
| trafo | wine          | 0.006    | 2456     | 100%     |
| trafo | breast_cancer | 0.003    | 2928     | 100%     |
| trafo | diabetes      | 0.016    | 2648     | 100%     |
| skl   | iris          | 0.015    | 1612     | 100%     |
| skl   | digits        | 0.036    | 2192     | 100%     |
| skl   | wine          | 0.016    | 1428     | 100%     |
| skl   | breast_cancer | 0.016    | 1428     | 100%     |
| skl   | diabetes      | 0.027    | 3712     | 100%     |


## A forest with 100 trees

``` python
{'bootstrap': True, 'ccp_alpha': 0.0, 'class_weight': None, 'criterion': 'gini', 'max_depth': None, 'max_features': 'sqrt', 'max_leaf_nodes': None, 'max_samples': None, 'min_impurity_decrease': 0.0, 'min_samples_leaf': 1, 'min_samples_split': 2, 'min_weight_fraction_leaf': 0.0, 'monotonic_cst': None, 'n_estimators': 100, 'n_jobs': -1, 'oob_score': False, 'random_state': None, 'verbose': 0, 'warm_start': False}
```

| bin   | dataset       | time (s) | D.rss (kb) | accuracy |
|-------|---------------|----------|------------|----------|
| trafo | iris          | 0.045    | 2548       | 100%     |
| trafo | digits        | 0.094    | 6940       | 100%     |
| trafo | wine          | 0.004    | 2755       | 100%     |
| trafo | breast_cancer | 0.015    | 3416       | 100%     |
| trafo | diabetes      | 0.121    | 3344       | 100%     |
| skl   | iris          | 0.186    | 2208       | 100%     |
| skl   | digits        | 0.225    | 9412       | 100%     |
| skl   | wine          | 0.198    | 2512       | 100%     |
| skl   | breast_cancer | 0.192    | 2340       | 100%     |
| skl   | diabetes      | 0.224    | 98548      | 100%     |


## Installation
Use cmake with the `CMakeLists.txt` file.


## See also
