#include "DesignPatterns/BaseEvent.h"
#include "DesignPatterns/PubSub.h"
#include "TestCommon.h"

#include <algorithm>
#include <random>
#include <vector>

class TestPublisher;

class TestEvent : BaseEvent<TestEvent> {
public:
    enum EventType { TEST_ONE };

    explicit TestEvent(EventType type) : eventType(type) {}

private:
    EventType eventType;
};

class TestSubscriber : public ISubscriber<TestEvent> {
public:
    void Notify(const TestEvent&) override {
        count++;
    }

    int GetCount() {
        return count;
    }

private:
    int count{0};
};

class TestPublisher : public IPublisher<TestEvent> {
public:
    bool HasSubscriber() {
        return HasSubscribers();
    }
};

class PublisherHarness : public ::testing::Test {
protected:
    TestPublisher* publisher{nullptr};

    void SetUp() override {
        publisher = new TestPublisher();
    }

    void TearDown() override {
        delete publisher;
    }
};

TEST_F(PublisherHarness, Subscriber_Initialized) {
    TestSubscriber subscriber;

    ASSERT_EQ(subscriber.GetCount(), 0);
}

TEST_F(PublisherHarness, Publisher_CanAddSubscriber) {
    TestSubscriber subscriber;
    ASSERT_TRUE(publisher->HasSubscriber());
}

TEST_F(PublisherHarness, Publisher_CanRemoveSubscriber) {
    const TestSubscriber* subscriber = new TestSubscriber();

    ASSERT_TRUE(publisher->HasSubscriber());

    delete subscriber;

    ASSERT_FALSE(publisher->HasSubscriber());
}

TEST_F(PublisherHarness, Publisher_CanRemoveInRandomOrder) {
    std::vector<int> indexes;
    std::vector<TestSubscriber*> subscribers;

    for (int i = 0; i < 10; i++) {
        subscribers.push_back(new TestSubscriber());
        indexes.push_back(i);
    }

    std::shuffle(indexes.begin(), indexes.end(), std::default_random_engine());

    for (auto index : indexes) {
        delete subscribers[index];
    }

    ASSERT_FALSE(publisher->HasSubscriber());
}

TEST_F(PublisherHarness, Publisher_CanNotify) {
    std::vector<TestSubscriber*> subscribers;
    for (int i = 0; i < 5; i++) {
        subscribers.push_back(new TestSubscriber());
    }

    publisher->RaiseEvent(TestEvent(TestEvent::TEST_ONE));

    for (auto subscriber : subscribers) {
        ASSERT_EQ(1, subscriber->GetCount());
    }
}

TEST(IPublisher, MultiplePublishers_AreAddedToAllSubscribers) {
    auto publisher1 = std::make_unique<TestPublisher>();
    auto subscriber1 = std::make_unique<TestSubscriber>();
    auto subscriber2 = std::make_unique<TestSubscriber>();
    auto publisher2 = std::make_unique<TestPublisher>();

    publisher1->RaiseEvent(TestEvent(TestEvent::TEST_ONE));

    ASSERT_EQ(1, subscriber1->GetCount());
    ASSERT_EQ(1, subscriber2->GetCount());

    publisher2->RaiseEvent(TestEvent(TestEvent::TEST_ONE));

    ASSERT_EQ(2, subscriber1->GetCount());
    ASSERT_EQ(2, subscriber2->GetCount());
}

TEST(IPublisher, Subscribers_CanBeAddedBeforePublishers) {
    auto subscriber = std::make_unique<TestSubscriber>();
    auto publisher = std::make_unique<TestPublisher>();

    publisher->RaiseEvent(TestEvent(TestEvent::TEST_ONE));

    ASSERT_EQ(1, subscriber->GetCount());
}

class TestPubAndSub : public ISubscriber<TestEvent>, public IPublisher<TestEvent> {
private:
    int count{0};

public:
    void Notify(const TestEvent&) override {
        count++;
    }

    int GetCount() const {
        return count;
    }
};

TEST(IPublisher, PublisherCanSubscribeToSameEvents) {
    auto pubSub = std::make_unique<TestPubAndSub>();

    pubSub->RaiseEvent(TestEvent(TestEvent::TEST_ONE));

    ASSERT_EQ(1, pubSub->GetCount());
}
