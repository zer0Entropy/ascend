#pragma once

#include <vector>
#include "../interface/attachment.hpp"
#include "../interface/publisher.hpp"
#include "../interface/system.hpp"

class TextFile;

class LogSystem: public ISystem, public ICanHaveAttachments {
public:
    LogSystem();
    ~LogSystem() = default;
    LogSystem(const LogSystem& copy) = delete;
    LogSystem(LogSystem&& move) = delete;
    LogSystem& operator=(const LogSystem& copy) = delete;
    LogSystem& operator=(LogSystem&& move) = delete;

    void                                Attach(Resource* resource) override;
    void                                Detatch(const ResourceID& resourceID) override;

    void                                Update() override;
    ISystem::SystemID                   GetSystemID() const override;

    void                                Subscribe(ILogMsgPublisher* publisher);
    void                                Unsubscribe(ILogMsgPublisher* publisher);

private:
    std::vector<ILogMsgPublisher*>      publishers;
    TextFile*                           log;
};