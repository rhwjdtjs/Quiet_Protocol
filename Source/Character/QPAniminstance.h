#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PJ_Quiet_Protocol/Commons/QPCombatTypes.h"
#include "QPAniminstance.generated.h"

/**
 * 
 */
UCLASS()
class PJ_QUIET_PROTOCOL_API UQPAniminstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override; // 애니메이션 인스턴스 초기화
	virtual void NativeUpdateAnimation(float DeltaSeconds) override; // 애니메이션 인스턴스 업데이트

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float Speed = 0.f; // 이동 속도
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsInAir = false; // 공중에 있는지 여부
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsAccelerating = false; // 가속 중인지 여부
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsCrouched = false; //앉아있는지 여부
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsSprinting = false; // 달리고 있는지 여부
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	EQPWeaponType WeaponType = EQPWeaponType::EWT_None; // 장착된 무기 타입
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsAttacking = false; // 공격 중인지 여부
private:
	TWeakObjectPtr <class AQPCharacter> CachedCharacter; // 캐릭터에 대한 약한 포인터
};
