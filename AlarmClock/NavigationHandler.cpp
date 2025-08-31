#include "NavigationHandler.h"


void NavigationHandler::Init() {
  RequestedView = e_TimeSetting;
}

void NavigationHandler::NavigateTo(e_Views requestedView) {
  RequestedView = requestedView;
}

void NavigationHandler::NavigationDone() {
  CurrentView = RequestedView;
  RequestedView = e_None;
}

e_Views NavigationHandler::GetRequestedView() {
  return RequestedView;
}

e_Views NavigationHandler::GetCurrentView() {
  return CurrentView;
}