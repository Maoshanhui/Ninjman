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
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    virtual bool onTouchBegan(  cocos2d::Touch* touch, cocos2d::Event *unused_event ) override ;
    
    virtual void onTouchMoved(  cocos2d::Touch* touch, cocos2d::Event *unused_event )  override;
    
    virtual void onTouchEnded(  cocos2d::Touch* touch, cocos2d::Event *unused_event )override ;

    const cocos2d::Vec2 tiledToVec( int i ,int j )const;
    
    const cocos2d::Vec2 vecToTiled( const cocos2d::Vec2 position )const;
    
    void update( float dt );
    
    void shot( );
    
    
    cocos2d::Camera* m_Camera;
    
    cocos2d::Size visibleSize;
    
    
    cocos2d::TMXTiledMap* tiledMap;
    int mapWidth;
    int mapHeight;
    cocos2d::TMXLayer* backgroudLayer;
    cocos2d::TMXObjectGroup* bornPoints;
    
    
    // direction ball
    cocos2d::Sprite* ball;
    
    // button
    cocos2d::Sprite* jumpButton;
    
    Ninja* player;
    
    std::vector< Ninja* > monsters;
    
    cocos2d::Sprite* bullet;
    
};

#endif // __HELLOWORLD_SCENE_H__
