#include "AudioSettings.hpp"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <sstream>
#include <algorithm>
#include <vector>
#include <iostream>

// Constructor
AudioSettings::AudioSettings(float x, float y, float width, float height, sf::Music* music)
    : volume(100.0f), isDragging(false), music(music) {
    if (music != nullptr) {
        music->setVolume(volume);
    }
    // Configurar la barra de fondo del slider
    bar.setSize(sf::Vector2f(width, height));
    bar.setPosition(x, y);
    bar.setFillColor(sf::Color(50, 50, 50)); // Color gris oscuro para la barra
    bar.setOutlineColor(sf::Color(200, 200, 200)); // Color del borde
    bar.setOutlineThickness(2.0f); // Grosor del borde

    // Configurar la barra pintada
    filledBar.setSize(sf::Vector2f(width, height));
    filledBar.setPosition(x, y);
    filledBar.setFillColor(sf::Color(0, 255, 0)); // Color verde para la barra pintada
    filledBar.setOutlineColor(sf::Color(200, 200, 200)); // Color del borde
    filledBar.setOutlineThickness(2.0f); // Grosor del borde

    // Configurar el mango del slider (thumb) como c�rculo
    thumb.setRadius(height / 2 + 5.f); // Un poco m�s grande que la barra para ser m�s visible
    thumb.setFillColor(sf::Color::Red);
    thumb.setOutlineColor(sf::Color::Black); // Color del borde del mango
    thumb.setOutlineThickness(2.0f); // Grosor del borde
    thumb.setPosition(x + width - thumb.getRadius() * 2, 6 + y - thumb.getRadius());  // Inicialmente en el 100% de la barra

    // Configurar el texto para mostrar el volumen
    if (!font.loadFromFile("resource/fonts/Pixel Times Bold.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
    }
    volumeText.setFont(font);
    volumeText.setCharacterSize(20);
    volumeText.setPosition(x + width + 15.f, y - 5.f);
    volumeText.setFillColor(sf::Color::White);
    updateVolumeText();
}

AudioSettings::AudioSettings(float x, float y, float width, float height, std::vector<sf::Sound*>& effects)
    : volume(100.0f), isDragging(false), effects(effects) {
    for (auto* effect : effects) {
        if (effect != nullptr) {
            effect->setVolume(volume);
        }
    }
    // Configurar la barra de fondo del slider
    bar.setSize(sf::Vector2f(width, height));
    bar.setPosition(x, y);
    bar.setFillColor(sf::Color(50, 50, 50)); // Color gris oscuro para la barra
    bar.setOutlineColor(sf::Color(200, 200, 200)); // Color del borde
    bar.setOutlineThickness(2.0f); // Grosor del borde

    // Configurar la barra pintada
    filledBar.setSize(sf::Vector2f(width, height));
    filledBar.setPosition(x, y);
    filledBar.setFillColor(sf::Color(0, 255, 0)); // Color verde para la barra pintada
    filledBar.setOutlineColor(sf::Color(200, 200, 200)); // Color del borde
    filledBar.setOutlineThickness(2.0f); // Grosor del borde

    // Configurar el mango del slider (thumb) como c�rculo
    thumb.setRadius(height / 2 + 5.f); // Un poco m�s grande que la barra para ser m�s visible
    thumb.setFillColor(sf::Color::Red);
    thumb.setOutlineColor(sf::Color::Black); // Color del borde del mango
    thumb.setOutlineThickness(2.0f); // Grosor del borde
    thumb.setPosition(x + width - thumb.getRadius() * 2, 6 + y - thumb.getRadius());  // Inicialmente en el 100% de la barra

    // Configurar el texto para mostrar el volumen
    if (!font.loadFromFile("resource/fonts/Pixel Times Bold.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
    }
    volumeText.setFont(font);
    volumeText.setCharacterSize(20);
    volumeText.setPosition(x + width + 15.f, y - 5.f);
    volumeText.setFillColor(sf::Color::White);
    updateVolumeText();
}


// Manejar los eventos del rat�n
void AudioSettings::handleEvent(sf::Event& event, const sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    switch (event.type) {
    case sf::Event::MouseButtonPressed:
        if (thumb.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
            isDragging = true;  // Comienza a arrastrar el mango
        }
        break;

    case sf::Event::MouseButtonReleased:
        isDragging = false;  // Termina de arrastrar el mango
        break;

    case sf::Event::MouseMoved:
        if (isDragging) {
            moveThumb(static_cast<float>(mousePos.x));
        }
        break;

    default:
        break;
    }
}

// Movimiento del mango del slider (se aplica a m�sica o efectos)
void AudioSettings::moveThumb(float mouseX) {
    float barLeft = bar.getPosition().x;
    float barRight = barLeft + bar.getSize().x;

    mouseX = clamp(mouseX, barLeft, barRight);

    thumb.setPosition(mouseX - thumb.getRadius(), thumb.getPosition().y);

    float percentage = (mouseX - barLeft) / bar.getSize().x;
    volume = percentage * 100.0f;

    filledBar.setSize(sf::Vector2f(percentage * bar.getSize().x, bar.getSize().y));

    // Ajustar el volumen de la m�sica
    if (music != nullptr) {
        music->setVolume(volume);
    }

    // Ajustar el volumen de los efectos
    for (auto* effect : effects) {
        if (effect != nullptr) {
            effect->setVolume(volume);
        }
    }

    updateVolumeText();
}


// Dibujar el slider en la pantalla
void AudioSettings::draw(sf::RenderWindow& window) const {
    window.draw(bar);
    window.draw(filledBar); // Dibuja la parte pintada de la barra
    window.draw(thumb);
    window.draw(volumeText);
}

// Obtener el volumen actual
float AudioSettings::getVolume() const {
    return volume;
}

// Actualizar el texto del volumen
void AudioSettings::updateVolumeText() {
    std::ostringstream oss;
    oss << "Vol: " << static_cast<int>(volume) << "%";
    volumeText.setString(oss.str());
}

// Funci�n de ayuda para clamping
float AudioSettings::clamp(float value, float min, float max) const {
    return std::max(min, std::min(value, max));
}
