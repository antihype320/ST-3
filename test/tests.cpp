// Copyright 2022 UNN

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cstdint>
#include <thread> // NOLINT [build/c++11]
#include <chrono> // NOLINT [build/c++11]

#include "TimedDoor.h"

class MockTimerClient : public TimerClient {
 public:
    MOCK_METHOD(void, Timeout, (), (override));
};

class TimedDoorTest : public ::testing::Test {
 protected:
    void SetUp() override {
        timerClient = new MockTimerClient();
    }

    void TearDown() override {
        delete timerClient;
    }

    TimedDoor door;
    Timer timer;
    MockTimerClient *timerClient{};

 public:
    TimedDoorTest() : door(5), timer() {}
};

TEST_F(TimedDoorTest, DoorStartsClosed) {
    EXPECT_FALSE(door.isDoorOpened());
}

TEST_F(TimedDoorTest, LockAlreadyOpenedDoor) {
    EXPECT_THROW(door.lock(), std::logic_error);
}

TEST_F(TimedDoorTest, DoorUnclock) {
    door.unlock();
    EXPECT_TRUE(door.isDoorOpened());
}

TEST_F(TimedDoorTest, DoorOpensAndCloses) {
    door.unlock();
    EXPECT_TRUE(door.isDoorOpened());
    door.lock();
    EXPECT_FALSE(door.isDoorOpened());
}

TEST_F(TimedDoorTest, UnlockTwiceThrowsException) {
    door.unlock();
    EXPECT_THROW(door.unlock(), std::logic_error);
}

TEST_F(TimedDoorTest, LockTwiceThrowsException) {
    door.unlock();
    door.lock();
    EXPECT_THROW(door.lock(), std::logic_error);
}

TEST_F(TimedDoorTest, LockWhileClosedThrowsException) {
    timer.tregister(door.getTimeOut(), timerClient);
    EXPECT_THROW(door.lock(), std::logic_error);
}

TEST_F(TimedDoorTest, DoorRemainsClosedAfterLockBeforeTimeout) {
    door.unlock();
    timer.tregister(door.getTimeOut(), timerClient);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    door.lock();
    EXPECT_FALSE(door.isDoorOpened());
}

TEST_F(TimedDoorTest, NoExceptionThrownAfterLockBeforeTimeout) {
    door.unlock();
    timer.tregister(door.getTimeOut(), timerClient);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    EXPECT_NO_THROW(door.lock());
}

TEST_F(TimedDoorTest, ThrowStateWhenDoorIsOpened) {
    door.unlock();
    EXPECT_THROW(door.throwState(), std::runtime_error);
}
