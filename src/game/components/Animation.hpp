#pragma once

#include <chrono>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

using AnimationValue = std::variant<bool, int, float>;
using AnimationParameters = std::unordered_map<std::string, AnimationValue>;

enum class AnimationComparisonOp {
  Equal,
  NotEqual,
  Greater,
  Less,
  GreaterEqual,
  LessEqual
};

struct AnimationCondition {
  std::string parameter;
  AnimationComparisonOp op = AnimationComparisonOp::Equal;
  AnimationValue value;

  bool matches(const AnimationParameters &parameters) const {
    auto it = parameters.find(parameter);
    if (it == parameters.end()) {
      return false;
    }

    return compare(it->second);
  }

private:
  bool compare(const AnimationValue &current) const {
    return std::visit(
        [&](const auto &lhs) -> bool {
          using T = std::decay_t<decltype(lhs)>;

          if (!std::holds_alternative<T>(value)) {
            return false;
          }

          const T &rhs = std::get<T>(value);

          switch (op) {
          case AnimationComparisonOp::Equal:
            return lhs == rhs;

          case AnimationComparisonOp::NotEqual:
            return lhs != rhs;

          case AnimationComparisonOp::Greater:
            if constexpr (std::is_arithmetic_v<T>) {
              return lhs > rhs;
            }
            return false;

          case AnimationComparisonOp::Less:
            if constexpr (std::is_arithmetic_v<T>) {
              return lhs < rhs;
            }
            return false;

          case AnimationComparisonOp::GreaterEqual:
            if constexpr (std::is_arithmetic_v<T>) {
              return lhs >= rhs;
            }
            return false;

          case AnimationComparisonOp::LessEqual:
            if constexpr (std::is_arithmetic_v<T>) {
              return lhs <= rhs;
            }
            return false;
          }

          return false;
        },
        current);
  }
};

struct AnimationRule {
  std::vector<AnimationCondition> conditions;
  std::vector<int> frames;
  int priority = 0;
  bool loop = true;

  bool matches(const AnimationParameters &parameters) const {
    for (const auto &condition : conditions) {
      if (!condition.matches(parameters)) {
        return false;
      }
    }

    return true;
  }
};

struct Animation {
  std::vector<int> frames;
  AnimationParameters parameters;
  std::vector<AnimationRule> rules;
  std::chrono::duration<float> elapsedTime{0.0f};
  std::chrono::duration<float> frameTime{1.0f};
  std::size_t currentFrame = 0;
  bool looping = true;
  int currentSpriteFrame = 0;

  void setParameter(const std::string &key, AnimationValue value) {
    parameters[key] = std::move(value);
  }

  template <typename T>
  T getParameter(const std::string &key, T defaultValue = T{}) const {
    auto it = parameters.find(key);
    if (it == parameters.end()) {
      return defaultValue;
    }

    if (std::holds_alternative<T>(it->second)) {
      return std::get<T>(it->second);
    }

    return defaultValue;
  }

  void sortRulesByPriority() {
    std::sort(rules.begin(), rules.end(),
              [](const AnimationRule &a, const AnimationRule &b) {
                return a.priority > b.priority;
              });
  }
};
