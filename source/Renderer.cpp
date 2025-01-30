#include "renderer.h"

void RenderManager2D::CalculateLayout()
{
    if (RenderTarget != NULL)
    {
        D2D1_SIZE_F size = RenderTarget->GetSize();
        const float x = size.width / 2;
        const float y = size.height / 2;
        const float radius = min(x, y);
        ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
    }
}

HRESULT RenderManager2D::CreateGraphicsResources()
{
    HRESULT hr = S_OK;
    if (RenderTarget == NULL)
    {
        RECT rc;
        GetClientRect(Window, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

        hr = Factory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(Window, size),
            &RenderTarget);

        if (SUCCEEDED(hr))
        {
            const D2D1_COLOR_F color = D2D1::ColorF(0.f, 1.f, 1.f);
            hr = RenderTarget->CreateSolidColorBrush(color, &Brush);

            if (SUCCEEDED(hr))
            {
                CalculateLayout();
            }
        }
    }
    return hr;
}

void RenderManager2D::GetHwnd(HWND hWnd)
{
    Window = hWnd;
}

void RenderManager2D::Shutdown()
{
    if (!RenderTarget)
    {
        RenderTarget->Release();
        RenderTarget = nullptr; 
    }

    if (!Brush)
    {
        Brush->Release();
        Brush = nullptr;
    }

    if (!Factory)
    {
        Factory->Release();
        Factory = nullptr;
    }
}

void RenderManager2D::OnPaint()
{
    HRESULT hr = CreateGraphicsResources();
    if (SUCCEEDED(hr))
    {
        PAINTSTRUCT ps;
        BeginPaint(Window, &ps);

        RenderTarget->BeginDraw();

        RenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Purple));
        RenderTarget->FillEllipse(ellipse, Brush);

        hr = RenderTarget->EndDraw();
        if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
        {
            Shutdown();
        }
        EndPaint(Window, &ps);
    }
}

void RenderManager2D::Resize()
{
    if (RenderTarget != NULL)
    {
        RECT rc;
        GetClientRect(Window, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

        RenderTarget->Resize(size);
        CalculateLayout();
        InvalidateRect(Window, NULL, FALSE);
    }
}
