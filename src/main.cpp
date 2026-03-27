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


void print(const sf::Text& text)
{
    std::cout << "Text pos: (" 
        << text.getPosition().x 
        << ", " << text.getPosition().y 
        << "). Value: " << (std::string)text.getString() 
        << std::endl;
}

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
        // std::cout << "Found " << vec.size() << " cols for setting " << vec[0] << std::endl;
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
                shapes.emplace_back(ShapeData(create_circle(vec), std::stof(vec[4]), std::stof(vec[5]), setting));
                // Asserts we have read font... maybe fix later
                texts.emplace_back(create_text(font, fontSize, vec[1], [&shapes](const sf::Text& text) { 
                    return get_text_pos(shapes.back().get_pos(), 
                                        shapes.back().get_size(), 
                                        shapes.back().shape->getScale(),
                                        text);
                }));
                // texts.emplace_back(create_text(font, fontSize, vec[1], [&shapes](const sf::Text& text) { return get_text_pos(*(shapes.back().shape.get()), text); }));
            }
            break;
        case Setting::Rectangle:
            if (vec.size() == 11) {
                shapes.emplace_back(ShapeData(create_rect(vec), std::stof(vec[4]), std::stof(vec[5]), setting));
                texts.push_back(create_text(font, fontSize, vec[1], [&shapes](const sf::Text& text) { 
                    return get_text_pos(shapes.back().get_pos(), 
                                        shapes.back().get_size(), 
                                        shapes.back().shape->getScale(),
                                        text);
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
    window.setPosition({1921,0}); // 0,0
    if(!ImGui::SFML::Init(window))
    {
        std::cout << "Could not init window" << "\n";
        std::exit(-1);
    }

    sf::Clock deltaClock;

    ImGui::GetStyle().ScaleAllSizes(2.0f);
    ImGui::GetIO().FontGlobalScale = 2.0f;

    // This is the color in the editor that you can select to set a shape
    static int selected_shape_id = 0;
    auto curr_col = shapes[selected_shape_id].shape->getFillColor();
    float c[3] = { static_cast<float>(curr_col.r)/255.0f, 
                   static_cast<float>(curr_col.g)/255.0f, 
                   static_cast<float>(curr_col.b)/255.0f };
    static char selected_shape_text[255];
    strcpy(selected_shape_text, texts[selected_shape_id].value().c_str());
    static ImGuiComboFlags flags = 0;
    

    /// TEST
    // test_text(window, texts);
    /// END TEST

    std::cout << "Starting main loop" << std::endl;
    // ImGui::SetWindowPos({window.getSize().x - ImGui::GetWindowWidth(), 0});
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
        }


        ImGui::SFML::Update(window, deltaClock.restart());

        // This did not turn out great...
        // setEditor(drawCircle, drawText, circleRadius, circleSegments, displayString, c);
        
        // std::cout << "Setting up imgui window" << std::endl;
        ImGui::Begin("Shape properties");
        if (ImGui::BeginCombo("Shapes", texts[selected_shape_id].value().c_str(), flags))
        {
            for (int n = 0; n < shapes.size(); n++)
            {
                const bool is_selected = (selected_shape_id == n);
                auto last_selected_shape_id = selected_shape_id;
                if (ImGui::Selectable(texts[n].value().c_str(), is_selected)) {
                    selected_shape_id = n;
                }
                if (selected_shape_id != last_selected_shape_id) {
                    strcpy(selected_shape_text, texts[selected_shape_id].value().c_str());
                    auto curr_col = shapes[selected_shape_id].shape->getFillColor();
                    c[0] = static_cast<float>(curr_col.r)/255.0f;
                    c[1] = static_cast<float>(curr_col.g)/255.0f;
                    c[2] = static_cast<float>(curr_col.b)/255.0f;
                }

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        auto& shape = shapes[selected_shape_id];
        auto& text = texts[selected_shape_id];
        float scale{shape.shape->getScale().x}; // for now x and y scale are always same
        
        // char display_string[255] = selected_shape_text;
        // ImGui::Text("Edit Options");
        ImGui::Checkbox("Draw shape", &shape.draw);
        text.draw = shape.draw;
        // ImGui::SameLine();
        // ImGui::Checkbox("Draw Text", &texts[selected_shape_id].draw);
        ImGui::SliderFloat("Scale", &scale, 0.0f, 4.0f);
        ImGui::SliderFloat2("Velocity", shape.velocity, -8, 8);
        // if (selected_shape_id != last_selected_shape_id) {
        // }
        ImGui::ColorEdit3("Color", c);
        shape.set_color(c);
        ImGui::InputText("Name", selected_shape_text, 255);
        if(ImGui::Button("Set Text"))
        {
            text.text.setString(selected_shape_text);
        }
        // ImGui::SameLine();
        // if(ImGui::Button("Set Color"))
        // {
        // }
        ImGui::SameLine();
        if(ImGui::Button("Reset"))
        {
            shape.shape->setPosition({0, 0});
        }
        ImGui::End();
        // std::cout << "Imgui setup done!" << std::endl;

        shape.shape->setScale({scale, scale}); // update point count if circle? circle.setPointCount(circleSegments);
        
        for (auto& s : shapes) {
            update_velocity(s, window.getSize());
            s.set_pos();
        }
        for (std::size_t i = 0; i < texts.size(); ++i) {
            auto& t = texts[i];
            t.text.setPosition(get_text_pos(shapes[i].get_pos(), 
                                            shapes[i].get_size(), 
                                            shapes[i].shape->getScale(),
                                            t.text));
        }
        
        window.clear();
        // std::cout << "Drawing shapes" << std::endl;
        for(const auto& shape : shapes)
        {
            if(shape.draw)
            {
                window.draw(*shape.shape);
            }

        }
        // std::cout << "Drawing texts" << std::endl;
        for(const auto& text : texts)
        {
            if(text.draw)
            {
                window.draw(text.text);
            }
        }
        // std::cout << "Render to screen" << std::endl;
        ImGui::SFML::Render(window);
        window.display(); 
        // last_selected_shape_id = selected_shape_id;   
    }
    ImGui::SFML::Shutdown();
    return 0;
}