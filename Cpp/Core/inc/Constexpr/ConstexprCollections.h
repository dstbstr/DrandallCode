#pragma once

#include <vector>
#include <array>
#include <string>
//#include <ranges>
#include <algorithm>
#include <iterator>

#include "Constexpr/ConstexprHash.h"

namespace Constexpr {
    /*
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
    */

    constexpr auto Without(auto collection, const auto& toRemove) {
        std::erase_if(collection, [&](const auto& e) {
            return std::find(toRemove.begin(), toRemove.end(), e) != toRemove.end();
            });
        return collection;
    }

    constexpr auto Within(auto collection, const auto& toKeep) {
        std::erase_if(collection, [&](const auto& e) {
            return std::find(toKeep.begin(), toKeep.end(), e) == toKeep.end();
            });
        return collection;
    }

    constexpr auto Where(auto collection, auto Pred) {
        std::erase_if(collection, [&](const auto& e) { return !Pred(e); });
        return collection;
    }

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

        constexpr Value& at(const Key& key) {
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
        constexpr auto begin() const {
            return mData.begin();
        }
        constexpr auto cbegin() const {
            return mData.cbegin();
        }
        constexpr auto end() {
            return mData.end();
        }
        constexpr auto end() const {
            return mData.end();
        }
        constexpr auto cend() const {
            return mData.cend();
        }

        constexpr std::vector<Key> GetKeys() const {
            std::vector<Key> result;
            for (const auto& [key, value] : mData) {
                result.push_back(key);
            }
            return result;
        }

        constexpr std::vector<Value> GetValues() const {
            std::vector<Value> result;
            for (const auto& [key, value] : mData) {
                result.push_back(value);
            }
            return result;
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
    constexpr inline auto cbegin(const SmallMap<Key, Value>& map) {
        return map.cbegin();
    }
    template<typename Key, typename Value>
    constexpr inline auto end(SmallMap<Key, Value>& map) {
        return map.end();
    }
    template<typename Key, typename Value>
    constexpr inline auto cend(const SmallMap<Key, Value>& map) {
        return map.cend();
    }

    template<typename Key, typename Value, size_t Capacity = 1'000'000, typename Hasher = Constexpr::Hasher<Key>>
    struct BigMap {
        constexpr BigMap() {
            MakeSentinel();
            mData = new std::array<std::pair<Key, Value>, Capacity>();
            mData->fill(Sentinel);
            mCurrentSize = 0;
        }

        constexpr BigMap(const BigMap& other) : Sentinel(other.Sentinel), mCurrentSize(other.mCurrentSize) {
            mData = new std::array<std::pair<Key, Value>, Capacity>();
            *mData = *other.mData;
        }
        constexpr BigMap(BigMap&& other) : Sentinel(std::move(other.Sentinel)), mCurrentSize(other.mCurrentSize) {
            mData = std::move(other.mData);
            other.mData = nullptr;
        }
        constexpr BigMap(const std::initializer_list<std::pair<Key, Value>>& initialState) {
            MakeSentinel();
            mData = new std::array<std::pair<Key, Value>, Capacity>();
            mData->fill(Sentinel);
            for (const auto& p : initialState) {
                (*mData)[FindSlot(p.first)] = p;
                mCurrentSize++;
            }
        }

        constexpr ~BigMap() {
            delete mData;
        }

        constexpr BigMap& operator=(const BigMap& other) {
            *(this->mData) = (*other.mData);
            Sentinel = other.Sentinel;
            mCurrentSize = other.mCurrentSize;
            return *this;
        }
        constexpr BigMap& operator=(BigMap&& other) {
            if (this == &other) return *this;
            mData = std::move(other.mData);
            other.mData = nullptr;

            Sentinel = other.Sentinel;
            mCurrentSize = other.mCurrentSize;
            return *this;
        }

        constexpr void SetSentinel(Key newSentinel) {
            for (auto& pair : *mData) {
                if (pair == Sentinel) pair.first = newSentinel;
            }
            Sentinel.first = newSentinel;
        }

        constexpr void SetDefaultValue(Value val) {
            mDefaultValue = val;
        }

        constexpr Value& operator[](const Key& key) {
            auto slot = FindSlot(key);
            if (!IsOccupied(slot)) {
                (*mData)[slot] = std::make_pair(key, mDefaultValue);
                mCurrentSize++;
            }
            return (*mData)[slot].second;
        }

        constexpr void emplace(const Key& key, Value value) {
            auto slot = FindSlot(key);
            if (!IsOccupied(slot)) {
                (*mData)[slot] = std::make_pair(key, value);
                mCurrentSize++;
            }
        }

        constexpr Value& at(const Key& key) {
            auto slot = FindSlot(key);
            if (!IsOccupied(slot)) throw "Key not found";
            return (*mData)[slot].second;
        }

        constexpr const Value& at(const Key& key) const {
            auto slot = FindSlot(key);
            if (!IsOccupied(slot)) {
                throw "Key not found";
            }
            return (*mData)[slot].second;
        }

        constexpr bool is_empty() const {
            return size() == 0;
        }
        constexpr size_t size() const {
            return mCurrentSize;
        }

        constexpr void clear() {
            mData->fill(Sentinel);
            mCurrentSize = 0;
        }
        constexpr bool contains(const Key& key) const {
            return IsOccupied(FindSlot(key));
        }
        constexpr size_t erase(const Key& key) {
            auto slot = FindSlot(key);
            if (!IsOccupied(slot)) return 0ull;
            (*mData)[slot] = Sentinel;
            mCurrentSize--;
            for (size_t j = (slot + 1) % Capacity; j != slot; j = (j + 1) % Capacity) {
                if (!IsOccupied(j)) return 1ull;
                size_t k = FindSlot((*mData)[j].first);
                if (slot <= j) {
                    if ((slot < k) && (k <= j)) {
                        continue;
                    }
                }
                else {
                    if ((slot < k) || (k <= j)) {
                        continue;
                    }
                }

                (*mData)[slot] = (*mData)[j];
                (*mData)[j] = Sentinel;
                slot = j;
            }
            //maybe warn that the map is full?
            return 1ull;
        }

        /*
        constexpr auto begin() {
            return mData->begin();
        }
        constexpr auto begin() const {
            return mData->begin();
        }
        constexpr auto cbegin() const {
            return mData->cbegin();
        }
        constexpr auto end() {
            return mData->end();
        }
        constexpr auto end() const {
            return mData->end();
        }
        constexpr auto cend() const {
            return mData->cend();
        }
        */

        constexpr std::vector<Key> GetKeys() const {
            std::vector<Key> result;
            for (const auto& p : *mData) {
                if (p != Sentinel) {
                    result.push_back(p.first);
                }
            }
            return result;
        }

        constexpr std::vector<Value> GetValues() const {
            std::vector<Value> result;
            for (const auto& p : *mData) {
                if (p != Sentinel) {
                    result.push_back(p.second);
                }
            }
            return result;
        }

        constexpr std::vector<std::pair<Key, Value>> GetAllEntries() const {
            std::vector<std::pair<Key, Value>> result;
            std::copy_if(mData->begin(), mData->end(), std::back_inserter(result), [&](auto p) {
                return p != Sentinel;
                });
            return result;
        }
    private:
        std::array<std::pair<Key, Value>, Capacity>* mData{};
        std::pair<Key, Value> Sentinel;
        size_t mCurrentSize{ 0 };
        Hasher mHash{};
        Value mDefaultValue{};

        constexpr void MakeSentinel() {
            if constexpr (std::is_same_v<std::remove_cvref_t<Key>, std::string>) {
                Sentinel = std::make_pair<Key, Value>("SentinelString", {});
            }
            else if constexpr (std::is_arithmetic_v<Key>) {
                Sentinel = std::make_pair<Key, Value>(9919, {});
            }
            else {
                Sentinel = std::make_pair<Key, Value>({}, {}); //bleh
            }
        }

        constexpr size_t FindSlot(const Key& key) const {
            size_t i = mHash(key) % Capacity;
            while ((*mData)[i].first != Sentinel.first && (*mData)[i].first != key) {
                i = (i + 1) % Capacity;
            }
            return i;
        };

        constexpr bool IsOccupied(size_t slot) const {
            return (*mData)[slot].first != Sentinel.first;
        }
    };

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
        constexpr T& top() const {
            if (mData.empty()) throw "Accessing empty stack";
            return mData.back();
        }

        constexpr T& top() {
            if (mData.empty()) throw "Accessing empty stack";
            return mData.back();
        }

        constexpr T pop() {
            if (mData.empty()) {
                throw "Accessing empty stack";
            }
            auto result = mData.back();
            mData.pop_back();
            return result;
        }

        constexpr bool is_empty() const {
            return mData.empty();
        }
        constexpr void clear() {
            mData.clear();
        }
        constexpr std::size_t size() const {
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
        constexpr Queue(T initial) : mData(initial) {}

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
        constexpr T front() const {
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
        constexpr bool is_empty() const {
            return mData.empty();
        }
        constexpr void clear() {
            mData.clear();
        }
        constexpr std::size_t size() const {
            return mData.size();
        }

    private:
        std::vector<T> mData;
    };

    template<typename T>
    struct PriorityQueue {
        constexpr PriorityQueue() = default;
        constexpr PriorityQueue(const PriorityQueue& other) : mData(other.mData) {}
        constexpr PriorityQueue(PriorityQueue&& other) : mData(std::move(other.mData)) {}
        constexpr PriorityQueue(const std::initializer_list<T>& initial) : mData(initial) {
            std::make_heap(mData.begin(), mData.end());
        }

        PriorityQueue& operator=(const PriorityQueue& other) {
            mData = other.mData;
            return *this;
        }
        PriorityQueue& operator=(PriorityQueue&& other) {
            mData = other.mData;
            return *this;
        }

        constexpr void push(T val) {
            mData.push_back(val);
            std::push_heap(mData.begin(), mData.end());
        }

        constexpr void push_or_update(T val) {
            auto existing = std::find(mData.begin(), mData.end(), val);
            if (existing == mData.end()) {
                push(val);
            }
            else {
                if ((*existing) < val) {
                    *existing = val;
                    std::make_heap(mData.begin(), mData.end());
                }
            }
        }

        constexpr T pop() {
            if (mData.empty()) throw "Popped empty queue";
            std::pop_heap(mData.begin(), mData.end());
            auto result = mData.back();
            mData.pop_back();
            return result;
        }

        constexpr bool empty() const {
            return mData.empty();
        }
        constexpr void clear() {
            mData.clear();
        }
        constexpr std::size_t size() const {
            return mData.size();
        }

    private:
        mutable std::vector<T> mData;
    };


    template<typename T>
    class SmallSet {
    public:
        constexpr SmallSet() = default;
        constexpr SmallSet(const SmallSet& other) :mData(other.mData) {}
        constexpr SmallSet(SmallSet&& other) : mData(std::move(other.mData)) {}
        constexpr SmallSet(const std::initializer_list<T>& initial) { insert(initial.begin(), initial.end()); }

        constexpr SmallSet& operator=(const SmallSet& other) {
            this->mData = other.mData;
            return *this;
        }
        constexpr SmallSet& operator=(SmallSet&& other) {
            this->mData = other.mData;
            return *this;
        }

        constexpr bool insert(const T& val) {
            if (mData.size() >= mNextSort) {
                std::sort(mData.begin(), mData.end());
                mNextSort *= 4;
            }
            if (contains(val)) {
                return false;
            }
            
            mData.push_back(val);
            return true;
        }

        constexpr T& operator[](const T& val) {
            if (!contains(val)) {
                throw "Key not found";
            }
            return *std::find(mData.begin(), mData.end(), val);
        }

        constexpr void insert(const auto& begin, const auto& end) {
            for (auto it = begin; it != end; it++) {
                insert(*it);
            }
        }

        constexpr bool contains(const T& val) const {
            return std::find(mData.cbegin(), mData.cend(), val) != mData.cend();
        }

        constexpr void erase(const T& val) {
            mData.erase(std::remove(mData.begin(), mData.end(), val), mData.end());
        }

        constexpr bool empty() {
            return mData.empty();
        }
        constexpr void clear() {
            mData.clear();
        }
        constexpr std::size_t size() {
            return mData.size();
        }

        constexpr auto begin() {
            return mData.begin();
        }
        constexpr auto begin() const {
            return mData.begin();
        }
        constexpr auto cbegin() const {
            return mData.cbegin();
        }
        constexpr auto end() {
            return mData.end();
        }
        constexpr auto end() const {
            return mData.end();
        }
        constexpr auto cend() const {
            return mData.cend();
        }

    private:
        size_t mNextSort{ 512 };
        std::vector<T> mData;
    };

    template<typename T>
    constexpr inline auto begin(SmallSet<T>& set) {
        return set.begin();
    }
    template<typename T>
    constexpr inline auto cbegin(const SmallSet<T>& set) {
        return set.cbegin();
    }
    template<typename T>
    constexpr inline auto end(SmallSet<T>& set) {
        return set.end();
    }
    template<typename T>
    constexpr inline auto cend(const SmallSet<T>& set) {
        return set.cend();
    }

    template<typename T, size_t Capacity = 1'000'000, typename Hasher = Constexpr::Hasher<T>>
    class BigSet {
    public:
        constexpr BigSet() {
            MakeSentinel();
            mData = new std::array<T, Capacity>();
            mData->fill(Sentinel);
        }
        constexpr BigSet(const BigSet& other) : Sentinel(other.Sentinel), mCurrentSize(other.mCurrentSize) {
            mData = new std::array<T, Capacity>();
            *mData = *other.mData;
        }

        constexpr BigSet(BigSet&& other) : Sentinel(other.Sentinel), mCurrentSize(other.mCurrentSize) {
            mData = std::move(other.mData);
            other.mData = nullptr;
        }

        constexpr BigSet(const std::initializer_list<T>& initial) { 
            MakeSentinel();
            mData = new std::array<T, Capacity>();
            mData->fill(Sentinel);

            for (const auto& e : initial) {
                insert(e);
            }
        }
        constexpr ~BigSet() { delete mData; }

        constexpr BigSet& operator=(const BigSet& other) {
            *(this->mData) = *(other.mData);
            Sentinel = other.Sentinel;
            mCurrentSize = other.mCurrentSize;
            return *this;
        }
        constexpr BigSet& operator=(BigSet&& other) {
            if (this == &other) return *this;
            this->mData = std::move(other.mData);
            other.mData = nullptr;

            Sentinel = other.Sentinel;
            mCurrentSize = other.mCurrentSize;
            return *this;
        }

        constexpr void SetSentinel(T newSentinel) {
            for (auto& s : *mData) {
                if (s == Sentinel) s = newSentinel;
            }
            Sentinel = newSentinel;
        }
        constexpr bool insert(const T& val) {
            auto slot = FindSlot(val);
            if (IsOccupied(slot)) return false; //either full, or already in set
            (*mData)[slot] = val;
            mCurrentSize++;
            return true;
        }

        constexpr void insert(const auto& begin, const auto& end) {
            for (auto it = begin; it != end; it++) {
                insert(*it);
            }
        }

        constexpr bool contains(const T& val) const {
            return IsOccupied(FindSlot(val));
        }

        constexpr void erase(const T& val) {
            auto slot = FindSlot(val);
            if (!IsOccupied(slot)) return;
            (*mData)[slot] = Sentinel;
            mCurrentSize--;
            for (size_t j = (slot + 1) % Capacity; j != slot; j = (j + 1) % Capacity) {
                if (!IsOccupied(j)) return;
                size_t k = FindSlot((*mData)[j]);
                if (slot <= j) {
                    if ((slot < k) && (k <= j)) {
                        continue;
                    }
                }
                else {
                    if ((slot < k) || (k <= j)) {
                        continue;
                    }
                }

                (*mData)[slot] = (*mData)[j];
                (*mData)[j] = Sentinel;
                slot = j;
            }
            return;
        }

        constexpr bool empty() const {
            return mCurrentSize == 0;
        }
        constexpr void clear() {
            mData->fill(Sentinel);
            mCurrentSize = 0;
        }
        constexpr std::size_t size() const {
            return mCurrentSize;
        }

        constexpr std::vector<T> GetValues() const {
            std::vector<T> result;
            std::copy_if(mData->begin(), mData->end(), std::back_inserter(result), [](const T& val) {
                return val != Sentinel;
                });
            return result;
        }

        //TODO: Yeah, these don't work
        /*
        constexpr auto begin() {
            return mData.begin();
        }
        constexpr auto begin() const {
            return mData.begin();
        }
        constexpr auto cbegin() const {
            return mData.cbegin();
        }
        constexpr auto end() {
            return mData.end();
        }
        constexpr auto end() const {
            return mData.end();
        }
        constexpr auto cend() const {
            return mData.cend();
        }
        */

    private:
        //T Sentinel { std::is_same_v<std::string, std::remove_cvref_t<T>> ? "SentinelString" : std::is_arithmetic_v<T> ? 9919 : {} };
        T Sentinel{};
        std::array<T, Capacity>* mData;
        size_t mCurrentSize = 0;
        Hasher mHash{};

        constexpr void MakeSentinel() {
            if constexpr (std::is_same_v<std::remove_cvref_t<T>, std::string>) {
                Sentinel = "SentinelString";
            }
            else if constexpr (std::is_arithmetic_v<T>) {
                Sentinel = 9919;
            }
            else {
                Sentinel = {}; //bleh
            }
        }
        

        constexpr size_t FindSlot(const T& t) const {
            if (mCurrentSize == Capacity) throw "SetFull";

            //size_t i = Constexpr::Hash(t) % Capacity;
            size_t i = mHash(t) % Capacity;
            while ((*mData)[i] != Sentinel && (*mData)[i] != t) {
                i = (i + 1) % Capacity;
            }
            return i;
        };

        constexpr bool IsOccupied(size_t slot) const {
            return (*mData)[slot] != Sentinel;
        }
    };

    template<typename T, size_t Capacity>
    constexpr inline auto begin(BigSet<T, Capacity>& set) {
        return set.begin();
    }
    template<typename T, size_t Capacity>
    constexpr inline auto cbegin(const BigSet<T, Capacity>& set) {
        return set.cbegin();
    }
    template<typename T, size_t Capacity>
    constexpr inline auto end(BigSet<T, Capacity>& set) {
        return set.end();
    }
    template<typename T, size_t Capacity>
    constexpr inline auto cend(const BigSet<T, Capacity>& set) {
        return set.cend();
    }

    template<typename T, size_t Capacity = 1024>
    struct Ring {
        constexpr void push_front(T val) {
            if (mHead == Unset) {
                mHead = 0;
                mTail = 0;
            }
            else {
                mHead = (mHead + Capacity - 1) % Capacity;
            }
            mData[mHead] = val;
        }

        constexpr void push_back(T val) {
            if (mHead == Unset) {
                mHead = 0;
                mTail = 0;
            }
            else {
                mTail = (mTail + 1) % Capacity;
            }
            mData[mTail] = val;
        }
        constexpr void pop_front() {
            if (mHead == mTail) {
                mHead = Unset;
                mTail = Unset;
            }
            else {
                mHead = (mHead + 1) % Capacity;
            }
        }
        constexpr void pop_back() {
            if (mHead == mTail) {
                mHead = Unset;
                mTail = Unset;
            }
            else {
                mTail = (mTail + Capacity - 1) % Capacity;
            }
        }
        constexpr T front() const {
            return mData[mHead];
        }
        constexpr T back() const {
            return mData[mTail];
        }

        constexpr bool is_empty() const {
            return mHead == Unset;
        }

        constexpr void rotate(int amount) {
            if (amount < 0) {
                for (auto i = 0; i < -amount; i++) {
                    push_front(back());
                    pop_back();
                }
            }
            else {
                for (auto i = 0; i < amount; i++) {
                    push_back(front());
                    pop_front();
                }
            }
        }

    private:
        size_t Unset = Capacity + 1;
        size_t mHead{ Unset };
        size_t mTail{ Unset };
        std::array<T, Capacity> mData{};
    };

    template<typename T>
    struct VecRing {
        constexpr VecRing(size_t capacity) {
            mData.resize(capacity);
            mCapacity = capacity;
        }

        constexpr void push_front(T val) {
            if (mHead == Unset) {
                mHead = 0;
                mTail = 0;
            }
            else {
                mHead = (mHead + mCapacity - 1) % mCapacity;
            }
            mData[mHead] = val;
        }

        constexpr void push_back(T val) {
            if (mHead == Unset) {
                mHead = 0;
                mTail = 0;
            }
            else {
                mTail = (mTail + 1) % mCapacity;
            }
            mData[mTail] = val;
        }
        constexpr void pop_front() {
            if (mHead == mTail) {
                mHead = Unset;
                mTail = Unset;
            }
            else {
                mHead = (mHead + 1) % mCapacity;
            }
        }
        constexpr void pop_back() {
            if (mHead == mTail) {
                mHead = Unset;
                mTail = Unset;
            }
            else {
                mTail = (mTail + mCapacity - 1) % mCapacity;
            }
        }
        constexpr T front() const {
            return mData[mHead];
        }
        constexpr T back() const {
            return mData[mTail];
        }

        constexpr bool is_empty() const {
            return mHead == Unset;
        }

        constexpr void rotate(int amount) {
            if (amount < 0) {
                for (auto i = 0; i < -amount; i++) {
                    push_front(back());
                    pop_back();
                }
            }
            else {
                for (auto i = 0; i < amount; i++) {
                    push_back(front());
                    pop_front();
                }
            }
        }

    private:
        size_t Unset = std::numeric_limits<size_t>::max();
        size_t mHead{ Unset };
        size_t mTail{ Unset };
        size_t mCapacity{ 0 };
        std::vector<T> mData;
    };

    bool RunCollectionTests();
}