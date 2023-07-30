//
// Created by impodog on 7/30/2023.
//

#include "init.h"

namespace pups_extension::alg {
    template<typename HasData>
    struct SortType {
        using type = HasData;

        static pups::ObjectPtr work(type &iter, pups::FunctionArgs &args, pups::Map *map) {
            args.pop_front();
            auto func = pups::cast<pups::Function>(*args.front());
            if (func) {
                std::sort(iter.data.begin(), iter.data.end(),
                          pups::library::builtins::compare::get_compare(func, map));
            } else {
                map->throw_error(
                        std::make_shared<pups::library::TypeError>(
                                "alg.sort requires a function as an additional argument, instead of "
                                + args.front()->get()->repr() +
                                ". No operation is done."));
            }
            return pups::pending;
        }
    };

    pups::ObjectPtr sort(pups::FunctionArgs &args, pups::Map *map) {
        if (args.size() != 2) {
            map->throw_error(
                    std::make_shared<pups::library::ArgumentError>(
                            "alg.sort requires 2 arguments."
                            " No operation is done."));
        } else {
            return pups::library::utility::work_with_iter
                    <SortType<pups::Array>, SortType<pups::Deque>>
                    (args, map);
        }
        return pups::pending;
    }

    template<typename HasData>
    struct StableSortType {
        using type = HasData;

        static pups::ObjectPtr work(type &iter, pups::FunctionArgs &args, pups::Map *map) {
            args.pop_front();
            auto func = pups::cast<pups::Function>(*args.front());
            if (func) {
                std::stable_sort(iter.data.begin(), iter.data.end(),
                                 pups::library::builtins::compare::get_compare(func, map));
            } else {
                map->throw_error(
                        std::make_shared<pups::library::TypeError>(
                                "alg.stable_sort requires a function as an additional argument, instead of "
                                + args.front()->get()->repr() +
                                ". No operation is done."));
            }
            return pups::pending;
        }
    };

    pups::ObjectPtr stable_sort(pups::FunctionArgs &args, pups::Map *map) {
        if (args.size() != 2) {
            map->throw_error(
                    std::make_shared<pups::library::ArgumentError>(
                            "alg.stable_sort requires 2 arguments."
                            " No operation is done."));
        } else {
            return pups::library::utility::work_with_iter
                    <StableSortType<pups::Array>, StableSortType<pups::Deque>>
                    (args, map);
        }
        return pups::pending;
    }

    template<typename HasData>
    struct MergeType {
        using type = HasData;

        static pups::ObjectPtr merge_with_func(type &iter, const std::shared_ptr<type> &ptr,
                                               const pups::FunctionArgs &args, pups::Map *map) {
            auto func = pups::cast<pups::Function>(*args.back());
            if (func) {
                data_type_of<type> result = create_sized<type>(iter.size() + ptr->size());

                std::merge(get_data(iter).begin(), get_data(iter).end(),
                           get_data(*ptr).begin(), get_data(*ptr).end(),
                           result.begin(), pups::library::builtins::compare::get_compare(func, map)
                );
                return std::make_shared<type>(result);
            } else {
                map->throw_error(
                        std::make_shared<pups::library::TypeError>(
                                "alg.merge requires a function as an additional argument, instead of "
                                + args.back()->get()->repr() +
                                ". pending is returned."));
            }
            return pups::pending;
        }

        static pups::ObjectPtr work(type &iter, pups::FunctionArgs &args, pups::Map *map) {
            args.pop_front();
            auto ptr = pups::cast<type>(*args.front());
            if (ptr) {
                return merge_with_func(iter, ptr, args, map);
            } else {
                map->throw_error(
                        std::make_shared<pups::library::TypeError>(
                                "alg.merge requires first 2 iterables of the same type, instead of "
                                + iter.repr() + " and " + args.front()->get()->repr() +
                                ". pending is returned."));
            }
            return pups::pending;
        }
    };

    pups::ObjectPtr merge(pups::FunctionArgs &args, pups::Map *map) {
        if (args.size() != 3) {
            map->throw_error(
                    std::make_shared<pups::library::ArgumentError>(
                            "alg.merge requires 3 only arguments."
                            " pending is returned."));
        } else {
            return pups::library::utility::work_with_iter
                    <MergeType<pups::Array>, MergeType<pups::Deque >>
                    (args, map);
        }
        return pups::pending;
    }

    pups::Id id_sort{"", "sort"}, id_stable_sort{"", "stable_sort"}, id_merge{"", "merge"};

    void init_iter(pups::PackageHandler &handler) {
        handler.add(id_sort, std::make_shared<pups::Function>(sort));
        handler.add(id_stable_sort, std::make_shared<pups::Function>(stable_sort));
        handler.add(id_merge, std::make_shared<pups::Function>(merge));
    }
}