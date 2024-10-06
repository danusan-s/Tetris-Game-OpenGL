#include <glm/glm.hpp> // For vec2
#include <vector>

enum class TetrominoType { I, O, T, S, Z, J, L };
enum class TetrominoColor { RED, GREEN, BLUE };
enum class Rotation { DEG_0, DEG_90, DEG_180, DEG_270 };

class Tetromino {
   public:
    TetrominoType Type;
    TetrominoColor Color;
    Rotation RotationState;
    glm::vec2 Position;                                // Current position on the grid (x, y)
    std::vector<std::vector<glm::ivec2>> ShapeOffsets; // Offsets for each rotation

    Tetromino(TetrominoType type, TetrominoColor color);

    void MoveLeft();
    void MoveRight();
    void MoveDown();
    void MoveUp();
    void RotateClockwise();
    void RotateAntiClockwise();
    std::vector<glm::ivec2> GetCurrentShape(); // Get block positions based on rotation

   private:
    void InitShapeOffsets(); // Initialize offsets for the Tetromino shapes
};
