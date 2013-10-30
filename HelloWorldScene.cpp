#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "GameOverScene.h"

USING_NS_CC;


CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    _targets = new CCArray;
    _projetiles = new CCArray;
    _projectilesDestroyed = 0;
    
    if (CCLayerColor::initWithColor(ccc4(255, 255, 255, 255))) {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        CCSprite *player = CCSprite::create("Player.png",CCRectMake(0, 0, 54, 80));
        player->setPosition(ccp(player->getContentSize().width/2, winSize.height/2));
        this->addChild(player);
    }
    this->setTouchEnabled(true);
    
    this->schedule(schedule_selector(HelloWorld::gameLogic), 1.0);
    
    this->schedule(schedule_selector(HelloWorld::update));
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background-music-aac.wav", true);
    
    return true;
    
//    if (!CCLayerColor::initWithColor(ccc4(255, 255, 255, 255))) {
//        return false;
//    }
}

HelloWorld::~HelloWorld() {
    if (_targets) {
        _targets->release();
        _targets = NULL;
    }
    if (_projetiles) {
        _projetiles->release();
        _projetiles = NULL;
    }
}

HelloWorld::HelloWorld()
:_targets(NULL)
,_projetiles(NULL)
{
}



void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();
    

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    exit(0);
#endif
}

void HelloWorld::addTarget() {
    CCSprite *target = CCSprite::create("Target.png",CCRectMake(0, 0, 27, 40));
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    int minY = target->getContentSize().height/2;
    int maxY = winSize.height - target->getContentSize().height/2;
    int rangeY = maxY - minY;
    int actualY = (rand() % rangeY) + minY;
    target->setPosition(ccp(winSize.width+(target->getContentSize().width/2), actualY));
    target->setTag(1);
    _targets->addObject(target);
    this->addChild(target);
    
    int minDuration = (int)2.0;
    int maxDuration = (int)4.0;
    int rangeDuration = maxDuration - minDuration;
    int actualDuration = (rand() % rangeDuration) + minDuration;
    
    
    CCFiniteTimeAction* actionMove = CCMoveTo::create((float)actualDuration, ccp(0 - target->getContentSize().width/2, actualY));
    CCFiniteTimeAction* actionMoveDone = CCCallFuncN::create(this, callfuncN_selector(HelloWorld::spriteMoveFinished));
    target->runAction(CCSequence::create(actionMove,actionMoveDone,NULL));
}

void HelloWorld::spriteMoveFinished(CCNode* sender) {
    CCSprite *sprite = (CCSprite *)sender;
    this->removeChild(sprite,true);
    
    if (sprite->getTag() == 1) {
        _targets->removeObject(sprite);
        GameOverScene *gameOverScene = GameOverScene::create();
        gameOverScene->getLayer()->getLabel1()->setString("You Lose :[");
        CCDirector::sharedDirector()->replaceScene(gameOverScene);
    }else if (sprite->getTag() == 2) {
        _projetiles->removeObject(sprite);
    }
}

void HelloWorld::gameLogic(float dt) {
    this->addTarget();
}

void HelloWorld::ccTouchesEnded(CCSet *touches, CCEvent *event) {
    CCTouch* touch = (CCTouch*)(touches->anyObject());
    CCPoint location = touch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSprite *projectile = CCSprite::create("Projectile.png",CCRectMake(0, 0, 20, 20));
    projectile->setPosition(ccp(20, winSize.height/2));
    projectile->setTag(2);
    _projetiles->addObject(projectile);
    
    int offX = location.x - projectile->getPosition().x;
    int offY = location.y - projectile->getPosition().y;
    
    if (offX <= 0) return;
    
    this->addChild(projectile);
    
    int realX = winSize.width + (projectile->getContentSize().width/2);
    float ratio = (float)offY / (float)offX;
    int realY = (realX * ratio) + projectile->getPosition().y;
    CCPoint realDest = ccp(realX, realY);
    
    int offRealX = realX - projectile->getPosition().x;
    int offRealY = realY - projectile->getPosition().y;
    float length = sqrtf((offRealX * offRealX) + (offRealY * offRealY));
    float velocity = 480/1; // 480pixels/1sec
    float realMoveDuration = length/velocity;
    
    projectile->runAction(CCSequence::create(CCMoveTo::create(realMoveDuration, realDest),CCCallFuncN::create(this, callfuncN_selector(HelloWorld::spriteMoveFinished)),NULL));
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("pew-pew-lei.wav");
}

void HelloWorld::update(float dt) {
    CCArray *projectilesToDelete = new CCArray;
    CCArray *targetsToDelete = new CCArray;
    CCObject *it = NULL;
    CCObject *jt = NULL;
    
    CCARRAY_FOREACH(_projetiles, it) {
        CCSprite *projectile = dynamic_cast<CCSprite*>(it);
        CCRect projectileRect = CCRectMake(projectile->getPosition().x - (projectile->getContentSize().width/2),
                                           projectile->getPosition().y - (projectile->getContentSize().height/2),
                                           projectile->getContentSize().width,
                                           projectile->getContentSize().height);
        CCARRAY_FOREACH(_targets, jt) {
            CCSprite *target = dynamic_cast<CCSprite*>(jt);
            CCRect targetRect = CCRectMake(target->getPosition().x - (target->getContentSize().width/2),
                                           target->getPosition().y - (target->getContentSize().width/2),
                                           target->getContentSize().width,
                                           target->getContentSize().height);
            if (projectileRect.intersectsRect(targetRect)) {
                targetsToDelete->addObject(target);
                projectilesToDelete->addObject(projectile);
            }
        }
    }
    
    CCARRAY_FOREACH(targetsToDelete, jt) {
        CCSprite *target = dynamic_cast<CCSprite*>(jt);
        _targets->removeObject(target);
        this->removeChild(target, true);
        
        _projectilesDestroyed++;
        if (_projectilesDestroyed >= 5) {
            GameOverScene *gameOverScene = GameOverScene::create();
            gameOverScene->getLayer()->getLabel1()->setString("You Win!");
            CCDirector::sharedDirector()->replaceScene(gameOverScene);
        }
    }
    
    CCARRAY_FOREACH(projectilesToDelete, it) {
        CCSprite *projectile = dynamic_cast<CCSprite*>(it);
        _projetiles->removeObject(projectile);
        this->removeChild(projectile, true);
    }
    
    projectilesToDelete->release();
    targetsToDelete->release();
    
    
}


