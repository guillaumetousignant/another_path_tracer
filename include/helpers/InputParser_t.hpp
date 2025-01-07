#ifndef APTRACER_HELPERS_INPUTPARSER_T_HPP
#define APTRACER_HELPERS_INPUTPARSER_T_HPP

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

namespace APTracer { namespace Helpers {
    /**
     * @brief The InputParser_t class parses and stores the input arguments of a program.
     *
     * The options can be checked for existence or their value can be retrieved.
     */
    class InputParser_t {
        public:
            /**
             * @brief Construct a new InputParser_t object from command-line arguments.
             *
             * @param argc Number of command-line arguments
             * @param argv Array of command-line arguments
             */
            InputParser_t(int& argc, char** argv);

            /**
             * @brief Returns the value of a command-line option, or an empty string if absent.
             *
             * @param option Command-line option to retrieve
             * @return std::string Value of the option, or an empty string if not found
             */
            auto getCmdOption(const std::string& option) const -> std::string;

            /**
             * @brief Checks if a command-line option was provided.
             *
             * @param option Command-line option to check
             * @return true The command-line option was provided
             * @return false The command-line option was not provided
             */
            auto cmdOptionExists(const std::string& option) const -> bool;

            /**
             * @brief Returns the value of a command-line option, or a default value if absent.
             *
             * @tparam T Type of the option value
             * @param option Command-line option to retrieve
             * @param default_value Default value of the option, that will be returned if absent
             * @return T Value of the option, or default value if not found
             */
            template<typename T>
            auto getCmdOptionOr(const std::string& option, const T& default_value) const -> T {
                auto itr = std::find(tokens_.begin(), tokens_.end(), option);
                if (itr != tokens_.end() && ++itr != tokens_.end()) {
                    std::stringstream sstream(*itr);
                    T result_value;
                    sstream >> result_value;
                    return result_value;
                }
                return default_value;
            }

        private:
            std::vector<std::string> tokens_; /**< @brief Vector of arguments that were provided to the program.*/
    };
}}

#endif
