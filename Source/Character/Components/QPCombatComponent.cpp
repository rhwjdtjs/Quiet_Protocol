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
	OwnerCharacter = Cast<ACharacter>(GetOwner()); // 罹由� 媛�몄ㅺ린
}

bool UQPCombatComponent::EquipWeapon(AWeaponBase* NewWeapon, bool bUnequipCurrent)
{
	if (!OwnerCharacter || !NewWeapon) return false; // 罹由�곕  臾닿린媛 ⑦吏 쇰㈃ false 諛
	if (EquippedWeapon == NewWeapon) return true; //대� μ갑 臾닿린쇰㈃ true 諛
	if (EquippedWeapon && bUnequipCurrent) { // 臾닿린媛 怨 댁 洹멸 true쇰㈃
		UnEquipWeapon(true); // 臾닿린 댁
	}
	EquippedWeapon = NewWeapon; // 臾닿린 μ갑
	///異⑸ 湲곕낯 泥由 (**1 以 )
	EquippedWeapon->SetOwner(OwnerCharacter);// ㅼ
	EquippedWeapon->SetInstigator(Cast<APawn>(OwnerCharacter)); //몄ㅽ곌댄 ㅼ
	EquippedWeapon->SetActorEnableCollision(false); //異⑸ 鍮깊
	EquippedWeapon->OnEquipped(OwnerCharacter); //臾닿린 μ갑 泥由 몄
	if (!AttachWeaponToCharacter(EquippedWeapon)) //罹由�곗 臾닿린 遺李 ㅽ 
	{
		EquippedWeapon->OnUnequipped(true); //臾닿린 댁 泥由 몄
		EquippedWeapon = nullptr; //μ갑 ㅽ  臾닿린 珥湲고
		SetWeaponType(EQPWeaponType::EWT_None); //臾닿린  쇰 ㅼ
		return false; //false 諛
	}
	SetWeaponType(NewWeapon->GetWeaponType()); //臾닿린  ㅼ //**2 WeaponBase GetWeaponType() 援ы 
	return true; //깃났�쇰 μ갑쇰濡 true 諛
}

bool UQPCombatComponent::UnEquipWeapon(bool bDropToWorld)
{
	if (!OwnerCharacter || !EquippedWeapon) {
		SetWeaponType(EQPWeaponType::EWT_None); //臾닿린  쇰 ㅼ
		return false; // 罹由�곕 μ갑 臾닿린媛 쇰㈃ false 諛
	}
	StopAttack(); //怨듦꺽 以吏
	//罹由� 遺由
	EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform); // 몃ㅽ 吏硫 遺由
	EquippedWeapon->OnUnequipped(bDropToWorld); //臾닿린 댁 泥由 몄
	EquippedWeapon = nullptr; //μ갑 臾닿린 珥湲고

	SetWeaponType(EQPWeaponType::EWT_None); //臾닿린  쇰 ㅼ
	return true; //깃났�쇰 댁쇰濡 true 諛
}
void UQPCombatComponent::StartAttack()
{
	if (!EquippedWeapon) {
		SetIsAttacking(false); //怨듦꺽  false濡 ㅼ
		return; //μ갑 臾닿린媛 쇰㈃ 諛
	}
	SetIsAttacking(true); //怨듦꺽  true濡 ㅼ
	EquippedWeapon->StartFire(); //臾닿린 諛 
}
void UQPCombatComponent::StopAttack()
{
	if (EquippedWeapon) {
		EquippedWeapon->StopAttack(); //臾닿린 怨듦꺽 以吏
	}
	SetIsAttacking(false); //怨듦꺽  false濡 ㅼ
}
void UQPCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UQPCombatComponent::AttachWeaponToCharacter(AWeaponBase* Weapon)
{
	if (!OwnerCharacter || !Weapon) return false; // 罹由�곕 臾닿린媛 ⑦吏 쇰㈃ false 諛
	USkeletalMeshComponent* MeshComponent = OwnerCharacter->GetMesh(); //罹由�곗 ㅼ� 硫 而댄щ 媛�몄ㅺ린
	if (!MeshComponent) return false; //硫 而댄щ멸 ⑦吏 쇰㈃ false 諛
	Weapon->AttachToComponent(MeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, EquipSocketName); //臾닿린瑜 罹由� 硫ъ 遺李
	return true; //깃났�쇰 遺李⑺쇰濡 true 諛
}

void UQPCombatComponent::SetWeaponType(EQPWeaponType NewType)
{
	if (EquippedWeaponType == NewType) return; //대� 媛 대㈃ 諛
	EquippedWeaponType = NewType; //臾닿린  ㅼ
	OnWeaponTypeChanged.Broadcast(EquippedWeaponType); //臾닿린  蹂寃 몃━寃댄 釉濡罹ㅽ
}
void UQPCombatComponent::SetIsAttacking(bool bNewIsAttacking)
{
	if (bIsAttacking == bNewIsAttacking) return; //대� 媛 대㈃ 諛
	bIsAttacking = bNewIsAttacking; //怨듦꺽  ㅼ
	OnAttackStateChanged.Broadcast(bIsAttacking); //怨듦꺽  蹂寃 몃━寃댄 釉濡罹ㅽ
}

