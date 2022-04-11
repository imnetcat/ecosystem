# ecosystem
Interactive virtual ecosystem. Cells evolution. Evolution from unicellular to multicellular organisms in a virtual world

### Concept
This is implementation of the genetic algorithm with natural selection in a virtual world. The world is a 2D grid of cells closed along the x axis. 

### World rules
The world is somewhat like a cross-section of a small reservoir.
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

### Benchmarks
World update max time:  1870 ms

### TODO:
- Decompose GUI from data
- Use new GUI libraries
- Use vpkg for manage dependencies
- Decompose genetic algorithm from it's special implementation
