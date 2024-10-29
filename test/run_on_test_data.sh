#!/bin/bash

function test_tree()
{
    echo ""
    echo " ----- $1 "
    echo ""
    #${bin} --train $1.tsv --cout $1.trafo --ntree 1
    #${bin} --predict $1.tsv --model $1.trafo
    python skl_rf_tree.py $1.tsv
}

function test_forest()
{
    echo ""
    echo " ----- $1 "
    echo ""
    ${bin} --train $1.tsv --cout $1.trafo --ntree 100
    ${bin} --predict $1.tsv --model $1.trafo
    python skl_rf_forest.py $1.tsv
}

bin=../src/trafo_cli


#test_tree "iris"
#test_tree "digits"
#test_tree "wine"
#test_tree "breast_cancer"
#test_tree "diabetes"

test_forest "iris"
test_forest "digits"
test_forest "wine"
test_forest "breast_cancer"
test_forest "diabetes"
