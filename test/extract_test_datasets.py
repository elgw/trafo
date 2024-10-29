#pip install scikit-learn
import numpy as np
from sklearn.datasets import *

# https://stackoverflow.com/questions/43640546/how-to-make-randomforestclassifier-faster
datasets = ['iris', 'digits', 'wine', 'breast_cancer','diabetes', 'rand']

for name in datasets:
    print("Processing " + name)
    if name == 'rand':
        ns = 100000
        nf = 100
        header = []
        for kk in range(0, nf):
            header.append(f'f{kk}')
        header.append('class')
        data = np.random.rand(ns, nf)
        target = np.random.randint(low=0, high=2, size=(ns, 1))
        data_array = np.concatenate((data, target), axis=1)
    else:
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
