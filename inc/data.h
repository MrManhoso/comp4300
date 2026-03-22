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
    ShapeData(const std::string& n, TShape&& s, float sx, float sy, Setting setting) :
        name(n), speed_x(sx), speed_y(sy) 
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

    std::function<sf::Vector2f()> get_size = []() -> sf::Vector2f
    {
        throw "get_size not implemented.";
    };
    
    std::string name;
    std::unique_ptr<sf::Shape> shape;
    float speed_x;
    float speed_y;
    // float color[3];
    bool draw{true};
};

struct TextData
{
    TextData(sf::Text&& t) : text(std::move(t)), draw{true} {}
    
    sf::Text text;
    // char displayString[255];
    bool draw;
};