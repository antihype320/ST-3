// Copyright 2022 UNN

#include "TimedDoor.h"
#include <thread> // NOLINT [build/c++11]
#include <chrono> // NOLINT [build/c++11]
#include <stdexcept>

DoorTimerAdapter::DoorTimerAdapter(TimedDoor& door) : door(door) {}

void DoorTimerAdapter::Timeout() {
    if (door.isDoorOpened()) {
        throw std::runtime_error("The door has been open too long");
    }
}

TimedDoor::TimedDoor(int timeout) : adapter(new DoorTimerAdapter(*this)),
                                    iTimeout(timeout), isOpened(false) {}

bool TimedDoor::isDoorOpened() {
    return isOpened;
}

void TimedDoor::unlock() {
    if (isOpened)
        throw std::logic_error("Door is already opened");
    isOpened = true;
}

void TimedDoor::lock() {
    if (!isOpened)
        throw std::logic_error("Door is already closed");
    isOpened = false;
}

int TimedDoor::getTimeOut() const {
    return iTimeout;
}

void TimedDoor::throwState() {
    if (isOpened) {
        throw std::runtime_error("The door is currently open");
    } else {
        throw std::runtime_error("The door is currently closed");
    }
}

void Timer::tregister(int timeout, TimerClient* client) {
    if (client == nullptr) {
        throw std::invalid_argument("Timer client cannot be null");
    }
    this->client = client;
    sleep(timeout);
    client->Timeout();
}

void Timer::sleep(int timeout) {
    std::this_thread::sleep_for(std::chrono::seconds(timeout));
}
