#include "NavigationHandler.h"

e_Views CurrentView = e_None;
e_Views RequestedView = e_None;

void NavigationHandler::Init() {
  RequestedView = e_Clock;
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