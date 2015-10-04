#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    // physics scene
    auto scene = Scene::createWithPhysics();
    
    // physics debug
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
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
    
    visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    
    setTouchEnabled(true);
     //one point touch
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    

    // TiledMap
    tiledMap = TMXTiledMap::create("map.tmx");
    mapWidth = tiledMap->getMapSize().width;
    mapHeight = tiledMap->getMapSize().height;
    //log("w =  %d , h = %d", mapWidth , mapHeight );
    
    // background
    backgroudLayer = tiledMap->getLayer( "background" );
    bornPoints = tiledMap->getObjectGroup( "born" );
    
    
    for(int i = 0 ; i < mapWidth; ++i )
        for(int j = 0 ; j < mapHeight; ++j )
            if (backgroudLayer->getTileGIDAt(Vec2( i , j )) != 0)
            {
                auto body = PhysicsBody::createBox( Size( 16.0 , 16.0 ) );
                body->setDynamic(false);
                auto boxnode = Node::create();
                boxnode->setPosition( tiledToVec( i , j ) );
                boxnode->setPhysicsBody( body );
                this->addChild( boxnode );
            }
    
    
    tiledMap->setPosition( Vec2::ZERO );
    this->addChild(tiledMap);
    
    // define world edge
    auto body = PhysicsBody::createEdgeBox(tiledMap->getContentSize(),PHYSICSBODY_MATERIAL_DEFAULT,5.0f);
    auto edgeNode = Node::create();
    edgeNode->setPosition( tiledMap->getContentSize().width * 0.5 ,tiledMap->getContentSize().height * 0.5);
    edgeNode->setPhysicsBody(body);
    this->addChild(edgeNode);
    
//    // Player
    ValueMap playerBorn = bornPoints->getObject( "playerBorn" );
    float x = playerBorn.at( "x" ).asFloat() ;
    float y = playerBorn.at( "y" ).asFloat() ;
    
    log("x = %f , y = %f" , x, y );
    
    player = Ninja::create( 0 );
    player->setPosition( Vec2(x,y) );
    
    auto playerBody = PhysicsBody::createBox( Size(28.0,30.0) );
    player->setPhysicsBody( playerBody );
    
    this->addChild( player );
    

    //m_Camera = Camera::createOrthographic( visibleSize.width , visibleSize.height, 0, 1000 );
    //m_Camera->setCameraFlag( CameraFlag::USER2);
    //m_Camera->setPosition3D( Vec3(player->getPositionX(),player->getPositionY(),0));
    //this->setCameraMask((unsigned short)CameraFlag::USER2);
    //this->addChild(m_Camera);
    
    scheduleUpdate();
    return true;
}

void HelloWorld::update( float dt )
{
    //m_Camera->setPosition3D( Vec3(player->getPositionX(),player->getPositionY(),0) );
    
    if( player->getState()==Ninja::State::WalkRight )
    {
        MoveBy* move = MoveBy::create( 0.1, Vec2( 2 , 0) );
        player->runAction(move);
        shot();
    }
    else if (player->getState()==Ninja::State::WalkLeft)
    {
        MoveBy* move = MoveBy::create( 0.1, Vec2( -2 , 0) );
        player->runAction(move);
        shot();
    }
    else
        player->stopAllActions();
    
}

void HelloWorld::shot()
{
    auto bullet = Sprite::create("bullet.png");
    bullet->setPosition( player->getPosition() );
    this->addChild( bullet );
    if ( player->getState() == Ninja::State::WalkLeft )
    {
        MoveBy* move = MoveBy::create( 1 , Vec2( -100 , 0) );
        bullet->runAction(move);
        
    }
    if ( player->getState() == Ninja::State::WalkLeft )
    {
        MoveBy* move = MoveBy::create( 1 , Vec2( 100 , 0) );
        bullet->runAction(move);
    }
    bullet->retain();

}

bool  HelloWorld::onTouchBegan(  Touch* touch, Event *unused_event )
{
    Point pos = touch->getLocation();
    //log("touch.x = %f ,touch.y = %f" , pos.x , pos.y );
    if ( pos.x > player->getPositionX() )
    {
        player->SetState(Ninja::State::WalkRight);
    }
    else
    {
        player->SetState(Ninja::State::WalkLeft);
    }
    
    return true;
}

void  HelloWorld::onTouchMoved( Touch* touch, Event *unused_event )
{
    Point pos = touch->getLocation();
    //log("touch.x = %f ,touch.y = %f" , pos.x , pos.y );
    if ( pos.x > player->getPositionX() &&  player->getState()==Ninja::State::WalkLeft )
        player->SetState(Ninja::State::None);
    if ( pos.x < player->getPositionX() &&  player->getState()==Ninja::State::WalkRight )
        player->SetState(Ninja::State::None);
    
}

void HelloWorld::onTouchEnded( Touch* touch, Event *unused_event )
{
    player->SetState(Ninja::State::None);
    
}

const cocos2d::Vec2 HelloWorld::tiledToVec( int i ,int j )const
{
    float X = i * 15.0 + 8.0 ;
    float Y = tiledMap->getContentSize().height - j * 15.0 - 8.0 ;
    return Vec2( X , Y );
}

const cocos2d::Vec2 HelloWorld::vecToTiled( const cocos2d::Vec2 position )const
{
    
    return Vec2( 0 , 0 );
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
