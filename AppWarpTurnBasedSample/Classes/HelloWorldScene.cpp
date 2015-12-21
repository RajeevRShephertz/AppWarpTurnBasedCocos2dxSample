#include "HelloWorldScene.h"
#include "ConnectionListener.hpp"
USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    isFirstLaunch = false;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    float x_offset = origin.x + visibleSize.width/2;
    float y_offset = origin.y+visibleSize.height;
    float y_addition = 20;
   
    auto connectLabel = Label::createWithSystemFont("Connect", "Marker Felt", 24);
    //connectLabel->setColor(Color3B::WHITE);
    auto connectButton = MenuItemLabel::create(connectLabel, this, CC_MENU_SELECTOR(HelloWorld::connectToAppWarp));
    y_offset -= connectButton->getContentSize().height+y_addition;
    connectButton->setPosition(Vec2( x_offset,y_offset));
    
    
    auto createRoomLabel = Label::createWithSystemFont("CreateRoom", "Marker Felt", 24);
    createRoomLabel->setColor(Color3B::WHITE);
    auto createRoomButton = MenuItemLabel::create(createRoomLabel, this, CC_MENU_SELECTOR(HelloWorld::createTurnRoom));
    y_offset -= createRoomButton->getContentSize().height+y_addition;
    createRoomButton->setPosition(Vec2( x_offset,y_offset));
    
    auto startGameLabel = Label::createWithSystemFont("StartGame", "Marker Felt", 24);
    startGameLabel->setColor(Color3B::WHITE);
    auto startGameButton = MenuItemLabel::create(startGameLabel, this, CC_MENU_SELECTOR(HelloWorld::startGame));
    y_offset -= startGameButton->getContentSize().height+y_addition;
    startGameButton->setPosition(Vec2( x_offset,y_offset));
    
    auto sendMoveLabel = Label::createWithSystemFont("SendMove", "Marker Felt", 24);
    sendMoveLabel->setColor(Color3B::WHITE);
    auto sendMoveButton = MenuItemLabel::create(sendMoveLabel, this, CC_MENU_SELECTOR(HelloWorld::sendMove));
    y_offset -= sendMoveButton->getContentSize().height+y_addition;
    sendMoveButton->setPosition(Vec2( x_offset,y_offset));
    
    auto stopGameLabel = Label::createWithSystemFont("StopGame", "Marker Felt", 24);
    stopGameLabel->setColor(Color3B::WHITE);
    auto stopGameButton = MenuItemLabel::create(stopGameLabel, this, CC_MENU_SELECTOR(HelloWorld::stopGame));
    y_offset -= stopGameButton->getContentSize().height+y_addition;
    stopGameButton->setPosition(Vec2( x_offset,y_offset));


    // create menu, it's an autorelease object
    auto menu = Menu::create(connectButton,createRoomButton,startGameButton,sendMoveButton,stopGameButton, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    return true;
}

void HelloWorld::connectToAppWarp(cocos2d::Ref* pSender)
{
    AppWarp::Client *warpClientRef;
    if (!isFirstLaunch)
    {
        isFirstLaunch = !isFirstLaunch;
        AppWarp::Client::initialize(APPWARP_APP_KEY,APPWARP_SECRET_KEY);
        warpClientRef = AppWarp::Client::getInstance();
        //warpClientRef->setGeo("sgp"); //Uncomment to set geo location
        //warpClientRef->setRecoveryAllowance(60); //Uncomment to use connection resiliency
        ConnectionListener *connListener = new ConnectionListener();
        AppWarp::Client::getInstance()->setConnectionRequestListener(connListener);
        warpClientRef->setNotificationListener(this);
        warpClientRef->setRoomRequestListener(this);
        warpClientRef->setZoneRequestListener(this);
        warpClientRef->setTurnBasedRoomRequestListener(this);
        userName = "Shephertz";
        warpClientRef->connect(userName);
    }
    else
    {
        AppWarp::Client::getInstance()->connect(userName);
    }
}

void HelloWorld::createTurnRoom(cocos2d::Ref* pSender)
{
    AppWarp::Client *warpClientRef = AppWarp::Client::getInstance();
    std::map<std::string,std::string> properties;
    properties["level"]= "Beginner";
    warpClientRef->createTurnRoom("R1", "Rajeev", 4,properties,60);
}


void HelloWorld::startGame(cocos2d::Ref* pSender)
{
    AppWarp::Client::getInstance()->startGame();
}

void HelloWorld::stopGame(cocos2d::Ref* pSender)
{
    AppWarp::Client::getInstance()->stopGame();
}

void HelloWorld::sendMove(cocos2d::Ref* pSender)
{
    AppWarp::Client::getInstance()->sendMove("My Move");
}

/*
 * AppWarp Listeners
 */
