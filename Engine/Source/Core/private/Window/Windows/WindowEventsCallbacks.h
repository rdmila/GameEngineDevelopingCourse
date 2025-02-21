#pragma once

#include <Camera.h>
#include <Constants.h>
#include <Window/IWindow.h>
#include <Window.h>

namespace GameEngine::Core
{
    void OnMouseDown(WPARAM btnState, int x, int y, Window* window)
    {
        window->SetMousePos(x, y);

        SetCapture(GetPlatformWindowHandle(window->GetWindowHandle()));
    }

    void OnMouseUp(WPARAM btnState, int x, int y)
    {
        ReleaseCapture();
    }

    void OnMouseMove(WPARAM btnState, int x, int y, Camera* camera, Window* window)
    {
        Math::Vector2i mousePos = window->GetMousePos();
        if ((btnState & MK_LBUTTON) != 0)
        {
            float dx = 0.25 * static_cast<float>(x - mousePos.x) * Math::Constants::PI / 180.f;
            float dy = 0.25 * static_cast<float>(y - mousePos.y) * Math::Constants::PI / 180.f;

            dy = -dy; // To avoid inverse movement

            camera->Rotate(dx, dy);
        }
        else if ((btnState & MK_RBUTTON) != 0)
        {
            float dx = 0.05f * static_cast<float>(x - mousePos.x);
            float dy = 0.05f * static_cast<float>(y - mousePos.y);

            Math::Vector3f offset = camera->GetViewDir() * (dx - dy);

            Math::Vector3f position = camera->GetPosition();
            position = position + offset;

            camera->SetPosition(position);
        }

        window->SetMousePos(x, y);
    }

    void OnMoveKeyDown(WPARAM btnState, Camera* camera) {
        const float SPEED = 0.1f;
        Math::Vector3f viewDir = camera->GetViewDir();
        Math::Vector2f offset;
        switch (btnState)
        {
        case 0x57: //W
            offset = Math::Vector2f(viewDir.x, viewDir.y);
            break;
        case 0x41: //A
            offset = Math::Vector2f(viewDir.y, -viewDir.x);
            break;
        case 0x53: //S
            offset = Math::Vector2f(-viewDir.x, -viewDir.y);
            break;
        case 0x44: //D
            offset = Math::Vector2f(-viewDir.y, viewDir.x);
            break;
        default:
            offset = Math::Vector2f::Zero();
        }
        Math::Vector3f position = camera->GetPosition();
        position = position + Math::Vector3f(offset.x, offset.y, 0) * SPEED;
        camera->SetPosition(position);
    }
}