#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PJ_Quiet_Protocol/Commons/QPCombatTypes.h"
#include "WeaponBase.generated.h"

class USkeletalMeshComponent;
class UDamageType;
class ACharacter;

UCLASS()
class PJ_QUIET_PROTOCOL_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponBase();
	UFUNCTION(BlueprintPure, Category="Weapon")
	FORCEINLINE EQPWeaponType GetWeaponType() const { return WeaponType; } //무기 타입 반환 함수
	UFUNCTION(BlueprintPure, Category="Weapon")
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; } //무기 메쉬 반환 함수

	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void OnEquipped(ACharacter* NewOwner); //무기 장착 시 호출 함수
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void OnUnequipped(bool bDropToWorld); //무기 해제 시 호출 함수

	UFUNCTION(BlueprintNativeEvent, Category = "Weapon")
	void StartFire(); //발사 시작 함수
	virtual void StartFire_Implementation(); //기본 구현
	UFUNCTION(BlueprintNativeEvent, Category = "Weapon")
	void StopAttack(); //공격 중지 함수
	virtual void StopAttack_Implementation(); //기본 구현
protected:
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh; //무기 메쉬 컴포넌트 // TObjectPtr - 자동 null 처리
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	EQPWeaponType WeaponType = EQPWeaponType::EWT_None; //무기 타입
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Damage", meta = (ClampMin= "0.0"))
	float BaseDamage = 10.f; //기본 데미지
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Damage")
	TSubclassOf<UDamageType> DamageTypeClass; //데미지 타입 클래스

public:	
	virtual void Tick(float DeltaTime) override;

};
