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

- GLSL: (also see my VulkanTutorial notes)
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

# Chapter 7 - Textures

- Texture 2D coords are in `[0,1]x[0,1]`, where `(0,0)` is lower left and `(1,1)` is upper right.
  Texture coords are usually denoted as `s,t` (and `r` in 3D).

- What happens for coords out of range?
  Default OpenGL behavior is to repeat the image using only the decimal part of the coorinate
  (e.g. periodic bdry conditions).
  Available options:
  - `GL_REPEAT`
  - `GL_MIRRORED_REPEAT`, mirrors image with each repeat
  - `GL_CLAMP_TO_EDGE`, results in stretched image for out-of-bounds
  - `GL_CLAMP_TO_BORDER`, coords out-of-bounds get user-specified color

- Texture coords are independent of resolution.
  That's done by OpenGL figuring out how to map texture pixels (texels) to texture coords.

- Mimaps are a collection of the same texture sampled at different resolutions.
  Used to save memory and compute and to avoid artifacts when sampling textures for
  objects that are close and far from the viewport.
  OpenGL can create Mipmaps for us once we created a texture.

- GLSL has builtin types `sampler2D, sampler3D` which together with `texture()` can
  be used to sample textures.
  Samples are declared as `uniform`.
  One can assign multiple texture samples (called a texture unit) to a shader
  using `layout = ...`.
  OpenGL guarantees at least 16 texture units for a shader.

# Chapter 8 - Transformations

- Mentions homogeneous coordinates, see https://en.wikipedia.org/wiki/Homogeneous_coordinates

- Use `libglm` for lin alg on CPU side/non-shader code.

- GLM and GLSL both use column-major layout for matrices.

# Chapter 9 - Coordinate Systems

- 5 important coordinate spaces:
  - local space (object space)
  - world space
  - view space (eye space)
  - clip space
  - screen space

- Typical vertex coordinate transformation goes like:
  local space as local coords -> world coords -> view coords -> clip coords -> screen coords
  - local coords: coords relative to origin where the object begins
  - world coords: placement object somewhere in the world
    world coords = model matrix * local coords
  - view coords: object as seen from the camera or viewer's point of view
    view coords = view matrix * world coords
  - clip coords: map view coords to `[-1,1]x[-1,1]` and then apply clipping
    clip coords = projection matrix * view coords
    Note: if one or two vertices of a triangle would be clipped away, OpenGL still considers them
    to render a complete triangle
    A projection matrix creates a kind of viewing box which is called frustum. Everything
    inside the frustum ends up on the screen.
  - screen coords: map clip coords to dimensions of `glViewport`
    This step is done automatically by OpenGL.


- Two types of projection matrices:
  orthographic (https://en.wikipedia.org/wiki/Orthographic_projection)
  vs perspective (https://en.wikipedia.org/wiki/3D_projection#Perspective_projection)
  - For orthographic use `glm:ortho`
  - For perspective use `glm:perspective`
  See https://www.songho.ca/opengl/gl_projectionmatrix.html for derivation of both matrices.

- OpenGL coord system is right-handed.
  Book uses the convention that `x` points right and `y` to the top, hence, `z` outwards of the screen.

- GLFW provides a depth buffer we can use for depth testing.
  Depth testing is automatically done by OpenGL, but its disabled by default.
