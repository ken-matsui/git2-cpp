#ifndef GIT2_CPP_DESCRIBE_HPP
#define GIT2_CPP_DESCRIBE_HPP

#include <string>

#include <git2/describe.h>
#include <git2-cpp/global.hpp>
#include <git2-cpp/exception.hpp>
#include <git2-cpp/repository.hpp>

namespace git2 {
    struct describe_options {
        git_describe_options opts;

        describe_options();
        ~describe_options() noexcept = default;

        describe_options(const describe_options&) = default;
        describe_options& operator=(const describe_options&) = default;
        describe_options(describe_options&&) = default;
        describe_options& operator=(describe_options&&) = default;

        describe_options& max_candidates_tags(const unsigned int max);
        describe_options& describe_tags();
        describe_options& describe_all();
        describe_options& only_follow_first_parent(const bool follow);
        describe_options& show_commit_oid_as_fallback(const bool show);
        describe_options& pattern(const std::string& pattern);
    };

    describe_options::describe_options() {
        git2_throw(git_describe_init_options(&opts, GIT_DESCRIBE_OPTIONS_VERSION));
    }

    describe_options&
    describe_options::max_candidates_tags(const unsigned int max) {
        opts.max_candidates_tags = max;
        return *this;
    }

    /// Sets the reference lookup strategy
    ///
    /// This behaves like the `--tags` option to git-describe.
    describe_options&
    describe_options::describe_tags() {
        opts.describe_strategy = GIT_DESCRIBE_TAGS;
        return *this;
    }

    /// Sets the reference lookup strategy
    ///
    /// This behaves like the `--all` option to git-describe.
    describe_options&
    describe_options::describe_all() {
        opts.describe_strategy = GIT_DESCRIBE_ALL;
        return *this;
    }

    /// Indicates when calculating the distance from the matching tag or
    /// reference whether to only walk down the first-parent ancestry.
    describe_options&
    describe_options::only_follow_first_parent(const bool follow) {
        opts.only_follow_first_parent = follow;
        return *this;
    }

    /// If no matching tag or reference is found whether a describe option would
    /// normally fail. This option indicates, however, that it will instead fall
    /// back to showing the full id of the commit.
    describe_options&
    describe_options::show_commit_oid_as_fallback(const bool show) {
        opts.show_commit_oid_as_fallback = show;
        return *this;
    }

    describe_options&
    describe_options::pattern(const std::string& pattern) {
        opts.pattern = pattern.c_str();
        return *this;
    }


    struct describe_format_options {
        git_describe_format_options opts;

        describe_format_options();
        ~describe_format_options() = default;

        describe_format_options(const describe_format_options&) = default;
        describe_format_options& operator=(const describe_format_options&) = default;
        describe_format_options(describe_format_options&&) = default;
        describe_format_options& operator=(describe_format_options&&) = default;

        describe_format_options& abbreviated_size(const unsigned int size);
        describe_format_options& always_use_long_format(const bool long_f);
        describe_format_options& dirty_suffix(const std::string& suffix);
    };

    describe_format_options::describe_format_options() {
        git2_throw(git_describe_init_format_options(&opts, GIT_DESCRIBE_FORMAT_OPTIONS_VERSION));
    }

    /// Sets the size of the abbreviated commit id to use.
    ///
    /// The value is the lower bound for the length of the abbreviated string,
    /// and the default is 7.
    describe_format_options&
    describe_format_options::abbreviated_size(const unsigned int size) {
        opts.abbreviated_size = size;
        return *this;
    }

    /// Sets whether or not the long format is used even when a shorter name
    /// could be used.
    describe_format_options&
    describe_format_options::always_use_long_format(const bool long_f) {
        opts.always_use_long_format = long_f;
        return *this;
    }

    /// If the workdir is dirty and this is set, this string will be appended to
    /// the description string.
    describe_format_options&
    describe_format_options::dirty_suffix(const std::string& suffix) {
        opts.dirty_suffix = suffix.c_str();
        return *this;
    }


    struct describe {
        git_describe_result* result = nullptr;

        describe() = default;
        ~describe();

        describe(const describe&) = delete;
        describe& operator=(const describe&) = delete;
        describe(describe&&) = default;
        describe& operator=(describe&&) = default;

        describe& workdir(const repository& repo, describe_options& opts);
        std::string format(const describe_format_options& opts);
    };

    describe::~describe() {
        git_describe_result_free(result);
    }

    describe&
    describe::workdir(const repository& repo, describe_options& opts) {
        git2_throw(git_describe_workdir(&result, repo.repo, &opts.opts));
        return *this;
    }

    std::string
    describe::format(const describe_format_options& opts) {
        git_buf ret = { nullptr, 0, 0 };
        git2_throw(git_describe_format(&ret, result, &opts.opts));
        return std::string(ret.ptr, ret.size);
    }
} // end namespace git2::describe

#endif	// !GIT2_CPP_DESCRIBE_HPP
