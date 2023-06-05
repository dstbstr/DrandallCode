#pragma once

#include <vector>
#include <array>
#include <string>
#include <ranges>

namespace Constexpr {
    template<typename R>
    concept nested_range = std::ranges::input_range<R> && std::ranges::range<std::ranges::range_reference_t<R>>;

    struct flatten_t {
        template<nested_range R>
        constexpr auto operator()(R&& r) const {
            return std::forward<R>(r) | std::ranges::views::transform(*this) | std::ranges::views::join;
        }
        template<typename T>
        constexpr auto operator()(T&& t) const {
            return std::forward<T>(t);
        }
    };

    template<typename T>
    constexpr auto operator |(T&& t, flatten_t f) {
        return f(std::forward<T>(t));
    }

    constexpr flatten_t flatten;

    constexpr bool TestFlatten() {
        std::vector<std::vector<std::vector<int>>> v;
        v.push_back({ {1, 2, 3}, {4, 5, 6} });
        v.push_back({ { 7, 8, 9} });

        auto count = 0;
        auto total = 0;
        for (auto x : v | flatten) {
            count++;
            total += x;
        }
        if (count != 9) return false;
        if (total != 45) return false;

        std::vector<std::vector<bool>> v2;
        v2.push_back({ true, true, false });
        v2.push_back({ false, true, false });
        v2.push_back({ false, false, true });
    
        count = 0;
        total = 0;
        for (auto x : v2 | flatten) {
            count++;
            total += x;
        }
        if (count != 9) return false;
        if (total != 4) return false;

        return true;
    }

    static_assert(TestFlatten());

    template<typename Key, typename Value>
    class SmallMap {
    public:
        constexpr SmallMap() = default;
        constexpr SmallMap(const SmallMap& other) : mData(other.mData) {}
        constexpr SmallMap(SmallMap&& other) : mData(std::move(other.mData)) {}
        constexpr SmallMap(const std::initializer_list<std::pair<Key, Value>>& initialState) {
            mData = initialState;
        }

        constexpr SmallMap& operator=(const SmallMap& other) {
            this->mData = other.mData;
            return *this;
        }
        constexpr SmallMap& operator=(SmallMap&& other) {
            this->mData = other.mData;
            return *this;
        }

        constexpr Value& operator[](const Key& key) {
            auto where = FindByKey(key);

            if (where == mData.end()) {
                mData.push_back(std::make_pair(key, Value{}));
                return mData.back().second;
            }
            else {
                return (*where).second;
            }
        }
        constexpr const Value& at(const Key& key) const {
            auto where = FindByKey(key);
            if (where == mData.cend()) {
                throw "Key not found";
            }
            return (*where).second;
        }

        constexpr bool is_empty() const {
            return mData.empty();
        }
        constexpr size_t size() const {
            return mData.size();
        }

        constexpr void clear() {
            mData.clear();
        }
        constexpr bool contains(const Key& key) const {
            return FindByKey(key) != mData.cend();
        }
        constexpr size_t erase(const Key& key) {
            return std::erase_if(mData, [&key](const auto& kvp) { return kvp.first == key; });
        }

        constexpr auto begin() {
            return mData.begin();
        }
        constexpr auto cbegin() const {
            return mData.cbegin();
        }
        constexpr auto end() {
            return mData.end();
        }
        constexpr auto cend() const {
            return mData.cend();
        }

    private:
        std::vector<std::pair<Key, Value>> mData{};

        constexpr auto FindByKey(const Key& key) const {
            return std::find_if(mData.cbegin(), mData.cend(), [&](const auto& kvp) { return kvp.first == key; });
        };
        constexpr auto FindByKey(const Key& key) {
            return std::find_if(mData.begin(), mData.end(), [&](const auto& kvp) { return kvp.first == key; });
        }
    };

    template<typename Key, typename Value>
    constexpr inline auto begin(SmallMap<Key, Value>& map) {
        return map.begin();
    }
    template<typename Key, typename Value>
    constexpr inline auto begin(const SmallMap<Key, Value>& map) {
        return map.cbegin();
    }
    template<typename Key, typename Value>
    constexpr inline auto end(SmallMap<Key, Value>& map) {
        return map.end();
    }
    template<typename Key, typename Value>
    constexpr inline auto end(const SmallMap<Key, Value>& map) {
        return map.cend();
    }

    template<typename T>
    class Stack {
    public:
        constexpr Stack() = default;
        constexpr Stack(const Stack& other) : mData(other.mData) {}
        constexpr Stack(Stack&& other) : mData(std::move(other.mData)) {}
        Stack& operator=(const Stack& other) {
            mData = other.mData;
            return *this;
        }
        Stack& operator=(Stack&& other) {
            mData = other.mData;
            return *this;
        }
        constexpr void push(const T& val) {
            mData.push_back(val);
        }
        constexpr T top() {
            if (mData.empty()) {
                throw "Accessing empty stack";
            }
            return mData.back();
        }
        constexpr void pop() {
            if (mData.empty()) {
                throw "Popped empty stack";
            }
            mData.pop_back();
        }
        constexpr bool is_empty() {
            return mData.empty();
        }
        constexpr void clear() {
            mData.clear();
        }
        constexpr std::size_t size() {
            return mData.size();
        }

    private:
        std::vector<T> mData;
    };

    template<typename T>
    class Queue {
    public:
        constexpr Queue() = default;
        constexpr Queue(const Queue& other) : mData(other.mData) {}
        constexpr Queue(Queue&& other) : mData(std::move(other.mData)) {}
        Queue& operator=(const Queue& other) {
            mData = other.mData;
            return *this;
        }
        Queue& operator=(Queue&& other) {
            mData = other.mData;
            return *this;
        }
        constexpr void push(const T& val) {
            mData.push_back(val);
        }
        constexpr T front() {
            if (mData.empty()) {
                throw "Accessing empty queue";
            }
            return mData[0];
        }
        constexpr void pop() {
            if (mData.empty()) {
                throw "Popping empty queue";
            }
            mData.erase(mData.begin());
        }
        constexpr bool is_empty() {
            return mData.empty();
        }
        constexpr void clear() {
            mData.clear();
        }
        constexpr std::size_t size() {
            return mData.size();
        }

    private:
        std::vector<T> mData;
    };

    /*
    TODO:
    set
    priority queue (used by A*)
    deque (Could improve queue?)
    */
}