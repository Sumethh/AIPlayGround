#include "Camera.h"
#include "Renderer2D.h"
Camera::Camera() :
  m_mainCamera( false)
{
}

Camera::~Camera()
{
}

void Camera::Update()
{
  Renderer2D::GI()->GetStaticRenderer().SetCameraPos( m_pos );
}