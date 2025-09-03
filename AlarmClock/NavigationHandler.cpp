#include "NavigationHandler.h"


void NavigationHandler::Init() {
  _requestedView = e_Main;
}

void NavigationHandler::NavigateTo(e_Views requestedView, uint8_t param) {
  _requestedView = requestedView;
  _param = param;
}

void NavigationHandler::NavigationDone() {
  _currentView = _requestedView;
  _requestedView = e_None;
}

e_Views NavigationHandler::GetRequestedView() {
  return _requestedView;
}

e_Views NavigationHandler::GetCurrentView() {
  return _currentView;
}

uint8_t NavigationHandler::GetParam() {
  return _param;
}