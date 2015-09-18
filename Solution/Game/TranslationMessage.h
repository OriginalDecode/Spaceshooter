#pragma once
#include "ComponentEnums.h"

class TranslationMessage
{
public:
	TranslationMessage(eTranslationType aType, float aAmount);

	eTranslationType GetTranslationType() const;
	float GetTranslationAmount() const;

private:
	eTranslationType myTranslationType;
	float myTranslationAmount;
};

inline eTranslationType TranslationMessage::GetTranslationType() const
{
	return myTranslationType;
}

inline float TranslationMessage::GetTranslationAmount() const
{
	return myTranslationAmount;
}