#include <iostream>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

std::vector<std::string> split(std::string s, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        tokens.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    tokens.push_back(s);

    return tokens;
}

inline void update_circle(std::size_t segments, float radius, const float* cols, float speed_x, float speed_y, sf::CircleShape& circle)
{
    circle.setPointCount(segments);
    circle.setRadius(radius);
    circle.setFillColor(sf::Color(uint8_t(cols[0]*255), uint8_t(cols[1]*255), uint8_t(cols[2]*255)));
    circle.setPosition({circle.getPosition().x + speed_y, circle.getPosition().y + speed_y});
}

inline sf::CircleShape create_circle(const std::vector<std::string>& vec)
{
    const unsigned int nr_segments = 32;
    sf::CircleShape circle(std::stoul(vec[9]), nr_segments);
    circle.setPosition({ std::stof(vec[2]), std::stof(vec[3]) });
    circle.setFillColor(sf::Color(std::stoi(vec[6]), std::stoi(vec[7]), std::stoi(vec[8])));
    return std::move(circle);
}

inline sf::RectangleShape create_rect(const std::vector<std::string>& vec)
{
    sf::RectangleShape rect({ std::stof(vec[9]), std::stof(vec[10]) });
    rect.setPosition({ std::stof(vec[2]), std::stof(vec[3]) });
    rect.setFillColor(sf::Color(std::stoi(vec[6]), std::stoi(vec[7]), std::stoi(vec[8])));
    return std::move(rect);
}

template <typename F>
inline sf::Text create_text(sf::Font font, unsigned int fontSize, const std::string& value, F get_pos)
{
    sf::Text text(font, value, fontSize);
    text.setFillColor(sf::Color::Black);
    text.setPosition(get_pos(text));
    return text;
}

// template <typename T>
// Note in case of circle size is diameter both for x and y
inline sf::Vector2f get_text_pos(const sf::Vector2f& shape_pos, const sf::Vector2f& shape_size, const sf::Text& t)
{
    std::cout << "get_text_pos generic" << std::endl;
    auto center = t.getLocalBounds().getCenter();
    auto x = shape_pos.x + (shape_size.x/2) - center.x;
    auto y = shape_pos.y + (shape_size.y/2) - center.y;
    return {x, y};
    // throw std::string("not implemented");
}

// TODO Prob not entirely correct at the moment
// template <>
// inline sf::Vector2f get_text_pos<sf::CircleShape>(const sf::CircleShape& c, const sf::Text& t)
// {
//     std::cout << "get_text_pos circle" << std::endl;
//     auto center = t.getLocalBounds().getCenter();
//     auto x = c.getPosition().x + c.getRadius() - center.x;// - text_len; //(text_len / 2);
//     auto y = c.getPosition().y + c.getRadius() - center.y;// (font_size / 2) - center.y;
//     return {x, y};
// }

// // TODO Prob not entirely correct at the moment
// template <>
// inline sf::Vector2f get_text_pos<sf::RectangleShape>(const sf::RectangleShape& r, const sf::Text& t)
// {
//     std::cout << "get_text_pos rectangle" << std::endl;
//     auto center = t.getLocalBounds().getCenter();
//     auto x = r.getPosition().x + (r.getSize().x/2) - center.x;
//     auto y = r.getPosition().y + (r.getSize().y/2) - center.y;
//     return {x, y};
// }