#include "WeaponBase.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh")); //무기 메쉬 컴포넌트 생성
	SetRootComponent(WeaponMesh); //RootComponent로 설정
	//픽업 가능하게
	WeaponMesh->SetSimulatePhysics(true); //물리 시뮬레이션 활성화
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); //충돌 활성화
	WeaponMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic); //충돌 채널 설정
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); //모든 채널에 대한 충돌 응답 무시
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap); //Pawn 채널에 대해 겹침 응답 설정
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block); //Visibility 채널에 대해 차단 응답 설정
	WeaponMesh->SetGenerateOverlapEvents(true); //겹침 이벤트 생성 활성화
}

void AWeaponBase::OnEquipped(ACharacter* NewOwner)
{
	SetOwner(Cast<APawn>(NewOwner)); //소유자 설정
	SetInstigator(Cast<APawn>(NewOwner)); //인스티게이터 설정
	SetActorEnableCollision(false); //액터 충돌 비활성화

	if (WeaponMesh) {
		WeaponMesh->SetSimulatePhysics(false); //물리 시뮬레이션 비활성화
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); //충돌 비활성화
		WeaponMesh->SetGenerateOverlapEvents(false); //겹침 이벤트 비활성화
	}

}

void AWeaponBase::OnUnequipped(bool bDropToWorld)
{
	SetOwner(nullptr); //소유자 해제
	SetInstigator(nullptr); //인스티게이터 해제
	if (!WeaponMesh) return; //무기 메쉬가 유효하지 않으면 반환
	SetActorEnableCollision(bDropToWorld); //액터 충돌 설정
	if (bDropToWorld) {
		WeaponMesh->SetSimulatePhysics(true); //물리 시뮬레이션 활성화
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); //충돌 활성화
		WeaponMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic); //충돌 채널 설정
		WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); //모든 채널에 대한 충돌 응답 무시
		WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap); //Pawn 채널에 대해 겹침 응답 설정
		WeaponMesh->SetGenerateOverlapEvents(true); //겹침 이벤트 생성 활성화
	}
	else {
		WeaponMesh->SetSimulatePhysics(false); //물리 시뮬레이션 비활성화
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); //충돌 비활성화
		WeaponMesh->SetGenerateOverlapEvents(false); //겹침 이벤트 비활성화
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
	//자식 클래스 작성 or 블루프린트
}

void AWeaponBase::StopAttack_Implementation()
{
	//자식 클래스 작성 or 블루프린트
}
