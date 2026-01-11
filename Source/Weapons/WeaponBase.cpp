#include "WeaponBase.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh")); //臾닿린 硫 而댄щ 
	SetRootComponent(WeaponMesh); //RootComponent濡 ㅼ
	//쎌 媛ν寃
	WeaponMesh->SetSimulatePhysics(true); //臾쇰━ 裕щ댁 깊
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); //異⑸ 깊
	WeaponMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic); //異⑸ 梨 ㅼ
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); //紐⑤ 梨  異⑸  臾댁
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap); //Pawn 梨  寃뱀묠  ㅼ
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block); //Visibility 梨  李⑤  ㅼ
	WeaponMesh->SetGenerateOverlapEvents(true); //寃뱀묠 대깽  깊
}

void AWeaponBase::OnEquipped(ACharacter* NewOwner)
{
	SetOwner(Cast<APawn>(NewOwner)); // ㅼ
	SetInstigator(Cast<APawn>(NewOwner)); //몄ㅽ곌댄 ㅼ
	SetActorEnableCollision(false); //≫ 異⑸ 鍮깊

	if (WeaponMesh) {
		WeaponMesh->SetSimulatePhysics(false); //臾쇰━ 裕щ댁 鍮깊
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); //異⑸ 鍮깊
		WeaponMesh->SetGenerateOverlapEvents(false); //寃뱀묠 대깽 鍮깊
	}

}

void AWeaponBase::OnUnequipped(bool bDropToWorld)
{
	SetOwner(nullptr); // 댁
	SetInstigator(nullptr); //몄ㅽ곌댄 댁
	if (!WeaponMesh) return; //臾닿린 硫ш ⑦吏 쇰㈃ 諛
	SetActorEnableCollision(bDropToWorld); //≫ 異⑸ ㅼ
	if (bDropToWorld) {
		WeaponMesh->SetSimulatePhysics(true); //臾쇰━ 裕щ댁 깊
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); //異⑸ 깊
		WeaponMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic); //異⑸ 梨 ㅼ
		WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); //紐⑤ 梨  異⑸  臾댁
		WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap); //Pawn 梨  寃뱀묠  ㅼ
		WeaponMesh->SetGenerateOverlapEvents(true); //寃뱀묠 대깽  깊
	}
	else {
		WeaponMesh->SetSimulatePhysics(false); //臾쇰━ 裕щ댁 鍮깊
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); //異⑸ 鍮깊
		WeaponMesh->SetGenerateOverlapEvents(false); //寃뱀묠 대깽 鍮깊
	}
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AWeaponBase::StartFire_Implementation()
{
	// 대  or 釉猷⑦由고
}

void AWeaponBase::StopAttack_Implementation()
{
	// 대  or 釉猷⑦由고
}
