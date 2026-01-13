#pragma once

#include "../GateState.hpp"

class SearchingForDevices : public GateState {
    public:
        SearchingForDevices(StateMachine& _context) : State(_context) {}
        
        void onEntry() override {
            std::cout << "Searching For Devices..." << std::endl;
            // Start searching for devices
            searchForDevices(); // TODO: Implement device search logic
            // this should return a promise that resolves to a list of devices
            // the promise should be resolved when the devices are found / after 10 seconds.
        }
        
        void onExit() override {
            // cancel the search for devices
            cancelSearchForDevices(); // TODO: Implement device search cancellation logic
            std::cout << "Searching For Devices... Cancelled" << std::endl;
        }
        
        
        State* onBackButton() override {
            // Go back to connect new
            cancelSearch();
            return getState(StateId::CONNECT_NEW);
        }
    
    private:
    // TODO: Implement device search logic
        void searchForDevices();
        void cancelSearch();
    };