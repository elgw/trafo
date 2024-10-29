#pip install scikit-learn
import numpy as np
from sklearn.datasets import *

# https://stackoverflow.com/questions/43640546/how-to-make-randomforestclassifier-faster
datasets = ['iris', 'digits', 'wine', 'breast_cancer','diabetes']

for name in datasets:
    print("Processing " + name)
    dataset = globals()['load_' + name]()
    data = dataset.data
    target = dataset.target
    target = target.reshape((len(target), 1))
    data_array = np.concatenate((data, target), axis=1)
    # breakpoint()
    header = dataset.feature_names
    header = list(map(str, header))
    header.append('class');
    np.savetxt(name + ".tsv", data_array,
               header = '\t'.join(header),
               delimiter='\t', fmt='%.2f')
