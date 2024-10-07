# Tetris game

A recreation of the classic tetris game using OpenGL for rendering, GLFW for window management, and FreeType for text rendering.

## How does it work ?

Works similar to regular tetris. This project simply serves to better understand basic rendering using tools like GLFW and FreeType.

[Installation Guide](GUIDE.md)

### Controls:

<ul>
  <li>A and D for moving piece left and right respectively.</li>
  <li>S for speeding up block fall rate.</li>
  <li>Q and E for Anti clockwise and clockwise rotation respectively.</li>
</ul>

### Scoring system:

50 point gain for a row clear.

If you were to clear multiple rows with one tetronimo, the score gained is (combo)*(combo+1)/2.

