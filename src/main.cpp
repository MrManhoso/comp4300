#include <filesystem>
#include <iostream>
#include <memory>
#include <fstream>

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

/*******************************
TODOS
1 move sample text into circle
2a read from config file instead (assets/config.txt) 
    - window
    - shapes
    - font 
    - yadayada
2b have some container that stores shapes (the ones found in config file)
3 draw shapes from container
********************************/

sf::Vector2f getTextPos(const sf::CircleShape& c, unsigned int font_size, unsigned int text_len)
{
    auto x = c.getPosition().x + c.getRadius();// - text_len; //(text_len / 2);
    auto y = c.getPosition().y + c.getRadius() - (font_size / 2);
    return {x, y};
}

int main(int argc, char* argv[])
{
    
    const int wWidth = 1280;
    const int wHeight = 720;
    const unsigned int fontSize = 14;

    sf::RenderWindow window(sf::VideoMode({ wWidth, wHeight }), "SFML works!");
    window.setFramerateLimit(60);
    window.setPosition({0,0}); // 1921,0
    if(!ImGui::SFML::Init(window))
    {
        std::cout << "Could not init window" << "\n";
        std::exit(-1);
    }

    sf::Clock deltaClock;

    ImGui::GetStyle().ScaleAllSizes(2.0f);
    ImGui::GetIO().FontGlobalScale = 2.0f;

    // One per shape?
    float c[3] = { 0.0f, 1.0f, 1.0f };

    // A test shape, take from config file instead
    float circleRadius = 50;
    int circleSegments = 32;
    float circleSpeedX = 1.0f;
    float circleSpeedY = 0.5f;
    bool drawCircle = true;
    bool drawText = true;
    
    sf::CircleShape circle(circleRadius, circleSegments);
    circle.setPosition({ 10.0f, 10.0f });

    sf::Font font;

    if(!font.openFromFile("assets/javatext.ttf"))
    {
        std::cout << "Could not open font file!" << "\n";
        std::exit(-1);
    }
    sf::Text text(font, "Sample Text", fontSize);
    std::size_t string_size = 11;

    // text.setPosition({ circle.getPosition().x, wHeight - (float)(text.getCharacterSize())});
    text.setPosition(getTextPos(circle, fontSize, string_size));

    char displayString[255] = "Sample Text";
    
    // main loop
    while(window.isOpen())
    {
        while(auto event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);
            if(event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                std::cout << "Key pressed with code = " << (int)keyPressed->scancode << "\n";
                if(keyPressed->scancode == sf::Keyboard::Scancode::X)
                {
                    circleSpeedX *= -1.0f;
                }

            }   

            ImGui::SFML::Update(window, deltaClock.restart());

            ImGui::Begin("Window title!");
            ImGui::Text("Window text");
            ImGui::Checkbox("Draw Circle", &drawCircle);
            ImGui::SameLine();
            ImGui::Checkbox("Draw Text", &drawText);
            ImGui::SliderFloat("Radius", &circleRadius, 0.0f, 300.0f);
            ImGui::SliderInt("Sides", &circleSegments, 3, 64);
            ImGui::ColorEdit3("Color Circle", c);
            ImGui::InputText("Text", displayString, 255);
            if(ImGui::Button("Set Text"))
            {
                text.setString(displayString);
            }
            ImGui::SameLine();
            if(ImGui::Button("Reset Circle"))
            {
                 circle.setPosition({0, 0});
            }
            ImGui::End();

            circle.setPointCount(circleSegments);
            circle.setRadius(circleRadius);
            circle.setFillColor(sf::Color(uint8_t(c[0]*255), uint8_t(c[1]*255), uint8_t(c[2]*255)));
            circle.setPosition({circle.getPosition().x + circleSpeedX, circle.getPosition().y + circleSpeedY});
            
            text.setPosition(getTextPos(circle, fontSize, string_size));
            
            window.clear();
            if(drawCircle)
            {
                window.draw(circle);
            }
            if(drawText)
            {
                window.draw(text);
            }
            ImGui::SFML::Render(window);
            window.display();
        }    
    }
    return 0;
}