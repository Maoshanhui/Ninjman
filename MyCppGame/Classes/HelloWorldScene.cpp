#include "HelloWorldScene.h"
#include "CCShake.h"
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
    
    scene->getPhysicsWorld()->setGravity(Vect(0.0f, -298.0f));
    
    layer->setPhyWorld(scene->getPhysicsWorld());
    
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
    
    // background node
    bgNode = Node::create();
    bgNode->setContentSize( Size( 1600 , 1120 ) );
    bgNode->setAnchorPoint(Vec2::ZERO);
    bgNode->setPosition( Vec2::ZERO );
    
    // bullet node
    bulletNode = Node::create();
    
    // brick node
    brickNode = Node::create();
    
    setTouchEnabled(true);
     //one point touch
    setTouchMode(Touch::DispatchMode::ALL_AT_ONCE);
    

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
                auto body = PhysicsBody::createBox( Size( 16.0 , 16.0 ),PhysicsMaterial(2.0f,0.0f,0.0f) );
                body->setDynamic(false);
                auto boxnode = Node::create();
                boxnode->setContentSize( Size(16.0,16.0));
                boxnode->setPosition( tiledToVec( i , j ) );
                boxnode->setPhysicsBody( body );
                brickNode->addChild( boxnode );
                
            }
    
    
    tiledMap->setPosition( Vec2::ZERO );
    bgNode->addChild(tiledMap);
    
    
    // define world edge
    auto body = PhysicsBody::createEdgeBox(tiledMap->getContentSize(),PHYSICSBODY_MATERIAL_DEFAULT,5.0f);
    auto edgeNode = Node::create();
    edgeNode->setPosition( tiledMap->getContentSize().width * 0.5 ,tiledMap->getContentSize().height * 0.5);
    edgeNode->setPhysicsBody(body);
    bgNode->addChild(edgeNode);
    
    // Player
    ValueMap playerBorn = bornPoints->getObject( "playerBorn" );
    float x = playerBorn.at( "x" ).asFloat() ;
    float y = playerBorn.at( "y" ).asFloat() ;
    
    //log("x = %f , y = %f" , x, y );
    
    player = Ninja::create( 0 );
    player->setPosition( Vec2(x,y) );
    
    playerBody = PhysicsBody::createBox( Size(28.0,30.0) ,PhysicsMaterial(0.0f,0.0f,0.0f));
    playerBody->setMass( 50.0 );
    playerBody->setRotationEnable(false);
    playerBody->setDynamic( true );
    player->setPhysicsBody( playerBody );
    
    bgNode->addChild( player );
    
    // monster
    ValueMap monsterBorn = bornPoints->getObject( "monsterBorn1" );
    x = monsterBorn.at( "x" ).asFloat() ;
    y = monsterBorn.at( "y" ).asFloat() ;
    for(int i = 0 ; i < 20 ; ++i )
    {
        monsters[ i ] =  Ninja::create( 1 );
        monsters[ i ]->setPosition( Vec2( x, y ) );
        monsters[ i ]->setVisible( false );
        bgNode->addChild( monsters[ i ]);
    }
    
    //Camera
    //m_Camera = Camera::createOrthographic( visibleSize.width , visibleSize.height, 0, 1000 );
    //m_Camera->setCameraFlag( CameraFlag::USER2);
    //m_Camera->setPosition3D( Vec3(player->getPositionX(),player->getPositionY(),0));
    //this->setCameraMask((unsigned short)CameraFlag::USER2);
    //this->addChild(m_Camera);
    
    
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem->setScale( 3 );
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - 50 ,origin.y + 50 ));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    dirBall = Sprite::create( "CloseNormal.png" );
    dirBall->setScale( 3 );
    dirBall->setPosition(Vec2(origin.x + 50 ,origin.y + 50 ));
    this->addChild(dirBall);
    
    dirBallRoot = Node::create();
    dirBallRoot->setPosition( dirBall->getPosition() );
    this->addChild(dirBallRoot);
    
    shotButton = Sprite::create( "CloseNormal.png" );
    shotButton->setScale( 3 );
    shotButton->setPosition(Vec2(origin.x + visibleSize.width - 50 ,origin.y + 100 ));
    this->addChild(shotButton);
    
    
    isShotting = false;
    isDirctionR = false;
    time = 0;
    monsNum = 0;
    dieId = 9999;
    
    bgNode->addChild( brickNode );
    bgNode->addChild( bulletNode );
    this->addChild(bgNode);
    
    bgNode->runAction(Follow::create(player));
    scheduleUpdate();
    
    
    return true;
}


