# Ecosystem
This is interactive cells ecosystem simulation.

### Concept
This is implementation of the cells genetic algorithm with natural selection in a virtual world. The world is a 2D grid of cells closed along the x axis. 

### World rules
The world is somewhat like a cross-section of a small reservoir, like lake or jar with watter.
- World 2D
- World closed along the x axis
- There are live cells. They can:
  - born (in several ways)
  - die of lack of food, after death, the cell turns into organic matter
  - die from old age, after death, the cell turns into organic
  - eat food. Food for cells can be:
    - light
    - organic
    - another live cell
  - be eaten by another cell
  - actions, diet and properties of the cell are determined by the genome
  - the genome of the cell can mutate, or it can remain the same
- Organic has a gravity and fall down to the bottom
- Light falls from above and with each passed cell its power becomes less and less

### Dependencies
SFML 2.6.1+
TGUI

##Install dependencies with vcpkg:

Install vcpkg:
```
git clone https://github.com/microsoft/vcpkg
.\vcpkg\bootstrap-vcpkg.bat
```
Install dependencies:
```
.\vcpkg\vcpkg install sfml:x64-windows
.\vcpkg\vcpkg install tgui:x64-windows
```

### Benchmarks
100x51 world, 4000 life entities
Render max time: 199431 ms
Simulation tick update max time: 1020 ms

### TODO:
- [ ] Decompose GUI from data
- [x] Use new GUI libraries
- [x] Use vpkg for manage dependencies
- [ ] Decompose genetic algorithm from it's special implementation
