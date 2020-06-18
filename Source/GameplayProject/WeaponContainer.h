// To store weapon names and fill the TMap

#pragma once

#include "Weapon.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WeaponContainer.generated.h"

UCLASS(Blueprintable, BlueprintType)
class GAMEPLAYPROJECT_API UWeaponContainer : public UObject
{
	GENERATED_BODY()
public:

	UWeaponContainer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	TMap<FString, TSubclassOf<AWeapon>> WeaponMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	TSubclassOf<AWeapon> Weapon_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	TSubclassOf<AWeapon> Weapon_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	TSubclassOf<AWeapon> Weapon_3;
};
