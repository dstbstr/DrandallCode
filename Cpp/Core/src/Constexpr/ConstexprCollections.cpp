#include "Core/Constexpr/ConstexprCollections.h"

namespace Constexpr {
    namespace SmallMapTests {
        constexpr bool DefaultConstructor_CreatesEmptyMap() {
            SmallMap<int, int> map;
            return map.is_empty();
        }

        constexpr bool Constructor_WithMultipleElements_AddsAllElements() {
            SmallMap<int, int> map = { {1, 2}, {3, 4}, {5, 6} };
            return map.size() == 3;
        }

        constexpr bool IndexOperator_WithMissingValue_InsertsDefaultValue() {
            SmallMap<int, int> map;
            map[42] = 24;
            return !map.is_empty();
        }

        constexpr bool IndexOperator_WithValue_ReturnsMutableReference() {
            SmallMap<int, int> map;
            map[42] = 24;
            return map[42] == 24;
        }

        constexpr bool IndexOperator_WithNewValue_OverwritesOldValue() {
            SmallMap<int, int> map;
            map[42] = 24;
            map[42] = 42;

            if (map.size() != 1) return false;
            return map[42] == 42;
        }

        constexpr bool At_WithExistingElement_ReturnsValue() {
            SmallMap<int, int> map;
            map[1] = 2;
            return map.at(1) == 2;
        }

        constexpr bool Clear_OnMap_IsEmpty() {
            SmallMap<int, int> map;
            map[0] = 1;
            map.clear();

            return map.is_empty();
        }

        constexpr bool Erase_WithExistingValue_RemovesValue() {
            SmallMap<int, int> map;
            map[42] = 24;
            if (map.erase(42) != 1) return false;
            return map.is_empty();
        }

        constexpr bool Erase_WithMissingValue_ReturnsZero() {
            SmallMap<int, int> map;
            map[42] = 24;
            return map.erase(24) == 0;
        }

        constexpr bool RangeBasedFor_WithValues_TraversesAllValues() {
            SmallMap<int, int> map;
            map[1] = 1;
            map[2] = 2;
            map[3] = 3;

            size_t totalElements = 0;
            size_t sumOfKeys = 0;
            size_t sumOfValues = 0;
            for (auto [key, value] : map) {
                totalElements++;
                sumOfKeys += key;
                sumOfValues += value;
            }

            if (totalElements != 3) return false;
            if (sumOfKeys != 6) return false;
            if (sumOfValues != 6) return false;
            return true;
        }

        constexpr bool RangeBasedFor_WithReferences_TraversesAllValues() {
            SmallMap<int, int> map;
            map[1] = 1;
            map[2] = 2;
            map[3] = 3;

            size_t totalElements = 0;
            size_t sumOfKeys = 0;
            size_t sumOfValues = 0;
            for (const auto& [key, value] : map) {
                totalElements++;
                sumOfKeys += key;
                sumOfValues += value;
            }

            if (totalElements != 3) return false;
            if (sumOfKeys != 6) return false;
            if (sumOfValues != 6) return false;
            return true;

        }
    }

    namespace BigMapTests {
        constexpr bool DefaultConstructor_WithDefaultCapacity_DoesNotCrash() {
            BigMap<int, int> map;
            return map.is_empty();
        }
        constexpr bool DefaultConstructor_CreatesEmptyMap() {
            BigMap<int, int, 100> map;
            return map.is_empty();
        }

        constexpr bool Constructor_WithMultipleElements_AddsAllElements() {
            BigMap<int, int, 100> map = { {1, 2}, {3, 4}, {5, 6} };
            return map.size() == 3;
        }

        constexpr bool IndexOperator_WithMissingValue_InsertsDefaultValue() {
            BigMap<int, int, 100> map;
            return map[42] == 0;
        }

        constexpr bool IndexOperator_WithValue_ReturnsMutableReference() {
            BigMap<int, int, 100> map;
            map[42] = 24;
            return map[42] == 24;
        }

        constexpr bool IndexOperator_WithNewValue_OverwritesOldValue() {
            BigMap<int, int, 100> map;
            map[42] = 24;
            map[42] = 42;

            if (map.size() != 1) return false;
            return map[42] == 42;
        }

        constexpr bool At_WithExistingElement_ReturnsValue() {
            BigMap<int, int, 100> map;
            map[1] = 2;
            return map.at(1) == 2;
        }

