**Atomic Sandbox (SFML 3.0.3)**

An interactive atomic simulation built using SFML 3.0.3.
You can construct elements by firing protons, neutrons, and electrons at a nucleus and observe how atomic structure changes in real time.

**Controls:**
P	 - Fire a Proton
N	 - Fire a Neutron
E	 - Fire an Electron

Particles collide with the nucleus and modify the atom accordingly.

**Features:**
**1. Dynamic Element Detection:**
The element automatically updates based on proton count using an internal periodic table.

**2. Display format:**
ElementName-MassNumber (Symbol)
Example: Carbon-12 (C)

**3. Isotope Support:**
Mass Number = Protons + Neutrons
Adding neutrons creates different isotopes of the same element.

**4.Nuclear Stability System**
The nucleus has three stability levels:
Level	Indicator:
Stable -	Green ring
Unstable	- Yellow ring
Highly Unstable -	Red ring

**Rules:**
For light elements (P ≤ 20): stable if |N - P| ≤ 1
For heavier elements: stable if P ≤ N ≤ 1.5P
Highly unstable atoms may randomly eject electrons.

**5.Electron Shell System**
Electrons populate shells with fixed capacities:
Shell	Capacity:
K - 2
L -	8
M -	18
N -	32
Electrons orbit the nucleus with animated rotation speeds.

**6.Ion Charge Visualization**
Charge is calculated as:
Charge = Protons - Electrons
Orbit ring color indicates ion state:
Charge	Color
Neutral (0)	Soft Blue
+1	Red
+2	Cyan
**Electron Physics:**
Excess electrons are repelled from negatively charged atoms.
Ejected electrons fade over time.
Velocity is capped to maintain simulation stability

**Requirements**
*C++17 or newer
*SFML 3.0.3
*Font file located at:
  src/pfont.ttf
***Example Build Command (macOS)[Important]**
  g++ -std=c++17 -I/opt/homebrew/opt/sfml/include src/main.cpp -o bin/main \
  -L/opt/homebrew/opt/sfml/lib \
  -lsfml-graphics -lsfml-window -lsfml-system
Adjust include and library paths for your platform.

**Project Structure:**
AtomicSandbox/
│
├── src/
│   ├── main.cpp
│   └── pfont.ttf
│
├── bin/
│   └── main
│
└── makefile

**Technical Overview:**
**Atom Class:**
1.Proton / neutron / electron counts
2.Stability logic
3.Shell configuration
4.Electron animation
5.Ejected electron behavior
6.Element lookup

**Projectile Class:**
1.Particle type
2.Movement physics
3.Collision detection

**Main Loop:**
1.Process input
2.Spawn particles
3.Update physics
4.Detect collisions
5.Render atom and projectiles

**Educational Concepts:**
This project demonstrates:
1.Atomic number defines the element
2.Neutrons determine isotopes
3.Electron imbalance creates ions
4.Stability depends on neutron-proton ratio
5.Shell capacity limits
