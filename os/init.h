//
// Created by impodog on 7/28/2023.
//

#ifndef OS_INIT_H
#define OS_INIT_H

#include "pups.h"

namespace pups_extension::os {
    using std::filesystem::directory_entry, std::filesystem::path;
    using std::filesystem::directory_iterator, std::filesystem::recursive_directory_iterator;

    class OsError : public pups::library::Error {
    public:
        explicit OsError(const std::string &s);

        std::string error_name() const noexcept override;
    };

    class DirEntry : public pups::HasMethods {
    public:
        directory_entry entry;

        explicit DirEntry(const path &path);

        explicit DirEntry(directory_entry entry);

        pups::ObjectPtr put(pups::ObjectPtr &object, pups::Map *map) override;

        pups::FunctionCore get_method(const pups::Id &name) override;

        std::string get() const noexcept;
    };

    void init_info(pups::PackageHandler &handler);

    void init_path(pups::PackageHandler &handler);

    void init_entry(pups::PackageHandler &handler);

    void init_iter(pups::PackageHandler &handler);

    void init_cmd(pups::PackageHandler &handler);
}

#endif //OS_INIT_H
