#pragma once

#include <vector>
#include "../interface/attachment.hpp"
#include "../interface/publisher.hpp"
#include "../interface/system.hpp"

class LogSystem: public ISystem, public ICanHaveAttachments {
public:
    LogSystem();
    ~LogSystem() = default;
    LogSystem(const LogSystem& copy) = delete;
    LogSystem(LogSystem&& move) = delete;
    LogSystem& operator=(const LogSystem& copy) = delete;
    LogSystem& operator=(LogSystem&& move) = delete;

    void                                Update();
    ISystem::SystemID                   GetSystemID() const;

    void                                Subscribe(ILogMsgPublisher* publisher);
    void                                Unsubscribe(ILogMsgPublisher* publisher);

private:
    std::vector<ILogMsgPublisher*>      publishers;
};