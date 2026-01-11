癤#pragma once

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
	FORCEINLINE EQPWeaponType GetWeaponType() const { return WeaponType; } //臾닿린  諛 ⑥
	UFUNCTION(BlueprintPure, Category="Weapon")
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; } //臾닿린 硫 諛 ⑥

	UFUNCTION(BlueprintCallable, Category="Weapon")
	virtual void OnEquipped(ACharacter* NewOwner); //臾닿린 μ갑  몄 ⑥
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void OnUnequipped(bool bDropToWorld); //臾닿린 댁  몄 ⑥

	UFUNCTION(BlueprintNativeEvent, Category = "Weapon")
	void StartFire(); //諛  ⑥
	virtual void StartFire_Implementation(); //湲곕낯 援ы
	UFUNCTION(BlueprintNativeEvent, Category = "Weapon")
	void StopAttack(); //怨듦꺽 以吏 ⑥
	virtual void StopAttack_Implementation(); //湲곕낯 援ы
protected:
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh; //臾닿린 硫 而댄щ // TObjectPtr -  null 泥由
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	EQPWeaponType WeaponType = EQPWeaponType::EWT_None; //臾닿린 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Damage", meta = (ClampMin= "0.0"))
	float BaseDamage = 10.f; //湲곕낯 곕�吏
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Damage")
	TSubclassOf<UDamageType> DamageTypeClass; //곕�吏  대

public:	
	virtual void Tick(float DeltaTime) override;

};
