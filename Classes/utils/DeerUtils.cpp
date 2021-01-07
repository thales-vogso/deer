/****************************************************************************
  
 *	@desc	鹿纪元公用功能
 *	@date	2015-1-8
 *	@author	110101
     
 *	@file	DeerUtils/DeerUtils.cpp
 *  @modify	null
 ******************************************************************************/

#include "DeerUtils.h"
USING_NS_CC;
DEERUTILS_BEGIN

Node* seekChildByName(Node* root, const std::string& name)
{
    if (!root)
    {
        return nullptr;
    }
    if (root->getName() == name)
    {
        return root;
    }
    const auto& arrayRootChildren = root->getChildren();
    for (auto& subNode : arrayRootChildren)
    {
        auto child = dynamic_cast<Node*>(subNode);
        if (child)
        {
            auto res = seekChildByName(child, name);
            if (res != nullptr)
            {
                return res;
            }
        }
    }
    return nullptr;
}

std::string autoKeepToString(float num)
{
    char s[10];
    sprintf(s, "%g", num);
    return std::string(s);
}
cocos2d::Color3B hexToColor3BByLevel(int nLevel)
{
    std::string hStr;
    const char* colorDefine[7]=
    {
        "#c1c1ca",
        "#8AF2B3",
        "#73D8F9",
        "#ce49ff",
        "#ffb034",
        "#ee4225"
    };
    if( nLevel >= 1 && nLevel < 7)
    {
        //white
        hStr = colorDefine[nLevel-1];
    }
    else
    {
        hStr = colorDefine[0];
    }
    return hexToColor3B(hStr);
}
cocos2d::Color3B hexToColor3B(std::string str)
{
    std::string s1 = str.substr(1,2);
    int r = (int)strtol(s1.c_str(), NULL, 16);
    s1 = str.substr(3,2);
    int g = (int)strtol(s1.c_str(), NULL, 16);
    s1 = str.substr(5,2);
    int b = (int)strtol(s1.c_str(), NULL, 16);
    cocos2d::Color3B color = cocos2d::Color3B(r,g,b);
    
    return color;

}
bool reqResultCompare(rapidjson::Document &data,int resultCode)
{
    if(data.HasMember("code"))
    {
        if(data["code"].IsInt())
        {
            if(data["code"].GetInt() == resultCode)
            {
                return true;
            }
        }
        else if(data["code"].IsString())
        {
            char str[20];
            sprintf(str, "%d",resultCode);
            const std::string str_1  =str;
            //* str_1 = str;
            if(data["code"].GetString() == str_1)
            {
                return true;
            }
        }
    }
    return false;
}
int getTextRowNum(char* str,long length,int fontSize,int textWidth)
{
    char *p = 0;
    int count = 0;
    int rowNum = 1;
    int preNum = 0;
    
    if (!str || length <= 0) {
        return 0;
    }
    
    for(p=str; p<str+length; p++) {
        if (UTF8_ASCII(*p) || (UTF8_FIRST(*p)) ) {
            count++;
            preNum++;
            
            if(preNum >= textWidth/fontSize || *p == '\n')
            {
                preNum = 0;
                rowNum++;
            }
        }
    }
    
    return rowNum;
}
int getTextRowNum(char* str,long length)
{
    char *p = 0;
    int rowNum = 1;
    if (!str || length <= 0) {
        return 0;
    }
    
    for(p=str; p<str+length; p++) {
        if (UTF8_ASCII(*p) || (UTF8_FIRST(*p)) ) {
            if(*p == '\n')
            {
                rowNum++;
            }
        }
    }
    
    return rowNum;
}
float getAngleTwoPoint(cocos2d::Vec2 point1,cocos2d::Vec2 point2)
{
    float dis = point1.getDistance(point2);
    if(dis > 0)
    {
        float y   = fabsf(point1.y - point2.y);
        float angel = asinf(y/dis)*180.0/3.14;
        return angel;
    }
    return  0;
}
std::string insertEnterInChineseStr(std::string str,int point)
{
    long length = str.length();
    if(point>0 && point < length)
    {
        str.insert(length-point, "\n");
    }
    //for()
    
    return str;
}
DEERUTILS_END