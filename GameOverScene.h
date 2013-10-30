//
//  GameOverScene.h
//  SimpleGame
//
//  Created by OSHIMAYOSHIAKI on 2013/10/30.
//
//

#ifndef __SimpleGame__GameOverScene__
#define __SimpleGame__GameOverScene__

#include <iostream>
#include "cocos2d.h"

class GameOverLayer : public cocos2d::CCLayerColor {
public:
    GameOverLayer():_label(NULL) {};
    virtual ~GameOverLayer();
    bool init();
    CREATE_FUNC(GameOverLayer);
    
    void gameOverDone();
    
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, _label, Label1);
};

class GameOverScene : public cocos2d::CCScene {
public:
    GameOverScene():_layer(NULL) {};
    ~GameOverScene();
    bool init();
    CREATE_FUNC(GameOverScene);
    
    CC_SYNTHESIZE_READONLY(GameOverLayer*, _layer, Layer);
};

#endif /* defined(__SimpleGame__GameOverScene__) */
