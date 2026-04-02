#pragma once
#include "engine/ui/renderable_object.h"
#include <functional>

namespace ui
{
class RenderableObject;

class MenuOption
{
  private:
    bool m_isSelected;
    std::function<void()> m_callback;

  protected:
    virtual void UpdateStyle() = 0;

  public:
    explicit MenuOption(bool isSelected, std::function<void()> callback = nullptr);

    virtual ~MenuOption() = default;

    void SetSelected(const bool selected);

    bool IsSelected() const;

    void Select() const;

    virtual RenderableObject& GetRenderableObject() = 0;
    virtual const RenderableObject& GetRenderableObject() const = 0;
};

} // namespace ui