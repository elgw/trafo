**trafo** (version 0.1.3, [CHANGELOG](CHANGELOG.md)) is a tiny [random
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

trafo * T = trafo_load("classifier.trafo");
uint32_t * class = trafo_predict(T, features, NULL, n_features);
trafo_free(T);
```


Train a classifier on labelled data and save it to disk:

``` C
#include <trafo.h>

// Basic configuration via a struct.
trafo_conf C = {0};
// Required: Describe the data
C.n_sample = n_sample;
C.n_feature = n_feature;
C.F_row_major = F;       // Input: Features
C.label = L;             // Input: Labels
// Optional: Algorithm settings
C.n_tree = n_tree;
// .. and possibly more.

// Fitting / Training
trafo * T = trafo_fit(C);

// Use it ...

// Save to disk
trafo_save(T, "classifier.trafo");

trafo_free(T); // And done
```

see `trafo.h` for the full API. For more examples, look in `trafo_cli.c`.

## Performance hints

Benchmarks should, among other things, provide averages over multiple
runs. There are only results from single runs reported here. Test
system: 4-core AMD Ryzen 3 PRO 3300U.

The random forest implementation in scikit-learn is denoted skl in the
tables. The memory usage metric are not directly comparable since the
values for **trafo** includes the whole cli interface.  For skl it is
just the delta value, i.e. difference in RSS memory before and after
the call to the `.fit` method, measured with a procedure like this:

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

Results for tree construction:

| bin   | dataset       | time (s) | RSS (kb) |
|-------|---------------|----------|---------:|
| trafo | iris          | 0.002    |     2436 |
| trafo | digits        | 0.020    |     6152 |
| trafo | wine          | 0.006    |     2456 |
| trafo | breast_cancer | 0.003    |     2928 |
| trafo | diabetes      | 0.016    |     2648 |
| trafo | rand          | 3.256    |   323660 |
| skl   | iris          | 0.015    |     1612 |
| skl   | digits        | 0.036    |     2192 |
| skl   | wine          | 0.016    |     1428 |
| skl   | breast_cancer | 0.016    |     1428 |
| skl   | diabetes      | 0.027    |     3712 |
| sk1   | rand          | 13.96    |    48344 |

In all cases the input data is correctly classified.

## A forest with 100 trees

For this test, skl was run by:

``` python
clf = RandomForestClassifier(n_estimators=100)
clf.n_jobs=-1
clf.min_samples_split=2
```

| bin   | dataset       | time (s) | RSS (kb) |
|-------|---------------|----------|---------:|
| trafo | iris          | 0.045    |     2548 |
| trafo | digits        | 0.094    |     6940 |
| trafo | wine          | 0.004    |     2755 |
| trafo | breast_cancer | 0.015    |     3416 |
| trafo | diabetes      | 0.121    |     3344 |
| trafo | rand          | 6.97     |   283088 |
| skl   | iris          | 0.186    |     2208 |
| skl   | digits        | 0.225    |     9412 |
| skl   | wine          | 0.198    |     2512 |
| skl   | breast_cancer | 0.192    |     2340 |
| skl   | diabetes      | 0.224    |    98548 |
| sk1   | rand          | 31.80    |   283560 |

The skl memory usage stand out on the diabetes dataset, due to the high
number of classes?

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
