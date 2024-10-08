#ifndef CLICKABLEOBJECT_H
#define CLICKABLEOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

class ClickableObject{
    private:
        glm::vec2 centerPosition,originalSize;
        unsigned int id;
    public:
        glm::vec2 Position, Size;
        bool Clicked;
        ClickableObject(glm::vec2 posn,glm::vec2 size,unsigned int function);
        ~ClickableObject();
        bool CheckClickPos(float x,float y);
        void ChangeScale(float scale);
};
#endif

