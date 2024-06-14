#pragma once

#include <queue>
#include <string>

class ILogMsgPublisher {
public:
    virtual ~ILogMsgPublisher() = default;

    virtual void                                    ClearMsgQueue() {
        while(!msgQueue.empty()) {
            msgQueue.pop();
        }
    }

    virtual void                                    PublishMsg(std::string_view message) {
        msgQueue.push(std::string{message});
    }

    virtual const std::queue<std::string>&          GetMsgQueue() const {
        return msgQueue;
    }

private:
    std::queue<std::string>                         msgQueue;
};