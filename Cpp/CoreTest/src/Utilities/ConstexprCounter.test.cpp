#include "Utilities/ConstexprCounter.h"

#include "TestCommon.h"


#if !_HAS_CXX20
struct SomeStruct {};
static_assert(ConstexprCounter::Count<SomeStruct>() == 0, "Counter has not been called yet");
static_assert(ConstexprCounter::Next<SomeStruct>() != ConstexprCounter::Next<SomeStruct>(), "Multiple calls to ConstexprCounterNext should provide unique values");
static_assert(ConstexprCounter::Count<SomeStruct>() == 2, "Counter has been called twice");

struct StructOne {};
struct StructTwo {};
static_assert(ConstexprCounter::Next<StructOne>() == ConstexprCounter::Next<StructTwo>(), "Counter resets by type");
#endif