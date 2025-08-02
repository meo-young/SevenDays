#pragma once

#define ECC_INTERACTION		ECC_GameTraceChannel1

UENUM(BlueprintType)
enum class EMissionType : uint8
{
	Missing = 0,
	New = 1,
	Horror = 2
};

UENUM(BlueprintType)
enum class ESFX : uint8
{
	Camera_Flash = 0,
	Camera_Shutter = 1,
	FootStep = 2
};

UENUM(BlueprintType)
enum class EBGM : uint8
{
	Title = 0
};