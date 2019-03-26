# Simulation-Stencil
Starter code for CS 224 Assignment 4

This starter code provides a simple 3D viewer for tetrahedral mesh simulations.
As given, it will load up and visualize a single tetrahedron.
Your job is to modify the code to load arbitrary meshes (e.g. the ones in `/example-meshes`), extract their surface meshes for visualization, and to compute simulation time step updates to the vertex positions of the mesh.

You'll want to look at `src/simulation.cpp` to get started, as that's the only file you need to change (although you'll probably make several of your own new files, too).
You also might want to look at `src/view.cpp`, if you're interested in adding new interactivity/controls to the program.

Speaking of controls: the controls offered by the starter code are:
 * Move Camera: WASD
 * Look around: Click and hold mouse and drag
 * Toggle between displaying the surface mesh and a wireframe of the full tet mesh: T

When the program first loads, you should see a ground plane and a single tet floating in space, against a gray background.
If the tet does not display: check the console output. Most likely the .mesh file failed to load because the file couldn't be found. You'll need to set the working directory in Qt Creator to be the root directory of this repository. To do that, select "Projects" on the left-hand sidebar in Qt Creator, select "Run" under the "Build & Run options", and enter the path to the repo root in the "Working directory" field.