        constexpr bool Clear_OnMap_IsEmpty() {
            BigMap<int, int, 100> map;
            map[0] = 1;
            map.clear();

            return map.is_empty();
        }

        constexpr bool Erase_WithExistingValue_RemovesValue() {
            BigMap<int, int, 100> map;
            map[42] = 24;
            if (map.erase(42) != 1) return false;
            return map.is_empty();
        }

        constexpr bool Erase_WithMissingValue_ReturnsZero() {
            BigMap<int, int, 100> map;
            map[42] = 24;
            return map.erase(24) == 0;
        }

        constexpr bool RangeBasedFor_WithValues_TraversesAllValues() {
            BigMap<int, int, 100> map;
            map[1] = 1;
            map[2] = 2;
            map[3] = 3;

            size_t totalElements = 0;
            size_t sumOfKeys = 0;
            size_t sumOfValues = 0;
            for (auto [key, value] : map.GetAllEntries()) {
                totalElements++;
                sumOfKeys += key;
                sumOfValues += value;
            }

            if (totalElements != 3) return false;
            if (sumOfKeys != 6) return false;
            if (sumOfValues != 6) return false;
            return true;
        }

        constexpr bool RangeBasedFor_WithReferences_TraversesAllValues() {
            BigMap<int, int, 100> map;
            map[1] = 1;
            map[2] = 2;
            map[3] = 3;

            size_t totalElements = 0;
            size_t sumOfKeys = 0;
            size_t sumOfValues = 0;
            for (const auto& [key, value] : map.GetAllEntries()) {
                totalElements++;
                sumOfKeys += key;
                sumOfValues += value;
            }

            if (totalElements != 3) return false;
            if (sumOfKeys != 6) return false;
            if (sumOfValues != 6) return false;
            return true;

        }
    }

    namespace StackTests {
        constexpr bool DefaultConstructor_CreatesValidStack() {
            Stack<int> defaultConstructor;
            return defaultConstructor.is_empty();
        }

        constexpr bool IsEmpty_OnEmptyStack_ReturnsTrue() {
            Stack<int> stack{};
            return stack.is_empty();
        }
        
        constexpr bool IsEmpty_OnNonEmptyStack_ReturnsFalse() {
            Stack<int> stack;
            stack.push(1);
            return !stack.is_empty();
        }

        constexpr bool Push_OnEmptyStack_HasSizeOne() {
            Stack<int> stack;
            stack.push(1);
            return stack.size() == 1;
        }

        constexpr bool Pop_WithNonEmptyStack_ReturnsTopElement() {
            Stack<int> stack;
            stack.push(1);
            return stack.pop() == 1;
        }

        constexpr bool Pop_WithMultipleElements_ReturnsMostRecentAdded() {
            Stack<int> stack;
            stack.push(1);
            stack.push(2);
            stack.push(3);

            if (stack.pop() != 3) return false;
            if (stack.pop() != 2) return false;
            if (stack.pop() != 1) return false;

            return stack.is_empty();
        }

        constexpr bool IsEmpty_AfterClearing_ReturnsTrue() {
            Stack<int> stack;
            stack.push(1);
            stack.push(2);
            stack.push(3);

            stack.clear();
            return stack.is_empty();
        }
    }

    namespace QueueTests {
        constexpr bool DefaultConstructor_CreatesValidQueue() {
            Queue<int> queue;
            return queue.is_empty();
        }

        constexpr bool IsEmpty_OnEmptyQueue_ReturnsTrue() {
            Queue<int> queue{};
            return queue.is_empty();
        }

        constexpr bool IsEmpty_OnNonEmptyQueue_ReturnsFalse() {
            Queue<int> queue;
            queue.push(1);
            return !queue.is_empty();
        }

        constexpr bool Push_OnEmptyQueue_HasSizeOne() {
            Queue<int> queue;
            queue.push(1);
            return queue.size() == 1;
        }

        constexpr bool Front_WithNonEmptyQueue_ReturnsFrontElement() {
            Queue<int> queue;
            queue.push(1);
            return queue.front() == 1;
        }

        constexpr bool Front_WithMultipleElements_ReturnsFirstAdded() {
            Queue<int> queue;
            queue.push(1);
            queue.push(2);
            queue.push(3);

            if (queue.front() != 1) return false;
            queue.pop();
            if (queue.front() != 2) return false;
            queue.pop();
            if (queue.front() != 3) return false;
            queue.pop();

            return queue.is_empty();
        }

