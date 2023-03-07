#include <fstream>
#include <iostream>
#include <gtest/gtest.h>
#include <catboost/wrapped_calcer.h>

#include "utils.h"

using namespace std;

TEST(CatboostClassifier, testModel) {
    ModelCalcerWrapper calcer("fixtures/model.cbm");
    ifstream testData{"fixtures/test_data_catboost.txt"};

    for (string line; getline(testData, line);) {
        auto [expected, features] = parseTestData(line);
        EXPECT_EQ(predict(calcer, features), expected);
    }
}
