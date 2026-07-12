#pragma once

#include <cassert>
#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace ecs {

using Entity = std::uint32_t;
static constexpr Entity null_entity = 0;

struct IComponentStore {
  virtual ~IComponentStore() = default;
  virtual void erase(Entity e) = 0;
  virtual bool has(Entity e) const = 0;
};

template <typename T> class ComponentStore : public IComponentStore {
public:
  void emplace(Entity e, T value) {
    if (!index.contains(e)) {
      index[e] = data.size();
      entities.push_back(e);
      data.push_back(std::move(value));
    } else {
      data[index[e]] = std::move(value);
    }
  }

  T *get(Entity e) {
    auto it = index.find(e);
    if (it == index.end())
      return nullptr;
    return &data[it->second];
  }

  const T *get(Entity e) const {
    auto it = index.find(e);
    if (it == index.end())
      return nullptr;
    return &data.at(it->second);
  }

  bool has(Entity e) const override { return index.contains(e); }

  void erase(Entity e) override {
    auto it = index.find(e);
    if (it == index.end())
      return;

    size_t i = it->second;
    size_t last = data.size() - 1;

    data[i] = std::move(data[last]);
    Entity moved_entity = entities[last];
    entities[i] = moved_entity;
    index[moved_entity] = i;

    data.pop_back();
    entities.pop_back();
    index.erase(e);
  }

  const std::vector<Entity> &get_entities() const { return entities; }

  std::vector<T> &raw() { return data; }
  const std::vector<T> &raw() const { return data; }

private:
  std::unordered_map<Entity, size_t> index;
  std::vector<Entity> entities;
  std::vector<T> data;
};

class Registry {
public:
  Entity create() { return next_entity++; }

  void destroy(Entity e) {
    for (auto &[_, store] : stores) {
      store->erase(e);
    }
  }

  template <typename T> void add(Entity e, T value) {
    get_or_create<T>().emplace(e, std::move(value));
  }

  template <typename T> T *get(Entity e) { return get_or_create<T>().get(e); }

  template <typename T> bool has(Entity e) const {
    auto store = get_store<T>();
    return store ? store->has(e) : false;
  }

  template <typename... Ts> auto find() -> std::optional<std::tuple<Ts &...>> {
    for (auto tuple : view<Ts...>()) {
      return std::apply(
          [](Entity, Ts &...components) {
            return std::optional<std::tuple<Ts &...>>(
                std::tuple<Ts &...>(components...));
          },
          tuple);
    }

    return std::nullopt;
  }

  template <typename T> T *first() {
    for (auto [e, c] : view<T>()) {
      return &c;
    }
    return nullptr;
  }

  template <typename T> std::optional<Entity> firstEntity() {
    for (auto [e, _] : view<T>()) {
      return e;
    }
    return std::nullopt;
  }

  template <typename... Ts> class View {
  public:
    View(Registry &reg) : registry(reg) {
      std::tuple<ComponentStore<Ts> *...> stores = {
          &reg.get_or_create<Ts>()...};

      smallest = pick_smallest(stores);
    }

    struct iterator {
      View *view;
      size_t index;

      void advance_to_valid() {
        while (index < view->entities.size()) {
          Entity e = view->entities[index];
          if ((view->registry.has<Ts>(e) && ...))
            break;
          ++index;
        }
      }

      iterator(View *v, size_t i) : view(v), index(i) { advance_to_valid(); }

      iterator &operator++() {
        ++index;
        advance_to_valid();
        return *this;
      }

      bool operator!=(const iterator &other) const {
        return index != other.index;
      }

      std::tuple<Entity, Ts &...> operator*() const {
        Entity e = view->entities[index];
        return {e, *view->registry.get<Ts>(e)...};
      }
    };

    iterator begin() { return iterator(this, 0); }
    iterator end() { return iterator(this, entities.size()); }

  private:
    Registry &registry;
    std::vector<Entity> entities;
    size_t smallest;

    template <typename Tuple> size_t pick_smallest(Tuple &stores) {
      size_t min_size = SIZE_MAX;

      std::apply(
          [&](auto *...s) {
            ((min_size = std::min(min_size, s->get_entities().size())), ...);
          },
          stores);

      std::apply(
          [&](auto *...s) {
            ((entities =
                  (s->get_entities().size() == min_size ? s->get_entities()
                                                        : entities)),
             ...);
          },
          stores);

      return min_size;
    }
  };

  template <typename... Ts> View<Ts...> view() { return View<Ts...>(*this); }

private:
  Entity next_entity = 1;

  std::unordered_map<std::type_index, std::unique_ptr<IComponentStore>> stores;

  template <typename T> ComponentStore<T> &get_or_create() {
    auto key = std::type_index(typeid(T));

    auto it = stores.find(key);
    if (it == stores.end()) {
      auto ptr = std::make_unique<ComponentStore<T>>();
      auto *raw = ptr.get();
      stores[key] = std::move(ptr);
      return *raw;
    }

    return *static_cast<ComponentStore<T> *>(it->second.get());
  }

  template <typename T> ComponentStore<T> *get_store() const {
    auto it = stores.find(std::type_index(typeid(T)));
    if (it == stores.end())
      return nullptr;
    return static_cast<ComponentStore<T> *>(it->second.get());
  }
};
} // namespace ecs
