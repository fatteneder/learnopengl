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

# Chapter 6 - Shaders

- Inputs to vertex shaders are called vertex attributes
- There is a max number of vertex attributes we are allowed to declare and the limit is set by hardware.
  OpenGL guarantees that there are at least 16 4-component vertex attributes available.
- GLSL allows for swizzling: `vec4 otherVec = someVec.xxxx + anotherVec.yxzy`. That's cool!
  Q: How does swizzling deal with mutation when a vector appears on both sides?

- The vertex shader *should* receive some input to do any work.
  The input it receives comes directly from the vertex data that we set up in the C program.
  To this end we have to apply the `(layout = 0)` qualifier.
  FFIW one can also use `glGetAttribLocation` to query the qualifier and then set it ...

- The fragment shader requires its output to be a vec4 color output!

- Using the same name for the output in the vertex shader and the input in the fragment shader
  makes OpenGL link those variables when compiling the shader program.

- Uniform is another way to send data from CPU to GPU.
  Uniforms are global, i.e. it is unique per shader program, and it can be accessed from any shader
  at any stage in the shader program.
  Uniforms will keep their value unless they are reset or updated.
  Because they are global, we only have to declare them either in the vertex or fragment shader.
  > Note: Unused uniforms can be omitted by the GLSL compiler, which can cause nasty bugs!!!

- We *have* to query uniform locations in shader programs to be able to set them.
  We don't have to `glUseProgram` for querying their location.
  BUt we *have* to `glUseProgram` it before setting the uniform value.
