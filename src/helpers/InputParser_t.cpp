#include "helpers/InputParser_t.hpp"

APTracer::Helpers::InputParser_t::InputParser_t(int& argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        tokens_.emplace_back(argv[i]);
    }
}

/// @author iain
auto APTracer::Helpers::InputParser_t::getCmdOption(const std::string& option) const -> std::string {
    auto itr = std::find(tokens_.begin(), tokens_.end(), option);
    if (itr != tokens_.end() && ++itr != tokens_.end()) {
        return *itr;
    }
    return "";
}

/// @author iain
auto APTracer::Helpers::InputParser_t::cmdOptionExists(const std::string& option) const -> bool {
    return std::find(tokens_.begin(), tokens_.end(), option) != tokens_.end();
}