// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "TimerManager.h"
#include "GunWeapon.generated.h"

/**
 * 
 */
UCLASS()
class PJ_QUIET_PROTOCOL_API AGunWeapon : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	AGunWeapon(); //깆

	virtual void StartFire_Implementation() override; //諛  ⑥ ъ
	virtual void StopAttack_Implementation() override; //怨듦꺽 以吏 ⑥ ъ

protected:
	void FireOnce(); // 踰 諛 ⑥
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Gun", meta = (ClampMin = "0.0"))
	float Range = 15000.f; //ш굅由
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon|Gun", meta = (ClampMin="0.0"))
	float FireRate = 0.1f; //諛 (珥 諛 )
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Gun")
	bool bAutomatic = true; // 諛 щ
private:
	FTimerHandle TimerHandle_AutoFire; // 諛 대㉧ 몃
};
