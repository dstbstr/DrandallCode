#include "TestCommon.h"

#include "Core/DesignPatterns/PubSub.h"

struct PubSubTest : public testing::Test {
	PubSub<int> pubSub {};
};

TEST_F(PubSubTest, Publish_AfterSubscribe_NotifiesSubscriber) {
	int capturedValue{ 0 };
	pubSub.Subscribe([&](const int& event) { capturedValue = event; });
	pubSub.Publish(42);

	ASSERT_EQ(42, capturedValue);
}

TEST_F(PubSubTest, Publish_WithMultipleSubscribers_NotifiesAll) {
	int capturedValue1{ 0 };
	pubSub.Subscribe([&](const int& event) { capturedValue1 = event; });

	int capturedValue2{ 0 };
	pubSub.Subscribe([&](const int& event) { capturedValue2 = event; });

	pubSub.Publish(42);

	ASSERT_EQ(42, capturedValue1);
	ASSERT_EQ(42, capturedValue2);
}

TEST_F(PubSubTest, Publish_AfterSubscribeThenUnsubscribe_DoesNotNotify) {
	int capturedValue{ 0 };
	auto onEvent = [&](const int& event) { capturedValue = event; FAIL(); };
	
	auto handle = pubSub.Subscribe(onEvent);
	pubSub.Unsubscribe(handle);
	pubSub.Publish(42);

	ASSERT_EQ(0, capturedValue);
}
