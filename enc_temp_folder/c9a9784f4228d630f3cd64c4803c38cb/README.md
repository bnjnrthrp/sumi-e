# sumi-e

This is a demo project to make sumi-e paintings using the mouse and keyboard.

# How is works

When you start the program, press "new stroke" to open the canvas and begin your first brush stroke.
Click the left mouse to add a control point, right click the mouse to undo the control point. A control point
is a "snapshot" of the brush's state when you clicked the mouse. The only thing you cannot change mid-stroke
is the number of bristles in a brush.

Currently working: Brush size, pressure, color (all together)
Not working: angle, color by location (tip, mid, base), 

# Dependencies

If you're having trouble compiling, you can try two options:

1. add the include folder into the project properties->VC++ Directories->Include Directories
and add "./include"

Additionally, under Source Directories, add "./sumi-e/lib/"

Finally, under Linker->General->Additional Library Directories add "./lib"

Under Linker->Input->Additional Dependencies add "glfw3.lib;opengl32.lib;"

2. add "./include/{dependency}" in front of every #include under "Dependency file headers" i.e. #include "./include/glad/glad.h"
Add "./lib/{header}" in front of every #include under "local program header files" i.e. #include "./lib/ShaderProgram.h"

