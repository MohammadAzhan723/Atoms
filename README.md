**Atomic Sandbox (SFML 3.0.3)**

An interactive atomic simulation built using SFML 3.0.3.
You can construct elements by firing protons, neutrons, and electrons at a nucleus and observe how atomic structure changes in real time.

Controls
Key	Action
P	Fire a Proton
N	Fire a Neutron
E	Fire an Electron

Particles collide with the nucleus and modify the atom accordingly.

Features
Dynamic Element Detection

The element automatically updates based on proton count using an internal periodic table.

Display format:

ElementName-MassNumber (Symbol)
Example: Carbon-12 (C)
Isotope Support

Mass number is calculated as:

Mass Number = Protons + Neutrons

Adding neutrons creates different isotopes of the same element.

Nuclear Stability System ⚛️

The nucleus has three stability levels:

Level	Indicator
Stable	Green ring
Moderately Unstable	Yellow ring
Highly Unstable	Red ring

Rules:

For light elements (P ≤ 20): stable if |N - P| ≤ 1

For heavier elements: stable if P ≤ N ≤ 1.5P

Highly unstable atoms may randomly eject electrons.

Electron Shell System

Electrons populate shells with fixed capacities:

Shell	Capacity
1	2
2	8
3	18
4	32

Electrons orbit the nucleus with animated rotation speeds.

Ion Charge Visualization

Charge is calculated as:

Charge = Protons - Electrons

Orbit ring color indicates ion state:

Charge	Color
Neutral (0)	Soft Blue
+1	Red
+2	Cyan
Electron Physics 🔬

Excess electrons are repelled from negatively charged atoms.

Ejected electrons fade over time.

Velocity is capped to maintain simulation stability.

Requirements

C++17 or newer

SFML 3.0.3

Font file located at:

src/pfont.ttf
Example Build Command (macOS)
g++ -std=c++17 -I/opt/homebrew/opt/sfml/include src/main.cpp -o bin/main \
-L/opt/homebrew/opt/sfml/lib \
-lsfml-graphics -lsfml-window -lsfml-system

Adjust include and library paths for your platform.

Project Structure
AtomicSandbox/
│
├── src/
│   ├── main.cpp
│   └── pfont.ttf
│
├── bin/
│   └── main
│
└── README.md
Technical Overview
Atom Class

Handles:

Proton / neutron / electron counts

Stability logic

Shell configuration

Electron animation

Ejected electron behavior

Element lookup

Projectile Class

Handles:

Particle type

Movement physics

Collision detection

Main Loop

Process input

Spawn particles

Update physics

Detect collisions

Render atom and projectiles

Educational Concepts

This project demonstrates:

Atomic number defines the element

Neutrons determine isotopes

Electron imbalance creates ions

Stability depends on neutron-proton ratio

Shell capacity limits
