癤#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "MeleeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class PJ_QUIET_PROTOCOL_API AMeleeWeapon : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	AMeleeWeapon(); //깆

	virtual void StartFire_Implementation() override; //怨듦꺽  ⑥ ъ

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Melee", meta = (ClampMin = "0.0"))
	float SwingRange = 180.f; //ㅼ 踰
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Melee", meta = (ClampMin = "0.0"))
	float SwingRadius = 40.f; //ㅼ 諛寃

};
