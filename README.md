## HyperAdrenaline's Hyperengine

Though reusable, this was created for a University Project that you can find here:

- Itch.io
- https://hard-bytes.itch.io/hyper-adrenaline

- Windows 
- https://archive.org/details/hyper-adrenaline
- Linux
- https://archive.org/details/hyper-adrenaline-linux
- Twitter
- https://twitter.com/Hard_Bytes

# OpenGl engine

This is an OpenGL-based game engine build from scratch.

It uses some technologies like:
- SOIL, for image loading
- Assimp, for model loading
- Bullet, for physics 
- ImGui, for 2D menu graphics
- etc.

All credits go to their respective contributors.

# How it works

This engine uses a superclass called Hyperengine to manage the window, input and scene nodes. 
Scene nodes can be models, cameras, lights, skyboxes, etc.
Create a Hyperengine class, then build the typical while loop that checks window's display
and go from there.

There is included a main.cpp for testing purposes, though will not have my test assets, try using a .obj and loading it or some other things :).

If you want to use this as a library, you will need all other dependencies to be linked. Also remember to give proper credits.

Shaders are build-in, as this is a university project.

If this has helped you, awesome.
