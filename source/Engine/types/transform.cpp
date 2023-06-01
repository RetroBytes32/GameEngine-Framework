#include "transform.h"


Transform::Transform() : 
    
    position(glm::vec3(0, 0, 0)),
    orientation(glm::quat(0, 0, 0, 1)),
    scale(glm::vec3(1, 1, 1)),
    
    matrix(glm::mat4(1))
{
}

void Transform::SetPosition(float x, float y, float z) {
    position = glm::vec3(x, y, z);
    UpdateMatrix();
    return;
}

void Transform::SetPosition(glm::vec3 newPosition) {
    position = glm::vec3(newPosition.x, newPosition.y, newPosition.z);
    UpdateMatrix();
    return;
}

void Transform::SetOrientation(float w, float x, float y, float z) {
    orientation = glm::quat(w, x, y, z);
    UpdateMatrix();
    return;
}

void Transform::SetOrientation(glm::quat newRotation) {
    orientation = newRotation;
    UpdateMatrix();
    return;
}

void Transform::SetScale(float x, float y, float z) {
    scale = glm::vec3(x, y, z);
    UpdateMatrix();
    return;
}

void Transform::SetScale(glm::vec3 newScale) {
    scale = glm::vec3(newScale.x, newScale.y, newScale.z);
    UpdateMatrix();
    return;
}

glm::vec3 Transform::GetPosition(void) {
    return position;
}

glm::quat Transform::GetRotation(void) {
    return orientation;
}

glm::vec3 Transform::GetScale(void){
    return scale;
}

void Transform::Translate(glm::vec3 translation) {
    matrix = glm::translate(matrix, translation);
}

void Transform::Translate(float x, float y, float z) {
    matrix = glm::translate(matrix, glm::vec3(x, y, z));
}

void Transform::RotateAxis(float angle, glm::vec3 axis) {
    matrix = glm::rotate(matrix, glm::radians(angle), glm::normalize(axis));
}

void Transform::RotateEuler(glm::vec3 eulerAngle) {
    orientation = glm::quat(glm::radians(eulerAngle));
    UpdateMatrix();
}

void Transform::RotateEuler(float yaw, float pitch, float roll) {
    orientation = glm::quat( glm::radians( glm::vec3(yaw, pitch, roll) ) );
    UpdateMatrix();
}

void Transform::Scale(glm::vec3 scaler) {
    matrix = glm::scale(matrix, scaler);
}

void Transform::Scale(float x, float y, float z) {
    matrix = glm::scale(matrix, glm::vec3(x, y, z) );
}

Transform Transform::Identity(void) {
    Transform identity = Transform();
    return identity;
}

void Transform::UpdateMatrix(void) {
    
    glm::mat4 modelTranslation = glm::translate(glm::mat4(1.0f), glm::vec3( position.x, position.y, position.z ));
    glm::mat4 modelRotation = glm::toMat4(orientation);
    glm::mat4 modelScale = glm::scale(glm::mat4(1.0f), glm::vec3( scale.x, scale.y, scale.z ));
    
    matrix = modelTranslation * modelRotation * modelScale;
    return;
}
