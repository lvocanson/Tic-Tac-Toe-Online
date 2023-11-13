#pragma once

// Forward declarations
namespace sf
{
class RenderWindow;
}

class Window
{
public:
    Window();
    ~Window();

#pragma region Lifecycle

    /// <summary>
    /// Creates (or recreates) a window with the given title and dimensions.
    /// </summary>
    void Create(const char* title, unsigned int width, unsigned int height);
    /// <summary>
    /// Polls events and calls callbacks.
    /// </summary>
    void PollEvents();
    /// <summary>
    /// Returns whether the window is open.
    /// </summary>
    bool IsOpen() const;

#pragma endregion

#pragma region Rendering

    /// <summary>
    /// Clears the window and render all the drawable objects.
    /// </summary>
    void Render();
    /// <summary>
    /// Sets the clear color of the window.
    /// </summary>
    void SetClearColor(const sf::Color& color) { m_ClearColor = color; }
    /// <summary>
    /// Registers a drawable object to be rendered.
    /// </summary>
    void RegisterDrawable(sf::Drawable* drawable) { m_Drawables.push_back(drawable); }
    /// <summary>
    /// Unregisters a drawable object.
    /// </summary>
    void UnregisterDrawable(sf::Drawable* drawable) { m_Drawables.erase(std::remove(m_Drawables.begin(), m_Drawables.end(), drawable), m_Drawables.end()); }
    /// <summary>
    /// Returns all the registered drawable objects.
    /// </summary>
    std::vector<sf::Drawable*>& GetDrawables() { return m_Drawables; }

#pragma endregion

#pragma region Input

    /// <summary>
    /// Returns whether the given key is pressed.
    /// </summary>
    bool IsKeyPressed(sf::Keyboard::Key key) const;
    /// <summary>
    /// Returns whether the given mouse button is pressed.
    /// </summary>
    bool IsMouseButtonPressed(sf::Mouse::Button button) const;
    /// <summary>
    /// Returns the mouse position relative to the window.
    /// </summary>
    sf::Vector2i GetMousePosition() const;

#pragma endregion

#pragma region Other

    sf::Vector2f GetCenter() const { return sf::Vector2f(m_Window->getSize().x * 0.5f, m_Window->getSize().y *0.5f); }

#pragma endregion

private:
    sf::RenderWindow* m_Window = nullptr;
    sf::Color m_ClearColor;
    std::vector<sf::Drawable*> m_Drawables;
};