void HelloWorld::update( float dt )
{
    //m_Camera->setPosition3D( Vec3(player->getPositionX(),player->getPositionY(),0) );
    
    Point playerBefore  = player->getPosition();
    
    if( player->getState()== Ninja::State::WalkRight )
    {
        MoveBy* move = MoveBy::create( 0.1, Vec2( 3 , 0) );
        player->runAction(move);
    }
    else if (player->getState()== Ninja::State::WalkLeft )
    {
        MoveBy* move = MoveBy::create( 0.1, Vec2( -3 , 0) );
        player->runAction(move);
    }
    else
        player->stopAllActions();
    
    Point delta = player->getPosition() - playerBefore ;
    
    //bgNode->setPosition( bgNode->getPosition() + delta );
    
    if( isShotting )
        shot();
    
    // collide detect
    auto arrBullet = bulletNode->getChildren();
    for( auto &it : arrBullet )
    {
        for( int j = 0 ; j < monsNum ; ++j )
        if ( it->getBoundingBox().intersectsRect( monsters[ j ]->getBoundingBox() ) && monsters[ j ]->getState() !=  Ninja::State::None )
        {
            //log("shot !!!!!");
            shake( 8 );
            monsters[ j ]->stopAllActions();
            monsters[ j ]->SetState(Ninja::State::None);
            monsterDie( j );
        }
    }
    
    // manageMonster
    if( monsNum > 0 )
    {
        for(int i = 0 ; i < monsNum ; ++i )
        {
            if( monsters[ i ]->getState() == Ninja::State::WalkRight )
            {
                MoveBy* move = MoveBy::create( 0.1, Vec2( 1 , 0) );
                monsters[ i ]->runAction(move);
            }
            
        }
    }
    
    // addMonster
    if( time % 30 == 0 && monsNum < 20)
    {
        addMonster(monsNum);
        ++monsNum;
    }
    
    ++time;
    
}

void HelloWorld::shot()
{
    auto bullet1 = Sprite::create("bullet.png");
    bullet1->setScale(2);
    bullet1->setPosition( player->getPosition() );
    
    auto bullet2 = Sprite::create("bullet.png");
    bullet2->setScale(2);
    bullet2->setPosition( player->getPosition() );
    
    
    auto bullet3 = Sprite::create("bullet.png");
    bullet3->setScale(2);
    bullet3->setPosition( player->getPosition() );
    
    
    bulletNode->addChild( bullet1 );
    bulletNode->addChild( bullet2 );
    bulletNode->addChild( bullet3 );
    
    if ( !isDirctionR )
    {
        bullet1->setScaleX(-2);
        bullet2->setScaleX(-2);
        bullet3->setScaleX(-2);
        
        bullet2->setRotation(-5);
        bullet3->setRotation(5);
        
        MoveBy* move1 = MoveBy::create( 1 , Vec2( -1000 , 0) );
        bullet1->runAction(move1);
        
        MoveBy* move2 = MoveBy::create( 1 , Vec2( -1000/cos(5) , 500*sin(5) ) );
        bullet2->runAction(move2);
        
        MoveBy* move3 = MoveBy::create( 1 , Vec2( -1000/cos(5) , -500*sin(5)) );
        bullet3->runAction(move3);
        
        //log("shot Left");
        
        player->setPositionX( player->getPositionX()+1 );
        shake(1);
        
    }
    else
    {
        bullet2->setRotation(5);
        bullet3->setRotation(-5);
        
        MoveBy* move1 = MoveBy::create( 1 , Vec2( 1000 , 0) );
        bullet1->runAction(move1);
        
        MoveBy* move2 = MoveBy::create( 1 , Vec2( 1000/cos(5) , 500*sin(5)) );
        bullet2->runAction(move2);
        
        MoveBy* move3 = MoveBy::create( 1 , Vec2( 1000/cos(5) , -500*sin(5)) );
        bullet3->runAction(move3);
        
        //log("shot Right");
        player->setPositionX( player->getPositionX()-1 );
        shake(1);
    }

}

void  HelloWorld::onTouchesBegan( const std::vector<cocos2d::Touch*>& touches, Event *unused_event )
{
    for( auto &it : touches)
    {
        Point pos = it->getLocation();
        if ( isDirBall( pos ) )
        {
            dirBall->setPositionX( pos.x );
            if ( pos.x > dirBallRoot->getPositionX() )
            {
                player->SetState(Ninja::State::WalkRight);
                isDirctionR = true;
            }
            else
            {
               player->SetState(Ninja::State::WalkLeft);
               isDirctionR = false;
            }
        }
        if ( isShotButton( pos ) )
        {
            isShotting = true;
        }
    }
    
}