        constexpr bool Front_WithElements_DoesNotRemoveElements() {
            Queue<int> queue;
            queue.push(1);
            queue.front();
            return !queue.is_empty();
        }

        constexpr bool IsEmpty_AfterClearing_ReturnsTrue() {
            Queue<int> queue;
            queue.push(1);
            queue.push(2);
            queue.push(3);

            queue.clear();
            return queue.is_empty();
        }
    }

    namespace RingTests {
        constexpr bool TestRing() {
            Ring<size_t> ring;
            if (!ring.is_empty()) return false;

            ring.push_back(5);
            ring.push_back(10);

            if (ring.back() != 10) return false;
            ring.pop_back();
            if (ring.back() != 5) return false;

            ring.push_front(15);
            if (ring.front() != 15) return false;
            ring.pop_front();
            if (ring.front() != ring.back()) return false;

            ring.pop_front();
            if (!ring.is_empty()) return false;

            return true;
        }

        constexpr bool TestVecRing() {
            VecRing<size_t> ring{1024};
            if (!ring.is_empty()) return false;

            ring.push_back(5);
            ring.push_back(10);

            if (ring.back() != 10) return false;
            ring.pop_back();
            if (ring.back() != 5) return false;

            ring.push_front(15);
            if (ring.front() != 15) return false;
            ring.pop_front();
            if (ring.front() != ring.back()) return false;

            ring.pop_front();
            if (!ring.is_empty()) return false;

            return true;
        }
    }

    namespace SmallSetTests {
        constexpr bool DefaultConstructor_CreatesEmptySet() {
            Constexpr::SmallSet<int> set;
            return set.empty();
        }

        constexpr bool InitializationListConstructor_CreatesSetWithElements() {
            Constexpr::SmallSet<int> set{1, 2, 3};
            return set.size() == 3;
        }

        constexpr bool Empty_WithElements_ReturnsFalse() {
            Constexpr::SmallSet<int> set{1, 2, 3};
            return !set.empty();
        }
        
        constexpr bool Empty_AfterClear_ReturnsTrue() {
            Constexpr::SmallSet<int> set{1, 2, 3};
            set.clear();
            return set.empty();
        }

        constexpr bool Insert_NewElement_ReturnsTrue() {
            Constexpr::SmallSet<int> set;
            return set.insert(42);
        }

        constexpr bool Insert_ExistingElement_ReturnsFalse() {
            Constexpr::SmallSet<int> set{42};
            return !set.insert(42);
        }

        constexpr bool InsertRange_InsertsNewElements() {
            Constexpr::SmallSet<int> set{42};
            std::vector<int> toInsert{1, 2, 2, 42};
            
            set.insert(toInsert.begin(), toInsert.end());
            return set.size() == 3; //1, 2, 42
        }

        constexpr bool Erase_ExistingElement_RemovesElement() {
            Constexpr::SmallSet<int> set{42};
            set.erase(42);
            return set.empty();
        }

        constexpr bool Erase_MissingElement_IsUnchanged() {
            Constexpr::SmallSet<int> set{42};
            set.erase(24);
            return !set.empty();
        }

        constexpr bool Contains_ExistingElement_ReturnsTrue() {
            Constexpr::SmallSet<int> set{42};
            return set.contains(42);
        }

        constexpr bool Contains_MissingElement_ReturnsFalse() {
            Constexpr::SmallSet<int> set{42};
            return !set.contains(24);
        }
    }

    namespace BigSetTests {
        constexpr bool DefaultConstructor_CreatesEmptySet() {
            Constexpr::BigSet<int, 100> set;
            return set.empty();
        }

        constexpr bool InitializationListConstructor_CreatesSetWithElements() {
            Constexpr::BigSet<int, 100> set{1, 2, 3};
            return set.size() == 3;
        }

        constexpr bool Empty_WithElements_ReturnsFalse() {
            Constexpr::BigSet<int, 100> set{1, 2, 3};
            return !set.empty();
        }

        constexpr bool Empty_AfterClear_ReturnsTrue() {
            Constexpr::BigSet<int, 100> set{1, 2, 3};
            set.clear();
            return set.empty();
        }

        constexpr bool Insert_NewElement_ReturnsTrue() {
            Constexpr::BigSet<int, 100> set;
            return set.insert(42);
        }

        constexpr bool Insert_ExistingElement_ReturnsFalse() {
            Constexpr::BigSet<int, 100> set{42};
            return !set.insert(42);
        }

