#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PJ_Quiet_Protocol/Commons/QPCombatTypes.h"
#include "QPCombatComponent.generated.h"

class AWeaponBase; //전방 선언
class ACharacter; //전방 선언

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponTypeChanged, 
	EQPWeaponType, NewWeaponType); //무기 장착 델리게이트 선언

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PJ_QUIET_PROTOCOL_API UQPCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UQPCombatComponent();
	UFUNCTION(BlueprintCallable, Category="Combat|Weapon")
	bool EquipWeapon(AWeaponBase* NewWeapon, bool bUnequipCurrent = true); //무기 장착 함수
	UFUNCTION(BlueprintCallable, Category="Combat|Weapon")
	bool UnEquipWeapon(bool bDropToWorld = true); //무기 해제 함수
	UFUNCTION(BlueprintPure, Category="Combat|Weapon")
	FORCEINLINE AWeaponBase* GetEquippedWeapon() const { return EquippedWeapon; } //장착된 무기 반환 함수
	UFUNCTION(BlueprintPure, Category = "Combat|Weapon") 
	FORCEINLINE EQPWeaponType GetEquippedWeaponType() const { return EquippedWeaponType; } //장착된 무기 타입 반환 함수
	UFUNCTION(BlueprintPure, Category="Combat|Weapon")
	FORCEINLINE bool HasWeapon() const { return EquippedWeapon != nullptr; } //무기 장착 여부 반환 함수

	UPROPERTY(BlueprintAssignable, Category="Combat")
	FOnWeaponTypeChanged OnWeaponTypeChanged;
protected:
	virtual void BeginPlay() override;
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	bool AttachWeaponToCharacter(AWeaponBase* Weapon); //캐릭터에 무기 부착 함수
	void SetWeaponType(EQPWeaponType NewType); //무기 타입 설정 함수
	UPROPERTY()
	ACharacter* OwnerCharacter; //소유한 캐릭터
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat|Weapon", meta = (AllowPrivateAccess = "true"))
	AWeaponBase* EquippedWeapon=nullptr; //장착된 무기
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Weapon", meta = (AllowPrivateAccess = "true"))
	EQPWeaponType EquippedWeaponType = EQPWeaponType::EWT_None; //장착된 무기 타입
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Weapon", meta = (AllowPrivateAccess = "true"))
	FName EquipSocketName = TEXT("WeaponSocket"); //무기 장착 소켓 이름
};
