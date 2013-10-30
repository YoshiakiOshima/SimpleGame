#include "AppDelegate.h"
#include "HelloWorldScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);
	
    // turn on display FPS
    pDirector->setDisplayStats(true);
    
//マルチディスプレイ対応コード-----------------------------------------------------------------------
    CCEGLView* view = CCDirector::sharedDirector()->getOpenGLView();
    CCSize screeenSize = view->getFrameSize();
    
//    std::vector<std::string> searchPaths;
//    std::vector<std::string> resDirOrders;
//    
//    TargetPlatform platform = CCApplication::sharedApplication()->getTargetPlatform();
//    
//    //iOSの場合
//    if (platform == kTargetIphone || platform == kTargetIpad) {
//        searchPaths.push_back("Published-iOS");
//        if (screeenSize.height > 480) {
//            pDirector->setContentScaleFactor(2.f);
//            if (screeenSize.width == 1136.0 || screeenSize.height == 1136.0) {
//                //Retina4-inch
//                resDirOrders.push_back("resources-iphonehd-4-inc");
//                CCEGLView::sharedOpenGLView()->setDesignResolutionSize(568, 320, kResolutionShowAll);
//            }else {
//                //Retina3.5-inch
//                resDirOrders.push_back("resources-iphonehd");
//                CCEGLView::sharedOpenGLView()->setDesignResolutionSize(480, 320, kResolutionShowAll);
//            }
//        }else {
//            //Retina以外
//            resDirOrders.push_back("resources-iphone");
//            CCEGLView::sharedOpenGLView()->setDesignResolutionSize(320, 480, kResolutionExactFit);
//        }
//    }else {
//        //iOS以外
//        searchPaths.push_back("Published-iOS");
//        resDirOrders.push_back("resources-iphone");
//        CCEGLView::sharedOpenGLView()->setDesignResolutionSize(320, 480, kResolutionExactFit);
//    }
//    
//    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);
//    CCFileUtils::sharedFileUtils()->setSearchResolutionsOrder(resDirOrders);
    
    if (screeenSize.height == 1136.0) {
        CCEGLView::sharedOpenGLView()->setDesignResolutionSize(320, 568, kResolutionExactFit);
    }else {
        CCEGLView::sharedOpenGLView()->setDesignResolutionSize(320, 480, kResolutionExactFit);
    }
    CCEGLView::sharedOpenGLView()->setDesignResolutionSize(320, 480, kResolutionExactFit);
//ここまで-------------------------------------------------------------------------------------
    

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = HelloWorld::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
