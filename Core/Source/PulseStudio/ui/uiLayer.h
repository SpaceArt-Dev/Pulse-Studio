#pragma once
#include "PulseStudio/Layer.h"
#include <vector>

namespace PulseStudio {

    class uiWindow;

    class uiLayer : public Layer
    {
    public:
        uiLayer();
        virtual ~uiLayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate(float deltaTime) override;
        virtual void OnEvent(Event& event) override;

        void AddWindow(uiWindow* window);
    private:
        std::vector<uiWindow*> m_Windows;
    };

}