#pragma once

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
	AMeleeWeapon(); //생성자

	virtual void StartFire_Implementation() override; //공격 시작 함수 재정의

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Melee", meta = (ClampMin = "0.0"))
	float SwingRange = 180.f; //스윙 범위
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Melee", meta = (ClampMin = "0.0"))
	float SwingRadius = 40.f; //스윙 반경

};
