**trafo** (version 0.1.4, [CHANGELOG](CHANGELOG.md)) is a tiny [random
forest](https://en.wikipedia.org/wiki/Random_forest) library written
in C11. Most likely this isn't what you are looking for, but feel free to
copy/fork/use or have fun finding bugs.

Features and Limitations

- Tiny: The compiled library, `libtrafo.so` is less than 50 kB.

- Parallel processing: Tree construction as well as predictions can
  run in parallel using OpenMP.

- Sort once: Features are only sorted once. Book keeping maintains this property
 throughout the tree constructions.

- Gini impurity or by Entropy can be used as the splitting criterion for nodes.

- Feature importance: approximations included at almost no extra
  cost. The more proper (?) feature permutation is simple to add on
  top of the library -- if needed.

- Command line interface: the binary `trafo_cli` can be used to test the
  library on tsv/csv-formated data although the parser is very basic.

- Supports integer labels and floating point features.

- Does not impute missing features.

- Very little functionality besides the basics. See the `trafo_cli.c`
  for one way to add k-fold cross validation on top of the library.

- Paramerers include: - The number of trees. - Fraction of samples per
  tree. - Number of features per tree.

- Internally, features are floats with double precision and labels are
  uint32. In 99% of all application it would probably be better with
  the combination of single precision and uint16. That is on the todo list.

- Only smoke tested ...

## Basic Library Usage

Load a classifier and apply it to some data

``` C
#include <trafo.h>

trf * T = trafo_load("classifier.trafo");
uint32_t * class = trafo_predict(T, features, NULL, n_features);
trafo_free(T);
```


Train a classifier on labelled data and save it to disk:

``` C
#include <trafo.h>

// Basic configuration via a struct.
trafo_settings C = {0};
// Required: Describe the data
C.n_sample = n_sample;
C.n_feature = n_feature;
C.F_row_major = F;       // Input: Features
C.label = L;             // Input: Labels
// Optional: Algorithm settings
C.n_tree = n_tree;
// .. and possibly more.

// Fitting / Training
trf * T = trafo_fit(&C);

// Use it ...

// Save to disk
trafo_save(T, "classifier.trafo");

trafo_free(T); // And done
```

see `trafo.h` for the full API. For more examples, look in
`trafo_cli.c`. For a minimal program to check that the library was
installed properly can be found in `test/minimal_example.c`.

## Performance hints

The random forest implementation in scikit-learn is denoted skl in the
tables.

<details>
<summary>Python timings and memory measurements</summary>
``` python
# Benchmarking fitting/training
mem0 = get_peak_memory()
t1 = time.perf_counter()
clf = clf.fit(X, Y)
t2 = time.perf_counter()
mem1 = get_peak_memory() # custom funciton parsing VmHWM from /proc/self/status
# Benchmarking classification
t3 = time.perf_counter()
P = clf.predict(X)
t4 = time.perf_counter()
# and the differences were used
t_train = t2-t1
t_predict = t4-t3
mem = mem1-mem0
```
</details>

See `test/benchmark/` for the full code.

Datasets:

| Name          | Samples | Features | Classes |
|---------------|---------|----------|---------|
| iris          | 150     | 5        | 3       |
| digits        | 1797    | 64       | 10      |
| wine          | 178     | 13       | 3       |
| breast_cancer | 569     | 30       | 2       |
| diabetes      | 442     | 10       | 347*    |
| rand          | 100000  | 100      | 2       |

(*) in case of the diabetes dataset some of the classes have no examples.

### A single tree

The scikit-learn package was configured by:

``` python
clf = RandomForestClassifier(n_estimators=1)
clf.n_jobs=-1
clf.bootstrap = False
clf.max_features=X.shape[1]
clf.min_samples_split=2
```

<details>
<summary>detailed scikit-learn settings</summary>

``` Python
{
    'bootstrap': False,
    'ccp_alpha': 0.0,
    'class_weight': None,
    'criterion': 'gini',
    'max_depth': None,
    'max_features': 10,
    'max_leaf_nodes': None,
    'max_samples': None,
    'min_impurity_decrease': 0.0,
    'min_samples_leaf': 1,
    'min_samples_split': 2,
    'min_weight_fraction_leaf': 0.0,
    'monotonic_cst': None,
    'n_estimators': 1,
    'n_jobs': -1,
    'oob_score': False,
    'random_state': None,
    'verbose': 0,
    'warm_start': False
    }
```

</details>

Results:

| dataset       | method   |   t_train_avg |   t_train_std |   t_predict_avg |   t_predict_std |   mem_fit_kb |
|:--------------|:---------|--------------:|--------------:|----------------:|----------------:|-------------:|
| breast_cancer | skl      |    0.0143084  |   0.000207006 |      0.00047848 |     5.32249e-05 |      1464.32 |
| breast_cancer | trafo    |    0.0027546  |   0.00218933  |      0.00087928 |     0.00132134  |       532.48 |
| diabetes      | skl      |    0.0142767  |   0.000250696 |      0.00163288 |     5.35689e-05 |      3860.48 |
| diabetes      | trafo    |    0.0160097  |   0.00231795  |      0.000682   |     0.00124117  |       291.84 |
| digits        | skl      |    0.0246463  |   0.00018163  |      0.0008926  |     6.99823e-05 |      2088.96 |
| digits        | trafo    |    0.012642   |   0.0014166   |      0.00043184 |     1.41921e-05 |      2069.6  |
| iris          | skl      |    0.0142313  |   0.000250566 |      0.0003468  |     4.67076e-05 |      1520.64 |
| iris          | trafo    |    0.00218152 |   0.00394059  |      0.0006894  |     0.00123656  |        46.08 |
| wine          | skl      |    0.0142528  |   0.000175615 |      0.00035784 |     6.46583e-05 |      1448.96 |
| wine          | trafo    |    0.00149696 |   0.00264111  |      0.0007174  |     0.00122883  |       189.44 |


## A forest with 100 trees

For this test, skl was run by:

``` python
clf = RandomForestClassifier(n_estimators=100)
clf.n_jobs=-1
clf.min_samples_split=2
```

| dataset       | method   |   t_train_avg |   t_train_std |   t_predict_avg |   t_predict_std |   mem_fit_kb |
|:--------------|:---------|--------------:|--------------:|----------------:|----------------:|-------------:|
| breast_cancer | skl      |    0.148502   |    0.0131817  |      0.0155947  |     0.00282572  |      3409.92 |
| breast_cancer | trafo    |    0.00632796 |    0.00321839 |      0.00056564 |     0.000235739 |       956.96 |
| diabetes      | skl      |    0.121286   |    0.00621094 |      0.0250204  |     0.00221358  |    109645    |
| diabetes      | trafo    |    0.0430037  |    0.00228249 |      0.00238732 |     0.00160394  |       762.88 |
| digits        | skl      |    0.149989   |    0.0185172  |      0.0248226  |     0.00214206  |     11013.1  |
| digits        | trafo    |    0.0187744  |    0.00139698 |      0.001572   |     0.00147337  |      4522.08 |
| iris          | skl      |    0.130045   |    0.00827622 |      0.0142564  |     0.000510158 |      2677.76 |
| iris          | trafo    |    0.00345708 |    0.0047106  |      0.0009314  |     0.00167781  |       343.04 |
| wine          | skl      |    0.131988   |    0.00568188 |      0.0156189  |     0.00347153  |      2785.28 |
| wine          | trafo    |    0.00251072 |    0.00167846 |      0.00056372 |     0.000653925 |       317.44 |


## Installation

Use cmake with the `CMakeLists.txt` file, something like this should
do:

``` shell
mkdir build
cd build
cmake ..
sudo make install
```

Then just add `-ltrafo` to the linker flags of your project.

## Example output

The command line program `trafo_cli` has the iris dataset built in and
will perform some tests when called without any arguments.

For command line arguments, use with `--help`

<details> <summary>Example output -- command line arguments</summary>

```
$ trafo_cli --help
Usage:
--train file.tsv
	table to train on
--cout file.trf
	Write the classifier to disk
--ntree n
	number of trees in the forest
--predict file.tsv
	table of point to classify
--model file.trf
	classifer to use
--classcol name
	specify the name of the column that contain the class/label
--tree_samples n
	Fraction of samples per tree (to override default)
--tree_features n
	Number of features per tree
--min_leaf_size
	How small a node be before it is automatically turned into a leaf
--verbose n
	Set verbosity level
--entropy
	Split on entropy instead of Gini impurity
--xfold n
	Perform n-fold cross validataion

Example: 10-fold cross validation
$ trafo --xfold 10 --train file.csv
```
</details>


<details> <summary>Example output -- training on wine.tsv</summary>

```
$ trafo_cli --version
trafo_cli version 0.1.3

$ trafo_cli --train wine.tsv  --ntree 1 --entropy
Reading from wine.tsv
columns: 14, data rows: 178
Found feature column "class"
Features provided in row major format (to be transposed)
Label array provided
Number of features: 13
Number of samples: 178
Number of trees: 1
Fraction of samples per tree: 1.00
Features per tree: 13
min_samples_leaf: 1
Largest label id: 2
Splitting criterion: Entropy
trafo: Forest training took 0.0020 s
Feature importance*:
#  0 : 2.1 %
#  1 : 0.0 %
#  2 : 2.1 %
#  3 : 0.0 %
#  4 : 0.0 %
#  5 : 0.0 %
#  6 : 42.1 %
#  7 : 0.0 %
#  8 : 0.0 %
#  9 : 21.5 %
# 10 : 0.0 %
# 11 : 0.0 %
# 12 : 32.2 %
VmPeak: 770996 (kb) VmHWM: 1332 (kb)

$ trafo_cli --train wine.tsv  --ntree 200 --entropy
Reading from wine.tsv
columns: 14, data rows: 178
Found feature column "class"
Features provided in row major format (to be transposed)
Label array provided
Number of features: 13
Number of samples: 178
Number of trees: 200
Fraction of samples per tree: 0.63
Features per tree: 4
min_samples_leaf: 1
Largest label id: 2
Splitting criterion: Entropy
trafo: Forest training took 0.0071 s
Feature importance*:
#  0 : 9.8 %
#  1 : 4.8 %
#  2 : 3.0 %
#  3 : 3.5 %
#  4 : 4.3 %
#  5 : 6.0 %
#  6 : 17.0 %
#  7 : 1.9 %
#  8 : 3.1 %
#  9 : 10.2 %
# 10 : 9.2 %
# 11 : 13.5 %
# 12 : 13.7 %
VmPeak: 978908 (kb) VmHWM: 2872 (kb)
```
</details>




## To do

- [ ] Single precision features/uint16 labels option for reduced
      memory usage.

- [ ] Proper benchmarks, also with Entropy as partitioning criterion.

- [ ] Tell something about the precision / predictive powers of this
      library vs other implementation.

## See also / Alternatives

- Python:
  [scikit-learn](https://scikit-learn.org/1.5/modules/generated/sklearn.ensemble.RandomForestClassifier.html).
- R: [randomForest](https://cran.r-project.org/web/packages/randomForest/index.html)
- MATAB: [TreeBagger](https://www.mathworks.com/help/stats/treebagger.html)
