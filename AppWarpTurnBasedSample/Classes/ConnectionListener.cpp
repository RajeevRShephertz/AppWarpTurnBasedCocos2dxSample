//
//  ConnectionListener.cpp
//  AppWarpTurnBasedSample
//
//  Created by Rajeev Ranjan on 26/10/15.
//
//

#include "ConnectionListener.hpp"

void ConnectionListener::onConnectDone(int resultCode, int reasonCode)
{
    printf("onConnectDoneCalled.....resultCode=%d",resultCode);
}
