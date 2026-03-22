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

// void setEditor(ShapeData& shape, bool& drawText, float& circleRadius, int& circleSegments, char* displayString, float c[3])
// {
//     ImGui::Begin("Shape properties");
//     ImGui::Combo()
//     // ImGui::Text("Edit Options");
//     ImGui::Checkbox("Draw shape", &drawCircle);
//     ImGui::SameLine();
//     ImGui::Checkbox("Draw Text", &drawText);
//     ImGui::SliderFloat("Radius", &circleRadius, 0.0f, 300.0f);
//     ImGui::SliderInt("Sides", &circleSegments, 3, 64);
//     ImGui::ColorEdit3("Color Circle", c);
//     ImGui::InputText("Text", displayString, 255);
//     // ImGui::Button("Set Text");
//     // ImGui::SameLine();
//     // ImGui::Button("Reset");
//     // ImGui::End();
// }

int main(int argc, char* argv[])
{
    unsigned int wWidth = 1280;
    unsigned int wHeight = 720;
    unsigned int fontSize = 14;

    std::vector<ShapeData> shapes;
    std::vector<TextData> texts;
    sf::Font font;    

    // Get config
    std::cout << "Reading config..." << std::endl;
    std::ifstream file("config/config.txt");
    std::string line;
    while (std::getline(file, line)) {
        auto vec = split(line, " ");
        std::cout << "Found " << vec.size() << " cols for setting " << vec[0] << std::endl;
        if(vec.size() < 3) continue;
        auto setting = to_setting(vec[0]);
        switch (setting)
        {
        case Setting::Window:
            wWidth = std::stoul(vec[1]);
            wHeight = std::stoul(vec[2]);
            break;
        case Setting::Fonts:
            if (vec.size() == 6) {
                if(!font.openFromFile(vec[1])) {
                    std::cout << "Could not open font file!" << "\n";
                    std::exit(-1);
                }
                fontSize = std::stoul(vec[2]); 
                // TODO the color things?
            }
            break;
        case Setting::Circle:
            if (vec.size() == 10) {
                shapes.emplace_back(ShapeData(vec[1], create_circle(vec), std::stof(vec[4]), std::stof(vec[5]), setting));
                // Asserts we have read font... maybe fix later
                texts.emplace_back(create_text(font, fontSize, vec[1], [&shapes](const sf::Text& text) { 
                    return get_text_pos(shapes.back().get_pos(), shapes.back().get_size(), text);
                }));
                // texts.emplace_back(create_text(font, fontSize, vec[1], [&shapes](const sf::Text& text) { return get_text_pos(*(shapes.back().shape.get()), text); }));
            }
            break;
        case Setting::Rectangle:
            if (vec.size() == 11) {
                shapes.emplace_back(ShapeData(vec[1], create_rect(vec), std::stof(vec[4]), std::stof(vec[5]), setting));
                texts.emplace_back(create_text(font, fontSize, "Sample Text", [&shapes](const sf::Text& text) { 
                    return get_text_pos(shapes.back().get_pos(), shapes.back().get_size(), text);
                    // return get_text_pos(*(dynamic_cast<sf::RectangleShape*>(shapes.back().shape.get())), text); 
                }));
            }
        default: 
            break;
        }
    }
    file.close();
    std::cout << "Config read! Found " << shapes.size() << " shapes." << std::endl;

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

    // This is the color in the editor that you can select to set a shape
    float c[3] = { 0.0f, 1.0f, 1.0f };
    static int selected_shape_id = 0;
    static char selected_shape_text[255];
    strcpy(selected_shape_text, shapes[selected_shape_id].name.c_str());
    static ImGuiComboFlags flags = 0;
    
    std::cout << "Starting main loop" << std::endl;
    // main loop
    while(window.isOpen())
    {
        while(auto event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);
            if(event->is<sf::Event::Closed>())
            {
                std::cout << "Got close event" << std::endl;
                window.close();
            }

            // if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            // {
            //     std::cout << "Key pressed with code = " << (int)keyPressed->scancode << "\n";
            //     if(keyPressed->scancode == sf::Keyboard::Scancode::X)
            //     {
            //         circleSpeedX *= -1.0f;
            //     }

            // }   

            ImGui::SFML::Update(window, deltaClock.restart());

            // This did not turn out great...
            // setEditor(drawCircle, drawText, circleRadius, circleSegments, displayString, c);
            
            std::cout << "Setting up imgui window" << std::endl;
            ImGui::Begin("Shape properties");
            if (ImGui::BeginCombo("Shapes", selected_shape_text, flags))
            {
                for (int n = 0; n < shapes.size(); n++)
                {
                    const bool is_selected = (selected_shape_id == n);
                    if (ImGui::Selectable(shapes[n].name.c_str(), is_selected))
                        selected_shape_id = n;

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            float scale;
            float velocity;
            // char display_string[255] = selected_shape_text;
            // ImGui::Text("Edit Options");
            ImGui::Checkbox("Draw shape", &shapes[selected_shape_id].draw);
            ImGui::SameLine();
            ImGui::Checkbox("Draw Text", &texts[selected_shape_id].draw);
            ImGui::SliderFloat("Scale", &scale, 0.0f, 4.0f);
            ImGui::SliderFloat2("Velocity", &velocity, -8, 8);
            ImGui::ColorEdit3("Color", c);
            ImGui::InputText("Name", selected_shape_text, 255);
            // if(ImGui::Button("Set Text"))
            // {
            //     text.setString(displayString);
            // }
            // ImGui::SameLine();
            if(ImGui::Button("Reset"))
            {
                shapes[selected_shape_id].shape->setPosition({0, 0});
            }
            ImGui::End();
            std::cout << "Imgui setup done!" << std::endl;

            // TODO update shape
            // TODO update text
            // circle.setPointCount(circleSegments);
            // circle.setRadius(circleRadius);
            // circle.setFillColor(sf::Color(uint8_t(c[0]*255), uint8_t(c[1]*255), uint8_t(c[2]*255)));
            // circle.setPosition({circle.getPosition().x + circleSpeedX, circle.getPosition().y + circleSpeedY});
            
            // text.setPosition(getTextPos(circle, text));
            
            window.clear();
            std::cout << "Drawing shapes" << std::endl;
            for(const auto& shape : shapes)
            {
                if(shape.draw)
                {
                    window.draw(*shape.shape);
                }

            }
            std::cout << "Drawing texts" << std::endl;
            for(const auto& text : texts)
            {
                if(text.draw)
                {
                    std::cout << "Drawing text: " << text.text.getPosition().x << " " << text.text.getPosition().y << " " << (std::string)text.text.getString() << std::endl;
                    window.draw(text.text);
                }
            }
            std::cout << "Render to screen" << std::endl;
            ImGui::SFML::Render(window);
            window.display();
        }    
    }
    return 0;
}