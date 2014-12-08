#ifndef __VISIBLERECT_H__
#define __VISIBLERECT_H__

#include "cocos2d.h"
USING_NS_CC;

class VisibleRect
{
public:
    static CCRect getVisibleRect();

    static CCPoint left();
    static CCPoint right();
    static CCPoint top();
    static CCPoint bottom();
    static CCPoint center();
    static CCPoint leftTop();
    static CCPoint rightTop();
    static CCPoint leftBottom();
    static CCPoint rightBottom();
private:
    static void lazyInit();
    static CCRect s_visibleRect;
};
#define MATH_PI 3.1415926

float GetSpriteTextureLength(CCSprite* sprite);
float GetSpriteTextureHeight(CCSprite* sprite);

//定义类的int型成员变量的getter宏
#define IntGetter(_valueName_) \
private: int m_n##_valueName_;\
public: int Get##_valueName_(){return m_n##_valueName_;}

#endif /* __VISIBLERECT_H__ */
