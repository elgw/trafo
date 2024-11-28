#!/bin/bash
set -e

stress -t 4s --cpu 4

function test_tree()
{
    echo ""
    echo " ----- $1 -- Single Tree "
    echo ""
    echo "Training"
    echo ""
    ${bin} --train $1.tsv --cout $1.trafo --ntree 1
    echo ""
    echo "Preciction"
    echo ""
    ${bin} --predict $1.tsv --model $1.trafo
    echo ""
    python skl_rf_tree.py $1.tsv
}

function test_forest()
{
    echo ""
    echo " ----- $1 -- Forest"
    echo ""
    echo "Training"
    echo ""
    ${bin} --train $1.tsv --cout $1.trafo --ntree 100
    echo ""
    echo "Prediction"
    echo ""
    ${bin} --predict $1.tsv --model $1.trafo
    echo ""
    python skl_rf_forest.py $1.tsv
}

bin=../src/trafo_cli


test_tree "iris"
test_tree "digits"
test_tree "wine"
test_tree "breast_cancer"
test_tree "diabetes"
test_tree "rand6"
test_forest "iris"
test_forest "digits"
test_forest "wine"
test_forest "breast_cancer"
test_forest "diabetes"
test_forest "rand6"
