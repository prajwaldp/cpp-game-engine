#pragma once

#include "Layer.h"

// Included in Precompiled Header. Can remove?
#include <vector>

namespace Ambient {

/*
A wrapper over a vector of Layer objects
It is not an actual stack. Layers get pushed into the starting half and Overlays
get pushed into the second half of the vector.
*/
class LayerStack {
  private:
    std::vector<Layer *> m_Layers;
    std::vector<Layer *>::iterator m_LayerIt;

  public:
    LayerStack();
    ~LayerStack();

    void Push(Layer *layer);
    void PushOverlay(Layer *overlay);
    void Pop(Layer *layer);
    void PopOverlay(Layer *layer);

    std::vector<Layer *>::iterator begin() { return m_Layers.begin(); }
    std::vector<Layer *>::iterator end() { return m_Layers.end(); }
};
} // namespace Ambient