        constexpr bool InsertRange_InsertsNewElements() {
            Constexpr::BigSet<int, 100> set{42};
            std::vector<int> toInsert{1, 2, 2, 42};

            set.insert(toInsert.begin(), toInsert.end());
            return set.size() == 3; //1, 2, 42
        }

        constexpr bool Erase_ExistingElement_RemovesElement() {
            Constexpr::BigSet<int, 100> set{42};
            set.erase(42);
            return set.empty();
        }

        constexpr bool Erase_MissingElement_IsUnchanged() {
            Constexpr::BigSet<int, 100> set{42};
            set.erase(24);
            return !set.empty();
        }

        constexpr bool Contains_ExistingElement_ReturnsTrue() {
            Constexpr::BigSet<int, 100> set{42};
            return set.contains(42);
        }

        constexpr bool Contains_MissingElement_ReturnsFalse() {
            Constexpr::BigSet<int, 100> set{42};
            return !set.contains(24);
        }
    }

    namespace PriorityQueueTests {
        constexpr bool Pop_AfterAdd_ReturnsValue() {
            Constexpr::PriorityQueue<int> q;
            q.push(12);
            return q.pop() == 12;
        }

        constexpr bool Empty_OnNewQueue_ReturnsTrue() {
            Constexpr::PriorityQueue<int> q;
            return q.empty();
        }

        constexpr bool Pop_AfterManyAdds_ReturnsValuesInDescendingOrder() {
            Constexpr::PriorityQueue<int> q;
            q.push(1);
            q.push(3);
            q.push(5);
            q.push(7);
            q.push(9);
            q.push(10);
            q.push(8);
            q.push(6);
            q.push(4);
            q.push(2);

            int prev = q.pop();
            while (!q.empty()) {
                auto next = q.pop();
                if (prev < next) return false;
                prev = next;
            }
            return true;
        }

        constexpr bool Queue_WithCustomType_UsesLessThanOperator() {
            struct V {
                int Value{ 0 };
                constexpr bool operator<(const V v) const {
                    return Value < v.Value;
                }            };

            PriorityQueue<V> q;
            q.push({ 3 });
            q.push({ 5 });
            q.push({ 7 });
            q.push({ 9 });

            if (q.pop().Value != 9) return false;
            if (q.pop().Value != 7) return false;
            if (q.pop().Value != 5) return false;
            if (q.pop().Value != 3) return false;
            return true;
        }
    }

