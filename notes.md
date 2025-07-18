# Chapter 2 - OpenGL

- For OpenGL `<=3.3` immediate mode was the default.
immediate mode is simple to use but inefficient.
- OpenGL is a large state machine. The state is called `context`.

# Chapter 3 - Creating a Window

- Goes through project setup. Uses CMake as build system generator.
- Introduces `GLAD`, which is a GL Loader-Generator.
  Use https://glad.dav1d.de/ over https://gen.glad.sh/.

# Chapter 4 - Hello World

- Use `glxinfo -B` to get installed GL versions.
- Check out the code for this chapter https://github.com/JoeyDeVries/LearnOpenGL/blob/master/src/1.getting_started/1.1.hello_window/hello_window.cpp
  to see where to call the use `GLAD`.

# Chapter 5 - Hello Triangle

- Sketch of the graphics pipeline:
  - vertex shader (can inject own shader)
  - shape assembly
  - geometry shader (can inject own shader)
  - rasterization
  - fragment shader (can inject own shader)
  - tests and blending

- > A fragment in OpenGL refers toa ll the data required for OpenGL to render a single pixel.

- Modern OpenGL *requires* to define at least a vertex and fragment shader.

- Normalized device coordinates (NDC)
  Output of vertex shader should be coordinates in `[-1.0,1.0]`.
  Anything outside that range gets discarded.
  The point `(0,0)` is at the center, `(0,1)` is top center, `(0,1)` is right center.
  NDC coordinates are later transformed to screen-space coordinates via the viewport transform
  (cf `glViewport`). Screen-space corodinates are then transformed to fragments as inputs for
  the fragment shader.

- Use `vertex buffer object (VBO)` to send batches of data to the GPU.
- Use `vertex array object (VAO)` to 'wrap' a VBO and save yourself repeating a number of commands
needed to assign a buffer to shader.
- Use `element buffer object (EBO)` to save storage for rendering connected surfaces or similar.
Think of a rectangle. If divided into two triangles, there are six vertices and two of them
are the duplicated. You can use EBOs and remove those duplciates.
EBOs essentially contain a data array and an index array referexing the elements of the data array.

- GLSL:
  - Output of vertex shader is `vec4 gl_Position`, it is implicitly defined.
  - Output of a fragment shader is a single `vec4`, it is *not* implicitly defined.
