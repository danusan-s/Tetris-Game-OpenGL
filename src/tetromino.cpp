#include "tetromino.h"

// Constructor
Tetromino::Tetromino(TetrominoType type)
    : Type(type), RotationState(Rotation::DEG_0), Position(4, 22) {
    InitShapeOffsets();
}

// Move methods
void Tetromino::MoveLeft() {
    --Position.x;
}
void Tetromino::MoveRight() {
    ++Position.x;
}
void Tetromino::MoveDown() {
    --Position.y;
}

void Tetromino::MoveUp() {
    ++Position.y;
}

void Tetromino::RotateClockwise() {
    RotationState =
        static_cast<Rotation>((static_cast<int>(RotationState) - 1) >= 0 ? static_cast<int>(RotationState) - 1 : 3);
}

void Tetromino::RotateAntiClockwise() {
    RotationState = static_cast<Rotation>((static_cast<int>(RotationState) + 1) % 4);
}

// Get the block positions relative to the current position and rotation
std::vector<glm::ivec2> Tetromino::GetCurrentShape() {
    return ShapeOffsets[static_cast<int>(RotationState)];
}

// Initialize the shape offsets for each Tetromino
void Tetromino::InitShapeOffsets() {
    // Block data generated using chatgpt cuz im lazy
    switch (Type) {
        case TetrominoType::I:
            ShapeOffsets = {
                {{0, 0}, {1, 0}, {2, 0}, {3, 0}}, // 0° rotation
                {{1, 0}, {1, 1}, {1, 2}, {1, 3}}, // 90° rotation
                {{0, 1}, {1, 1}, {2, 1}, {3, 1}}, // 180° rotation
                {{2, 0}, {2, 1}, {2, 2}, {2, 3}}  // 270° rotation
            };
            break;
        case TetrominoType::O:
            ShapeOffsets = {
                {{1, 0}, {2, 0}, {1, 1}, {2, 1}}, // 0° rotation (O is symmetric, all rotations are the same)
                {{1, 0}, {2, 0}, {1, 1}, {2, 1}}, // 90° rotation
                {{1, 0}, {2, 0}, {1, 1}, {2, 1}}, // 180° rotation
                {{1, 0}, {2, 0}, {1, 1}, {2, 1}}  // 270° rotation
            };
            break;
        case TetrominoType::T:
            ShapeOffsets = {
                {{0, 0}, {1, 0}, {2, 0}, {1, 1}}, // 0° rotation
                {{1, 0}, {1, 1}, {1, 2}, {0, 1}}, // 90° rotation
                {{0, 1}, {1, 1}, {2, 1}, {1, 0}}, // 180° rotation
                {{1, 0}, {1, 1}, {1, 2}, {2, 1}}  // 270° rotation
            };
            break;
        case TetrominoType::S:
            ShapeOffsets = {
                {{1, 0}, {2, 0}, {0, 1}, {1, 1}}, // 0° rotation
                {{1, 0}, {1, 1}, {2, 1}, {2, 2}}, // 90° rotation
                {{1, 1}, {2, 1}, {0, 2}, {1, 2}}, // 180° rotation
                {{0, 0}, {0, 1}, {1, 1}, {1, 2}}  // 270° rotation
            };
            break;
        case TetrominoType::Z:
            ShapeOffsets = {
                {{0, 0}, {1, 0}, {1, 1}, {2, 1}}, // 0° rotation
                {{1, 0}, {1, 1}, {0, 1}, {0, 2}}, // 90° rotation
                {{0, 1}, {1, 1}, {1, 2}, {2, 2}}, // 180° rotation
                {{2, 0}, {2, 1}, {1, 1}, {1, 2}}  // 270° rotation
            };
            break;
        case TetrominoType::J:
            ShapeOffsets = {
                {{0, 0}, {0, 1}, {1, 1}, {2, 1}}, // 0° rotation
                {{1, 0}, {2, 0}, {1, 1}, {1, 2}}, // 90° rotation
                {{0, 1}, {1, 1}, {2, 1}, {2, 2}}, // 180° rotation
                {{1, 0}, {1, 1}, {0, 2}, {1, 2}}  // 270° rotation
            };
            break;
        case TetrominoType::L:
            ShapeOffsets = {
                {{2, 0}, {0, 1}, {1, 1}, {2, 1}}, // 0° rotation
                {{1, 0}, {1, 1}, {1, 2}, {2, 2}}, // 90° rotation
                {{0, 1}, {1, 1}, {2, 1}, {0, 2}}, // 180° rotation
                {{0, 0}, {1, 0}, {1, 1}, {1, 2}}  // 270° rotation
            };
            break;
        default:
            // Default to no offsets in case of an invalid type
            ShapeOffsets = {{}};
            break;
    }
}
