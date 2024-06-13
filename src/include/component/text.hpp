#pragma once

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Color.hpp>
#include "../core/component.hpp"
#include "../interface/attachment.hpp"

class Text: public Component, public ICanHaveAttachments {
public:
    Text(Entity cEntity);

    sf::Text&                       GetText() const;

    void                            LoadFromJSON(const nlohmann::json& json) override;
    
    nlohmann::json                  SaveToJSON() const override;

    void                            Attach(Resource* resource) override;

    void                            Detatch(const ResourceID& resourceID) override;

    struct FontParameters{
        ResourceID                  fontID;
        unsigned int                fontSize;
        sf::Color                   outlineColor;
        sf::Color                   fillColor;
    };
    const FontParameters&           GetFontParameters() const;
    void                            SetFontParameters(const FontParameters& param);

    const std::string&              GetContents() const;
    void                            SetContents(std::string_view txtContents);

private:
    sf::Text                        text;
    FontParameters                  fontParam;
    std::string                     contents;
};

class TextMgr {
public:
    void                                                    Add(Entity owner);

    void                                                    Remove(Entity owner);

    Text*                                                   Get(Entity owner);

    std::vector<Text*>                                      GetList() const;

private:
    std::unordered_map<Entity, std::unique_ptr<Text>>       textMap;
};