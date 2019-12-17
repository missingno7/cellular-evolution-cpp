# cellular-evolution
2D cellular evolution framework.
Code includes examples:
Travelling salesman
Floorplannig

## Getting Started
Configuration for travelling salesman example is stored in cfg/cities_config.txt
```
xpopsize 512
ypopsize 512
crossrate 0.0
mutamount 5
mutprob 0.005
shiftprob 0.2
revprob 0.2
gennew true
drawpop true
cities 50
scwidth 200
scheight 200
threads 8
```

## Visualisation
Image of population can be generated by setting parameter drawpop in config.txt to true.
Generated image size is population_width x population_height, where each pixel corresponds to one individual. Lightness of each pixel/individual is based on fitness function and genetically similar individuals have similar colors.
![example_output](https://github.com/missingno7/project-images/blob/master/genalg_individuals.png)

# salesman-example
Travelling salesman solver using cellular evolutionary algorithm

### Example output
![example_output](https://github.com/missingno7/project-images/blob/master/evo_salesman_example.png)

# floor-planning-example
Program that creates random 2D rectangles and tries to minimize surface of their bounding box by cellular evolution algorithm.

### Example output
![example_output](https://github.com/missingno7/project-images/blob/master/IGEN15257.png)

