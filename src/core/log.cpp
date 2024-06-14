#include "../include/core/log.hpp"
#include "../include/resource/textFile.hpp"

LogSystem::LogSystem():
    ISystem{},
    ICanHaveAttachments{},
    log{nullptr} {

}

void LogSystem::Attach(Resource* resource) {
    if(GetAttachments().size() == 0)
    {
        if(resource->GetTypeID() == Resource::TypeID::TextFile) {
            log = dynamic_cast<TextFile*>(resource);
            ICanHaveAttachments::Attach(log);
        }
    }
}

void LogSystem::Detatch(const ResourceID& resourceID) {
    if(log && resourceID == log->GetID()) {
        ICanHaveAttachments::Detatch(resourceID);
        log = nullptr;
    }   
}

void LogSystem::Update() {
    if(log) {
        for(const auto& publisher : publishers) {
            auto queue{publisher->GetMsgQueue()};
            publisher->ClearMsgQueue();
            while(!queue.empty()) {
                log->Append(queue.front());
                queue.pop();
            }
        }
    }
}

ISystem::SystemID LogSystem::GetSystemID() const {
    return ISystem::SystemID::LogSystem;
}

void LogSystem::Subscribe(ILogMsgPublisher* publisher) {
    publishers.push_back(publisher);
}

void LogSystem::Unsubscribe(ILogMsgPublisher* publisher) {
    for(auto iter = publishers.begin(); iter != publishers.end(); ++iter) {
        if((*iter) == publisher) {
            publishers.erase(iter);
        }
    }
}
