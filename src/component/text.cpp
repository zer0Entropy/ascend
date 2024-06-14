#include "../include/component/text.hpp"
#include "../include/resource/font.hpp"
#include "../include/core/app.hpp"

Text::Text(Entity cEntity):
    Component{Component::TypeID::Text, cEntity},
    ICanHaveAttachments{},
    fontParam{
        "",
        0,
        sf::Color::Black,
        sf::Color::White
    } {

}

sf::Text& Text::GetText() const {
    return const_cast<sf::Text&>(text);
}

void Text::LoadFromJSON(const nlohmann::json& json) {
    ResourceMgr& resourceMgr{Application::GetInstance().GetResourceMgr()};
    
    const auto& findContents{json.find("contents")};
    if(findContents != json.end()) {
        std::string contentsFound{findContents.value().template get<std::string>()};
        SetContents(contentsFound);
    }

    const auto& findFontParam{json.find("fontParameters")};
    if(findFontParam != json.end()) {
        const auto& fontParamJSON{findFontParam.value()};
        const auto& findID{fontParamJSON.find("id")};
        const auto& findSize{fontParamJSON.find("fontSize")};
        const auto& findOutlineColor{fontParamJSON.find("outlineColor")};
        const auto& findFillColor{fontParamJSON.find("fillColor")};
        FontParameters fontParamFound{"", 0, sf::Color::Black, sf::Color::White};
        if(findID != fontParamJSON.end()) {
            findID.value().get_to(fontParamFound.fontID);
        }
        if(findSize != fontParamJSON.end()) {
            fontParamFound.fontSize = findSize.value();
        }
        if(findOutlineColor != fontParamJSON.end()) {
            std::string hexString{findOutlineColor.value().template get<std::string>()};
            fontParamFound.outlineColor = GetColorFromHex(hexString);
        }
        if(findFillColor != fontParamJSON.end()) {
            std::string hexString{findFillColor.value().template get<std::string>()};
            fontParamFound.fillColor = GetColorFromHex(hexString);
        }
        SetFontParameters(fontParamFound);
    }

    const auto& findAttachments{json.find("attachments")};
    if(findAttachments != json.end()) {
        const auto& attachmentsJSON{findAttachments.value()};
        const auto& findFonts{attachmentsJSON.find("fonts")};
        if(findFonts != attachmentsJSON.end()) {
            for(auto fontIter : findFonts.value()) {
                ResourceID  fontID{fontIter.template get<std::string>()};
                Font*       font{resourceMgr.GetFont(fontID)};
                if(font) {
                    this->Attach(font);
                }
            }
        }
    }
}

nlohmann::json Text::SaveToJSON() const {
    const nlohmann::json            contentsJSON{
        {"contents", {
            contents
        } }
    };
    const nlohmann::json            fontParamJSON{
        {"fontParameters", {
            "id", fontParam.fontID,
            "fontSize", std::to_string(fontParam.fontSize),
            "outlineColor", std::to_string(fontParam.outlineColor.toInteger()),
            "fillColor", std::to_string(fontParam.fillColor.toInteger())
        } }
    };
    nlohmann::json                  attachmentJSON;
    
    const nlohmann::json            output{
        {fontParamJSON},
        {"attachments",
            attachmentJSON}
    };
    return                      output;
}

void Text::Attach(Resource* resource) {
    const auto& attached{this->GetAttachments()};
    if(     attached.size() == 0
        &&  resource->GetTypeID() == Resource::TypeID::Font) {
        Font* font = dynamic_cast<Font*>(resource);
        if(font) {
            text.setFont(font->GetFont());
            ICanHaveAttachments::Attach(font);
        }
    }
}

void Text::Detatch(const ResourceID& resourceID) {
    ICanHaveAttachments::Detatch(resourceID);
}

const Text::FontParameters& Text::GetFontParameters() const {
    return fontParam;
}

void Text::SetFontParameters(const Text::FontParameters& param) {
    fontParam = param;
    Font* font{Application::GetInstance().GetResourceMgr().GetFont(fontParam.fontID)};
    if(font) {
        text.setFont(font->GetFont());
        text.setCharacterSize(fontParam.fontSize);
        text.setOutlineColor(fontParam.outlineColor);
        if(fontParam.fillColor == sf::Color::White) {
            text.setFillColor(fontParam.fillColor);
        }
        else {
            text.setFillColor(sf::Color::Red);
        }
        
    }
}

const std::string& Text::GetContents() const {
    return contents;
}

void Text::SetContents(std::string_view txtContents) {
    contents = txtContents;
    text.setString(contents);
}

void TextMgr::Add(Entity owner) {
    textMap.insert(std::make_pair(owner, std::make_unique<Text>(owner)));
}

void TextMgr::Remove(Entity owner) {
    const auto& iter{textMap.find(owner)};
    if(iter != textMap.end()) {
        textMap.erase(iter);
    }
}

Text* TextMgr::Get(Entity owner) {
    const auto& iter{textMap.find(owner)};
    if(iter != textMap.end()) {
        return iter->second.get();
    }
    return nullptr;
}

std::vector<Text*> TextMgr::GetList() const {
    std::vector<Text*> textList;
    for(const auto& text : textMap) {
        textList.push_back(text.second.get());
    }
    return textList;
}