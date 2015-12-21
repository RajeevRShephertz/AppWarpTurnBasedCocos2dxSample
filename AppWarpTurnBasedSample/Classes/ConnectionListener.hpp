//
//  ConnectionListener.hpp
//  AppWarpTurnBasedSample
//
//  Created by Rajeev Ranjan on 26/10/15.
//
//

#ifndef ConnectionListener_hpp
#define ConnectionListener_hpp

#include <stdio.h>
#include "appwarp.h"

class ConnectionListener : public AppWarp::ConnectionRequestListener
{
public:
    void onConnectDone(int, int);
};
#endif /* ConnectionListener_hpp */
