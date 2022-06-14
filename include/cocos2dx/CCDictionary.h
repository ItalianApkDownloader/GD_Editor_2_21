#pragma once

#include "GDAPI_Macros.h"

#include <string>
#include "CCArray.h"
#include "CCString.h"

NS_CC_BEGIN

class CCObject;
class CCDictionary;

enum CCDictType
{
	kCCDictUnknown = 0,
	kCCDictStr,
	kCCDictInt
};

#define MAX_KEY_LEN 256

class CC_DLL CCDictElement
{
public:
	char _szKey[MAX_KEY_LEN];
	intptr_t _iKey;
	CCObject *_pObject;
	unsigned char _unknown[8 * sizeof(int)];

private:
	CCDictElement(int iKey, CCObject *pObject);
	CCDictElement(const char *pszKey, CCObject *pObject);

public:
	~CCDictElement();

	int getIntKey() const;
	CCObject *getObject() const;
	const char *getStrKey() const;
};

class CC_DLL CCDictionary : public CCObject
{
public:
	CCDictElement *_pElements;
	CCDictType _eDictType;

public:
	CCDictionary();
	virtual ~CCDictionary();

	static CCDictionary *create();
	static CCDictionary *createWithContentsOfFile(const char *path);
	static CCDictionary *createWithContentsOfFileThreadSafe(const char *path);
	static CCDictionary *createWithDictionary(CCDictionary *pDict);

	CCArray *allKeys();
	CCArray *allKeysForObject(CCObject *pObject);
	const char *charForKey(const std::string &key);
	unsigned int count();
	std::string getFirstKey();
	CCObject *objectForKey(const std::string &key);
	CCObject *objectForKey(int key);
	CCObject *randomObject();
	void removeAllObjects();
	void removeObjectForElememt(CCDictElement *pElement);
	void removeObjectForKey(const std::string &key);
	void removeObjectForKey(int key);
	void removeObjectsForKeys(CCArray *pArray);
	void setObject(CCObject *pObject, const std::string &key);
	void setObject(CCObject *pObject, int key);
	const CCString *valueForKey(const std::string &key);
	const CCString *valueForKey(int key);
	bool writeToFile(const char *path);

	void setIntForKey(int value, const std::string& key) {
		auto str = CCString::createWithFormat("%d",value);
		this->setObject(str,key);
	}

	int getIntForKey(const std::string& key) {
		auto result = this->objectForKey(key);
		if(result){
			return reinterpret_cast<CCString*>(result)->intValue();
		}else{
			return 0;
		}
	}

	void setStringForKey(std::string value, const std::string& key) {
		auto str = CCString::createWithFormat("%s",value.c_str());
		this->setObject(str,key);
	}

	std::string getStringForKey(const std::string& key) {
		auto result = this->objectForKey(key);
		if(result){
			return reinterpret_cast<CCString*>(result)->_sString;
		}else{
			return "";
		}
	}



	void setDictionaryForKey(CCDictionary *dict, const std::string& key) {
		this->setObject(dict,key);
	}


	CCDictionary * getDictionaryForKey(const std::string& key) {
		auto result = this->objectForKey(key);
		if(result){
			return reinterpret_cast<CCDictionary*>(result);
		}else{
			return CCDictionary::create();
		}
	}

private:
	void setObjectUnSafe(CCObject *pObject, const std::string &key);
	void setObjectUnSafe(CCObject *pObject, int key);

public:
	virtual CCObject *copyWithZone(CCZone *pZone);
	virtual void acceptVisitor(CCDataVisitor &rDataVisitor);
};

NS_CC_END