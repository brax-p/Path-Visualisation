# Path-Visualisation
An interactive application that allows visualization of Breadth-First-Search and Depth-First-Search algorithms. <br />

Walls can be added and removed, the spawn and goal nodes can be rearranged. <br />

There are two modes: first, is automatic where the a single path is shown; second, is simulation mode where the algorithm and its steps are shown. <br />

This application follows a paradigm similar to the Model-View-Controller system. <br /> 

Implemented with SFML.

![alt text](https://github.com/brax-p/Path-Visualisation/blob/master/src/gifs/Path-Visualization_Demo.gif "Demo")

## How to Run:
For Ubuntu-based linux, run:

```bash
sudo apt-get install libsfml-dev
```

Clone the repo

In the root directory, run:

```bash
make && ./main
```

## Controls
<li> W: Activate wall placement. Left click to place, right click to remove.
<li> S: Activate spawn movement. Left click on any non-wall/goal tile to move the spawn.
<li> G: Activate goal movement. Left click on any non-wall/spawn tile to move the goal.

Enjoy :D.

## Resources
SFML can be found [here](https://www.sfml-dev.org/).
