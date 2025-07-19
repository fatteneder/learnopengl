#version 330 core

in vec4 ourColor;

out vec4 FragColor;

void main()
{
  // Why is the bottom left side of our triangle black?
  // Coords of bottom left are (-0.5, -0.5, 0.0, 1.0), which I assume gets clipped to
  // (0, 0, 0, 1) which is black.
  // This is consistent with the bottom right corner being red for which
  // (0.5, -0.5, 0.0, 1.0) would get clipped to (0.5, 0, 0, 1) and the first part is red in RGBA.
  // Similarly for the top corner being green.
  FragColor = ourColor;
}
