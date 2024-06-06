#include <iostream>
#include "screen.h"
#include "vec2.h"

// Estructura para representar un cuerpo celeste
struct Body
{
    float r;  // Radio del cuerpo
    float m;  // Masa del cuerpo
    vec2 pos;  // Posición del cuerpo
    vec2 vel;  // Velocidad del cuerpo
    vec2 acc;  // Aceleración del cuerpo

    // Constructor por defecto
    Body()
    {
        m = 1.0f;
        r = 0.2f;
        pos = 0;
        vel = 0;
        acc = 0;
    }

    // Constructor que inicializa la masa y calcula el radio basado en la masa
    Body(float m) : m(m)
    {
        r = 0.2f * cbrt(m);
        pos = 0;
        vel = 0;
        acc = 0;
    }

    // Constructor que inicializa la masa y el radio
    Body(float m, float r) : r(r), m(m)
    {
        pos = 0;
        vel = 0;
        acc = 0;
    }

    // Función para establecer la posición del cuerpo
    void setPos(float x, float y)
    {
        pos.x = x;
        pos.y = y;
    }

    // Función para calcular la atracción gravitatoria ejercida por otro cuerpo
    void PulledBy(const Body& other)
    {
        const float G = 1.0f;  // Constante de gravitación universal
        float dist = sqrt((pos - other.pos) * (pos - other.pos));
        acc += G * other.m * (other.pos - pos) / dist / dist / dist;
    }

    // Función para actualizar la posición y velocidad del cuerpo
    void Update(float dt)
    {
        vel += dt * acc;
        pos += dt * vel;
        acc = 0;
    }
};

// Función para plotear el cuerpo en la pantalla
void Plot(const Body& body, Screen& scr)
{
    scr.PlotCircle(body.pos.x, body.pos.y, body.r);
}

int main()
{
    Screen scr(0, 0, 10);

    constexpr float dt = 1.0 / 100.0f;  // Intervalo de tiempo
    constexpr float r = 5.5f, R = 30.0f;  // Distancias

    // Inicialización de los cuerpos celestes
    Body Sun(10000.0f, 7.0f);
    Body Earth(1000.0f, 2.0f);
    Body Moon(1.0f, 1.2f);

    Sun.pos = {0.0f, 0.0f};
    Sun.vel = {0.0f, 0.0f};

    Earth.pos = {R, 0.0f};
    Earth.vel = {0.0f, sqrt(Sun.m / R)};

    Moon.pos = {R, r};
    Moon.vel = {sqrt(Earth.m / r), Earth.vel.y};

    while (true)
    {
        scr.Clear();

        // Calcular las fuerzas gravitatorias entre los cuerpos
        Moon.PulledBy(Earth);
        Moon.PulledBy(Sun);
        Earth.PulledBy(Moon);
        Earth.PulledBy(Sun);
        Sun.PulledBy(Moon);
        Sun.PulledBy(Earth);

        // Actualizar las posiciones y velocidades de los cuerpos
        Moon.Update(dt);
        Earth.Update(dt);
        Sun.Update(dt);

        // Establecer la posición de la pantalla centrada en el Sol
        scr.Position(Sun.pos.x, Sun.pos.y);

        // Ploteo de los cuerpos en la pantalla
        Plot(Moon, scr);
        Plot(Earth, scr);
        Plot(Sun, scr);

        scr.Draw();  // Dibujar la pantalla actualizada
    }

    return 0;
}