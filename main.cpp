#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cmath>
#include <set>
#include <algorithm>

using namespace sf;
using namespace std;

//Data
struct ElementData{
    int proton;
    string name;
    string symbol;
    string type;
    Color color;
 
};

struct UnlockPopup{
    Text text;
    float life;
    Vector2f position;
     UnlockPopup(const sf::Font& font)
        : text(font), life(0), position(0, 0)
    {
    }
};

set<int> discoveredElements;
vector<UnlockPopup> unlockPopups;

//Periodic Table
const vector<ElementData> PeriodicTable = {
    {0, "Unknown", "n", "Unknown", Color(128, 128, 128)},
    {1, "Hydrogen", "H", "Elementary",Color::Blue},
    {2, "Helium", "He", "Noble-Gas",Color::Magenta},
    {3, "Lithium", "Li", "Alkali",Color::Yellow},
    {4, "Beryllium", "Be", "Alkaline",Color(255, 165, 0)},
    {5, "Boron", "B", "Metalloid",Color(112, 128, 144)},
    {6, "Carbon", "C", "Non-Metal",Color::Red},
    {7, "Nitrogen", "N", "Non-Metal",Color::Red},
    {8, "Oxygen", "O", "Non-Metal",Color::Red},
    {9, "Flourine", "F", "Halogen",Color::Green},
    {10, "Neon", "Ne", "Noble-Gas",Color::Magenta},
    {11, "Sodium", "Na", "Alkali",Color::Yellow},
    {12, "Magnesium", "Mg", "Alkaline",Color(255, 165, 0)},
    {13, "Aluminium", "Al", "Other",Color(71, 74, 81)},
    {14, "Silicon", "Si", "Non-Metal",Color::Red},
    {15, "Phosphorus", "P", "Non-Metal",Color::Red},
    {16, "Sulfur", "S", "Non-Metal",Color::Red},
    {17, "Chlorine", "Cl", "Halogen",Color::Green},
    {18, "Argon", "Ar", "Noble-Gas",Color::Magenta},
    {19, "Potassium", "K", "Alkali",Color::Yellow},
    {20, "Calcium", "Ca", "Alkaline",Color(255, 165, 0)},
    {26, "Iron", "Fe", "Transition",Color::Cyan},
    {29, "Copper", "Cu", "Transition",Color::Cyan},
    {47, "Silver", "Ag", "Transition",Color::Cyan},
    {79, "Gold", "Au", "Transition",Color(255, 215, 0)},
    {82, "Lead", "Pb", "Other",Color(71, 74, 81)},
    {92, "Uranium", "U",  "Actinide",Color(0, 255, 0)},
    {94, "Plutonium", "Pu", "Actinide",Color(50, 200, 50)}
};
//Bread and Butter
class Atom{
    int protons;
    int neutrons;
    int electrons;
    Vector2f center;

    CircleShape nucleus;
    CircleShape orbit;
    CircleShape stabilityRing;
    Text nameLabel;

    float electronRepel = 30000000.0;

    struct Shell{
        float radius;
        int maxCap;
    };

    struct EjectedE{
        CircleShape eject;
        Vector2f velocity;
        float life;
    };
    
    vector<EjectedE> ejected;

    vector<Shell> shellConfigs = {
            {80.0, 2},
            {130.0, 8},
            {180.0, 18},
            {230.0, 32}
    };

    vector<CircleShape> electron;
    float rotationSpeed = 1.2;

public:
    bool isStable(){
        int N = neutrons;
        int P = protons;
        if(P <= 20){
            return abs(N-P) <= 1;
        } else{
            return N >= P && N <= (int)(1.5 * P);
        }
    }

    int instabilityLevel(){
        int N = neutrons;
        int P = protons;
        int diff = abs(N-P);
        if(P <= 20){
            if(diff <= 1){
                return 0;
            } else if(diff <= 3){
                return 1;
            } else{
                return 2;
            }
        } else {
        if (N >= P && N <= (int)(1.5f * P)){ 
            return 0;
        }
        if (N >= P - 5 && N <= (int)(1.6f * P)){ 
            return 1;
        } else{
        return 2;
        }
    }
    }
    bool checkDiscovery(){
        ElementData data = getElementByProtons(protons);
        if(data.proton == 0){
            return false;
        } 
        if(!isStable()){
            return false;
        }
        int charge = protons - electrons;
        if(abs(charge) > 2){
            return false;
        }
        return true;
    }

