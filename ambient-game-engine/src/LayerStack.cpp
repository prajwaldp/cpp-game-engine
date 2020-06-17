#include "LayerStack.h"

namespace Ambient {
LayerStack::LayerStack() { m_LayerIt = m_Layers.begin(); }
LayerStack::~LayerStack() {
    for (Layer* layer : m_Layers) {
        delete layer;
    }
}

void LayerStack::Push(Layer* layer) {
    m_LayerIt = m_Layers.emplace(m_LayerIt, layer);
}

void LayerStack::PushOverlay(Layer* overlay) { m_Layers.emplace_back(overlay); }

void LayerStack::Pop(Layer* layer) {
    auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);

    if (it != m_Layers.end()) {
        m_Layers.erase(it);
        m_LayerIt--;
    }
}

void LayerStack::PopOverlay(Layer* overlay) {
    auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
    if (it != m_Layers.end()) {
        m_Layers.erase(it);
    }
}
} // namespace Ambient
