#include <functional>
#include <memory>
#include <string>
#include <string_view>

#include <SFML/Graphics.hpp>

enum class Setting
{
    Window,
    Fonts,
    Circle,
    Rectangle,
    Unknown
};

inline std::string_view to_string(Setting s)
{
    switch(s)
    {
    case Setting::Circle: return "Circle";
    case Setting::Rectangle: return "Rectangle";
    case Setting::Fonts: return "Fonts";
    case Setting::Window: return "Window";
    }
    return {};
}

inline Setting to_setting(const std::string& s)
{
    if (s == "Circle") return Setting::Circle;
    if (s == "Fonts") return Setting::Fonts;
    if (s == "Rectangle") return Setting::Rectangle;
    if (s == "Window") return Setting::Window;
    return Setting::Unknown;
}

struct ShapeData
{
    template <typename TShape>
    ShapeData(TShape&& s, float sx, float sy, Setting setting) : velocity{sx, sy}
    {
        shape = std::make_unique<TShape>(std::move(s));
        if (setting == Setting::Circle){
            auto d = dynamic_cast<sf::CircleShape*>(shape.get())->getRadius() * 2;
            get_size = [d]() { return sf::Vector2f{d, d}; };
        } else if (setting == Setting::Rectangle) {
            get_size = [sz = dynamic_cast<sf::RectangleShape*>(shape.get())->getSize()]() { return sz; };
        }
    }

    inline sf::Vector2f get_pos()
    {
        return shape->getPosition(); 
    }

    // update pos really
    inline void set_pos()
    {
        auto pos = get_pos();
        shape->setPosition({pos.x + velocity[0], pos.y + velocity[1]});
    }

    inline void set_color(float c[3])
    {
        shape->setFillColor(sf::Color(uint8_t(c[0]*255), uint8_t(c[1]*255), uint8_t(c[2]*255)));
    }

    std::function<sf::Vector2f()> get_size = []() -> sf::Vector2f
    {
        throw "get_size not implemented.";
    };
    std::unique_ptr<sf::Shape> shape;
    float velocity[2]; // {x,y}
    // float color[3];
    bool draw{true};
};

struct TextData
{
    TextData(sf::Text&& t) : text(std::move(t)), draw{true} {}
    // TextData(TextData&& t)
    // {
    //     this->text = std::move(t.text);
    //     this->draw = t.draw;
    // }
    inline std::string value()
    {
        return text.getString();
    }
    sf::Text text;
    // char displayString[255];
    bool draw;
};