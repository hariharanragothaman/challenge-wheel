#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <cmath>

#include <SFML/Graphics.hpp>


const float PI = 3.14159265359f;

sf::Color generateColor(int index)
{
    /* Generate a simple color pattern based on index */
    static std::vector<sf::Color> colors = {sf::Color::Red,
                                            sf::Color::Green,
                                            sf::Color::Blue,
                                            sf::Color::Yellow, sf::Color::Cyan, sf::Color::Magenta,
                                            sf::Color(255, 165, 0), // Orange
                                            sf::Color(128, 0, 128)  // Purple
                                            };
    return colors[index % colors.size()];
}

void drawWheel(sf::RenderWindow& window, const std::vector<std::string>& names, float angleOffset, sf::Font& font)
{
    float radius = 200.0f;
    sf::Vector2f center(400, 400);
    int numSectors = names.size();
    float anglePerSector = 360.0f / numSectors;

    for (int i = 0; i < numSectors; ++i)
    {
        // Calculate sector shape (using a triangle fan approach)
        sf::ConvexShape sector;
        sector.setPointCount(3);
        sector.setPoint(0, center);  // Center of the wheel
        sector.setPoint(1, sf::Vector2f(
                center.x + radius * cos(PI * (angleOffset + i * anglePerSector) / 180.0f),
                center.y + radius * sin(PI * (angleOffset + i * anglePerSector) / 180.0f)
        ));
        sector.setPoint(2, sf::Vector2f(
                center.x + radius * cos(PI * (angleOffset + (i + 1) * anglePerSector) / 180.0f),
                center.y + radius * sin(PI * (angleOffset + (i + 1) * anglePerSector) / 180.0f)
        ));
        sector.setFillColor(generateColor(i));

        // Draw sector
        window.draw(sector);

        // Draw the name
        sf::Text nameText;
        nameText.setFont(font);
        nameText.setString(names[i]);
        nameText.setCharacterSize(20);
        nameText.setFillColor(sf::Color::Black);

        // Position text in the center of the sector
        float textAngle = angleOffset + (i + 0.5f) * anglePerSector;
        float textX = center.x + (radius / 2) * cos(PI * textAngle / 180.0f);
        float textY = center.y + (radius / 2) * sin(PI * textAngle / 180.0f);
        nameText.setPosition(textX - nameText.getLocalBounds().width / 2, textY - nameText.getLocalBounds().height / 2);

        window.draw(nameText);
    }
}

std::string pickRandomWinner(const std::vector<std::string>& names)
{
    if (names.empty())
        return "No names available";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, names.size() - 1);
    return names[dis(gen)];
}


