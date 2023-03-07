#pragma once

#include <vector>
#include <string>
#include <utility>
#include <catboost/wrapped_calcer.h>

std::pair<unsigned, std::vector<float> > parseTestData(const std::string& inputStream);
unsigned predict(ModelCalcerWrapper& calcer, const std::vector<float>& features);
