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
	AGunWeapon(); //생성자

	virtual void StartFire_Implementation() override; //발사 시작 함수 재정의
	virtual void StopAttack_Implementation() override; //공격 중지 함수 재정의

protected:
	void FireOnce(); //한 번 발사 함수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Gun", meta = (ClampMin = "0.0"))
	float Range = 15000.f; //사거리
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon|Gun", meta = (ClampMin="0.0"))
	float FireRate = 0.1f; //발사 속도(초당 발사 횟수)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Gun")
	bool bAutomatic = true; //자동 발사 여부
private:
	FTimerHandle TimerHandle_AutoFire; //자동 발사 타이머 핸들
};
