/* 此软件名称为 XEIM 飞鸽传书，受 GPL 开源协议保护，任何违反本协议的人我们将尽最大努力追究其法律责任。
XEIM 飞鸽传书官方网站：http://www.freeeim.com/xeim/
XEIM 飞鸽传书官方网站：http://bbs.freeeim.com/
*/
#ifndef XEIM_FUNCTION
#define XEIM_FUNCTION


//做为解Url使用
char CharToInt(char ch);
char StrToBin(char *str);
string UrlGB2312Decode(string str);

// 获取程序的目录
void GetProgramDirectory(char *buf);

#endif // XEIM_FUNCTION