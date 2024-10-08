#include "clickable_object.h"

ClickableObject::ClickableObject(glm::vec2 posn, glm::vec2 size, unsigned int function) {
    centerPosition = posn;
    originalSize = size;
    id = function;
    Clicked = false;
    ChangeScale(1.0f);
}

ClickableObject::~ClickableObject() {}

bool ClickableObject::CheckClickPos(float x, float y) {
    return (x < Position.x + Size.x && x > Position.x && y < Position.y + Size.y && y > Position.y);
}

void ClickableObject::ChangeScale(float scale){
    Size = originalSize*scale;
    Position = centerPosition - (Size*0.5f);
}


