#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PJ_Quiet_Protocol/Commons/QPCombatTypes.h"
#include "QPCombatComponent.generated.h"

class AWeaponBase; //전방 선언
class ACharacter; //전방 선언

/**
무기 타입이 변경될 때 브로드캐스트되는 델리게이트
호출 시점: SetWeaponType() 함수에서 EquippedWeaponType이 실제로 변경될 때
주요 용도:
UI 업데이트 (무기 아이콘, 탄약 표시 등)
애니메이션 블루프린트에서 무기별 애니메이션 전환
사운드/이펙트 시스템에 무기 변경 알림
@param NewWeaponType 새로 장착된 무기의 타입 (EQPWeaponType)
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponTypeChanged, 
	EQPWeaponType, NewWeaponType); //무기 장착 델리게이트 선언
/**
공격 상태가 변경될 때 브로드캐스트되는 델리게이트
호출 시점: 캐릭터가 공격을 시작하거나 멈출 때
주요 용도:
UI 업데이트 (공격 모션 표시 등)
애니메이션 블루프린트에서 공격 애니메이션 전환
사운드/이펙트 시스템에 공격 상태 변경 알림
@param bNewIsAttacking 새로운 공격 상태 (true: 공격 중, false: 비공격 중)
 */ 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttackStateChanged, bool, bNewIsAttacking);//공격 상태 변경 델리게이트 선언

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PJ_QUIET_PROTOCOL_API UQPCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UQPCombatComponent();
	//무기 기능
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

	//공격 기능
	UFUNCTION(BlueprintCallable, Category = "Combat|Attack")
	void StartAttack(); //공격 시작 함수
	UFUNCTION(BlueprintCallable, Category = "Combat|Attack")
	void StopAttack(); //공격 중지 함수
	UFUNCTION(BlueprintPure, Category = "Combat|Attack")
	FORCEINLINE bool IsAttacking() const { return bIsAttacking; } //공격 중인지 여부 반환 함수
	UPROPERTY(BlueprintAssignable, Category="Combat")
	FOnAttackStateChanged OnAttackStateChanged;
	UPROPERTY(BlueprintAssignable, Category="Combat")
	FOnWeaponTypeChanged OnWeaponTypeChanged;
protected:
	virtual void BeginPlay() override;
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	bool AttachWeaponToCharacter(AWeaponBase* Weapon); //캐릭터에 무기 부착 함수
	void SetWeaponType(EQPWeaponType NewType); //무기 타입 설정 함수
	void SetIsAttacking(bool bNewIsAttacking); //공격 상태 설정 함수
	UPROPERTY()
	ACharacter* OwnerCharacter=nullptr; //소유한 캐릭터
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat|Weapon", meta = (AllowPrivateAccess = "true"))
	AWeaponBase* EquippedWeapon=nullptr; //장착된 무기
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Weapon", meta = (AllowPrivateAccess = "true"))
	EQPWeaponType EquippedWeaponType = EQPWeaponType::EWT_None; //장착된 무기 타입
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Weapon", meta = (AllowPrivateAccess = "true"))
	FName EquipSocketName = TEXT("WeaponSocket"); //무기 장착 소켓 이름
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Attack", meta = (AllowPrivateAccess = "true"))
	bool bIsAttacking = false; //공격 중인지 여부
};
