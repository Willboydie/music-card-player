#include "StateBuilder.hpp"
#include "../event/EventBus.hpp"
#include "../view/Renderer.hpp"
#include "../view/View.hpp"

std::unique_ptr<StateMachine> buildStateMachine(EventBus& bus, Renderer& renderer) {
    auto stateMachine = std::make_unique<StateMachine>();

    auto menuView = std::make_unique<MenuView>();
    auto loadingView = std::make_unique<LoadingView>();
    auto playerView = std::make_unique<PlayerView>();
    stateMachine->setViews(std::move(menuView), std::move(loadingView), std::move(playerView));

    auto* mv = stateMachine->getMenuView();
    auto* lv = stateMachine->getLoadingView();
    auto* pv = stateMachine->getPlayerView();

    stateMachine->registerState(StateId::MAIN_MENU,
        std::make_unique<MainMenuState>(bus, renderer, *mv, "Main Menu"));
    stateMachine->registerState(StateId::PLAYER,
        std::make_unique<PlayerState>(bus, renderer, *pv, "Player"));
    stateMachine->registerState(StateId::BLUETOOTH_MENU,
        std::make_unique<BluetoothMenuState>(bus, renderer, *mv, "Bluetooth Menu"));
    stateMachine->registerState(StateId::SAVED_DEVICES,
        std::make_unique<SavedDevicesMenu>(bus, renderer, *mv, "Saved Devices"));
    stateMachine->registerState(StateId::CONNECT_NEW,
        std::make_unique<ConnectNew>(bus, renderer, *lv, "Connect New"));
    stateMachine->registerState(StateId::SEARCHING_FOR_DEVICES,
        std::make_unique<SearchingForDevices>(bus, renderer, *lv, "Searching For Devices"));
    stateMachine->registerState(StateId::FOUND_DEVICES,
        std::make_unique<FoundDevicesMenu>(bus, renderer, *mv, "Found Devices"));
    stateMachine->registerState(StateId::CONNECTING,
        std::make_unique<BluetoothConnecting>(bus, renderer, *lv, "Connecting"));

    stateMachine->transitionTo(StateId::MAIN_MENU);

    return stateMachine;
}