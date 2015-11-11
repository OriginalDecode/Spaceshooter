#include "stdafx.h"
//#include <Engine.h>
#include "LoadingScreen.h"
#include <InputWrapper.h>
#include <Sprite.h>
#include <XMLReader.h>

Screen::Screen()
	: myBackground(nullptr)
	, myMessages(8)
{
};
Screen::~Screen()
{
	delete myBackground;
	myBackground = nullptr;
};

LoadingScreen::LoadingScreen(CU::InputWrapper* anInputWrapper, const volatile bool& aLevelIsLoading, const int& aLevelID)
	: myScreens(8)
	, myTextIndex(0)
	, myIsDone(false)
	, myInputWrapper(anInputWrapper)
	, myLevelID(aLevelID)
	, myLevelIsLoading(aLevelIsLoading)
{
	OnResize(Prism::Engine::GetInstance()->GetWindowSize().x, Prism::Engine::GetInstance()->GetWindowSize().y);
	ReadXML();
}

LoadingScreen::~LoadingScreen()
{
	myScreens.DeleteAll();
}

void LoadingScreen::Render()
{
	myScreens[myLevelID - 1]->myBackground->Render(myWindowMiddle);

	Prism::Engine::GetInstance()->PrintText(myScreens[myLevelID - 1]->myMessages[0].myMessage
		, { myWindowMiddle.x - 600.f, myWindowMiddle.y * 0.7f }, Prism::eTextType::RELEASE_TEXT);

	if (myLevelIsLoading == false)
	{
		Prism::Engine::GetInstance()->PrintText("Press [Space] to enter level!"
			, { myWindowMiddle.x * 0.7f, myWindowMiddle.y * 1.4f }, Prism::eTextType::RELEASE_TEXT);
	}
}

void LoadingScreen::Update(float aDeltaTime)
{
	if (myLevelIsLoading == false && (myInputWrapper->KeyDown(DIK_SPACE) == true || myInputWrapper->KeyDown(DIK_ESCAPE) == true))
	{
		myIsDone = true;
		return;
	}
}

void LoadingScreen::OnResize(int aWidth, int aHeight)
{
	// set size

	myWindowMiddle.x = float(aWidth) / 2.f;
	myWindowMiddle.y = -float(aHeight) / 2.f;
}

void LoadingScreen::Reset()
{
	myIsDone = false;
	myTextIndex = 0;

	for (int i = 0; i < myScreens.Size(); i++)
	{
		for (int j = 0; j < myScreens[i]->myMessages.Size(); j++)
		{
			myScreens[i]->myMessages[j].myCurrentTime = 0.f;
		}
	}
}

void LoadingScreen::ReadXML()
{
	CU::Vector2<float> windowSize = { float(Prism::Engine::GetInstance()->GetWindowSize().x), float(Prism::Engine::GetInstance()->GetWindowSize().y) };
	XMLReader reader;
	reader.OpenDocument("Data/Menu/MN_loading_screen.xml");
	tinyxml2::XMLElement* rootElement = reader.ForceFindFirstChild("root");

	for (tinyxml2::XMLElement* screenElement = reader.FindFirstChild(rootElement, "screen"); screenElement != nullptr;
		screenElement = reader.FindNextElement(screenElement, "screen"))
	{
		std::string aSpritePath = "";
		int levelID = 0;

		reader.ForceReadAttribute(reader.FindFirstChild(screenElement, "ID"), "value", levelID);
		if (levelID != myScreens.Size())
		{
			DL_ASSERT("[LoadingScreen] Wrong ID-number in MN_loading_screen.xml! The numbers should be counting up, in order, starting from 0.");
		}

		reader.ForceReadAttribute(reader.FindFirstChild(screenElement, "background"), "path", aSpritePath);

		Screen* newScreen = new Screen();
		newScreen->myBackground = new Prism::Sprite(aSpritePath, windowSize, windowSize / 2.f);

		tinyxml2::XMLElement* textElement = reader.FindFirstChild(screenElement, "text");

		TimedMessage newMessage;
		newMessage.myCurrentTime = 0.f;

		reader.ForceReadAttribute(textElement, "value", newMessage.myMessage);
		reader.ForceReadAttribute(textElement, "time", newMessage.myMaxTime);

		newScreen->myMessages.Add(newMessage);

		myScreens.Add(newScreen);
	}

	reader.CloseDocument();
}