#include "stdafx.h"
#include "SpriteFontLoader.h"
#include <iostream>
#include <filesystem>

SpriteFont* SpriteFontLoader::LoadContent(const ContentLoadInfo& loadInfo)
{
	const auto pReader = new BinaryReader();
	pReader->Open(loadInfo.assetFullPath);

	if (!pReader->Exists())
	{
		Logger::LogError(L"Failed to read the assetFile!\nPath: \'{}\'", loadInfo.assetSubPath);
		return nullptr;
	}

	TODO_W5(L"Implement SpriteFontLoader >> Parse .fnt file")

		if ((pReader->Read<char>() != 'B' || pReader->Read<char>() != 'M' || pReader->Read<char>() != 'F'))
		{
			std::cout << "Error: SpriteFontLoader::LoadContent > Not a valid .fnt font\n";
			return nullptr;
		}



	if ((int)pReader->Read<char>() < 3)
	{
		std::cout << "Error: SpriteFontLoader::LoadContent > Only .fnt version 3 is supported\n";
		return nullptr;
	}
	std::cout << "test\n";
	//Valid .fnt file >> Start Parsing!
	//use this SpriteFontDesc to store all relevant information (used to initialize a SpriteFont object)
	SpriteFontDesc fontDesc{};

	//**********
	// BLOCK 0 *
	//**********
	//Retrieve the blockId and blockSize
	int blockId0 = (int)pReader->Read<char>();
	blockId0;
	int blockSize0 = pReader->Read<int>();
	blockSize0;

	fontDesc.fontSize = pReader->Read<short>();

	pReader->MoveBufferPosition(12);
	char nextChar = pReader->Read<char>();
	while (nextChar != '\0')
	{
		fontDesc.fontName += nextChar;
		nextChar = pReader->Read<char>();
	}
	//Retrieve the FontSize [fontDesc.fontSize]
	//Move the binreader to the start of the FontName [BinaryReader::MoveBufferPosition(...) or you can set its position using BinaryReader::SetBufferPosition(...))
	//Retrieve the FontName [fontDesc.fontName]
	//...

	//**********
	// BLOCK 1 *
	//**********
	//Retrieve the blockId and blockSize
	int blockId1 = static_cast<int>(pReader->Read<char>());
	blockId1;
	int blockSize1 = pReader->Read<int>();
	blockSize1;
	pReader->MoveBufferPosition(4);

	fontDesc.textureWidth = pReader->Read<short>();
	fontDesc.textureHeight = pReader->Read<short>();

	if (pReader->Read<short>() > 1)
	{
		std::cout << "Error: Only one texture per font is allowed!\n";
	}

	pReader->MoveBufferPosition(5);

	//Retrieve Texture Width & Height [fontDesc.textureWidth/textureHeight]
	//Retrieve PageCount
	//> if pagecount > 1
	//	> Log Error (Only one texture per font is allowed!)
	//Advance to Block2 (Move Reader)
	//...

	//**********
	// BLOCK 2 *
	//**********
	//Retrieve the blockId and blockSize
	int blockId2 = static_cast<int>(pReader->Read<char>());
	blockId2;
	auto blockSize2 = pReader->Read<int>();
	blockSize2;
	auto PageName = pReader->ReadNullString();
	auto Path = std::filesystem::path::path(loadInfo.assetFullPath.parent_path()) / PageName;
	fontDesc.pTexture = ContentManager::Load<TextureData>(Path);
	//Retrieve the PageName (BinaryReader::ReadNullString)
	//Construct the full path to the page texture file
	//	>> page texture should be stored next to the .fnt file, pageName contains the name of the texture file
	//	>> full texture path = asset parent_path of .fnt file (see loadInfo.assetFullPath > get parent_path) + pageName (filesystem::path::append)
	//	>> Load the texture (ContentManager::Load<TextureData>) & Store [fontDesc.pTexture]

	//**********
	// BLOCK 3 *
	//**********
	//Retrieve the blockId and blockSize
	//Retrieve Character Count (see documentation)
	int blockId3 = static_cast<int>(pReader->Read<char>());
	blockId3;
	auto blockSize3 = pReader->Read<int>();
	blockSize3;
	int constCharsSize = 20;
	int size = blockSize3 / constCharsSize;
	for (int i = 0; size > i; ++i)
	{
		wchar_t  charId = static_cast<wchar_t>(pReader->Read<int>());
		FontMetric charInstance;
		charInstance.character = charId;
		float xPos = static_cast<float>(pReader->Read<short>());
		float yPos = static_cast<float>(pReader->Read<short>());
		charInstance.width = (pReader->Read<unsigned short>());
		charInstance.height = (pReader->Read<unsigned short>());
		charInstance.offsetX = (pReader->Read<short>());
		charInstance.offsetY = (pReader->Read<short>());
		charInstance.advanceX = (pReader->Read<short>());
		charInstance.page = (pReader->Read<unsigned char>());
		int channel = (int)(pReader->Read<char>());
		switch (channel)
		{
		case 1: charInstance.channel = 2; break;
		case 2: charInstance.channel = 1; break;
		case 4: charInstance.channel = 0; break;
		case 8: charInstance.channel = 3; break;
		}
		charInstance.texCoord = XMFLOAT2(xPos / (float)fontDesc.textureWidth, (float)yPos / (float)fontDesc.textureHeight); //calculate texcoord
		fontDesc.metrics.insert(std::pair<wchar_t, FontMetric>(charId, charInstance));
		//fontDesc.metrics.insert(i, std::pair<wchar_t, FontMetric>(charId, charInstance));

	}

	//Create loop for Character Count, and:
	//> Retrieve CharacterId (store Local) and cast to a 'wchar_t'
	//> Create instance of FontMetric (struct)
	//	> Set Character (CharacterId) [FontMetric::character]
	//	> Retrieve Xposition (store Local)
	//	> Retrieve Yposition (store Local)
	//	> Retrieve & Set Width [FontMetric::width]
	//	> Retrieve & Set Height [FontMetric::height]
	//	> Retrieve & Set OffsetX [FontMetric::offsetX]
	//	> Retrieve & Set OffsetY [FontMetric::offsetY]
	//	> Retrieve & Set AdvanceX [FontMetric::advanceX]
	//	> Retrieve & Set Page [FontMetric::page]
	//	> Retrieve Channel (BITFIELD!!!) 
	//		> See documentation for BitField meaning [FontMetrix::channel]
	//	> Calculate Texture Coordinates using Xposition, Yposition, fontDesc.TextureWidth & fontDesc.TextureHeight [FontMetric::texCoord]
	//
	//> Insert new FontMetric to the metrics [font.metrics] map
	//	> key = (wchar_t) charId
	//	> value = new FontMetric
	//(loop restarts till all metrics are parsed)

	//Done!
	delete pReader;
	return new SpriteFont(fontDesc);
}

void SpriteFontLoader::Destroy(SpriteFont* objToDestroy)
{
	SafeDelete(objToDestroy);
}
