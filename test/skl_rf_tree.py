import time
import numpy as np
import os
import sys


def get_peak_memory():
    hwm = 0
    with open('/proc/self/status', 'r') as fid:
        for line in fid:
            if 'VmHWM' in line:
                parts = line.split(' ')
                hwm = int(parts[-2])

    return hwm



def read_tsv(fname):
    with open(fname, "r") as fid:
        header = fid.readline()
        data = np.fromfile(fid, sep='\t')
    header = header.split('\t')
    #breakpoint()
    data = data.reshape((len(data)//len(header), len(header)))
    return header, data


def print_peak_memory():
    with open('/proc/self/status', 'r') as fid:
        for line in fid:
            if 'VmPeak' in line:
                print(line.strip())
            if 'VmHWM' in line:
                print(line.strip())


if __name__ == '__main__':
    # print_peak_memory()

    (header, data) = read_tsv(sys.argv[1])
    X = data[:,0:-1]
    # Y = data[:,-1:]
    Y = data[:,-1]


    from sklearn.ensemble import RandomForestClassifier

    mem0 = get_peak_memory()
    t1 = time.perf_counter()
    clf = RandomForestClassifier(n_estimators=1)

    clf.n_jobs=-1
    clf.max_features=X.shape[1]
    clf.min_samples_split=2
    clf.bootstrap = False



    clf = clf.fit(X, Y)
    mem1 = get_peak_memory()
    t2 = time.perf_counter()

    print(f"Delta memory: {mem1-mem0} kb")

    P = clf.predict(X)
    print(f"sk: {np.sum(P==Y)} / {len(P)} predicted correctly")

    t3 = time.perf_counter()
    print(clf.get_params())
    print_peak_memory()
    print(f"sk_timing: Training: {t2-t1:.4f} Prediction {t3-t2:.4f}")
