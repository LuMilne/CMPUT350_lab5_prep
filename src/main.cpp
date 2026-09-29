
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <memory>
#include <random>
#include <vector>
#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>

using namespace std;

#define PI 3.14

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int FPS_LIMIT = 30;

// Tweening progress and rate
float pos = 0.f;
float vel = 0.01f;

// Tweening protocol keycodes
std::vector<sf::Keyboard::Key> keyp = {
    sf::Keyboard::Key::Num1,
    sf::Keyboard::Key::Num2, 
    sf::Keyboard::Key::Num3, 
    sf::Keyboard::Key::Num4, 
    sf::Keyboard::Key::Num5, 
    sf::Keyboard::Key::Num6, 
    sf::Keyboard::Key::Num7, 
    sf::Keyboard::Key::Num8, 
    sf::Keyboard::Key::Num9
};
int protocol = 1;

float lin_inter(float a, float b, float t) {
    return (1-t)*a + t*b;
}

// global tween function
std::function<float(float, float, float)> tween = [](float a, float b, float t) {
    float out;
    switch(protocol) {
    // 1. Linear interpolation
    case 1: {out = lin_inter(a,b,t); break;}
    // 2. Ease-in quadratic
    case 2: {out = lin_inter(a,b,t*t); break;}
    // 3. Ease in sin
    case 3: {out = lin_inter(a,b,sin(t*PI)/2); break;}
    // 4. Ease-out cos
    case 4: {out = lin_inter(a,b,-(cos(t*PI)-1)/2); break;}
    // 5. Ease in cubic
    case 5: {out = lin_inter(a,b,t*t*t); break;}
    // 6. Quadratic Bezier
    case 6: {
        float p2 = WINDOW_HEIGHT/2;
        //out = 2*t*(a - 2*p2 + b) + (-2*a + 2*p2);

        break;
    }
    // 7. Cubic Bezier
    case 7: {
        float p2 = WINDOW_HEIGHT/3;
        float p3 = 2*p2;
        out = 2*(1-t)*(1-t)*(p2-a) + 6*(1-t)*t*(p3-p2) + 3*t*t*(b-p3);
        break;
    }
    // 8. Ease-in elastic (taken from https://easings.net/#easeInElastic)
    case 8: {
        float p = (3*PI)/3;

        if(t == 0) {
            return a;
        }
        else if (t == 1) {
            return b;
        }
        else {
            return float(-pow(2,10*t-10) * sin((t*10-10.75)*p));
        }

    }
    // 9. 
    case 9: {}
    }
    
    // 
    return out;
};

void handleInput(sf::Window& window, bool& shouldQuit) {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            shouldQuit = true;
        }

        // ====== ====== ======
        // TODO: (Q2)
        //  implement key presses (1-9) that replace the tween function
        //  with different alternate tween functions.
        //  Functions can be from lecture or from https://easings.net/#
        // ====== ====== ======

        // Get keypress
        if (auto key = event->getIf<sf::Event::KeyPressed>()) {
            // Get an index for the selected tweening form and use as protocol selector
            for(int i = 0; i < keyp.size(); i++) {
                if(keyp[i] == key->code) {
                    protocol = i+1;
                    cout << protocol << '\n';
                }
            }
        }
    }
}

void render(sf::RenderWindow& window) {
    // Clear with blue background (sky)
    window.clear(sf::Color::Black);
    // ====== ====== ======
    // TODO: (Q1) Draw circle that moves between
    // the left/right half of the screen.
    // Movement should be governed by the tween function.
    // ====== ====== ======
    // Make circle
    sf::CircleShape circ(50.0f);
    circ.setFillColor(sf::Color::Blue);
    // Tween and position circle
    if(0.f > pos+vel) {
        pos = 0.f;
        vel *= -1;
    }
    else if (pos+vel > 1.f) {
        pos = 1;
        vel *= -1;
    }
    else {pos += vel;}
    circ.setPosition({lin_inter(0.f,WINDOW_WIDTH-100.f,pos),tween(0.f,WINDOW_WIDTH-100,pos)});
    window.draw(circ);

    // ====== ====== ======
    // TODO: (Q3) Draw tween function graph with a dot
    // on the current portion of the curve
    // ====== ====== ======

    window.display();
}

int main() {
    sf::RenderWindow window;

    try {
        // Initialize window
        window.create(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Tween");
        window.setFramerateLimit(FPS_LIMIT);
        // Prevent key repeats.
        window.setKeyRepeatEnabled(false);

        bool shouldQuit = false;
        // Main game loop
        while (window.isOpen()) {
            handleInput(window, shouldQuit);
            if (shouldQuit) {
                break;
            }
            render(window);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
