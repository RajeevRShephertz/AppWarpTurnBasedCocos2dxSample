#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "appwarp.h"

#define APPWARP_APP_KEY     "cad2bfab6310acd9696187b98682925125e469ab0d0d585db0b00609f461b791"
#define APPWARP_SECRET_KEY  "55811709916e7ce4405cde0cdc5a254cf4b506fbafdae05760a73100b8080b67"

class HelloWorld : public cocos2d::Layer, public AppWarp::ConnectionRequestListener, public AppWarp::ZoneRequestListener, public AppWarp::RoomRequestListener, public AppWarp::TurnBasedRoomRequestListener, public AppWarp:: ChatRequestListener,public AppWarp::NotificationListener
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    bool isFirstLaunch;
    std::string userName;

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    void connectToAppWarp(cocos2d::Ref* pSender);
    void createTurnRoom(cocos2d::Ref* pSender);
    void startGame(cocos2d::Ref* pSender);
    void stopGame(cocos2d::Ref* pSender);
    void sendMove(cocos2d::Ref* pSender);

    void scheduleRecover();
    void unscheduleRecover();
    void recover(float dt);
    
    /*
     * AppWarp ConnectionRequestListener
     */
    void onConnectDone(int, int);
    void onDisconnectDone(int);
    
    /*
     * AppWarp ZoneRequestListener
     */
    void onCreateRoomDone (AppWarp::room revent);
    
    /*
     * AppWarp RoomRequestListener
     */
    void onSubscribeRoomDone(AppWarp::room revent);
    void onJoinRoomDone(AppWarp::room revent);

    /*
     * AppWarp TurnBasedRoomRequestListener
     */
    void onStartGameDone(int res);
    void onStopGameDone(int res);
    void onSendMoveDone(int res);
    void onSetNextTurnDone(int res);
    
    /*
     * AppWarp ChatRequestListener
     */
    /*void onSendChatDone(int res);
    void onSendPrivateChatDone(int res);*/
    
    /*
     * AppWarp NotificationListener
     */
    
    void onRoomCreated(AppWarp::room rData) ;
    void onUserLeftRoom(AppWarp::room rData, std::string user) ;
    void onUserJoinedRoom(AppWarp::room rData, std::string user) ;
    void onChatReceived(AppWarp::chat chatevent) ;
    
    void onUserPaused(std::string user,std::string locId,bool isLobby);
    void onUserResumed(std::string user,std::string locId,bool isLobby);
    
    void onGameStarted(std::string sender, std::string id, std::string nextTurn);
    void onGameStopped(std::string sender, std::string id);
    void onMoveCompleted(AppWarp::move event);
    //void onNextTurnRequest(std::string lastTurn);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
