**trafo** (version 0.1.0) is a tiny and limited
but quite performant, [random
forest](https://en.wikipedia.org/wiki/Random_forest) library.

Why? For my own amusement and self-education, and because I needed
something small.

Features and Limitations

- No support. Only smoke tested.
- Tiny: 3 < kSLOC. `libtrafo.so` is 43K (version 0.1.0).
- Trees are trained in parallel using OpenMP.
- Features are only sorted once. Book keeping maintains this property
 throughout the tree constructions.
- Nodes are split by Gini impurity as the only option.
- Supports integer labels and floating point features.
- Does not impute missing features.
- Very little functionality besides the basics. See the `trafo_cli.c`
  for one way to add k-fold cross validation on top of the library. It
  should be very simple implement the feature permutation method to
  estimate feature importance on top of this library as well.
- The command line interface `trafo_cli` can be used to test the
  library on tsv-formated data. The tsv parser is very limited.
- Paramerers include: - The number of trees. - Fraction of samples per
  tree. - Number of features per tree.
- Internally, features are floats with double precision and labels are
  uint32. In 99% of all application it would probably be better with
  the combination of single precision and uint16. That is on the todo list.

## Basic Library Usage

Train on labelled data and save the classifier to disk:

``` C
#include "elgw/trafo.h"

# Basic configuration
trafo_conf C = {0};
C.n_sample = n_sample;
C.n_feature = n_feature;
C.F_row_major = F;       // Features
C.label = L;             // Labels
C.n_tree = conf->n_tree;

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

Benchmarks should, among other things, provide averages over multiple
runs. There are only results from single runs reported here. Test
system: 4-core AMD Ryzen 3 PRO 3300U.

The memory usage metric not directly comparable since the values for
**trafo** includes the whole cli interface.  For skl it is
just the delta value, memory before and after the call to the `.fit`
method.

i.e., something like:

``` python
mem0 = get_peak_memory()
clf = RandomForestClassifier(...)
...
clf = clf.fit(X, Y)
mem1 = get_peak_memory()
delta_rss = mem1-mem0
```
See `test/run_on_test_data.sh` for the full code.

Datasets:

| Name          | Samples | Features | Classes |
|---------------|---------|----------|---------|
| iris          | 150     | 5        | 3       |
| digits        | 1797    | 64       | 10      |
| wine          | 178     | 13       | 3       |
| breast_cancer | 569     | 30       | 2       |
| diabetes      | 442     | 10       | 347     |
| rand          | 100000  | 100      | 2       |


### A single tree

In this case for scikit-learn the following settings were used:

``` Python
{'bootstrap': False, 'ccp_alpha': 0.0, 'class_weight': None, 'criterion': 'gini', 'max_depth': None, 'max_features': 10, 'max_leaf_nodes': None, 'max_samples': None, 'min_impurity_decrease': 0.0, 'min_samples_leaf': 1, 'min_samples_split': 2, 'min_weight_fraction_leaf': 0.0, 'monotonic_cst': None, 'n_estimators': 1, 'n_jobs': -1, 'oob_score': False, 'random_state': None, 'verbose': 0, 'warm_start': False}
```

| bin   | dataset       | time (s) | RSS (kb) | accuracy |
|-------|---------------|----------|----------|----------|
| trafo | iris          | 0.002    | 2436     | 100%     |
| trafo | digits        | 0.020    | 6152     | 100%     |
| trafo | wine          | 0.006    | 2456     | 100%     |
| trafo | breast_cancer | 0.003    | 2928     | 100%     |
| trafo | diabetes      | 0.016    | 2648     | 100%     |
| trafo | rand          | 3.256    | 323660   | 100%     |
| skl   | iris          | 0.015    | 1612     | 100%     |
| skl   | digits        | 0.036    | 2192     | 100%     |
| skl   | wine          | 0.016    | 1428     | 100%     |
| skl   | breast_cancer | 0.016    | 1428     | 100%     |
| skl   | diabetes      | 0.027    | 3712     | 100%     |
| sk1   | rand          | 13.96    | 48344    | 100%     |


## A forest with 100 trees

skl settings:
``` python
{'bootstrap': True, 'ccp_alpha': 0.0, 'class_weight': None, 'criterion': 'gini', 'max_depth': None, 'max_features': 'sqrt', 'max_leaf_nodes': None, 'max_samples': None, 'min_impurity_decrease': 0.0, 'min_samples_leaf': 1, 'min_samples_split': 2, 'min_weight_fraction_leaf': 0.0, 'monotonic_cst': None, 'n_estimators': 100, 'n_jobs': -1, 'oob_score': False, 'random_state': None, 'verbose': 0, 'warm_start': False}
```

| bin   | dataset       | time (s) | RSS (kb) | accuracy |
|-------|---------------|----------|----------|----------|
| trafo | iris          | 0.045    | 2548     | 100%     |
| trafo | digits        | 0.094    | 6940     | 100%     |
| trafo | wine          | 0.004    | 2755     | 100%     |
| trafo | breast_cancer | 0.015    | 3416     | 100%     |
| trafo | diabetes      | 0.121    | 3344     | 100%     |
| trafo | rand          | 6.97     | 283088   | 100%     |
| skl   | iris          | 0.186    | 2208     | 100%     |
| skl   | digits        | 0.225    | 9412     | 100%     |
| skl   | wine          | 0.198    | 2512     | 100%     |
| skl   | breast_cancer | 0.192    | 2340     | 100%     |
| skl   | diabetes      | 0.224    | 98548    | 100%     |
| sk1   | rand          | 31.80    | 283560   | 100%     |


## Installation
Use cmake with the `CMakeLists.txt` file.

## To do
- [ ] Feature importance estimation.
- [ ] Single precision features/uint16 labels option for reduced
      memory usage. Or as default.
- [ ] Complete the `CMakeLists.txt` with installation instructions.

## See also
- Your first stop shoud be
  [scikit-learn](https://scikit-learn.org/1.5/modules/generated/sklearn.ensemble.RandomForestClassifier.html).
