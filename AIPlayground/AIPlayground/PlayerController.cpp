#include "PlayerController.h"
#include "Common/Input.h"
#include "Common/log.h"
#include "World.h"
#include "Camera.h"
#include "Renderer2D.h"
#include "RendererComponent.h"
#include "SelectableGameObjectComponent.h"
#include "ColliderComponent.h"
#include "Common/imgui.h"
PlayerController::PlayerController(Grid* a_grid, Camera* a_camera) :
  m_grid(a_grid),
  m_camera(a_camera)
{
}

PlayerController::~PlayerController()
{
}

void PlayerController::Init()
{
  if (m_world)
  {

    m_selectionGO = m_world->CreateGameObject(EGameObjectType::GOT_Empty);
    if (m_selectionGO)
    {
      RendererComponent* renderComp = (RendererComponent*)m_selectionGO->AddComponent(EComponentTypes::CT_RenderComponent);
      if (renderComp)
      {
        renderComp->ChangeTextureID(1);
        renderComp->ChangeRenderType(RenderType::Dynamic);
      }
    }
  }
}

void PlayerController::Update(float a_dt)
{
  if (m_selectionGO)
  {
    if (!Input::IsMouseOverUI())
    {
      glm::vec2 mousePos = Input::GetMousePosition();
      WorldInfo worldInfo = m_world->GetWorldInfo();
      glm::vec2 newGoLoc;
      newGoLoc.x = (float)((int)(m_camera->GetPos().x + mousePos.x) / (int)worldInfo.tileSize.x);
      newGoLoc.y = (float)((int)(m_camera->GetPos().y + mousePos.y) / (int)worldInfo.tileSize.y);

      if (Input::GetMouseButton(0))
      {
        TestableCollider testCol;
        testCol.position = mousePos + m_camera->GetPos();
        testCol.radius = 8.0f;
        std::vector<GameObject*> overlappingGO = m_world->GetCollidingGameObjects(&testCol);
        if (overlappingGO.size())
        {
          for (uint i = 0; i < overlappingGO.size(); i++)
          {
            SelectableGameObjectComponent* selectableComponent = (SelectableGameObjectComponent*)overlappingGO[i]->GetComponentOfType(EComponentTypes::CT_SelectableGameObjectComponent);
            if (selectableComponent)
            {
              selectableComponent->Select();
              m_selectedGameObjects.push_back(overlappingGO[i]);
            }
          }
        }
        else if (m_selectedGameObjects.size() && !Input::GetKeyDown(sf::Keyboard::Key::LShift))
        {
          for (auto go = m_selectedGameObjects.begin(); go != m_selectedGameObjects.end();)
          {
            SelectableGameObjectComponent* selectableComponent = (SelectableGameObjectComponent*)(*go)->GetComponentOfType(EComponentTypes::CT_SelectableGameObjectComponent);
            if (selectableComponent)
            {
              selectableComponent->Deselect();
            }
            go = m_selectedGameObjects.erase(go);
          }
        }
      }

      newGoLoc *= worldInfo.tileSize;
      newGoLoc += worldInfo.tileSize / 2.0f;

      m_selectionGO->SetPosition(newGoLoc);
      float moveSpeedMod = 1.0f;
      if (Input::GetKeyDown(sf::Keyboard::Key::LShift))
        moveSpeedMod = 2.0f;
      if (Input::GetKeyDown(sf::Keyboard::Key::A))
        m_camera->MoveX( -m_cameraMoveSpeed* a_dt * moveSpeedMod);
      if (Input::GetKeyDown(sf::Keyboard::Key::D))
        m_camera->MoveX(m_cameraMoveSpeed* a_dt* moveSpeedMod);
      
      if (Input::GetKeyDown(sf::Keyboard::Key::W))
        m_camera->MoveY(-m_cameraMoveSpeed* a_dt* moveSpeedMod);
      if (Input::GetKeyDown(sf::Keyboard::Key::S))
        m_camera->MoveY(m_cameraMoveSpeed* a_dt* moveSpeedMod);
    }
  }
  UpdateUI();
}

void PlayerController::PreRender()
{
}

void PlayerController::Render(Renderer2D* a_renderer)
{
}

void PlayerController::UpdateUI()
{
  int buttonSpacing = 4;
  int buttonSizeX = 64, buttonSizeY = 64;
  int buttonCount = 0;
  int windowPadding = ImGui::GetStyle().WindowPadding.x;
  ImGui::SetNextWindowPos(ImVec2(32, 624));
  ImGui::SetNextWindowSize(ImVec2(1216, 80));
  ImGui::Begin("BuildMenu", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove |ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

  ImGui::Button("Coal Dirt", ImVec2(buttonSizeX, buttonSizeY));
  buttonCount++;

  ImGui::Button("Coal Rock", ImVec2(buttonSizeX, buttonSizeY));
  ImGui::SameLine(buttonCount*buttonSizeX +buttonSpacing + buttonSizeX);
  buttonCount++;

  ImGui::Button("Copper Dirt", ImVec2(buttonSizeX, buttonSizeY));
  ImGui::SameLine(buttonCount*buttonSizeX +buttonSpacing + buttonSizeX );
  buttonCount++;

  ImGui::Button("Copper Rock", ImVec2(buttonSizeX, buttonSizeY));
  ImGui::SameLine(buttonCount*buttonSizeX + buttonSpacing + buttonSizeX );
  buttonCount++;

  ImGui::End();
}