    bool RunCollectionTests() {
        static_assert(SmallMapTests::At_WithExistingElement_ReturnsValue());
        static_assert(SmallMapTests::Clear_OnMap_IsEmpty());
        static_assert(SmallMapTests::Constructor_WithMultipleElements_AddsAllElements());
        static_assert(SmallMapTests::DefaultConstructor_CreatesEmptyMap());
        static_assert(SmallMapTests::Erase_WithExistingValue_RemovesValue());
        static_assert(SmallMapTests::Erase_WithMissingValue_ReturnsZero());
        static_assert(SmallMapTests::IndexOperator_WithMissingValue_InsertsDefaultValue());
        static_assert(SmallMapTests::IndexOperator_WithNewValue_OverwritesOldValue());
        static_assert(SmallMapTests::IndexOperator_WithValue_ReturnsMutableReference());

        if (!SmallMapTests::At_WithExistingElement_ReturnsValue()) return false;
        if (!SmallMapTests::Clear_OnMap_IsEmpty()) return false;
        if (!SmallMapTests::Constructor_WithMultipleElements_AddsAllElements()) return false;
        if (!SmallMapTests::DefaultConstructor_CreatesEmptyMap()) return false;
        if (!SmallMapTests::Erase_WithExistingValue_RemovesValue()) return false;
        if (!SmallMapTests::Erase_WithMissingValue_ReturnsZero()) return false;
        if (!SmallMapTests::IndexOperator_WithMissingValue_InsertsDefaultValue()) return false;
        if (!SmallMapTests::IndexOperator_WithNewValue_OverwritesOldValue()) return false;
        if (!SmallMapTests::IndexOperator_WithValue_ReturnsMutableReference()) return false;

        
        static_assert(BigMapTests::At_WithExistingElement_ReturnsValue());
        static_assert(BigMapTests::Clear_OnMap_IsEmpty());
        static_assert(BigMapTests::Constructor_WithMultipleElements_AddsAllElements());
        static_assert(BigMapTests::DefaultConstructor_CreatesEmptyMap());
        static_assert(BigMapTests::Erase_WithExistingValue_RemovesValue());
        static_assert(BigMapTests::Erase_WithMissingValue_ReturnsZero());
        static_assert(BigMapTests::IndexOperator_WithMissingValue_InsertsDefaultValue());
        static_assert(BigMapTests::IndexOperator_WithNewValue_OverwritesOldValue());
        static_assert(BigMapTests::IndexOperator_WithValue_ReturnsMutableReference());
        

        if (!BigMapTests::At_WithExistingElement_ReturnsValue()) return false;
        if (!BigMapTests::Clear_OnMap_IsEmpty()) return false;
        if (!BigMapTests::Constructor_WithMultipleElements_AddsAllElements()) return false;
        if (!BigMapTests::DefaultConstructor_CreatesEmptyMap()) return false;
        if (!BigMapTests::Erase_WithExistingValue_RemovesValue()) return false;
        if (!BigMapTests::Erase_WithMissingValue_ReturnsZero()) return false;
        if (!BigMapTests::IndexOperator_WithMissingValue_InsertsDefaultValue()) return false;
        if (!BigMapTests::IndexOperator_WithNewValue_OverwritesOldValue()) return false;
        if (!BigMapTests::IndexOperator_WithValue_ReturnsMutableReference()) return false;

        static_assert(StackTests::DefaultConstructor_CreatesValidStack());
        static_assert(StackTests::IsEmpty_AfterClearing_ReturnsTrue());
        static_assert(StackTests::IsEmpty_OnNonEmptyStack_ReturnsFalse());
        static_assert(StackTests::Push_OnEmptyStack_HasSizeOne());
        static_assert(StackTests::Pop_WithMultipleElements_ReturnsMostRecentAdded());
        static_assert(StackTests::Pop_WithNonEmptyStack_ReturnsTopElement());

        if (!StackTests::DefaultConstructor_CreatesValidStack()) return false;
        if (!StackTests::IsEmpty_AfterClearing_ReturnsTrue()) return false;
        if (!StackTests::IsEmpty_OnNonEmptyStack_ReturnsFalse()) return false;
        if (!StackTests::Push_OnEmptyStack_HasSizeOne()) return false;
        if (!StackTests::Pop_WithMultipleElements_ReturnsMostRecentAdded()) return false;
        if (!StackTests::Pop_WithNonEmptyStack_ReturnsTopElement()) return false;

        static_assert(QueueTests::DefaultConstructor_CreatesValidQueue());
        static_assert(QueueTests::Front_WithElements_DoesNotRemoveElements());
        static_assert(QueueTests::Front_WithMultipleElements_ReturnsFirstAdded());
        static_assert(QueueTests::Front_WithNonEmptyQueue_ReturnsFrontElement());
        static_assert(QueueTests::IsEmpty_AfterClearing_ReturnsTrue());
        static_assert(QueueTests::IsEmpty_OnEmptyQueue_ReturnsTrue());
        static_assert(QueueTests::IsEmpty_OnNonEmptyQueue_ReturnsFalse());
        static_assert(QueueTests::Push_OnEmptyQueue_HasSizeOne());

        if (!QueueTests::DefaultConstructor_CreatesValidQueue()) return false;
        if (!QueueTests::Front_WithElements_DoesNotRemoveElements()) return false;
        if (!QueueTests::Front_WithMultipleElements_ReturnsFirstAdded()) return false;
        if (!QueueTests::Front_WithNonEmptyQueue_ReturnsFrontElement()) return false;
        if (!QueueTests::IsEmpty_AfterClearing_ReturnsTrue()) return false;
        if (!QueueTests::IsEmpty_OnEmptyQueue_ReturnsTrue()) return false;
        if (!QueueTests::IsEmpty_OnNonEmptyQueue_ReturnsFalse()) return false;
        if (!QueueTests::Push_OnEmptyQueue_HasSizeOne()) return false;

        static_assert(RingTests::TestRing());
        static_assert(RingTests::TestVecRing());

        if (!RingTests::TestRing()) return false;
        if (!RingTests::TestVecRing()) return false;


        static_assert(SmallSetTests::DefaultConstructor_CreatesEmptySet());
        static_assert(SmallSetTests::InitializationListConstructor_CreatesSetWithElements());
        static_assert(SmallSetTests::Empty_WithElements_ReturnsFalse());
        static_assert(SmallSetTests::Empty_AfterClear_ReturnsTrue());
        static_assert(SmallSetTests::Insert_NewElement_ReturnsTrue());
        static_assert(SmallSetTests::Insert_ExistingElement_ReturnsFalse());
        //static_assert(SmallSetTests::InsertRange_InsertsNewElements());
        static_assert(SmallSetTests::Erase_ExistingElement_RemovesElement());
        static_assert(SmallSetTests::Erase_MissingElement_IsUnchanged());
        static_assert(SmallSetTests::Contains_ExistingElement_ReturnsTrue());
        static_assert(SmallSetTests::Contains_MissingElement_ReturnsFalse());

        if(!SmallSetTests::DefaultConstructor_CreatesEmptySet()) return false;
        if(!SmallSetTests::InitializationListConstructor_CreatesSetWithElements()) return false;
        if(!SmallSetTests::Empty_WithElements_ReturnsFalse()) return false;
        if(!SmallSetTests::Empty_AfterClear_ReturnsTrue()) return false;
        if(!SmallSetTests::Insert_NewElement_ReturnsTrue()) return false;
        if(!SmallSetTests::Insert_ExistingElement_ReturnsFalse()) return false;
        if(!SmallSetTests::InsertRange_InsertsNewElements()) return false;
        if(!SmallSetTests::Erase_ExistingElement_RemovesElement()) return false;
        if(!SmallSetTests::Erase_MissingElement_IsUnchanged()) return false;
        if(!SmallSetTests::Contains_ExistingElement_ReturnsTrue()) return false;
        if (!SmallSetTests::Contains_MissingElement_ReturnsFalse()) return false;

        static_assert(BigSetTests::DefaultConstructor_CreatesEmptySet());
        static_assert(BigSetTests::InitializationListConstructor_CreatesSetWithElements());
        static_assert(BigSetTests::Empty_WithElements_ReturnsFalse());
        static_assert(BigSetTests::Empty_AfterClear_ReturnsTrue());
        static_assert(BigSetTests::Insert_NewElement_ReturnsTrue());
        static_assert(BigSetTests::Insert_ExistingElement_ReturnsFalse());
        //static_assert(BigSetTests::InsertRange_InsertsNewElements());
        static_assert(BigSetTests::Erase_ExistingElement_RemovesElement());
        static_assert(BigSetTests::Erase_MissingElement_IsUnchanged());
        static_assert(BigSetTests::Contains_ExistingElement_ReturnsTrue());
        static_assert(BigSetTests::Contains_MissingElement_ReturnsFalse());

        if(!BigSetTests::DefaultConstructor_CreatesEmptySet()) return false;
        if(!BigSetTests::InitializationListConstructor_CreatesSetWithElements()) return false;
        if(!BigSetTests::Empty_WithElements_ReturnsFalse()) return false;
        if(!BigSetTests::Empty_AfterClear_ReturnsTrue()) return false;
        if(!BigSetTests::Insert_NewElement_ReturnsTrue()) return false;
        if(!BigSetTests::Insert_ExistingElement_ReturnsFalse()) return false;
        if(!BigSetTests::InsertRange_InsertsNewElements()) return false;
        if(!BigSetTests::Erase_ExistingElement_RemovesElement()) return false;
        if(!BigSetTests::Erase_MissingElement_IsUnchanged()) return false;
        if(!BigSetTests::Contains_ExistingElement_ReturnsTrue()) return false;
        if (!BigSetTests::Contains_MissingElement_ReturnsFalse()) return false;

        static_assert(PriorityQueueTests::Pop_AfterAdd_ReturnsValue());
        static_assert(PriorityQueueTests::Empty_OnNewQueue_ReturnsTrue());
        static_assert(PriorityQueueTests::Pop_AfterManyAdds_ReturnsValuesInDescendingOrder());
        static_assert(PriorityQueueTests::Queue_WithCustomType_UsesLessThanOperator());

        if (!PriorityQueueTests::Pop_AfterAdd_ReturnsValue()) return false;
        if (!PriorityQueueTests::Empty_OnNewQueue_ReturnsTrue()) return false;
        if (!PriorityQueueTests::Pop_AfterManyAdds_ReturnsValuesInDescendingOrder()) return false;
        if (!PriorityQueueTests::Queue_WithCustomType_UsesLessThanOperator()) return false;

        return true;
    }
}