void showWinnerPopup(sf::RenderWindow& window, const std::string& winner, sf::Font& font)
{
    sf::RenderWindow popup(sf::VideoMode(400, 200), "Lucky Winner!");
    sf::Text winnerText;
    winnerText.setFont(font);
    winnerText.setString("The lucky winner is:\n" + winner);
    winnerText.setCharacterSize(24);
    winnerText.setFillColor(sf::Color::Black);
    winnerText.setPosition(50, 50);

    while (popup.isOpen())
    {
        sf::Event event;
        while (popup.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                popup.close();
        }
        popup.clear(sf::Color::White);
        popup.draw(winnerText);
        popup.display();
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Wheel of Fortune");

    /* Font for rendering text */
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return -1;
    }

    /* Text to display the input */
    sf::Text userInputText;
    userInputText.setFont(font);
    userInputText.setCharacterSize(18);
    userInputText.setFillColor(sf::Color::White);
    userInputText.setPosition(50, 800); // Adjust position as needed

    std::string userInput;

    /* Button for submission */
    sf::RectangleShape submitButton(sf::Vector2f(150, 50));
    submitButton.setFillColor(sf::Color::Green);
    submitButton.setPosition(800, 850); // Adjust position as needed

    sf::Text submitButtonText;
    submitButtonText.setFont(font);
    submitButtonText.setString("Submit");
    submitButtonText.setCharacterSize(24);
    submitButtonText.setFillColor(sf::Color::Black);
    submitButtonText.setPosition(820, 860); // Adjust position as needed

    /* Button for shuffle */
    sf::RectangleShape shuffleButton(sf::Vector2f(150, 50));
    shuffleButton.setFillColor(sf::Color::Blue);
    shuffleButton.setPosition(800, 780); // Adjust position as needed

    sf::Text shuffleButtonText;
    shuffleButtonText.setFont(font);
    shuffleButtonText.setString("Shuffle");
    shuffleButtonText.setCharacterSize(24);
    shuffleButtonText.setFillColor(sf::Color::White);
    shuffleButtonText.setPosition(820, 790); // Adjust position as needed

    /* Button for sort */
    sf::RectangleShape sortButton(sf::Vector2f(150, 50));
    sortButton.setFillColor(sf::Color::Red);
    sortButton.setPosition(800, 720); // Adjust position as needed

    sf::Text sortButtonText;
    sortButtonText.setFont(font);
    sortButtonText.setString("Sort");
    sortButtonText.setCharacterSize(24);
    sortButtonText.setFillColor(sf::Color::White);
    sortButtonText.setPosition(820, 730); // Adjust position as needed

    /* Button for start spinning */
    sf::RectangleShape startSpinButton(sf::Vector2f(150, 50));
    startSpinButton.setFillColor(sf::Color::Yellow);
    startSpinButton.setPosition(800, 650); // Adjust position as needed

    sf::Text startSpinButtonText;
    startSpinButtonText.setFont(font);
    startSpinButtonText.setString("StartSpin");
    startSpinButtonText.setCharacterSize(24);
    startSpinButtonText.setFillColor(sf::Color::Black);
    startSpinButtonText.setPosition(820, 660); // Adjust position as needed

    /* Button for stop spinning */
    sf::RectangleShape stopSpinButton(sf::Vector2f(150, 50));
    stopSpinButton.setFillColor(sf::Color::Cyan);
    stopSpinButton.setPosition(800, 580); // Adjust position as needed

    sf::Text stopSpinButtonText;
    stopSpinButtonText.setFont(font);
    stopSpinButtonText.setString("StopSpin");
    stopSpinButtonText.setCharacterSize(24);
    stopSpinButtonText.setFillColor(sf::Color::Black);
    stopSpinButtonText.setPosition(820, 590); // Adjust position as needed

    std::vector<std::string> names;
    float wheelRotationSpeed = 0.0f; // Initial speed
    bool isSpinning = false; // To control wheel spin state
    float currentAngle = 0.0f;

    // Clock to track time for smooth animation
    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode == '\b')
                {
                    // Handle backspace
                    if (!userInput.empty())
                        userInput.pop_back();
                }
                else if (event.text.unicode < 128)
                {
                    // Handle regular characters
                    userInput += static_cast<char>(event.text.unicode);
                }
            }

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                    if (submitButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
                    {
                        // Process the input when the submit button is clicked
                        std::istringstream iss(userInput);
                        std::string name;
                        names.clear(); // Clear previous entries
                        while (std::getline(iss, name, ',')) {
                            names.push_back(name);
                        }

                        // Clear the user input after submission
                        userInput.clear();
                    }

                    if (shuffleButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
                    {
                        // Shuffle the names
                        std::random_device rd;
                        std::mt19937 g(rd());
                        std::shuffle(names.begin(), names.end(), g);
                    }

                    if (sortButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
                    {
                        // Sort the names alphabetically
                        std::sort(names.begin(), names.end());
                    }

                    if (startSpinButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
                    {
                        // Start spinning the wheel with a random speed
                        wheelRotationSpeed = 300.0f + std::rand() % 200; // Random speed between 300 and 500
                        isSpinning = true;
                    }

                    if (stopSpinButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
                    {
                        // Stop spinning the wheel
                        isSpinning = false;
                        wheelRotationSpeed = 0.0f;
                        std::string winner = pickRandomWinner(names);
                        showWinnerPopup(window, winner, font);
                    }
                }
            }
        }

        // Calculate time passed since the last frame
        sf::Time deltaTime = clock.restart();

        // Update the wheel rotation
        if (isSpinning)
        {
            currentAngle += wheelRotationSpeed * deltaTime.asSeconds();
            // wheelRotationSpeed *= 0.99f; // Gradually slow down
        }

        // Update the text displayed
        userInputText.setString("Enter names (comma-separated): " + userInput);

        window.clear();
        drawWheel(window, names, currentAngle, font);
        window.draw(userInputText);
        window.draw(submitButton);
        window.draw(submitButtonText);
        window.draw(shuffleButton);
        window.draw(shuffleButtonText);
        window.draw(sortButton);
        window.draw(sortButtonText);
        window.draw(startSpinButton);
        window.draw(startSpinButtonText);
        window.draw(stopSpinButton);
        window.draw(stopSpinButtonText);
        window.display();
    }

    return 0;
}
