#ifndef GIT2_CONFIG_HPP
#define GIT2_CONFIG_HPP

#include <string>
#include <git2/config.h>
#include <git2-cpp/global.hpp>
#include <git2-cpp/exception.hpp>

namespace git2 {
    class config {
        git_config* raw;

    public:
        config() {
            git2::init();
            git2_throw(git_config_new(&this->raw));
        }
        ~config() {
            git_config_free(this->raw);
        }

        explicit config(git_config* raw) : raw(raw) {}

        config(const config&) = delete;
        config& operator=(const config&) = delete;
        config(config&&) = default;
        config& operator=(config&&) = default;

        /// Get the value of a string config variable as an owned string.
        std::string get_string(const std::string& name) {
            git_buf ret = { nullptr, 0, 0 };
            git2_throw(git_config_get_string_buf(&ret, this->raw, name.c_str()));
            return std::string(ret.ptr, ret.size);
        }
    };
} // end namespace git2

#endif	// !GIT2_CONFIG_HPP
