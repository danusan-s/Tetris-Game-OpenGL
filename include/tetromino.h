#include <vector>
#include <glm/glm.hpp> // For vec2

enum class TetrominoType { I, O, T, S, Z, J, L };
enum class Rotation { DEG_0, DEG_90, DEG_180, DEG_270 };

class Tetromino {
public:
    TetrominoType Type;
    Rotation RotationState;
    glm::vec2 Position; // Current position on the grid (x, y)
    std::vector<std::vector<glm::ivec2>> ShapeOffsets; // Offsets for each rotation

    Tetromino(TetrominoType type);
    
    void MoveLeft();
    void MoveRight();
    void MoveDown();
    void Rotate();
    std::vector<glm::ivec2> GetCurrentShape(); // Get block positions based on rotation

private:
    void InitShapeOffsets(); // Initialize offsets for the Tetromino shapes
};
