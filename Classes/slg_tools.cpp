#include "slg_tools.h"

CCRect VisibleRect::s_visibleRect;

void VisibleRect::lazyInit()
{
    if (s_visibleRect.size.width == 0.0f && s_visibleRect.size.height == 0.0f)
    {
        CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
        s_visibleRect.origin = pEGLView->getVisibleOrigin();
        s_visibleRect.size = pEGLView->getVisibleSize();
    }
}

CCRect VisibleRect::getVisibleRect()
{
    lazyInit();
    return CCRectMake(s_visibleRect.origin.x, s_visibleRect.origin.y, s_visibleRect.size.width, s_visibleRect.size.height);
}

CCPoint VisibleRect::left()
{
    lazyInit();
    return ccp(s_visibleRect.origin.x, s_visibleRect.origin.y+s_visibleRect.size.height/2);
}

CCPoint VisibleRect::right()
{
    lazyInit();
    return ccp(s_visibleRect.origin.x+s_visibleRect.size.width, s_visibleRect.origin.y+s_visibleRect.size.height/2);
}

CCPoint VisibleRect::top()
{
    lazyInit();
    return ccp(s_visibleRect.origin.x+s_visibleRect.size.width/2, s_visibleRect.origin.y+s_visibleRect.size.height);
}

CCPoint VisibleRect::bottom()
{
    lazyInit();
    return ccp(s_visibleRect.origin.x+s_visibleRect.size.width/2, s_visibleRect.origin.y);
}

CCPoint VisibleRect::center()
{
    lazyInit();
    return ccp(s_visibleRect.origin.x+s_visibleRect.size.width/2, s_visibleRect.origin.y+s_visibleRect.size.height/2);
}

CCPoint VisibleRect::leftTop()
{
    lazyInit();
    return ccp(s_visibleRect.origin.x, s_visibleRect.origin.y+s_visibleRect.size.height);
}

CCPoint VisibleRect::rightTop()
{
    lazyInit();
    return ccp(s_visibleRect.origin.x+s_visibleRect.size.width, s_visibleRect.origin.y+s_visibleRect.size.height);
}

CCPoint VisibleRect::leftBottom()
{
    lazyInit();
    return s_visibleRect.origin;
}

CCPoint VisibleRect::rightBottom()
{
    lazyInit();
    return ccp(s_visibleRect.origin.x+s_visibleRect.size.width, s_visibleRect.origin.y);
}



float GetSpriteTextureLength(CCSprite* sprite)
{
	return sprite->getTextureRect().getMaxX()-sprite->getTextureRect().getMinX();
}

float GetSpriteTextureHeight(CCSprite* sprite)
{
	return sprite->getTextureRect().getMaxY()-sprite->getTextureRect().getMinY();
}

//int ConvertCharsetTool::code_convert( const char *from_charset, const char *to_charset, const char *inbuf, size_t inlen, char *outbuf, size_t outlen )
//{
//	iconv_t cd;
//	const char *temp = inbuf;
//	const char **pin = &temp;
//	char **pout = &outbuf;
//	memset(outbuf,0,outlen);
//	cd = iconv_open(to_charset,from_charset);
//	if(cd==0) return -1;
//	if(iconv(cd,pin,&inlen,pout,&outlen)==-1) return -1;
//	iconv_close(cd);
//	return 0;
//}
///*UTF8 To GB2312*/
//string ConvertCharsetTool::u2a( const char *inbuf )
//{
//	size_t inlen = strlen(inbuf);
//	char * outbuf = new char[inlen * 2 + 2];
//	string strRet;
//	if(code_convert("utf-8", "gb2312", inbuf, inlen, outbuf, inlen * 2 + 2) == 0)
//	{
//		strRet = outbuf;
//	}
//	delete [] outbuf;
//	return strRet;
//}
///*GB2312 To UTF8*/ //使用中文时用该函数转换
//string ConvertCharsetTool::a2u( const char *inbuf )
//{
//	size_t inlen = strlen(inbuf);
//	char * outbuf = new char[inlen * 2 + 2];
//	string strRet;
//	if(code_convert("gb2312", "utf-8", inbuf, inlen, outbuf, inlen * 2 + 2) == 0)
//	{
//		strRet = outbuf;
//	}
//	delete [] outbuf;
//	return strRet;
//}