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
  m_camera(a_camera),
  m_currentSpawningObj(nullptr)
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
      newGoLoc *= worldInfo.tileSize;
      newGoLoc += worldInfo.tileSize / 2.0f;

      if (!m_currentSpawningObj)
      {
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
      }
      else
      {
        if (Input::GetMouseButton(0))
        {
          if (m_grid)
          {
            Node* node = m_grid->GetNode(newGoLoc);
            if (node->bwalkable)
            {
              m_currentSpawningObj = nullptr;
              RendererComponent* selectionGoRenderComponent = (RendererComponent*)m_selectionGO->GetComponentOfType(EComponentTypes::CT_RenderComponent);
              selectionGoRenderComponent->SetActive(true);
              m_grid->SetNodeWalkable(node->index.x, node->index.y, false);
            }
          }
        }
      }




      if (m_currentSpawningObj)
      {
        m_currentSpawningObj->SetPosition(newGoLoc);
      }
      else
      {
        m_selectionGO->SetPosition(newGoLoc);
      }
      float moveSpeedMod = 1.0f;
      if (Input::GetKeyDown(sf::Keyboard::Key::LShift))
        moveSpeedMod = 2.0f;
      if (Input::GetKeyDown(sf::Keyboard::Key::A))
        m_camera->MoveX(-m_cameraMoveSpeed* a_dt * moveSpeedMod);
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
  int buttonSizeX = 32, buttonSizeY = 32;
  ImGui::SetNextWindowPos(ImVec2(32, 624));
  ImGui::SetNextWindowSize(ImVec2(1216, 80));
  ImGui::Begin("BuildMenu", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

  if (ImGui::SmallButton("Coal Dirt"))
    SpawningObject(ESpawnableItemIDs::CoalDirt);
  ImGui::SameLine();
  if (ImGui::SmallButton("Coal Rock"))
    SpawningObject(ESpawnableItemIDs::CoalRock);

  ImGui::SameLine();
  if (ImGui::SmallButton("Copper Dirt"))
    SpawningObject(ESpawnableItemIDs::CopperDirt);

  ImGui::SameLine();
  if (ImGui::SmallButton("Copper Rock"))
    SpawningObject(ESpawnableItemIDs::CopperRock);

  ImGui::SameLine();

  ImGui::End();
}


void PlayerController::SpawningObject(ESpawnableItemIDs a_id)
{
  RendererComponent* component = nullptr;
  if (!m_currentSpawningObj)
  {
    m_currentSpawningObj = GetWorld()->CreateGameObject(EGameObjectType::GOT_Empty);
    component = (RendererComponent*)m_currentSpawningObj->AddComponent(
      EComponentTypes::CT_RenderComponent);
    component->ChangeRenderType(RenderType::Static);
  }
  RendererComponent* selectionGoRenderComponent = (RendererComponent*)m_selectionGO->GetComponentOfType(EComponentTypes::CT_RenderComponent);
  selectionGoRenderComponent->SetActive(false);
  if (m_currentSpawningObj)
  {
    if (!component)
      component = m_currentSpawningObj->TGetComponentOfType<RendererComponent>(EComponentTypes::CT_RenderComponent);
    component->ChangeTextureID((uint)a_id);
  }
}