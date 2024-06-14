#pragma once

#include <array>
#include <queue>
#include <string>

enum class MsgPriorityID {
    Info,
    Warning,
    Error
};
constexpr int NumMsgPriorities = 3;
constexpr std::array<std::string, NumMsgPriorities> MsgPriorityNames{
    "INFO",
    "WARNING",
    "ERROR"
};


class ILogMsgPublisher {
public:
    virtual ~ILogMsgPublisher() = default;

    virtual void                                ClearMsgQueue() {
        while(!msgQueue.empty()) {
            msgQueue.pop();
        }
    }

    virtual void                                PublishMsg( std::string_view message,
                                                            MsgPriorityID priority = MsgPriorityID::Info) {
        msgQueue.push(std::make_pair(priority, std::string{message}));
    }

    virtual const std::queue<std::pair<MsgPriorityID, std::string>>&    GetMsgQueue() const {
        return msgQueue;
    }

private:
    std::queue<std::pair<MsgPriorityID, std::string>>       msgQueue;
};