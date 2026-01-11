#include "QPCombatComponent.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "PJ_Quiet_Protocol/Weapons/WeaponBase.h"

UQPCombatComponent::UQPCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UQPCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner()); //소유한 캐릭터 가져오기
}

bool UQPCombatComponent::EquipWeapon(AWeaponBase* NewWeapon, bool bUnequipCurrent)
{
	if (!OwnerCharacter || !NewWeapon) return false; //소유한 캐릭터나 새 무기가 유효하지 않으면 false 반환
	if (EquippedWeapon == NewWeapon) return true; //이미 장착된 무기라면 true 반환
	if (EquippedWeapon && bUnequipCurrent) { //현재 무기가 있고 해제 플래그가 true라면
		UnEquipWeapon(true); //현재 무기 해제
	}
	EquippedWeapon = NewWeapon; //새 무기 장착
	//소유자/충돌 기본 처리 (**1 나중에 확장)
	EquippedWeapon->SetOwner(OwnerCharacter);//소유자 설정
	EquippedWeapon->SetInstigator(Cast<APawn>(OwnerCharacter)); //인스티게이터 설정
	EquippedWeapon->SetActorEnableCollision(false); //충돌 비활성화
	EquippedWeapon->OnEquipped(OwnerCharacter); //무기 장착 처리 호출
	if (!AttachWeaponToCharacter(EquippedWeapon)) //캐릭터에 무기 부착 실패 시
	{
		EquippedWeapon->OnUnequipped(true); //무기 해제 처리 호출
		EquippedWeapon = nullptr; //장착 실패 시 무기 초기화
		SetWeaponType(EQPWeaponType::EWT_None); //무기 타입 없음으로 설정
		return false; //false 반환
	}
	SetWeaponType(NewWeapon->GetWeaponType()); //무기 타입 설정 //**2 WeaponBase에서 GetWeaponType() 구현 필요
	return true; //성공적으로 장착했으므로 true 반환
}

bool UQPCombatComponent::UnEquipWeapon(bool bDropToWorld)
{
	if (!OwnerCharacter || !EquippedWeapon) {
		SetWeaponType(EQPWeaponType::EWT_None); //무기 타입 없음으로 설정
		return false; //소유한 캐릭터나 장착된 무기가 없으면 false 반환
	}
	StopAttack(); //공격 중지
	//캐릭터 분리
	EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform); //월드 트랜스폼 유지하며 분리
	EquippedWeapon->OnUnequipped(bDropToWorld); //무기 해제 처리 호출
	EquippedWeapon = nullptr; //장착된 무기 초기화

	SetWeaponType(EQPWeaponType::EWT_None); //무기 타입 없음으로 설정
	return true; //성공적으로 해제했으므로 true 반환
}
void UQPCombatComponent::StartAttack()
{
	if (!EquippedWeapon) {
		SetIsAttacking(false); //공격 상태 false로 설정
		return; //장착된 무기가 없으면 반환
	}
	SetIsAttacking(true); //공격 상태 true로 설정
	EquippedWeapon->StartFire(); //무기 발사 시작
}
void UQPCombatComponent::StopAttack()
{
	if (EquippedWeapon) {
		EquippedWeapon->StopAttack(); //무기 공격 중지
	}
	SetIsAttacking(false); //공격 상태 false로 설정
}
void UQPCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UQPCombatComponent::AttachWeaponToCharacter(AWeaponBase* Weapon)
{
	if (!OwnerCharacter || !Weapon) return false; //소유한 캐릭터나 무기가 유효하지 않으면 false 반환
	USkeletalMeshComponent* MeshComponent = OwnerCharacter->GetMesh(); //캐릭터의 스켈레탈 메쉬 컴포넌트 가져오기
	if (!MeshComponent) return false; //메쉬 컴포넌트가 유효하지 않으면 false 반환
	Weapon->AttachToComponent(MeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, EquipSocketName); //무기를 캐릭터 메쉬에 부착
	return true; //성공적으로 부착했으므로 true 반환
}

void UQPCombatComponent::SetWeaponType(EQPWeaponType NewType)
{
	if (EquippedWeaponType == NewType) return; //이미 같은 타입이면 반환
	EquippedWeaponType = NewType; //무기 타입 설정
	OnWeaponTypeChanged.Broadcast(EquippedWeaponType); //무기 타입 변경 델리게이트 브로드캐스트
}
void UQPCombatComponent::SetIsAttacking(bool bNewIsAttacking)
{
	if (bIsAttacking == bNewIsAttacking) return; //이미 같은 상태이면 반환
	bIsAttacking = bNewIsAttacking; //공격 상태 설정
	OnAttackStateChanged.Broadcast(bIsAttacking); //공격 상태 변경 델리게이트 브로드캐스트
}

