#include "converter_with_additional_params.hpp"

namespace conv {
  void ConverterWithAdditionalParams::setParams(const std::vector<size_t>& kInStreams,
                                                const std::vector<size_t>& kParams) {
    Converter::setParams(kInStreams, kParams);
    for( size_t i = 2; i < kParams.size(); ++i ) {
      OtherParams.push_back(kParams[i]);
    }
  }
}// namespace conv
