#include "../include/core/log.hpp"
#include "../include/resource/textFile.hpp"

LogSystem::LogSystem():
    ISystem{},
    ICanHaveAttachments{} {

}

void LogSystem::Update() {
    TextFile* textFile{nullptr};
    const auto& attachmentList{this->GetAttachments()};
    if(attachmentList.empty()) {
        return;
    }
    else {
        Resource* attachedResource{*attachmentList.begin()};
        if(attachedResource->GetTypeID() == Resource::TypeID::TextFile) {
            textFile = dynamic_cast<TextFile*>(attachedResource);
        }
    }
    
    if(textFile) {
        for(const auto& publisher : publishers) {
            auto queue{publisher->GetMsgQueue()};
            publisher->ClearMsgQueue();
            while(!queue.empty()) {
                textFile->Append(queue.front());
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
