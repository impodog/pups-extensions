//
// Created by impodog on 7/28/2023.
//

#include "init.h"

namespace pups_extension::sys {
    class Version : public pups::HasMethods {
    public:
        int major, minor, bugfix;

        explicit Version(const std::string &version) {
            int status = 0;
            std::string tmp;
            for (auto c: version) {
                if (c == '.') {
                    switch (status) {
                        case 0:
                            major = std::stoi(tmp);
                            break;
                        case 1:
                            minor = std::stoi(tmp);
                            break;
                        default:
                            throw std::invalid_argument("Too many dots in a version, 2 expected.");
                    }
                    status++;
                    tmp.clear();
                } else
                    tmp.push_back(c);
            }
            if (status == 2)
                bugfix = std::stoi(tmp);
            else
                throw std::invalid_argument("Too few dots in a version, 2 expected.");
        }

        pups::ObjectPtr put(pups::ObjectPtr &object, pups::Map *map) override {
            map->throw_error(std::make_shared<pups::library::TypeError>("Version object does not support putting."));
            return nullptr;
        }

        pups::FunctionCore get_method(const pups::Id &name) override;

        std::string str() const noexcept override {
            return std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(bugfix);
        }

        std::string type_name() const noexcept override {
            return "version";
        }

        bool satisfies(const Version &rhs) const noexcept {
            if (major > rhs.major) return true;
            if (major == rhs.major) {
                if (minor > rhs.minor) return true;
                if (minor == rhs.minor) {
                    if (bugfix >= rhs.bugfix) return true;
                    else return false;
                } else return false;
            } else return false;
        }
    };

    class VersionInit : public pups::Function {
    public:
        VersionInit() : Function([](pups::FunctionArgs &args, pups::Map *map) -> pups::ObjectPtr {
            if (args.size() != 1) {
                map->throw_error(
                        std::make_shared<pups::library::ArgumentError>("Version requires one only argument."));
            } else {
                auto ptr = pups::cast<pups::String>(*args.front());
                if (ptr) {
                    try {
                        return std::make_shared<Version>(ptr->data());
                    } catch (const std::invalid_argument &exc) {
                        map->throw_error(std::make_shared<pups::library::ValueError>(exc.what()));
                    }
                } else {
                    map->throw_error(
                            std::make_shared<pups::library::TypeError>("Version requires a string argument."));
                }
            }
            return pups::pending;
        }) {}
    };

    pups::ObjectPtr ver_satisfies(Version &version, pups::FunctionArgs &args, pups::Map *map) {
        if (args.size() != 1) {
            map->throw_error(
                    std::make_shared<pups::library::ArgumentError>("Version.satisfies requires one only argument."));
        } else {
            auto ptr = pups::cast<Version>(*args.front());
            if (ptr) {
                return version.satisfies(*ptr) ? pups::True : pups::False;
            } else {
                map->throw_error(
                        std::make_shared<pups::library::TypeError>("Version.satisfies requires a Version object."));
            }
        }
        return pups::pending;
    }

    pups::ObjectPtr ver_major(Version &version, pups::FunctionArgs &args, pups::Map *map) {
        if (!args.empty()) {
            map->throw_error(
                    std::make_shared<pups::library::ArgumentError>("Version.major requires no arguments."));
        } else {
            return std::make_shared<pups::Int>(version.major);
        }
        return pups::pending;
    }

    pups::ObjectPtr ver_minor(Version &version, pups::FunctionArgs &args, pups::Map *map) {
        if (!args.empty()) {
            map->throw_error(
                    std::make_shared<pups::library::ArgumentError>("Version.minor requires no arguments."));
        } else {
            return std::make_shared<pups::Int>(version.minor);
        }
        return pups::pending;
    }

    pups::ObjectPtr ver_bugfix(Version &version, pups::FunctionArgs &args, pups::Map *map) {
        if (!args.empty()) {
            map->throw_error(
                    std::make_shared<pups::library::ArgumentError>("Version.bugfix requires no arguments."));
        } else {
            return std::make_shared<pups::Int>(version.bugfix);
        }
        return pups::pending;
    }

    using VersionFunction = std::function<pups::ObjectPtr(Version &, pups::FunctionArgs &args, pups::Map *map)>;
    using VersionFunctionMap = pups::library::IdMap<VersionFunction>;

    pups::Id id_satisfies{"", "satisfies"},
            id_major{"", "get_major"}, id_minor{"", "get_minor"}, id_bugfix{"", "get_bugfix"};

    VersionFunctionMap version_functions = {
            {id_satisfies, ver_satisfies},
            {id_major,     ver_major},
            {id_minor,     ver_minor},
            {id_bugfix,    ver_bugfix}
    };

    pups::FunctionCore Version::get_method(const pups::Id &name) {
        auto &core = version_functions.at(name);
        return [this, &core](pups::FunctionArgs &args, pups::Map *map) {
            return core(*this, args, map);
        };
    }

    pups::Id id_versionInit{"", "Ver"}, id_sysVersion{"", "version"};

    void init_ver(pups::PackageHandler &handler) {
        handler.add(id_versionInit, std::make_shared<VersionInit>());
        handler.add(id_sysVersion, std::make_shared<Version>(pups::PUPS_LIB_VERSION));
    }
}