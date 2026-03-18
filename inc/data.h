#include <string>
#include <string_view>

enum class Setting
{
    Window,
    Fonts,
    Circle,
    Rectangle,
    Unknown
};

std::string_view to_string(Setting s)
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

Setting to_setting(const std::string& s)
{
    if (s == "Circle") return Setting::Circle;
    if (s == "Fonts") return Setting::Fonts;
    if (s == "Rectangle") return Setting::Rectangle;
    if (s == "Window") return Setting::Window;
    return Setting::Unknown;
}