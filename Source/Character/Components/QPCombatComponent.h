癤#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PJ_Quiet_Protocol/Commons/QPCombatTypes.h"
#include "QPCombatComponent.generated.h"

class AWeaponBase; //�諛 
class ACharacter; //�諛 

/**
臾닿린  蹂寃쎈  釉濡罹ㅽ몃 몃━寃댄
몄 �: SetWeaponType() ⑥ EquippedWeaponType ㅼ濡 蹂寃쎈 
二쇱 ⑸:
UI 곗댄 (臾닿린 댁,   )
硫댁 釉猷⑦由고몄 臾닿린蹂 硫댁 �
ъ대/댄 ㅽ 臾닿린 蹂寃 由
@param NewWeaponType 濡 μ갑 臾닿린  (EQPWeaponType)
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponTypeChanged, 
	EQPWeaponType, NewWeaponType); //臾닿린 μ갑 몃━寃댄 
/**
怨듦꺽 媛 蹂寃쎈  釉濡罹ㅽ몃 몃━寃댄
몄 �: 罹由�곌 怨듦꺽 嫄곕 硫異 
二쇱 ⑸:
UI 곗댄 (怨듦꺽 紐⑥  )
硫댁 釉猷⑦由고몄 怨듦꺽 硫댁 �
ъ대/댄 ㅽ 怨듦꺽  蹂寃 由
@param bNewIsAttacking 濡 怨듦꺽  (true: 怨듦꺽 以, false: 鍮怨듦꺽 以)
 */ 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttackStateChanged, bool, bNewIsAttacking);//怨듦꺽  蹂寃 몃━寃댄 

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PJ_QUIET_PROTOCOL_API UQPCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UQPCombatComponent();
	//臾닿린 湲곕
	UFUNCTION(BlueprintCallable, Category="Combat|Weapon")
	bool EquipWeapon(AWeaponBase* NewWeapon, bool bUnequipCurrent = true); //臾닿린 μ갑 ⑥
	UFUNCTION(BlueprintCallable, Category="Combat|Weapon")
	bool UnEquipWeapon(bool bDropToWorld = true); //臾닿린 댁 ⑥
	UFUNCTION(BlueprintPure, Category="Combat|Weapon")
	FORCEINLINE AWeaponBase* GetEquippedWeapon() const { return EquippedWeapon; } //μ갑 臾닿린 諛 ⑥
	UFUNCTION(BlueprintPure, Category = "Combat|Weapon") 
	FORCEINLINE EQPWeaponType GetEquippedWeaponType() const { return EquippedWeaponType; } //μ갑 臾닿린  諛 ⑥
	UFUNCTION(BlueprintPure, Category="Combat|Weapon")
	FORCEINLINE bool HasWeapon() const { return EquippedWeapon != nullptr; } //臾닿린 μ갑 щ 諛 ⑥

	//怨듦꺽 湲곕
	UFUNCTION(BlueprintCallable, Category = "Combat|Attack")
	void StartAttack(); //怨듦꺽  ⑥
	UFUNCTION(BlueprintCallable, Category = "Combat|Attack")
	void StopAttack(); //怨듦꺽 以吏 ⑥
	UFUNCTION(BlueprintPure, Category = "Combat|Attack")
	FORCEINLINE bool IsAttacking() const { return bIsAttacking; } //怨듦꺽 以몄 щ 諛 ⑥
	UPROPERTY(BlueprintAssignable, Category="Combat")
	FOnAttackStateChanged OnAttackStateChanged;
	UPROPERTY(BlueprintAssignable, Category="Combat")
	FOnWeaponTypeChanged OnWeaponTypeChanged;
protected:
	virtual void BeginPlay() override;
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	bool AttachWeaponToCharacter(AWeaponBase* Weapon); //罹由�곗 臾닿린 遺李 ⑥
	void SetWeaponType(EQPWeaponType NewType); //臾닿린  ㅼ ⑥
	void SetIsAttacking(bool bNewIsAttacking); //怨듦꺽  ㅼ ⑥
	UPROPERTY()
	ACharacter* OwnerCharacter=nullptr; // 罹由�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat|Weapon", meta = (AllowPrivateAccess = "true"))
	AWeaponBase* EquippedWeapon=nullptr; //μ갑 臾닿린
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Weapon", meta = (AllowPrivateAccess = "true"))
	EQPWeaponType EquippedWeaponType = EQPWeaponType::EWT_None; //μ갑 臾닿린 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Weapon", meta = (AllowPrivateAccess = "true"))
	FName EquipSocketName = TEXT("WeaponSocket"); //臾닿린 μ갑 耳 대
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Attack", meta = (AllowPrivateAccess = "true"))
	bool bIsAttacking = false; //怨듦꺽 以몄 щ
};
