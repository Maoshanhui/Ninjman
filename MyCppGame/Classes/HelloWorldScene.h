#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Ninja.h"

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    void setPhyWorld(cocos2d::PhysicsWorld* world){m_world = world;}
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    virtual void onTouchesBegan( const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event ) override ;
    
    virtual void onTouchesMoved( const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event ) override ;
    
    virtual void onTouchesEnded( const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event ) override ;

    const cocos2d::Vec2 tiledToVec( int i ,int j )const;
    
    const cocos2d::Vec2 vecToTiled( const cocos2d::Vec2 position )const;
    
    void update( float dt );
    
    void shot( );
    
    void shake( float f );
    
    bool isDirBall( const cocos2d::Vec2 position );
    
    bool isShotButton( const cocos2d::Vec2 Position );
    
    void addMonster( int id );
    
    void monsterDie( int id );
    
    bool isCollide( Ninja* &ninja );
    
    
private:
    cocos2d::PhysicsWorld* m_world;
    
    bool isShotting;
    
    bool isDirctionR;
    
    cocos2d::Camera* m_Camera;
    
    cocos2d::Size visibleSize;
    
    
    cocos2d::TMXTiledMap* tiledMap;
    int mapWidth;
    int mapHeight;
    cocos2d::TMXLayer* backgroudLayer;
    cocos2d::TMXObjectGroup* bornPoints;
    
    
    // direction ball
    cocos2d::Sprite* dirBall;
    cocos2d::Node* dirBallRoot;
    
    // shot button
    cocos2d::Sprite* shotButton;
    
    Ninja* player;
    
    Ninja* monsters[ 20 ];
    
    cocos2d::Sprite* bullet;
    
    cocos2d::PhysicsBody *playerBody;
    
    cocos2d::Node* bgNode;
    
    cocos2d::Node* bulletNode;
    
    cocos2d::Node* brickNode;
    
    int time;
    
    int monsNum;
    
    int dieId;
    
};

#endif // __HELLOWORLD_SCENE_H__
