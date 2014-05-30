#pragma once
#include "bookmark.hpp"

#include "../graphics/defines.hpp"

#include "../geometry/point2d.hpp"

#include "../std/shared_ptr.hpp"
#include "../std/function.hpp"


class Framework;
class PaintEvent;
namespace location { class GpsInfo; }
namespace gui { class Element; }
namespace url_scheme { struct ApiPoint; }

class PinClickManager
{
  Framework & m_f;

  void OnActivateUserMark(UserMarkCopy * mark);
  void OnDismiss();

  void SetBalloonVisible(bool isVisible);

public:
  PinClickManager(Framework & f);

  void RenderPolicyCreated(graphics::EDensity density) {}
  void LocationChanged(location::GpsInfo const & info) {}

  void OnClick(m2::PointD const & pxPoint, bool isLongTouch);
  void OnBookmarkClick(BookmarkAndCategory const & bnc);

  void Hide();

  void RemovePin();
  void Dismiss();

private:
  /// @name Platform dependent listeners to show special activities.
  //@{
  // Use must remove UserMarkCopy that transfer for you by this callback
  function<void (UserMarkCopy *)>      m_userMarkListener;
  function<void (void)>            m_dismissListener;

public:
  template <class T> void ConnectUserMarkListener(T const & t)   { m_userMarkListener = t; }
  template <class T> void ConnectDismissListener(T const & t)    { m_dismissListener = t; }

  void ClearListeners();
  //@}
};
