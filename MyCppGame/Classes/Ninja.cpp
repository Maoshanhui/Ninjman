//
//  Ninja.cpp
//  MyCppGame
//
//  Created by 毛善辉 on 15/10/4.
//
//

#include "Ninja.h"

const std::string ID00 = "player.png";
const std::string ID01 = "monster.png";

Ninja::Ninja()
{
    m_State = State::None ;
}


Ninja::~Ninja()
{
}

Ninja* Ninja::create( int RoleId )
{
    Ninja *pRet = new Ninja( );
    if ( pRet && pRet->initMySprite( RoleId ) )
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool Ninja::initMySprite( int RoleId )
{
    
    m_RoleId = RoleId ;
    
    std::string id = "" ;
    switch (RoleId)
    {
        case 0 :
        {
            id = ID00 ;
            auto pTex = cocos2d::TextureCache::getInstance()->addImage( id );
            
            m_size.height = pTex->getContentSize( ).height / 11;
            m_size.width = pTex->getContentSize( ).width / 12;
            
            m_pSprMain = cocos2d::Sprite::create( id , cocos2d::Rect( m_size.width * 0 , m_size.height * 1 , m_size.width , m_size.height ) );
            m_pSprMain->setScale( 1.4 );
            this->addChild( m_pSprMain );
            m_pSprMain->setAnchorPoint( cocos2d::Point( 0.5 , 0.65 ) );
        }break;
        case 1 :
        {
            id = ID01 ;
            auto pTex = cocos2d::TextureCache::getInstance()->addImage( id );
            
            m_size.height = pTex->getContentSize( ).height / 11;
            m_size.width = pTex->getContentSize( ).width / 12;
            
            m_pSprMain = cocos2d::Sprite::create( id , cocos2d::Rect( m_size.width * 0 , m_size.height * 3 , m_size.width , m_size.height ) );
            m_pSprMain->setScale( 1.4 );
            this->addChild( m_pSprMain );
            m_pSprMain->setAnchorPoint( cocos2d::Point( 0.5 , 0.65 ) );
        }break;
            
        default:
            break;
    }
    
    
    
    return true;
}

void Ninja::SetState( State type )
{
    
    std::string id = "" ;
    switch (m_RoleId)
    {
        case 0 :
        {
            id = ID00 ;
            m_State = type ;
            auto pTex = cocos2d::TextureCache::getInstance()->addImage( id );
            
            
            
            switch ( m_State )
            {
                    
                case Ninja::State::None:
                    break;
                    
                case Ninja::State::WalkLeft:
                {
                    m_pSprMain->setTexture( id ) ;
                    m_pSprMain->setTextureRect( cocos2d::Rect( m_size.width * 0 , m_size.height * 1 ,m_size.width , m_size.height ) ) ;
                    cocos2d::log( "WalkLeft" ) ;
                }
                    break;
                    
                case Ninja::State::WalkRight:
                {
                    m_pSprMain->setTexture( id ) ;
                    m_pSprMain->setTextureRect( cocos2d::Rect( m_size.width * 11 , m_size.height * 1 , m_size.width , m_size.height ) ) ;
                    cocos2d::log( "WalkRight" ) ;
                }
                    break;
            }
        }break;
        case 1 :
        {
            id = ID01 ;
            m_State = type ;
            auto pTex = cocos2d::TextureCache::getInstance()->addImage( id );
            
            
            
            switch ( m_State )
            {
                    
                case Ninja::State::None:
                    break;
                    
                case Ninja::State::WalkLeft:
                {
                    m_pSprMain->setTexture( id ) ;
                    m_pSprMain->setTextureRect( cocos2d::Rect( m_size.width * 0 , m_size.height * 3 ,m_size.width , m_size.height ) ) ;
                    cocos2d::log( "WalkLeft" ) ;
                }
                    break;
                    
                case Ninja::State::WalkRight:
                {
                    m_pSprMain->setTexture( id ) ;
                    m_pSprMain->setTextureRect( cocos2d::Rect( m_size.width * 11 , m_size.height * 3 , m_size.width , m_size.height ) ) ;
                    cocos2d::log( "WalkRight" ) ;
                }
                    break;
            }
        }break;
    }
    
    
}

const Ninja::State Ninja::getState()
{
    
    return m_State;

}