void  HelloWorld::onTouchesMoved( const std::vector<cocos2d::Touch*>& touches, Event *unused_event )
{
    for( auto &it : touches)
    {
        Point pos = it->getLocation();
        if ( isDirBall( pos ) )
        {
            dirBall->setPositionX( pos.x );
            if ( pos.x > dirBallRoot->getPositionX() )
            {
                player->SetState(Ninja::State::WalkRight);
                isDirctionR = true;
            }
            else
            {
                player->SetState(Ninja::State::WalkLeft);
                isDirctionR = false;
            }
        }
    }
//    Point pos = touches[0]->getLocation();
//    //log("touch.x = %f ,touch.y = %f" , pos.x , pos.y );
//    if ( pos.x > player->getPositionX() &&  player->getState()==Ninja::State::WalkLeft )
//        player->SetState(Ninja::State::None);
//    if ( pos.x < player->getPositionX() &&  player->getState()==Ninja::State::WalkRight )
//        player->SetState(Ninja::State::None);
    
}

void HelloWorld::onTouchesEnded( const std::vector<cocos2d::Touch*>& touches, Event *unused_event )
{
    player->SetState(Ninja::State::None);
    //shake();
    isShotting = false;
    
    dirBall->setPositionX( dirBallRoot->getPositionX() );
    
}

const cocos2d::Vec2 HelloWorld::tiledToVec( int i ,int j )const
{
    float X = i * 15.0  ;
    float Y = tiledMap->getContentSize().height - j * 15.0 - 16.0 ;
    return Vec2( X , Y );
}

const cocos2d::Vec2 HelloWorld::vecToTiled( const cocos2d::Vec2 position )const
{
    
    return Vec2( 0 , 0 );
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    if( isCollide( player ) )
    {
        playerBody->applyImpulse( Vec2( 0 , 10000 ) );
        log("jump");
    }
}


void HelloWorld::shake( float f )
{
    this->runAction(Shake::create(0.1f, f ));
}

bool HelloWorld::isDirBall( const cocos2d::Vec2 position )
{
    if( position.getDistance( dirBallRoot->getPosition() ) < 20 )
    {
        log("dirBall");
        return true;
    }
    else
        return false;
}

bool HelloWorld::isShotButton( const cocos2d::Vec2 position )
{
    if( position.getDistance( shotButton->getPosition() ) < 30 )
    {
        log("shotButton");
        return true;
    }
    else
        return false;
}

void HelloWorld::addMonster( int id )
{
    monsters[ id ]->setVisible( true );
    monsters[ id ]->SetState( Ninja::State::WalkRight);
    auto monsterBody = PhysicsBody::createBox( Size(28.0,30.0) ,PhysicsMaterial(0.0f,0.0f,0.0f));
    monsterBody->setMass( 1.0 );
    monsterBody->setRotationEnable(false);
    monsterBody->setDynamic( true );
    monsters[ id ]->setPhysicsBody( monsterBody );
}

void HelloWorld::monsterDie( int id )
{
    if( monsters[ id ]->getPhysicsBody() )
    {
        monsters[ id ]->getPhysicsBody()->applyImpulse( Vec2( -100 , 100 ) );
        //monsters[ id ]->getPhysicsBody()->removeFromWorld();
        auto scheduler = Director::getInstance()->getScheduler();
        //log("******%d",id);
        //std::string name = "xiaota";
        //scheduler->schedule(CC_CALLBACK_1(HelloWorld::dieCallback,this,&id,id),this,0.1f,-1,1,false,"hello");
        
        
        //monsters[ id ]->
//        runAction( Sequence::create( DelayTime::create( 1.0f ) , CallFunc::create([=](){    monsters[ id ]->getPhysicsBody()->removeFromWorld();
//            monsters[ id ]->setRotation(-90);
//            monsters[ id ]->runAction( MoveBy::create( 3 / 60.0 , Vec2(0 , -3 )));
//            
//        }), nullptr) );
        
         if( isCollide( monsters[ id ] ) )
         {
             monsters[ id ]->getPhysicsBody()->removeFromWorld();
             monsters[ id ]->setRotation(-90);
             monsters[ id ]->runAction( MoveBy::create( 3 / 60.0 , Vec2(0 , -5 )));
         }
        
    }
}

bool HelloWorld::isCollide( Ninja* &ninja )
{
    auto arrBrick = brickNode->getChildren();
    for( auto &it : arrBrick )
    {
        if( it->getPosition().distance( ninja->getPosition() ) < 35   && ( it->getPositionY() < ninja->getPositionY() )  )
           {
               float  X = fabsf( it->getPositionX() - ninja->getPositionX() ) ;
               if( X < 10 )
               {
                   log("@@@ninja on floor ");
                   return true;
               }
               
           }
    }
    
    for( int i = 0 ; i < 20 ; ++i  )
    {
        if(  ninja != monsters[ i ] )
        {
            if( monsters[ i ]->getPosition().distance( ninja->getPosition() ) < 35   && ( monsters[ i ]->getPositionY() < ninja->getPositionY()) )
            {
                float  X = fabsf( monsters[ i ]->getPositionX() - ninja->getPositionX() ) ;
                if( X < 15 )
                {
                    log("@@@ninja on monsters[ %d ] ", i );
                    return true;
                }
            }
        }
    }
    
    return false;
}

