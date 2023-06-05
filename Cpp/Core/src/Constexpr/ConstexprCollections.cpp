#include "Constexpr/ConstexprCollections.h"

namespace Constexpr {
    namespace SmallMapTests {
        constexpr bool DefaultConstructor_Works() {
            SmallMap<int, int> map;
            return map.is_empty();
        }
        static_assert(DefaultConstructor_Works());

        constexpr bool Constructor_WithMultipleElements_AddsAllElements() {
            SmallMap<int, int> map = { {1, 2}, {3, 4}, {5, 6} };
            return map.size() == 3;
        }
        static_assert(Constructor_WithMultipleElements_AddsAllElements());

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
        static_assert(IndexOperator_WithValue_ReturnsMutableReference());

        constexpr bool IndexOperator_WithNewValue_OverwritesOldValue() {
            SmallMap<int, int> map;
            map[42] = 24;
            map[42] = 42;

            if (map.size() != 1) return false;
            return map[42] == 42;
        }
        static_assert(IndexOperator_WithNewValue_OverwritesOldValue);

        constexpr bool At_WithExistingElement_ReturnsValue() {
            SmallMap<int, int> map;
            map[1] = 2;
            return map.at(1) == 2;
        }
        static_assert(At_WithExistingElement_ReturnsValue());

        constexpr bool Clear_OnMap_IsEmpty() {
            SmallMap<int, int> map;
            map[0] = 1;
            map.clear();

            return map.is_empty();
        }
        static_assert(Clear_OnMap_IsEmpty());

        constexpr bool Erase_WithExistingValue_RemovesValue() {
            SmallMap<int, int> map;
            map[42] = 24;
            if (map.erase(42) != 1) return false;
            return map.is_empty();
        }
        static_assert(Erase_WithExistingValue_RemovesValue());

        constexpr bool Erase_WithMissingValue_ReturnsZero() {
            SmallMap<int, int> map;
            map[42] = 24;
            return map.erase(24) == 0;
        }
        static_assert(Erase_WithMissingValue_ReturnsZero());

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
        static_assert(RangeBasedFor_WithValues_TraversesAllValues());
    }

    namespace ConstexprStackTests {
        constexpr bool DefaultConstructor_CreatesValidStack() {
            Stack<int> defaultConstructor;
            return defaultConstructor.is_empty();
        }
        static_assert(DefaultConstructor_CreatesValidStack());

        constexpr bool IsEmpty_OnEmptyStack_ReturnsTrue() {
            Stack<int> stack{};
            return stack.is_empty();
        }
        static_assert(IsEmpty_OnEmptyStack_ReturnsTrue());
        
        constexpr bool IsEmpty_OnNonEmptyStack_ReturnsFalse() {
            Stack<int> stack;
            stack.push(1);
            return !stack.is_empty();
        }
        static_assert(IsEmpty_OnNonEmptyStack_ReturnsFalse());

        constexpr bool Push_OnEmptyStack_HasSizeOne() {
            Stack<int> stack;
            stack.push(1);
            return stack.size() == 1;
        }
        static_assert(Push_OnEmptyStack_HasSizeOne());

        constexpr bool Top_WithNonEmptyStack_ReturnsTopElement() {
            Stack<int> stack;
            stack.push(1);
            return stack.top() == 1;
        }
        static_assert(Top_WithNonEmptyStack_ReturnsTopElement());

        constexpr bool Top_WithMultipleElements_ReturnsMostRecentAdded() {
            Stack<int> stack;
            stack.push(1);
            stack.push(2);
            stack.push(3);

            if (stack.top() != 3) return false;
            stack.pop();
            if (stack.top() != 2) return false;
            stack.pop();
            if (stack.top() != 1) return false;
            stack.pop();

            return stack.is_empty();
        }
        static_assert(Top_WithMultipleElements_ReturnsMostRecentAdded());

        constexpr bool Top_WithElements_DoesNotRemoveTopElement() {
            Stack<int> stack;
            stack.push(1);
            stack.top();
            return !stack.is_empty();
        }
        static_assert(Top_WithElements_DoesNotRemoveTopElement());

        constexpr bool IsEmpty_AfterClearing_ReturnsTrue() {
            Stack<int> stack;
            stack.push(1);
            stack.push(2);
            stack.push(3);

            stack.clear();
            return stack.is_empty();
        }
        static_assert(IsEmpty_AfterClearing_ReturnsTrue());
    }

    namespace ConstexprQueueTests {
        constexpr bool DefaultConstructor_CreatesValidQueue() {
            Queue<int> queue;
            return queue.is_empty();
        }
        static_assert(DefaultConstructor_CreatesValidQueue());

        constexpr bool IsEmpty_OnEmptyQueue_ReturnsTrue() {
            Queue<int> queue{};
            return queue.is_empty();
        }
        static_assert(IsEmpty_OnEmptyQueue_ReturnsTrue());

        constexpr bool IsEmpty_OnNonEmptyQueue_ReturnsFalse() {
            Queue<int> queue;
            queue.push(1);
            return !queue.is_empty();
        }
        static_assert(IsEmpty_OnNonEmptyQueue_ReturnsFalse());

        constexpr bool Push_OnEmptyQueue_HasSizeOne() {
            Queue<int> queue;
            queue.push(1);
            return queue.size() == 1;
        }
        static_assert(Push_OnEmptyQueue_HasSizeOne());

        constexpr bool Front_WithNonEmptyQueue_ReturnsFrontElement() {
            Queue<int> queue;
            queue.push(1);
            return queue.front() == 1;
        }
        static_assert(Front_WithNonEmptyQueue_ReturnsFrontElement());

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
        static_assert(Front_WithMultipleElements_ReturnsFirstAdded());

        constexpr bool Front_WithElements_DoesNotRemoveElements() {
            Queue<int> queue;
            queue.push(1);
            queue.front();
            return !queue.is_empty();
        }
        static_assert(Front_WithElements_DoesNotRemoveElements());

        constexpr bool IsEmpty_AfterClearing_ReturnsTrue() {
            Queue<int> queue;
            queue.push(1);
            queue.push(2);
            queue.push(3);

            queue.clear();
            return queue.is_empty();
        }
        static_assert(IsEmpty_AfterClearing_ReturnsTrue());
    }
}