        //Helper function to get element data by proton number
        static ElementData getElementByProtons(int protonNumber) {
            for (const auto& elem : PeriodicTable) {
                if (elem.proton == protonNumber) {
                    return elem;
                }
            }
            return PeriodicTable[0];
        }

    void refresh() {
    ElementData data = getElementByProtons(protons);
    int massNumber = protons + neutrons;

    if (discoveredElements.count(protons)) {
        nameLabel.setString(
            data.name + "-" + std::to_string(massNumber) +
            " (" + data.symbol + ")"
        );
    } else {
        nameLabel.setString("Unknown-" + std::to_string(massNumber));
    }

    

    int level = instabilityLevel();
    if(level == 0){
        stabilityRing.setOutlineColor(Color::Green);
    } else if(level == 1){
        stabilityRing.setOutlineColor(Color::Yellow);
    } else if(level == 2){
        stabilityRing.setOutlineColor(Color::Red);
    }
    int charge = protons - electrons;

    if(charge == 0){
        orbit.setOutlineColor(Color(100,100,150));
    } if(charge == 1){
        orbit.setOutlineColor(Color::Red);
    } if(charge == 2){
        orbit.setOutlineColor(Color::Cyan);
    }

    electron.clear();
    int remaining = electrons;
    for (const auto& shell : shellConfigs) {
            int inThisShell = std::min(remaining, shell.maxCap);
            for (int i = 0; i < inThisShell; i++) {
                CircleShape e(4.5f);
                e.setFillColor(Color::Yellow);
                e.setOrigin({4.5f, 4.5f});
                electron.push_back(e);
            }
            remaining -= inThisShell;
            if (remaining <= 0){
                break;
            }
    }
    nucleus.setFillColor(data.color);

    // Recenter text
    sf::Rect<float> bounds = nameLabel.getLocalBounds();
    nameLabel.setOrigin({
        bounds.position.x + bounds.size.x / 2.0f,
        bounds.position.y + bounds.size.y / 2.0f
    });
    
}

