#pragma once

#include <vector>
#include "../resource/resource.hpp"

class ICanHaveAttachments {
public:
    virtual ~ICanHaveAttachments() = default;

    virtual void                            Attach(Resource* resource) {attachments.push_back(resource);}
    
    virtual void                            Detatch(const ResourceID& resourceID) {
        for(auto iter = attachments.begin(); iter < attachments.end(); ++iter) {
            if((*iter)->GetID().compare(resourceID) == 0) {
                attachments.erase(iter);
            }
        }
    }

    virtual const std::vector<Resource*>&   GetAttachments() const {return attachments;}

private:
    std::vector<Resource*>                  attachments;
};