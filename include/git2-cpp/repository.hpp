#ifndef GIT2_REPOSITORY_HPP
#define GIT2_REPOSITORY_HPP

#include <string>
#include <git2/repository.h>
#include <git2-cpp/config.hpp>
#include <git2-cpp/exception.hpp>

namespace git2 {
    struct repository {
        git_repository* raw = nullptr;

        repository() = default;
        ~repository() {
            git_repository_free(this->raw);
        }

        repository(const repository&) = delete;
        repository& operator=(const repository&) = delete;
        repository(repository&&) = default;
        repository& operator=(repository&&) = default;

        /// Attempt to open an already-existing repository at `path`.
        ///
        /// The path can point to either a normal or bare repository.
        repository& open(const std::string& path) {
            git2::init();
            git2_throw(git_repository_open(&this->raw, path.c_str()));
            return *this;
        }
        /// Attempt to open an already-existing bare repository at `path`.
        ///
        /// The path can point to only a bare repository.
        repository& open_bare(const std::string& path) {
            git2::init();
            git2_throw(git_repository_open_bare(&this->raw, path.c_str()));
            return *this;
        }

        /// Creates a new repository in the specified folder.
        ///
        /// This by default will create any necessary directories to create the
        /// repository, and it will read any user-specified templates when creating
        /// the repository. This behavior can be configured through `init_opts`.
        repository& init(const std::string& path) {
            git2::init();
            git2_throw(git_repository_init(&this->raw, path.c_str(), false));
            return *this;
        }
        /// Creates a new `--bare` repository in the specified folder.
        ///
        /// The folder must exist prior to invoking this function.
        repository& init_bare(const std::string& path) {
            git2::init();
            git2_throw(git_repository_init(&this->raw, path.c_str(), true));
            return *this;
        }

        /// Get the configuration file for this repository.
        ///
        /// If a configuration file has not been set, the default config set for the
        /// repository will be returned, including global and system configurations
        /// (if they are available).
        git2::config config() {
            git_config* cfg = nullptr;
            git2_throw(git_repository_config(&cfg, this->raw));
            return git2::config(cfg);
        }
    };
} // end namespace git2

#endif	// !GIT2_REPOSITORY_HPP