void HelloWorld::onConnectDone(int result, int reasonCode)
{
    printf("\nonConnectDone .. Code=%d\n",result);
    if (result==AppWarp::ResultCode::success)
    {
        unscheduleRecover();
        printf("\nonConnectDone .. SUCCESS..session=%d\n",AppWarp::AppWarpSessionID);
    }
    else if (result==AppWarp::ResultCode::auth_error)
    {
        unscheduleRecover();
        printf("\nonConnectDone .. Failed with auth_error and reasonCode = %d..session=%d\n",reasonCode,AppWarp::AppWarpSessionID);
    }
    else if (result==AppWarp::ResultCode::success_recovered)
    {
        unscheduleRecover();
        printf("\nonConnectDone .. SUCCESS with success_recovered..session=%d\n",AppWarp::AppWarpSessionID);
    }
    else if (result==AppWarp::ResultCode::connection_error_recoverable)
    {
        scheduleRecover();
        printf("\nonConnectDone .. FAILED..connection_error_recoverable..session=%d\n",AppWarp::AppWarpSessionID);
    }
    else if (result==AppWarp::ResultCode::connection_error)
    {
        scheduleRecover();
        printf("\nonConnectDone .. FAILED..connection_error..session=%d\n",AppWarp::AppWarpSessionID);
    }
    else if (result==AppWarp::ResultCode::bad_request)
    {
        unscheduleRecover();
        printf("\nonConnectDone .. FAILED with bad request..session=%d\n",AppWarp::AppWarpSessionID);
    }
    else
    {
        unscheduleRecover();
        printf("\nonConnectDone .. FAILED with reasonCode=%d..session=%d\n",reasonCode,AppWarp::AppWarpSessionID);
    }
}

void HelloWorld::onDisconnectDone(int res)
{
    printf("\nonDisconnectDone");
}

void HelloWorld::onCreateRoomDone (AppWarp::room revent)
{
    
    if (revent.result == AppWarp::ResultCode::success)
    {
        printf("\nonCreateRoomDone SUCCESS...room id=%s",revent.roomId.c_str());
        AppWarp::Client::getInstance()->joinRoom(revent.roomId);
    }
    else
    {
        //Handle failed case here
        printf("\nonCreateRoomDone FAILED");
    }
}

void HelloWorld::onJoinRoomDone(AppWarp::room revent)
{
    if (revent.result == AppWarp::ResultCode::success)
    {
        printf("\n onJoinRoomDone SUCCESS");
        AppWarp::Client::getInstance()->subscribeRoom(revent.roomId);
    }
    else
    {
        //Handle failed case here
        printf("\n onJoinRoomDone FAILED");
    }
}

void HelloWorld::onSubscribeRoomDone(AppWarp::room revent)
{
    if (revent.result == AppWarp::ResultCode::success)
    {
        printf("\n onSubscribeRoomDone SUCCESS");
    }
    else
    {
        printf("\n onSubscribeRoomDone FAILED");
    }
}


void HelloWorld::onStartGameDone(int result)
{
    if (result == AppWarp::ResultCode::success)
    {
        printf("\n onStartGameDone SUCCESS");
    }
    else
    {
        printf("\n onStartGameDone FAILED");
    }
}

void HelloWorld::onStopGameDone(int result)
{
    if (result == AppWarp::ResultCode::success)
    {
        printf("\n onStopGameDone SUCCESS");
    }
    else
    {
        printf("\n onStopGameDone FAILED");
    }
}

void HelloWorld::onSendMoveDone(int result)
{
    if (result == AppWarp::ResultCode::success)
    {
        printf("\n onSendMoveDone SUCCESS");
    }
    else
    {
        printf("\n onSendMoveDone FAILED");
    }
}

void HelloWorld::onSetNextTurnDone(int result)
{
    if (result == AppWarp::ResultCode::success)
    {
        printf("\n onSetNextTurnDone SUCCESS");
    }
    else
    {
        printf("\n onSetNextTurnDone FAILED");
    }
}


/**
 * Connection-Resiliency
 */
void HelloWorld::scheduleRecover()
{
    printf("\nHelloWorld::scheduleRecover");
    this->schedule(schedule_selector(HelloWorld::recover), 5.0f);
}

void HelloWorld::unscheduleRecover()
{
    printf("\nHelloWorld::unscheduleRecover");
    unschedule(schedule_selector(HelloWorld::recover));
}

void HelloWorld::recover(float dt)
{
    printf("\nHelloWorld::recover");
    AppWarp::Client::getInstance()->recoverConnection();
}



#pragma mark- - AppWarp Notifications -

void HelloWorld::onRoomCreated(AppWarp::room rData)
{
    printf("\n onRoomCreated");
}

void HelloWorld::onUserLeftRoom(AppWarp::room rData, std::string user)
{
    printf("\n onUserLeftRoom User = %s",user.c_str());
}

void HelloWorld::onUserJoinedRoom(AppWarp::room rData, std::string user)
{
    printf("\n onUserJoinedRoom User = %s",user.c_str());
}

void HelloWorld::onChatReceived(AppWarp::chat chatevent)
{
    printf("\n onChatReceived");
}

void HelloWorld::onUserPaused(std::string user,std::string locId,bool isLobby)
{
    printf("\n onUserPaused User = %s",user.c_str());
}

void HelloWorld::onUserResumed(std::string user,std::string locId,bool isLobby)
{
    printf("\n onUserResumed User = %s",user.c_str());
}

void HelloWorld::onGameStarted(std::string sender, std::string id, std::string nextTurn)
{
    printf("\n onGameStarted  Sender=%s id=%s  NextTurn=%s",sender.c_str(),id.c_str(),nextTurn.c_str());
}

void HelloWorld::onGameStopped(std::string sender, std::string id)
{
    printf("\n onGameStopped Sender=%s",sender.c_str());
}

void HelloWorld::onMoveCompleted(AppWarp::move event)
{
    printf("\n onMoveCompleted");
    printf("\n Sender = %s",event.sender.c_str());
    printf("\n moveData = %s",event.moveData.c_str());
    printf("\n nextTurn = %s",event.nextTurn.c_str());
    printf("\n roomId = %s",event.roomId.c_str());
}



void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
