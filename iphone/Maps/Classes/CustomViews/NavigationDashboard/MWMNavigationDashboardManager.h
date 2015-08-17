//
//  MWMNavigationDashboardManager.h
//  Maps
//
//  Created by v.mikhaylenko on 20.07.15.
//  Copyright (c) 2015 MapsWithMe. All rights reserved.
//

#import "LocationManager.h"
#import "MWMNavigationViewProtocol.h"

#include "Framework.h"
#include "platform/location.hpp"

typedef NS_ENUM(NSUInteger, MWMNavigationDashboardState)
{
  MWMNavigationDashboardStateHidden,
  MWMNavigationDashboardStatePlanning,
  MWMNavigationDashboardStateError,
  MWMNavigationDashboardStateReady,
  MWMNavigationDashboardStateNavigation
};

@protocol MWMNavigationDashboardManagerProtocol <MWMNavigationViewProtocol>

- (void)buildRouteWithType:(enum routing::RouterType)type;
- (void)didStartFollowing;
- (void)didCancelRouting;
- (void)updateStatusBarStyle;

@end

@class MWMNavigationDashboardEntity;

@interface MWMNavigationDashboardManager : NSObject <LocationObserver>

@property (nonatomic, readonly) MWMNavigationDashboardEntity * entity;
@property (nonatomic) MWMNavigationDashboardState state;
@property (nonatomic) CGFloat topBound;
@property (nonatomic, readonly) CGFloat height;

- (instancetype)init __attribute__((unavailable("init is not available")));
- (instancetype)initWithParentView:(UIView *)view delegate:(id<MWMNavigationDashboardManagerProtocol>)delegate;
- (void)setupDashboard:(location::FollowingInfo const &)info;
- (void)playTurnNotifications;
- (void)willRotateToInterfaceOrientation:(UIInterfaceOrientation)orientation;
- (void)setRouteBuildingProgress:(CGFloat)progress;
- (void)showHelperPanels;
- (void)hideHelperPanels;

@end