    static Font& getFont(){
        static Font font;
        static bool loaded = false;
        if(!loaded){
            if(!font.openFromFile("src/pfont.ttf")){
            }
            loaded = true;
        }
        return font;
    }
    Atom(int p, int n, int e, Vector2f ctr)
        : protons(p), neutrons(n), electrons(e), center(ctr),nameLabel(getFont())
    {
        nucleus.setRadius(30.f);
        nucleus.setOrigin({30.f, 30.f});
        nucleus.setPosition(center);

        orbit.setRadius(120.f);
        orbit.setOrigin({120.f, 120.f});
        orbit.setPosition(center);
        orbit.setFillColor(Color::Transparent);
        orbit.setOutlineThickness(1.5f);
        orbit.setOutlineColor(Color(100, 100, 100, 150));

        nameLabel.setFillColor(Color::White);
        nameLabel.setPosition({center.x, 50.f});

        stabilityRing.setRadius(40.f);
        stabilityRing.setOrigin({40.f, 40.f});
        stabilityRing.setPosition(center);
        stabilityRing.setFillColor(sf::Color::Transparent);
        stabilityRing.setOutlineThickness(4.f);

        refresh();
    }
    void updateE(float time){
        int eId = 0;
        int remaining = electrons;
        for(int s = 0; s < shellConfigs.size(); s++){
        int inShell = min(remaining, shellConfigs[s].maxCap);
        for(int i = 0; i < inShell ; i++){
                float speed = rotationSpeed / (s+1);
                float spacing = i * ((2 * 3.1415926535) / inShell);
                float angle = spacing + (time * speed);

                float x = center.x + shellConfigs[s].radius * cos(angle);
                float y = center.y + shellConfigs[s].radius * sin(angle);

                if(eId < electron.size()){
                    electron[eId].setPosition({x, y});
                    eId++;
                }

            }
            remaining -= inShell;
            if(remaining <= 0){
                break;
            }
        }
    }
    bool tryUnlock(){
        if(!isStable()){
            return false;
        }
        ElementData data = getElementByProtons(protons);
        if(data.proton == 0){
            return false;
        }
        int charge = protons - electrons;
        if(abs(charge) > 2){
            return false;
        }
        if(discoveredElements.count(protons)){
            return false;
        }

        discoveredElements.insert(protons);
        
        return true;
    }
    int getProtonCount() const{
        return protons;
    }
    static ElementData getElementbyProtons(int p);
    void updateEjected(float dt){
        for(int i = 0; i < ejected.size();){
            Vector2f repel = computeRepulsion(ejected[i].eject.getPosition());
            ejected[i].velocity += repel * dt;
            ejected[i].eject.move(ejected[i].velocity * dt);
            ejected[i].life -= dt;
        if(instabilityLevel() == 2){
            if((float)rand() / RAND_MAX < 0.5 * dt){
                removeElectron();
            }
        }

        // Fade out
        float alpha = max(0.f, ejected[i].life);
        Color c = ejected[i].eject.getFillColor();
        c.a = static_cast<uint8_t>(255 * alpha);
        ejected[i].eject.setFillColor(c);

        if (ejected[i].life <= 0.f) {
            ejected.erase(ejected.begin() + i);
        } else {
            ++i;
        }
        }
    }
    void addProton(){
        protons++;
        if((float)rand() / RAND_MAX < 0.2){
            removeElectron();
        }
        refresh();
    }
    void addNeutron(){
        neutrons++;
        refresh();
    }
    void addElectron(){
        electrons++;
        refresh();
        if(electrons > protons + 5){
            electrons--;
        }
    }
    void removeElectron(){
        if(electrons > 0){
        electrons--;
        }
        EjectedE ee;
        ee.eject.setRadius(3.0);
        ee.eject.setOrigin({3.0, 3.0});
        ee.eject.setFillColor(Color::Red);
        ee.eject.setPosition(center);
        float angle = static_cast<float>(rand()) / RAND_MAX * 2.0 * 3.1415926535;
        float speed = 200.0;
        ee.velocity = Vector2f(
            cos(angle) * speed,
            sin(angle) * speed
        );
        ee.life = 1.0;
        ejected.push_back(ee);
        refresh();
    }
    Vector2f computeRepulsion(Vector2f position){
            int charge = protons - electrons;
            if(charge >= 0){
                return Vector2f(0.0, 0.0);
            }
            float chargeStrength = abs(charge);

            Vector2f dir = position - center;

            float distSq = dir.x * dir.x + dir.y * dir.y;
            distSq = max(distSq, 100.f);

            float magForce = electronRepel / distSq;

            float length = sqrt(distSq);
            dir = dir / length;

            return dir * magForce;
    }
    void draw(RenderWindow& window) {
        int remaining = electrons;

    for (int s = 0; s < shellConfigs.size(); s++) { 
    int inShell = min(remaining, shellConfigs[s].maxCap);

    if (inShell > 0) {
        CircleShape ring(shellConfigs[s].radius);
        ring.setOrigin({shellConfigs[s].radius, shellConfigs[s].radius});
        ring.setPosition(center);
        ring.setFillColor(Color::Transparent);
        ring.setOutlineThickness(0.8f);
        ring.setOutlineColor(Color(255,255,255,40));
        window.draw(ring);
    }

    remaining -= inShell;
    if (remaining <= 0){
        break;
    }
    }
        for (auto& e : electron){
            window.draw(e);
        }
        for(auto& ee : ejected){
            window.draw(ee.eject);
        }
        window.draw(orbit);
        window.draw(stabilityRing);
        window.draw(nucleus);
        window.draw(nameLabel);
    }

};
//Projectiles
class Projectile{
public:
    enum Type {PROTON, NEUTRON, ELECTRON};
    CircleShape particle;
    Vector2f velocity;
    Type type;
    bool destroyed = false;
    Projectile(Vector2f start, Vector2f end, Type t){
        type = t;
        float radius = 5.0;
        if(type == PROTON){
            particle.setFillColor(Color::Green);
        } else if(type == ELECTRON){
            particle.setFillColor(Color::Red);
            radius = 3.0;
        } else if(type == NEUTRON){
            particle.setFillColor(Color::Yellow);
        }
        particle.setRadius(radius);
        particle.setOrigin({radius,radius});
        particle.setPosition(start);

        Vector2f dir = end - start;
        float length = sqrt(dir.x * dir.x + dir.y * dir.y);
        velocity = Vector2f((dir.x / length) * 400.0f, (dir.y / length) * 400.0f);
    }

