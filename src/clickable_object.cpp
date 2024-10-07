#include "clickable_object.h"

ClickableObject::ClickableObject(glm::vec2 posn, glm::vec2 size, unsigned int function) {
    CenterPosition = posn;
    OriginalSize = size;
    id = function;
    clicked = false;
    changeScale(1.0f);
}

ClickableObject::~ClickableObject() {}

bool ClickableObject::isClicked(float x, float y) {
    return (x < Position.x + Size.x && x > Position.x && y < Position.y + Size.y && y > Position.y);
}

void ClickableObject::changeScale(float scale){
    Size = OriginalSize*scale;
    Position = CenterPosition - (Size*0.5f);
}


