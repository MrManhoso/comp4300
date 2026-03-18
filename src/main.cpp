#include "data.h"
#include "utils.h"

#include <filesystem>
#include <iostream>
#include <memory>
#include <fstream>
#include <string>

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

/*******************************
TODOS
Assignment 1:
1 move sample text into circle
2a read from config file instead (assets/config.txt) 
    - window
    - shapes
    - font 
    - yadayada
2b have some container that stores shapes (the ones found in config file)
3 draw shapes from container
4 shapes skall studsa mot kanter. Dvs reversa x speed om den studsar mot lodrät kant
5 shape name should be drawn in center of shape with given color and font size
5 Imgui interface (editorn/shape properties)
    - List all shapes, select any one and edit selected shapes properties
    - Toggle whether the shape should be drawn or not
    - Change scale of shape, 0-4
    - Change x and y velocity, -8 - 8
    - Change color of shape
    - Change name of shape
********************************/

void setEditor(bool& drawCircle, bool& drawText, float& circleRadius, int& circleSegments, char* displayString, float c[3])
{
    ImGui::Begin("Editor");
    ImGui::Text("Edit Options");
    ImGui::Checkbox("Draw Circle", &drawCircle);
    ImGui::SameLine();
    ImGui::Checkbox("Draw Text", &drawText);
    ImGui::SliderFloat("Radius", &circleRadius, 0.0f, 300.0f);
    ImGui::SliderInt("Sides", &circleSegments, 3, 64);
    ImGui::ColorEdit3("Color Circle", c);
    ImGui::InputText("Text", displayString, 255);
    // ImGui::Button("Set Text");
    // ImGui::SameLine();
    // ImGui::Button("Reset");
    // ImGui::End();
}

// TODO Prob not entirely correct at the moment
sf::Vector2f getTextPos(const sf::CircleShape& c, const sf::Text& t)
{
    auto center = t.getLocalBounds().getCenter();
    auto x = c.getPosition().x + c.getRadius() - center.x;// - text_len; //(text_len / 2);
    auto y = c.getPosition().y + c.getRadius() - center.y;// (font_size / 2) - center.y;
    return {x, y};
}



int main(int argc, char* argv[])
{
    unsigned int wWidth = 1280;
    unsigned int wHeight = 720;
    unsigned int fontSize = 14;
    
    // Get config
    std::ifstream file("config/config.txt");
    std::string line;
    while (std::getline(file, line)) {
        auto vec = split(line, " ");
        if(vec.size() < 3) continue;
        switch (to_setting(vec[0]))
        {
        case Setting::Window:
            wWidth = std::stoul(vec[1]);
            wHeight = std::stoul(vec[2]);
            break;
        // TODO rest!
        }
    }
    file.close();

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
    text.setFillColor(sf::Color::Black);
    text.setPosition(getTextPos(circle, text));

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

            // This did not turn out great...
            setEditor(drawCircle, drawText, circleRadius, circleSegments, displayString, c);
            if(ImGui::Button("Set Text"))
            {
                text.setString(displayString);
            }
            ImGui::SameLine();
            if(ImGui::Button("Reset"))
            {
                circle.setPosition({0, 0});
            }
            ImGui::End();

            circle.setPointCount(circleSegments);
            circle.setRadius(circleRadius);
            circle.setFillColor(sf::Color(uint8_t(c[0]*255), uint8_t(c[1]*255), uint8_t(c[2]*255)));
            circle.setPosition({circle.getPosition().x + circleSpeedX, circle.getPosition().y + circleSpeedY});
            
            text.setPosition(getTextPos(circle, text));
            
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