    void update(float dt){
        particle.move(velocity*dt);
    }
    void draw(RenderWindow& window){
        window.draw(particle);
    }
    Vector2f getPosition() const { return particle.getPosition(); }
    Type getType() const { return type; }
    bool isDestroyed() const { return destroyed; }
    void destroy() { destroyed = true; }
};

int main() {
    RenderWindow window(VideoMode({800, 600}), "Atomic Sandbox");
    window.setFramerateLimit(120);
    Vector2f center(400.0f, 300.0f);

    Atom a1(1, 0, 1, center);
    vector<Projectile> projectiles;
    Clock clock;

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            if (event->is<Event::Closed>()) {
                window.close();
            }

            // SFML 3.0 Event handling
            if (const auto* keyPressed = event->getIf<Event::KeyPressed>()) {
                if (keyPressed->code == Keyboard::Key::P) {
                    projectiles.emplace_back(Vector2f(0.f, 300.f), center, Projectile::PROTON);
                }
                if (keyPressed->code == Keyboard::Key::N) {
                    projectiles.emplace_back(Vector2f(800.f, 300.f), center, Projectile::NEUTRON);
                }
                if (keyPressed->code == Keyboard::Key::E) {
                    projectiles.emplace_back(Vector2f(800.f, 300.f), center, Projectile::ELECTRON);
                }
            }
        }

        float dt = clock.restart().asSeconds();
        static float totalTime = 0;
        totalTime += dt; 

        for(int i = 0; i < unlockPopups.size(); ) {
            unlockPopups[i].life -= dt;
            if(unlockPopups[i].position.x > 550.0){
                unlockPopups[i].position.x -= 400.0 * dt;
            }
            // Update the text position
            unlockPopups[i].text.setPosition(unlockPopups[i].position);

            float alpha = fmax(0.f, unlockPopups[i].life / 3.f);
            Color c = unlockPopups[i].text.getFillColor();
            c.a = static_cast<uint8_t>(255.f * alpha);

            unlockPopups[i].text.setFillColor(c);
            if(unlockPopups[i].life <= 0){
                unlockPopups.erase(unlockPopups.begin() + i);
            } else {
                ++i;
            }
        }

        for (int i = 0; i < projectiles.size(); ) {
            if (projectiles[i].getType() == Projectile::ELECTRON) {
            Vector2f pos = projectiles[i].getPosition();
            Vector2f repel = a1.computeRepulsion(pos);
            projectiles[i].velocity += repel * dt;
            float maxSpeed = 600.0;
            float speed = sqrt(projectiles[i].velocity.x * projectiles[i].velocity.x + projectiles[i].velocity.y * projectiles[i].velocity.y);
            if(speed > maxSpeed){
                projectiles[i].velocity = (projectiles[i].velocity/speed) * maxSpeed;
            }
            }

            projectiles[i].update(dt);
            
            float dx = projectiles[i].getPosition().x - center.x;
            float dy = projectiles[i].getPosition().y - center.y;
            float dist = sqrt(dx * dx + dy * dy);

            // Collision check (30.0f matches your nucleus radius)
            if (dist < 30.0f) {
                if (projectiles[i].getType() == Projectile::PROTON) a1.addProton();
                if (projectiles[i].getType() == Projectile::NEUTRON) a1.addNeutron();
                if(projectiles[i].getType() == Projectile::ELECTRON) a1.addElectron();
                projectiles.erase(projectiles.begin() + i);
            } else {
                ++i;
            }
        }
        a1.updateE(totalTime);
        a1.updateEjected(dt);
        window.clear(Color(10, 10, 25));
        UnlockPopup popup(Atom::getFont());
        if (a1.tryUnlock()) {
        ElementData data = Atom::getElementByProtons(a1.getProtonCount());

        UnlockPopup popup(Atom::getFont());
        popup.text.setCharacterSize(28);
        popup.text.setFillColor(Color::White);
        popup.text.setString("Unlocked: " + data.name + " (" + data.symbol + ")");
        popup.position = Vector2f(820.f, 100.f);
        popup.life = 3.0f;

        unlockPopups.push_back(popup);
        }
        a1.draw(window);

        // Draw unlock popups
        for (auto& popup : unlockPopups) {
            window.draw(popup.text);
        }

        // Draw projectiles
        for (auto& proj : projectiles) {
            proj.draw(window);
        }

        window.display();
    }
    return 0;
}
